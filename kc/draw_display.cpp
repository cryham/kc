#include "gui.h"
#include "kc_data.h"
#include "Ada4_ST7735.h"


//  Display
//....................................................................................
void Gui::DrawDispCur(int i, int16_t y)
{
	d->setCursor(2, y);
	int c = abs(i - ym2Disp);  // dist dim
	if (!c)
	{	d->fillRect(0, y-1, W-1, 10, RGB(8,8,4));
		d->setClr(31,22,6);
		d->print("\x10 ");  // >
	}else
		d->print("  ");

	FadeClr(C_Disp, 4, c, 1);
}

//  date from day number
uint8_t MDays[13]= {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

inline bool isLeap(int y)
{
	return y%400==0 || (y%4==0 && y%100!=0);
}

//  days in month 1..12
int getMthDays(bool leap, int m)
{
	int md = MDays[m];
	if (leap && m == 2)
		md = 29;
	return md;
}

//  day in year 0..365
void getMD(bool leap, int day, int* mday, int* mth)
{
	int m=1;
	while (day >= getMthDays(leap, m))
	{
		day -= getMthDays(leap, m);  ++m;
	}
	*mday = day+1;  *mth = m;
}


void Gui::DrawDisplay()
{		
	char a[64];
	d->setClr(29,28,6);
	d->print(strMain[ym]);  //d->setFont(0);

	//  time  ---
	unsigned long t = rtc_get();
	if (t)
	{	int h = t/3600%24, m = t/60%60, s = t%60,
			dt = t/3600/24, yr = dt/365 +2000;

		d->setCursor(W/2, H-13);
		d->setClr(16,24,8);

		sprintf(a,"%2d:%02d:%02d", h, m, s);
		d->print(a);
		d->setFont(0);

		int mth=0, day=0;
		getMD(isLeap(yr), dt%365, &day, &mth);
		d->setCursor(0, H-1-8);  // date
		sprintf(a,"%04d %02d %02d", yr, mth, day);
		d->print(a);

		//  time since on
		t -= tm_on;
		dt = t/3600/24; h = t/3600%24; m = t/60%60; s = t%60;

		d->setCursor(W/2, H-13-10);
		sprintf(a,"%d %2d:%02d:%02d", dt%10, h, m, s);
		d->print(a);
	}
	d->setFont(0);

	//  page
	d->setCursor(W-1 -3*6, 4);
	d->setClr(30,22,12);
	sprintf(a,"%d/%d", pgDisp+1, 3);
	d->print(a);

	//  par values  ---
	int pg = DispPages[pgDisp];
	int16_t y = 32;
	switch (pgDisp)
	{
	case 0:
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
			sprintf(a,"Start screen: %s", StrScreen(par.startScreen));  break;
		}
		d->print(a);  y += h+8;
	}	break;

	case 1:
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
		case 2:
			sprintf(a,"Ram info: %d", iRam);  h = 2;  break;
		case 3:
			sprintf(a,"Frames per sec: %d", demos.iFps);  break;
		case 4:
			sprintf(a,"Quick keys: %d", par.quickKeys);  break;
		}
		d->print(a);  y += h+8;
	}	break;

	case 2:
		for (int i=0; i <= pg; ++i)
		{
			DrawDispCur(i, y);
			int8_t h = 2;
			switch(i)
			{
			case 0:
				sprintf(a,"Hour");  break;
			case 1:
				sprintf(a,"Minute");  break;
			case 2:
				sprintf(a,"Second");  h = 4;  break;
			case 3:
				sprintf(a,"Day");  break;
			case 4:
				sprintf(a,"Month");  break;
			case 5:
				sprintf(a,"Year");  break;
			}
			d->print(a);  y += h+8;
		}	break;
	}
}
