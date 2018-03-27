#include "gui.h"
#include "demos.h"  // ver
#define PROGMEM
#include "FreeSans9pt7b.h"

#include "usb_keyboard.h"
#include "matrix.h"

extern uint tim_freq;  // scan counter, freq
const static int rr = Matrix_colsNum;
extern uint32_t us_scan;

#include "kbd_draw.h"

//extern const char* strMain[M_All];

const char* strMain[M_All]=
{
	"Testing",   // status?, keys-mods-lay, matrix-gho
	"Mappings",  // kbd draw, edit bind>
	"Sequences", // view pg, edit>
	"Display",	 // brightn, backgr effect, inactive
	//Clock

	"Old demos",
	"Full demos",
	"Game",
	//Help F1
};


//  Draw, settings
//....................................................................................
void Gui::Draw(Ada4_ST7735& d)
{
	//if (mlevel==0 || mlevel==1 && ym == M_Sequences || tInfo > 0 || help || status)
	{
		d.setFont(&FreeSans9pt7b);
		d.setCursor(0,0);
		d.setTextColor(RGB(12,22,31));
	}

	if (mlevel==0)  //  main menu
	{
		d.print("Main Menu");  d.setFont(0);

		d.setCursor(0, 32);
		for (int i=0; i < M_All; ++i)
		{
			d.setTextColor(RGB(18,18,31));
			d.print(i==ym ? " \x10 ":"   ");

			int8_t c = abs(i-ym);
			d.setTextColor(RGB( max(0, 21-c*3), max(0, 26-c*2), 31-c));
			d.println(strMain[i]);

			d.moveCursor(0, i==2 ? 6 : 2);  // gap
		}
		d.setCursor(W-1 -7*6, H-8);
		//d.print("F1 Help");
		//.
		//d.setCursor(W-1 -1*6, H-16);
		//d.println(USBKeys_Protocol == 1 ? "N" : "B");
		return;
	}
	char a[128];
	

	//  testing
	//------------------------------------------------
	if (ym == M_Testing)
	{
		d.print("Test Matrix");
		d.setFont(0);
		d.setTextColor(0xFFFF);

		//  matrix  ---
		bool ghost = false;
		for (uint c=0; c < Matrix_colsNum; ++c)
		for (uint r=0; r < Matrix_rowsNum; ++r)
		{
			d.setCursor(c*8, 64 + r*8);
			const KeyState& k = Matrix_scanArray[Matrix_colsNum * r + c];

			#define CGh(b)  d.setTextColor(b ? RGB(31,26,12) : RGB(12,20,26))

			if (k.state == KeyState_Off)
			{	sprintf(a,".");
				bool gh = col_ghost[c] || row_ghost[r];
				CGh(gh);
				ghost |= gh;
			}else{
				sprintf(a,"%d", k.state);
				d.setTextColor(RGB(24,28,31));
			}
			d.print(a);
		}

		//  keys  ---
		d.setCursor(0,16);
		d.setTextColor(RGB(16,21,26));
		sprintf(a,"Scan %u Hz  t %lu us", tim_freq, us_scan);
		d.print(a);
		d.setTextColor(RGB(24,28,31));

		d.setCursor(0,24+4);
		sprintf(a,"Held %d  press %d", cnt_press-cnt_rel, cnt_press);//, cnt_hold % 1000);
		d.print(a);

		//  ghosting  ---
		d.setCursor(0,40);
		d.setTextColor(ghost ? RGB(31,26,12) : RGB(16,21,26));
		//sprintf(a,"Ghost col %d row %d  K%d", ghost_cols, ghost_rows, keys);
		sprintf(a,"Ghost col %d row %d", ghost_cols, ghost_rows);
		d.print(a);

		//  gho col,row use
		for (uint c=0; c < Matrix_colsNum; ++c)
		{
			CGh(col_ghost[c]);
			d.setCursor(c*8, 64 - 1*8);
			sprintf(a,"%d", col_use[c]);
			d.print(a);
		}
		for (uint r=0; r < Matrix_rowsNum; ++r)
		{
			CGh(row_ghost[r]);
			d.setCursor(W/2-8, 64 + r*8);
			sprintf(a,"%d", row_use[r]);
			d.print(a);
		}
	}
	//  mappings kbd
	//------------------------------------------------
	else if (ym == M_Mappings)
	{
		//  kbd draw
		int x = 0, y = 0;
		for (int i=0; i < numKeys; ++i)
		{
			const SKey& k = drawKeys[i];
			if (k.x >=0)  x = k.x;  else  x -= k.x;
			if (k.y > 0)  y = k.y + 60; /*Y*/  else  y -= k.y;

			d.drawRect(x, y, k.w+1, k.h+1, clrRect[k.o]);
			d.setCursor(
				k.o==5 || k.o==7 ? x + k.w - 6 :
						k.o==3 ? x+1 : x+1,
				k.h < H ? y :
						k.o==3 ? y+1 : y+1);
			d.setTextColor(clrText[k.o]);
			sprintf(a,"%c", k.c);
			d.print(a);
		}
	}
	//  seq kbd view, edit
	//------------------------------------------------
	else if (ym == M_Sequences)
	{

	}

	
	if (0)  //  status info
	{
		d.print("Status");  d.setFont(0);

		d.setCursor(W-1 -3*6, 0);
		d.print(testPage+1);  d.print("/");  d.print(StatAll);

		d.setCursor(0, 16+2);
		
		switch (testPage)  //  ----
		{
		case 0:
			d.print("Protocol: ");  //d.print(USBKeys_Protocol);
			//d.println(USBKeys_Protocol == 1 ? " NKRO" : " Boot");
			d.moveCursor(0,4);

			//  layers stack  - -
			d.print("Layers: ");
			/*for (int l=1; l < layersCnt; ++l)  // 1st is 1 in menu
			{
				d.print(layersOn[l]);  d.moveCursor(6,0);
			}*/
			//d.println();  d.moveCursor(0,2);

		//	Demos::Font_ver(d, true);  //*
			break;

		case 1:
		{	//  locks
			d.print("Lock: ");  // Num Caps Scrl
			d.print("");  d.moveCursor(0,2);

			//  modifiers
			d.print("Mod:  ");
//			for (int i=KEY_LCTRL; i<=KEY_RGUI; ++i)
//			if (kk[i])
//			{	d.print(i >= KEY_RCTRL ? "R" : "L");
//				d.print(mod[i-KEY_LCTRL]);  d.print(" ");
//			}
			d.print("");  d.moveCursor(0,2);
			
			//  keys
			d.print("Key:  ");
			int c = 0;
//			for (int i = KEY_A; i < strALL; ++i)
//				if (kk[i])
//				{	d.print(STR(i));  d.print(" ");  ++c;  }
			//  count
			d.setCursor(W-1 -(c > 9 ? 2*6 : 6), H-1-8);
			d.print(c);
			
			//  ghosting -
//			if (ghost_cols /*|| ghost_rows*/)
			{
				d.setCursor(3*6, H-1-8);
//				d.print("Gho |");  d.print(ghost_cols);
//				d.print(" -");  d.print(ghost_rows);
			}
		}	break;
			
		case 2:
			//  config setup, build vars
//			d.print("Debounce ms: ");  d.println(MinDebounceTime_define);
			d.moveCursor(0,2);
			//d.print("Strobe delay us: ");  d.println(STROBE_DELAY);
			// rest in capabilities.kll *_define
			//DebounceThrottleDiv_define, StateWordSize_define;

//			d.print("Sequences: ");  d.println(iSlots);
			d.moveCursor(0,2);
//			d.print("Seq. max len: ");  d.println(iSeqLen);
			break;

		case 3:
			//  build vars
			///  dbg  mouse speed  --
			//const int16_t y1 = H-1-2*8, y2 = H-1-1*8;
			d.print("Mouse");
			d.moveCursor(0,4);
			d.print("hold delay spd");

//			d.setCursor(0, y1);  d.print(mx_holdtime);
//			d.setCursor(0, y2);  d.print(my_holdtime);
//
//			d.setCursor(W/3, y1);  d.print(mx_delay);
//			d.setCursor(W/3, y2);  d.print(my_delay);
//
//			d.setCursor(2*W/3, y1);  d.print(mx_speed);
//			d.setCursor(2*W/3, y2);  d.print(my_speed);
			break;
		}
		return;
	}
	
	
	#if 0
	if (mlevel==1 && ym == MSeq)
	if (!edit)
	{
		d.print("View");  d.setFont(0);
		
		//  list slots
		int s = page * iPage, i,n,x, xm;
		for (i=0; i < iPage && s < iSlots; ++i,++s)
		{
			d.setCursor(0, 18 + i*9);  //16 + i*8
			//d.print(i==slot ? "\x10":" ");   d.print(s);  d.print(": ");
			if (s<10)  d.print(" ");  //align
			d.print(s);  d.print(i==slot ? "\x10":" ");
			
			//  write sequence  ---
			n=0;  x=0;  xm=1;
			while (n < seql[s] && xm)
			{
				uint8_t z = seq[s][n];
				const char* st = STR(z);
				if (d.getCursorX() + 6*strlen(st) >= W -6*2)  // outside
				{	d.print(".");  d.moveCursor(-3,0);
					d.print(".");  xm = 0;  // more sign
				}else
				{	d.print(st);  //d.print(" ");
					d.moveCursor(z<=1 ? 0 : 2, 0);
					++n;
			}	}
		}
		//  page, center   / 
		d.setCursor(60, 4);
		d.print(page+1);  d.print("/");  d.print(iSlots/iPage);
		
		///  seq key
		if (tInfo == 0)
		{	int q = seqId();
			int l = strlen(csSeqKey[q]);
			d.setCursor(W-1 -l*8, 4);
			d.print(csSeqKey[q]);
		}
	}
	else
	{
		d.print("Edit");  d.setFont(0);
		int q = seqId();
		d.setCursor(W-1 -2*6, 4);  d.print(q);
		d.setCursor(W/2-6, 4);  d.print(edins ? "ins" : "ovr");
		//d.setCursor(W-1 -2*6, H-8);  d.print(edpos);

		//  write sequence  ---
		d.setCursor(0, 22);
		int n, l = seql[q];
		for (n=0; n <= l; ++n)  // +1 for cursor
		{
			int16_t x = d.getCursorX(), y = d.getCursorY();
			if (n < l)
			{
				uint8_t z = seq[q][n];
				d.print(STR(z));  //d.print(" ");
				d.moveCursor(z<=1 ? 0 : 2, 0);
			}
			if (n == edpos)  // show cursor
			{
				int16_t b = 8 - 8 * tBlnk / cBlnk;
//				if (edins)  // ins |
//					d.drawFastVLine(x, y-1-b+8, b+1, WHITE);
//				else  // ovr _
//					d.drawFastHLine(x, y+8, b+1, WHITE);
			}
		}
		++tBlnk;  // blink cur
		if (tBlnk > cBlnk)  tBlnk = 0;
	}
	#endif
	d.setFont(0);
	
	
	#if 0
	if (tInfo < 0)  // trigger
		tInfo = 400;  // par
	
	if (tInfo > 0)  //  info eeprom
	{	--tInfo;
		int x = 90;

		d.setFont(0);
		d.setCursor(x, 0);
		const static char* strInf[5] = {
			"Reset", "Loaded", "Saved:", "Copied", "Pasted" };
		d.print(strInf[infType]);
		if (infType == 1 || infType == 2)
		{
			d.setCursor(x+6, 8);
			d.print(memSize);  d.print(" B");
	}	}
	#endif
}
