#include "gui.h"
#include "kc_params.h"


//  Clock
//....................................................................................
void Gui::KeysClock()
{
	if (kUp)  // y
	{	ym2Clock = RangeAdd(ym2Clock, kUp, 0, ClockVars(pgClock), 1);  }
	else
	if (kPgUp)  // pg
	{	pgClock = RangeAdd(pgClock, kPgUp, 0, Cl_All-1, 1);
		ym2Clock = RangeAdd(ym2Clock, 0, 0, ClockVars(pgClock), 1);
	}else
	//  adjust time  ---
	if (kRight && pgClock == Cl_Adjust)
	{
		unsigned long tm = rtc_get(), td = 0;
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

			case 6:
				par.rtcCompensate += a;
				rtc_compensate(par.rtcCompensate);  break;
			}

			if (td)
			if (tm + td >= 0)
			{	tm += td;
				if (tm_on + td >= 0)
					tm_on += td;
				rtc_set(tm);
			}
		}
	}
	//  graphs cursor move  ---
	else if (pgClock == Cl_Graphs)
	{
		if (kEnd)
			xCur = W-1;  // to end
		if (kMul)
		{	if (xCur == W)  xCur = W-1;  // show
			else  xCur = W;  // hide
		}
		int a = kRight * (kCtrl ? 30 : kSh ? 1 : 10);
		if (a)
			xCur = RangeAdd(xCur, a, 0, W-1, 1);
	}
	if (kAdd || kBckSp)  --mlevel;
}
