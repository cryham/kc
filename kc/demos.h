#pragma once
#include "WProgram.h"
#include "def.h"

#ifdef DEMOS

//#ifdef DEMOS_PLASMA
// for: plasma, wave, ck_logo

//  sinus table  ----
#define SX  8192   // 8192  mem size 16kB
//#define SX  16384
#define SY  16384  // 16384  y quality
#define Cos(x)  sint[(SX/2+(x))%SX]  // %SX
#define Sin(x)  sint[      (x) %SX]

extern const int16_t sint[SX];
//#endif
#endif


struct Demos
{
	//  main  ----
	Demos();
	void Init(class Ada4_ST7735* tft);

	Ada4_ST7735* d;
	uint16_t* data = 0;  // scren buffer

	int8_t iFps;  // show frames per second, 1 on, 0 off
	uint32_t ti, oti;  // fps: time ms, old

	//  Fonts
	int8_t fntCur;
	const static int8_t fntMax = 3;  // all pages
	void Fonts();  // ver, date, chars


#ifdef DEMOS
	int8_t iInfo;  // show demo params
	void KeyPress(EDemo demo, struct Gui* gui);

	uint t;  // frame counter


#ifdef DEMOS_PLASMA
	//  Plasma  ~~~~
	int8_t plasma;  // cur mode
	const static int8_t num_plasma = 7;  // all sets
	int8_t tadd[num_plasma];  // speeds

	void Plasma(), PlasmaT(int8_t dt);
	void Plasma1(),Plasma2(),Plasma3(),Plasma4(),
		PlasmaC1(),PlasmaC2(),PlasmaC2b();
#endif

	//  Wave
	int8_t waveSpd;
	void Wave(), Fire();
	//Spiral, Water..


	//  CK logo, 2D lines
	const static int8_t ckMax = 3;
	int8_t ckCur, ckSpeed;
	void CK_logo();


#ifdef DEMOS_OLD_PAR
	//  Balls  --------
	const static int  // _params_
	//sMax = 10, bMax = 10, dMax = 10;  // 73%  48k ram
	//sMax = 20, bMax = 20, dMax = 20;  // 74%  49k ram
	//sMax = 240, bMax = 200, dMax = 250;  // 78%
	sMax = 240, bMax = 300, dMax = 550;  // 80%  53k =7k

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


	//  Space
	void SpaceInit();
	void StarNew(int i);  // new
	void Space();

	//  Fountain, drops  ----
	int fInt, fWave;
	void FountainInit();
	void Fountain();
#endif


	//  Rain  ----
	int8_t rCur, r1Int,r1Size, r2Int,r2Size;
	void Rain();
	

	//  Ngons 2D
	int16_t ngt;  int8_t ngCur,  ngtOn, ngRot;
	const static int8_t ngMin = 5, ngMax = 25, ngRotMax = 4;  // all n
	void Ngons();


#ifdef DEMOS_3D
	//  Polyhedrons 3D  ----
	const static int8_t hdA = 11, hdRotMax = 4, hdDiagMax = 5;  // all presets
	const static int16_t hdtMax = 100;  // cycle time

	int16_t hdt;  int8_t hdCur,  hdtOn, hdRot,  hdSpd, hdDiag;
	void Hedrons();
#endif

#endif  //DEMOS
};
