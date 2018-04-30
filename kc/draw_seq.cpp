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
//  write sequence  ---
void Gui::WriteSeq(int8_t seq, int8_t q)
{
	int n=0, xm=1;
	while (n < kc.set.seqs[seq].len() && xm)
	{
		uint8_t z = kc.set.seqs[seq].data[n];
		const char* st = cKeyStr[z];
		uint8_t g = cKeyGrp[z];
		FadeGrp(g, 9, q, 3);

		if (d->getCursorX() + 6*strlen(st) >= W -6*2)  // outside
		{	d->print(".");  d->moveCursor(-3,0);
			d->print(".");  xm = 0;  // more sign
		}else
		{	d->print(st);  //d->print(" ");
			d->moveCursor(z<=1 ? 0 : 2, 0);
			++n;
	}	}
}

void Gui::DrawSequences()
{
	char a[64];
	//d->print(strMain[ym]);  d->setFont(0);

	if (!edit)  //  View  ----------
	{
		d->setTextColor(RGB(14,26,26));
		d->print("View");  d->setFont(0);
		d->setTextColor(RGB(20,25,30));

		//  list slots
		int s = page * iPage, i, y, q;
		for (i=0; i < iPage && s < kc.set.seqSlots; ++i,++s)
		{
			y = 28 + i*9;
			d->setCursor(0, y);
			d->setTextColor(RGB(20,30,25));
			q = abs(i - slot);
			FadeClr(C_Seq, 4, q, 2);
			sprintf(a,"%2d",s);  d->print(a);

			d->setTextColor(RGB(0,30,30));
			d->moveCursor(2,0);
			if (!q)  d->print("\x10");  // >
			d->setTextColor(RGB(20,31,31));

			d->setCursor(2*6, y);  // copy mark
			if (s == cpId)  d->print("\x7");
			d->setCursor(4*6, y);

			WriteSeq(s, q);  // write
		}
		//  page, center   /
		d->setCursor(W/2 -2*6, 4);
		sprintf(a,"%2d/%d", page+1, kc.set.seqSlots/iPage);  d->print(a);

		//  seq preview key, find  ---
		q = K_Seq0 + seqId();  // code
		int l, ll=-1, ii=-1;
		for (l=0; l < KC_MaxLayers; ++l)
		for (i=0; i < kc.set.scanKeys; ++i)
			if (kc.set.key[l][i] == q)
			{	ll=l; ii=i;  break;  }  // found
		if (ii != -1)
		{	int x = W-1 -5*6;
			d->setCursor(x, 0);
			d->setTextColor(RGB(30,23,26));
			sprintf(a,"L %d", ll);  d->print(a);

			d->setCursor(x, 10);
			int8_t dt = kc.set.key[0][ii];  // layer 0 key
			FadeGrp(cKeyGrp[dt], 9, 0, 3);
			sprintf(a,"%s", cKeyStr[dt]);  d->print(a);
		}
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
				FadeGrp(g, 9, q, 4);

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
		int x = W-1 - 6*9, x1 = x+6, xe = 6*3, y = 12;
		bool h = infType == 1 || infType == 2;

		d->setFont(0);
		d->setCursor(x, 0);  //  bck
		d->fillRect(x-3, 0, W-1-(x-3), h ? 42 : 10, RGB(4,6,8));
		d->setTextColor(RGB(27,29,31));

		const static char* strInf[6] = {
			"Reset", "Loaded", "Saved:", "Copied", "Pasted", "Swapped" };
		d->print(strInf[infType]);

		if (h)
		{
			d->setTextColor(RGB(28,25,31));  // mem`
			d->setCursor(x1, y);
			sprintf(a,"%d B", kc.memSize);  d->print(a);

			d->setTextColor(RGB(24,21,28));  // cnt
			d->setCursor(x1, y+10);
			sprintf(a,"cnt %d", par.verCounter);  d->print(a);

			d->setTextColor(RGB(20,16,24));  // ver-
			d->setCursor(x1, y+20);
			sprintf(a,"ver %d", kc.set.ver);  d->print(a);

			d->fillRect(xe-3, y-2, x-3-(xe-3), 12, RGB(6,4,4));
			d->setTextColor(RGB(31,22,21));
			d->setCursor(xe, y);
			d->print(KCerrStr[kc.err]);
	}	}
}