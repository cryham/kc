#pragma once

//  draw kbd

//typedef enum Eclr { E0=0,E1,Eclr
struct SKey
{
	int16_t x;  int8_t y, w,h;  // pos, size
	char c;  // caption char
	uint8_t o;  // color type:
};

const static int8_t Emax = 9, kW = 8, kH = 9, X = -kW, G = 6, NX = 127;  // add x, size
//const int16_t  W = 8, X = -W,  H = 9, G = 6,  NX = 127;  // add x, size


#define RGB(r,g,b) ((r<<11)+ ((g)<<6) +b)	// R F800  G 07E0  B 001F  R 32 G 64 B 32

const uint16_t  // [colors]
	// 0 letters,  1 numbers,   2 func,   3 symbols,
	// 4 modif L,  5 modif R,   6 enter,  7 enter R,   8 arrows
clrRect[Emax] = { RGB( 6, 6, 9), RGB( 8, 8, 5), RGB( 8,14,19), RGB(23,23,26),
	RGB(12,19,19), RGB(12,19,19), RGB( 9,21,12), RGB( 9,21,12),  RGB( 6,12,18) },
clrText[Emax] = { RGB(27,29,31), RGB(31,31,21), RGB(27,29,31), RGB(28,28,31),
	RGB( 6,26,26), RGB( 6,26,26),  RGB(13,31,16), RGB(13,31,16),  RGB(11,21,31) };


const int numKeys = 94;
SKey drawKeys[numKeys] = {  ///  keys draw

{ 0,  9,kW,G, 'e',6}, //esc 13x
{-13, 0,kW,G, '1',2},{X, 0,kW,G, '2',2},{X,0,kW,G, '3',2},{X,0,kW,G, '4',2},
{-12, 0,kW,G, '5',2},{X, 0,kW,G, '6',2},{X,0,kW,G, '7',2},{X,0,kW,G, '8',2},
{-12, 0,kW,G, '9',2},{X, 0,kW,G, '0',2},{X,0,kW,G, '1',2},{X,0,kW,G, '2',2},//f12

{ 0,20, 8,kH, '`',3}, //`
{-8, 0,kW,kH, '1',1},{X, 0,kW,kH, '2',1},{X,0,kW,kH, '3',1},{X,0,kW,kH, '4',1},{X,0,kW,kH, '5',1},
{ X, 0,kW,kH, '6',1},{X, 0,kW,kH, '7',1},{X,0,kW,kH, '8',1},{X,0,kW,kH, '9',1},{X,0,kW,kH, '0',1},
{ X, 0,kW,kH, '-',3},{X, 0,kW,kH, '=',3},{X,0,13,kH, '<',7}, //bck 14x

{ 0,29,12,kH, '-',6}, //tab
{-12,0,kW,kH, 'Q',0},{X, 0,kW,kH, 'W',0},{X,0,kW,kH, 'E',0},{X,0,kW,kH, 'R',0},{X,0,kW,kH, 'T',0},
{ X, 0,kW,kH, 'Y',0},{X, 0,kW,kH, 'U',0},{X,0,kW,kH, 'I',0},{X,0,kW,kH, 'O',0},{X,0,kW,kH, 'P',0},
{ X, 0,kW,kH, '[',3},{X, 0,kW,kH, ']',3},{X,0, 9,kH, '\\',3}, // \| 14x

{ 0,38,15,kH, ' ',6}, //caps
{-15,0,kW,kH, 'A',0},{X, 0,kW,kH, 'S',0},{X,0,kW,kH, 'D',0},{X,0,kW,kH, 'F',0},{X,0,kW,kH, 'G',0},
{ X, 0,kW,kH, 'H',0},{X, 0,kW,kH, 'J',0},{X,0,kW,kH, 'K',0},{X,0,kW,kH, 'L',0},{X,0,kW,kH, ';',3},
{ X, 0,kW,kH,'\'',3},{X, 0,14,kH, '|',7}, //ent 13x

{ 0,47,18,kH, 's',4}, //L Sh
{-18,0,kW,kH, 'Z',0},{X, 0,kW,kH, 'X',0},{X,0,kW,kH, 'C',0},{X,0,kW,kH, 'V',0},{X,0,kW,kH, 'B',0},
{ X, 0,kW,kH, 'N',0},{X, 0,kW,kH, 'M',0},{X,0,kW,kH, ',',3},{X,0,kW,kH, '.',3},{X,0,kW,kH, '/',3},
{ X, 0,19,kH, 's',5}, //R Sh 12x

//L Ctrl, L Gui, L Alt, space, R Alt, Apps, R Gui, R Ctrl
{ 0,56,12,kH, 'c',4},{-12,0,8,kH, '.',4},{-8,0,12,kH,'a',4},{-12,0,45,kH,' ',6}, //spc
{-45,0,10,kH, 'a',5},{-10,0,8,kH, '-',6},{-8,0, 9,kH,'.',5},{-9, 0,13,kH,'c',5}, //R Ct 8x

//num 20x
{NX,20,kW,kH, ' ',6},{X, 0,kW,kH, '/',3},{X,0,kW,kH, '*',3},{X,0,kW,kH, '-',3},
{NX,29,kW,kH, '7',1},{X, 0,kW,kH, '^',8},{X,0,kW,kH, '9',1},{X,0,kW,kH*2, '+',3},
{NX,38,kW,kH, '<',8},{X, 0,kW,kH, '.',6},{X,0,kW,kH, '>',8},
{NX,47,kW,kH, '1',1},{X, 0,kW,kH, 'v',8},{X,0,kW,kH, '3',1},{X,0,kW,kH*2, '|',6}, //ent
{NX,56,kW*2,kH,'_',6},{-kW*2,0,kW,kH,'.',6}, //ins del
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
