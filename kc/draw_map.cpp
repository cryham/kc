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
	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	if (pickCode)
	{
		d->setCursor(x,0);
		d->setTextColor(RGB(28,28,26));
		d->print("Pick key..");

		d->setCursor(0,10);
		d->setTextColor(RGB(22,22,12));
		//sprintf(a,"%3d/%d", keyCode, KEYS_ALL_EXT);
		//d->print(a);
		if (grpFilt)  d->print("/  Group:");
				else  d->print("/  All");
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
	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	int id = scId;
	if (moveCur)
	{	if (drawKeys[drawId].sc == NO)  id = -1;
		else  id = drawKeys[drawId].sc;
	}
	x=2;  y=0;
	for (int i=0; i < 4; ++i)
	{
		c = abs(i - yy);
		if (!c)  d->fillRect(x+6, y-1, W/2, 10, RGB(5,7,2));
		d->setTextColor(RGB(28,28,9));
		d->setCursor(x+8,y);
		d->print(!c ? "\x10":" ");  // >
		d->setCursor(x,y);
		FadeClr(C_Map, 4, c, 1);

		switch (i)
		{
		case 0:  // press
			switch (pressKey)
			{
			case 0:
			case 1:  sprintf(a,"/  Press");  break;
			case 2:  sprintf(a,"/  Press key ..");
				d->fillRect(0, y-1, 2*W/3, 10, RGB(6,8,2));
				/*d->drawRect(0, y-1, 2*W/3, 10, RGB(20,28,12));*/  break;
			}
			d->print(a);  break;

		case 1:
			if (moveCur) {
				d->fillRect(0, y-1, 2*W/3, 10, RGB(6,8,2));
				/*d->drawRect(0, y-1, 2*W/3, 10, RGB(12,28,20));*/  }
			sprintf(a, moveCur ? "*  Moving .." : "*  Move");
			d->print(a);  break;

		case 2:
			if (nLay == KC_MaxLayers)
				d->print("p  Layer: use");
			else
			{	sprintf(a,"p  Layer: %d", nLay);
				d->print(a);
			}	break;

		case 3:
			if (id < 0 || id >= kc.set.nkeys())
				sprintf(a,"-  Key: NONE");
			else
			{
				uint8_t u = kc.set.key[nLay == KC_MaxLayers ? 0 : nLay][id];
					 if (u == KEY_NONE)  sprintf(a,"-  Key: None");
				else if (u >= KEYS_ALL_EXT)  sprintf(a,"-  Key: OUT");
				else
				{
					FadeGrp(cKeyGrp[u], 9, 0, 3);
					sprintf(a,"-  Key: %s", cKeyStr[u]);
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
					9, 1/*fade*/, 2);
				d->setCursor(x,y);
				if (i == nLay)
					d->fillRect(x-2, y-1, W-1-x+2, 10, RGB(4,7,10));
				sprintf(a,"%d %s",i,
					kd < KEYS_ALL_EXT ? cKeyStr[kd] : "OUT");
				d->print(a);  y+=9;
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
//  layer use count colors
const uint8_t cluM = 5;
const uint16_t clu[cluM] = {
	RGB(16,24,31), RGB(5,30,30), RGB(30,30,4), RGB(31,19,8), RGB(31,21,22), };

void Gui::DrawLayout(bool edit)
{
	int16_t x=2, y=0;

	for (int i=0; i < nDrawKeys; ++i)
	{
		const DrawKey& k = drawKeys[i], pk = drawKeys[max(0,i-1)];

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

		//  layer keys visible on all layers ``
		uint8_t dtL0 = kc.set.key[0][k.sc];
		bool layKey = dtL0 >= K_Layer1 && dtL0 < K_Layer1+KC_MaxLayers;
		bool layUse = nLay == KC_MaxLayers;  // vis mode
		bool tiny = k.w < 6;

		//  skip caption for tiny keys
		if (tiny && !layKey && !layUse)
			continue;

		d->setCursor(  // txt pos
			k.o==5 || k.o==7 ? x + k.w - 6 :  // right align
			(k.o==3 ? x+1 : x+2),  // symb 3
			k.h == kF ? y-2 : y-1);  // short

		if (k.sc != NO && k.sc < kc.set.nkeys())
		{
			uint8_t dt = layKey ? dtL0 :
				kc.set.key[edit ? nLay : kc.nLayer][k.sc];

			const char* ch = cKeySh[dt];
			//  font size
			bool m = tiny || strlen(ch) == 2;

			if (edit && layUse && !layKey)
			{
				//  layer use vis
				uint8_t u = 0;  // count
				for (int l=0; l < KC_MaxLayers; ++l)
					if (kc.set.key[l][k.sc] != KEY_NONE)  ++u;

				if (u > 0)
				{	d->moveCursor(tiny ? -1 : 0, tiny ? 0 : 2);
					if (d->getCursorX() > W-4)  // off scr-
						d->setCursor(W-4, d->getCursorY());
					d->setFont(&TomThumb);

					d->setTextColor(clu[ min(cluM-1, u-1) ]);
					d->print(u+'0');
				}
			}else	//  normal
			if (dt != KEY_NONE)
			{
				if (m)  d->moveCursor(-1,2);
				d->setFont(m ? &TomThumb : 0);  // 3x5

				const uint8_t* c = &cGrpRgb[cKeyGrp[dt]][0][0];
				d->setTextColor(RGB(c[0],c[1],c[2]));
				d->print(tiny && layKey ? &ch[1] : ch);
		}	}
	}
	d->setFont(0);
}
