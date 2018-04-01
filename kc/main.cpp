#include "WProgram.h"

#include "usb_keyboard.h"
#include "matrix.h"
#include "periodic.h"
#include "keys_usb.h"

#include "Ada4_ST7735.h"
#include "gui.h"


//  scan counter, freq
uint scan_cnt = 0, scan_freq = 0;
uint32_t us_scan = 0, ms_scan = 0;

int keys = 0;  // send usb

Gui gui;
KC_Setup kc;


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
	if (Key(5,0))  keys = 1-keys;
	if (!keys)
	{
		gui.KeyPress();
	}

	//  todo  keyboard send  use kc
	//------------------------------------------------
	if (keys && kc.nkeys() >= int(ScanKeys))
	{
		for (uint c=0; c < NumCols; ++c)
		for (uint r=0; r < NumRows; ++r)
		{
			uint id = NumCols * r + c;  // scan id
			const KeyState& k = Matrix_scanArray[id];
			//  state
			bool on = k.state == KeyState_Press;
			bool off = k.state == KeyState_Release;
			if (on || off)
			{
				//  get from kc
				uint8_t code = kc.keys[id].get(0);  // curLayer
				if (code != KEY_NONE)
				{
					uint usb = usbKey[code];

					if (on)
					{
						Keyboard.press(usb);
						Keyboard.send_now();
					}
					else if (off)
					{
						Keyboard.release(usb);
						Keyboard.send_now();
					}
				}
			}
		}
	}

	//  147 us: strobe del 4  131: 3  115: 2  90: 0
	us_scan = micros() - us;
}


//  main
//-------------------------------------------------------------------------
int main()
{
	Ada4_ST7735 tft;
	tft.begin();

	gui.Init(&tft);


	//  kbd
	Matrix_setup();

	//  40000  1.2 kHz  d: 50 fps
	//  50000  960 Hz   d: 52 fps
	Periodic_init( 50000 );
	Periodic_function( &main_periodic );


	while(1)
	{
		gui.Draw();

		gui.DrawEnd();
	}
}
