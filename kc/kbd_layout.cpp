#include "kbd_layout.h"
#include "def.h"
#include "keys_usb.h"

const uint16_t  // _colors for key types_
//  0 letters  1 digits       2 func F1      3 symbols ,.   4 modifiers L  5 modif R      6 ent,tab      7 ent R        8 arrows
clrRect[Omax] = {
RGB(12,12,12), RGB(13,13,10), RGB( 8,14,19), RGB(21,21,12),	RGB(12,19,19), RGB(12,19,19), RGB( 9,21,12), RGB( 9,21,12), RGB(1,20,20) },
clrText[Omax] = {
RGB(25,28,31), RGB(28,28,23), RGB(20,26,30), RGB(29,29,16),	RGB( 9,26,26), RGB( 9,26,26), RGB(13,28,16), RGB(13,28,16), RGB(2,28,28) };


//  ------------  CK3  ------------------------------------------------
const char* CKname = "CK1 8x6";
const int8_t  kW = 8, kH = 9, kF = 6, /* size */  X = -kW, XN = 127;  // pos

const SKey drawKeys[numKeys] = {  //  Layout draw

{ 0, 9,kW,kF, 'e',6,NO,K_ESC}, //Esc 13x
{-13,0,kW,kF, '1',2,NO,K_F1},{X, 0,kW,kF, '2',2,NO,K_F2}, {X,0,kW,kF, '3',2,15,K_F3}, {X,0,kW,kF, '4',2,31,K_F4},
{-12,0,kW,kF, '5',2,NO,K_F5},{X, 0,kW,kF, '6',2,NO,K_F6}, {X,0,kW,kF, '7',2,NO,K_F7}, {X,0,kW,kF, '8',2,NO,K_F8},
{-12,0,kW,kF, '9',2,NO,K_F9},{X, 0,kW,kF, '0',2,NO,K_F10},{X,0,kW,kF, '1',2,24,K_F11},{X,0,kW,kF, '2',2,40,K_F12},//F12

{ 0,20, 8,kH, '`',3,NO,K_TILDE}, //~
{-8, 0,kW,kH, '1',1,NO,K_1},{X, 0,kW,kH, '2',1,NO,K_2},{X,0,kW,kH, '3',1,NO,K_3},{X,0,kW,kH, '4',1,NO,K_4},{X,0,kW,kH, '5',1,NO,K_5},
{ X, 0,kW,kH, '6',1,NO,K_6},{X, 0,kW,kH, '7',1,NO,K_7},{X,0,kW,kH, '8',1,NO,K_8},{X,0,kW,kH, '9',1,NO,K_9},{X,0,kW,kH, '0',1,NO,K_0},
{ X, 0,kW,kH, '-',3,NO,K_MINUS},{X, 0,kW,kH, '=',3,NO,K_EQUAL},{X,0,13,kH, '<',7, 8,K_BACK}, //Back 14x

{ 0,29,12,kH, 29 ,6,NO,K_TAB}, //Tab
{-12,0,kW,kH, 'Q',0,NO,K_Q},{X, 0,kW,kH, 'W',0,NO,K_W},{X,0,kW,kH, 'E',0, 7,K_E},{X,0,kW,kH, 'R',0, 5,K_R},{X,0,kW,kH, 'T',0,13,K_T},
{ X, 0,kW,kH, 'Y',0,11,K_Y},{X, 0,kW,kH, 'U',0, 3,K_U},{X,0,kW,kH, 'I',0,NO,K_I},{X,0,kW,kH, 'O',0,NO,K_O},{X,0,kW,kH, 'P',0,NO,K_P},
{ X, 0,kW,kH, '[',3,NO,K_LBRACE},{X, 0,kW,kH, ']',3,NO,K_RBRACE},{X,0, 9,kH,'\\',3,16,K_BSLASH}, // \| 14x

{ 0,38,15,kH, 249,6,NO,K_A}, //Caps
{-15,0,kW,kH, 'A',0,NO,K_A},{X, 0,kW,kH, 'S',0,NO,K_S},{X,0,kW,kH, 'D',0,23,K_D},{X,0,kW,kH, 'F',0,21,K_F},{X,0,kW,kH, 'G',0,29,K_G},
{ X, 0,kW,kH, 'H',0,27,K_H},{X, 0,kW,kH, 'J',0,19,K_J},{X,0,kW,kH, 'K',0,NO,K_K},{X,0,kW,kH, 'L',0,NO,K_L},{X,0,kW,kH, ';',3,NO,K_SEMIC},
{ X, 0,kW,kH,'\'',3,NO,K_QUOTE},{X, 0,14,kH, 217,7,32,K_ENT}, //Ent 13x

{ 0,47,18,kH, 's',4,NO,K_LSHIFT}, //L Sh
{-18,0,kW,kH, 'Z',0,NO,K_Z},{X, 0,kW,kH, 'X',0,NO,K_X},{X,0,kW,kH, 'C',0,39,K_C},{X,0,kW,kH, 'V',0,37,K_V},{X,0,kW,kH, 'B',0,45,K_B},
{ X, 0,kW,kH, 'N',0,43,K_N},{X, 0,kW,kH, 'M',0,35,K_M},{X,0,kW,kH, ',',3,NO,K_COMMA},{X,0,kW,kH, '.',3,NO,K_PERIOD},{X,0,kW,kH, '/',3,NO,K_SLASH},
{ X, 0,19,kH, 's',5,NO,K_RSHIFT}, //R Sh 12x

//modif:  L Ctrl, L Gui, L Alt, space, R Alt, App,K_Ss, R Gui, R Ctrl
{ 0,56,12,kH, 'c',4,NO,K_LCTRL},{-12,0,8,kH,  9 ,4,NO,K_LGUI},{-8,0,12,kH,'a',4,NO,K_LALT},{-12,0,45,kH,' ',6,26,K_SPACE}, //Space
{-45,0,10,kH, 'a',5,NO,K_RALT}, {-10,0,8,kH,  9 ,6,NO,K_RGUI},{-8,0, 9,kH,254,5,NO,K_MENU},{-9, 0,13,kH,'c',5,NO,K_RCTRL}, //R Ct 8x

//numpad: 17x
{XN,20,kW,kH, 250,6,34,K_NUML},{X, 0,kW,kH, '/',3,36,KP_DIV},{X,0,kW,kH, 'x',3,33,KP_MUL},{X,0,kW,kH,  '-',3,41,KP_SUB},
{XN,29,kW,kH, '7',8, 2,K_HOME},{X, 0,kW,kH, 24 ,8, 4,K_UP},  {X,0,kW,kH, '9',8, 1,K_PGUP}, {X,0,kW,kH*2,'+',3, 6,KP_ADD},
{XN,38,kW,kH, 27 ,8,10,K_LEFT},{X, 0,kW,kH,  7 ,6,12,K_DEL}, {X,0,kW,kH, 26 ,8, 9,K_RIGHT},
{XN,47,kW,kH, '1',8,18,K_END}, {X, 0,kW,kH, 25 ,8,20,K_DOWN},{X,0,kW,kH, '3',8,17,K_PGDN},{X,0,kW,kH*2, 217,6,22,KP_ENT}, //Ent
{XN,56,kW*2,kH,'_',6,28,K_INS},{-kW*2,0,kW,kH,'.',6,25,K_DEL}, //Ins Del
};
//symbols: `240=  248 deg  249. 250,  7 cir  9 dot`
	// < x1B 27  > x1A 26  ^ x18 24  v x19 25
