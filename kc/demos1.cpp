#include "demos.h"
#include "Ada4_ST7735.h"
#include "FreeSans9pt7b.h"  // 2k flash, 1%
#include "TomThumb.h"

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
	case 0:  // logo, ver
	{
		d->setCursor(0,18);
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
		d->print("ver 0.95");
		//  version text  ^

		d->setTextColor(RGB(21,26,31));
		d->setCursor(0, H-8);
		const char* a={__DATE__}, *m={__TIME__};
		const char dt[] = {  //  build date, time   format yyyy-mmm-dd hh:mm
			a[7],a[8],a[9],a[10],' ',a[0],a[1],a[2],' ',a[4],a[5],' ',' ',m[0],m[1],':',m[3],m[4],0};
		d->print(dt);
	}	break;

	case 1:  // big chars
	{
		uint x=0, y=0, yw=18;
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

	case 3:  // tiny all chars
	{
		d->setFont(&TomThumb);
		d->setCursor(6,8);
		d->setTextColor(RGB(21,26,31));

		for (int i=0; i < 128; i++)
		{
			if (i == '\n') continue;
			d->write(i);

			if (i % 20 == 0)
			{	if (i > 0)
				{	d->print('\n');
					d->moveCursor(0, 1);  }
				d->setTextColor(RGB(24,27,31));

				int ii = i / 20;
				if (ii % 4 == 0)
					d->moveCursor(0, 2);
				if (ii % 2 == 0)
					d->setTextColor(RGB(26,29,31));
			}
			if (i % 8 == 0 && i > 0)
				d->moveCursor(2, 0);
		}
		d->setFont(0);
	}	break;
}
}
