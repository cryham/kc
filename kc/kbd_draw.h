#pragma once

//  Draw Keboard, Layout, const data
struct SKey
{
	int16_t x;  int8_t y, w,h;  // pos, size
	char c;  // caption char

	uint8_t o;  // color type
	uint8_t sc;  // scan code for cur matrix
};
#define NO 255  // no scan code

const static int8_t Omax = 9, kW = 8, kH = 9, X = -kW, G = 6, NX = 127;  // add x, size

const uint16_t  // _colors types_
	// 0 letters,  1 numbers,   2 func,   3 symbols,
	// 4 modif L,  5 modif R,   6 enter,  7 enter R,   8 arrows
clrRect[Omax] = {
	RGB( 6, 6, 9), RGB(11,11, 8), RGB( 8,14,19), RGB(23,23,26),
	RGB(12,19,19), RGB(12,19,19), RGB( 9,21,12), RGB( 9,21,12), RGB(21,16,12) },
clrText[Omax] = {
	RGB(25,28,31), RGB(28,28,23), RGB(18,25,31), RGB(28,28,31),
	RGB( 6,26,26), RGB( 6,26,26), RGB(13,31,16), RGB(13,31,16), RGB(31,26,21) };

//  ------------  CK3  ------------------------------------------------
const char* CKname = "CK3";
const int numKeys = 94;
SKey drawKeys[numKeys] = {  ///  keys draw
		//todo + menu key binds ..
{ 0,  9,kW,G, 'e',6,NO}, //esc 13x
{-13, 0,kW,G, '1',2,NO},{X, 0,kW,G, '2',2,NO},{X,0,kW,G, '3',2,NO},{X,0,kW,G, '4',2,NO},
{-12, 0,kW,G, '5',2,NO},{X, 0,kW,G, '6',2,NO},{X,0,kW,G, '7',2,NO},{X,0,kW,G, '8',2,NO},
{-12, 0,kW,G, '9',2,NO},{X, 0,kW,G, '0',2,NO},{X,0,kW,G, '1',2,NO},{X,0,kW,G, '2',2,NO},//f12

{ 0,20, 8,kH, '`',3,NO}, //`
{-8, 0,kW,kH, '1',1,NO},{X, 0,kW,kH, '2',1,NO},{X,0,kW,kH, '3',1,NO},{X,0,kW,kH, '4',1,NO},{X,0,kW,kH, '5',1,NO},
{ X, 0,kW,kH, '6',1,NO},{X, 0,kW,kH, '7',1,NO},{X,0,kW,kH, '8',1,NO},{X,0,kW,kH, '9',1,NO},{X,0,kW,kH, '0',1,NO},
{ X, 0,kW,kH, '-',3,NO},{X, 0,kW,kH, '=',3,NO},{X,0,13,kH, '<',7,NO}, //bck 14x

{ 0,29,12,kH, '-',6,NO}, //tab
{-12,0,kW,kH, 'Q',0,NO},{X, 0,kW,kH, 'W',0,NO},{X,0,kW,kH, 'E',0,NO},{X,0,kW,kH, 'R',0,NO},{X,0,kW,kH, 'T',0,NO},
{ X, 0,kW,kH, 'Y',0,NO},{X, 0,kW,kH, 'U',0,NO},{X,0,kW,kH, 'I',0,NO},{X,0,kW,kH, 'O',0,NO},{X,0,kW,kH, 'P',0,NO},
{ X, 0,kW,kH, '[',3,NO},{X, 0,kW,kH, ']',3,NO},{X,0, 9,kH,'\\',3,NO}, // \| 14x

{ 0,38,15,kH, ' ',6,NO}, //caps
{-15,0,kW,kH, 'A',0,NO},{X, 0,kW,kH, 'S',0,NO},{X,0,kW,kH, 'D',0,NO},{X,0,kW,kH, 'F',0,NO},{X,0,kW,kH, 'G',0,NO},
{ X, 0,kW,kH, 'H',0,NO},{X, 0,kW,kH, 'J',0,NO},{X,0,kW,kH, 'K',0,NO},{X,0,kW,kH, 'L',0,NO},{X,0,kW,kH, ';',3,NO},
{ X, 0,kW,kH,'\'',3,NO},{X, 0,14,kH, '|',7,NO}, //ent 13x

{ 0,47,18,kH, 's',4,NO}, //L Sh
{-18,0,kW,kH, 'Z',0,NO},{X, 0,kW,kH, 'X',0,NO},{X,0,kW,kH, 'C',0,NO},{X,0,kW,kH, 'V',0,NO},{X,0,kW,kH, 'B',0,NO},
{ X, 0,kW,kH, 'N',0,NO},{X, 0,kW,kH, 'M',0,NO},{X,0,kW,kH, ',',3,NO},{X,0,kW,kH, '.',3,NO},{X,0,kW,kH, '/',3,NO},
{ X, 0,19,kH, 's',5,NO}, //R Sh 12x

//modif: L Ctrl, L Gui, L Alt, space, R Alt, Apps, R Gui, R Ctrl
{ 0,56,12,kH, 'c',4,NO},{-12,0,8,kH, '.',4,NO},{-8,0,12,kH,'a',4,NO},{-12,0,45,kH,' ',6,NO}, //spc
{-45,0,10,kH, 'a',5,NO},{-10,0,8,kH, '-',6,NO},{-8,0, 9,kH,'.',5,NO},{-9, 0,13,kH,'c',5,NO}, //R Ct 8x

//numpad: 20x
{NX,20,kW,kH, ' ',6,NO},{X, 0,kW,kH, '/',3,NO},{X,0,kW,kH, '*',3,NO},{X,0,kW,kH, '-',3,NO},
{NX,29,kW,kH, '7',1,NO},{X, 0,kW,kH, '^',8,NO},{X,0,kW,kH, '9',1,NO},{X,0,kW,kH*2, '+',3,NO},
{NX,38,kW,kH, '<',8,NO},{X, 0,kW,kH, '.',6,NO},{X,0,kW,kH, '>',8,NO},
{NX,47,kW,kH, '1',1,NO},{X, 0,kW,kH, 'v',8,NO},{X,0,kW,kH, '3',1,NO},{X,0,kW,kH*2, '|',6,NO}, //ent
{NX,56,kW*2,kH,'_',6,NO},{-kW*2,0,kW,kH,'.',6,NO}, //ins del
};


#if 0
///  string from usb hid code
const int strALL = 0x73;
const char str[strALL][6] = { ".",".",".",".", /*04*/"A","B","C","D",
/*08*/"E","F","G","H","I","J","K","L", /*10*/"M","N","O","P","Q","R","S","T",
/*18*/"U","V","W","X","Y","Z","1","2", /*20*/"3","4","5","6","7","8","9","0",
/*28*/"Ent","Esc","Bck","Tab","Spc",
/*2D*/"-","=","[","]","\\", /*32*/"NUM-",";","\"","`",",",".", "/","CapsL",
/*3A*/"F1","F2","F3","F4", "F5", "F6",
/*40*/"F7","F8","F9","F10","F11","F12", /*46*/"PtrSc","ScrL","Pause",
/*49*/"Ins","Home","PgUp", "Del","End","PgDn", //*4F*/"Right","Left","Down","Up",
/*4F*/{26,0},{27,0},{25,0},{24,0},
/*53*/"NumL","P/","P*","P-","P+","PEnt",
/*59*/"P1","P2","P3","P4","P5","P6","P7","P8","P9","P0","P.",
/*68*/"F13","F14","F15","F16","F17","F18","F19","F20","F21","F22","F23","F24"};
/*..*/
const char mod[0x08][6] = {
/*E0*/"Ct","Sh","Al","Gui", //L
/*E4*/"Ct","Sh","Al","Gui", //R
};

//  key to string  // wait, delay symbols,  modifiers, key
#define STR(k)  (k==2 ? "\xB1" : k==1 ? "\xB0" : \
				 k >= 0xE0 ? mod[k-0xE0] : \
				 k <= strALL ? str[k] : "()")

///  Key for sequence  - update after ck4layer2.kll change
const char csSeqKey[Gui::iSlots][5] = {
#if 0  // ck4
	"1","2","3", "Q","W",
	"E",";","'", "/","\\",
	"Home","Left", "End","Ins","A",
	"S","D","Z","X","C"  };
#else  // ck3
	"1","2","3", "Q","W",
	"E","\\","Back", "Home","Ent",
	"Left", ";", "'","/","A",
	"S","D","Z","X","C"  };
#endif
#endif
