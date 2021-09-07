#include "def.h"
#include "gui.h"

//  time intervals *0.1s
const uint16_t gIntervals[gIntvMask+1] =
{
	0, 1 /*100ms 1*/, 2, 4, 6, 8,
	10   /*1s 6*/   ,20,40,60,80,
	100  /*10s 11*/ ,200,300,450,
	600  /*1m 15*/  ,720,900,
	1200 /*2m 18*/  ,2100,3000,4500,
	6000 /*10m 22*/ ,7200,9000,
	12000/*20m 25*/ ,15000,18000,24000,
	36000/*1h 29*/  ,45000,
	54000/*1.5h 31*/
};

//  menu colors
const uint8_t Gui::Mclr[Gui::C_ALL][2][3] =
{
	{{20,26,31},{5,3,1}},  // 0 main
	{{27,26,31},{2,3,1}},  // 1 demos
	{{20,30,26},{6,3,4}},  //  2 test
	{{22,31,18},{6,3,7}},  //  3 map
	{{17,31,31},{5,4,3}},  //  4 seqs
	{{24,27,27},{5,4,3}},  // 5 setup
	{{31,31,16},{3,5,5}},  //  6 display
	{{30,30,30},{5,3,1}},  //  7 clock
	{{26,28,28},{4,3,3}},  // 8 setup2
	{{31,26,12},{1,4,5}},  //  9 game
	{{31,26,22},{1,6,8}},  //  10 game opt
	{{26,26,28},{6,6,7}},  // 11 info
};

//  Main Menu
const char* strMain[M_All] =
{
	"Testing", "Setup", "Info",
	"Display", "Clock", "Help",
#ifdef DEMOS
	"Demos",
#endif
};

//  Testing kbd
const char* strTest[T_All] =
{
	"Layout", "Pressed", "Matrix #",
};
//  Setup kbd
const char* strSetup[S_All] =
{
	"Layer", "Keyboard", "Mouse", "Scan #",
};
//  Info use, ver
const char* strInfo[I_All] =
{
	"Use", "Version",
};
//  Clock pages
const char* strClock[Cl_All] =
{
	"Stats", "Graph", "Adjust", 
};


//  Demos
#ifdef DEMOS
const char* strDemo[D_All] =
{
#ifdef DEMOS_PLASMA
	"Plasma",
#endif
	"Wave", "Fire",
#ifdef DEMOS_3D
	"Hedrons",
#endif
	"CK Logo",
	"Rain",
	"N-gons", "Fonts",
};
#endif


//  sub pages
const uint8_t YM1[M_All] =
{
	T_All,  // M_Testing
	S_All,  // M_Setup
	I_All,  // M_Info
	1,  // M_Display
	1,  // M_Clock  Cl_All
	1,  // M_Help
#ifdef DEMOS
	D_All,  // M_Demos
#endif
};
