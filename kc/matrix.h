#pragma once

// ----- Enums -----

typedef enum Port {
	Port_A = 0,	Port_B = 1,	Port_C = 2,	Port_D = 3,	Port_E = 4,
} Port;

typedef enum Pin {
	Pin_0  = 0,	Pin_1  = 1,	Pin_2  = 2,	Pin_3  = 3,	Pin_4  = 4,	Pin_5  = 5,	Pin_6  = 6,	Pin_7  = 7,
	Pin_8  = 8,	Pin_9  = 9,	Pin_10 = 10,	Pin_11 = 11,	Pin_12 = 12,	Pin_13 = 13,	Pin_14 = 14,
	Pin_15 = 15,	Pin_16 = 16,	Pin_17 = 17,	Pin_18 = 18,	Pin_19 = 19,	Pin_20 = 20,	Pin_21 = 21,
	Pin_22 = 22,	Pin_23 = 23,	Pin_24 = 24,	Pin_25 = 25,	Pin_26 = 26,	Pin_27 = 27,	Pin_28 = 28,
	Pin_29 = 29,	Pin_30 = 30,	Pin_31 = 31,
} Pin;

// Type of pin
typedef enum Type {
	Type_StrobeOn,	Type_StrobeOff,	Type_StrobeSetup,	Type_Sense,	Type_SenseSetup,
} Type;

// Sense/Strobe configuration
typedef enum Config {
	Config_Pullup,  Config_Pulldown,  Config_Opendrain,
} Config;

// Keypress States
typedef enum KeyPosition {
	KeyState_Off     = 0,
	KeyState_Press   = 1,
	KeyState_Hold    = 2,
	KeyState_Release = 3,
	KeyState_Invalid,
} KeyPosition;


// ----- Structs -----

// Struct container for defining Rows (Sense) and Columns (Strobes)
typedef struct GPIO_Pin {
	Port port;  Pin  pin;
} GPIO_Pin;

#define gpio( port, pin ) { Port_##port, Pin_##pin }


//  ----  pinout  ----
#define T3_0  gpio(B,16)
#define T3_1  gpio(B,17)
#define T3_2  gpio(D,0 )
#define T3_3  gpio(A,12)
#define T3_4  gpio(A,13)
#define T3_5  gpio(D,7 )
#define T3_6  gpio(D,4 )
#define T3_7  gpio(D,2 )
#define T3_8  gpio(D,3 )
#define T3_9  gpio(C,3 ) //spi
#define T3_10 gpio(C,4 ) //spi
#define T3_11 gpio(C,6 ) //spi
#define T3_12 gpio(C,7 )
#define T3_13 gpio(C,5 ) //spi
#define T3_14 gpio(D,1 )
#define T3_15 gpio(C,0 )
#define T3_16 gpio(B,0 )
#define T3_17 gpio(B,1 )
#define T3_18 gpio(B,3 )
#define T3_19 gpio(B,2 )
#define T3_20 gpio(D,5 )
#define T3_21 gpio(D,6 )
#define T3_22 gpio(C,1 )
#define T3_23 gpio(C,2 )
#define T3_24 gpio(A,5 )
#define T3_25 gpio(B,19)
#define T3_26 gpio(E,1 )
#define T3_27 gpio(C,9 )
#define T3_28 gpio(C,8 )
#define T3_29 gpio(C,10)
#define T3_30 gpio(C,11)
#define T3_31 gpio(E,0 )
#define T3_32 gpio(B,18)
#define T3_33 gpio(A,4 )
#define T3_40 gpio(A,14) //dac


//  ----***  matrix  ***----
// 4 x 2
//const static GPIO_Pin Matrix_cols[] = { T3_14, T3_15, T3_18, T3_19 };
//const static GPIO_Pin Matrix_rows[] = { T3_16, T3_17 };
// 7 x 6
//| 2:19  4:16  8:18  9:14  10:15  11:17  12:20  13:21
//- 3:0  4:1  5:2  6:3  7:23  8:22
const static GPIO_Pin Matrix_cols[] = { T3_19, T3_16, T3_18, T3_14, T3_15, T3_17, T3_20, T3_21 };
const static GPIO_Pin Matrix_rows[] = { T3_0, T3_1, T3_2, T3_3, T3_23, T3_22 };
// 3 x 3
//const static GPIO_Pin Matrix_cols[] = { T3_16, T3_18, T3_15 };
//const static GPIO_Pin Matrix_rows[] = { T3_0, T3_1, T3_2 };


#define Matrix_colsNum  sizeof( Matrix_cols ) / sizeof( GPIO_Pin )
#define Matrix_rowsNum  sizeof( Matrix_rows ) / sizeof( GPIO_Pin )
#define Matrix_maxKeys  sizeof( Matrix_scanArray ) / sizeof( KeyState )


// Define type of scan matrix
#define /*Config*/ Matrix_type Config_Pullup


// This will enable the anti-ghosting code
#define GHOSTING_MATRIX

// Delay in microseconds before and after each strobe change during matrix scan
#define STROBE_DELAY  4  // 3 too low for >=3 in row, 4 ok

#define DebounceDivThreshold_define 65535
#define DebounceCounter uint16_t

#define DebounceThrottleDiv_define 0
#define MinDebounceTime_define 1

//#define StrobeDelay_define 0
//#define StateWordSize_define 16
//#define IndexWordSize_define 16
//#define KeyboardLocale_define 0
//#define USBProtocol_define 1

// Debounce Element
typedef struct KeyState {
	DebounceCounter activeCount;
	DebounceCounter inactiveCount;
	KeyPosition     prevState;
	KeyPosition     curState;
	uint32_t        prevDecisionTime;
	KeyPosition     state;  // final after ghost
} KeyState;

#ifdef GHOSTING_MATRIX
// Ghost Element, after ghost detection/cancelation
typedef struct KeyGhost {
	KeyPosition     prev;
	KeyPosition     cur;
	KeyPosition     saved;  // state before ghosting
} KeyGhost;

// utility
inline uint8_t keyOn(/*KeyPosition*/uint8_t st)
{
	return (st == KeyState_Press || st == KeyState_Hold) ? 1 : 0;
}

//  for ghost status info
extern int ghost_cols, ghost_rows;  // ghosting if any > 0
extern uint8_t col_ghost[Matrix_colsNum], row_ghost[Matrix_rowsNum];  // marked as having ghost if 1
#endif

extern int cnt_press, cnt_hold, cnt_rel;  // test


// ----- Functions -----
#ifdef	__cplusplus
extern "C"
{
#endif

extern void Matrix_setup();
extern void Matrix_scan( uint16_t scanNum );

extern KeyState Matrix_scanArray[ Matrix_colsNum * Matrix_rowsNum ];


#ifdef	__cplusplus
}
#endif
