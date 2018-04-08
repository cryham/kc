#include "gui.h"
#include "Ada4_ST7735.h"
#include "FreeSans9pt7b.h"
#include "usb_keyboard.h"
#include "matrix.h"
#include "kbd_layout.h"
#include "kc_data.h"
#include "keys_usb.h"

extern KC_Main kc;


//  Sequences kbd view, edit
//....................................................................................
void Gui::DrawSequences()
{
	//d->print(strMain[ym]);  d->setFont(0);

	if (!edit)  //  View  ----------
	{
		d->setTextColor(RGB(20,27,27));
		d->print("View");  d->setFont(0);
		d->setTextColor(RGB(20,25,30));

		//  list slots
		int s = page * iPage, i,n, xm, y, q;
		for (i=0; i < iPage && s < kc.set.iSlots; ++i,++s)
		{
			y = 28 + i*9;
			d->setCursor(0, y);
			d->setTextColor(RGB(20,30,25));
			q = abs(i - slot);
			FadeClr(&Mclr[3][0][0], &Mclr[3][1][0], 4, q, 2);
			sprintf(a,"%2d",s);  d->print(a);

			d->setTextColor(RGB(31,31,20));
			if (!q)  d->print("\x10");
			d->setTextColor(RGB(20,31,31));
			d->setCursor(2*6, y);
			if (s == cpId)  d->print("\x7");
			d->setCursor(4*6, y);

			//  write sequence  ---
			n=0;  xm=1;
			while (n < kc.set.seqs[s].len() && xm)
			{
				uint8_t z = kc.set.seqs[s].data[n];
				const char* st = cKeyStr[z];
				int g = cKeyGrp[z];
				FadeClr(&cGrpRgb[g][0][0], &cGrpRgb[g][1][0], 9, q, 3);

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
		d->setCursor(W/2 -2*6, 4);
		sprintf(a,"%2d/%d", page+1, kc.set.iSlots/iPage);  d->print(a);

		///  seq key
		/*if (tInfo == 0)
		{	int q = seqId();
			int l = strlen(cSeqKey[q]);
			d->setCursor(W-1 -l*8, 4);
			d->print(csSeqKey[q]);
		}*/
	}
	else  //  Edit  ----------
	{
		d->setTextColor(RGB(23,23,27));
		d->print("Edit");  d->setFont(0);
		d->setTextColor(RGB(22,26,30));

		int q = seqId();
		d->setCursor(W-1 -2*6, 4);  sprintf(a,"%2d", q);  d->print(a);
		d->setCursor(W/2 -2*6, 4);  d->print(edins ? "Ins" : "Ovr");
		//d->setCursor(W/2 -3*6, 12);
		//sprintf(a,"%d %d", edpos,l);  d->print(a);

		//  write sequence  ---
		int16_t x = 1, y = 32, xx=0;
		int n, l = kc.set.seqs[q].len();
		for (n=0; n <= l; ++n)  // +1 for cursor
		{
			xx = 0;
			if (n < l)
			{
				uint8_t z = kc.set.seqs[q].data[n];
				int g = cKeyGrp[z];  // clr
				int q = abs(n - edpos);
				FadeClr(&cGrpRgb[g][0][0], &cGrpRgb[g][1][0], 9, q, 4);

				xx = strlen(cKeyStr[z]) * 6 +2;
				if (x + xx > W-1)
				{	x = 1;  y += 8+4;  }

				d->setCursor(x,y);
				d->print(cKeyStr[z]);
			}
			if (n == edpos)  // show cursor
			{
				int16_t b = 8 - 8 * tBlnk / cBlnk;
				if (edins)  // ins |
					d->drawFastVLine(x-1, y-1-b+8, b+1, 0xFFFF);
				else  // ovr _
					d->drawFastHLine(x-1, y+8, b+1, 0xFFFF);
			}
			x += xx;  // z<=1 ? 0 : 2;  // fun delay-
		}
		++tBlnk;  // blink cur
		if (tBlnk > cBlnk)  tBlnk = 0;
	}


	//  Info operation  ----------
	if (tInfo < 0)  // trigger
		tInfo = 100;  // par

	if (tInfo > 0)
	{	--tInfo;
		int x = W/2 + 6*4;

		d->setFont(0);
		d->setCursor(x, 2);
		const static char* strInf[5] = {
			"Reset", "Loaded", "Saved:", "Copied", "Pasted" };
		d->print(strInf[infType]);
		if (infType == 1 || infType == 2)
		{
			d->setCursor(x+6, 8+4);
			d->print(memSize);  d->print(" B");
	}	}
}
