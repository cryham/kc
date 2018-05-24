#include "gui.h"
#include "Ada4_ST7735.h"
#include "matrix.h"
#include "kc_data.h"

#ifdef TEMP1
#include <OneWire.h>
#include <DS18B20.h>

//  sensor address
byte addr[8] = {0,};

OneWire onewire(TEMP1);  // pin
DS18B20 sensors(&onewire);

int8_t temp1 = 1;  // fist, init
float fTemp = 0.f;  // 'C
uint16_t skip = 0;  // read inactive
#endif


//  const
uint8_t MDays[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const int8_t t12[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

const char* mths[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
const char* wdays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};


//  date from day number
inline bool isLeap(int y)
{
	return y%400==0 || (y%4==0 && y%100!=0);
}

//  days in month 1..12
int getMthDays(bool leap, int m)
{
	int md = MDays[m];
	if (leap && m == 2)
		md = 29;
	return md;
}

//  day in year 0..365
void getMD(bool leap, int day, int* mday, int* mth)
{
	int m = 1, md;
	while (day >= (md = getMthDays(leap, m)))
	{
		day -= md;  ++m;
	}
	*mday = day+1;  *mth = m;
}

int DayOfWeek(int d, int m, int y)
{
    y -= m < 3;
    return (y + y/4 - y/100 + y/400 + int(t12[m-1]) + d) % 7;
}

const char* sPgClock[Gui::Clock_All] = {"", "Stats", "Adjust"};


//  Display
//....................................................................................
void Gui::DrawClockCur(int i, int16_t y)
{
	d->setCursor(2, y);
	int c = abs(i - ym2Clock);  // dist dim
	if (!c)
	{	d->fillRect(0, y-1, W/2, 10, RGB(4,7,10));
		d->setClr(21,26,31);
		d->print("\x10 ");  // >
	}else
		d->print("  ");

	FadeClr(C_Clock, 4, c, 1);
}

void Gui::DrawClock()
{		
	char a[64],f[32];
	//  time  ---
	unsigned long t = rtc_get(), to, ti;
	int h = t/3600%24, m = t/60%60, s = t%60,
		dt = t/3600/24, yr = dt/365 + 2000;

	bool clock = !pgClock || pgClock == 2;
	bool date = yr >= 2018;  // rtc set


	//  late hours, background color
	//if (pgClock < 2)
	{
		int8_t r=0, g=0, b=0, hf = m >= 30 ? 4 : 0;
		if (h == 22){  r = hf + 10;  g = 4;  }  else
		if (h == 23){  r = 19;  g = hf + 6;  }  else
		if (h == 0) {  r = 23 + hf;  g = hf + 12;  b = 6;  }  else
		if (h < 6)  {  r = 30;  g = 20;  b = 10;  }

		if (r)
		for (int y=0; y < r + 15; ++y)
		{
			d->drawFastHLine(0,y,W-1, RGB(r,g,b));
			if (r > 0 && y%2==0)  --r;
			if (g > 0 && y%4==0)  --g;
			if (b > 0 && y%3==0)  --b;
		}
	}

	//  title
	d->setClr(12,14,17);
	d->print(strMain[ym]);


	//  x,y pos
	int16_t x = W/2, y = 32, yo = H-20, yd = 32+24,
			yp = y+22, yi = yo-20;
	to = t - tm_on;


	//  time  ----
	if (clock || date)
	{
		d->setCursor(x, y);
		d->setClr(22,24,26);

		if (!clock)
			d->setClr(15,19,24);
		sprintf(a,"%2d:%02d:%02d", h, m, s);
		d->print(a);
	}

	//  uptime, since on  --
	int dn = to/3600/24 %10;
	h = to/3600%24;  m = to/60%60;  s = to%60;

	y = yo;
	d->setCursor(x -10, y);
	if (pgClock == 1)
		d->setClr(18,22,28);
	else
		d->setClr(16,21,26);
	sprintf(a,"%d %2d:%02d:%02d", dn, h, m, s);
	d->print(a);


	//  time inactive  --
	if (pgClock == 1)
	{
		ti = t - kc.tm_key;
		h = ti/3600%24;  m = ti/60%60;  s = ti%60;

		d->setCursor(x+5, yi);
		d->setClr(14,18,24);
		sprintf(a,"%2d:%02d:%02d", h, m, s);
		d->print(a);

		y = yp + 13;  // press/min
		d->setCursor(x+4, y);
		d->setClr(14,22,24);
		float fto = to ? to : 1;
		float pm = 60.f * cnt_press / fto;
		dtostrf(pm, 5,2, f);
		d->print(f);
	}


	#ifdef TEMP1  // Temp'C  **
	if (pgClock == 0 && fTemp > 0.f)
	{
		dtostrf(fTemp, 4,2, f);
		d->setClr(18,22,26);
		d->setCursor(6,71);  d->print(f);
	}
	#endif


	//  date  ----
	if (clock && date)
	{
		//  date
		int mth=0, day=0, x2 = x + 36;
		getMD(isLeap(yr), dt%365, &day, &mth);

		//  day, week
		x += 6;  y = yd;
		d->setCursor(x, y);
		d->setClr(20,23,26);
		sprintf(a,"%02d", day);  d->print(a);
		d->setCursor(x2, y);
		d->print(wdays[DayOfWeek(day, mth, yr)]);

		y += 16;  // month
		d->setCursor(x, y);
		sprintf(a,"%d", mth);  d->print(a);
		d->setCursor(x2, y);
		d->print(mths[mth-1]);
		d->setFont(0);

		y += 16;  // year
		d->setCursor(x, y);
		sprintf(a,"%04d", yr);  d->print(a);
	}else
		d->setFont(0);


	//  page, subtitle  --
	d->setCursor(W-1 -3*6, 4);
	d->setClr(12,16,22);
	sprintf(a,"%d/%d", pgClock+1, Clock_All);
	d->print(a);

	d->setCursor(W/2 -2*6, 4);
	d->print(sPgClock[pgClock]);


	//  par values  ====
	int pg = ClockPages[pgClock];
	y = 32;
	switch (pgClock)
	{
	case 0:
	{	d->setClr(12,18,22);
		x = 6;	y = yo;  d->setCursor(x,y+4);  d->print("Uptime");

		#ifdef TEMP1  // 18B20  Temp'C
		//  first, setup
		if (temp1 == 1)
		{	temp1 = 0;

			//  Look for 1-Wire devices
			if (onewire.search(addr))  // while
			{
				if (OneWire::crc8(addr, 7) != addr[7])
					break;
				//onewire.reset_search();

				//  setup
				sensors.begin(12);  // quality bits
				sensors.request(addr);
				temp1 = 2;
			}
		}
		if (temp1 == 2)
		{
			++skip;
			//  slower if not in gui, every few sec
			if (skip > 300 || !kbdSend)
			//  if measurement ready
			if (sensors.available())
			{	skip = 0;
				fTemp = sensors.readTemperature(addr);
				sensors.request(addr);  // next
			}
			//  Temp'C
			d->setClr(12,20,25);
			d->setCursor(6,58);  d->print("Temp \xF8""C");
		}
		#endif
	}	break;

	case 1:  //  stats  labels
	{	d->setClr(12,22,30);
		x = W/2+6;  y = yp;  d->setCursor(x,y);
		sprintf(a, "%d", cnt_press);  d->print(a);

		d->setClr(12,16,24);
		x = 6;	y = yp;  d->setCursor(x,y);  d->print("Pressed");
		y += 16;  d->setCursor(x,y);  d->print("Press/min");

		x = 6;	y = yi;  d->setCursor(x,y+2);  d->print("Inactive");
		x = 6;	y = yo;  d->setCursor(x,y+4);  d->print("Uptime");
	}	break;

	case 2:  //  adjust
		for (int i=0; i <= pg; ++i)
		{
			DrawClockCur(i, y);
			int8_t h = 2;
			switch(i)
			{
			case 0:  sprintf(a,"Hour");  break;
			case 1:  sprintf(a,"Minute");  break;
			case 2:  sprintf(a,"Second");  h = 4;  break;
			case 3:  sprintf(a,"Day");  break;
			case 4:  sprintf(a,"Month");  break;
			case 5:  sprintf(a,"Year");  break;
			}
			d->print(a);  y += h+8;
		}	break;
	}
}
