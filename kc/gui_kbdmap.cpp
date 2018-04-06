#include "gui.h"
#include "Ada4_ST7735.h"
#include "FreeSans9pt7b.h"
#include "usb_keyboard.h"
#include "matrix.h"
#include "kbd_layout.h"
#include "kc_data.h"
#include "keys_usb.h"

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

		const int xw = 42, y0 = 28;
		//  Filtered view by group, 3 cols
		if (grpFilt)
		{
			const int g = keyGroup;
			d->print(cGrpName[g]);

			int k = kc.grpStart[g], y = y0, x = xw;
			k += (keyCode - k)/12*12;

			while (k <= kc.grpEnd[g] && x < W-5*6)
			{
				d->setCursor(x, y);
				int q = abs(k - keyCode);  // diff clr

				if (!q)  // cursor [ ]
				{	d->fillRect(x-1,y, 41,8, RGB(10,10,9));
					d->drawRect(x-2,y, 42,8, RGB(23,23,13));
				}
				FadeClr(&cGrpRgb[g][0][0], &cGrpRgb[g][1][0], 9, q, 3);
				d->print(cKeyStr[k]);

				++k;
				y += 8;  if (y > H-1-8)
				{	y = y0;  x += xw;  }
			}
			return;
		}
		//  All view  list  4 cols, 12 rows
		d->print(cGrpName[cKeyGrp[keyCode]]);

		int pg = max(-12*1, min(pgMin, keyCode/12*12 -12*1));

		for (int c=grpFilt ? 1:0; c < 4; ++c)
		for (int i=0; i < 12; ++i)
		{
			x = c * xw;  y = y0 + i*8;
			d->setCursor(x, y);
			int k = (pg + i + c*12 + KEYS_ALL_EXT) % KEYS_ALL_EXT;
			int q = abs(k - keyCode);  // diff clr
			int g = cKeyGrp[k];
			{
				if (!q)  // cursor [ ]
				{	d->fillRect(x-1,y, 41,8, RGB(10,10,9));
					d->drawRect(x-2,y, 42,8, RGB(23,23,13));
				}
				FadeClr(&cGrpRgb[g][0][0], &cGrpRgb[g][1][0], 9, q, 3);
				d->print(cKeyStr[k]);
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
		d->setTextColor(RGB(31,31,11));
		d->print(i == yy ? "\x10 ":"  ");

		c = abs(i - yy);  // dist dim
		d->setTextColor(RGB(
			max(0, 21 - c * 5),
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
				sprintf(a,"Key: NONE");
			else
			{
				uint8_t u = kc.set.keys[id].get(nLay);
					 if (u == KEY_NONE)  sprintf(a,"Key: None");
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
	if (id >= 0 && id < si)
	{
		const KC_Key& k = kc.set.keys[id];
		si = k.data.size();
		sprintf(a,"L key");
		d->print(a);  y+=8+2;
		//if (si>5) si=5;  // cut
		d->setTextColor(RGB(27,27,14));

		for (int i=0; i<si && y < H/2; ++i)
		{
			uint8_t kd = k.data[i];
			if (kd != KEY_NONE)
			{
				d->setCursor(x,y);
				sprintf(a,"%d %s",i,
					kd < KEYS_ALL_EXT ? cKeyStr[kd] : "OUT");
				d->print(a);  y+=8;
		}	}

		/*d->setCursor(x,y+2);
		sprintf(a,"Luse %lu", k.layUse);
		d->print(a);  y+=8;*/
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
