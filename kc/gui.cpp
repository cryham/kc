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
	kbdSend = 0;
	//menu = 1;
	mlevel = 1;

//	ym = M_Testing;
	ym = M_Mapping;
//	ym = M_Sequences;
//	ym = M_Display;
	for (int i=0; i < M_All; ++i)
		ym1[i]=0;
	ym1[M_Testing]=3;
	
	oldti = 0;
	tdemo = 0;
#ifdef DEMOS
	demos.Init(d);
#endif

	nLay=0;  scId=0;  drawId=-1;
	keyCode=0;
	pressKey=0;  moveCur=0;  pickCode=0;

	edit = 0;  edins = 1;
	slot=0; page=0; edpos=0;


	//  init Group Starts and Ends
	for (int g=0; g < grpMax; ++g)
	{
		grpStart[g] = 0;  // all
		grpEnd[g] = KEYS_ALL_EXT;

		//  find first and last key, for this group
		for (int i=0; i < KEYS_ALL_EXT; ++i)
			if (cKeyGrp[i] == g)
			{
				if (grpStart[g] == 0)  grpStart[g] = i;
				grpEnd[g] = i;
			}
	}
}
