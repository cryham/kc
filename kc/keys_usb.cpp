#include "keys_usb.h"
#include "keylayouts.h"

//  usb codes  from byte key
//--------------------------------------------
const uint16_t cKeyUsb[KEYS_ALL]={  KEY_ESC, //0
	MODIFIERKEY_LEFT_CTRL, MODIFIERKEY_LEFT_SHIFT, MODIFIERKEY_LEFT_ALT, MODIFIERKEY_LEFT_GUI,  // modif
	MODIFIERKEY_RIGHT_CTRL, MODIFIERKEY_RIGHT_SHIFT, MODIFIERKEY_RIGHT_ALT, MODIFIERKEY_RIGHT_GUI,

	KEY_A,KEY_B,KEY_C,KEY_D,KEY_E,KEY_F,KEY_G,KEY_H,KEY_I,KEY_J,KEY_K,KEY_L,KEY_M,KEY_N,  // letters
	KEY_O,KEY_P,KEY_Q,KEY_R,KEY_S,KEY_T,KEY_U,KEY_V,KEY_W,KEY_X,KEY_Y,KEY_Z,
	KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,  // digits

	KEY_ENTER, KEY_ESC, KEY_BACKSPACE, KEY_TAB, KEY_SPACE, KEY_MENU,  // long
	KEY_CAPS_LOCK, KEY_PRINTSCREEN, KEY_SCROLL_LOCK, KEY_PAUSE, KEY_NUM_LOCK,

	KEY_MINUS, KEY_EQUAL, KEY_LEFT_BRACE,KEY_RIGHT_BRACE, KEY_BACKSLASH,  // symbols
	KEY_NON_US_NUM, KEY_SEMICOLON, KEY_QUOTE, KEY_TILDE, KEY_COMMA, KEY_PERIOD, KEY_SLASH, KEY_NON_US_BS,

	KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,KEY_F12,  // func
	KEY_INSERT,KEY_HOME,KEY_PAGE_UP,KEY_DELETE,KEY_END,KEY_PAGE_DOWN,KEY_RIGHT,KEY_LEFT,KEY_DOWN,KEY_UP,  // arrows

	KEYPAD_SLASH, KEYPAD_ASTERIX, KEYPAD_MINUS, KEYPAD_PLUS, KEYPAD_ENTER,  // numpad
	KEYPAD_1,KEYPAD_2,KEYPAD_3,KEYPAD_4,KEYPAD_5,KEYPAD_6,KEYPAD_7,KEYPAD_8,KEYPAD_9,KEYPAD_0, KEYPAD_PERIOD,

	KEY_F13,KEY_F14,KEY_F15,KEY_F16,KEY_F17,KEY_F18,KEY_F19,KEY_F20,KEY_F21,KEY_F22,KEY_F23,KEY_F24,  // func2
	KEY_MEDIA_PLAY, KEY_MEDIA_PAUSE, KEY_MEDIA_RECORD, KEY_MEDIA_FAST_FORWARD, KEY_MEDIA_REWIND,  // media
	KEY_MEDIA_NEXT_TRACK, KEY_MEDIA_PREV_TRACK, KEY_MEDIA_STOP,
	KEY_MEDIA_CALC, KEY_MEDIA_RANDOM_PLAY, KEY_MEDIA_PLAY_PAUSE, KEY_MEDIA_PLAY_SKIP,

	KEY_MEDIA_MUTE, KEY_MEDIA_VOLUME_INC, KEY_MEDIA_VOLUME_DEC,  // volume
	KEY_SYSTEM_POWER_DOWN, KEY_SYSTEM_SLEEP, KEY_SYSTEM_WAKE_UP,  // power
};

//  key names strings
const char* cKeyStr[KEYS_ALL_EXT]={  "None",  // max [255]
	"CtrL", "ShL", "AltL", "GuiL",  "CtrR", "ShR", "AltR", "GuiR",  // modif

	"A","B","C","D","E","F","G","H","I","J","K","L","M","N",  // letters
	"O","P","Q","R","S","T","U","V","W","X","Y","Z",
	"1","2","3","4","5","6","7","8","9","0",  // digits

	"Ent", "Esc", "Bck", "Tab", "Spc", "Menu",  // long
	"CapsL", "PtScr", "ScrL", "Break", "NumL",
	"-", "=", "[","]", "\\", "N\\", ";", "\'", "`", ",",".", "/", "O\\",  // symbols

	"F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12",  // func
	"Ins","Home","PgUp","Del","End","PgDn", "\x1A","\x1B","\x19","\x18", //"Ri", "Le", "Dn", "Up"  arrows

	"P/", "P*", "P-", "P+", "PEnt",  // numpad
	"P1","P2","P3","P4","P5","P6","P7","P8","P9","P0", "P.",

	"F13","F14","F15","F16","F17","F18","F19","F20","F21","F22","F23","F24",  // func2
	"Play-", "Pause-", "Rec-", "Forw-", "Rew-", "Next", "Prev", "Stop",  // media
	"Calc", "Randm-", "PlPau", "PlSkp-",

	"Mute", "Vol+", "Vol-",  "Power", "Sleep", "Wake",  // volume, power
	"All",  // _extras_

	"Fun10", "Fun11", "Fun12", "Fun13", "Fun14", "Fun15", "Fun16", "Fun17",  // internal funct
	"Fun0", "Fun1", "Fun2", "Fun3", "Fun4", "Fun5", "Fun6", "Fun7", "Fun8", "Fun9",

	"M\x1B","M\x1A","M\x18","M\x19", "LMB","MMB","RMB",  // mouse
	"Mwh\x18","Mwh\x19", "Mwh\x1B","Mwh\x1A", "Mbck","Mfrw",

	"L1", "L2", "L3", "L4", "L5", "L6", "L7", "L8",  // layers
	"L9", "L10", "L11", "L12", "L13", "L14", "L15", "L16",

	"S0","S1","S2","S3","S4","S5","S6","S7","S8","S9",  // sequences
	"S10","S11","S12","S13","S14","S15","S16","S17","S18","S19",
	"S20","S21","S22","S23","S24","S25","S26","S27","S28","S29",
	"S30","S31","S32","S33","S34","S35","S36","S37","S38","S39",
	"S40","S41","S42","S43","S44","S45","S46","S47","S48","S49",
	"S50","S51","S52","S53","S54","S55","S56","S57","S58","S59",
};

//  short names
const char* cKeySh[KEYS_ALL_EXT]={  "no",
	"Cl", "Sl", "Al", "Gl",  "Cr", "Sr", "Ar", "Gr",  // modif

	"A","B","C","D","E","F","G","H","I","J","K","L","M","N",  // letters
	"O","P","Q","R","S","T","U","V","W","X","Y","Z",
	"1","2","3","4","5","6","7","8","9","0",  // digits

	"\x1C", "e", "<", "tb", "sp", "m",  // long
	"ca", "p", "sc", "br", "nu",
	"-", "=", "[","]", "\\", "\\n", ";", "\'", "`", ",",".", "/", "\\o",

	"1 ","2 ","3 ","4 ","5 ","6 ","7 ","8 ","9 ","10","11","12",  // func
	"I_","Ho","Pu","D.","En","Pd", "\x1A","\x1B","\x19","\x18", //"Ri", "Le", "Dn", "Up"  arrows

	"/", "x", "-", "+", "\x1C", //"E"  numpad
	"1","2","3","4","5","6","7","8","9","0", ".",

	"13","14","15","16","17","18","19","20","21","22","23","24",  // func2
	"Pl", "Pa", "Rc", "Fw", "Rw", "Nx", "Pv", "St",  // media
	"Ca", "Rn", "Pp", "Pk",

	"Mt", "V+", "V-",  "Pw", "Sl", "Wk",  // volume, power
	"Aa",  // _extras_

	"10", "11", "12", "13", "14", "15", "16", "17",  // internal funct
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",

	"\x1B","\x1A","\x18","\x19", "L","M","R",  // mouse
	"wU","wD", "wL","wR", "Mb","Mf",

	"L1", "L2", "L3", "L4", "L5", "L6", "L7", "L8",  // layers
	"L9", "10", "11", "12", "13", "14", "15", "16",

	"S0","S1","S2","S3","S4","S5","S6","S7","S8","S9",  // sequences
	"10","11","12","13","14","15","16","17","18","19",
	"20","21","22","23","24","25","26","27","28","29",
	"30","31","32","33","34","35","36","37","38","39",
	"40","41","42","43","44","45","46","47","48","49",
	"50","51","52","53","54","55","56","57","58","59",
};

//  key Groups for color
//--------------------------------------------
const uint8_t cKeyGrp[KEYS_ALL_EXT]=
{	0,  // none
	1, 1, 1, 1,  1, 1, 1, 1,  // modif

	2,2,2,2,2,2,2,2,2,2,2,2,2,2,  // letters
	2,2,2,2,2,2,2,2,2,2,2,2,
	3,3,3,3,3,3,3,3,3,3,      // digits

	4, 4, 4, 4, 4, 4,         // long
	4, 4, 4, 4, 4,
	7, 7, 7,7, 7, 7, 7, 7, 7, 7,7, 7, 7,  // symbols

	5,5,5,5,5,5,5,5,5,5,5,5,  // func
	6,6,6,6,6,6, 6,6,6,6,     // arrows

	8, 8, 8, 8, 8,
	8,8,8,8,8,8,8,8,8,8, 8,   // numpad

	9,9,9,9,9,9,9,9,9,9,9,9,  // func2
	10,10,10,10,10,10,10,10,  // media
	10,10,10,10,
	11,11,11, 11,11,11,  // volume, power
	0,  //-  _extras_

	12,12,12,12,12,12,12,12,  // internal funct
	12,12,12,12,12,12,12,12,12,12,

	13,13,13,13,13,13,13,     // mouse
	13,13,13,13,13,13,

	14,14,14,14,14,14,14,14,  // layers
	14,14,14,14,14,14,14,14,

	15,15,15,15,15,15,15,15,15,15,  // sequences
	15,15,15,15,15,15,15,15,15,15,
	15,15,15,15,15,15,15,15,15,15,
	15,15,15,15,15,15,15,15,15,15,
	15,15,15,15,15,15,15,15,15,15,
	15,15,15,15,15,15,15,15,15,15,
};

//  group names
const char* cGrpName[grpMax]=
{	"None",
	"Modifiers", "Letters", "Digits", "Long", "Function",  // 5
	"Arrows", "Sybmols", "Numpad", // 8
	"Function2", "Media", "Volume,Power", // 11
	// _extras_
	"Internal", "Mouse", "Layers", "Sequences",  // 15
};

//  group colors  r,g,b and mul rgb fade
const uint8_t cGrpRgb[grpMax][2][3]=
{
	{31,20,20, 1,3,4},  // 0 none-
	{20,29,29, 4,3,3},  // 1 modif
	{26,26,26, 5,3,1},  // 2 letter
	{29,29,16, 3,3,5},  // 3 digits
	{23,31,12, 4,2,4},  // 4 long
	{20,26,31, 4,2,0},  // 5 func

	{17,31,24, 4,1,3},  // 6 arrows
	{26,26,31, 3,3,1},  // 7 symb
	{31,25,18, 1,3,5},  // 8 numpad

	{31,23,31, 2,3,2},  // 9 func2
	{31,20,25, 1,3,3},  // 10 media
	{30,30,30, 3,3,2},  // 11 vol,sys

	{14,25,31, 2,1,0},  // 13 disp fun _ext_
	{30,30, 8, 2,2,4},  // 14 mouse
	{31,18,20, 0,2,1},  // 15 layer
	{28,26,31, 4,4,2},  // 16 sequence
};


//  internal funtion names  ***
//--------------------------------------------
const char* cFunStr[K_FunLast-K_Fun0+1]=
{
	"", "", "", "", "", "", "", "",
	"\x13"" GUI toggle", "\x14"" Bright -", "\x16"" Bright +",
	"\x08"" Soft Reset", "\x0F"" Light",
	"X Quit Sequence", "~ Reset stats",
	"- Default Layer","+ Default Layer", "* Un/lock layer"
};


