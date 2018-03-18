#pragma once
//#include <stdint.h>
#include "WProgram.h"
#include "Ada4_ST7735.h"

#define DEMOS  // if not, only keyboard Gui

#define W 160  // area
#define H 128

#define RGB(r,g,b) ((r<<11)+ ((g)<<6) +b)	// R F800  G 07E0  B 001F  R 32 G 64 B 32


//  sinus table  ----
#define SX  8192   // 8192  mem size 16kB
//#define SX  16384
#define SY  16384  // 16384  y quality
#define Cos(x)  sint[(SX/2+(x))%SX]  // %SX
#define Sin(x)  sint[      (x) %SX]

extern const int16_t sint[SX];


enum EDemo
{
	D_None=0, D_Space, D_Balls, D_Rain, D_Fountain,
	D_Ngons, D_Hedrons, D_CK_Logo, D_Fonts, D_Chars, D_Plasma, D_All
};

struct Demos
{
	//  main  ----
	Demos();

	Ada4_ST7735* d;
	uint16_t* data = 0;  // buffer

	void Init(Ada4_ST7735* tft);  //, Reset(D d), KeyGlob(D d);
//	void Draw(D d, int8_t menu, int8_t ym, int8_t ym2);

	int8_t fps;  // show frames per second, 1 on, 0 off
	uint32_t ti, oti;  // fps: time us, old

	void Font_ver(bool st);  // ver, date, st - in status or demo

#ifdef DEMOS
	int8_t iPrev;   // prev demo, for init
	int16_t iInfo;
	int8_t iInfoOff;  // params info text
	const static int8_t iOff = 2;
	void KeyPress(EDemo demo, int k, int e, int ct, int kinf);


	//  Plasma
	uint t;  // frame counter
	int8_t plasma;  // cur mode
	const static int8_t num_plasma = 6;
	int8_t tadd[num_plasma];  // speeds

	void Plasma(), PlasmaT(int8_t dt);
	void Plasma0(),Plasma1(),Plasma2(),Plasma3(),Plasma4(),Plasma5();


	//  Balls  --------
//	const static int sMax = 10, bMax = 10, dMax = 10;
	const static int sMax = 240, bMax = 300, dMax = 550;  // 76%  50k
//	const static int sMax = 240, bMax = 300, dMax = 1350;  // 90% ram

	int sCnt, sVel;  // stars: count, velocity
	int bCnt, bSpd, bSpRnd, bRad;  // balls: count, speed, radius max
	const static int bSpRMax = 8;
	#define bDet  128  // move detail

	enum EInit {  INone=0, IBalls, ISpace, IDrops  } einit;
	struct Star {  int x,y,z, v;  uint16_t c;  };
	struct Ball {  int16_t x,y, vx,vy, r;  uint16_t c;  };
	struct Drop {  int16_t x,y, vx,vy, t;  uint16_t c;  };
	union
	{	//  common data
		Star star[sMax];
		Ball ball[bMax];
		Drop drop[dMax];
	};
	void BallsInit();
	void Balls();


	//  Rain
	int8_t rCur, r1Int,r1Size, r2Int,r2Size;
	void Rain();
	

	//  CK logo, 2D lines
	const static int8_t ckMax = 2;
	int8_t ckCur, ckSpeed;
	void CK_logo();
	void Chars();


	//  Space
	void SpaceInit();
	void Star(int i);  // new
	void Space();

	//  Fountain, drops  ----
	int fInt, fWave;
	void FountainInit();
	void Fountain();


	//  Ngons 2D
	int16_t ngt;  int8_t ngCur,  ngtOn, ngRot;
	const static int8_t ngMin = 5, ngMax = 14, ngRotMax = 4;
	void Ngons();

	//  Polyhedrons 3D  ----
	const static int8_t hdA = 11, hdRotMax = 4;  // all presets
	const static int16_t hdtMax = 100;  // cycle time

	int16_t hdt;  int8_t hdCur,  hdtOn, hdRot;
	void Hedrons();
	
	//void Spiral, Fire, Water..
#endif
};

//#undef D
