#pragma once
#include <stdint.h>
#include "def.h"
#include "demos.h"
#include "kc_data.h"
#include "keys_usb.h"
#ifdef GAME
#include "games.h"
#endif


struct Gui
{
	Demos demos;
#ifdef GAME
	Games game;
#endif
	Ada4_ST7735* d=0;

	//  main
	Gui();
	void Init(Ada4_ST7735* tft);
	void Draw(), DrawEnd();
	void KeyPress();


	//  draw
	void DrawTesting(), DrawMapping(), DrawSequences();
	void DrawLayout(bool edit), Chk_y1();
	int8_t KeysSeq(), KeysMap();
	void WriteSeq(int8_t seq, int8_t q);

	void SetScreen(int8_t start);
	const char* StrScreen(int8_t s);


	//  fade color menu
	enum EFadeClr
	{	C_Main=0, C_Demos, C_Test, C_Map, C_Seq, C_Disp, C_Scan, C_Game, Cl_ALL  };
	const static uint8_t Mclr[Cl_ALL][2][3];

	void FadeClr(EFadeClr ec, const uint8_t minRGB, const uint8_t mul, const uint8_t div);
	void FadeGrp(uint8_t g, const uint8_t minRGB, const uint8_t mul, const uint8_t div);
	void DrawMenu(int cnt, const char** str, EFadeClr ec, uint16_t curClr,
		int16_t yadd=10, int16_t nextCol=-1, int16_t numGap=-1);


	//  vars
	int8_t kbdSend = 0;  // 1 send to usb  0 in menu
	int8_t mlevel;  // 0 main, 1 level1, 2 level2

	int8_t ym;      // 0 main y cursor
	int8_t ym1[M_All];  // 1 y cursor for all main menu entries
	int8_t yy;      // = ym1[ym]  level1 y cursor

	//  time
	unsigned long tm;
	uint32_t oldti, tdemo;  // demo time ms

	uint32_t oldti_kr;
	int8_t kr(uint8_t sc, uint16_t dt);
	int8_t iRam = 0;


	//  Mapping  - - - -
	const int8_t yPosLay = 62;
	int16_t keyCode=0, scId=0, drawId=-1, drawX=0,drawY=0;
	int8_t nLay=0,
		pressKey=0, moveCur=0, pickCode=0, // edit operations
		keyGroup=0, grpFilt=0;  // group filter

	//  keys pressed, some +-1
	int8_t kRight=0, kUp=0,  kPgUp=0, kEnd=0,  kDnH=0,
			kBack=0, kEnt=0,  kCtrl=0, kSh=0,  kMul=0, kSub=0, kDiv=0,
	/*seq*/	kBckSp=0, kIns=0, kDel=0,  kCopy=0, kPaste=0, kSwap=0,
			kLoad=0, kSave=0;

	//  level 2 y cursor  - -
	int8_t ym2Test = 0;  // Testing
	int8_t ym2Disp = 0, pgDisp = 0;  // Display


	//  Sequences  - - - -
	int8_t edit = 0;   // seq 0 view / 1 edit
	int8_t edins = 0;  // 1 ins 0 overwrite
	int8_t slot, page, edpos;  // edit vars
	int seqId()
	{	return slot + page*iPage;  }
	int8_t cpId = -1;  // copy/swap from

	int8_t tBlnk = 0;
	const int8_t iPage = 10, cBlnk = 25;  // par -
	int16_t tInfo=0, infType=0;  // info text vars

	//  const from grp
	uint8_t grpStart[grpMax], grpEnd[grpMax];
};
