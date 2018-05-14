#include "WProgram.h"
#include "RamMonitor.h"

#include "matrix.h"
#include "periodic.h"
#include "kc_data.h"

#include "Ada4_ST7735.h"
#include "gui.h"


//  scan counter, freq
uint scan_cnt = 0, scan_freq = 0;
uint32_t us_scan = 0, ms_scan = 0;
uint8_t scan_n = 0;

RamMonitor ram;
Gui gui;
KC_Main kc;
extern void ParInit();


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
	++scan_n;


	//  kbd scan
	bool bsc = false;
	if (gui.kbdSend ||  // slower for demos
		gui.ym != M_Demos || scan_n % 4==0)
	{	Matrix_scan(0);  bsc = true;  }  // K


	//  gui keys
	//------------------------
	if (bsc)
		gui.KeyPress();


	//  keyboard send
	//------------------------
	kc.UpdLay(ms);

	if (gui.kbdSend)
		kc.Send(ms);


	//  scan time vs strobe delay
	// 570 us: 10,  353 us: 4  18x8
	// 147 us: 4,  90: 0  8x6
	us_scan = micros() - us;
}


//  main
//-------------------------------------------------------------------------
int main()
{
	ram.initialize();
	ParInit();  // par defaults

	//  dac for tft led
	analogWriteRes(12);
	analogWriteDAC0(0);  // dark


	Ada4_ST7735 tft;
	tft.begin();

	gui.Init(&tft);
	tft.clear();
	tft.display();  // black


	//  load set from ee
	kc.Load();
	gui.SetScreen(par.startScreen);
	gui.kbdSend = 1;  // release


	//  kbd
	Matrix_setup();

	//  48 MHz/50 000 = 960 Hz   d: 52 fps
	Periodic_init( par.scanFreq * 1000 );
	Periodic_function( &main_periodic );

	#ifdef LED
	pinMode(LED, OUTPUT);
	#endif

	while(1)
	{
		ram.run();

		gui.Draw();

		gui.DrawEnd();
	}
}
