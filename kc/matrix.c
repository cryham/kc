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


//-------------------------------------------------------------------------------------------
// code below, originally by DMA from  https://github.com/dmaone/controller
// https://deskthority.net/viewtopic.php?p=345299#p345299
#define NL "\r\n"
#define print(s) 
#define printInt32(i) 
#define printInt16(i) 


// Number of scans since the last USB send
uint16_t Scan_scanCount = 0;

volatile uint8_t CoCo1 = 0;
volatile uint8_t CoCo2 = 0;

// ADC-related stuf
#define ADC0_CHAN_COUNT 16
#define ADC1_CHAN_COUNT 16
uint8_t adc0_sequencer[ADC0_CHAN_COUNT] = {
	 4, 30, 15, 30,   7, 30,  6, 30,
	12, 30, 13, 30,  14, 30,  5, 30
};
uint8_t adc1_sequencer[ADC1_CHAN_COUNT] = {
	30,  9, 30,  8,  30,  7 ,30,  6,
	30,  4, 30,  5,  30,  0 ,30,  3
};

volatile uint16_t adc0_results[8][ADC0_CHAN_COUNT];
volatile uint16_t adc0_results_prev[8][ADC0_CHAN_COUNT];
volatile uint16_t adc1_results[8][ADC0_CHAN_COUNT];
volatile uint16_t adc1_results_prev[8][ADC0_CHAN_COUNT];
// ----- Functions -----

/*
 * To calibrate: SC2 = SW trigger no DMA, alt Vref
 * CFG2 = MUX b channels, 24ADCLK sample (24/16/10/6) if ADLSMP 
 * plus gain must be calculated and set by hand because calibration doesn't.
 */
#define ADC_CALIBRATE(idx)\
	ADC##idx##_SC2 = 0x0; \
	ADC##idx##_CFG1	= ADC_CFG1_ADIV(8) | ADC_CFG1_MODE(1) \
			| ADC_CFG1_ADLSMP | ADC_CFG1_ADICLK(3); \
\
	ADC##idx##_CFG2	= ADC_CFG2_MUXSEL | ADC_CFG2_ADACKEN \
			| ADC_CFG2_ADHSC | ADC_CFG2_ADLSTS(0); \
\
	ADC##idx##_SC3 = ADC_SC3_AVGE | ADC_SC3_AVGS(3); /* 32x */ \
\
	print( "Cal Start.." ); \
	ADC##idx##_SC3 |= ADC_SC3_CAL; \
	uint32_t t##idx = 100000; \
	while (t##idx && 0 == (ADC##idx##_SC1A & ADC_SC1_COCO)) { t##idx--; };\
	print( ".end. Cycles remaining: " );\
	printInt32( t##idx );\
	print( NL );\
\
	if (ADC##idx##_SC3 & ADC_SC3_CALF) return 255; /* failed */ \
	ADC##idx##_PG = (ADC##idx##_CLP0 + ADC##idx##_CLP1 + ADC##idx##_CLP2 \
 	+ ADC##idx##_CLP3 + ADC##idx##_CLP4 + ADC##idx##_CLPS) / 2 | 0x8000u; \


/* To setup:
 * CFG2 = MUX b channels, ADACKEN, ADHSC, 24ADCLK sample (24/16/10/6) if ADLSMP
 * SC3 = Single shot, no averaging
 * SC2 = internal Vref (=1)
*/
#define ADC_SETUP(mod)\
	ADC##mod##_CFG1 = ADC_CFG1_ADIV(1) | ADC_CFG1_MODE(1) | ADC_CFG1_ADICLK(1) /*| ADC_CFG1_ADLSMP*/ ;\
	ADC##mod##_CFG2 	= ADC_CFG2_MUXSEL | ADC_CFG2_ADACKEN \
			| ADC_CFG2_ADHSC | ADC_CFG2_ADLSTS(2); \
	ADC##mod##_SC3 = 0u; \
	ADC##mod##_SC2 = 0x01u; \

#define SENSOR_SETUP(idx) \
	ADC_CALIBRATE(idx) \
	ADC_SETUP(idx) \

// Calibration macro returns 255 on calibration fail.
// Dunno, may be remove that and integrate into Scan_setup
uint8_t ADC_Setup()
{
	// Power
	SIM_SCGC3 |= SIM_SCGC3_ADC1;
	SIM_SCGC6 |= SIM_SCGC6_ADC0;

	ADC_CALIBRATE(0)
	ADC_SETUP(0)

	ADC_CALIBRATE(1)
	ADC_SETUP(1)
	return 0;
}

// Setup
inline void Scan_setup()
{
	// Register Scan CLI dictionary
	//CLI_registerDictionary( scanCLIDict, scanCLIDictName );

	// Setup GPIO pins for matrix scanning
	Matrix_setup();

	// Reset scan count
	Scan_scanCount = 0;

	SIM_SCGC4 |= SIM_SCGC4_VREF;

	// Vref: VREFEN + REGEN + ICOMPEN + MODE_LV=1 (high power, 2 is low)
	VREF_SC = 0x80u + 0x40u + 0x20u + 0x01u;

	//SIM_SCGC5 |= SIM_SCGC5_PORTA | SIM_SCGC5_PORTB | SIM_SCGC5_PORTC | SIM_SCGC5_PORTD | SIM_SCGC5_PORTE;

	ADC_Setup();
}


void Matrix_sense(uint8_t enable)
{
	/*for (uint8_t pin = 0; pin < Matrix_colsNum; pin++)
	{
		if (enable)
		{
			Matrix_pin( Matrix_rows[ pin ], Type_AnalogSenseOn );
		} else {
			Matrix_pin( Matrix_rows[ pin ], Type_AnalogSenseOff );
		}
	}*/
}

void Matrix_strobe(uint8_t pin, uint8_t enable)
{
	if (enable)
	{
		Matrix_pin( Matrix_cols[ pin ], Type_StrobeOn );
	} else {
		Matrix_pin( Matrix_cols[ pin ], Type_StrobeOff );
	}
}


// Main Detection Loop
inline uint8_t Scan_loop()
{
	return 0;
	for (uint8_t i = 0; i < 8; i++)
	{
		Matrix_sense(1);
		Matrix_strobe(i, 1);
		//printInt32( SYST_CVR );
		for (uint8_t k = 0; k < ADC0_CHAN_COUNT; k++)
		{
			ADC0_SC1A = adc0_sequencer[k];
			ADC1_SC1A = adc1_sequencer[k];
			while (!(ADC0_SC1A && ADC_SC1_COCO)){};
			adc0_results[i][k] = (uint16_t)ADC0_RA >> 1;
			while (!(ADC1_SC1A && ADC_SC1_COCO)){};
			adc1_results[i][k] = (uint16_t)ADC1_RA >> 1;
			delayMicroseconds(3);
		}
		Matrix_sense(0);
		Matrix_strobe(i, 0);
		delayMicroseconds(3); // Give it time to settle down
	}
	// Not ready for prime time.
	//Matrix_scan( Scan_scanCount++ );
	return 0;
}

/*
// Signal from Macro Module that all keys have been processed (that it knows about)
inline void Scan_finishedWithMacro( uint8_t sentKeys )
{
}


// Signal from Output Module that all keys have been processed (that it knows about)
inline void Scan_finishedWithOutput( uint8_t sentKeys )
{
	// Reset scan loop indicator (resets each key debounce state)
	// TODO should this occur after USB send or Macro processing?
	Scan_scanCount = 0;
}
*/

// ----- Capabilities -----
/*
// Custom capability examples
// Refer to kll.h in Macros/PartialMap for state and stateType information
void CustomAction_action1_capability( uint8_t state, uint8_t stateType, uint8_t *args )
{
	// Display capability name
	// XXX This is required for debug cli to give you a list of capabilities
	if ( stateType == 0xFF && state == 0xFF )
	{
		print("CustomAction_action1_capability()");
		return;
	}

	// Prints Action1 info message to the debug cli
	info_print("Action1");
}

uint8_t CustomAction_blockHold_storage = 0;
void CustomAction_blockHold_capability( uint8_t state, uint8_t stateType, uint8_t *args )
{
	// Display capability name
	if ( stateType == 0xFF && state == 0xFF )
	{
		print("CustomAction_blockHold_capability(usbCode)");
		return;
	}

	// Retrieve 8-bit argument
	uint8_t key = args[0];

	// We only care about normal keys
	if ( stateType == 0x00 )
	{
		// Block given key if we're in the "Press" or "Hold" state
		if ( ( state == 0x01 || state == 0x02 )
			&& CustomAction_blockHold_storage == 0 )
		{
			CustomAction_blockHold_storage = key;
			info_msg("Blocking Key: ");
			printHex( key );
			print( NL );
		}
		// Release if in the "Off" or "Release" state and we're blocking
		else if ( ( state == 0x00 || state == 0x03 )
			&& key == CustomAction_blockHold_storage )
		{
			info_msg("Unblocking Key: ");
			printHex( CustomAction_blockHold_storage );
			print( NL );
			CustomAction_blockHold_storage = 0;
		}
	}
}

void CustomAction_blockKey_capability( uint8_t state, uint8_t stateType, uint8_t *args )
{
	// Display capability name
	if ( stateType == 0xFF && state == 0xFF )
	{
		print("CustomAction_blockKey_capability(usbCode)");
		return;
	}

	// Retrieve 8-bit argument
	uint8_t key = args[0];

	// If key is not blocked, process
	if ( key != CustomAction_blockHold_storage )
	{
		extern void Output_usbCodeSend_capability( uint8_t state, uint8_t stateType, uint8_t *args );
		Output_usbCodeSend_capability( state, stateType, &key );
	}
}


// Signal from the Output Module that the available current has changed
// current - mA
void Scan_currentChange( unsigned int current )
{
	// Indicate to all submodules current change
	Matrix_currentChange( current );
}
*/


// ----- CLI Command Functions -----
void cliFunc_T( char* args )
{
	// Test the scan loop (at max speed or general debugging)
	print( NL );
	for (uint32_t j = 0; j < 1; j++)
	{
		for (uint8_t i = 0; i < 8; i++)
		{
			Matrix_sense(1);
			Matrix_strobe(i, 1);
			for (uint8_t k = 0; k < ADC0_CHAN_COUNT; k++)
			{
				ADC0_SC1A = adc0_sequencer[k];
				ADC1_SC1A = adc1_sequencer[k];
				while (!(ADC0_SC1A && ADC_SC1_COCO)){};
				adc0_results[i][k] = (uint16_t)ADC0_RA;
				while (!(ADC1_SC1A && ADC_SC1_COCO)){};
				adc1_results[i][k] = (uint16_t)ADC1_RA;
				delayMicroseconds(3);
			}
			Matrix_sense(0);
			Matrix_strobe(i, 0);
			delayMicroseconds(3); // Give it time to settle down
		}
		cliFunc_ADCPrint("");
		if (j % 1000 == 0)
		{
			print ( ".");
		}
	}

}

void cliFunc_ADCPrint( char* args )
{
	print( NL );
	for (uint8_t j=0; j < 8; j++)
	{
		for (uint8_t i=0; i < ADC0_CHAN_COUNT; i++)
		{
			if (adc0_results[j][i] - adc0_results_prev[j][i] < 0)
			{
				print("-");
				printInt16(adc0_results_prev[j][i] - adc0_results[j][i]);
			}
			else
				printInt16(adc0_results[j][i] - adc0_results_prev[j][i]);
			adc0_results_prev[j][i] = adc0_results[j][i];
			print (" ");
		}
		print ("| ");
		for (uint8_t i=0; i < ADC1_CHAN_COUNT; i++)
		{
			if (adc1_results[j][i] - adc1_results_prev[j][i] < 0)
			{
				print("-");
				printInt16(adc1_results_prev[j][i] - adc1_results[j][i]);
			}
			else
				printInt16(adc1_results[j][i] - adc1_results_prev[j][i]);
			adc1_results_prev[j][i] = adc1_results[j][i];
			print (" ");
		}
		print( NL );
	}
}

/*
inline void printADCCalData( void )
{
	printHex(ADC0_PG);
	dPrint (" ");
	printHex(ADC0_CLP0);
	dPrint (" ");
	printHex(ADC0_CLP1);
	dPrint (" ");
	printHex(ADC0_CLP2);
	dPrint (" ");
	printHex(ADC0_CLP3);
	dPrint (" ");
	printHex(ADC0_CLP4);
	dPrint (" ");
	printHex(ADC0_CLPS);
	dPrint (" ");
	printHex(ADC0_CLPD);
	dPrint ( NL );
}

void cliFunc_ADCCal( char* args )
{
	print( NL );
	print ( "Old: " );
	printADCCalData();
	ADC_Setup();
	print ( "New: ");
	printADCCalData();
}
*/
