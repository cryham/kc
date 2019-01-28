#pragma once
#include <stdint.h>
#include "def.h"
#include "demos.h"
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

	//  main  ----
	Gui();
	void Init(Ada4_ST7735* tft);
	void Draw(), DrawEnd();
	void KeyPress();


	//  draw menus
	void DrawMapping(), DrawSequences(), DrawTesting(), DrawSetup();
	void DrawDisplay(), DrawClock(), DrawHelp(), DrawOperInfo();

	//  draw util
	void DrawPressed(), DrawLayout(bool edit), Chk_y1();
	void DrawSeq(int8_t seq, int8_t q);
	void DrawDispCur(int i, int16_t y), DrawClockCur(int i, int16_t y);

	//  keys
	int8_t KeysSeq();  void KeysMap();
	int PressKey(int8_t& var);
	void KeysParSetup(int sp), KeysParDisplay(int sp), KeysClock();

	//  start
	void SetScreen(int8_t start);
	const char* StrScreen(int8_t s);


	//  fade color menu  ---
	enum EFadeClr
	{	C_Main=0, C_Demos, C_Test, C_Map, C_Seq,
		C_Setup, C_Disp, C_Clock, C_Setup2,
		C_Game, C_GameOpt, Cl_ALL  };
	const static uint8_t
		Mclr[Cl_ALL][2][3];

	void FadeClr(EFadeClr ec, const uint8_t minRGB, const uint8_t mul, const uint8_t div);
	void FadeGrp(uint8_t g, const uint8_t minRGB, const uint8_t mul, const uint8_t div);
	void DrawMenu(int cnt, const char** str, EFadeClr ec, uint16_t curClr,
		uint16_t bckClr, int16_t yadd=10, int16_t nextCol=-1, int16_t numGap=-1);


	//  vars  ---
	int8_t kbdSend = 0; // 1 send to usb  0 in menu
	int8_t mlevel = 0;  // 0 main, 1 level1, 2 level2

	int8_t ym = 0;      // 0 main y cursor
	int8_t ym1[M_All];  // 1 y cursor for all main menu entries
	int8_t yy = 0;      // = ym1[ym]  level1 y cursor

	//  time, key repeat
	uint32_t oldti=0, oldti_kr=0;
	int8_t kr(uint8_t sc, uint16_t dt);
	int8_t iRam = 0;  // ram info

	//  help
	int8_t hpage = 0;
	const static int8_t HAll = 11;
	//  rtc on start
	unsigned long tm_on = 0;
	#ifdef LED
	int8_t led = 0;
	#endif


	//  keys pressed, some +-1  _k_
	int8_t kRight=0, kUp=0,  kPgUp=0, kEnd=0,  kDnH=0,
	/*Add*/ kEsc=0, kAdd=0, kEnt=0,kEnt2=0,  kCtrl=0, kSh=0,  kMul=0, kSub=0, kDiv=0,
	/*seq*/	kBckSp=0, kIns=0, kDel=0,  kCopy=0, kPaste=0, kSwap=0, /*F4,5*/kLoad=0, kSave=0,
			kF1=0,kF2=0,kF3=0,kF6=0,kF7=0,kF8=0,kF9=0,kF10=0,kF11=0,kF12=0;


	//  Mapping  - - - -
	const int8_t yPosLay = 62;
	int16_t keyCode=0, scId=0, scIdCpy=0, drawId=-1, drawX=0,drawY=0;
	int8_t nLay=0, nLayCpy=0,
		pressKey=0, pickCode=0, // edit operations
		keyGroup=0, grpFilt=0;  // pick group filter

	//  level 2 y cursors  - -
	int8_t ym2Scan = 0, ym2Keyb = 0, ym2Mouse = 0, pressGui = 0;  // Setup
	int8_t ym2Disp = 0, pgDisp = 0;  // Display
	int8_t ym2Clock = 0, pgClock = 2;  // Clock

	const static uint8_t Disp_All = 2, Clock_All = 4, pgClkAdj = Clock_All-1;  // adj on last
	const static uint8_t DispPages[Disp_All], ScanPages[S_All-1], ClockPages[Clock_All];

	//  util
	int16_t RangeAdd(int16_t val, int16_t add, int16_t vmin, int16_t vmax, int8_t cycle=0);
	void Save(), Load(int8_t reset);


	//  Sequences  - - - -
	int8_t edit = 0;   // seq 0 view / 1 edit
	int8_t edins = 1;  // 1 ins 0 overwrite
	int8_t copyId = -1;  // copy/swap from
	int8_t slot=0, page=0;  int16_t edpos=0;  // edit vars
	void slotMax(),slotMin(),pageInc(),pageDec();
	int seqId()
	{	return slot + page*iPage;  }

	int8_t tBlnk = 0;
	int16_t tInfo=0;  int8_t infType=0;  // info text vars
	const int8_t iPage = 10, cBlnk = 25;  // par-

	//  const from grp
	uint8_t grpStart[grpMax], grpEnd[grpMax];
};

