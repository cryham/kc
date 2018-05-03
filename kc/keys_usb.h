#pragma once
#include <stdint.h>

enum EKeys  // byte key codes
{
	//  normal usb keys ----
	KEY_NONE=0,
	K_LCTRL, K_LSHIFT, K_LALT, K_LGUI,  // modif
	K_RCTRL, K_RSHIFT, K_RALT, K_RGUI, K_ModLast = K_RGUI,

	K_A,K_B,K_C,K_D,K_E,K_F,K_G,K_H,K_I,K_J,K_K,K_L,K_M,K_N,  // letters
	K_O,K_P,K_Q,K_R,K_S,K_T,K_U,K_V,K_W,K_X,K_Y,K_Z,
	K_1,K_2,K_3,K_4,K_5,K_6,K_7,K_8,K_9,K_0,  // digits

	K_ENT, K_ESC, K_BACK, K_TAB, K_SPACE, K_MENU,  // long
	K_CAPS, K_PRTSCR, K_SCRL, K_PAUSE, K_NUML,
	K_MINUS, K_EQUAL, K_LBRACE,K_RBRACE, K_BSLASH,  // symbol
	K_NON_US_NUM, K_SEMIC, K_QUOTE, K_TILDE, K_COMMA, K_PERIOD, K_SLASH, K_NON_US_BS,

	K_F1,K_F2,K_F3,K_F4,K_F5,K_F6,K_F7,K_F8,K_F9,K_F10,K_F11,K_F12,  // func
	K_INS,K_HOME,K_PGUP,K_DEL,K_END,K_PGDN,K_RIGHT,K_LEFT,K_DOWN,K_UP,  // arrows

	KP_DIV, KP_MUL, KP_SUB, KP_ADD, KP_ENT,  // numpad
	KP_1,KP_2,KP_3,KP_4,KP_5,KP_6,KP_7,KP_8,KP_9,KP_0, KP_DOT,

	K_F13,K_F14,K_F15,K_F16,K_F17,K_F18,K_F19,K_F20,K_F21,K_F22,K_F23,K_F24,  // func2
	KM_PLAY, KM_PAUSE, KM_RECORD, KM_FAST_FORWARD, KM_REWIND,  // media
	KM_NEXT_TRACK, KM_PREV_TRACK, KM_STOP,
	KM_EJECT, KM_RANDOM_PLAY, KM_PLAY_PAUSE, KM_PLAY_SKIP,

	K_MUTE, K_VOL_INC, K_VOL_DEC,  // vol,sys
	KS_POWER_DOWN, KS_SLEEP, KS_WAKE_UP,
	KEYS_ALL,  // < End of normal, 137 ----

	//  padding-  for any forgotten usb codes
	K_Data0, K_Data1, K_Data2, K_Data3, K_Data4, K_Data5, K_Data6, K_Data7,

	//  special  EXT  < Start  ----
	//  display, internal functions
	K_Fun0, K_Fun1, K_Fun2, K_Fun3, K_Fun4,	K_Fun5, K_Fun6, K_Fun7, K_Fun8, K_Fun9,
	//  mouse
	KM_Left,KM_Right,KM_Up,KM_Down, KM_LMB,KM_MMB,KM_RMB,
	KM_WhlUp,KM_WhlDown, KM_WhlLeft,KM_WhlRight, KM_Back,KM_Forw,

	//  layer switch  hold/set etc config elsewhere
	K_Layer1, K_Layer2, K_Layer3, K_Layer4, K_Layer5, K_Layer6, K_Layer7, K_Layer8,
	K_Layer9, K_Layer10, K_Layer11, K_Layer12, K_Layer13, K_Layer14, K_Layer15, K_Layer16,
	//  sequences
	K_Seq0, K_S1,K_S2,K_S3,K_S4,K_S5,K_S6,K_S7,K_S8,K_S9,
	K_S10,K_S11,K_S12,K_S13,K_S14,K_S15,K_S16,K_S17,K_S18,K_S19,
	K_S20,K_S21,K_S22,K_S23,K_S24,K_S25,K_S26,K_S27,K_S28,K_S29,
	K_S30,K_S31,K_S32,K_S33,K_S34,K_S35,K_S36,K_S37,K_S38,K_S39, K_SeqLast = K_S39,
//	K_S40,K_S41,K_S42,K_S43,K_S44,K_S45,K_S46,K_S47,K_S48,K_S49,
//	K_S50,K_S51,K_S52,K_S53,K_S54,K_S55,K_S56,K_S57,K_S58,K_S59, K_SeqLast = K_S59,
	KEYS_ALL_EXT  // < End
};

//  usb codes  for byte key codes
extern const uint16_t cKeyUsb[KEYS_ALL];

//  key names strings  for key codes
extern const char* cKeyStr[KEYS_ALL_EXT];
//  short key names
extern const char* cKeySh[KEYS_ALL_EXT];

//  key groups  color for key codes
extern const uint8_t cKeyGrp[KEYS_ALL_EXT];

//  group names and colors
const uint8_t grpMax = 17;
extern const char* cGrpName[grpMax];
extern const uint8_t cGrpRgb[grpMax][2][3];

//  funtion names
extern const char* cFunStr[K_Fun9-K_Fun0+1];
