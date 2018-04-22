#pragma once
#include <stdint.h>


//----  Setup  ----			ram B  flash
//   no demos			// 47820  57%
#define DEMOS			// 47844  66%  all 7k 11% ram, 41k 16% flash
#define DEMOS_PLASMA	// 47852  68%
#define DEMOS_3D		// 47860  72%
//#define DEMOS_OLD_PAR	// 48096  73%  min
//   keyboard
//#define CK1  // 8x6
#define CK3  // 18x8
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
#ifdef DEMOS
	M_Demos,
#endif
	/*M_Game,*/ M_All
};

#ifdef DEMOS
enum EDemo  // Demos, level1
{
#ifdef DEMOS_PLASMA
	D_Plasma,
#endif
	D_Wave,
	D_CK_Logo,
#ifdef DEMOS_3D
	D_Hedrons,
#endif
#ifdef DEMOS_OLD_PAR
	D_Space, D_Balls, D_Fountain,
#endif
	D_Rain, D_Ngons, D_Fonts,

	D_All,
#ifdef DEMOS_OLD_PAR
	D_Next = D_Space  // next column
#else
	D_Next = D_Rain
#endif
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
