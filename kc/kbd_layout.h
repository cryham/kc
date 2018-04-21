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
extern const int8_t kW, kH, kF, X, XN;

extern const uint16_t clrRect[Omax], clrText[Omax];  // colors types


//  CK  layout
extern const char* CKname;

#if defined(CK1)   //  CK1
const static int nDrawKeys = 91;
#elif defined(CK3)   //  CK3
const static int nDrawKeys = 91;
#endif
extern const DrawKey drawKeys[nDrawKeys];
