#include "def.h"


//  Main Menu
const char* strMain[M_All] =
{
	"Testing", "Mapping", "Sequences",
	"Display", //"Clock",
#ifdef GAME
	"Game",
#endif
#ifdef DEMOS
	"Demos",
#endif
	//"Help", F1
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
	"Pressed", "Matrix", "Layout", "Scan Setup", "Mouse",
};

//  sub pages
const uint8_t YM1[M_All] =
{
	T_All,  // M_Testing
	4,  // M_Mapping
	1,  // M_Sequences

	1,  // M_Display
#ifdef GAME
	1,
#endif
#ifdef DEMOS
	D_All,  // M_Demos
#endif
};
