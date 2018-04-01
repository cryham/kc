#include "keys_usb.h"
#include "keylayouts.h"

//  usb codes  from byte key
uint16_t usbKey[KEYS_ALL]={
	KEY_ESC, //0
	MODIFIERKEY_LEFT_CTRL, MODIFIERKEY_LEFT_SHIFT, MODIFIERKEY_LEFT_ALT, MODIFIERKEY_LEFT_GUI,
	MODIFIERKEY_RIGHT_CTRL, MODIFIERKEY_RIGHT_SHIFT, MODIFIERKEY_RIGHT_ALT, MODIFIERKEY_RIGHT_GUI,

	KEY_A,KEY_B,KEY_C,KEY_D,KEY_E,KEY_F,KEY_G,KEY_H,KEY_I,KEY_J,KEY_K,KEY_L,KEY_M,KEY_N,
	KEY_O,KEY_P,KEY_Q,KEY_R,KEY_S,KEY_T,KEY_U,KEY_V,KEY_W,KEY_X,KEY_Y,KEY_Z,
	KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,

	KEY_ENTER, KEY_ESC, KEY_BACKSPACE, KEY_TAB, KEY_SPACE,
	KEY_MINUS, KEY_EQUAL, KEY_LEFT_BRACE,KEY_RIGHT_BRACE, KEY_BACKSLASH,
	KEY_NON_US_NUM, KEY_SEMICOLON, KEY_QUOTE, KEY_TILDE, KEY_COMMA, KEY_PERIOD, KEY_SLASH,

	KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,KEY_F12,

	KEY_CAPS_LOCK, KEY_PRINTSCREEN, KEY_SCROLL_LOCK, KEY_PAUSE, KEY_NUM_LOCK,
	KEY_INSERT,KEY_HOME,KEY_PAGE_UP,KEY_DELETE,KEY_END,KEY_PAGE_DOWN,KEY_RIGHT,KEY_LEFT,KEY_DOWN,KEY_UP,

	KEYPAD_SLASH, KEYPAD_ASTERIX, KEYPAD_MINUS, KEYPAD_PLUS, KEYPAD_ENTER,
	KEYPAD_1,KEYPAD_2,KEYPAD_3,KEYPAD_4,KEYPAD_5,KEYPAD_6,KEYPAD_7,KEYPAD_8,KEYPAD_9,KEYPAD_0,
	KEYPAD_PERIOD, KEY_NON_US_BS, KEY_MENU,

	KEY_F13,KEY_F14,KEY_F15,KEY_F16,KEY_F17,KEY_F18,KEY_F19,KEY_F20,KEY_F21,KEY_F22,KEY_F23,KEY_F24,

	KEY_MEDIA_PLAY, KEY_MEDIA_PAUSE, KEY_MEDIA_RECORD, KEY_MEDIA_FAST_FORWARD, KEY_MEDIA_REWIND,
	KEY_MEDIA_NEXT_TRACK, KEY_MEDIA_PREV_TRACK, KEY_MEDIA_STOP,
	KEY_MEDIA_EJECT, KEY_MEDIA_RANDOM_PLAY, KEY_MEDIA_PLAY_PAUSE, KEY_MEDIA_PLAY_SKIP,

	KEY_MEDIA_MUTE, KEY_MEDIA_VOLUME_INC, KEY_MEDIA_VOLUME_DEC,
	KEY_SYSTEM_POWER_DOWN, KEY_SYSTEM_SLEEP, KEY_SYSTEM_WAKE_UP,
};

//  key names strings
const char* strKey[KEYS_ALL]={
	"None",
	"L Ct", "L Sh", "L Al", "L Gui",
	"R Ct", "R Sh", "R Al", "R Gui",

	"A","B","C","D","E","F","G","H","I","J","K","L","M","N",
	"O","P","Q","R","S","T","U","V","W","X","Y","Z",
	"1","2","3","4","5","6","7","8","9","0",

	"Ent", "Esc", "Bck", "Tab", "Spc",
	"-", "=", "[","]", "\\", "NN", ";", "\'", "`", ",",".", "/",

	"F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12",

	"CapsL", "PtSc", "ScrL", "Paus", "NumL", //"Ri", "Le", "Dn", "Up"
	"Ins","Home","PgUp","Del","End","PgDn", "\x1A","\x1B","\x19","\x18",

	"P/", "P*", "P-", "P+", "PEnt",
	"P1","P2","P3","P4","P5","P6","P7","P8","P9","P0", "P.", "NBS", "Menu",

	"F13","F14","F15","F16","F17","F18","F19","F20","F21","F22","F23","F24",

	"mPlay", "mPaus", "mRec", "mForw", "mRew", "mNext", "mPrev", "mStop",
	"mEjct", "mRand", "mPlPau", "mPlSkp",

	"Mute", "Vol+", "Vol-",
	"PowOff", "Sleep", "WakeUp",
};

//  key groups for color
const uint8_t grpKey[KEYS_ALL]=
{
	0,  // sys
	1, 1, 1, 1,  // mod
	1, 1, 1, 1,

	2,2,2,2,2,2,2,2,2,2,2,2,2,2, // letters
	2,2,2,2,2,2,2,2,2,2,2,2,
	3,3,3,3,3,3,3,3,3,3,  // digits

	4, 4, 4, 4, 4,  // long
	7, 7, 7,7, 7, 7, 7, 7, 7, 7,7, 7,  // symb

	5,5,5,5,5,5,5,5,5,5,5,5,  // func

	4, 4, 4, 4, 4, //Ri, Le, Dn, Up
	6,6,6,6,6,6, 6,6,6,6,  // arrows

	8, 8, 8, 8, 8,
	8,8,8,8,8,8,8,8,8,8, 8, 5, 5,  // numpad

	9,9,9,9,9,9,9,9,9,9,9,9,  // func2

	10, 10, 10, 10, 10, 10, 10, 10,  // media
	10, 10, 10, 10,

	11, 11, 11,  // vol
	0, 0, 0,  // sys
};

const char* grpName[grpMax]={
	"system","modifier","letter","digit","long","function", //5
	"arrows","sybmol","numpad", "function2","media","volume",
};
