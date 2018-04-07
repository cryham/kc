#pragma once
#include <stdint.h>
#include <vector>
#include "keys_usb.h"

/*  size:
  18x8 = 144  x 16L = 2304B max
  60 seq * 20 = 1200B  E=3500 big /ram
  144 *1+2L = 432B + 10seq *15  E=600B small
*/
const int KC_MaxLayers = 8;

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
};

struct KC_Setup
{
	uint8_t kk, ver;  // header

	//  rows * cols = scanKeys,  matrix setup
	uint8_t rows, cols, scanKeys;
	// ver num or date saved-

	std::vector<KC_Key> keys;  // size scanKeys, maps scan to byte codes
	std::vector<KC_Sequence> seqs;
	KC_Sequence copy;  // for paste, swap

	const int nkeys() const {  return keys.size();  }
	const int nseqs() const {  return seqs.size();  }

	KC_Setup();
	void Clear(), InitCK1_8x6();

	int8_t minLay, layUsed;  // stats
	// ram
	//uint16_t adr[maxKeys];  // offset starts, if data^ was variable size
	//uint8_t lay[sc];  //layers used
};

struct KC_Main  // main, state
{
	//  current layer, by keys
	int8_t nLayer = 0;

	//  sequence running vars
	int8_t inSeq = -1,  // id run, -1 none
		seqPos = 0,  // code id in seq data
		seqRel = 0;  // pressed / released
	int8_t seqMod[K_ModLast+1];  // modifs state
	uint32_t tiSeq = 0;
	void SeqModClear();

	int8_t setDac = 1;
	int16_t valDac = 3900;

	//  const from grp
	uint8_t grpStart[grpMax], grpEnd[grpMax];

	KC_Setup set;

	KC_Main();

	void UpdLay();
	void Send(uint32_t ms);
};
