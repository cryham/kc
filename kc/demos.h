#pragma once
//#include <stdint.h>
#include "WProgram.h"
#include "Ada4_ST7735.h"

#define DEMOS  // if not, only keyboard Gui

#define W 160  // area
#define H 128

#define D  Ada4_ST7735&

#define WHITE 0xFFFF
#define BLACK 0x0000

//  sin table  ----
#define SX  8192   // 8192  mem size 16kB
//#define SX  16384
#define SY  16384  // 16384  y quality
#define Cos(x)  sint[(SX/2+(x))%SX]
#define Sin(x)  sint[      (x) %SX]

extern const int16_t sint[SX];


enum EDemo
{
	D_None=0, D_Space, D_Balls, D_Rain, D_Fountain,
	D_Ngons, D_Hedrons, D_CK_Logo, D_Plasma, D_All
};

struct Demos
{
	//  main  ----
	Demos();
	void Init();  //, Reset(D d), KeyGlob(D d);
//	void Draw(D d, int8_t menu, int8_t ym, int8_t ym2);

	int8_t dim;  // dim display toggle
	int8_t fps;  // show frames per second, 1 on, 0 off
	uint32_t ti, oti;  // fps: time us, old

	static void Font_ver(D d, bool st);  // ver, date, st - in status or demo

#ifdef DEMOS
	int8_t iPrev;   // prev demo, for init
	int16_t iInfo;
	int8_t iInfoOff;  // params info text
	const static int8_t iOff = 2;
//	void KeyPress(int8_t demo, int8_t ym, int8_t ym2);
	void KeyPress(EDemo demo, int k, int e, int ct, int kinf);


	uint t;  // frame counter
	//  Plasma
	int8_t tadd[5];  // speed


	//  Balls  --------
//	const static int sMax = 10, bMax = 10, dMax = 10;
	const static int sMax = 240, bMax = 300, dMax = 1350;  //1350
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
	void Balls(D d);


	//  Rain
	int8_t rCur, r1Int,r1Size, r2Int,r2Size;
	void Rain(D d);
	
	//  text
	void Chars(D d, uint8_t set=0);
	
	//  CK logo, 2D lines
	const static int8_t ckMax = 2;
	int8_t ckCur, ckSpeed;
	void CK_logo(D d);


	//  Space
	void SpaceInit();
	void Star(int i);  // new
	void Space(D d);

	//  Fountain, drops  ----
	int fInt, fWave;
	void FountainInit();
	void Fountain(D d);


	//  Ngons 2D
	int16_t ngt;  int8_t ngCur,  ngtOn, ngRot;
	const static int8_t ngMin = 5, ngMax = 14, ngRotMax = 4;
	void Ngons(D d);

	//  Polyhedrons 3D  ----
	const static int8_t hdA = 11, hdRotMax = 4;  // all presets
	const static int16_t hdtMax = 100;  // cycle time

	int16_t hdt;  int8_t hdCur,  hdtOn, hdRot;
	void Hedrons(D d);
	
	//void Spiral, Fire..
#endif
};

//#undef D