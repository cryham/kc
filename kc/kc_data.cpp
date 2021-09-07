#include "kc_data.h"
#include "kbd_layout.h"
#include "usb_keyboard.h"
#include "usb_mouse.h"
#include "matrix.h"
#include "keys_usb.h"
#include "gui.h"
#include "WProgram.h"

KC_Params par;
extern Gui gui;


//  update layers  (always)
//------------------------------------------------------------------------
void KC_Main::UpdLay(uint32_t ms)
{
	//  brightness dac led  ~~~
	if (setDac)
	{	setDac = 0;
		int bri = par.brightness;
		const int minBri = 3580;
		int val = bri == 0 ? 0 : bri * (4095 - minBri) / 100 + minBri;
		analogWriteDAC0(val);
	}

	//  1 minute time, stats
	if (par.time1min)
	if (ms - msMin1 > 1000 * t1min(par) || ms < msMin1)
	{
		msMin1 = ms;
		min1_Keys = cnt_press1min * 60 / t1min(par);
		cnt_press1min = 0;

		//  graph inc pos
		++grPpos;
		if (grPpos >= W)  grPpos = 0;
		//  add to graph
		grPMin[grPpos] = min1_Keys > 255 ? 255 : min1_Keys;
	}


	//  all matrix scan codes  ----
	uint c,r;  int id;
	for (c=0; c < NumCols; ++c)
	for (r=0; r < NumRows; ++r)
	{
		id = NumCols * r + c;  // scan id
		if (id >= set.nkeys())  continue;

		//  state
		const KeyState& k = Matrix_scanArray[id];
		bool on = k.state == KeyState_Press;
		bool off = k.state == KeyState_Release;

		bool hold = k.state == KeyState_Hold;
		uint8_t codeL = set.key[nLayer][id];
		bool fun = codeL >= K_Fun0 && codeL <= K_FunLast;

		if (on || off)
		{
			//  display, internal functions  ***
			//..........................................................
			if (on && fun)
			{
				switch (codeL)
				{
				case K_Fun1:
				case K_Fun2:  // brightness -+
					tiFun = ms;  break;  // delay no par

				case K_Fun3:  // soft reset  //NVIC_SystemReset();
					#define SCB_AIRCR (*(volatile uint32_t *)0xE000ED0C)
					SCB_AIRCR = 0x05FA0004;  break;

				case K_Fun4:  // light, led toggle
					#ifdef LED
					gui.led = 1 - gui.led;
					digitalWrite(LED, gui.led ? LOW : HIGH);
					#endif
					break;

				case K_Fun6:  // reset stats
					ResetStats(true);
					break;
				}
			}
		}
		else if (hold)  // repeat, funct
		{
			if (fun)
			if (ms - tiFun > par.krRepeat*5 || ms < tiFun)
			{	tiFun = ms;
				uint8_t& br = par.brightOff;
				switch (codeL)
				{
				case K_Fun1:  // brightness -+
					br = gui.RangeAdd(br, (gui.kCtrl ?-10 :-2), 0, 100);
					setDac = 1;  break;
				case K_Fun2:
					br = gui.RangeAdd(br, (gui.kCtrl ? 10 : 2), 0, 100);
					setDac = 1;  break;
				}
			}
		}
	}
}
