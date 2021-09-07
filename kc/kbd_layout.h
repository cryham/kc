#pragma once
#include <stdint.h>
#include "def.h"


//  Draw Keboard, Layout, const data
struct DrawKey
{
	int16_t x;  int8_t y, w,h;  // pos, size
	char c;  // caption char

	uint8_t o;  // color type
	uint8_t sc;  // NO or scan code for matrix
	uint8_t code;  // byte code, translated to usb key
};

#define NO 255  // no scan code


const static int8_t Omax = 10;
extern const int8_t kW, kH, kF, X;

extern const uint16_t clrRect[Omax], clrText[Omax];  // colors types


//  CK  layout
extern const char* CKname;

const static int nDrawKeys = 8;
extern const DrawKey drawKeys[nDrawKeys];

//  scan codes for gui keys
extern const uint8_t
	gLoad,gSave, gDiv, gEsc,gAdd,gEnt,
	gRight,gLeft, gDown,gUp, gPgUp,gPgDn, gEnd,gHome,
	gCtrl,gSh, gMul,gSub, gIns,gDel, gBckSp,gSpc;
