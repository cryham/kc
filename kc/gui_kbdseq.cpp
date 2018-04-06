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

	if (!edit)
	{
		d->setTextColor(RGB(20,27,27));
		d->print("View");  d->setFont(0);
		d->setTextColor(RGB(20,25,30));

		//  list slots
		int s = page * iPage, i,n, xm;
		for (i=0; i < iPage && s < iSlots; ++i,++s)
		{
			d->setCursor(0, 28 + i*9);  //16 + i*8
			//d->print(i==slot ? "\x10":" ");   d->print(s);  d->print(": ");
			sprintf(a,"%2d",s);
			d->setTextColor(RGB(15,25,20));
			d->print(a);  d->print(i==slot ? "\x10 ":"  ");
			d->setTextColor(RGB(20,25,30));

			//  write sequence  ---
			n=0;  xm=1;
			while (n < kc.set.seqs[s].len() && xm)
			{
				uint8_t z = kc.set.seqs[s].data[n];
				const char* st = cKeyStr[z];
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
		d->setCursor(W/2, 4);
		sprintf(a,"%2d/%d", page+1,iSlots/iPage);
		d->print(a);

		///  seq key
		/*if (tInfo == 0)
		{	int q = seqId();
			int l = strlen(csSeqKey[q]);
			d->setCursor(W-1 -l*8, 4);
			d->print(csSeqKey[q]);
		}*/
	}
	else
	{
		d->setTextColor(RGB(23,23,27));
		d->print("Edit");  d->setFont(0);
		d->setTextColor(RGB(22,26,30));

		int q = seqId();
		d->setCursor(W-1 -2*6, 4);  d->print(q);
		d->setCursor(W/2-6, 4);  d->print(edins ? "ins" : "ovr");
		//d->setCursor(W-1 -2*6, H-8);  d->print(edpos);

		//  write sequence  ---
		d->setCursor(0, 22);
		int n, l = kc.set.seqs[q].len();
		for (n=0; n <= l; ++n)  // +1 for cursor
		{
			int16_t x = d->getCursorX(), y = d->getCursorY();
			if (n < l)
			{
				uint8_t z = kc.set.seqs[q].data[n];
				d->print(cKeyStr[z]);  //d->print(" ");
				d->moveCursor(z<=1 ? 0 : 2, 0);
			}
			if (n == edpos)  // show cursor
			{
				int16_t b = 8 - 8 * tBlnk / cBlnk;
				if (edins)  // ins |
					d->drawFastVLine(x, y-1-b+8, b+1, 0xFFFF);
				else  // ovr _
					d->drawFastHLine(x, y+8, b+1, 0xFFFF);
			}
		}
		++tBlnk;  // blink cur
		if (tBlnk > cBlnk)  tBlnk = 0;
	}


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

}
