#pragma once
#include <stdint.h>
#include <vector>
#include "keys_usb.h"

/*  size:
  18x8 = 144  x 16L = 2304B max
  60 seq * 20 = 1200B  E=3500 big /ram
  144 *1+2L = 432B + 10seq *15  E=600B
*/
const int KC_MaxLayers = 8;

struct KC_Key  // for each scan code
{
	uint32_t layUse;  // KC_MaxLayers x 1bit
	std::vector<uint8_t> data;

	KC_Key() : layUse(0)  // nothing
	{  }

	//  number of set bits
	int numLays()
	{
		uint8_t b = layUse;
		b = b - ((b >> 1) & 0x55);
		b = (b & 0x33) + ((b >> 2) & 0x33);
		return (((b + (b >> 4)) & 0x0F) * 0x01);
	}
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
};

struct KC_Setup
{
	uint8_t kk, ver;  // header

	//  rows * cols = scanKeys,  matrix setup
	uint8_t rows, cols, scanKeys;
	// ver num or date saved-

	std::vector<KC_Key> keys;  // size scanKeys
	std::vector<KC_Sequence> seqs;

	int nkeys(){  return keys.size();  }
	int nseqs(){  return seqs.size();  }

	KC_Setup();
	void Clear(), InitCK1_8x6();

	int8_t minLay, layUsed;  // stats
	// ram
	//uint16_t adr[maxKeys];  // offset starts, if data^ was variable size
	//uint8_t lay[sc];  //layers used
};

struct KC_Main  // main, state
{
	int nLayer = 0;

	int grpStart[grpMax], grpEnd[grpMax];

	KC_Setup set;

	KC_Main();

	void UpdLay(), Send();
};


#if 0
  - all in ram  > eeprom 32kB

	3rd var: byte seq num, word seq starts adr, len auto
	  then var seq bytes: keys or cmd eg. delay, modif dn,up?

	//  MSeq  ----  seq edit vars
	int8_t edit;   // seq 0 view / 1 edit
	int8_t edins;  // 1 ins 0 overwrite
	int8_t slot, page, edpos;  // edit vars
#endif
