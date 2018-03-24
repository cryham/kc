#include "WProgram.h"

#include "usb_keyboard.h"
#include "matrix.h"

#ifdef	__cplusplus
extern "C"
{
#endif
#include "periodic.h"
#ifdef	__cplusplus
}
#endif

#include "demos.h"

#include "Ada4_ST7735.h"
#include "kbd_draw.h"

//  vars
unsigned long tm;
uint32_t oldti = 0;  // dt
uint frm_cnt = 0;   // frame counter

//#define pin 6

//  tim
uint tim_cnt = 0, tim_fq = 0;  // scan counter, freq
const static int rr = Matrix_colsNum;

uint8_t v = LOW;  // pin
uint32_t us_scan = 0, ms_scan = 0;


//  gui
int text = 1, key = 1;
int demo = D_Plasma;

Demos demos;


void main_periodic()
{
	uint32_t us = micros();

	//  freq info
	uint32_t ms = millis(),
		ms_diff = ms - ms_scan;
	if (ms_diff >= 1000)  // 1s
	{
		ms_scan = ms;
		tim_fq = tim_cnt;  // Hz
		tim_cnt = 0;
	}
	++tim_cnt;

	//  kbd scan
	Matrix_scan( 0 );  // K

	#ifdef pin
	digitalWriteFast(pin, v);
	v = v==LOW ? HIGH : LOW;
	#endif

	//  kbd  gui
	//------------------------------------------------
	#define Key(y,x)  (Matrix_scanArray[y * rr + x].curState == KeyState_Press ? 1 : 0)

	if (Key(5,0))  key = 1-key;
	//int d = Key(5,0) - Key(3,0);  // F12+ F11-
	//int d = Key(4,0) - Key(2,0);  // Ent+ \-
	//int d = Key(2,2) - Key(0,2);  // End+ Hom-
	int d = Key(2,1) - Key(0,1);  // PgDn+ PgUp-
	if (d)
	{	demo += d;  if (demo < 0) demo = D_All-1;
		if (demo >= D_All) demo = D_None;
	}

	d = Key(0,6) - Key(2,6);  // Add+ Ent-
	if (d)
		text = (text + d + 4) % 4;

	//if (K(0,1))
	//	key = 1 - key;
	/*
		-|  0    1     2    3  4    5  6    7
		0        PgUp  Hom  U  ^    R  +    E
		1   Bck  ->    <-   Y  Del  T       F3
		2   \    PgDn  End  J  v    F  Ent  D
		3   F11  Del.  Spc  H  Ins  G  Up   F4
		4   Ent  *     Num  M  /    V       C
		5   F12  -          N       B
	*/

	//  demo keys
	int k = Key(1,1) - Key(1,2),  // ->  <-  next
		e = Key(0,4) - Key(2,4);  // Up+ Dn-  speed
	demos.KeyPress((EDemo)demo, k, e, Key(3,4), Key(5,1));  // ins ct, - inf


	//  keyboard send
	//------------------------------------------------
	if (key)
	{
		//int u = 0;
		for (uint c=0; c < Matrix_colsNum; ++c)
		for (uint r=0; r < Matrix_rowsNum; ++r)
		{
			int id = rr * r + c;
			const KeyState& k = Matrix_scanArray[id];
			if (k.curState == KeyState_Press)
			{	Keyboard.press(KEY_A + id);
				Keyboard.send_now();
			}
			else if (k.curState == KeyState_Release)
			{	Keyboard.release(KEY_A + id);
				Keyboard.send_now();
			}
		}
	}

	us_scan = micros() - us;
	// 115 us,  90 us w/o strobe delay
}

//------------------------------------------------------------------------------------
int main()
{
//	Demos demos;

	Ada4_ST7735 tft;
	tft.begin();

	tm = rtc_get();

	#ifdef pin
	pinMode(pin, OUTPUT);
	digitalWriteFast(pin, LOW);
	#endif
	char a[128];


	demos.Init(&tft);

	Matrix_setup();

	// Setup periodic timer function
	// 30000  1.6kHz  d:47fps
	// 40000  1.2kHz  d:50fps
	// 50000  960Hz  d:52fps
	Periodic_init( 50000 );
	Periodic_function( &main_periodic );


	while(1)
	{
		//------------------------------------------------
		if (demo != D_Rain)
			tft.clear();

		uint32_t oti1 = millis();

		switch (demo)
		{
		case D_Plasma:   demos.Plasma();  break;  // clr 55, 40 ms
		case D_None:  break;
		case D_Space:    demos.Space();  break;
		case D_Balls:    demos.Balls();  break;
		case D_Rain:     demos.Rain();  break;
		case D_Fountain: demos.Fountain();  break;
		case D_Ngons:    demos.Ngons();  break;  // 12 8ms 14 10ms
		case D_Hedrons:  demos.Hedrons();  break;  // 5-9ms
		case D_Wave:     demos.Wave();  break;
		case D_CK_Logo:  demos.CK_logo();  break;  // 7
		case D_Fonts:	 demos.Font_ver(false);  break;
		case D_Chars:    demos.Chars();  break;
		}
		uint32_t tdemo = millis() - oti1;

		// txt 55fps 3ln
		//------------------------------------------------
		if (text)
		{
			//  kbd draw  ---------
			if (text == 3)
			{
				int x = 0, y = 0;
				for (int i=0; i < numKeys; ++i)
				{
					const SKey& k = keys[i];
					if (k.x >=0)  x = k.x;  else  x -= k.x;
					if (k.y > 0)  y = k.y + 60; /*Y*/  else  y -= k.y;
	
					tft.drawRect(x, y, k.w+1, k.h+1, clrRect[k.o]);
					tft.setCursor(
						k.o==5 || k.o==7 ? x + k.w - 6 :
								k.o==3 ? x+1 : x+1,
						k.h < H ? y :
								k.o==3 ? y+1 : y+1);
					tft.setTextColor(clrText[k.o]);
					sprintf(a,"%c", k.c);
					tft.print(a);
				}
			}
			tft.setTextColor(0xFFFF);

			//  matrix  -------
			if (text == 2)
			for (uint c=0; c < Matrix_colsNum; ++c)
			for (uint r=0; r < Matrix_rowsNum; ++r)
			{
				tft.setCursor(c*12, 48 + r*8);
				const KeyState& k = Matrix_scanArray[Matrix_colsNum * r + c];
				if (k.curState == KeyState_Off)
				{	sprintf(a,".");
					tft.setTextColor(RGB(12,20,26));
				}else{
					sprintf(a,"%d", k.curState);
					tft.setTextColor(RGB(21,26,31));
				}
				tft.print(a);
			}
			tft.setTextColor(RGB(24,28,31));


			//  fps  ---------
			uint32_t ti = millis();
			tft.setCursor(0,0);
			float fr = 1000.f / (ti - oldti);
			int ff = fr;

			sprintf(a,"%d %lu %u %lu", ff, tdemo, tim_fq, us_scan);
			tft.print(a);
			oldti = ti;

			//  time  ---
			if (text == 3)
			{
				tm = rtc_get();
				int h = tm/3600%24, m = tm/60%60, s = tm%60;
				tft.setCursor(0,8);

				sprintf(a,"%2d:%02d:%02d  %d", h,m,s, frm_cnt);
				tft.print(a);
			}

			//  keys
			if (text == 2)
			{
				tft.setCursor(0,20);
				sprintf(a,"P %d  R %d  H %d", cnt_press, cnt_rel, cnt_hold);
				tft.print(a);
			}
		}

		++frm_cnt;


		if (text || demo > D_None)
		{
			tft.display();  // 58 Fps, 15ms @144MHz
		}
	}
}
