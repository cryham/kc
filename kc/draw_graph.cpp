#include "gui.h"
#include "Ada4_ST7735.h"
#include "kc_data.h"


//  Grid  | |
inline void GridLineP(Ada4_ST7735* d, KC_Params& par, int m, uint16_t c, const char* s)
{
	int h = m * 60 / t1min(par);  // m min time
	int x = W-1 - h;
	if (x > 0)  d->drawFastVLine(x, 0, H/2, c);
	else  return;
	x -= 6;
	if (x < 12 || x >= W-12)  return;
	if (x < 0)  x = 0;
	d->setCursor(x, 0);
	d->setColor(c);
	d->print(s);
}
inline void GridLineT(Ada4_ST7735* d, KC_Params& par, int m, uint16_t c, const char* s)
{
	int h = m * 60000 / tTgraph(par);  // m min time
	int x = W-1 - h;
	if (x > 0)  d->drawFastVLine(x, H/2, H/2-1, c);
	else  return;
	x -= 6;
	if (x < 12 || x >= W-12)  return;
	if (x < 0)  x = 0;
	d->setCursor(x, H/2);
	d->setColor(c);
	d->print(s);
}

//  Graphs ~ ~
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void Gui::DrawGraph()
{
#ifdef GRAPHS

	#define getPv(i)  ii = kc.grPpos + i - (W-1) + W;  v = kc.grPMin[ii % W];
	#define getTv(i)  ii =    grTpos + i - (W-1) + W;  v =    grTemp[ii % W];

	char a[64];
	d->setFont(0);
	bool cursor = xCur < W;
	int i,ii, y0,h;  uint16_t c;

	//  grid 	// press/1min  ------------
	if (par.time1min)
	{
		GridLineP(d,par, 10, RGB( 9,  9,  9),"10");  // m
		GridLineP(d,par, 30, RGB(12, 12, 12),"30");
		GridLineP(d,par, 60, RGB(16, 16, 16),"1h");  // h
		GridLineP(d,par,120, RGB(16, 16, 16),"2h");
		GridLineP(d,par,240, RGB(16, 16, 16),"4h");
		GridLineP(d,par,480, RGB(16, 16, 16),"8h");
	}

	//  legend
	int x = 0, y = 9, v;
	d->setClr(20, 20, 25);
	d->setCursor(x, y);
	d->println("Press/min");
	d->moveCursor(0,2);

	if (cursor)
	{
		getPv(xCur);
		ClrPress(v);
		sprintf(a,"%d", v);  d->println(a);

		d->moveCursor(0,1);
		PrintInterval(t1min(par)*1000*(W-1-xCur));  d->println("");
	}
	v = H/2 * 4 / 2;  // max
	d->setClr(16, 16, 20);
	d->moveCursor(0,2);
	if (!cursor)  d->print("max ");
	sprintf(a,"%d", v);  d->println(a);

	//  graph  Press/1min
	for (i=0; i <= W-1; ++i)
	{
		getPv(i);
		if (v > 0)
		{
			ClrPress(v);  c = d->getClr();

			h = 2 * v / 4;  // max
			if (h > H/2)  h = H/2;

			y0 = H/2 - h;
			if (y0 < 0)  y0 = 0;

			if (y0+h < H)
				d->drawFastVLine(i, y0, h, c);

			if (i == xCur)
				d->drawPixel(i, y0, RGB(31,31,31));  //.
	}	}

#ifdef TEMP1
	//  grid	// Temp'C  ------------
	{
		GridLineT(d,par, 10, RGB( 9,  9,  9),"10");  // m
		GridLineT(d,par, 30, RGB(12, 12, 12),"30");
		GridLineT(d,par, 60, RGB(16, 16, 16),"1h");  // h
		GridLineT(d,par,120, RGB(16, 16, 16),"2h");
		GridLineT(d,par,240, RGB(16, 16, 16),"4h");
		GridLineT(d,par,480, RGB(16, 16, 16),"8h");
	}

	//  legend
	x = 0;  y += H/2;
	d->setClr(18, 22, 25);
	d->setCursor(x, y);
	d->println("Temp \x01""C");
	d->moveCursor(0,2);

	if (cursor)
	{
		d->drawPixel(xCur,H/2, RGB(29,29,29));  //.

		//d->print("cur ");
		getTv(xCur);
		ClrPress(v / 2);

		float f = xCur == W-1 ? fTemp : // latest
			v / 255.f * (par.maxTemp - par.minTemp) + par.minTemp;
		dtostrf(f,4,2,a);  d->println(a);

		d->moveCursor(0,1);
		PrintInterval(tTgraph(par)*(W-1-xCur));  d->println("");
	}
	d->setClr(14, 17, 20);
	d->moveCursor(0,2);
	if (!cursor)  d->print("max ");
	sprintf(a,"%d", par.maxTemp);  d->println(a);
	if (!cursor)  d->print("min ");
	sprintf(a,"%d", par.minTemp);  d->println(a);

	//  graph  Temp
	for (i=0; i <= W-1; ++i)
	{
		getTv(i);
		if (v > 0)
		{
			ClrPress(v / 2);  c = d->getClr();
			if (i == xCur)  c = RGB(31,31,31);  //.

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
