#pragma once
#include <stdint.h>
#include "Ada4_ST7735.h"
//#include "demos.h"

//#include "Ada4_ST7735.h"
//#include "kbd_draw.h"


enum EMenu  // main menu entries, level1
{
	M_Testing=0,
	M_Mappings,
	M_Sequences,
	M_Display,

	M_OldDemos,
	M_FullDemos,
	M_Game, M_All
};

struct Gui
{
	//  main
	Gui();
	void Init();
	void Draw(Ada4_ST7735& d);
	void KeyPress(int8_t right, int8_t up);

	//  const
	const static uint8_t 
//		HelpAll = 11,    // Help pages
		StatAll = 4,    // Status pages
		YM2[M_All];    // menu entries, level2

	//  vars
	//int8_t help, hPage;  // shows help, on top, page
	int8_t testPage;  // M_Test
	
//	inline int DrawDemo()
//	{	return menu && mlevel > 0 && !help && !status;  }
//	inline int IsGame()
//	{	return DrawDemo() && ym == MGames;  }
	
	int8_t menu;    // 0 off, 1 in menu
	int8_t mlevel;  // 0 main, 1 level2

	int8_t ym;      // EMenu main, y cursor
	int8_t ym2[M_All];  // level2 page,  cursor for submenu

//	int16_t tInfo, infType, memSize;  // info text vars
};
