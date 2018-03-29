#pragma once
#include <stdint.h>


//  Draw Keboard, Layout, const data
struct SKey
{
	int16_t x;  int8_t y, w,h;  // pos, size
	char c;  // caption char

	uint8_t o;  // color type
	uint8_t sc;  // scan code for cur matrix
	//todo + menu key binds ..
};

#define NO 255  // no scan code


const static int8_t Omax = 9;
extern const int8_t kW, kH, kF, X, XN;

extern const uint16_t clrRect[Omax], clrText[Omax];  // colors types


//  CK3  layout
extern const char* CKname;
const static int numKeys = 94;
extern SKey drawKeys[numKeys];


#if 0
///  string from usb hid code
const static int strALL = 0x73;
extern const char str[strALL][6];

extern const char mod[0x08][6];

//  key to string  // wait, delay symbols,  modifiers, key
#define STR(k)  (k==2 ? "\xB1" : k==1 ? "\xB0" : \
				 k >= 0xE0 ? mod[k-0xE0] : \
				 k <= strALL ? str[k] : "()")
#endif
