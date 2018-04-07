#pragma once
#include <stdint.h>
#include "def.h"
#include "demos.h"
#include "kc_data.h"


struct Gui
{
	Demos demos;
	Ada4_ST7735* d=0;
	int kbdSend = 0;  // send usb

	//  main
	Gui();
	void Init(Ada4_ST7735* tft);
	void Draw(), DrawEnd();
	void KeyPress();

	//  util
	void DrawTesting(), DrawMapping(), DrawSequences(), Chk_y1();

	void DrawMenu(int cnt, const char** str, int8_t clrId=0,
				int16_t yadd=10, int16_t nextCol=-1, int16_t numGap=-1);
	void FadeClr(const uint8_t* clrRGB, const uint8_t* mulRGB,
				const uint8_t minRGB, const uint8_t mul, const uint8_t div);
	const static uint8_t Cl = 4;  // menu colors
	const static uint8_t Mclr[Cl][2][3];

	//  vars
	//int8_t menu;  // 0 off, 1 in menu
	int8_t mlevel;  // 0 main, 1 level1, 2 level2

	int8_t ym;      // 0 main y cursor
	int8_t ym1[M_All];  // 1 y cursor for all main menu entries
	int8_t yy;      // = ym1[ym]  level1 y cursor

	//  time
	unsigned long tm;
	uint32_t oldti, tdemo;  // demo time ms

	uint32_t oldti_kr;
	uint16_t krDelay=250, krRepeat=60;  // ms  key auto repeat
	int8_t kr(int8_t y, int8_t x, uint16_t dt);


	//  Mapping  - - - -
	int16_t keyCode=0;
	int8_t nLay=0, scId=0, drawId=-1,
		pressKey=0, moveCur=0, pickCode=0, // edit operations
		keyGroup=0, grpFilt=0;  // group filter

	//  keys pressed, some +-1
	int8_t kRight=0, kUp=0,  kPgUp=0, kEnd=0,
			kBack=0, kEnt=0,  kCtrl=0, kSh=0,  kInf=0, kFps=0,
	/*seq*/	kBckSp=0, kIns=0, kDel=0,  kCopy=0, kPaste=0, kSwap=0,
			kLoad=0, kSave=0;


	//  Sequences  - - - -
	int8_t editSeq=0;
	int8_t edit = 0;   // seq 0 view / 1 edit
	int8_t edins = 0;  // 1 ins 0 overwrite
	int8_t slot, page, edpos;  // edit vars
	int seqId()
	{	return slot + page*iPage;  }
	int8_t cpId = -1;  // copy/swap from

	int8_t tBlnk = 0;
	const int8_t iPage = 10, iSlots = 60, cBlnk = 35;
	int16_t tInfo=0, infType=0, memSize=0;  // info text vars

	char a[128];  // temp str
};
