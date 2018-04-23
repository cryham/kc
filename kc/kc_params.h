#pragma once
#include <stdint.h>

/*  data mem sizes
//--------------------------------------
  18x8 = 144  * 9L = 1296 B max
	+ 60 seq * 20 = 1200 B
  = 2500 B  RAM big

  144 keys * (1 size + 2 Lay data) = 432 B
	+ 10 seq * 15 len = 150 B
  = 600 B  EE small
*/
const int KC_MaxRows = 8;
const int KC_MaxCols = 18;
const int KC_MaxLayers = 8;  // 16
const int KC_MaxSeqs = 40;  // 60


enum EStScr  //  start screen
{	ST_main0=0, ST_Test=1, ST_Test2=2,
	ST_Map=10, ST_Seqs=11, ST_Displ=12,
	ST_Demos=13, ST_Demos2=13,
};

//  params, saved
//--------------------------------------
struct KC_Params
{
	//* scan setup
	uint8_t debounce;  // ms
	//  delay before and after each strobe
	uint8_t strobe_delay;  // us
	//  scanFreq * 1000 = timer period, 50 is 960 Hz
	uint8_t scanFreq;

	//  brightness led, dac value, 4095 = 100%
	uint8_t brightness, brightOff;

	//  todo default screen
	int8_t startScreen;

	uint8_t verCounter;  // inc on each save

	//  gui key auto repeat, ms
	uint8_t krDelay, krRepeat;
	//  mouse keys
	uint8_t mkSpeed, mkAccel;

	//* sequences default keys dt
	uint8_t dtSeqDef;

	//  fade time
	uint8_t fadeTime;
};
//  --- ADD new to END ----


#ifdef __cplusplus
extern "C" {
#endif

extern struct KC_Params par;

#ifdef __cplusplus
}
#endif

