#pragma once
#include <stdint.h>


//----  Setup  ----		   ram B      flash
//   no demos ck1		// 45572 69%  58%
//  all demos ck1 max	// 52252 79%  75% ok

//   no demos ck3		//  0    0%
#define DEMOS			//  28   9%
#define DEMOS_PLASMA	//  36   11%
#define DEMOS_3D		//  44   15%
//#define DEMOS_OLD_PAR	//  280 <1%   16%  min
					// all  6680 10%  17%  max 41k  75% total
#define GAME		// game 1.5k 2%   22.9k 7%

//   keyboard
//#define CK1  // 8x6
//#define CK6  // 18x8  new CK3
#define CK7  // 18x8  old CK4/2
//-----------------


//  display dim
#define W 160
#define H 128

//  R F800  G 07E0  B 001F  R 32 G 64 B 32  565
#define RGB(r,g,b) ( ((r)<<11)+ ((g)<<6) +(b))   // 31 31 31
#define RGB2(r,g,b) ( ((r)<<11)+ ((g)<<5) +(b))  // 31 63 31

#ifdef __cplusplus
class Ada4_ST7735;
#endif


enum EMainMenu  //  main menu entries, level0
{
		M_Testing=0, M_Mapping, M_Sequences,
		M_Display, //M_Clock
	#ifdef GAME
		M_Game,
	#endif
	#ifdef DEMOS
		M_Demos,
	#endif
		M_All
};

#ifdef DEMOS
enum EDemo  // Demos, level1
{
	#ifdef DEMOS_PLASMA
		D_Plasma,
	#endif
		D_Wave, D_Fire,
	#ifdef DEMOS_3D
		D_Hedrons,
	#endif
		D_CK_Logo,
	#ifdef DEMOS_OLD_PAR
		D_Space, D_Balls, D_Fountain,
	#endif
		D_Rain, D_Ngons, D_Fonts,

		D_All,
		D_Next = D_CK_Logo,
};
extern const char *strDemo[D_All];
#endif

enum ETesting  // Testing kbd, level1
{
	T_Pressed=0, T_Matrix, T_Layout, T_ScanSetup, T_Mouse, T_All
};

//  string names for all above ^
extern const char *strMain[M_All], *strTest[T_All];

//  sub page counts, inside each main menu entry
extern const uint8_t YM1[M_All];
