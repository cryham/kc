#include "def.h"


//  Main Menu
const char* strMain[M_All] =
{
	"Testing", "Mappings", "Sequences",
	"Display", /*"Clock",*/ "Demos", /*"Game",*/ //"Help", F1
};

//  Demos
const char* strDemo[D_All] =
{
	"Plasma", "Wave", "Hedrons", "CK Logo",
	"Space", "Balls", "Rain", "Fountain",
	"N-gons", "Fonts",
};

//  Testing kbd
const char* strTest[T_All] =
{
	"Scan Setup", "Test Matrix", "Pressed", "Mouse",
};

//  sub pages
const uint8_t YM2[M_All] =
{
	T_All,  //M_Testing=0,
	1,  //M_Mappings,
	1,  //M_Sequences,

	1,  //M_Display,
	D_All,  // M_Demos,
};
