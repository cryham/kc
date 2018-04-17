#pragma once
#include <stdint.h>
#include <vector>
#include "keys_usb.h"

/*  size:
  18x8 = 144  x 16L = 2304B max
  60 seq * 20 = 1200B  E=3500 big /ram
  144 *1+2L = 432B + 10seq *15  E=600B small
*/
const int KC_MaxLayers = 8;  //16


struct KC_Key  // for each scan code
{
	uint32_t layUse;  // KC_MaxLayers x 1bit
	std::vector<uint8_t> data;

	KC_Key() : layUse(0)  // nothing
	{  }

	//  test use bit
	#define hasLay(n)  (layUse & (1ul << n))

	//  get data for layer, if any
	uint8_t get(uint32_t lay);

	//  add data at layer
	void add(uint8_t code, uint32_t lay);

	//  remove layer data
	void rem(uint32_t lay);
};


struct KC_Sequence
{
	//  var length
	std::vector<uint8_t> data;

	const int len() const {  return int(data.size());  }
	void add(uint8_t b) {  data.push_back(b);  }
};


struct KC_Setup
{
	//  const
	uint8_t h1,h2, ver;  // header

	//  rows * cols = scanKeys,  matrix setup
	uint8_t rows, cols, scanKeys, seqSlots;  // max seqs

	std::vector<KC_Key> keys;  // size scanKeys, maps scan to byte codes
	std::vector<KC_Sequence> seqs;
	KC_Sequence copy;  // for paste, swap

	const int nkeys() const {  return keys.size();  }
	const int nseqs() const {  return seqs.size();  }

	KC_Setup();
	void Clear(), InitCK1_8x6();

	//int8_t minLay, layUsed;  // stats
};


//  main, state
//--------------------------------------
struct KC_Main
{
	//  current layer, set by keys
	int8_t nLayer = 0;

	//  sequence running vars
	int8_t inSeq = -1,  // id run, -1 none
		seqPos = 0,  // index in seq data
		seqRel = 0;  // pressed / released
	uint32_t tiSeq = 0;  // ms time delay
	uint16_t dtSeq = 20;  // var delay  param

	int8_t seqMod[K_ModLast+1];  // modifiers state
	void SeqModClear();


	//  params, saved  ----
	//--------------------------------------
	//  scan setup
	uint8_t debounce = 1, strobe_delay = 4;
	uint8_t scanFreq = 50;  // mul by 1 kHz

	//uint8_t offset;  // params size B
	iunt16_t dtSeqDef = 20;  // default dt for seq keys

	uint8_t verCounter = 0;  // inc on each save

	//  gui key auto repeat, ms
	uint16_t krDelay = 250, krRepeat = 60;

	//  brightnes, dac
	int8_t setDac = 1;  // upd
	int16_t valDac = 4000;
	// default screen, fade dac, time..
	//int8_t startScr

	//  mouse keys
	uint8_t mkSpeed = 100, mkAccel = 100;
	//--------------------------------------


	//  main  ----
	KC_Setup set;

	KC_Main();

	void UpdLay();  // update
	void Send(uint32_t ms);  // send usb

	//  eeprom  ----
	void //GetSize(),
		Load(), Save();
	uint16_t memSize = 0;  // result B

	char err[64];  // error string
};
