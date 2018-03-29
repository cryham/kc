#pragma once
#include <stdint.h>

#define DEMOS

//  display dim
#define W 160
#define H 128

//  R F800  G 07E0  B 001F  R 32 G 64 B 32  565
#define RGB(r,g,b) ( ((r)<<11)+ ((g)<<6/*<<5*/) +b)


enum EMainMenu  //  main menu entries, level0
{
	M_Testing=0, M_Mappings, M_Sequences,
	M_Display, M_Demos,	/*M_Game,*/ M_All
};
enum EDemo  // Demos, level1
{
	D_Plasma=0, D_Wave, D_Hedrons, D_CK_Logo,
	D_Space, D_Balls, D_Rain, D_Fountain,
	D_Ngons, D_Fonts,
	D_All, D_Next = D_CK_Logo,  // next column
};
enum ETest  // Testing kbd
{
	T_ScanSetup, T_Matrix, T_Pressed, T_Mouse, T_All
};

//  string names for all above ^
extern const char
	*strMain[M_All], *strDemo[D_All], *strTest[T_All];

//  sub pages  inside each main menu entry
extern const uint8_t YM2[M_All];
