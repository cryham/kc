#include "def.h"
#ifdef DEMOS
#ifdef DEMOS_OLD_PAR
#include "demos.h"
#include "Ada4_ST7735.h"
#include "FreeSans9pt7b.h"  // 2k flash, 1%


//  Balls
//....................................................................................
void Demos::BallsInit()
{
	int v1 = bSpd * (bSpRMax - bSpRnd) / bSpRMax,
		v0 = bSpd * bSpRnd / bSpRMax;

	for (int i=0; i < bCnt; i++)
	{	Ball& b = ball[i];    b.r = random(bRad);
		b.x = (random(W - b.r*2) + b.r) *bDet;
		b.y = (random(H - b.r*2) + b.r) *bDet;
		b.vx = random(v1) + v0;
		b.vy = random(v1) + v0;

		b.c = RGB(random(32), random(32), random(32));
		if (random(2))  b.vx = -b.vx;
		if (random(2))  b.vy = -b.vy;
	}
	einit = IBalls;
}

void Demos::Balls()
{
	if (einit != IBalls)
		BallsInit();
	
	for (int i=0; i < bCnt; i++)
	{	Ball& b = ball[i];
		if (b.x < b.r *bDet || b.x >= (W-b.r) *bDet)  b.vx = -b.vx;
		if (b.y < b.r *bDet || b.y >= (H-b.r) *bDet)  b.vy = -b.vy;
		b.x += b.vx;  b.y += b.vy;

		if (b.r)
			d->drawCircle(b.x/bDet, b.y/bDet, b.r, b.c);
		else
			d->drawPixel(b.x/bDet, b.y/bDet, b.c);
	}

	if (iInfo > 0)
	{
		d->setCursor(0,0);
		d->print("Cnt ");  d->println(bCnt);
		d->print("Spd ");  d->println(bSpd);
		d->print("Rnd ");  d->println(bSpRnd);
		d->print("Rad ");  d->println(bRad);
	}
}


//  Space
//....................................................................................
void Demos::SpaceInit()
{
	for (int i=0; i<sCnt; i++)
		StarNew(i);
	einit = ISpace;
}
#define sRng 7000  //300 init range
#define sDet 4  // detail

const static int
	sNear = sDet*200, sFar = sDet*1350;  //750 range: cut off, new appear

void Demos::StarNew(int i)
{
	star[i].x = (random(W*sRng) -W*sRng/2) * sDet;
	star[i].y = (random(H*sRng) -H*sRng/2) * sDet;
	star[i].z = random(sFar) + sNear;   // depth
	star[i].v = random(sDet*sVel/2) + sDet*sVel/4;  // speed
	//  clr
	int8_t r=31,g=31,b=31;  // white
	switch(random(10))
	{
	case 0:
	case 1:  r= 31;  g = random(5)+26, b = random(15)+10;  break;  // yellow
	case 2: case 3:
	case 4:  r= random(10)+11;  g = random(10)+21;  b = 31;  break;  // cyan
	case 5:
	case 6:  r= 11;  g = random(15)+16;  b = 31;  break;  // blue
	case 7:  r= random(20)+11;  g = random(10)+6;  b = random(5);  break;  // red
	}
	star[i].c = RGB(r, g, b);
}
void Demos::Space()
{
	if (einit != ISpace)
		SpaceInit();
	
	for (int i=0; i < sCnt; i++)
	{
		int z = star[i].z + 1*sDet;
		int x = star[i].x/z / sDet +W/2;  // pos 3d to 2d
		int y = star[i].y/z / sDet +H/2;
		d->drawPixel(x,y, star[i].c);
			
		star[i].z -= star[i].v;
		if (x < 0 || x > W ||
			y < 0 || y > H ||  // outside
			star[i].z < sNear)  // too close
		{
			StarNew(i);  star[i].z = sNear+sFar;
		}
	}
	if (iInfo > 0)
	{
		d->setCursor(0,0);
		d->print("Cnt ");  d->println(sCnt);
		d->print("Vel ");  d->println(sVel);
	}
}


//  Fountain
//....................................................................................
void Demos::FountainInit()
{
	for (int i=0; i < dMax; i++)
		drop[i].t = -1;
	einit = IDrops;
}

void Demos::Fountain()
{
	if (einit != IDrops)
		FountainInit();
	
	int nn = 0;  int io = 0;
	for (int i=0; i < dMax; ++i)
	{	Drop& o = drop[i];

		if (o.t >= 0)  // alive
		{	--o.t;  ++io;

			o.x += o.vx;  // vel
			o.y += o.vy;
			o.vy += 4;  // gravity

			int x = o.x / bDet;  // pos
			int y = o.y / bDet;

			if (x < 0 || x >= W || //y < 0 ||
				y >= H)  // outside
			{	o.t = -1;  }
			else
				d->drawPixel(x,y, o.c);
		}
		else  // spawn new
		if (nn < fInt)  // max 9
		{	++nn;
			o.t = random(160) + 20 + fInt * 8;  // time to live
			#define dVx  150
			o.x = 78*bDet + random(4*bDet);
			o.y = 127*bDet - random(4*bDet);
			//  vel up, with wave
			int rx = 120 + 110 * Sin(t*61)/SY * Cos(t*44)/SY,
				px =       100 * Sin(t*42)/SY * Cos(t*24)/SY,
				py =   40 + 40 * Sin(t*53)/SY * Cos(t*39)/SY;
			o.vx = random(rx+1)-rx/2 + px;
			o.vy = -random(70) - 320 + py;

			int c = random(32);
			o.c = RGB(min(31, c/2+10), min(31, c+18), 31);
		}
	}

	if (iInfo > 0)
	{
		d->setCursor(0,0);
		d->print("Int ");  d->println(fInt);
		d->print("Spd ");  d->println(fWave);

		d->setCursor(0, H-8);
		d->println(io);  // drops used
	}
	t += fWave;
}
#endif
#endif
