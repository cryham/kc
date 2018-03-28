#include "WProgram.h"

#include "usb_keyboard.h"
#include "matrix.h"
#include "periodic.h"

#include "Ada4_ST7735.h"

#include "gui.h"


//  scan counter, freq
const static int rr = Matrix_colsNum;
uint scan_cnt = 0, scan_freq = 0;
uint32_t us_scan = 0, ms_scan = 0;


//  gui
int text = 1, keys = 0;

Gui gui;


//  kbd  timer event,  scan, send
//------------------------------------------------
void main_periodic()
{
	uint32_t us = micros();

	//  freq info
	uint32_t ms = millis(),
		ms_diff = ms - ms_scan;
	if (ms_diff >= 1000)  // 1s
	{
		ms_scan = ms;
		scan_freq = scan_cnt;  // Hz
		scan_cnt = 0;
	}
	++scan_cnt;


	//  kbd scan
	Matrix_scan( 0 );  // K


	//  kbd  gui
	//------------------------------------------------
	#define Key(y,x)  (Matrix_scanArray[y * rr + x].state == KeyState_Press ? 1 : 0)

	if (Key(5,0))  keys = 1-keys;
	if (!keys)
	{
		int8_t right = Key(1,1) - Key(1,2),  // ->  <-  next
			up = Key(0,4) - Key(2,4),    // Up+ Dn-  speed
			pgup = Key(2,1) - Key(0,1),  // PgDn+ PgUp-
			add = Key(0,6) - Key(2,6);  // Add+ Ent-

		//d = Key(5,0) - Key(3,0);  // F12+ F11-
		//d = Key(4,0) - Key(2,0);  // Ent+ \-
		//d = Key(2,2) - Key(0,2);  // End+ Hom-

		//if (add)
		//	text = (text + add + 4) % 4;

		/*	    0    1     2    3  4    5  6    7
			0        PgUp  Hom  U  ^    R  +    E
			1   Bck  ->    <-   Y  Del  T       F3
			2   \    PgDn  End  J  v    F  Ent  D
			3   F11  Del.  Spc  H  Ins  G  Up   F4
			4   Ent  *     Num  M  /    V       C
			5   F12  -          N       B			*/

		gui.KeyPress(right, up, pgup, add);
	}

	//  keyboard send
	//------------------------------------------------
	if (keys)
	{
		//int u = 0;
		for (uint c=0; c < Matrix_colsNum; ++c)
		for (uint r=0; r < Matrix_rowsNum; ++r)
		{
			int id = rr * r + c;
			const KeyState& k = Matrix_scanArray[id];
			uint kk = (r==3 && c==1) ? MODIFIERKEY_SHIFT : KEY_A + id;

			if (k.state == KeyState_Press)
			{
				Keyboard.press(kk);
				Keyboard.send_now();
			}
			else if (k.state == KeyState_Release)
			{
				Keyboard.release(kk);
				Keyboard.send_now();
			}
		}
	}

	us_scan = micros() - us;
	// 131 us del 3,  115 us del 2,  90 us del 0
}


//------------------------------------------------------------------------------------
int main()
{
	Ada4_ST7735 tft;
	tft.begin();

	gui.Init(&tft);


	//  kbd
	Matrix_setup();

	// 40000  1.2kHz  d:50fps
	// 50000  960Hz  d:52fps
	Periodic_init( 50000 );
	Periodic_function( &main_periodic );


	while(1)
	{

		gui.Draw();

		gui.DrawEnd();
	}
}
