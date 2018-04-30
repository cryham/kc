#include "gui.h"
#include "Ada4_ST7735.h"
#include "TomThumb.h"
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
	char a[64];
	d->setFont(0);
	int16_t x=2, y=0, c;


	//  key codes list
	//. . . . . . . . . . . . . . . . . . . . . . . . .
	if (pickCode)
	{
		d->setCursor(x,0);
		d->setTextColor(RGB(28,28,26));
		d->print("Pick key..");

		d->setCursor(20,10);
		d->setTextColor(RGB(22,22,12));
		//sprintf(a,"%3d/%d", keyCode, KEYS_ALL_EXT);
		//d->print(a);
		if (grpFilt)  d->print("Group:");
				else  d->print("All");
		//  line
		d->drawFastHLine(0, 19, W-1,
			grpFilt ? RGB(16,10,12) : RGB(10,10,12));

		int x = (W-1-5) * keyCode / KEYS_ALL_EXT;
		d->drawFastHLine(x, 19, 5,
			grpFilt ? RGB(30,26,26) : RGB(25,25,28));

		//  key, grp
		d->setCursor(W/2-1,0);
		d->setTextColor(RGB(25,28,31));
		d->print(cKeyStr[keyCode]);

		//  seq preview  ---
		if (keyCode >= K_Seq0 && keyCode <= K_SeqLast)
		{	int8_t seq = keyCode - K_Seq0;
			if (seq < KC_MaxSeqs)
			{	d->setCursor(0, 21);
				WriteSeq(seq, 2);
		}	}

		d->setCursor(W/2-1,10);
		d->setTextColor(RGB(31,15,21));

		const int xw = 42, y0 = 31;
		//  Filtered view by group, 3 cols
		if (grpFilt)
		{
			const uint8_t g = keyGroup;
			d->print(cGrpName[g]);

			int k = grpStart[g], y = y0, x = xw;
			k += (keyCode - k)/12*12;

			while (k <= grpEnd[g] && x < W-5*6)
			{
				d->setCursor(x, y);
				int q = abs(k - keyCode);  // diff clr

				if (!q)  // cursor [ ]
				{	d->fillRect(x-1,y, 41,8, RGB(10,10,9));
					d->drawRect(x-2,y, 42,8, RGB(23,23,13));
				}
				FadeGrp(g, 9, q, 3);
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
			uint8_t g = cKeyGrp[k];
			{
				if (!q)  // cursor [ ]
				{	d->fillRect(x-1,y, 41,8, RGB(10,10,9));
					d->drawRect(x-2,y, 42,8, RGB(23,23,13));
				}
				FadeGrp(g, 9, q, 3);
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
		d->setTextColor(RGB(28,28,9));
		d->print(i == yy ? "\x10 ":"  ");  // >

		c = abs(i - yy);
		FadeClr(C_Map, 4, c, 1);

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
				uint8_t u = kc.set.key[nLay][id];
					 if (u == KEY_NONE)  sprintf(a,"Key: None");
				else if (u >= KEYS_ALL_EXT)  sprintf(a,"Key: OUT");
				else
				{
					FadeGrp(cKeyGrp[u], 9, 0, 3);
					sprintf(a,"Key: %s", cKeyStr[u]);
					d->print(a);

					//  seq preview  ---
					if (u >= K_Seq0 && u <= K_SeqLast)
					{	int8_t seq = u - K_Seq0;
						if (seq < KC_MaxSeqs)
						{	d->setCursor(x, y+12);
							WriteSeq(seq, 2);
					}	}
					a[0]=0;
				}
			}
			d->print(a);  break;
		}
		y += 10;
	}

	//  ids dbg -
	/*d->setTextColor(RGB(16,20,12));
	d->setCursor(0, 6*8+4);
	sprintf(a,"scId: %d draw %d  send %d", scId, drawId, kbdSend);  d->print(a);
	sprintf(a,"draw %2d  x %2d y %2d", drawId, drawX, drawY);  d->print(a);*/


	//  stats, data
	//. . . . . . . . . . . . . . . . . . . . . . . . .
	x = W-8*6;  y=0;
	d->setTextColor(RGB(25,24,12));
	d->setCursor(x,y);
	int si = kc.set.nkeys();
	//sprintf(a,"all %d", si);  d->print(a);  y+=10;

	d->setCursor(x,y);
	if (id >= 0 && id < si)
	{
		sprintf(a,"L key");  // hdr
		d->print(a);  y+=8+2;

		for (int i=0; i < KC_MaxLayers && y < H/2; ++i)
		{
			uint8_t kd = kc.set.key[i][id];
			if (kd != KEY_NONE)
			{
				FadeGrp(cKeyGrp[kd < KEYS_ALL_EXT ? kd : 0],
					9, 2/*fade*/, 3);
				d->setCursor(x,y);
				sprintf(a,"%d %s",i,
					kd < KEYS_ALL_EXT ? cKeyStr[kd] : "OUT");
				d->print(a);  y+=8;
		}	}
	}
	else
	{	sprintf(a,"key: no");
		d->print(a);
	}

	DrawLayout(true);

	d->setFont(0);
}


//  kbd draw   Layout
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void Gui::DrawLayout(bool edit)
{
	int16_t x=2, y=0;

	for (int i=0; i < nDrawKeys; ++i)
	{
		const DrawKey& k = drawKeys[i],
				pk = drawKeys[max(0,i-1)];

		//  find if pressed
		int f = k.sc != NO && !moveCur &&
				Matrix_scanArray[k.sc].state == KeyState_Hold ? 1 : 0;

		//  mark  mapping edit
		if (edit)  {
		if (moveCur && drawId >= 0 && i == drawId)  f = 2;
		if (!moveCur && scId != NO && scId == k.sc)  f = 2;  }

		//  set coords or advance
		if (k.x >=0)  x = k.x;  else  x -= k.x;
		if (k.y > 0)  y = k.y + yPosLay;  else
		{	if (pk.w < 6)  y += k.y;  else  y -= k.y;  }  // tiny up

		if (i == drawId)  // save center for move
		{	drawX = x + k.w/2;
			drawY = y + k.h/2;  }

		if (f)
			d->fillRect(x+1, y-1, k.w-1, k.h-1, clrRect[k.o]);

		uint16_t  // clr
			cR = f==2 ? RGB(31,30,29) : f==1 ? RGB(28,28,29) : clrRect[k.o];

		//  darken  if draw has NO scId
		if (moveCur && k.sc == NO)
		{	cR = RGB(9,9,9);  /*if (!f)  cT = RGB(17,17,17);*/  }

		d->drawRect(x, y-2, k.w+1, k.h+1, cR);

		//  skip caption for tiny keys
		if (k.w < 6)
			continue;

		d->setCursor(  // txt
			k.o==5 || k.o==7 ? x + k.w - 6 :  // right align
			(k.o==3 ? x+1 : x+2),  // symb 3
			k.h == kF ? y-2 : y-1);  // short

	#if 0
		d->setTextColor(cT);
		sprintf(a,"%c", k.c);
		d->print(a);
	#else
		if (k.sc != NO && k.sc < kc.set.nkeys())
		{
			// todo layer use vis..
			uint8_t dt = kc.set.key[edit ? nLay : 0][k.sc];
			if (dt != KEY_NONE)
			{
				const uint8_t* c = &cGrpRgb[cKeyGrp[dt]][0][0];
				const char* ch = cKeySh[dt];
				int le = strlen(ch);
				d->setFont(le == 2 ? &TomThumb : 0);  // 3x5
				if (le == 2)
					d->moveCursor(-1,2);

				d->setTextColor(RGB(c[0],c[1],c[2]));
				//sprintf(a,"%s", );
				d->print(ch);
		}	}
	#endif
	}
	d->setFont(0);
}