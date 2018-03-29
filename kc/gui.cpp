#include "gui.h"
#include "def.h"


//  Main  ----
Gui::Gui() : d(0)
{
	Init(0);
}

void Gui::Init(Ada4_ST7735* tft)
{
	d = tft;
	//menu = 1;
	mlevel = 0;

	ym = 0;
	for (int i=0; i < M_All; ++i)
		ym2[i]=0;
	
	oldti = 0;
	tdemo = 0;
	demos.Init(d);
}
