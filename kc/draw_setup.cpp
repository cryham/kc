#include "gui.h"
#include "Ada4_ST7735.h"
#include "usb_keyboard.h"
#include "matrix.h"
#include "kbd_layout.h"
#include "kc_data.h"
#include "keys_usb.h"
#include "usb_mouse.h"

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
		d->setTextColor(RGB(21,23,23));
		d->print(strMain[ym]);  d->setFont(0);

		DrawMenu(S_All,strSetup, C_Setup,RGB(18,24,22),RGB(5,7,6), 10, -1, 2);
		return;
	}
	char a[64];


	if (yy == S_Version)
	{	demos.Version();  return;  }

	//  title
	d->setTextColor(RGB(16,24,20));
	d->print(strSetup[yy]);
	d->setFont(0);
	d->setTextColor(RGB(21,26,26));


	switch (yy)
	{
	//-----------------------------------------------------
	case S_Keyboard:
	{
		int16_t y=32;
		for (int i=0; i < 3; ++i)
		{
			d->setCursor(2,y);
			int c = abs(i - ym2Keyb);
			if (!c)
			{	d->setTextColor(RGB(10,30,30));
				d->fillRect(0, y-1, W-1, 10, RGB(3,6,6));
				d->print("\x10 ");  // >
			}else
				d->print("  ");

			FadeClr(C_Setup2, 4, c, 1);
			switch(i)
			{
			case 0:
				sprintf(a,"Sequence delay: %d ms", par.dtSeqDef);  break;
			case 1:
				sprintf(a,"Default layer: %d", par.defLayer);  break;
			case 2:
				sprintf(a,"Seq. edit layer: %d", par.editLayer);  break;
			}
			d->print(a);  y += 8+4;
		}
	}	break;

	//-----------------------------------------------------
	case S_Scan:
	{
		int16_t y=32;
		for (int i=0; i < 3; ++i)
		{
			d->setCursor(2,y);
			int c = abs(i - ym2Scan);
			if (!c)
			{	d->setTextColor(RGB(10,30,30));
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

		d->setCursor(2,H-35);
		d->setTextColor(RGB(22,23,23));
		sprintf(a,"  Time: %lu us  %u Hz", us_scan, scan_freq);
		d->println(a);  d->moveCursor(0,8);
		d->setTextColor(RGB(20,23,26));

		d->setCursor(0,H-20);
		sprintf(a,"Matrix keys: %d = %d x %d", ScanKeys, NumCols, NumRows);
		d->println(a);

		d->setCursor(0,H-10);
		sprintf(a,"Layout keys: %d  %s", nDrawKeys, CKname);
		d->println(a);

	}	break;


	//-----------------------------------------------------
	case S_Mouse:
	{
		int16_t y=32;
		for (int i=0; i < 2; ++i)
		{
			d->setCursor(2,y);
			int c = abs(i - ym2Mouse);
			if (!c)
			{	d->setTextColor(RGB(10,20,30));
				d->fillRect(0, y-1, W-1, 10, RGB(3,5,6));
				d->print("\x10 ");  // >
			}else
				d->print("  ");

			FadeClr(C_Setup2, 4, c, 1);
			switch(i)
			{
			case 0:
				sprintf(a,"Speed: %d", par.mkSpeed);  break;
			case 1:
				sprintf(a,"Accel: %d", par.mkAccel);  break;
			}
			d->print(a);  y += 8+4;
		}

		///  dbg  mouse accel  --
		const int16_t x0 = 0, x1 = W/3+6, x2 = 2*W/3+6;
		const int16_t y0 = 66, y1 = y0+10+2, y2 = y1+10;

		d->setTextColor(RGB(20,24,28));
		d->setCursor(0, y0);     d->print("hold");
		d->setCursor(W/3, y0);   d->print("delay");
		d->setCursor(2*W/3, y0); d->print("speed");

		d->setCursor(x0, y1);  dtostrf(mx_holdtime, 4,2, a);  d->print(a);
		d->setCursor(x0, y2);  dtostrf(my_holdtime, 4,2, a);  d->print(a);

		d->setCursor(x1, y1);  sprintf(a,"%d", mx_delay);  d->print(a);
		d->setCursor(x1, y2);  sprintf(a,"%d", my_delay);  d->print(a);

		d->setCursor(x2, y1);  sprintf(a,"%d", mx_speed);  d->print(a);
		d->setCursor(x2, y2);  sprintf(a,"%d", my_speed);  d->print(a);

		d->setCursor(0, H-1-18);  sprintf(a,"move  x %+d  y %+d",
			Mouse_input_x/8, Mouse_input_y/8);  d->print(a);
		d->setCursor(0, H-1-8);  sprintf(a,"buttons %d  wh- x %d y %d",
			usb_mouse_buttons_state, Mouse_wheel_x, Mouse_wheel_y);  d->print(a);
	}	break;
	}
}
