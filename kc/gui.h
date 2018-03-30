#pragma once
#include <stdint.h>
#include "def.h"
#include "demos.h"
#include "kc_data.h"


struct Gui
{
	Demos demos;
	class Ada4_ST7735* d;

	KC_Key kc;

	//  main
	Gui();
	void Init(Ada4_ST7735* tft);
	void Draw(), DrawEnd();
	void KeyPress();

	//  util
	void DrawTesting(), DrawMapping(), DrawSequences(), Chk_y1();

	void DrawMenu(int cnt, const char** str, int8_t clrId=0,
				int16_t yadd=10, int16_t nextCol=-1, int16_t numGap=-1);

	//  vars
	//int8_t menu;  // 0 off, 1 in menu
	int8_t mlevel;  // 0 main, 1 level1, 2 level2

	int8_t ym;      // 0 main y cursor
	int8_t ym1[M_All];  // 1 y cursor for all main menu entries
	int8_t yy;      // = ym1[ym]  level1 y cursor

	//  time
	unsigned long tm;
	uint32_t oldti, tdemo;  // demo time ms

	//	int16_t tInfo, infType, memSize;  // info text vars
	char a[128];  // temp str
};
