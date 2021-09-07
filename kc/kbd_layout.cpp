#include "kbd_layout.h"
#include "def.h"
#include "keys_usb.h"


const uint16_t clrRect[Omax] = {  // _colors for key types_
//  0 letters  1 digits       2 func F1      3 symbols ,.   4 modifiers L  5 modif R
RGB(12,12,12), RGB(13,13,10), RGB( 8,14,19), RGB(21,21,12),	RGB(12,19,19), RGB(12,19,19),
//  6 ent,tab  7 ent R        8 arrows      9 tiny
RGB( 9,21,12), RGB( 9,21,12), RGB(1,20,20), RGB(17,18,18) };


//  CK1  ------------------------------------------------
#define  wf  kW,kF
#define  wh  kW,kH
const char* CKname = "CK1 8x6";  const int16_t XN = 127;  // pos
const int8_t  kW = 8, kH = 9, kF = 6, /* size */  X = -kW;

const DrawKey drawKeys[nDrawKeys] = {  //  Layout draw
//numpad: 17x
{XN,29,wh, 24 ,8, 4,K_UP},  {X,0,kW,kH*2,'+',3, 6,KP_ADD},
{XN,47,wh, '1',8,18,K_END}, {X, 0,wh, 25 ,8,20,K_DOWN},{X,0,wh, '3',8,17,K_PGDN},{X,0,kW,kH*2, 28,6,22,KP_ENT},
{XN,56,kW*2,kH,'_',6,28,K_INS},{-kW*2,0,wh,'.',6,25,K_DEL},
};
//symbols:  2. 3,  7 cir  9 dot` 28 ent

const uint8_t gLoad=15,gSave=31,gDiv=36, gEsc=0,gAdd=6,gEnt=22,
	gRight=9,gLeft=10,gDown=20,gUp=4, gPgDn=17,gPgUp=1,gEnd=18,gHome=2,
	gCtrl=26,gSh=16,gMul=33,gSub=41, gIns=28,gDel=25, gBckSp=8,gSpc=26;

#undef wf
#undef wh
