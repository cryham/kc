#include "gui.h"
#include "Ada4_ST7735.h"
#include "usb_keyboard.h"
#include "matrix.h"
#include "kc_data.h"
//#include "usb_mouse.h"

//  extern
extern uint scan_freq;  // scan counter, freq
extern uint32_t us_scan;


//  Setup
//....................................................................................
void Gui::DrawSetup()
{
	//  menu
	if (mlevel == 1)
	{
		d->setClr(21,23,23);
		d->print(strMain[ym]);  d->setFont(0);

		DrawMenu(S_All,strSetup, C_Setup,RGB(18,24,22),RGB(4,6,6), 10);
		pressGui = 0;
		return;
	}
	char a[64];
	int16_t y = 32;


	//  title
	d->setClr(17,22,22);
	d->print(strSetup[yy]);
	d->setFont(0);
	d->setClr(21,26,26);


	int ii = ScanPages[yy];
	switch (yy)
	{

	//-----------------------------------------------------
	case S_Scan:
	{
		for (int i=0; i <= ii; ++i)
		{
			d->setCursor(2,y);
			int c = abs(i - ym2Scan);
			if (!c)
			{	d->setClr(10,30,30);
				d->fillRect(0, y-1, W-1, 10, RGB(3,6,6));
				d->print("\x10 ");  // >
			}else
				d->print("  ");

			FadeClr(C_Setup2, 4, c, 1);
			switch(i)
			{
			case 0:
				sprintf(a,"Scan: %u Hz", F_BUS/par.scanFreq/1000);  break;
			case 1:
				sprintf(a,"Strobe delay: %d us", par.strobe_delay);  break;
			case 2:
				sprintf(a,"Debounce: %d ms", par.debounce);  break;
			}
			d->print(a);  y += 8+4;
		}

		d->setClr(22,23,23);
		d->setCursor(W-1-6*6,0);
		d->print("Fps");

		d->setCursor(2,H-42);
		sprintf(a,"  Time: %lu us  %u Hz", us_scan, scan_freq);
		d->println(a);  d->moveCursor(0,8);
		d->setClr(20,23,26);

		d->setCursor(0,H-20);
		sprintf(a,"Matrix keys: %d = %d x %d", ScanKeys, NumCols, NumRows);
		d->println(a);

	}	break;

	}
}
