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
		d->setClr(21,23,23);
		d->print(strMain[ym]);  d->setFont(0);

		DrawMenu(S_All,strSetup, C_Setup,RGB(18,24,22),RGB(4,6,6), 10, -1, 2);
		pressGui = 0;
		return;
	}
	char a[64];
	int16_t y = 32;


	if (yy == S_Version)
	{	demos.Version();  return;  }

	//  title
	d->setClr(17,22,22);
	d->print(strSetup[yy]);
	d->setFont(0);
	d->setClr(21,26,26);


	int ii = ScanPages[yy];
	switch (yy)
	{
	//-----------------------------------------------------
	case S_Keyboard:
	{
		for (int i=0; i <= ii; ++i)
		{
			d->setCursor(2,y);
			int c = abs(i - ym2Keyb);
			if (!c)
			{	d->setClr(30,25,20);
				d->fillRect(0, y-1, W-1, 10, RGB(3,6,6));
				d->print("\x10 ");  // >
			}else
				d->print("  ");

			FadeClr(C_Setup2, 4, c, 1);
			switch(i)
			{
			case 0:
				sprintf(a,"Default layer: %d", par.defLayer);  y-=2;  break;
			case 1:  // todo warning layer empty
				sprintf(a,"Sequence delay: %d ms", par.dtSeqDef);  break;

			case 2:  // todo warning seq key not bound
				sprintf(a,"Seq. edit layer: %d", par.editLayer);  y-=2;  break;
			case 3:
				if (pressGui)
					sprintf(a,"Gui toggle Key: Press ..");
				else
					sprintf(a,"Gui toggle Key: %d", par.keyGui);  break;
			case 4:
				d->print("Layer\\ Fast max: ");
				dtostrf(par.msLLTapMax*0.01f, 4,2, a);  d->print(a);
				d->print(" s");
				y-=2;  break;
			case 5:
				d->print(" Lock/ Hold min: ");
				dtostrf(par.msLLHoldMin*0.1f, 3,1, a);  d->print(a);
				d->print(" s");
				y+=2;  break;
			}
			if (i < 4)
				d->print(a);  y += 8+4;
		}
	}	break;

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

		d->setCursor(0,H-10);
		sprintf(a,"Layout keys: %d  %s", nDrawKeys, CKname);
		d->println(a);

	}	break;


	//-----------------------------------------------------
	case S_Mouse:
	{
		int16_t x=2;
		for (int i=0; i <= ii; ++i)
		{
			d->setCursor(x,y);
			int c = abs(i - ym2Mouse);
			if (!c)
			{	d->setClr(15,23,30);
				d->fillRect(x, y-1, W/2-1, 10, RGB(3,5,6));
				d->print("\x10 ");  // >
			}else
				d->print("  ");

			FadeClr(C_Setup2, 4, c, 1);
			switch(i)
			{
			case 0:
				sprintf(a,"Speed: %3d  Move", par.mkSpeed);  break;
			case 1:
				sprintf(a,"Accel: %3d", par.mkAccel);  break;
			case 2:
				if (pressGui)
					sprintf(a,"Slow key: Press ..");
				else
					sprintf(a,"Slow key: %d", par.keyMouseSlow);
				//x = W/2-1;  y = 32-8;  // next column
				break;
			case 3:
				sprintf(a,"Speed: %3d  Wheel", par.mkWhSpeed);  break;
			case 4:
				sprintf(a,"Accel: %3d", par.mkWhAccel);  break;
			}
			d->print(a);  y += 8+1;
		}

		///  dbg  mouse accel  --
		const int16_t x0 = 0, x1 = W/3+6, x2 = 2*W/3+6;
		y = H-1 - 2*8 - 2*(8+2);
		d->setClr(16,20,24);
		d->setCursor(0, y);     d->print("hold");
		d->setCursor(W/3, y);   d->print("delay");
		d->setCursor(2*W/3, y); d->print("speed");  y+= 8+2;

		d->setCursor(x0,y);  dtostrf(mx_holdtime, 4,2, a);  d->print(a);
		d->setCursor(x1,y);  sprintf(a,"%d", mx_delay);  d->print(a);
		d->setCursor(x2,y);  sprintf(a,"%d", mx_speed);  d->print(a);  y += 8;

		d->setCursor(x0,y);  dtostrf(my_holdtime, 4,2, a);  d->print(a);
		d->setCursor(x1,y);  sprintf(a,"%d", my_delay);  d->print(a);
		d->setCursor(x2,y);  sprintf(a,"%d", my_speed);  d->print(a);  y += 8+2;

		const static char ch[3]={'-',' ','+'};  // input status
		#define Ch(v)  ch[max(0, min(2, v))]
		d->setCursor(0, H-1-8);  sprintf(a,"m x%cy%c wh x%cy%c bt %d",
			Ch(Mouse_input_x/8+1), Ch(Mouse_input_y/8+1),
			Ch(Mouse_wheel_x+1), Ch(Mouse_wheel_y+1), usb_mouse_buttons_state);  d->print(a);
	}	break;

	//-----------------------------------------------------
	case S_Info:  // use
	{
		d->setClr(15,23,30);
		d->setCursor(0, y);
		d->print("\x10 ");  // >

		d->setClr(16,20,24);
		sprintf(a,"Save counter: %d", par.verCounter);
		d->print(a);  y += 12;

		d->setClr(15,22,29);
		int i, s = 0, t = 0;

		//  count seqs
		for (i=0; i < KC_MaxSeqs; ++i)
			if (kc.set.seqs[i].len() > 0)  ++s;

		sprintf(a,"Used Sequences:  %d /%d", s, KC_MaxSeqs);
		d->setCursor(0, y);  d->print(a);  y+=10;

		//  count layers, keys
		s = 0;
		for (i=0; i < KC_MaxLayers; ++i)
		{	bool b = false;
			for (int k=0; k < kc.set.nkeys(); ++k)
				if (kc.set.key[i][k] != KEY_NONE)
				{	++t;  b = true;  }
			if (b)  ++s;
		}

		sprintf(a,"   Used Layers:  %d /%d", s, KC_MaxLayers);
		d->setCursor(0, y);  d->print(a);  y+=10;

		sprintf(a,"   Total keys:  %d", t);
		d->setCursor(0, y);  d->print(a);  y+=16;

		//  usb const  --
		d->setClr(22,22,25);
		d->setCursor(0, y);  d->print("USB const");  y+=10;
		d->setClr(18,18,22);

		sprintf(a,"All %d  Ext %d", KEYS_ALL, KEYS_ALL_EXT);
		d->setCursor(0, y);  d->print(a);  y+=10;

		sprintf(a,"Lay %d  Fun %d", K_Layer1, K_Fun0);
		d->setCursor(0, y);  d->print(a);  y+=10;

		sprintf(a,"Med %d  Pow %d", K_F24+1, KS_POWER_DOWN);
		d->setCursor(0, y);  d->print(a);  y+=10;
	}	break;
	}
}
