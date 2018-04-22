#pragma once
#include <stdint.h>

/*  size:
  18x8 = 144  x 16L = 2304B max
  60 seq * 20 = 1200B  E=3500 big /ram
  144 *1+2L = 432B + 10seq *15  E=600B small
*/
const int KC_MaxLayers = 8;  //16


//  params, saved
//--------------------------------------
struct KC_Params
{
	//  scan setup
	uint8_t debounce;  // ms
	//  delay in microseconds before and after each strobe change during matrix scan
	uint8_t strobe_delay;
	//  scanFreq * 1000 = timer period, 50 is 960 Hz
	uint8_t scanFreq;

	//  brightnes, dac value, 4095 = 100%
	int16_t valDac;

	//  todo default screen
	int8_t startScreen;

	uint8_t verCounter;  // inc on each save

	//  gui key auto repeat, ms
	uint8_t krDelay, krRepeat;
	//  mouse keys
	uint8_t mkSpeed, mkAccel;

	//  sequences default keys dt
	uint8_t dtSeqDef;

	//  todo fade dac, time..
};
//  --- ADD new to END ----


#ifdef __cplusplus
extern "C" {
#endif

extern struct KC_Params par;

#ifdef __cplusplus
}
#endif

