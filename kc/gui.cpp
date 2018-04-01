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
	mlevel = 1;

	ym = 1;
	for (int i=0; i < M_All; ++i)
		ym1[i]=0;
	
	oldti = 0;
	tdemo = 0;
#ifdef DEMOS
	demos.Init(d);
#endif

	nLay=0;  scId=0;  drawId=-1;
	keyCode=0;
	pressKey=0;  moveCur=0;  pickCode=0;

	krDelay=250;  krRepeat=80;  // ms
}
