#include "def.h"


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


//  Testing kbd
const char* strTest[T_All] =
{
	"Pressed", "Matrix", "Layout",
};
//  Setup kbd
const char* strSetup[S_All] =
{
	"Scan", "Keyboard", "Mouse", "Version"
};


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
