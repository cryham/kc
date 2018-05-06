#include "gui.h"
#include "Ada4_ST7735.h"
#include "TomThumb.h"
#include "matrix.h"
#include "kbd_layout.h"
#include "kc_data.h"
#include "keys_usb.h"


//  Mappings kbd
//....................................................................................
const static int pgMin = ((KEYS_ALL_EXT-12*3)/12 +1)*12;

void Gui::DrawMapping()
{
	char a[64];
	int16_t x=2, y=0;


	//  key codes list
	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	if (pickCode)
	{
		d->setFont(0);
		d->setCursor(x,0);
		d->setClr(28,28,26);
		d->print("Pick key..");

		d->setCursor(0,10);
		d->setClr(22,22,12);
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
		d->setClr(25,28,31);
		d->print(cKeyStr[keyCode]);

		//  seq preview  ---
		if (keyCode >= K_Seq0 && keyCode <= K_SeqLast)
		{	int8_t seq = keyCode - K_Seq0;
			if (seq < KC_MaxSeqs)
			{	d->setCursor(0, 21);
				DrawSeq(seq, 2);
		}	}
		else if (keyCode >= K_Fun0 && keyCode <= K_Fun9)
		{	int8_t fun = keyCode - K_Fun0;
			d->setCursor(0, 21);
			d->print(cFunStr[fun]);
		}


		d->setCursor(W/2-1,10);
		d->setClr(31,15,21);

		const int xw = 42, y0 = 31;
		//  Filtered view by group, 3 cols
		if (grpFilt)
		{
			const uint8_t gr = keyGroup;
			d->print(cGrpName[gr]);

			int gn = grpStart[gr], y = y0, x = xw;
			gn += (keyCode - gn)/12*12;

			while (gn <= grpEnd[gr] && x < W-5*6)
			{
				d->setCursor(x, y);
				int q = abs(gn - keyCode);  // diff clr

				if (!q)  // cursor [ ]
				{	d->fillRect(x-1,y, 41,8, RGB(10,10,9));
					d->drawRect(x-2,y, 42,8, RGB(23,23,13));
				}
				FadeGrp(gr, 9, q, 3);
				d->print(cKeyStr[gn]);

				++gn;
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
			uint8_t gr = cKeyGrp[k];
			{
				if (!q)  // cursor [ ]
				{	d->fillRect(x-1,y, 41,8, RGB(10,10,9));
					d->drawRect(x-2,y, 42,8, RGB(23,23,13));
				}
				FadeGrp(gr, 9, q, 3);
				d->print(cKeyStr[k]);
			}
		}
		return;
	}


	//  menu
	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	d->setClr(13,20,9);  // mapping
	d->print(strMain[ym]);
	d->setFont(0);

	int id = scId;
	if (drawKeys[drawId].sc == NO)  id = -1;
	else  id = drawKeys[drawId].sc;

	x=2;  y=24;
	for (int i=0; i < 3; ++i)
	{
		d->setCursor(x,y);
		FadeClr(C_Map, 4, i+1, 1);

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
			if (nLay == KC_MaxLayers)
				d->print("p  Layer: use");
			else
			{	sprintf(a,"p  Layer: %d", nLay);
				d->print(a);
			}	break;

		case 2:
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
							DrawSeq(seq, 2);
					}	}
					else if (u >= K_Fun0 && u <= K_Fun9)
					{	int8_t fun = u - K_Fun0;
						d->setCursor(x, y+12);
						d->print(cFunStr[fun]);
					}
					a[0]=0;
			}	}
			d->print(a);  break;
		}
		y += 10;
	}

	//  ids dbg -
	/*d->setClr(16,20,12);
	d->setCursor(0, 6*8+4);
	sprintf(a,"scId: %d draw %d  send %d", scId, drawId, kbdSend);  d->print(a);
	sprintf(a,"draw %2d  x %2d y %2d", drawId, drawX, drawY);  d->print(a);*/


	//  key binds, lay data
	//. . . . . . . . . . . . . . . . . . . . . . . . .
	x = W-8*6;  y=0;
	d->setClr(25,24,12);
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
