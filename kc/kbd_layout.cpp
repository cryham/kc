#include "kbd_layout.h"
#include "def.h"

const uint16_t  // _colors types_
	// 0 letters,  1 numbers,   2 func,   3 symbols,
	// 4 modif L,  5 modif R,   6 enter,  7 enter R,   8 arrows
clrRect[Omax] = {
	RGB(12,12,12), RGB(13,13,10), RGB( 8,14,19), RGB(23,23,26),
	RGB(12,19,19), RGB(12,19,19), RGB( 9,21,12), RGB( 9,21,12), RGB(21,16,12) },
clrText[Omax] = {
	RGB(25,28,31), RGB(28,28,23), RGB(18,25,31), RGB(25,25,31),
	RGB( 6,26,26), RGB( 6,26,26), RGB(13,29,16), RGB(13,29,16), RGB(31,26,21) };

//  ------------  CK3  ------------------------------------------------
SKey drawKeys[numKeys] = {  //  Layout draw

{ 0, 9,kW,kF, 'e',6,NO}, //Esc 13x
{-13,0,kW,kF, '1',2,NO},{X, 0,kW,kF, '2',2,NO},{X,0,kW,kF, '3',2,15},{X,0,kW,kF, '4',2,31},
{-12,0,kW,kF, '5',2,NO},{X, 0,kW,kF, '6',2,NO},{X,0,kW,kF, '7',2,NO},{X,0,kW,kF, '8',2,NO},
{-12,0,kW,kF, '9',2,NO},{X, 0,kW,kF, '0',2,NO},{X,0,kW,kF, '1',2,24},{X,0,kW,kF, '2',2,40},//F12

{ 0,20, 8,kH, '`',3,NO}, //~
{-8, 0,kW,kH, '1',1,NO},{X, 0,kW,kH, '2',1,NO},{X,0,kW,kH, '3',1,NO},{X,0,kW,kH, '4',1,NO},{X,0,kW,kH, '5',1,NO},
{ X, 0,kW,kH, '6',1,NO},{X, 0,kW,kH, '7',1,NO},{X,0,kW,kH, '8',1,NO},{X,0,kW,kH, '9',1,NO},{X,0,kW,kH, '0',1,NO},
{ X, 0,kW,kH, '-',3,NO},{X, 0,kW,kH, '=',3,NO},{X,0,13,kH, '<',7, 8}, //Back 14x

{ 0,29,12,kH, 29 ,6,NO}, //Tab
{-12,0,kW,kH, 'Q',0,NO},{X, 0,kW,kH, 'W',0,NO},{X,0,kW,kH, 'E',0, 7},{X,0,kW,kH, 'R',0, 5},{X,0,kW,kH, 'T',0,13},
{ X, 0,kW,kH, 'Y',0,11},{X, 0,kW,kH, 'U',0, 3},{X,0,kW,kH, 'I',0,NO},{X,0,kW,kH, 'O',0,NO},{X,0,kW,kH, 'P',0,NO},
{ X, 0,kW,kH, '[',3,NO},{X, 0,kW,kH, ']',3,NO},{X,0, 9,kH,'\\',3,16}, // \| 14x

{ 0,38,15,kH, 249,6,NO}, //Caps
{-15,0,kW,kH, 'A',0,NO},{X, 0,kW,kH, 'S',0,NO},{X,0,kW,kH, 'D',0,23},{X,0,kW,kH, 'F',0,21},{X,0,kW,kH, 'G',0,29},
{ X, 0,kW,kH, 'H',0,27},{X, 0,kW,kH, 'J',0,19},{X,0,kW,kH, 'K',0,NO},{X,0,kW,kH, 'L',0,NO},{X,0,kW,kH, ';',3,NO},
{ X, 0,kW,kH,'\'',3,NO},{X, 0,14,kH, 217,7,32}, //Ent 13x

{ 0,47,18,kH, 's',4,NO}, //L Sh
{-18,0,kW,kH, 'Z',0,NO},{X, 0,kW,kH, 'X',0,NO},{X,0,kW,kH, 'C',0,39},{X,0,kW,kH, 'V',0,37},{X,0,kW,kH, 'B',0,45},
{ X, 0,kW,kH, 'N',0,43},{X, 0,kW,kH, 'M',0,35},{X,0,kW,kH, ',',3,NO},{X,0,kW,kH, '.',3,NO},{X,0,kW,kH, '/',3,NO},
{ X, 0,19,kH, 's',5,NO}, //R Sh 12x

//modif:  L Ctrl, L Gui, L Alt, space, R Alt, Apps, R Gui, R Ctrl
{ 0,56,12,kH, 'c',4,NO},{-12,0,8,kH,  9 ,4,NO},{-8,0,12,kH,'a',4,NO},{-12,0,45,kH,' ',6,26}, //Space
{-45,0,10,kH, 'a',5,NO},{-10,0,8,kH,  9 ,6,NO},{-8,0, 9,kH,254,5,NO},{-9, 0,13,kH,'c',5,NO}, //R Ct 8x

//numpad: 20x
{XN,20,kW,kH, 250,6,34},{X, 0,kW,kH, '/',3,36},{X,0,kW,kH, '*',3,33},{X,0,kW,kH,  '-',3,41},
{XN,29,kW,kH, '7',8, 2},{X, 0,kW,kH, 24 ,8, 4},{X,0,kW,kH, '9',8, 1},{X,0,kW,kH*2,'+',3, 6},
{XN,38,kW,kH, 27 ,8,10},{X, 0,kW,kH,  7 ,6,12},{X,0,kW,kH, 26 ,8, 9},
{XN,47,kW,kH, '1',8,18},{X, 0,kW,kH, 25 ,8,20},{X,0,kW,kH, '3',8,17},{X,0,kW,kH*2, 217,6,22}, //Ent
{XN,56,kW*2,kH,'_',6,28},{-kW*2,0,kW,kH,'.',6,25}, //Ins Del
};  //symbols: `240=  248 deg  249. 250,  7 cir  9 dot`
// < x1B 27  > x1A 26  ^ x18 24  v x19 25


#if 0
///  string from usb hid code
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
#endif
