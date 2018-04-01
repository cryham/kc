#pragma once
#include <stdint.h>

enum EKeys  // byte key codes
{
	KEY_NONE=0,
	K_LCTRL, K_LSHIFT, K_LALT, K_LGUI,
	K_RCTRL, K_RSHIFT, K_RALT, K_RGUI,

	K_A,K_B,K_C,K_D,K_E,K_F,K_G,K_H,K_I,K_J,K_K,K_L,K_M,K_N,
	K_O,K_P,K_Q,K_R,K_S,K_T,K_U,K_V,K_W,K_X,K_Y,K_Z,
	K_1,K_2,K_3,K_4,K_5,K_6,K_7,K_8,K_9,K_0,

	K_ENT, K_ESC, K_BACK, K_TAB, K_SPACE,
	K_MINUS, K_EQUAL, K_LBRACE,K_RBRACE, K_BSLASH,
	K_NON_US_NUM, K_SEMIC, K_QUOTE, K_TILDE, K_COMMA, K_PERIOD, K_SLASH,

	K_F1,K_F2,K_F3,K_F4,K_F5,K_F6,K_F7,K_F8,K_F9,K_F10,K_F11,K_F12,

	K_CAPS, K_PRTSCR, K_SCRL, K_PAUSE, K_NUML,
	K_INS,K_HOME,K_PGUP,K_DEL,K_END,K_PGDN,K_RIGHT,K_LEFT,K_DOWN,K_UP,

	KP_DIV, KP_MUL, KP_SUB, KP_ADD, KP_ENT,
	KP_1,KP_2,KP_3,KP_4,KP_5,KP_6,KP_7,KP_8,KP_9,KP_0,
	KP_DOT, K_NON_US_BS, K_MENU,

	K_F13,K_F14,K_F15,K_F16,K_F17,K_F18,K_F19,K_F20,K_F21,K_F22,K_F23,K_F24,

	KM_PLAY, KM_PAUSE, KM_RECORD, KM_FAST_FORWARD, KM_REWIND,
	KM_NEXT_TRACK, KM_PREV_TRACK, KM_STOP,
	KM_EJECT, KM_RANDOM_PLAY, KM_PLAY_PAUSE, KM_PLAY_SKIP,

	K_MEDIA_MUTE, K_MEDIA_VOLUME_INC, K_MEDIA_VOLUME_DEC,
	KS_POWER_DOWN, KS_SLEEP, KS_WAKE_UP,
	KEYS_ALL
};

//  usb codes  from byte key
extern uint16_t usbKey[KEYS_ALL];

//  key names strings  for byte keys
extern const char* strKey[KEYS_ALL];

//  key groups  for color
extern const uint8_t grpKey[KEYS_ALL];
const uint8_t grpMax = 12;
extern const char* grpName[grpMax];
