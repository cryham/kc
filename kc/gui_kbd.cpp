#include "gui.h"
#include "Ada4_ST7735.h"
#include "FreeSans9pt7b.h"
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


//  Mappings kbd
//....................................................................................
const static int pgMin = ((KEYS_ALL_EXT-12*3)/12 +1)*12;

void Gui::DrawMapping()
{
	d->setFont(0);
	int16_t x=2, y=0, c;


	//  key codes list
	//. . . . . . . . . . . . . . . . . . . . . . . . .
	if (pickCode)
	{
		d->setCursor(x,0);
		d->setTextColor(RGB(28,28,26));
		d->print("Pick key..");

		d->setCursor(0,10);
		d->setTextColor(RGB(22,22,12));
		sprintf(a,"%3d/%d", keyCode, KEYS_ALL_EXT);
		d->print(a);
		if (grpFilt)  d->print(" grp:");
				else  d->print(" all");
		d->drawFastHLine(0,20,W-1,RGB(8,8,10));

		d->setCursor(W/2-1,0);
		d->setTextColor(RGB(25,28,31));
		d->print(cKeyStr[keyCode]);

		d->setCursor(W/2-1,10);
		d->setTextColor(RGB(31,15,21));
		int gr = grpFilt ? keyGroup : cKeyGrp[keyCode];
		gr = max(0, min(grpMax-1, gr));
		d->print(cGrpName[gr]);

		//  list  3 cols, 12 rows
		int pg = //grpFilt ? kc.grpStart[gr]-12 :
			max(-12*1, min(pgMin, keyCode/12*12 -12*1));
		int kmax = KEYS_ALL_EXT; //grpFilt ? kc.grpEnd[gr] : KEYS_ALL_EXT;

		for (int c=0; c < 4; ++c)
		for (int i=0; i < 12; ++i)
		{
			x = c * 42;  y = 28 + i*8;
			d->setCursor(x, y);
			int k = (pg + i + c*12 + KEYS_ALL_EXT) % KEYS_ALL_EXT;
			int q = abs(k - keyCode);  // diff clr
			int g = cKeyGrp[k];
			//if (!grpFilt || g == keyGroup)  // todo: crash
			{
				if (!q)  // cursor
				{	d->fillRect(x-1,y, 41,8, RGB(10,10,9));
					d->drawRect(x-2,y, 42,8, RGB(23,23,13));
				}
				if (k >= 0 && k < kmax)
				{	d->setTextColor(RGB(
						max(9, cGrpRgb[g][0][0] - cGrpRgb[g][1][0] * q/3),
						max(9, cGrpRgb[g][0][1] - cGrpRgb[g][1][1] * q/3),
						max(9, cGrpRgb[g][0][2] - cGrpRgb[g][1][2] * q/3) ));
					d->print(cKeyStr[k]);
				}
			}
		}
		return;
	}

	//  menu
	//. . . . . . . . . . . . . . . . . . . . . . . . .
	int id = scId;
	if (moveCur)
	{	if (drawKeys[drawId].sc == NO)  id = -1;
		else  id = drawKeys[drawId].sc;
	}
	x=2;  y=0;
	for (int i=0; i < 4; ++i)
	{
		d->setCursor(x,y);
		d->setTextColor(RGB(31,31,21));
		d->print(i == yy ? "\x10 ":"  ");

		c = abs(i - yy);  // dist dim
		d->setTextColor(RGB(
			max(0, 21 - c * 6),
			max(0, 31 - c * 2),
			max(0, 21 - c * 5) ));

		switch (i)
		{
		case 0:  // press
			switch (pressKey)
			{
			case 0:
			case 1:  sprintf(a,"Press");  break;
			case 2:  sprintf(a,"Press key ..");  break;
			}
			d->print(a);  break;

		case 1:
			sprintf(a, moveCur ? "Moving .." : "Move");
			d->print(a);  break;

		case 2:
			sprintf(a,"Layer: %d", nLay);
			d->print(a);  break;

		case 3:
			if (id < 0 || id >= kc.set.nkeys())
				sprintf(a,"Key: None");
			else
			{
				uint8_t u = kc.set.keys[id].get(nLay);
					 if (u == KEY_NONE)  sprintf(a,"Key: NONE");
				else if (u >= KEYS_ALL_EXT)  sprintf(a,"Key: OUT");
				else
				{
					d->setTextColor(RGB(23,27,31));
					sprintf(a,"Key: %s", cKeyStr[u]);
				}
			}
			d->print(a);  break;
		}
		y += 10;
	}

	//  ids dbg -
	d->setTextColor(RGB(16,20,12));
	d->setCursor(0, 6*8+4);
	sprintf(a,"scId: %d draw %d  send %d", scId, drawId, kbdSend);  d->print(a);


	//  stats, data
	//. . . . . . . . . . . . . . . . . . . . . . . . .
	x = W-8*6, y=0;
	d->setTextColor(RGB(25,24,12));
	d->setCursor(x,y);
	int si = kc.set.nkeys();
	//sprintf(a,"all %d", si);  d->print(a);  y+=10;

	d->setCursor(x,y);
	if (id < si && id >= 0)
	{
		const KC_Key& k = kc.set.keys[id];
		si = k.data.size();
		sprintf(a,"data %d", si);
		d->print(a);  y+=8;
		if (si>4) si=4;  // cut

		for (int i=0; i<si; ++i)
		{
			d->setCursor(x,y);
			uint8_t kd = k.data[i];
			sprintf(a,"%d %s", i,
				kd < KEYS_ALL_EXT ? cKeyStr[kd] : "ERR");
			d->print(a);  y+=8;
		}

		d->setCursor(x,y);
		sprintf(a,"Luse %lu", k.layUse);
		d->print(a);  y+=8;
	}
	else
	{	sprintf(a,"key: no");
		d->print(a);
	}


	//  kbd draw   Layout
	// * * * * * * * * * * * * * * * * * * * * * * * *
	for (int i=0; i < nDrawKeys; ++i)
	{
		const SKey& k = drawKeys[i];

		//  find if pressed
		int f = k.sc != NO && !moveCur &&
				Matrix_scanArray[k.sc].state == KeyState_Hold ? 1 : 0;

		//  mark  mapping edit
		if (moveCur && drawId >= 0 && i == drawId)  f = 2;
		if (!moveCur && scId >= 0 && scId == k.sc)  f = 2;

		if (k.x >=0)  x = k.x;  else  x -= k.x;
		if (k.y > 0)  y = k.y + 62; /*Y*/  else  y -= k.y;

		if (f)
			d->fillRect(x+1, y-1, k.w-1, k.h-1, clrRect[k.o]);

		uint16_t  // clr
			cR = f==2 ? RGB(31,30,29) : f==1 ? RGB(28,28,29) : clrRect[k.o],
			cT = f==2 ? RGB(31,30,29) : f==1 ? RGB(31,31,31) : clrText[k.o];  //<def

		//  darken  if draw has NO scId
		if (moveCur && k.sc == NO)
		{	cR = RGB(9,9,9);  if (!f)  cT = RGB(17,17,17);  }

		d->drawRect(x, y-2, k.w+1, k.h+1, cR);

		d->setCursor(  // txt
			k.o==5 || k.o==7 ? x + k.w - 6 :  // right align
			(k.o==3 ? x+1 : x+2),  // symb 3
			k.h == kF ? y-2 : y-1);  // short

		d->setTextColor(cT);
		sprintf(a,"%c", k.c);
		d->print(a);
	}
}

//  Sequences kbd view, edit
//....................................................................................
void Gui::DrawSequences()
{
	d->print(strMain[ym]);  d->setFont(0);

}


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
		for (uint c=0; c < NumCols; ++c)
		{
			CGh(col_ghost[c], col_use[c]);
			d->setCursor(x1 + c*8, y1 - 1*8);
			sprintf(a,"%d", col_use[c]);
			d->print(a);
		}
		for (uint r=0; r < NumRows; ++r)
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
		d->setTextColor(RGB(14,23,28));
		d->print("Scan: ");
		d->setTextColor(RGB(17,26,31));
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
				if (code != KEY_NONE)  // layer keys
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
