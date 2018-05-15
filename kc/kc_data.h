#pragma once
#include <stdint.h>
#include <vector>
#include "keys_usb.h"
#include "kc_params.h"


struct KC_Sequence
{
	//  var length
	std::vector<uint8_t> data;

	const int len() const {  return int(data.size());  }
	void add(uint8_t b) {  data.push_back(b);  }
};

//  seq commands
enum ESeqCmd {
	CMD_SetDelay=0, CMD_Wait, CMD_Comment, CMD_Hide,
	//  mouse move: x,y, buttons: press,release, click,double, wheels: x,y
	CM_x,CM_y, CM_BtnOn,CM_BtnOff, CM_Btn,CM_Btn2x, CM_WhX,CM_WhY,
	CMD_RunSeq, CM_xbig,CM_ybig, CM_xset,CM_yset,CM_mset, CMD_ALL
};	// note: need >= sequence ids than commands

extern const uint16_t cCmdClrLn[CMD_ALL];  // underline colors
extern const uint8_t cCmdStrLen[CMD_ALL];  // string lengths


struct KC_Setup
{
	//  const
	uint8_t h1,h2, ver;  // header

	//  rows * cols = scanKeys,  matrix setup
	uint8_t rows, cols, scanKeys, seqSlots;  // max seqs

	//  maps scan to byte codes
	//  const size: layers * scanKeys
	uint8_t key[KC_MaxLayers][KC_MaxRows * KC_MaxCols];

	KC_Sequence seqs[KC_MaxSeqs];
	KC_Sequence copy;  // for paste, swap

	const int nkeys() const {  return KC_MaxRows * KC_MaxCols;  }
	const int nseqs() const {  return KC_MaxSeqs;  }

	KC_Setup();
	void Clear(), InitCK();

	//int8_t minLay, layUsed;  // stats
};


//  errors load, save
enum KC_Err {
	E_ok=0, E_size,  E_H1, E_H2, E_ver,
	E_rows, E_cols,  E_slots, E_lay,
	E_rcEq, E_nkeys,  E_max
};
extern const char* KCerrStr[E_max];


//  main, state
//--------------------------------------
struct KC_Main
{
	//  current layer, set by keys
	int8_t nLayer = 0;
	int8_t setDac = 1;  // update

	//  sequence running vars  0 norm, 1 from seq
	int8_t inSeq[2] = {-1,-1},	// id run, -1 none
		seqRel[2] = {0,0},		// pressed / released
		seqWait = 0;			// one time, restore dtSeq
	int16_t seqPos[2] = {0,0};	// cur index in seq data
	uint16_t dtSeq = 20;		// var delay  param
	uint32_t tiSeq = 0, tiFun = 0;  // ms time delay

	int8_t seqMod[K_ModLast+1];  // modifiers state
	void SeqModClear(), QuitSeq();
	uint16_t xm=0, ym=0;  // abs mouse pos


	//  main  ----
	KC_Setup set;

	KC_Main();

	void UpdLay(uint32_t ms);  // update
	void Send(uint32_t ms);  // send usb
	bool SeqEnd(int lev, const KC_Sequence& sq);

	//  eeprom  ----
	void Load(), Save();
	uint16_t GetSize();  // mem

	uint16_t memSize = 0;  // result B
	KC_Err err = E_ok;

	unsigned long tm_key = 0;  // for stats
};

extern KC_Main kc;
