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


const static int8_t Omax = 9;
extern const int8_t kW, kH, kF, X;

extern const uint16_t clrRect[Omax], clrText[Omax];  // colors types


//  CK  layout
extern const char* CKname;

#if defined(CK1)   //  CK1
const static int nDrawKeys = 91;
#elif defined(CK3)   //  CK3
const static int nDrawKeys = 103;
#endif
extern const DrawKey drawKeys[nDrawKeys];

//  scan codes for gui keys
extern const uint8_t
	gGui, gMslow, gLoad,gSave, gDiv,
	gRight,gLeft, gDown,gUp, gPgUp,gPgDn, gEnd,gHome, gAdd,gEnt,
	gCtrl,gSh, gMul,gSub, gIns,gDel,gBckSp, gC,gV,gX;
