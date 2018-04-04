#include "gui.h"
#include "Ada4_ST7735.h"
#include "usb_keyboard.h"
#include "matrix.h"
#include "kbd_layout.h"
#include "kc_data.h"
#include "keys_usb.h"

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
		d->print(strMain[ym]);  d->setFont(0);

		//  menu
		DrawMenu(T_All,strTest,2);

		return;
	}

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
		d->print(a);
		d->println("");  d->moveCursor(0,4);

		//  locks  -
		d->setTextColor(RGB(18,21,24));
		d->print("Locks: ");  // Num Caps Scrl
		d->println("");  d->moveCursor(0,2);

		//  modifiers  -
		d->setTextColor(RGB(18,21,24));
		d->print("Modif:  ");
		#if 0
		for (int i=KEY_LCTRL; i<=KEY_RGUI; ++i)
		if (kk[i])
		{	d->print(i >= KEY_RCTRL ? "R" : "L");
			d->print(mod[i-KEY_LCTRL]);  d->print(" ");
		}
		#endif
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
				if (code > KEY_NONE && code < KEYS_ALL)
				{	sprintf(a,"%s ",cKeyStr[code]);
					d->print(a);
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
		d->setTextColor(RGB(23,26,29));
		sprintf(a,"Scan: %u Hz  t %lu us", scan_freq, us_scan);
		d->println(a);  d->moveCursor(0,4);
		//d->setTextColor(RGB(24,28,31));

		//d->print("Protocol: ");  //d->println(USBKeys_Protocol == 1 ? " NKRO" : " Boot");
		//d->moveCursor(0,4);

		sprintf(a,"Strobe delay: %d us", STROBE_DELAY);
		d->println(a);  d->moveCursor(0,4);

		//DebounceThrottleDiv_define-
		sprintf(a,"Debounce: %d ms", MinDebounceTime_define);
		d->println(a);  d->moveCursor(0,8);

		d->setTextColor(RGB(20,23,26));
		sprintf(a,"Matrix keys: %d = %d x %d", ScanKeys, NumCols, NumRows);
		d->println(a);  d->moveCursor(0,2);

		sprintf(a,"Layout keys: %d  %s", nDrawKeys, CKname);
		d->println(a);  d->moveCursor(0,4);

	}	break;


	//-----------------------------------------------------
	case T_Mouse:
	{
		///  dbg  mouse speed  --
		d->setCursor(0,32);
		//const int16_t y1 = H-1-2*8, y2 = H-1-1*8;
		d->print("hold  delay  speed");
		#if 0
		d->setCursor(0, y1);  d->print(mx_holdtime);
		d->setCursor(0, y2);  d->print(my_holdtime);

		d->setCursor(W/3, y1);  d->print(mx_delay);
		d->setCursor(W/3, y2);  d->print(my_delay);

		d->setCursor(2*W/3, y1);  d->print(mx_speed);
		d->setCursor(2*W/3, y2);  d->print(my_speed);
		#endif
	}	break;
	}
}
