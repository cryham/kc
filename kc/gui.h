#pragma once
#include <stdint.h>
#include "demos.h"


//  main menu entries, level1
enum EMenu
{
	M_Testing=0,
	M_Mappings,
	M_Sequences,

	M_Display,
	M_Demos,
	//M_Game,
	M_All
};

struct Gui
{
	Demos demos;
	class Ada4_ST7735* d;

	//  main
	Gui();
	void Init(Ada4_ST7735* tft);
	void Draw(), DrawEnd();
	void KeyPress(int8_t right, int8_t up, int8_t pgup, int8_t esc);


	//  menu entries on level2, for each level1
	const static uint8_t YM2[M_All];

	//  vars
	//int8_t menu;    // 0 off, 1 in menu
	int8_t mlevel;  // 0 main, 1 level2

	int8_t ym;      // EMenu main, y cursor
	int8_t ym2[M_All];  // level2 page,  cursor for submenu

	// time
	unsigned long tm;
	uint32_t oldti, tdemo;  // demo time ms

	//	int16_t tInfo, infType, memSize;  // info text vars
};
