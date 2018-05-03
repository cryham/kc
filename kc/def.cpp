#include "def.h"
#include "gui.h"

//  menu colors
const uint8_t Gui::Mclr[Gui::Cl_ALL][2][3] = {
	{{20,26,31},{5,3,1}},  // 0 main
	{{27,26,31},{2,3,1}},  // 1 demos
	{{20,30,26},{6,3,4}},  //  2 test
	{{22,31,18},{6,3,7}},  //  3 map
	{{17,31,31},{5,4,3}},  //  4 seqs
	{{24,27,27},{5,4,3}},  // 5 setup
	{{31,31,16},{3,5,5}},  // 6 display
	{{26,28,28},{4,3,3}},  // 7 setup2
	{{31,26,12},{1,4,5}},  //  8 game
	{{31,26,22},{1,6,8}},  //  9 game opt
};

//  Main Menu
const char* strMain[M_All] =
{
	"Mapping", "Sequences",
	"Testing", "Setup",
	"Display", "Help",
#ifdef GAME
	"Game",
#endif
#ifdef DEMOS
	"Demos",
#endif
};

//  Testing kbd
const char* strTest[T_All] =
{
	"Pressed", "Matrix", "Layout",
};
//  Setup kbd
const char* strSetup[S_All] =
{
	"Scan", "Keyboard", "Mouse", "Version",
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
#ifdef DEMOS_OLD_PAR
	"Space", "Balls", "Fountain",
#endif
	"Rain",
	"N-gons", "Fonts",
};
#endif


//  sub pages
const uint8_t YM1[M_All] =
{
	4,  // M_Mapping
	1,  // M_Sequences
	T_All,  // M_Testing
	S_All,  // M_Setup
	1,  // M_Display
	1,  // M_Help
#ifdef GAME
	1,  // M_Game
#endif
#ifdef DEMOS
	D_All,  // M_Demos
#endif
};
