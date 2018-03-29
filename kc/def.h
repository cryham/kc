#pragma once


#define DEMOS  // if not, only keyboard Gui

//  display dim
#define W 160
#define H 128

//  R F800  G 07E0  B 001F  R 32 G 64 B 32  565
#define RGB(r,g,b) ((r<<11)+ ((g)<<6/*<<5*/) +b)


enum EDemo
{
	D_Plasma=0, D_Wave, D_Hedrons, D_CK_Logo,
D_Next = D_CK_Logo,  // next column
	D_Space, D_Balls, D_Rain, D_Fountain,
	D_Ngons, D_Fonts, D_Chars,
D_All
};

