#include "gui.h"
#include "Ada4_ST7735.h"
#include "matrix.h"
#include "kc_data.h"


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


//  util
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

//  color from value
void Gui::ClrPress(int pm)
{
	if (pm >200)  d->setClr(31, 25,31);  else
	if (pm >150)  d->setClr(31, 20,25);  else
	if (pm >120)  d->setClr(31, 10,10);  else

	if (pm > 90)  d->setClr(31, 16, 2);  else
	if (pm > 70)  d->setClr(31, 20, 2);  else
	if (pm > 50)  d->setClr(24, 24, 4);  else
	if (pm > 30)  d->setClr( 6, 24, 6);  else
	if (pm > 10)  d->setClr( 4, 22,22);  else
				  d->setClr( 6, 16,24);
}
void Gui::ClrTemp(int tm)
{
	if (tm >224)  d->setClr(31, 10,10);  else
	if (tm >192)  d->setClr(31, 18, 4);  else
	if (tm >160)  d->setClr(30, 30, 2);  else
	if (tm >128)  d->setClr(16, 29, 2);  else
	if (tm > 96)  d->setClr( 6, 27, 6);  else
	if (tm > 64)  d->setClr( 6, 26,26);  else
	if (tm > 32)  d->setClr(10, 20,30);  else
				  d->setClr( 6, 16,29);
}

//  Display
//....................................................................................
void Gui::DrawClock()
{
	char a[64], f[32];


	//  time  ---
	unsigned long t = rtc_get(), to, ti;
	int h = t / 3600 % 24, m = t / 60 % 60, s = t % 60,
		dt = t / 3600 / 24, yr = dt / 365 + 2000;

	bool adjust = pgClock == Cl_Adjust;
	bool clock = adjust;

	bool date = yr >= 2019; // rtc set
	bool leap = isLeap(yr);

	bool ext = true;
	bool stats = true;

	//  x,y pos~
	int16_t x, y;
	const int16_t x0 = W / 2,
			yt = 32, yu = H - 20, yd = yt + 24, // time, uptime, date
			yp = yt + 22, yi = yu - 19;
	to = t - kc.tm_on;


	//  title
	d->setClr(12, 14, 17);
	if (clock)
		d->print(strMain[ym]);


	//  Graphs  ~~~~~~~~~~~~~~~~
	int v, y0, ii, i;
	if (ext)
	for (i=0; i <= W-1; ++i)
	{
		ii = kc.grPpos + i - (W-1) + W;
		v = kc.grPMin[ii % W];
		if (v > 0)
		{
			ClrPress(v);  uint16_t c = d->getClr();
			y0 = yt - v / 3;  // 96 is max  //par scale-
			if (y0 < 0)  y0 = 0;
			d->drawPixel(i,y0, c);
	}	}

	if (pgClock == Cl_Graphs)
	{
		DrawGraph();
		return;
	}

	x = 60;  y = 42;


	//  Time  ----------------
	if (clock || date)
	{
		d->setCursor(x, y);
		if (clock)
			d->setClr(22, 24, 26);
		else
			d->setClr(18, 22, 26);

		sprintf(a, "%2d:%02d:%02d", h, m, s);  d->print(a);
	}


	//  Uptime, since on  --------
	if (!adjust)
	{
		h = to / 3600 % 24;  m = to / 60 % 60;  s = to % 60;
		y = yu;
		d->setCursor(x, y);
		d->setClr(18, 22, 28);
		sprintf(a, "%d:%02d:%02d", h, m, s);  d->print(a);
	}


	//  Temp'C  val  --------
	#ifdef TEMP1
	if (!adjust && fTemp > -90.f)
	{
		dtostrf(fTemp, 4,2, f);
		if (pgClock == 0)
		{	d->setClr(18,22,26);
			d->setCursor(6, 71);
		}else
		{	if (pgClock == 2)
				d->setClr(16,20,24);
			else
				d->setClr(14,18,22);
			d->setCursor(6, 32);
		}
		d->print(f);
	}
	#endif


	d->setFont(0);


	//  small font  ----------------------


	//  page / all  ---
	d->setClr(12, 16, 22);
	if (!stats)
	{
		d->setCursor(W - 1 - 3 * 6, 4);
		sprintf(a, "%d/%d", pgClock + 1, Cl_All);  d->print(a);
	}

	//  subtitle
	if (!stats)
	{
		d->setCursor(W / 2 - 2 * 6, 4);
		d->print(strClock[pgClock]);
	}


	//  labels, par values  ====--------
	int pg = ClockVars(pgClock);
    #ifdef TEMP1
    bool temp = temp1 > 2;
    #endif
    y = yt;
	switch (pgClock)
	{

	case Cl_Stats:  //------------
	{
		#ifdef TEMP1
		if (temp)  // 'C
		{	d->setCursor(9*6, 32+4);  d->print("\x01""C");  }
		#endif

		//  press k
		d->setClr(9, 18, 22);
		x = x0 - 22;
		y = yp + 13;
		d->setCursor(x, y);
		sprintf(a, "%dk", cnt_press / 1000);  d->print(a);

		//  date small  ----
		if (date)
		{
			int mth = 0, day = 0;
			getMD(leap, dt % 365, &day, &mth);

			//  day, week
			x = x0 + 8;  y = 32 + 18;
			d->setClr(14, 18, 23);
			d->setCursor(x, y);
			d->print(wdays[DayOfWeek(day, mth, yr)]);
			x += 30;
			d->setCursor(x, y);
			sprintf(a, "%d.%d", day, mth);  d->print(a);
		}

		//  press / 1min
		x = 6;  y = yp;
		d->setCursor(x, y);

		int m1 = kc.min1_Keys;
		ClrPress(m1);
		//dtostrf(m1, 3,0, f);  d->print(f);

	}	break;


	case Cl_Adjust:  //  Adjust  ----
		for (int i = 0; i <= pg; ++i)
		{
			DrawClockCur(i, y);
			int8_t h = 2;
			switch (i)
			{
			case 0:  sprintf(a, "Hour");  break;
			case 1:  sprintf(a, "Minute");break;
			case 2:  sprintf(a, "Second");  h = 4;  break;

			case 3:  sprintf(a, "Day");  break;
			case 4:  sprintf(a, "Month");  break;
			case 5:  sprintf(a, "Year");  h = 10;  break;

			case 6:  sprintf(a, "RTC Compensate: %d", par.rtcCompensate);  h = 2;  break;
			}
			d->print(a);
			y += h + 8;
		}
		break;
	}
}
