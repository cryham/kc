#include "gui.h"
#include "Ada4_ST7735.h"
#include "kc_data.h"


//  Graphs ~ ~
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
inline void GridLineP(Ada4_ST7735* d, const KC_Params& par, int m, uint16_t c, const char* s)
{
	int h = m * 60 / (6 * par.time1min);  // m min time
	int x = W-1 - h;
	if (x > 0)  d->drawFastVLine(x, 0, H/2, c);
	else  return;
	x -= 6;
	if (x < 12)  return;
	if (x < 0)  x = 0;
	d->setCursor(x, 0);
	d->setColor(c);
	d->print(s);
}

void Gui::DrawGraph()
{
#ifdef GRAPHS
	char a[64];
	d->setFont(0);

	//  grid lines ||
	GridLineP(d,par, 10, RGB( 9,  9,  9),"10");  // min
	GridLineP(d,par, 30, RGB(12, 12, 12),"30");
	GridLineP(d,par, 60, RGB(16, 16, 16),"1h");  // h
	GridLineP(d,par,120, RGB(16, 16, 16),"2h");
	GridLineP(d,par,240, RGB(16, 16, 16),"4h");
	GridLineP(d,par,480, RGB(16, 16, 16),"8h");


	//  legends text
	int x = 0, y = 14, v;
	d->setClr(20, 20, 25);
	d->setCursor(x, y);
	d->println("Press/min");

	v = H/2 * 4 / 2;  // max
	d->setClr(16, 16, 20);
	sprintf(a,"max %d", v);  d->println(a);

#ifdef TEMP1
	x = 0;  y = H/2 +14;
	d->setClr(18, 22, 25);
	d->setCursor(x, y);
	d->++("Temp \x01""C");

	d->setClr(14, 17, 20);
	sprintf(a,"max %d", maxTemp);  d->println(a);
	sprintf(a,"min %d", minTemp);  d->println(a);
#endif
	int i,ii, y0,h;  uint16_t c;

	//  graphs
	for (i=0; i <= W-1; ++i)
	{
		ii = kc.grPpos + i - (W-1) + W;
		v = kc.grPMin[ii % W];
		if (v > 0)
		{
			ClrPress(v);  c = d->getClr();

			h = 2 * v / 4;  // max
			if (h > H/2)  h = H/2;

			y0 = H/2 - h;
			if (y0 < 0)  y0 = 0;

			if (y0+h < H)
			d->drawFastVLine(i, y0, h, c);
	}	}
	//  Temp
#ifdef TEMP1
	for (i=0; i <= W-1; ++i)
	{
		ii = grTpos + i - (W-1) + W;
		v = grTemp[ii % W];
		//  0 = minTemp  255 = maxTemp
		if (v > 0)
		{
			ClrPress(v / 2);  c = d->getClr();

			h = H/2 * v / 256;
			if (h > H/2)  h = H/2;

			y0 = H-1 - h;
			if (y0 < 0)  y0 = 0;

			d->drawPixel(i,y0, c);
			//if (y0+h < H)
			//d->drawFastVLine(i, y0, h, c);
	}	}
#endif

#endif
}
