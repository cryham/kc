#pragma once
#include <stdint.h>
#include <vector>
#include "kc_params.h"
#include "def.h"


//  errors load, save
enum KC_Err {
	E_ok=0, E_size,  E_H1, E_H2, E_ver,
	E_max
};
extern const char* KCerrStr[E_max];


//  main, state
//--------------------------------------
struct KC_Main
{
	//  current layer, set by keys
	int8_t nLayer = 0,  // final, held layer key (or set)
		   nLayerLock = -1;  // tap layer key to lock/unlock

	int8_t setDac = 1;  // update

	//  sequence running vars  0 norm, 1 from seq
	int8_t inSeq[2] = {-1,-1},	// id run, -1 none
		seqRel[2] = {0,0},		// pressed / released
		seqWait = 0;			// one time, restore dtSeq
	int16_t seqPos[2] = {0,0};	// cur index in seq data
	uint16_t dtSeq = 20;		// var delay  param
	uint32_t tiSeq = 0, tiFun = 0;  // ms time delay

	uint16_t xm=0, ym=0;  // abs mouse pos


	//  main  ----
	KC_Main();

	void UpdLay(uint32_t ms);  // update

	//  eeprom  ----
	void Load(), Save();
	uint16_t GetSize();  // mem

	uint16_t memSize = 0;  // result B
	KC_Err err = E_ok;


	//  inactive time(s) for stats
	uint32_t
		tm_on = 0,  // uptime   tm_= rtc, ms_= milis
		tm_key = 0, tm_keyOld = 0,  // last key press time, previous
		tm_keyAct = 0,
		msKeyLay = 0,  // layer key press, for lock times
		msMin1 = 0;    // 1 minute time
	uint16_t  // counts
		tInact1 = 0, tInact2 = 0,  // previous 2 inact mins in minutes
		tInactSum = 0,  // sum inactive times
		min1_Keys = 0;  // 1 minute keys press, show
	void ResetStats(bool rtc=false);

	//--inact-----|-active--|----------|---|----------------------
	//   0   1   2* *3 * 4  *5   6   7 * 8 * 9  10 now    * key press

	uint8_t grPMin[W];  // press/1min graph array
	uint8_t grPpos = 0;  // write pos
};

extern KC_Main kc;
