#pragma once
#include <stdint.h>
#include <vector>


enum KC_Info
{
	KC_1Key,  // single key,  code in data
	KC_Seq,   // sequence,  id in data
	KC_Layer, // layer key,  num in data
	KC_Func,  // special,
};

struct KC_Key
{
	KC_Info info;
	uint8_t layUse;
	std::vector<uint8_t> data; // [KC_MaxLayers];

	KC_Key()
		:info(KC_1Key)
		,layUse(0)  // nothing
	{
	}
};

struct KC_Sequence
{
	//  var length
	std::vector<uint8_t> data;
};

struct KC_Data
{
	uint8_t kk, ver;  // header
	uint16_t maxKeys;  // rows * cols, matrix

	std::vector<KC_Key> keys;
	std::vector<KC_Sequence> seqs;

	// ram
	//uint16_t adr[maxKeys];  // offset starts, if data^ was variable size
	//uint8_t lay[sc];  //layers used
};


#if 0
  - all in ram  > eeprom 32kB

  > syntax 	18x8 = 144 x 3B = 432B = sc
	1st ca: L layer occupancy 8bits per key, 8lay max
	2nd ca/var: 2-8 bytes: I, K x times used layers, from L
	  I bits: if single key K, or seq id in K
	  K (or I): extra codes for: layer keys, menu or const, mouse,
	  ^add ram arr[sc] for offset starts
	3rd var: byte seq num, word seq starts adr, len auto
	  then var seq bytes: keys or cmd eg. delay, modif dn,up?

	//  MSeq  ----  seq edit vars
	int8_t edit;   // seq 0 view / 1 edit
	int8_t edins;  // 1 ins 0 overwrite
	int8_t slot, page, edpos;  // edit vars
	int seqId();
	void SeqClear(int8_t q);
	uint8_t tBlnk;  // cur blink anim
	uint8_t seql[iSlots];  // lengths of each seq, 0 empty
	uint8_t seq[iSlots][iSeqLen];  // sequence data
	
	//  for copy
	uint8_t sql;
	uint8_t sq[iSeqLen];

	void Clear();  // ram
	void Load(), Save();  // eeprom
#endif
