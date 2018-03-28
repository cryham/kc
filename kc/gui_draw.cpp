#include "gui.h"
#include "demos.h"  // ver
#include "Ada4_ST7735.h"

#define PROGMEM
#include "FreeSans9pt7b.h"

#include "usb_keyboard.h"
#include "matrix.h"


extern uint scan_freq;  // scan counter, freq
const static int rr = Matrix_colsNum;
extern uint32_t us_scan;

#include "kbd_draw.h"


//  Main Menu
const char* strMain[M_All] =
{
	"Testing",   // status, keys-mods-lay, matrix ghost
	"Mappings",  // kbd draw, edit bind>
	"Sequences", // view pg, edit>

	"Display",	 // brightn, backgr effect, inactive
	//"Clock",
	"Demos",
	//"Game",
	//"Help", F1
};
const char* strDemo[D_All] =
{
	"Plasma", "Wave", "Hedrons", "CK Logo",
	"Space", "Balls", "Rain", "Fountain",
	"N-gons", "Fonts", "Chars",
};
const char* strTest[4/*Gui::YM2[M_Testing]*/] =
{
	"Scan Setup",
	"Test Matrix",
	"Pressed",
	"Mouse",
};

char a[128];  // temp str


//  Draw, settings
//....................................................................................
void Gui::Draw()
{

	//if (demo != D_Rain)
		d->clear();

	//if (mlevel==0 || mlevel==1 && ym == M_Sequences || tInfo > 0 || help || status)
	{
		d->setFont(&FreeSans9pt7b);
		d->setCursor(0,0);
		d->setTextColor(RGB(12,22,31));
	}

	//  Main menu
	//------------------------------------------------------
	if (mlevel==0)
	{
		d->print("Main Menu");  d->setFont(0);

		d->setCursor(0, 32);
		for (int i=0; i < M_All; ++i)
		{
			d->setTextColor(RGB(18,18,31));
			d->print(i==ym ? " \x10 ":"   ");

			int8_t c = abs(i-ym);
			d->setTextColor(RGB( max(0, 21-c*3), max(0, 26-c*2), 31-c));
			d->println(strMain[i]);

			d->moveCursor(0, i==2 ? 6 : 2);  // gap
		}

		//d->setCursor(W-1 -7*6, H-8);
		//d->print("F1 Help");
		return;
	}

	int yy = ym2[ym];


	//  Demos
	//------------------------------------------------------
	if (ym == M_Demos)
	{
		if (mlevel == 2)
		{
			d->setFont(0);

			uint32_t oti1 = millis();
			switch (yy)
			{
				case D_Plasma:   demos.Plasma();  break;  // clr 55, 40 ms
				case D_Wave:     demos.Wave();  break;
				case D_Hedrons:  demos.Hedrons();  break;  // 5-9ms

				case D_CK_Logo:  demos.CK_logo();  break;  // 7
				case D_Ngons:    demos.Ngons();  break;  // 12 8ms 14 10ms

				case D_Space:    demos.Space();  break;
				case D_Balls:    demos.Balls();  break;
				case D_Rain:     demos.Rain();  break;
				case D_Fountain: demos.Fountain();  break;

				case D_Fonts:	 demos.Font_ver(false);  break;
				case D_Chars:    demos.Chars();  break;
			}
			tdemo = millis() - oti1;
		}else
		{
			d->print(strMain[ym]);  d->setFont(0);

			//  menu
			int16_t x=0, xw=70, y1=32, y=y1, c;
			for (int i=0; i < D_All; ++i)
			{
				d->setCursor(x,y);
				d->setTextColor(RGB(18,18,31));
				d->print(i==yy ? " \x10 ":"   ");

				c = abs(i-yy);
				d->setTextColor(RGB( max(0, 21-c*3), max(0, 26-c*2), 31-c));

				d->print(strDemo[i]);

				y += 10;
				if (i == D_Next)  // next col
				{	x += xw;  y = y1;  }
			}
		}
		return;
	}


	//  Testing
	//------------------------------------------------
	if (ym == M_Testing)
	{
		if (mlevel == 1)
		{
			d->print(strMain[ym]);  d->setFont(0);

			//  menu
			int16_t y1=32;
			for (int i=0; i < YM2[ym]; ++i)
			{
				d->setTextColor(RGB(18,18,31));
				d->setCursor(0, y1 + i*8);

				d->print(i==yy ? " \x10 ":"   ");
				d->print(strTest[i]);
			}
			return;
		}

		//  title
		d->print(strTest[yy]);
		d->setFont(0);
		d->setTextColor(RGB(21,26,31));


		switch (yy)
		{
		case 1:
		{
			//  matrix  ---
			const static int16_t x1 = 12, y1 = 64;

			bool ghost = false;
			for (uint c=0; c < Matrix_colsNum; ++c)
			for (uint r=0; r < Matrix_rowsNum; ++r)
			{
				d->setCursor(x1 + c*8, y1 + r*8);
				const KeyState& k = Matrix_scanArray[Matrix_colsNum * r + c];

				#define CGh(b)  d->setTextColor(b ? RGB(31,26,12) : RGB(12,20,26))

				if (k.state == KeyState_Off)
				{	sprintf(a,".");
					bool gh = col_ghost[c] || row_ghost[r];
					CGh(gh);
					ghost |= gh;
				}else{
					sprintf(a,"%d", k.state);
					d->setTextColor(RGB(24,28,31));
				}
				d->print(a);
			}

			//  keys  ---
			d->setCursor(0,26);
			d->setTextColor(RGB(24,24,31));
			sprintf(a,"Held %d  press %d", cnt_press-cnt_rel, cnt_press);//, cnt_hold % 1000);
			d->print(a);

			//  ghosting  ---
			d->setCursor(0,40);
			d->setTextColor(ghost ? RGB(31,26,12) : RGB(16,21,26));
			//sprintf(a,"Ghost col %d row %d  K%d", ghost_cols, ghost_rows, keys);
			sprintf(a,"Ghost col %d row %d", ghost_cols, ghost_rows);
			d->print(a);

			//  col| row- use
			for (uint c=0; c < Matrix_colsNum; ++c)
			{
				CGh(col_ghost[c]);
				d->setCursor(x1 + c*8, y1 - 1*8);
				sprintf(a,"%d", col_use[c]);
				d->print(a);
			}
			for (uint r=0; r < Matrix_rowsNum; ++r)
			{
				CGh(row_ghost[r]);
				d->setCursor(0, y1 + r*8);
				sprintf(a,"%d", row_use[r]);
				d->print(a);
			}
		}	break;

		case 0:
		{
			d->setCursor(0,32);
			d->setTextColor(RGB(16,21,26));
			sprintf(a,"Scan %u Hz  t %lu us", scan_freq, us_scan);
			d->println(a);
			d->moveCursor(0,4);
			//d->setTextColor(RGB(24,28,31));

			//d->print("Protocol: ");  //d->println(USBKeys_Protocol == 1 ? " NKRO" : " Boot");
			//d->moveCursor(0,4);

			//DebounceThrottleDiv_define-
			sprintf(a,"Debounce ms: %d", MinDebounceTime_define);
			d->println(a);
			d->moveCursor(0,4);

			sprintf(a,"Strobe delay us: %d", STROBE_DELAY);
			d->println(a);
			d->moveCursor(0,4);

//			d->print("Sequences: ");  d->println(iSlots);
//			d->print("Seq. max len: ");  d->println(iSeqLen);
		}	break;

		case 2:
		{
			d->setCursor(0,32);
			//  layers  -
			d->print("Layers: ");
			/*for (int l=1; l < layersCnt; ++l)  // 1st is 1 in menu
			{	d->print(layersOn[l]);  d->moveCursor(6,0);  }*/
			d->println("");  d->moveCursor(0,4);

			//Demos::Font_ver(d, true);  //*

			//  locks  -
			d->print("Lock: ");  // Num Caps Scrl
			d->println("");  d->moveCursor(0,4);

			//  modifiers  -
			d->print("Mod:  ");
//			for (int i=KEY_LCTRL; i<=KEY_RGUI; ++i)
//			if (kk[i])
//			{	d->print(i >= KEY_RCTRL ? "R" : "L");
//				d->print(mod[i-KEY_LCTRL]);  d->print(" ");
//			}
			d->println("");  d->moveCursor(0,4);

			//  keys  - -
			d->print("Key:  ");
			int c = 0;
//			for (int i = KEY_A; i < strALL; ++i)
//				if (kk[i])
//				{	d->print(STR(i));  d->print(" ");  ++c;  }
			//  count
			d->setCursor(W-1 -(c > 9 ? 2*6 : 6), H-1-8);
			d->print(c);

//			if (ghost_cols /*|| ghost_rows*/)
//			{
//				d->setCursor(3*6, H-1-8);
//				d->print("Gho |");  d->print(ghost_cols);
//				d->print(" -");  d->print(ghost_rows);
//			}
		}	break;

		case 3:
		{
			///  dbg  mouse speed  --
			d->setCursor(0,32);
			//const int16_t y1 = H-1-2*8, y2 = H-1-1*8;
			d->print("hold  delay  speed");

//			d->setCursor(0, y1);  d->print(mx_holdtime);
//			d->setCursor(0, y2);  d->print(my_holdtime);
//
//			d->setCursor(W/3, y1);  d->print(mx_delay);
//			d->setCursor(W/3, y2);  d->print(my_delay);
//
//			d->setCursor(2*W/3, y1);  d->print(mx_speed);
//			d->setCursor(2*W/3, y2);  d->print(my_speed);
		}	break;
		}
		return;
	}

	//  Mappings kbd
	//------------------------------------------------
	if (ym == M_Mappings)
	{
		d->print(strMain[ym]);  d->setFont(0);

		//  kbd draw
		int x = 0, y = 0;
		for (int i=0; i < numKeys; ++i)
		{
			const SKey& k = drawKeys[i];
			if (k.x >=0)  x = k.x;  else  x -= k.x;
			if (k.y > 0)  y = k.y + 60; /*Y*/  else  y -= k.y;

			d->drawRect(x, y, k.w+1, k.h+1, clrRect[k.o]);
			d->setCursor(
				k.o==5 || k.o==7 ? x + k.w - 6 :
						k.o==3 ? x+1 : x+1,
				k.h < H ? y :
						k.o==3 ? y+1 : y+1);
			d->setTextColor(clrText[k.o]);
			sprintf(a,"%c", k.c);
			d->print(a);
		}

		// todo: cursor [] ..

		return;
	}

	//  Sequences kbd view, edit
	//------------------------------------------------
	if (ym == M_Sequences)
	{
		d->print(strMain[ym]);  d->setFont(0);

		return;
	}


	//  Display
	//------------------------------------------------
	if (ym == M_Display)
	{
		d->print(strMain[ym]);  d->setFont(0);

		return;
	}


	//  else
	d->setFont(0);

	
	#if 0
	if (mlevel==1 && ym == MSeq)
	if (!edit)
	{
		d->print("View");  d->setFont(0);
		
		//  list slots
		int s = page * iPage, i,n,x, xm;
		for (i=0; i < iPage && s < iSlots; ++i,++s)
		{
			d->setCursor(0, 18 + i*9);  //16 + i*8
			//d->print(i==slot ? "\x10":" ");   d->print(s);  d->print(": ");
			if (s<10)  d->print(" ");  //align
			d->print(s);  d->print(i==slot ? "\x10":" ");
			
			//  write sequence  ---
			n=0;  x=0;  xm=1;
			while (n < seql[s] && xm)
			{
				uint8_t z = seq[s][n];
				const char* st = STR(z);
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
		d->setCursor(60, 4);
		d->print(page+1);  d->print("/");  d->print(iSlots/iPage);
		
		///  seq key
		if (tInfo == 0)
		{	int q = seqId();
			int l = strlen(csSeqKey[q]);
			d->setCursor(W-1 -l*8, 4);
			d->print(csSeqKey[q]);
		}
	}
	else
	{
		d->print("Edit");  d->setFont(0);
		int q = seqId();
		d->setCursor(W-1 -2*6, 4);  d->print(q);
		d->setCursor(W/2-6, 4);  d->print(edins ? "ins" : "ovr");
		//d->setCursor(W-1 -2*6, H-8);  d->print(edpos);

		//  write sequence  ---
		d->setCursor(0, 22);
		int n, l = seql[q];
		for (n=0; n <= l; ++n)  // +1 for cursor
		{
			int16_t x = d->getCursorX(), y = d->getCursorY();
			if (n < l)
			{
				uint8_t z = seq[q][n];
				d->print(STR(z));  //d->print(" ");
				d->moveCursor(z<=1 ? 0 : 2, 0);
			}
			if (n == edpos)  // show cursor
			{
				int16_t b = 8 - 8 * tBlnk / cBlnk;
//				if (edins)  // ins |
//					d->drawFastVLine(x, y-1-b+8, b+1, WHITE);
//				else  // ovr _
//					d->drawFastHLine(x, y+8, b+1, WHITE);
			}
		}
		++tBlnk;  // blink cur
		if (tBlnk > cBlnk)  tBlnk = 0;
	}
	#endif
	d->setFont(0);
	
	
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


//  Draw End
//------------------------------------------------
void Gui::DrawEnd()
{
	// txt
	//if (text)
	{

		//  fps  ---------
		uint32_t ti = millis();
		d->setCursor(W-6*6,0);
		float fr = 1000.f / (ti - oldti);
		int ff = fr;

		sprintf(a,"%d %lu", ff, tdemo);
		d->setTextColor(RGB(24,28,31));
		d->print(a);
		oldti = ti;


		//  time  ---
		#if 0
		if (text == 3)
		{
			tm = rtc_get();
			int h = tm/3600%24, m = tm/60%60, s = tm%60;
			d->setCursor(0,3*8);

			sprintf(a,"%2d:%02d:%02d", h,m,s);
			d->print(a);
		}
		#endif
	}

	//if (text || demo > D_None)
	{
		d->display();  // 58 Fps, 15ms @144MHz
	}
}
