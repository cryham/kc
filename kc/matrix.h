#pragma once
#include <stdint.h>

//  ----- Enums -----
typedef enum Port {
	Port_A = 0,	Port_B = 1,	Port_C = 2,	Port_D = 3,	Port_E = 4,
} Port;

typedef enum Pin {
	Pin_0  = 0, Pin_1   = 1,  Pin_2  = 2,  Pin_3  = 3,  Pin_4  = 4,  Pin_5  = 5,  Pin_6  = 6,  Pin_7  = 7,
	Pin_8  = 8, Pin_9   = 9,  Pin_10 = 10, Pin_11 = 11, Pin_12 = 12, Pin_13 = 13, Pin_14 = 14, Pin_15 = 15,
	Pin_16 = 16, Pin_17 = 17, Pin_18 = 18, Pin_19 = 19, Pin_20 = 20, Pin_21 = 21, Pin_22 = 22, Pin_23 = 23,
	Pin_24 = 24, Pin_25 = 25, Pin_26 = 26, Pin_27 = 27, Pin_28 = 28, Pin_29 = 29, Pin_30 = 30, Pin_31 = 31,
} Pin;

typedef enum Type {  //  Type of pin
	Type_StrobeOn,	Type_StrobeOff,	Type_StrobeSetup,	Type_Sense,	Type_SenseSetup,
} Type;

//  Sense/Strobe config
typedef enum Config {
	Config_Pullup,  Config_Pulldown,  Config_Opendrain,
} Config;

//  Keypress States
typedef enum KeyPosition {
	KeyState_Off     = 0,
	KeyState_Press   = 1,
	KeyState_Hold    = 2,
	KeyState_Release = 3,
	KeyState_Invalid,
} KeyPosition;


//  Struct for defining Rows- (Sense) and Columns| (Strobes)
typedef struct GPIO_Pin {
	Port port;  Pin  pin;
} GPIO_Pin;

#define gpio(port, pin)  { Port_##port, Pin_##pin }


//  ---- Pinout ----
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


//  ----***  Matrix  ***----
//  8 x 6
//c| 2:19  4:16  8:18  9:14  10:15  11:17  12:20  13:21
//r- 3:0  4:1  5:2  6:3  7:23  8:22
const static GPIO_Pin
	Matrix_cols[] = { T3_19, T3_16, T3_18, T3_14, T3_15, T3_17, T3_20, T3_21 },
	Matrix_rows[] = { T3_0, T3_1, T3_2, T3_3, T3_23, T3_22 };


#define NumCols  sizeof( Matrix_cols ) / sizeof( GPIO_Pin )
#define NumRows  sizeof( Matrix_rows ) / sizeof( GPIO_Pin )
#define MaxKeys  (NumCols * NumRows)  // sizeof( Matrix_scanArray ) / sizeof( KeyState )


//  Type of scan matrix
#define /*Config*/ Matrix_type Config_Pullup


//  Enable anti-ghosting code
#define GHOSTING_MATRIX

//  Delay in microseconds before and after each strobe change during matrix scan
extern int STROBE_DELAY;  // 3 too low for >=3 in row, 4 ok

#define DebounceDivThreshold_define 65535
#define DebounceCounter uint16_t

#define DebounceThrottleDiv_define 0
#define MinDebounceTime_define 1  // todo 5 ??..


//  Debounce Element
typedef struct KeyState {
	DebounceCounter activeCount;
	DebounceCounter inactiveCount;
	KeyPosition     prevState;
	KeyPosition     curState;
	uint32_t        prevDecisionTime;
	KeyPosition     state;  // final after ghost
} KeyState;

#ifdef GHOSTING_MATRIX
//  Ghost Element, after ghost detection/cancelation
typedef struct KeyGhost {
	KeyPosition     prev;
	KeyPosition     cur;
	KeyPosition     saved;  // state before ghosting
} KeyGhost;


//  -----  utility  -----

#define Key(y,x)  (Matrix_scanArray[y * NumCols + x].state == KeyState_Press ? 1 : 0)

inline uint8_t keyOn(/*KeyPosition*/uint8_t st)
{
	return (st == KeyState_Press || st == KeyState_Hold) ? 1 : 0;
}

//  Ghost status info
extern int ghost_cols, ghost_rows;  // ghosting if any > 0
extern uint8_t col_use[NumCols], row_use[NumRows];  // used count
extern uint8_t col_ghost[NumCols], row_ghost[NumRows];  // marked as having ghost if 1
#endif

extern int cnt_press, cnt_hold, cnt_rel;  // testing


//  ----- Functions -----
#ifdef	__cplusplus
extern "C"
{
#endif

extern void Matrix_setup();
extern void Matrix_scan( uint16_t scanNum );  // todo ??..

extern KeyState Matrix_scanArray[MaxKeys];

#ifdef	__cplusplus
}
#endif
