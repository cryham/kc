#include "demos.h"
#include "Ada4_ST7735.h"
#include "FreeSans9pt7b.h"  // 2k flash, 1%


#ifdef DEMOS

//  regular polygons with diagonals
//....................................................................................
void Demos::Ngons()
{
	#define ngtM 200
	float a=0, b=0, e = 2*PI / ngCur;  // angles
	float tm = float(ngt)/ngtM;
	float c = 0.0, s = 0.8;

	switch (ngRot)
	{
	case 0: c = -e*2 * tm;  s = 0.85 + 0.75 * tm;  break;
	case 1: c = -e * tm;  s = 1.0;  break;
	case 2: c = 0.0;  s = 0.8 + 0.6 * tm;  break;
	case 3: c = 0.0;  s = 1.0;  break;
	}
	float xx = W/2, yy = H/2, sy = s * yy;

	uint8_t cc=0;
	for (b=0; b < 2*PI; b+=e)
	{
		float xb = xx + sy*cos(b+c), yb = yy - sy*sin(b+c);
		for (a=0; a < b && a < 2*PI; a+=e)
		{
			float xa = xx + sy*cos(a+c), ya = yy - sy*sin(a+c);
			if (a != b)
				d->drawLine(xb, yb, xa, ya, RGB(31-cc/32,(cc/4)%32,(cc*2)%32));
			++cc;
		}
	}
	++ngt;
	if (ngt > ngtM) {  ngt = 0;
		if (ngtOn) {  ++ngCur;  if (ngCur > ngMax)  ngCur = ngMin;  }  }

	if (iInfo > 0)
	{
		d->setCursor(0,0);
		d->setTextColor(RGB(29,29,20));
		d->print("An");  d->println(ngRot);

		d->setCursor(0, H-8);
		d->println(ngCur);  // n sides
	}
}


//  Rain circles
//....................................................................................
void Demos::Rain()
{
	int x,y,r,i, s = rCur ? r2Size : r1Size;
	#define RClr  RGB(random(12)+5, random(20)+8, random(5)+25)
	if (!rCur)  // 0
	{		x=random(W); y=random(H);  r= random(26);  d->drawCircle(x,y,r,0);
			x=random(W); y=random(H);  r= random(24);  d->drawCircle(x,y,r,0);
			x=random(W); y=random(H);  r= random(20);  d->drawCircle(x,y,r,0);

		if ((int)random(100) < 30 + r1Int * 5)
		{	x=random(W); y=random(H);  r= random(6+s)+1+s;  d->drawCircle(x,y,r,RClr);
		}
	}else  // 1
	{
		for (i=0; i < 12 + r2Int; ++i)
		{
			if (random(400) < 10) {  x=random(W); y=random(H);  r= random(28)+20;  d->drawCircle(x,y,r,0);  }
			if (random(200) < 10) {  x=random(W); y=random(H);  r= random(20)+16;  d->drawCircle(x,y,r,0);  }
			if (random(100) < 20) {  x=random(W); y=random(H);  r= random(14)+14;  d->drawCircle(x,y,r,0);  }
			if (random(10)  < 3)  {  x=random(W); y=random(H);  r= random(10)+1;   d->drawCircle(x,y,r,0);  }
		}
		for (i=0; i < 2 + r2Int; ++i)  //if (random(10) < 8)
		{
			if (random(600) < 30) {  x=random(W); y=random(H);  r= s+random(6+s)+3;  d->drawCircle(x,y,r,RClr);  }
			if (random(300) < 40) {  x=random(W); y=random(H);  r= s+random(5+s)+4;  d->drawCircle(x,y,r,RClr);  }
			if (random(100) < 50) {  x=random(W); y=random(H);  r= s+random(4)+2;  d->drawCircle(x,y,r,RClr);  }
									 x=random(W); y=random(H);  r= random(3)+1;  d->drawCircle(x,y,r,RClr);
	}	}
	if (iInfo > 0)
	{
		d->setCursor(0,0);
		d->print("Int ");  d->println(rCur ? r2Int : r1Int);
		d->print("Size "); d->println(s);
		d->print("Cur ");  d->println(rCur);
	}
}
#endif


//  Fonts
//....................................................................................
void Demos::Fonts()
{
	switch (fntCur)
	{
	case 0:
	{	d->setCursor(0,18);  // logo
		d->setFont(&FreeSans9pt7b);
		d->setTextColor(RGB(1,28,28));
		d->println("CrystaL");

		d->setCursor(36,40);
		d->setTextColor(RGB(22,16,31));
		d->print("Keyboard");
		d->setFont(0);

		d->setCursor(0,70);
		d->setTextColor(RGB(12,21,31));
		d->print("K.C. \"Kacey\" Controller");

		d->setCursor(0,H-23);
		d->setTextColor(RGB(16,25,31));
		d->print("ver 0.80");  /// version text

		d->setTextColor(RGB(21,26,31));
		d->setCursor(0, H-8);
		const char* a={__DATE__}, *m={__TIME__};
		const char dt[] = {  //  build date, time   format yyyy-mmm-dd hh:mm
			a[7],a[8],a[9],a[10],' ',a[0],a[1],a[2],' ',a[4],a[5],' ',' ',m[0],m[1],':',m[3],m[4],0};
		d->print(dt);
	}	break;

	case 1:  // big chars
	{	uint x=0, y=0, yw=18;
		d->setFont(&FreeSans9pt7b);
		d->setTextColor(RGB(21,18,31));
		d->setCursor(x,y); y+=yw;
		d->println("ABCDEFGHIJKLM");
		d->setCursor(x,y); y+=yw;
		d->println("NOPQRSTUVWXYZ");

		d->setTextColor(RGB(11,22,31));
		d->setCursor(x+22,y); y+=yw;
		d->println("abcdefghijklm");  d->setCursor(x,y); y+=yw;
		d->println("nopqrstuvwxyz");  d->setCursor(x+22,y); y+=yw;

		d->setTextColor(RGB(30,25,10));
		d->println("0123456789");	d->setCursor(x,y); y+=yw;
		d->setTextColor(RGB(14,27,11));
		d->println("!\"#$%&\'()*+,-./:;");  d->setCursor(x,y); y+=yw;
		d->println("<=>?@[\\]^_`{|}~");

		d->setFont(0);
		d->setTextColor(RGB(21,26,31));

	}	break;

	case 2:  // small all chars
	{
		d->setCursor(6,8);
		d->setTextColor(RGB(21,26,31));

		for (int i=0; i < 256; i++)
		{
			if (i == '\n') continue;
			d->write(i);

			if (i % 24 == 0)
			{	if (i > 0)
				{	d->print('\n');
					d->moveCursor(0, 1);  }
				d->setTextColor(RGB(21,26,31));

				int ii = i / 24;
				if (ii % 4 == 0)
					d->moveCursor(0, 3);
				if (ii % 2 == 0)
					d->setTextColor(RGB(26,29,31));
			}
			if (i % 8 == 0 && i > 0)
				d->moveCursor(2, 0);
		}
	}	break;
	}
}


#ifdef DEMOS

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
