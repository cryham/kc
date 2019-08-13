#include "gui.h"
#include "Ada4_ST7735.h"
#include "TomThumb3x5.h"
#include "matrix.h"
#include "kbd_layout.h"
#include "kc_data.h"
#include "keys_usb.h"


//  kbd draw   Layout
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  layer use count colors
const uint8_t cluM = 5;
const uint16_t clu[cluM] = {
	RGB(16,24,31), RGB(5,30,30), RGB(30,30,4), RGB(31,19,8), RGB(31,21,22), };

void Gui::DrawLayout(bool edit)
{
	int16_t x=2, y=0;
	d->setWrap(false);

	for (int i=0; i < nDrawKeys; ++i)
	{
		const DrawKey& k = drawKeys[i], pk = drawKeys[max(0,i-1)];

		//  find if pressed
		int f = !edit && k.sc != NO &&
			Matrix_scanArray[k.sc].state == KeyState_Hold ? 1 : 0;

		//  mapping cursor
		if (edit && drawId >= 0 && i == drawId)  f = 2;


		//  vars  layer keys visible on all layers ``
		uint8_t dtL0 = kc.set.key[0][k.sc];
		bool layKey = dtL0 >= K_Layer1 && dtL0 < K_Layer1+KC_MaxLayers;
		bool layUse = nLay == KC_MaxLayers;  // vis mode
		bool tiny = k.w < 6;
		bool lk = layKey && nLay == dtL0 -K_Layer1 +1;  // cur layer key


		//  set coords or advance
		if (k.x >=0)  x = k.x;  else  x -= k.x;
		if (k.y > 0)  y = k.y + yPosLay;  else
		{	if (pk.w < 6)  y += k.y;  else  y -= k.y;  }  // tiny up

		if (i == drawId)  // save center for move
		{	drawX = x + k.w/2;
			drawY = y + k.h/2;  }


		if (f)  // backgr  ----
			d->fillRect(x+1, y-1, k.w-1, k.h-1, clrRect[k.o]);

		uint16_t  // clr
			cR = f==2 ? RGB(31,30,29) : f==1 ? RGB(28,28,29) :
				 lk ? RGB(31,26,28) : clrRect[k.o];

		//  darken  if draw has NO scId
		if (edit && k.sc == NO)  cR = RGB(9,9,9);

		d->drawRect(x, y-2, k.w+1, k.h+1, cR);  // frame []


		if (lk)  // cur lay key backg
			d->fillRect(x+1, y-1, k.w-1, k.h-1, RGB(22,12,16));


		//  text  ----
		d->setCursor(
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
					d->setFont(&TomThumb);

					d->setColor(clu[ min(cluM-1, u-1) ]);
					d->print(u+'0');
				}
			}else	//  normal
			if (dt != KEY_NONE)
			{
				if (m)  d->moveCursor(-1, tiny ? 0 : 2);
				d->setFont(m ? &TomThumb : 0);  // 3x5

				const uint8_t* c = &cGrpRgb[cKeyGrp[dt]][0][0];
				if (tiny)  // tiny rect for color, no text
					d->drawRect(d->getCursorX(), d->getCursorY()+1,
						2,2, RGB(c[0],c[1],c[2]));
				else
				{	d->setClr(c[0],c[1],c[2]);
					d->print(tiny && layKey ? &ch[1] : ch);
				}
		}	}
	}
	d->setFont(0);  d->setWrap(true);
}
