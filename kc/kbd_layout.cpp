#include "kbd_layout.h"
#include "def.h"
#include "keys_usb.h"

const uint16_t  // _colors for key types_
//  0 letters  1 digits       2 func F1      3 symbols ,.   4 modifiers L  5 modif R      6 ent,tab      7 ent R        8 arrows
clrRect[Omax] = {
RGB(12,12,12), RGB(13,13,10), RGB( 8,14,19), RGB(21,21,12),	RGB(12,19,19), RGB(12,19,19), RGB( 9,21,12), RGB( 9,21,12), RGB(1,20,20) },
clrText[Omax] = {
RGB(25,28,31), RGB(28,28,23), RGB(20,26,30), RGB(29,29,16),	RGB( 9,26,26), RGB( 9,26,26), RGB(13,28,16), RGB(13,28,16), RGB(2,28,28) };
#define  wf  kW,kF
#define  wh  kW,kH

#if defined(CK1)   //  CK1  ------------------------------------------------
const char* CKname = "CK1 8x6";  const int16_t XN = 127;  // pos
const int8_t  kW = 8, kH = 9, kF = 6, /* size */  X = -kW;

const DrawKey drawKeys[nDrawKeys] = {  //  Layout draw
{ 0, 9,wf, 'e',6,NO,K_ESC}, //Esc 13x
{-13,0,wf, '1',2,NO,K_F1},{X, 0,wf, '2',2,NO,K_F2}, {X,0,wf, '3',2,15,K_F3}, {X,0,wf, '4',2,31,K_F4},
{-12,0,wf, '5',2,NO,K_F5},{X, 0,wf, '6',2,NO,K_F6}, {X,0,wf, '7',2,NO,K_F7}, {X,0,wf, '8',2,NO,K_F8},
{-12,0,wf, '9',2,NO,K_F9},{X, 0,wf, '0',2,NO,K_F10},{X,0,wf, '1',2,24,K_F11},{X,0,wf, '2',2,40,K_F12},//F12

{ 0,20, 8,kH, '`',3,NO,K_TILDE}, //~
{-8, 0,wh, '1',1,NO,K_1},{X, 0,wh, '2',1,NO,K_2},{X,0,wh, '3',1,NO,K_3},{X,0,wh, '4',1,NO,K_4},
{ X, 0,wh, '5',1,NO,K_5},{X, 0,wh, '6',1,NO,K_6},{X,0,wh, '7',1,NO,K_7},{X,0,wh, '8',1,NO,K_8},
{ X, 0,wh, '9',1,NO,K_9},{X, 0,wh, '0',1,NO,K_0},
{X,0,wh, '-',3,NO,K_MINUS},{X, 0,wh, '=',3,NO,K_EQUAL},{X,0,13,kH, '<',7, 8,K_BACK}, //Back 14x

{ 0,29,12,kH, 29 ,6,NO,K_TAB}, //Tab
{-12,0,wh, 'Q',0,NO,K_Q},{X, 0,wh, 'W',0,NO,K_W},{X,0,wh, 'E',0, 7,K_E},{X,0,wh, 'R',0, 5,K_R},
{ X, 0,wh, 'T',0,13,K_T},{X, 0,wh, 'Y',0,11,K_Y},{X,0,wh, 'U',0, 3,K_U},{X,0,wh, 'I',0,NO,K_I},
{ X, 0,wh, 'O',0,NO,K_O},{X, 0,wh, 'P',0,NO,K_P},{X,0,wh, '[',3,NO,K_LBRACE},
{X, 0,wh, ']',3,NO,K_RBRACE},{X,0, 9,kH,'\\',3,16,K_BSLASH}, // \| 14x

{ 0,38,15,kH, 249,6,NO,K_A}, //Caps
{-15,0,wh, 'A',0,NO,K_A},{X, 0,wh, 'S',0,NO,K_S},{X,0,wh, 'D',0,23,K_D},{X,0,wh, 'F',0,21,K_F},
{ X, 0,wh, 'G',0,29,K_G},{X, 0,wh, 'H',0,27,K_H},{X,0,wh, 'J',0,19,K_J},{X,0,wh, 'K',0,NO,K_K},
{ X, 0,wh, 'L',0,NO,K_L},{X,0,wh, ';',3,NO,K_SEMIC},
{ X, 0,wh,'\'',3,NO,K_QUOTE},{X, 0,14,kH, 217,7,32,K_ENT}, //Ent 13x

{ 0,47,18,kH, 's',4,NO,K_LSHIFT}, //L Sh
{-18,0,wh, 'Z',0,NO,K_Z},{X, 0,wh, 'X',0,NO,K_X},{X,0,wh, 'C',0,39,K_C},{X,0,wh, 'V',0,37,K_V},
{ X, 0,wh, 'B',0,45,K_B},{X, 0,wh, 'N',0,43,K_N},{X,0,wh, 'M',0,35,K_M},{X,0,wh, ',',3,NO,K_COMMA},
{ X, 0,wh, '.',3,NO,K_PERIOD},{X,0,wh, '/',3,NO,K_SLASH},
{ X, 0,19,kH, 's',5,NO,K_RSHIFT}, //R Sh 12x

//modif:  L Ctrl, L Gui, L Alt, space, R Alt, App,K_Ss, R Gui, R Ctrl
{ 0,56,12,kH, 'c',4,NO,K_LCTRL},{-12,0,8,kH,  9 ,4,NO,K_LGUI},{-8,0,12,kH,'a',4,NO,K_LALT},
{-12,0,45,kH, ' ',6,26,K_SPACE}, //Space
{-45,0,10,kH, 'a',5,NO,K_RALT}, {-10,0,8,kH,  9 ,6,NO,K_RGUI},{-8,0, 9,kH,254,5,NO,K_MENU},
{-9, 0,13,kH, 'c',5,NO,K_RCTRL}, //R Ct 8x
//numpad: 17x
{XN,20,wh, 250,6,34,K_NUML},{X, 0,wh, '/',3,36,KP_DIV},{X,0,wh, 'x',3,33,KP_MUL},{X,0,wh,  '-',3,41,KP_SUB},
{XN,29,wh, '7',8, 2,K_HOME},{X, 0,wh, 24 ,8, 4,K_UP},  {X,0,wh, '9',8, 1,K_PGUP},{X,0,kW,kH*2,'+',3, 6,KP_ADD},
{XN,38,wh, 27 ,8,10,K_LEFT},{X, 0,wh,  7 ,6,12,K_DEL}, {X,0,wh, 26 ,8, 9,K_RIGHT},
{XN,47,wh, '1',8,18,K_END}, {X, 0,wh, 25 ,8,20,K_DOWN},{X,0,wh, '3',8,17,K_PGDN},{X,0,kW,kH*2, 217,6,22,KP_ENT}, //Ent
{XN,56,kW*2,kH,'_',6,28,K_INS},{-kW*2,0,wh,'.',6,25,K_DEL}, //Ins Del
};
//symbols: `240=  248 deg  249. 250,  7 cir  9 dot`
const uint8_t gGui=40, gMslow=32, gLoad=15,gSave=31,gDiv=36,
	gRight=9,gLeft=10,gDown=20,gUp=4, gPgDn=17,gPgUp=1,gEnd=18,gHome=2,gAdd=6,gEnt=22,
	gCtrl=26,gSh=16,gMul=33,gSub=41, gIns=28,gDel=25,gBckSp=8,gC=39,gV=37,gX=45;

#elif defined(CK3)   //  CK3  ------------------------------------------------
const char* CKname = "CK3 18x8";  const int16_t XR = 116, XN = 128;  // pos
const int8_t  kW = 8, kH = 9, /* size */ kF = 6,  X = -kW;

const DrawKey drawKeys[nDrawKeys] = {  //  Layout draw
{ 0, 9,wf, 'e',6, 15,K_ESC},{X,0,wf, 'p',6,140,K_PRTSCR},  //Esc 15x
{ X, 0,wf, '1',2,139,K_F1},{X, 0,wf, '2',2,103,K_F2}, {X,0,wf, '3',2, 13,K_F3}, {X,0,wf, '4',2,110,K_F4},
{-12,0,wf, '5',2, 95,K_F5},{X, 0,wf, '6',2,131,K_F6}, {X,0,wf, '7',2,127,K_F7}, {X,0,wf, '8',2,109,K_F8},
{-12,0,wf, '9',2, 19,K_F9},{X, 0,wf, '0',2,111,K_F10},{X,0,wf, '1',2, 75,K_F11},{X,0,wf, '2',2, 74,K_F12},//F12
 {XR,0,wf, '.',6, 57,0},  //Displ

{ 0,20, 8,kH, '`',3,141,K_TILDE}, //~
{-8, 0,wh, '1',1,123,K_1},{X, 0,wh, '2',1,122,K_2},{X,0,wh, '3',1,121,K_3},{X,0,wh, '4',1,116,K_4},
{ X, 0,wh, '5',1,134,K_5},{X, 0,wh, '6',1,133,K_6},{X,0,wh, '7',1,115,K_7},{X,0,wh, '8',1,114,K_8},
{ X, 0,wh, '9',1,113,K_9},{X, 0,wh, '0',1,108,K_0},
{ X, 0,wh, '-',3,126,K_MINUS},{X, 0,wh, '=',3,132,K_EQUAL},{X,0,12,kH, '<',7, 91,K_BACK}, //Back 14x
 {XR,0,wh, 'e',6,138,K_ESC},

{ 0,29,12,kH, 29 ,6,105,K_TAB}, //Tab
{-12,0,wh, 'Q',0, 87,K_Q},{X, 0,wh, 'W',0, 86,K_W},{X,0,wh, 'E',0, 85,K_E},{X,0,wh, 'R',0, 80,K_R},
{ X, 0,wh, 'T',0, 98,K_T},{X, 0,wh, 'Y',0, 97,K_Y},{X,0,wh, 'U',0, 79,K_U},{X,0,wh, 'I',0, 78,K_I},
{ X, 0,wh, 'O',0, 77,K_O},{X, 0,wh, 'P',0, 72,K_P},{X,0,wh, '[',3, 90,K_LBRACE},
{ X, 0,wh, ']',3, 96,K_RBRACE},{X,0, 8,kH,'\\',3, 55,K_BSLASH}, // \| 14x
 {XR,0,wh, '<',7,137,K_BACK},

{ 0,38,15,kH, 249,6,104,K_CAPS}, //Caps
{-15,0,wh, 'A',0, 69,K_A},{X, 0,wh, 'S',0, 68,K_S},{X,0,wh, 'D',0, 67,K_D},{X,0,wh, 'F',0, 62,K_F},
{ X, 0,wh, 'G',0,  8,K_G},{X, 0,wh, 'H',0,  7,K_H},{X,0,wh, 'J',0, 61,K_J},{X,0,wh, 'K',0, 60,K_K},
{ X, 0,wh, 'L',0, 59,K_L},{X, 0,wh, ';',3, 54,K_SEMIC},
{ X, 0,wh,'\'',3,  0,K_QUOTE},{X, 0,13,kH, 217,7, 37,K_ENT}, //Ent 13x
 {XR,0,wh, 217,7,119,K_ENT},

{ 0,47,18,kH, 's',4, 94,K_LSHIFT}, //L Sh
{-18,0,wh, 'Z',0, 51,K_Z},{X, 0,wh, 'X',0, 50,K_X},{X,0,wh, 'C',0, 49,K_C},{X,0,wh, 'V',0, 44,K_V},
{ X, 0,wh, 'B',0, 26,K_B},{X, 0,wh, 'N',0, 25,K_N},{X,0,wh, 'M',0, 43,K_M},{X,0,wh, ',',3, 42,K_COMMA},
{ X, 0,wh, '.',3, 41,K_PERIOD},{X,0,wh, '/',3, 18,K_SLASH},
{ X, 0,10,kH, 's',5, 58,K_RSHIFT}, {X,0,8,kH, '^',5, 58,K_UP}, //R Sh 12x
 {XR,0,wh, 's',7,120,K_RSHIFT},/*12 up  120 end*/

 //modif:  L Ctrl, L Gui, L Alt, space, R Alt, App,K_Ss, R Gui, R Ctrl
{ 0,56,11,kH, 'c',4,128,K_LCTRL},{-12,0,7,kH,  9 ,4,106,K_LGUI},{-12,0,7,kH,  '\\',4, 14,K_SLASH},
{-8, 0,11,kH, 'a',4,  3,K_LALT},{-12,0,3,kH,' ',6,  9,K_SPACE}, //Space
{-45,0, 9,kH, 'a',5, 21,K_RALT}, /*23 menu 136 ins 135 del  30 ri 27 dn  28 le */
{-10,0, 8,kH,  9 ,6, 23,K_RGUI},{-8,0, 9,kH,254,5,136,K_MENU},{-9, 0,12,kH,'c',5,135,K_RCTRL}, //R Ct 8x
{-9, 0,12,kH, '>',5,135,K_RIGHT},{-9, 0,12,kH,'v',5,135,K_DOWN},
 {XR,0,wh, 'c',7,28,K_RCTRL},
//numpad: 17x
{XN,20,wh, 250,6, 45,K_NUML},{X, 0,wh, '/',3, 46,KP_DIV},{X,0,wh, 'x',3, 47,KP_MUL},{X,0,kW-2,kH,  '-',3, 29,KP_SUB},
{XN,29,wh, '7',8, 81,K_HOME},{X, 0,wh, 24 ,8, 82,K_UP},  {X,0,wh, '9',8, 83,K_PGUP},{X,0,kW-2,kH*2,'+',3, 84,KP_ADD},
{XN,38,wh, 27 ,8, 99,K_LEFT},{X, 0,wh,  7 ,6,100,K_DEL}, {X,0,wh, 26 ,8,101,K_RIGHT},
{XN,47,wh, '1',8, 63,K_END}, {X, 0,wh, 25 ,8, 64,K_DOWN},{X,0,wh, '3',8, 65,K_PGDN},{X,0,kW-2,kH*2, 217,6, 66,KP_ENT}, //Ent
{XN,56,kW*2,kH,'_',6, 10,K_INS},{-kW*2,0,wh,'.',6, 11,K_DEL}, //Ins Del
};
const uint8_t gGui=57, gMslow=94, gLoad=95,gSave=110,gDiv=46,
	gRight=101,gLeft=99,gDown=64,gUp=82, gPgUp=65,gPgDn=83,gEnd=63,gHome=81,gAdd=84,gEnt=66,
	gCtrl=128,gSh=94,gMul=47,gSub=29, gIns=10,gDel=11,gBckSp=91,gC=49,gV=44,gX=50;
#endif

#undef wf
#undef wh
