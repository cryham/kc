#pragma once
#include <stdint.h>
#include <vector>
#include "keys_usb.h"


enum KC_Info  // data purpose
{
	KC_1Key=0, // single key, code in data
	KC_Seq,    // sequence,   id in data
	KC_Layer,  // layer hold, num in data
	KC_Func,   // special,
	KC_InfAll
};

const uint8_t KC_MaxLayers = 8;

struct KC_Key
{
	KC_Info info;
	uint8_t layUse;  // KC_MaxLayers x 1bit
	std::vector<uint8_t> data;

	KC_Key()
		:info(KC_1Key)
		,layUse(0)  // nothing
	{  }

	uint8_t get(int lay)  // get data for layer, if any
	{
		// layUse  1101..
		// eg. lay 0123..  data id =
		if (lay == 0 && (layUse & 0x1) && !data.empty())
			return data[0];
		return KEY_NONE;
	}
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


#if 0
  - all in ram  > eeprom 32kB

  > syntax 	18x8 = 144 =ca
	1st ca: L layer occupancy 8bits per key, 8lay max

	2nd var: 2-8 bytes: I, K x times used layers, from L
	  I bits: if single key K, or seq id in K
	  K (or I): extra codes for: layer keys, menu or const, mouse,
	  ^add ram arr[sc] for offset starts

	3rd var: byte seq num, word seq starts adr, len auto
	  then var seq bytes: keys or cmd eg. delay, modif dn,up?

	//  MSeq  ----  seq edit vars
	int8_t edit;   // seq 0 view / 1 edit
	int8_t edins;  // 1 ins 0 overwrite
	int8_t slot, page, edpos;  // edit vars
	void SeqClear(int8_t q);
	uint8_t tBlnk;  // cur blink anim
	uint8_t seql[iSlots];  // lengths of each seq, 0 empty
	uint8_t seq[iSlots][iSeqLen];  // sequence data
#endif
