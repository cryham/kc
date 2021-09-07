#include "kc_data.h"
//#include "usb_keyboard.h"
#include "usb_mouse.h"
#include "matrix.h"
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
		/*if (id >= set.nkeys())  continue;

		//  state
		const KeyState& k = Matrix_scanArray[id];
		bool on = k.state == KeyState_Press;
		bool off = k.state == KeyState_Release;

		bool hold = k.state == KeyState_Hold;
		uint8_t codeL = set.key[nLayer][id];
		//bool fun = codeL >= K_Fun0 && codeL <= K_FunLast;
		*/
	}
}
