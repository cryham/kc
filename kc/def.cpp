#include "def.h"


//  Main Menu
const char* strMain[M_All] =
{
	"Testing", "Mapping", "Sequences",
	"Display", //"Clock",
#ifdef DEMOS
	"Demos",
#endif
	//"Game", //"Help", F1
};

//  Demos
#ifdef DEMOS
const char* strDemo[D_All] =
{
	"Plasma", "Wave", "Hedrons", "CK Logo",
	"Space", "Balls", "Rain", "Fountain",
	"N-gons", "Fonts",
};
#endif

//  Testing kbd
const char* strTest[T_All] =
{
	"Pressed", "Matrix", "Scan Setup", "Mouse",
};

//  sub pages
const uint8_t YM1[M_All] =
{
	T_All,  //M_Testing=0,
	4,  //M_Mapping,
	1,  //M_Sequences,

	1,  //M_Display,
#ifdef DEMOS
	D_All,  // M_Demos,
#endif
};
