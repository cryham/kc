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
	if (pm >150)  d->setClr(31, 22,22);  else
	if (pm >120)  d->setClr(31, 12,12);  else
	if (pm > 90)  d->setClr(31, 12, 2);  else
	if (pm > 70)  d->setClr(30, 18, 2);  else
	if (pm > 50)  d->setClr(24, 24, 4);  else
	if (pm > 30)  d->setClr( 6, 24, 6);  else
	if (pm > 10)  d->setClr( 4, 22,22);  else
				  d->setClr( 6, 16,24);
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
	bool simple = pgClock == Cl_Simple;
	bool clock = simple || adjust;

	bool date = yr >= 2019; // rtc set
	bool leap = isLeap(yr);

	bool ext = pgClock == Cl_StatsExt;
	bool stats = pgClock == Cl_Stats || ext;

	//  x,y pos~
	int16_t x, y;
	const int16_t x0 = W / 2,
			yt = 32, yu = H - 20, yd = yt + 24, // time, uptime, date
			yp = yt + 22, yi = yu - 19;
	to = t - tm_on;


	//  late hours Background  --------
	x = x0;  y = yt;
	if (date && kbdSend)  // not in menu
	{
		int8_t r = 0, g = 0, b = 0, hf = m >= 30 ? 5 : 0;
		if (h == 22){  r = hf + 10;  g = 4;  }  else
		if (h == 23){  r = 19;  g = hf + 6;  }  else
		if (h == 0) {  r = 25 + hf;  g = hf + 12;  b = 6;  }  else
		if (h < 6)  {  r = 31;  g = 24;  b = 10;  }

		if (r)
		for (int y=0; y < r + 17; ++y)
		{
			d->drawFastHLine(0, y, W - 1, RGB(r, g, b));
			//d->drawFastHLine(0,H-1-y,W-1, RGB(r/2,g/2,b/2));
			if (r > 0 && y%2==0)  --r;
			if (g > 0 && y%2==0)  --g;
			if (b > 0 && y%3==0)  --b;
		}
	}

	//  title
	d->setClr(12, 14, 17);
	if (clock)
		d->print(strMain[ym]);


	//  Graphs  ~~~~~~~~~~~~~~~~
	#ifdef GRAPHS
	int v, y0, ii, i;
	if (ext)
	for (i=0; i <= W-1; ++i)
	{
		ii = kc.grPpos + i - (W-1) + W;
		v = kc.grPMin[ii % W];
		if (v > 0)
		{
			ClrPress(v);  uint16_t c = d->getClr();
			y0 = yt - v / 3;  // 96 is max
			if (y0 < 0)  y0 = 0;
			d->drawPixel(i,y0, c);
	}	}

	if (pgClock == Cl_Graphs)
	{
		DrawGraph();
		return;
	}
	#endif


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


	//  time Inactive  --------
	if (!clock)
	{
		ti = t - kc.tm_key;  // actual
		bool inact = ti > 60 * par.minInactive;
		h = ti / 3600 % 24;  m = ti / 60 % 60;

		s = h*60 + m;
		if (s > 90)  d->setClr(30, 24, 30);  else
		if (s > 60)  d->setClr(24, 22, 28);  else
		if (s > 30)  d->setClr(20, 20, 26);  else
		             d->setClr(14, 18, 24);
		y = yi;
		d->setCursor(x, y);
		sprintf(a, "%d:%02d%c", h, m, inact?'*':' ');  d->print(a);


		//  time Active  --------
		if (stats)
		{
			ti = kc.tm_key - kc.tm_keyAct;
			h = ti / 3600 % 24;  m = ti / 60 % 60;

			s = h*60 + m;
			if (s > 180)  d->setClr(28, 10,  5);  else
			if (s > 110)  d->setClr(26, 13, 16);  else
			if (s > 50)   d->setClr(24, 18, 10);  else
			if (s > 25)   d->setClr(14, 22, 24);  else
			              d->setClr(15, 15, 20);
			y = yp + 14;
			d->setCursor(x, y);
			sprintf(a, "%d:%02d%c", h, m, inact?' ':'*');  d->print(a);
		}


		//  Press/min  --------
		x = stats ? 6 : x0;
		y = yp + (pgClock == Cl_StatsText ? 13 : 14 -4);
		d->setCursor(x, y);

		float fto = to ? to : 1;
		float pm = 60.f * cnt_press / fto;
		dtostrf(pm, 4, 2, f);

		ClrPress(pm);
		d->print(f);
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


	//  Layer  --------
	if (stats)
	{
		bool lock = kc.nLayerLock >= 0,
			  def = kc.nLayer == par.defLayer;

		if (lock) d->setClr(28, 23, 30);  else
		if (def)  d->setClr(14, 14, 22);  else
		/*hold*/  d->setClr(17, 17, 28);

		sprintf(a, "L%d %c", kc.nLayer, lock ? '*' : def ? ' ' : '+');
		d->setCursor(6, yu);  d->print(a);
	}


	//  Date big  --------
	if (clock && date)
	{
		int mth = 0, day = 0, x2 = x0 + 36;
		getMD(leap, dt % 365, &day, &mth);
		//  day, week
		x = x0 + 4;
		y = yd;
		d->setCursor(x, y);
		d->setClr(20, 23, 26);
		sprintf(a, "%02d", day);  d->print(a);

		d->setCursor(x2, y);
		d->print(wdays[DayOfWeek(day, mth, yr)]);

		y += 16; // month
		d->setCursor(x, y);
		sprintf(a, "%d", mth);  d->print(a);

		d->setCursor(x2, y);
		d->print(mths[mth - 1]);
		d->setFont(0);

		y += 16; // year
		d->setCursor(x, y);
		sprintf(a, "%04d", yr);  d->print(a);
	}
	else
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
	if (!stats && !simple)
	{
		d->setCursor(W / 2 - 2 * 6, 4);
		d->print(strClock[pgClock]);
	}


	//  labels, par values  ====--------
	int pg = ClockVars(pgClock);
	y = yt;
	switch (pgClock)
	{

	case Cl_Simple:
	{
		d->setClr(12, 18, 22);
		x = 6;  y = yu;
		d->setCursor(x, y + 4);  d->print("Uptime");

		#ifdef TEMP1  // Temp'C
		if (temp1 == 2)
		{	d->setClr(12,20,25);
			d->setCursor(6,54);  d->print("Temp \x01""C");
		}
		#endif
	}	break;

	case Cl_StatsText:  //  old, labels
	{
		d->setClr(12, 22, 30);
		x = x0 + 4;  y = yp;
		d->setCursor(x, y);
		sprintf(a, "%d", cnt_press);  d->print(a);
		//sprintf(a, "%lu", t);  d->print(a);

		d->setClr(10, 14, 18);
		x = 6;  y = yp;
		d->setCursor(x, y);  d->print("Pressed");
		y += 16;
		d->setCursor(x, y);  d->print("Press/min");

		x = 6;  y = yi;
		d->setCursor(x, y + 2);  d->print("Inactive");
		x = 6;  y = yu;
		d->setCursor(x, y + 4);  d->print("Uptime");

		#ifdef TEMP1
		if (temp1 == 2)  // 'C
		{	d->setCursor(9*6, 32+4);  d->print("\x01""C");  }
		#endif
	}	break;

	case Cl_Stats:  //  new  no labels
	case Cl_StatsExt:  //------------
	{
		#ifdef TEMP1
		if (temp1 == 2)  // 'C
		{	d->setCursor(9*6, 32+4);  d->print("\x01""C");  }
		#endif
		if (pgClock == Cl_StatsExt)
		{
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

			int m1 = kc.min1_Keys; // / 60.f;
			ClrPress(m1);
			dtostrf(m1, 3,0, f);  d->print(f);


			//  previous 2 inactive times  --
			x = W - 30;
			ti = kc.tInact1;
			h = ti / 60 % 24;  m = ti % 60;

			d->setClr(15, 19, 24);
			d->setCursor(x, yi + 5);
			sprintf(a, "%d:%02d", h, m);  d->print(a);

			ti = kc.tInact2;
			h = ti / 60 % 24;  m = ti % 60;

			d->setClr(12, 16, 22);
			d->setCursor(x, yi - 4);
			sprintf(a, "%d:%02d", h, m);  d->print(a);

			//  inactive Sum  --
			ti = kc.tInactSum;
			h = ti / 60 % 24;  m = ti % 60;

			d->setClr(18, 18, 26);
			d->setCursor(x, yi - 18);
			sprintf(a, "%d:%02d", h, m);  d->print(a);
		}


		//  Sequence, prv  --------
		if (kc.inSeq[0] >= 0)
		{
			d->setCursor(0, yu - 13);
			d->setClr(25, 23, 31);
			sprintf(a, "S%d", kc.inSeq[0]);  d->print(a);

			d->setCursor(0, yu - 23);
			DrawSeq(kc.inSeq[0], 2);
		}

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
