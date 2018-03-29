#pragma once
#include <stdint.h>
#include "demos.h"


struct Gui
{
	Demos demos;
	class Ada4_ST7735* d;

	//  main
	Gui();
	void Init(Ada4_ST7735* tft);
	void Draw(), DrawEnd();

	void KeyPress(int8_t right, int8_t up, int8_t pgup, int8_t back, int8_t inf);

	//  util
	void DrawTesting(), DrawMapping(), DrawSequences();

	void DrawMenu(int cnt, const char** str, int8_t clrId=0,
				int16_t yadd=10, int16_t nextCol=-1, int16_t numGap=-1);

	//  vars
	//int8_t menu;    // 0 off, 1 in menu
	int8_t mlevel;  // 0 main, 1 level2

	int8_t ym;      // 0 main y cursor
	int8_t ym2[M_All];  // level2 page,  cursor for submenu
	int8_t yy;      // = ym2[ym]  level2 y cursor

	//  time
	unsigned long tm;
	uint32_t oldti, tdemo;  // demo time ms

	//	int16_t tInfo, infType, memSize;  // info text vars
	char a[128];  // temp str
};
