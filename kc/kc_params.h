#pragma once
#include <stdint.h>
#include "def.h"

/*  data mem sizes
//--------------------------------------
  18x8 = 144  * 9L = 1296 B max
     + 60 seq * 20 = 1200 B
  = 2500 B  RAM big

  144 keys * (1 size + 2 Lay data) = 432 B
    + 10 seq * 15 len = 150 B
  = 600 B  EE small
*/
const static int8_t
	KC_MaxRows = 8,
	KC_MaxCols = 18,
	KC_MaxLayers = 8,  // 16
	KC_MaxSeqs = 40;  // 60


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

	//  brightness led, 0..100%
	uint8_t brightness, brightOff;
	int8_t startScreen;  // screen after start

	uint8_t verCounter;  // inc on each save

	//  gui key auto repeat, ms
	uint8_t krDelay, krRepeat;
	uint8_t mkSpeed, mkAccel;

	//* sequence, default keys dt
	uint8_t dtSeqDef;
	//  default layer 0, sequence edit layer 2
	uint8_t defLayer, editLayer;
	uint8_t keyGui;  // scId for gui toggle

	//  todo par, low pri
	//  start demo screen
	//  auto start seq to login, after time
	//  inactive time show, start demo-
};
//  --- ADD new to END ----


enum EStScr  //  start screen
{	ST_main0 = 0,  ST_Test, ST_Test2,
	ST_Map= ST_Test2 +T_All,  ST_Seqs, ST_Displ,
	ST_Demos, ST_Demos2,  ST_ALL= ST_Demos2
	#ifdef DEMOS
		+D_All,
	#endif
};

#ifdef __cplusplus
extern "C" {
#endif

extern struct KC_Params par;

#ifdef __cplusplus
}
#endif
