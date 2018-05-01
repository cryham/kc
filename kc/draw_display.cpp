#include "gui.h"
#include "Ada4_ST7735.h"


//  Display
//....................................................................................
void Gui::DrawDisplay()
{		
	char a[64];
	d->setTextColor(RGB(29,28,6));
	d->print(strMain[ym]);  //d->setFont(0);

	//  time  ---
	tm = rtc_get();
	if (tm)
	{	int h = tm/3600%24, m = tm/60%60, s = tm%60;

		d->setCursor(W/2, H-13);
		d->setTextColor(RGB(16,24,8));

		sprintf(a,"%2d:%02d:%02d", h,m,s);
		d->print(a);
	}
	d->setFont(0);

	//  page
	d->setCursor(W-1 -3*6, 4);
	d->setTextColor(RGB(30,22,12));
	sprintf(a,"%d/%d", pgDisp+1, 2);
	d->print(a);

	//  par values  ---
	int pg = DispPages[pgDisp];
	int16_t y = 32;
	switch (pgDisp)
	{
	case 0:
	for (int i=0; i < pg; ++i)
	{
		d->setCursor(2, y);
		int c = abs(i - ym2Disp);  // dist dim
		if (!c)
		{	d->fillRect(0, y-1, W-1, 10, RGB(8,8,4));
			d->setTextColor(RGB(31,22,6));
			d->print("\x10 ");  // >
		}else
			d->print("  ");

		FadeClr(C_Disp, 4, c, 1);
		int8_t h = 4;
		switch(i)
		{
		case 0:
			sprintf(a,"Brightness: %d %%", par.brightness);  break;
		case 1:
			sprintf(a,"Off bright: %d %%", par.brightOff);  break;
		case 2:
			sprintf(a,"Fade time-: %d", par.fadeTime);  h = 2;  break;
		case 3:
			sprintf(a,"Start screen: %s", StrScreen(par.startScreen));  break;
		}
		d->print(a);  y += h+8;
	}	break;

	case 1:
	for (int i=0; i < pg; ++i)
	{
		d->setCursor(2, y);
		int c = abs(i - ym2Disp);  // dist dim
		if (!c)
		{	d->fillRect(0, y-1, W-1, 10, RGB(8,8,4));
			d->setTextColor(RGB(31,22,6));
			d->print("\x10 ");  // >
		}else
			d->print("  ");

		FadeClr(C_Disp, 4, c, 1);
		int8_t h = 4;
		switch(i)
		{
		case 0:
			sprintf(a,"Key delay:  %d ms", par.krDelay*5);  h = 2;  break;
		case 1:
			sprintf(a,"Key repeat: %d ms", par.krRepeat*5);  break;
		case 2:
			sprintf(a,"Ram info: %d", iRam);  break;

		//uint8_t mkSpeed, mkAccel;
		//start demo time
		}
		d->print(a);  y += h+8;
	}	break;
	}
}

const uint8_t Gui::DispPages[Gui::Disp_All] = {4,3};
