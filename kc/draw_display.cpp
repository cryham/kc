#include "gui.h"
#include "kc_data.h"
#include "Ada4_ST7735.h"


const char* sPgDisplay[Di_All] = {
	"Bright", "Gui keys", "Intervals", "Graph""\x01""C", "Debug" };

//  Display
//....................................................................................
void Gui::DrawDisplay()
{		
	char a[64];
	d->setClr(29,28,6);
	d->print(strMain[ym]);  d->setFont(0);

	//  subtitle
	d->setCursor(W/2 -6, 4);
	d->setClr(30,22,12);
	d->print(sPgDisplay[pgDisp]);

	//  page
	d->setCursor(W-1 -3*6, 4);
	d->setClr(28,22,10);
	sprintf(a,"%d/%d", pgDisp+1, Di_All);
	d->print(a);

	//  par values  ---
	int pg = DispPages[pgDisp];
	int16_t y = 32;
	switch (pgDisp)
	{
	case Di_Bright:
	for (int i=0; i <= pg; ++i)
	{
		DrawDispCur(i, y);
		int8_t h = 4;
		switch(i)
		{
		case 0:
			sprintf(a,"Brightness: %d %%", par.brightness);  h = 2;  break;
		case 1:
			sprintf(a,"Off bright: %d %%", par.brightOff);  break;
		case 2:
			sprintf(a,"Start with: %s", StrScreen(par.startScreen));  break;
		}
		d->print(a);  y += h+8;
	}	break;

	case Di_Key:
	for (int i=0; i <= pg; ++i)
	{
		DrawDispCur(i, y);
		int8_t h = 4;
		switch(i)
		{
		case 0:
			sprintf(a,"Key delay:  %d ms", par.krDelay*5);  h = 2;  break;
		case 1:
			sprintf(a,"Key repeat: %d ms", par.krRepeat*5);  break;
		}
		d->print(a);  y += h+8;
	}	break;

	case Di_Stats:
	for (int i=0; i <= pg; ++i)
	{
		DrawDispCur(i, y);
		int8_t h = 4;
		switch(i)
		{
		case 0:
			sprintf(a,"Time for 1min:  %dm%02ds", t1min(par)/60, t1min(par)%60);  break;
		case 1:
			sprintf(a,"Inactive after: %d min", par.minInactive);  break;
		}
		d->print(a);  y += h+8;
	}	break;

	case Di_Graph:
	for (int i=0; i <= pg; ++i)
	{
		DrawDispCur(i, y);
		int8_t h = 4;
		switch(i)
		{
		case 0:
			d->print("Temp read:  ");  PrintInterval(tTemp(par));  h = 2;  break;
		case 1:
			d->print("Graph add:  ");  PrintInterval(tTgraph(par));  break;
		case 2:
			sprintf(a,"T min:  %d ""\x01""C", par.minTemp);  d->print(a);  h = 2;  break;
		case 3:
			sprintf(a,"T max:  %d ""\x01""C", par.maxTemp);  d->print(a);  break;
		}
		y += h+8;
	}	break;

	case Di_Debug:
	for (int i=0; i <= pg; ++i)
	{
		DrawDispCur(i, y);
		int8_t h = 4;
		switch(i)
		{
		case 0:
			sprintf(a,"Ram info: %d", iRam);  h = 2;  break;
		case 1:
			sprintf(a,"Frames per sec: %d", demos.iFps);  break;
		case 2:
			sprintf(a,"Temp offset: ");  break;
		}
		d->print(a);  y += h+8;
		if (i==2)
		{
			dtostrf(0.03f * par.tempOfs, 4,2, a);
			d->print(a);  d->print(" ""\x01""C");
		}
	}	break;
	}
}
