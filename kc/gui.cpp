#include "gui.h"

///-----------------  //par  demo counts
const uint8_t Gui::YM2[M_All] = { 4, 6, 4, 5 };


//  Main  ----
Gui::Gui()
{
	Init();
}

void Gui::Init()
{
	menu = 1;
	mlevel = 0;

	ym = 0;
	for (int i=0; i < M_All; ++i)
		ym2[i]=0;
	
	//help = 0;  hPage = 0;
	testPage = 0;
}
