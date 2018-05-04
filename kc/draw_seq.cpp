#include "gui.h"
#include "Ada4_ST7735.h"
#include "FreeSans9pt7b.h"
#include "usb_keyboard.h"
#include "matrix.h"
#include "kbd_layout.h"
#include "kc_data.h"
#include "keys_usb.h"

extern KC_Main kc;


//  write sequence, 1 line, short
//......................................................
void Gui::DrawSeq(int8_t seq, int8_t q)
{
	int n=0, len = kc.set.seqs[seq].len();
	while (n < len)
	{
		uint8_t dt = kc.set.seqs[seq].data[n];
		bool isCmd = dt >= K_Cmd0 && dt <= K_CmdLast;

		const char* st = cKeyStr[dt];
		uint8_t gr = cKeyGrp[dt];
		FadeGrp(gr, 9, q, 3);

		int16_t x = d->getCursorX(),
			xx = isCmd ? 4 : 6*strlen(st);

		if (x + xx >= W-1 -6*2)
		{	//  outside, more sign
			d->print(".");  d->moveCursor(-3,0);
			d->print(".");  return;
		}
		else
		{	//  commands ___ draw short
			if (isCmd)
			{	//  just indicator|
				int cmd = dt - K_Cmd0;
				int16_t y = d->getCursorY();
				switch (cmd)
				{
				case CMD_SetDelay:
				case CMD_Wait:
					d->drawFastVLine(x,y+1,5, RGB(31,31,20));
					break;
				case CMD_Comment:
					d->drawFastVLine(x,y+2,3, RGB(6,31,31));
					break;
				case CMD_Hide:
					d->drawFastVLine(x,y+1,5, RGB(10,20,30));
					return;
				}
				d->moveCursor(4, 0);
				++n;  // skip next, param byte
			}
			else  //  keys
			{	d->print(st);  //d->print(" ");
				d->moveCursor(dt<=1 ? 0 : 2, 0);
			}
			++n;
	}	}
}

//  Sequences kbd  view, edit
//....................................................................................
void Gui::DrawSequences()
{
	char a[64];

	//  View
	//......................................................
	if (!edit)
	{
		d->setClr(12,26,26);
		d->print("View");  d->setFont(0);
		d->setClr(20,25,28);

		//  page, center   /
		d->setCursor(W/2 -2*6, 4);
		sprintf(a,"%d/%d", page+1, kc.set.seqSlots/iPage);  d->print(a);

		//  list slots
		int s = page * iPage, i, y, q;
		for (i=0; i < iPage && s < kc.set.seqSlots; ++i,++s)
		{
			y = 28 + i*9;
			d->setCursor(0, y);
			d->setClr(20,30,25);
			q = abs(i - slot);
			FadeClr(C_Seq, 4, q, 2);
			if (!q)  d->fillRect(0, y-1, W-1, 10, RGB(2,6,8));
			sprintf(a,"%2d",s);  d->print(a);

			d->setClr(0,30,30);
			d->moveCursor(2,0);
			if (!q)  d->print("\x10");  // >
			d->setClr(20,31,31);

			d->setCursor(2*6, y);  // copy mark
			if (s == cpId)  d->print("\x7");
			d->setCursor(4*6, y);

			DrawSeq(s, q);  // write
		}

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
			d->setClr(30,23,26);
			sprintf(a,"L %d", ll);  d->print(a);

			d->setCursor(x, 10);
			int8_t dt = kc.set.key[0][ii];  // layer 0 key
			FadeGrp(cKeyGrp[dt], 9, 0, 3);
			sprintf(a,"%s", cKeyStr[dt]);  d->print(a);
		}
	}
	//  Edit
	//......................................................
	else
	{
		d->setClr(5,27,27);
		d->print("Edit");  d->setFont(0);
		d->setClr(23,27,31);

		int si = seqId();
		d->setCursor(W-1 -2*6, 4);  sprintf(a,"%2d", si);  d->print(a);
		d->setCursor(W/2 -2*6, 4);  d->print(edins ? "Ins" : "Ovr");

		//  write sequence  ------------------------------------
		int16_t x = 1, y = 32, xx=0;
		int n, l = kc.set.seqs[si].len();
		#if 0
		d->setCursor(W/2 -3*6, 14);  // test
		sprintf(a,"%d %d", edpos,l);  d->print(a);
		#endif
		bool inCmt = false;

		for (n=0; n <= l; ++n)  // +1 for cursor
		{
			xx = 0;
			bool cur = n == edpos;
			if (n < l)
			{
				uint8_t dt = kc.set.seqs[si].data[n];
				//  command
				bool isCmd = dt >= K_Cmd0 && dt <= K_CmdLast;
				int cmd = dt - K_Cmd0;
				const static uint8_t cmdStrLen[CMD_ALL]={
					7, 6, 4, 5, 7,  };  // cmd str lengths

				int gr = cKeyGrp[dt];  // clr
				int q = abs(n - edpos);
				FadeGrp(gr, 9, q, 4);

				//  string length
				xx = (isCmd ? cmdStrLen[min(CMD_ALL-1, cmd)] :
						strlen(cKeyStr[dt])) * 6 +2;
				if (x + xx > W-1)
				{	x = 1;  y += 8+4;  }  // new line

				d->setCursor(x,y);
				if (isCmd)
				{	++n;  //  commands ___ draw
					uint8_t cp = n < l ? kc.set.seqs[si].data[n] : 0;

					d->drawFastHLine(x, y+8, xx-2, RGB(20,20,31));  //_
					d->setClr(30,30,30);

					//  format command name and param  ---
					switch (cmd)
					{
					case CMD_SetDelay:
						sprintf(a,"D=%3dms", cp);  d->print(a);  break;
					case CMD_Wait:
					{	char f[6];  dtostrf(cp*0.1f, 4,1, f);
						sprintf(a,"W%ss", f);  d->print(a);  }  break;

					case CMD_Comment:
						d->print(inCmt ? "}Cmt" : "Cmt{");
						inCmt = !inCmt;  break;
					case CMD_Hide:
						d->print("Hide>");  break;

					case CMD_Mx:  //int16_t-
						sprintf(a,"Mx%+3d", cp-128);  d->print(a);  break;
						break;
					}
				}else  // key
					d->print(cKeyStr[dt]);
			}
			if (cur)  // cursor
			{
				int16_t b = 8 - 8 * tBlnk / cBlnk;
				if (edins)  // ins |
					d->drawFastVLine(x-1, y-1-b+8, b+1, 0xFFFF);
				else  // ovr _
					d->drawFastHLine(x-1, y+8, b+1, 0xFFFF);
			}
			x += xx;
		}
		++tBlnk;  // blink cur
		if (tBlnk > cBlnk)  tBlnk = 0;
	}
}


//  Info Operation
//....................................................................................
void Gui::DrawOperInfo()
{
	char a[32];
	if (tInfo < 0)  // trigger
		tInfo = 70;  // par

	if (tInfo > 0)
	{	--tInfo;
		bool h = infType == 1 || infType == 2;
		int x = W-1 - 6*9, x1 = x+6, xe = 6*3,
			y = 12, yy = h ? 42 : 10;

		d->setFont(0);
		d->setCursor(x, 0);  //  bck
		d->fillRect(x-3, 0, W-1-(x-3), yy, RGB(4,6,8));
		d->drawFastVLine(W-1, 0, yy * tInfo / 70, RGB(10,13,16));  // time|
		d->setClr(27,29,31);

		const static char* strInf[6] = {
			"Reset", "Loaded", "Saved:", "Copied", "Pasted", "Swapped" };
		d->print(strInf[infType]);

		if (h)
		{	d->setClr(28,25,31);  // mem`
			d->setCursor(x1, y);
			sprintf(a,"%d B", kc.memSize);  d->print(a);

			d->setClr(24,21,28);  // cnt
			d->setCursor(x1, y+10);
			sprintf(a,"cnt %d", par.verCounter);  d->print(a);

			d->setClr(20,16,24);  // ver-
			d->setCursor(x1, y+20);
			sprintf(a,"ver %d", kc.set.ver);  d->print(a);

			if (kc.err != E_ok)  // error string
			{
				d->fillRect(xe-3, y-2, x-3-(xe-3), 12, RGB(6,4,4));
				d->setClr(31,22,21);
				d->setCursor(xe, y);
				d->print(KCerrStr[kc.err]);
	}	}	}
}
