#pragma once

//  draw kbd

//typedef enum Eclr { E0=0,E1,Eclr
struct SKey
{
	int16_t x;  int8_t y, w,h;  // pos, size
	char c;  // caption char
	uint8_t o;  // color type:
};

#define Emax 9
#define W 8
#define X -W
#define H 9
#define G 6
#define NX 127  // add x, size
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
SKey keys[numKeys] = {  ///  keys draw

{ 0,  9, W,G, 'e',6}, //esc 13x
{-13, 0, W,G, '1',2},{ X,  0, W,G, '2',2},{ X,  0, W,G, '3',2},{ X,  0, W,G, '4',2},
{-12, 0, W,G, '5',2},{ X,  0, W,G, '6',2},{ X,  0, W,G, '7',2},{ X,  0, W,G, '8',2},
{-12, 0, W,G, '9',2},{ X,  0, W,G, '0',2},{ X,  0, W,G, '1',2},{ X,  0, W,G, '2',2},//f12

{ 0, 20, 8,H, '`',3}, //`
{-8,  0, W,H, '1',1},{ X,  0, W,H, '2',1},{ X,  0, W,H, '3',1},{ X,  0, W,H, '4',1},{ X,  0, W,H, '5',1},{ X,  0, W,H, '6',1},
{ X,  0, W,H, '7',1},{ X,  0, W,H, '8',1},{ X,  0, W,H, '9',1},{ X,  0, W,H, '0',1},{ X,  0, W,H, '-',3},{ X,  0, W,H, '=',3},
{ X,  0,13,H, '<',7}, //bck 14x

{ 0, 29,12,H, '-',6}, //tab
{-12, 0, W,H, 'Q',0},{ X,  0, W,H, 'W',0},{ X,  0, W,H, 'E',0},{ X,  0, W,H, 'R',0},{ X,  0, W,H, 'T',0},{ X,  0, W,H, 'Y',0},
{ X,  0, W,H, 'U',0},{ X,  0, W,H, 'I',0},{ X,  0, W,H, 'O',0},{ X,  0, W,H, 'P',0},{ X,  0, W,H, '[',3},{ X,  0, W,H, ']',3},
{ X,  0, 9,H, '\\',3}, // \| 14x

{ 0, 38,15,H, ' ',6}, //caps
{-15, 0, W,H, 'A',0},{ X,  0, W,H, 'S',0},{ X,  0, W,H, 'D',0},{ X,  0, W,H, 'F',0},{ X,  0, W,H, 'G',0},{ X,  0, W,H, 'H',0},
{ X,  0, W,H, 'J',0},{ X,  0, W,H, 'K',0},{ X,  0, W,H, 'L',0},{ X,  0, W,H, ';',3},{ X,  0, W,H, '\'',3},
{ X,  0,14,H, '|',7}, //ent 13x

{ 0, 47,18,H, 's',4}, //L Sh
{-18, 0, W,H, 'Z',0},{ X,  0, W,H, 'X',0},{ X,  0, W,H, 'C',0},{ X,  0, W,H, 'V',0},{ X,  0, W,H, 'B',0},{ X,  0, W,H, 'N',0},
{ X,  0, W,H, 'M',0},{ X,  0, W,H, ',',3},{ X,  0, W,H, '.',3},{ X,  0, W,H, '/',3},
{ X,  0,19,H, 's',5}, //R Sh 12x

//L Ctrl, L Gui, L Alt, space, R Alt, Apps, R Gui, R Ctrl
{ 0, 56,12,H, 'c',4},{-12, 0, 8,H, '.',4},{-8,  0,12,H, 'a',4},{-12, 0,45,H, ' ',6}, //spc
{-45, 0,10,H, 'a',5},{-10, 0, 8,H, '-',6},{-8,  0, 9,H, '.',5},{-9,  0,13,H, 'c',5}, //R Ct 8x

//num 20x
{NX, 20, W,H, ' ',6},{ X,  0, W,H, '/',3},{ X,  0, W,H, '*',3},{ X,  0, W,H, '-',3},
{NX, 29, W,H, '7',1},{ X,  0, W,H, '^',8},{ X,  0, W,H, '9',1},{ X,  0, W,H*2, '+',3},
{NX, 38, W,H, '<',8},{ X,  0, W,H, '.',6},{ X,  0, W,H, '>',8},
{NX, 47, W,H, '1',1},{ X,  0, W,H, 'v',8},{ X,  0, W,H, '3',1},{ X,  0, W,H*2, '|',6}, //ent
{NX, 56, W*2,H, '_',6},{-W*2,  0, W,H, '.',6}, //ins del
};
