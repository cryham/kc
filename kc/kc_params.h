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
	KC_MaxSeqs = 60;  // 60


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
	uint8_t mkSpeed, mkAccel;  // mouse keys

	//* sequence, default keys delay
	uint8_t dtSeqDef;

	uint8_t defLayer;   // default layer 0
	uint8_t editLayer;  // sequence edit layer 2
	uint8_t keyGui;  // scId for gui toggle

	uint8_t keyMouseSlow;  // mouse shift scId
	uint8_t mkWhSpeed, mkWhAccel;  // mouse wheel
	uint8_t quickKeys;  // F1..12 access

	//  layer lock
	uint8_t msLLTapMax;   // *10ms  max fast press (tap) time to lock layer
	uint8_t msLLHoldMin;  // *100ms min hold press time to lock layer

	//  ofs
	int8_t rtcCompensate; // xtal capacitance, adjust ppm
	uint8_t minInactive;  // minimum minutes for inactive
	int8_t tempOfs;       // adj Temp val  *0.03'C  +-3.8'C

	//  time intervals
	uint8_t time1min;     // *6s  press/1min
	uint8_t timeTemp;     // Temp'C read
	uint8_t timeTgraph;   // Temp'C add to graph, no average
	//  Temp graph scale
	uint8_t minTemp, maxTemp;
	uint8_t xCur;         // cursor

	#define t1min(par)    (6 * par.time1min)  // s
	#define tTemp(par)    (100 * gIntervals[par.timeTemp   & gIntvMask])  // ms
	#define tTgraph(par)  (100 * gIntervals[par.timeTgraph & gIntvMask])
};
//  --- ADD new to END ----
//  set defaults in ParInit()


enum EStartScr  //  start screen
{	ST_Main0 = 0,
	ST_Test,
	ST_Test2, ST_Test2Max = ST_Test2 +T_All,
	ST_Map = ST_Test2Max,
	ST_Seqs,
	ST_Displ,
	ST_Clock, ST_ClockMax = ST_Clock +Cl_All,
	ST_Help = ST_ClockMax,
	ST_Setup,
	ST_Info, ST_Info2, ST_Info2Max = ST_Info2 +I_All,
	ST_Setup2 = ST_Info2Max, ST_Setup2Max = ST_Setup2 +S_All,
	ST_Demos = ST_Setup2Max,
	ST_Demos2,  ST_ALL = ST_Demos2 //=ST_Demos2Max
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
