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
const static int rr = NumCols;
extern uint32_t us_scan;

extern KC_Main kc;


//  Testing
//....................................................................................
void Gui::DrawTesting()
{
	if (mlevel == 1)
	{
		d->setTextColor(RGB(18,26,18));
		d->print(strMain[ym]);  d->setFont(0);

		//  menu
		DrawMenu(T_All,strTest, C_Test,RGB(22,31,14));

		return;
	}
	char a[64];

	//  title
	d->print(strTest[yy]);
	d->setFont(0);
	d->setTextColor(RGB(21,26,31));

	switch (yy)
	{
	//-----------------------------------------------------
	case T_Pressed:
	{
		d->setCursor(0,32);
		//  layer  -
		d->setTextColor(RGB(20,23,31));
		sprintf(a,"Layer: %d", kc.nLayer);
		d->setTextColor(RGB(24,27,31));
		d->print(a);
		d->println("");  d->moveCursor(0,4);

		//  locks  -
		d->setTextColor(RGB(18,21,24));
		d->print("Locks: ");  // Num Caps Scrl
		d->println("");  d->moveCursor(0,2);

		//  modifiers  -
		d->setTextColor(RGB(18,21,24));
		d->print("Modif:  ");
		d->setTextColor(RGB(20,25,28));
		for (uint i = 0; i < ScanKeys; ++i)
		{
			const KeyState& k = Matrix_scanArray[i];
			if (kc.set.nkeys() >= int(ScanKeys))
			if (k.state == KeyState_Hold)
			{
				int8_t code = kc.set.keys[i].get(kc.nLayer);
				if (code > KEY_NONE && code <= K_ModLast)
				{	sprintf(a,"%s ",cKeyStr[code]);
					d->print(a);
		}	}	}
		d->println("");  d->moveCursor(0,4);


		//  scan codes  - - -
		d->setTextColor(RGB(17,23,26));
		d->print("Scan: ");
		d->setTextColor(RGB(15,20,25));
		int c = 0;
		for (uint i = 0; i < ScanKeys; ++i)
		{
			const KeyState& k = Matrix_scanArray[i];
			if (k.state == KeyState_Hold)
			{
				sprintf(a,"%d ",i);  // scan code
				d->print(a);  ++c;
		}	}
		d->println("");  d->moveCursor(0,4);

		//  keys  - - - -
		d->setTextColor(RGB(20,25,28));
		d->print("Keys: ");
		d->setTextColor(RGB(24,28,31));
		for (uint i = 0; i < ScanKeys; ++i)
		{
			const KeyState& k = Matrix_scanArray[i];
			if (kc.set.nkeys() >= int(ScanKeys))
			if (k.state == KeyState_Hold)
			{
				int8_t code = kc.set.keys[i].get(kc.nLayer);
				if (code > K_ModLast && code < KEYS_ALL)
				{
					const uint8_t* c = &cGrpRgb[cKeyGrp[code]][0][0];
					d->setTextColor(RGB(c[0],c[1],c[2]));
					d->print(cKeyStr[code]);  d->print(" ");
		}	}	}

		//  held count
		d->setCursor(0, H-1-20);
		d->setTextColor(RGB(16+c, min(31,24+c), 31));
		sprintf(a,"Held: %d", c);
		d->print(a);

		if (ghost_cols)  // ghost
		{	d->setTextColor(RGB(min(31,24+c), 18+c, 31));
			sprintf(a,"  Ghost: %d %d", ghost_cols, ghost_rows);
			d->print(a);
		}
		//  press count -
		d->setCursor(0, H-1-8);
		d->setTextColor(RGB(21,21,27));
		sprintf(a,"Press: %d", cnt_press); //, cnt_hold % 1000);
		d->print(a);

	}	break;


	//-----------------------------------------------------
	case T_Layout:
	{
		DrawLayout(false);
	}	break;


	//-----------------------------------------------------
	case T_Matrix:
	{
		//  matrix  :::
		const static int16_t x1 = 12, y1 = 64;
		uint c,r;

		bool ghost = false;
		for (c=0; c < NumCols; ++c)
		for (r=0; r < NumRows; ++r)
		{
			d->setCursor(x1 + c*8, y1 + r*8);
			const KeyState& k = Matrix_scanArray[NumCols * r + c];

			//  color from ghost, use
			#define CGh(gh,u)  d->setTextColor(gh ? RGB(31,26,12) : \
				RGB( min(31,7+u*5), min(31, 14+u*8), max(4, 24-u*2) ))

			a[1]=0;
			if (k.state == KeyState_Off)
			{	//sprintf(a,".");
				a[0]=249;
				bool gh = col_ghost[c] || row_ghost[r];
				int u = max(col_use[c], row_use[r]);
				CGh(gh, u);
				ghost |= gh;
			}else{
				a[0]='*';
				//sprintf(a,"%d", k.state);
				d->setTextColor(RGB(24,28,31));
			}
			d->print(a);
		}

		//  col| row- use
		for (c=0; c < NumCols; ++c)
		{
			CGh(col_ghost[c], col_use[c]);
			d->setCursor(x1 + c*8, y1 - 1*8);
			sprintf(a,"%d", col_use[c]);
			d->print(a);
		}
		for (r=0; r < NumRows; ++r)
		{
			CGh(row_ghost[r], row_use[r]);
			d->setCursor(0, y1 + r*8);
			sprintf(a,"%d", row_use[r]);
			d->print(a);
		}

		//  held  ---
		d->setCursor(0,26);
		d->setTextColor(RGB(24,24,31));
		sprintf(a,"Held %d  press %d", cnt_press-cnt_rel, cnt_press);
		d->print(a);

		//  ghosting  ---
		d->setCursor(0,40);
		d->setTextColor(ghost ? RGB(31,26,12) : RGB(16,21,26));
		//sprintf(a,"Ghost col %d row %d  K%d", ghost_cols, ghost_rows, keys);
		sprintf(a,"Ghost col %d row %d", ghost_cols, ghost_rows);
		d->print(a);

	}	break;

	//-----------------------------------------------------
	case T_ScanSetup:
	{
		d->setCursor(0,32);
		for (int i=0; i < 4; ++i)
		{
			int c = abs(i - ym2Test);
			if (!c)
			{	d->setTextColor(RGB(10,30,30));
				d->print("\x10 ");  // >
			}else
				d->print("  ");

			FadeClr(C_Scan, 4, c, 1);
			switch(i)
			{
			case 0:
				sprintf(a,"Scan: %u Hz", F_BUS/par.scanFreq/1000);  break;
			case 1:
				sprintf(a,"Strobe delay: %d us", par.strobe_delay);  break;
			case 2:
				sprintf(a,"Debounce: %d ms", par.debounce);  break;
			case 3:
				sprintf(a,"Sequence delay: %d ms", par.dtSeqDef);  break;
			}
			d->println(a);  d->moveCursor(0,4);
		}
		d->moveCursor(0,4);
		//DebounceThrottleDiv_define-
		d->setTextColor(RGB(20,23,23));
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
	case T_Mouse:
	{
		///  dbg  mouse accel  --
		const int16_t x0 = 0, x1 = W/3+6, x2 = 2*W/3+6;
		const int16_t y0 = 32, y1 = y0+10+4, y2 = y1+10;

		d->setCursor(0, y0);     d->print("hold");
		d->setCursor(W/3, y0);   d->print("delay");
		d->setCursor(2*W/3, y0); d->print("speed");

		d->setCursor(x0, y1);  dtostrf(mx_holdtime, 4,2, a);  d->print(a);
		d->setCursor(x0, y2);  dtostrf(my_holdtime, 4,2, a);  d->print(a);

		d->setCursor(x1, y1);  sprintf(a,"%d", mx_delay);  d->print(a);
		d->setCursor(x1, y2);  sprintf(a,"%d", my_delay);  d->print(a);

		d->setCursor(x2, y1);  sprintf(a,"%d", mx_speed);  d->print(a);
		d->setCursor(x2, y2);  sprintf(a,"%d", my_speed);  d->print(a);

		d->setCursor(0, H-1-4*8);  sprintf(a,"move  x %+d  y %+d",
			Mouse_input_x/8, Mouse_input_y/8);  d->print(a);
		d->setCursor(0, H-1-2*8);  sprintf(a,"buttons %d  wheel x %d y %d",
			usb_mouse_buttons_state, Mouse_wheel_x, Mouse_wheel_y);  d->print(a);
	}	break;
	}
}
