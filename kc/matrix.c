#include "WProgram.h"
#include "matrix.h"
#include "kc_params.h"


// Debounce Array
KeyState Matrix_scanArray[ScanKeys];

// Ghost Arrays
#ifdef GHOSTING_MATRIX
KeyGhost Matrix_ghostArray[ScanKeys];

uint8_t col_use[NumCols], row_use[NumRows];  // used count
uint8_t col_ghost[NumCols], row_ghost[NumRows];  // marked as having ghost if 1
uint8_t col_ghost_old[NumCols], row_ghost_old[NumRows];  // old ghost state
#endif

// Matrix State Table Debug Counter - If non-zero display state table after every matrix scan
uint16_t matrixDebugStateCounter = 0;

// Matrix Scan Counters
uint16_t matrixMaxScans  = 0;
uint16_t matrixCurScans  = 0;
uint16_t matrixPrevScans = 0;

int ghost_cols = 0, ghost_rows = 0;
int cnt_press = 0, cnt_rel = 0, cnt_hold = 0;
int16_t cnt_press1min = 0;

// System Timer used for delaying debounce decisions
extern volatile uint32_t systick_millis_count;

//extern struct KC_Params par;  // params


// ----- Functions -----

// Pin action (Strobe, Sense, Strobe Setup, Sense Setup)
// NOTE: This function is highly dependent upon the organization of the register map
//       Only guaranteed to work with Freescale MK20 series uCs
uint8_t Matrix_pin( GPIO_Pin gpio, Type type )
{
	// Register width is defined as size of a pointer
	unsigned int gpio_offset = gpio.port * 0x40   / sizeof(unsigned int*);
	unsigned int port_offset = gpio.port * 0x1000 / sizeof(unsigned int*) + gpio.pin;

	// Assumes 0x40 between GPIO Port registers and 0x1000 between PORT pin registers
	// See Lib/mk20dx.h
	volatile unsigned int *GPIO_PDDR = (unsigned int*)(&GPIOA_PDDR) + gpio_offset;
	#ifndef GHOSTING_MATRIX
	volatile unsigned int *GPIO_PSOR = (unsigned int*)(&GPIOA_PSOR) + gpio_offset;
	#endif
	volatile unsigned int *GPIO_PCOR = (unsigned int*)(&GPIOA_PCOR) + gpio_offset;
	volatile unsigned int *GPIO_PDIR = (unsigned int*)(&GPIOA_PDIR) + gpio_offset;
	volatile unsigned int *PORT_PCR  = (unsigned int*)(&PORTA_PCR0) + port_offset;

	// Operation depends on Type
	switch ( type )
	{
	case Type_StrobeOn:
		#ifdef GHOSTING_MATRIX
		*GPIO_PCOR |= (1 << gpio.pin);
		*GPIO_PDDR |= (1 << gpio.pin);  // output, low
		#else
		*GPIO_PSOR |= (1 << gpio.pin);
		#endif
		break;

	case Type_StrobeOff:
		#ifdef GHOSTING_MATRIX
		// Ghosting martix needs to put not used (off) strobes in high impedance state
		*GPIO_PDDR &= ~(1 << gpio.pin);  // input, high Z state
		#endif
		*GPIO_PCOR |= (1 << gpio.pin);
		break;

	case Type_StrobeSetup:
		#ifdef GHOSTING_MATRIX
		*GPIO_PDDR &= ~(1 << gpio.pin);  // input, high Z state
		*GPIO_PCOR |= (1 << gpio.pin);
		#else
		// Set as output pin
		*GPIO_PDDR |= (1 << gpio.pin);
		#endif

		// Configure pin with slow slew, high drive strength and GPIO mux
		*PORT_PCR = PORT_PCR_SRE | PORT_PCR_DSE | PORT_PCR_MUX(1);

		// Enabling open-drain if specified
		switch ( Matrix_type )
		{
		case Config_Opendrain:
			*PORT_PCR |= PORT_PCR_ODE;
			break;

		// Do nothing otherwise
		default:
			break;
		}
		break;

	case Type_Sense:
		#ifdef GHOSTING_MATRIX  // inverted
		return *GPIO_PDIR & (1 << gpio.pin) ? 0 : 1;
		#else
		return *GPIO_PDIR & (1 << gpio.pin) ? 1 : 0;
		#endif

	case Type_SenseSetup:
		// Set as input pin
		*GPIO_PDDR &= ~(1 << gpio.pin);

		// Configure pin with passive filter and GPIO mux
		*PORT_PCR = PORT_PCR_PFE | PORT_PCR_MUX(1);

		// Pull resistor config
		switch ( Matrix_type )
		{
		case Config_Pullup:
			*PORT_PCR |= PORT_PCR_PE | PORT_PCR_PS;
			break;

		case Config_Pulldown:
			*PORT_PCR |= PORT_PCR_PE;
			break;

		// Do nothing otherwise
		default:
			break;
		}
		break;
	}

	return 0;
}


//  Setup  GPIO pins for matrix scanning
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
void Matrix_setup()
{
	// Setup Strobe Pins
	for ( uint8_t pin = 0; pin < NumCols; pin++ )
	{
		Matrix_pin( Matrix_cols[ pin ], Type_StrobeSetup );
		#ifdef GHOSTING_MATRIX
		col_use[pin] = 0;
		col_ghost[pin] = 0;
		col_ghost_old[pin] = 0;
		#endif
	}

	// Setup Sense Pins
	for ( uint8_t pin = 0; pin < NumRows; pin++ )
	{
		Matrix_pin( Matrix_rows[ pin ], Type_SenseSetup );
		#ifdef GHOSTING_MATRIX
		row_use[pin] = 0;
		row_ghost[pin] = 0;
		row_ghost_old[pin] = 0;
		#endif
	}

	// Clear out Debounce Array
	for (uint8_t i = 0; i < ScanKeys; ++i)
	{
		#define mk Matrix_scanArray[i]
		mk.prevState        = KeyState_Off;
		mk.curState         = KeyState_Off;
		mk.activeCount      = 0;
		mk.inactiveCount    = DebounceDivThreshold_define; // Start at 'off' steady state
		mk.prevDecisionTime = 0;

		mk.state = KeyState_Off;
		mk.autoRepeat = 0;
		mk.layerOn = 0;

		#ifdef GHOSTING_MATRIX
		#define mg Matrix_ghostArray[i]
		mg.prev  = KeyState_Off;
		mg.cur   = KeyState_Off;
		mg.saved = KeyState_Off;
		#endif
	}

	// Clear scan stats counters
	matrixMaxScans  = 0;
	matrixPrevScans = 0;
	ghost_cols = 0;
	ghost_rows = 0;
	cnt_press = cnt_hold = cnt_rel = 0;  // test
	cnt_press1min = 0;
}


//  Scan  the matrix for keypresses
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// NOTE: scanNum should be reset to 0 after a USB send (to reset all the counters)
void Matrix_scan( uint16_t scanNum )
{
#if ( DebounceThrottleDiv_define > 0 )
	// Scan-rate throttling
	// By scanning using a divider, the scan rate slowed down
	// DebounceThrottleDiv_define == 1 means -> /2 or half scan rate
	// This helps with bouncy switches on fast uCs
	if ( !( Matrix_divCounter++ & (1 << ( DebounceThrottleDiv_define - 1 )) ) )
		return;
#endif

	// Increment stats counters
	if ( scanNum > matrixMaxScans ) matrixMaxScans = scanNum;
	if ( scanNum == 0 )
	{
		matrixPrevScans = matrixCurScans;
		matrixCurScans = 0;
	}
	else
	{
		matrixCurScans++;
	}

	// Read systick for event scheduling
	uint8_t currentTime = (uint8_t)systick_millis_count;

	// For each strobe, scan each of the sense pins
	for ( uint8_t strobe = 0; strobe < NumCols; strobe++ )
	{

		uint32_t start = micros();
		if (par.strobe_delay > 0)
			while ((micros() - start) < par.strobe_delay);

		// Strobe Pin
		Matrix_pin( Matrix_cols[ strobe ], Type_StrobeOn );

		start = micros();
		if (par.strobe_delay > 0)
			while ((micros() - start) < par.strobe_delay);


		// Scan each of the sense pins
		for ( uint8_t sense = 0; sense < NumRows; sense++ )
		{
			// Key position
			uint8_t key = NumCols * sense + strobe;
			KeyState *state = &Matrix_scanArray[ key ];

			// If first scan, reset state
			if ( scanNum == 0 )
			{
				// Set previous state, and reset current state
				state->prevState = state->curState;
				state->curState  = KeyState_Invalid;
			}

			// Signal Detected
			// Increment count and right shift opposing count
			// This means there is a maximum of scan 13 cycles on a perfect off to on transition
			//  (coming from a steady state 0xFFFF off scans)
			// Somewhat longer with switch bounciness
			// The advantage of this is that the count is ongoing and never needs to be reset
			// State still needs to be kept track of to deal with what to send to the Macro module
			if ( Matrix_pin( Matrix_rows[ sense ], Type_Sense ) )
			{
				// Only update if not going to wrap around
				if ( state->activeCount < DebounceDivThreshold_define ) state->activeCount += 1;
				state->inactiveCount >>= 1;
			}
			// Signal Not Detected
			else
			{
				// Only update if not going to wrap around
				if ( state->inactiveCount < DebounceDivThreshold_define ) state->inactiveCount += 1;
				state->activeCount >>= 1;
			}

			// Check for state change if it hasn't been set
			// But only if enough time has passed since last state change
			// Only check if the minimum number of scans has been met
			//   the current state is invalid
			//   and either active or inactive count is over the debounce threshold
			if ( state->curState == KeyState_Invalid )
			{
				// Determine time since last decision
				uint8_t lastTransition = currentTime - state->prevDecisionTime;

				// Attempt state transition
				switch ( state->prevState )
				{
				case KeyState_Press:
				case KeyState_Hold:
					if ( state->activeCount > state->inactiveCount )
						state->curState = KeyState_Hold;
					else
					{
						// If not enough time has passed since Hold
						// Keep previous state
						if ( lastTransition < par.debounce )
						{
							//warn_print("FAST Release stopped");
							state->curState = state->prevState;
							continue;
						}
						state->curState = KeyState_Release;
					}
					break;

				case KeyState_Release:
				case KeyState_Off:
					if ( state->activeCount > state->inactiveCount )
					{
						// If not enough time has passed since Hold
						// Keep previous state
						if ( lastTransition < par.debounce )
						{
							//warn_print("FAST Press stopped");
							state->curState = state->prevState;
							continue;
						}
						state->curState = KeyState_Press;
					}else
						state->curState = KeyState_Off;
					break;

				case KeyState_Invalid:
				default:
					// Matrix scan bug
					break;
				}

				// Update decision time
				state->prevDecisionTime = currentTime;

				// Send keystate to macro module
				#ifndef GHOSTING_MATRIX
				//Macro_keyState( key, state->curState );

				if (state->curState == KeyState_Press)
					cnt_press++;
				if (state->curState == KeyState_Hold)
					cnt_hold++;
				if (state->curState == KeyState_Release)
					cnt_rel++;
				#endif

			}
		}

		// Unstrobe Pin
		Matrix_pin( Matrix_cols[ strobe ], Type_StrobeOff );
	}


	// Matrix ghosting check and elimination
	// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
#ifdef GHOSTING_MATRIX
	// strobe = column, sense = row

	// Count (rows) use for columns
	for ( uint8_t col = 0; col < NumCols; col++ )
	{
		uint8_t used = 0;
		for ( uint8_t row = 0; row < NumRows; row++ )
		{
			uint8_t key = NumCols * row + col;
			KeyState *state = &Matrix_scanArray[ key ];
			if ( keyOn(state->curState) )
				used++;
		}
		col_use[col] = used;
		col_ghost_old[col] = col_ghost[col];
		col_ghost[col] = 0;  // clear
	}

	// Count (columns) use for rows
	for ( uint8_t row = 0; row < NumRows; row++ )
	{
		uint8_t used = 0;
		for ( uint8_t col = 0; col < NumCols; col++ )
		{
			uint8_t key = NumCols * row + col;
			KeyState *state = &Matrix_scanArray[ key ];
			if ( keyOn(state->curState) )
				used++;
		}
		row_use[row] = used;
		row_ghost_old[row] = row_ghost[row];
		row_ghost[row] = 0;  // clear
	}

	// Check if matrix has ghost
	// Happens when key is pressed and some other key is pressed in same row and another in same column
	for ( uint8_t col = 0; col < NumCols; col++ )
	{
		for ( uint8_t row = 0; row < NumRows; row++ )
		{
			uint8_t key = NumCols * row + col;
			KeyState *state = &Matrix_scanArray[ key ];
			if ( keyOn(state->curState) && col_use[col] >= 2 && row_use[row] >= 2 )
			{
				// mark col and row as having ghost
				col_ghost[col] = 1;
				row_ghost[row] = 1;
			}
		}
	}

	int gh_cols = 0, gh_rows = 0;
	for ( uint8_t col = 0; col < NumCols; col++ )
		if (col_ghost[col])  ++gh_cols;
	for ( uint8_t row = 0; row < NumRows; row++ )
		if (row_ghost[row])  ++gh_rows;
	ghost_cols = gh_cols;
	ghost_rows = gh_rows;

	// Send keys
	for ( uint8_t col = 0; col < NumCols; col++ )
	{
		for ( uint8_t row = 0; row < NumRows; row++ )
		{
			uint8_t key = NumCols * row + col;
			KeyState *state = &Matrix_scanArray[ key ];
			KeyGhost *st = &Matrix_ghostArray[ key ];

			// col or row is ghosting (crossed)
			uint8_t ghost = (col_ghost[col] > 0 || row_ghost[row] > 0) ? 1 : 0;
			uint8_t ghost_old = (col_ghost_old[col] > 0 || row_ghost_old[row] > 0) ? 1 : 0;
			ghost = ghost || ghost_old ? 1 : 0;

			st->prev = st->cur;  // previous
			// save state if no ghost or outside ghosted area
			if ( ghost == 0 )
				st->saved = state->curState;  // save state if no ghost
			// final
			// use saved state if ghosting, or current if not
			st->cur = ghost > 0 ? st->saved : state->curState;

			//  Send keystate to macro module
			KeyPosition k = !st->cur
				? (!st->prev ? KeyState_Off : KeyState_Release)
				: ( st->prev ? KeyState_Hold : KeyState_Press);

			state->state = k;  // final
			//Macro_keyState( key, k );

			if (k == KeyState_Press)
			{	cnt_press++;  cnt_press1min++;  }
			if (k == KeyState_Hold)
				cnt_hold++;
			if (k == KeyState_Release)
				cnt_rel++;
		}
	}
#endif
}
