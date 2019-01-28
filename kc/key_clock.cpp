#include "gui.h"

const uint8_t Gui::ClockPages[Gui::Clock_All] = {0,0,0,5};


//  Clock
//....................................................................................
void Gui::KeysClock()
{
	if (kUp)  // y
	{	ym2Clock = RangeAdd(ym2Clock, kUp, 0, ClockPages[pgClock], 1);  }
	else
	if (kPgUp)  // pg
	{	pgClock = RangeAdd(pgClock, kPgUp, 0, Clock_All-1, 1);
		ym2Clock = RangeAdd(ym2Clock, 0, 0, ClockPages[pgClock], 1);
	}else
	if (kRight && pgClock == pgClkAdj)  // adjust values
	{	unsigned long tm = rtc_get(), td = 0;
		int a = kRight * (kCtrl ? 10 : 1);
		if (a)
		{	switch (ym2Clock)
			{
			case 0:  td = a * 3600;  break;  // h
			case 1:  td = a * 60;  break;  // m
			case 2:  td = a * 1;  break;  // s
			case 3:  td = a * 3600*24;  break;  // day
			case 4:  td = a * 3600*24*30;  break;  // mth-
			case 5:  td = a * 3600*24*365;  break;  // yr
			//todo: rtc_compensate(int adjust)
			}
			if (tm + td >= 0)
			{	tm += td;  if (tm_on + td >= 0)  tm_on += td;
				rtc_set(tm);  }
		}
	}
	if (kAdd || kBckSp)  --mlevel;
}
