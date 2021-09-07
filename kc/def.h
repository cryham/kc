#pragma once
#include <stdint.h>

//----  Setup  ----	  use:  ram B      flash
//  optional features, comment out to disable
#define DEMOS			//  28        9%
#define DEMOS_PLASMA	//  36        11%
#define DEMOS_3D		//  44        15%


//   Keyboard  ----
//  define just one type, for matrix.h
#define CK1  // 8x6

//-----------------
//  extra keyboard features, pins
#ifdef CK6
    //  led light
	//#define LED  12
	//  temperature 'C sensor DS18B20
	#define TEMP1  31	//  44   9%  24k
#endif


#define W 160  //  display dim
#define H 128

//  R F800  G 07E0  B 001F  R 32 G 64 B 32  565
#define RGB(r,g,b)  ( ((r)<<11)+ ((g)<<6) +(b))   // 31 31 31
#define RGB2(r,g,b) ( ((r)<<11)+ ((g)<<5) +(b))  // 31 63 31

#ifdef __cplusplus
class Ada4_ST7735;
#endif


enum EMainMenu  //  main menu entries, level0
{
	M_Testing, M_Setup, M_Info,
	M_Display, M_Clock, M_Help,
	#ifdef DEMOS
		M_Demos,
	#endif
	M_All,  M_Next = M_Display  // 2nd column, -1 off
};

#ifdef DEMOS
enum EDemo  //  Demos, level1
{
	#ifdef DEMOS_PLASMA
		D_Plasma,
	#endif
		D_Wave, D_Fire,
	#ifdef DEMOS_3D
		D_Hedrons,
	#endif
		D_CK_Logo,
	D_Rain, D_Ngons, D_Fonts,
	D_All,  D_Next = D_CK_Logo
};
extern const char *strDemo[D_All];
#endif

//  menus  --
enum ETesting  //  Testing kbd, level1
{
	T_Layout, T_Pressed, T_Matrix, T_All
};

enum ESetup  //  Setup kbd, level1
{
	S_Layer, S_Keyboard, S_Mouse, S_Scan, S_All
};

enum EInfo  //  Info use,ver
{
	I_Use, I_Version, I_All
};

//  pages  --
enum EDisplay
{
	Di_Bright, Di_Key, Di_Stats, Di_Graph, Di_Debug, Di_All
};
enum EClock
{
	Cl_Stats,
	Cl_Graphs,
	Cl_Adjust,
	Cl_All
};

//  string names for all above ^
extern const char
	*strMain[M_All], *strTest[T_All], *strSetup[S_All],
	*strInfo[I_All], *strClock[Cl_All];

//  sub page counts, inside each main menu entry
extern const uint8_t YM1[M_All];

//  time intervals  *0.1s
extern const uint16_t gIntervals[];
const static uint8_t gIntvMask = 0x1F;

