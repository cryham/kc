#include "gui.h"
#include "Ada4_ST7735.h"
#include "FreeSans9pt7b.h"

#include "usb_keyboard.h"
#include "matrix.h"


extern uint scan_freq;  // scan counter, freq
const static int rr = NumCols;
extern uint32_t us_scan;

#include "kbd_layout.h"


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
	case T_Matrix:
	{
		//  matrix  :::
		const static int16_t x1 = 12, y1 = 64;

		bool ghost = false;
		for (uint c=0; c < NumCols; ++c)
		for (uint r=0; r < NumRows; ++r)
		{
			d->setCursor(x1 + c*8, y1 + r*8);
			const KeyState& k = Matrix_scanArray[NumCols * r + c];

			#define CGh(b)  d->setTextColor(b ? RGB(31,26,12) : RGB(12,20,26))

			a[1]=0;
			if (k.state == KeyState_Off)
			{	//sprintf(a,".");
				a[0]=249;
				bool gh = col_ghost[c] || row_ghost[r];
				CGh(gh);
				ghost |= gh;
			}else{
				a[0]='*';
				//sprintf(a,"%d", k.state);
				d->setTextColor(RGB(24,28,31));
			}
			d->print(a);
		}

		//  col| row- use
		for (uint c=0; c < NumCols; ++c)
		{
			CGh(col_ghost[c]);
			d->setCursor(x1 + c*8, y1 - 1*8);
			sprintf(a,"%d", col_use[c]);
			d->print(a);
		}
		for (uint r=0; r < NumRows; ++r)
		{
			CGh(row_ghost[r]);
			d->setCursor(0, y1 + r*8);
			sprintf(a,"%d", row_use[r]);
			d->print(a);
		}

		//  held  ---
		d->setCursor(0,26);
		d->setTextColor(RGB(24,24,31));
		sprintf(a,"Held %d  press %d", cnt_press-cnt_rel, cnt_press);//, cnt_hold % 1000);
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
		sprintf(a,"Scan %u Hz  t %lu us", scan_freq, us_scan);
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
		sprintf(a,"Matrix: %d x %d", NumCols, NumRows);
		d->println(a);  d->moveCursor(0,2);

		sprintf(a,"Max keys: %d", MaxKeys);
		d->println(a);  d->moveCursor(0,2);

		sprintf(a,"Num keys: %d  %s", numKeys, CKname);
		d->println(a);  d->moveCursor(0,4);

	}	break;

	//-----------------------------------------------------
	case T_Pressed:
	{
		d->setCursor(0,32);
		//  layers  -
		d->setTextColor(RGB(20,23,31));
		d->print("Layers: ");
		#if 0
		for (int l=1; l < layersCnt; ++l)  // 1st is 1 in menu
		{	d->print(layersOn[l]);  d->moveCursor(6,0);  }
		#endif
		d->println("");  d->moveCursor(0,4);

		//Demos::Font_ver(d, true);  //*

		//  locks  -
		d->setTextColor(RGB(18,21,24));
		d->print("Locks: ");  // Num Caps Scrl
		d->println("");  d->moveCursor(0,4);

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


		//  keys  - - - -
		d->setTextColor(RGB(24,24,31));
		d->print("Keys: ");
		d->setTextColor(RGB(24,28,31));
		int c = 0;
		for (uint i = 0; i < MaxKeys; ++i)
		{
			const KeyState& k = Matrix_scanArray[i];
			if (k.state == KeyState_Hold)
			{
				sprintf(a,"%d ",i);  // scan code
				d->print(a);  ++c;
			}  //STR(i) todo: map to key name
		}
		//  count
		d->setCursor(0, H-1-2*8);
		d->setTextColor(RGB(16+c, min(31,24+c), 31));
		sprintf(a,"%d",c);
		d->print(a);

		#if 0
		if (ghost_cols /*|| ghost_rows*/)
		{
			d->setCursor(3*6, H-1-8);
			d->print("Gho |");  d->print(ghost_cols);
			d->print(" -");  d->print(ghost_rows);
		}
		#endif
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


//  Mappings kbd
//....................................................................................
void Gui::DrawMapping()
{
	d->print(strMain[ym]);  d->setFont(0);

	//  kc
	/*d->setCursor(0,32);
	for (uint i=0; i < kc.data.size(); ++i)
	{
		sprintf(a,"%d ", kc.data[i]);
		d->print(a);
	}*/

	//  kbd draw   Layout  * * * *
	int x = 0, y = 0;
	for (int i=0; i < numKeys; ++i)
	{
		const SKey& k = drawKeys[i];

		//  find if pressed
		bool f = k.sc != NO && Matrix_scanArray[k.sc].state == KeyState_Hold;

		if (k.x >=0)  x = k.x;  else  x -= k.x;
		if (k.y > 0)  y = k.y + 62; /*Y*/  else  y -= k.y;

		if (f)
			d->fillRect(x+1, y-1, k.w-1, k.h-1, clrRect[k.o]);

		uint16_t  // clr
			cR = f ? RGB(28,28,29) : clrRect[k.o],
			cT = f ? RGB(31,31,31) : clrText[k.o];  //<def
		d->drawRect(x, y-2, k.w+1, k.h+1, cR);

		d->setCursor(  // txt
			k.o==5 || k.o==7 ? x + k.w - 6 :  // right align
			(k.o==3 ? x+1 : x+2),
			k.h == kF ? y-2 :  // short,  symb 3
			(k.o==3 ? y-1 : y-1));

		d->setTextColor(cT);
		sprintf(a,"%c", k.c);
		d->print(a);
	}

	// todo: cursor [] ..

}

//  Sequences kbd view, edit
//....................................................................................
void Gui::DrawSequences()
{
	d->print(strMain[ym]);  d->setFont(0);

}


	#if 0
	if (mlevel==1 && ym == MSeq)
	if (!edit)
	{
		d->print("View");  d->setFont(0);
		
		//  list slots
		int s = page * iPage, i,n,x, xm;
		for (i=0; i < iPage && s < iSlots; ++i,++s)
		{
			d->setCursor(0, 18 + i*9);  //16 + i*8
			//d->print(i==slot ? "\x10":" ");   d->print(s);  d->print(": ");
			if (s<10)  d->print(" ");  //align
			d->print(s);  d->print(i==slot ? "\x10":" ");
			
			//  write sequence  ---
			n=0;  x=0;  xm=1;
			while (n < seql[s] && xm)
			{
				uint8_t z = seq[s][n];
				const char* st = STR(z);
				if (d->getCursorX() + 6*strlen(st) >= W -6*2)  // outside
				{	d->print(".");  d->moveCursor(-3,0);
					d->print(".");  xm = 0;  // more sign
				}else
				{	d->print(st);  //d->print(" ");
					d->moveCursor(z<=1 ? 0 : 2, 0);
					++n;
			}	}
		}
		//  page, center   / 
		d->setCursor(60, 4);
		d->print(page+1);  d->print("/");  d->print(iSlots/iPage);
		
		///  seq key
		if (tInfo == 0)
		{	int q = seqId();
			int l = strlen(csSeqKey[q]);
			d->setCursor(W-1 -l*8, 4);
			d->print(csSeqKey[q]);
		}
	}
	else
	{
		d->print("Edit");  d->setFont(0);
		int q = seqId();
		d->setCursor(W-1 -2*6, 4);  d->print(q);
		d->setCursor(W/2-6, 4);  d->print(edins ? "ins" : "ovr");
		//d->setCursor(W-1 -2*6, H-8);  d->print(edpos);

		//  write sequence  ---
		d->setCursor(0, 22);
		int n, l = seql[q];
		for (n=0; n <= l; ++n)  // +1 for cursor
		{
			int16_t x = d->getCursorX(), y = d->getCursorY();
			if (n < l)
			{
				uint8_t z = seq[q][n];
				d->print(STR(z));  //d->print(" ");
				d->moveCursor(z<=1 ? 0 : 2, 0);
			}
			if (n == edpos)  // show cursor
			{
				int16_t b = 8 - 8 * tBlnk / cBlnk;
//				if (edins)  // ins |
//					d->drawFastVLine(x, y-1-b+8, b+1, WHITE);
//				else  // ovr _
//					d->drawFastHLine(x, y+8, b+1, WHITE);
			}
		}
		++tBlnk;  // blink cur
		if (tBlnk > cBlnk)  tBlnk = 0;
	}
	#endif
	
	
	#if 0
	if (tInfo < 0)  // trigger
		tInfo = 400;  // par
	
	if (tInfo > 0)  //  info eeprom
	{	--tInfo;
		int x = 90;

		d->setFont(0);
		d->setCursor(x, 0);
		const static char* strInf[5] = {
			"Reset", "Loaded", "Saved:", "Copied", "Pasted" };
		d->print(strInf[infType]);
		if (infType == 1 || infType == 2)
		{
			d->setCursor(x+6, 8);
			d->print(memSize);  d->print(" B");
	}	}
	#endif
