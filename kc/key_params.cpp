#include "gui.h"
#include "matrix.h"
#include "kc_params.h"
#include "kc_data.h"
#include "periodic.h"

const uint8_t Gui::DispPages[Di_All] = {2,2,1,3,2};
const uint8_t Gui::ScanPages[S_All] = {3,1,4,2};


//  Info
//....................................................................................

void Gui::KeysParInfo(int sp)
{
	switch (yy)
	{
	case I_Use:
		par.verCounter = RangeAdd(par.verCounter, kRight * sp, 0, 255, 1);  break;
	}
}

//  Setup
//....................................................................................
void Gui::KeysParSetup(int sp)
{
	int16_t ysp = ScanPages[yy];
	switch (yy)
	{
	case S_Scan:
		if (kUp)
		{	ym2Scan = RangeAdd(ym2Scan, kUp, 0, ysp, 1);  }
		else if (kRight)
		switch (ym2Scan)
		{
		case 0:
			par.scanFreq = RangeAdd(par.scanFreq, -kRight * (kSh ? 1 : 4), 2, 150);
			Periodic_init( par.scanFreq * 1000 );  break;  // upd
		case 1:
			par.strobe_delay = RangeAdd(par.strobe_delay, kRight, 0, 50);  break;
		case 2:
			par.debounce = RangeAdd(par.debounce, kRight, 0, 50);  break;
		}	break;
	}
	if (kSave)  Save();
	if (kLoad)  Load(kCtrl);
}


//  Display
//....................................................................................
void Gui::KeysParDisplay(int sp)
{
	if (kUp)  // y
	{	ym2Disp = RangeAdd(ym2Disp, kUp, 0, DispPages[pgDisp], 1);  }
	else
	if (kPgUp)  // pg
	{	pgDisp = RangeAdd(pgDisp, kPgUp, 0, Di_All-1, 1);
		ym2Disp = RangeAdd(ym2Disp, 0, 0, DispPages[pgDisp], 1);
	}else
	if (kRight)  // adjust values
	switch (pgDisp)
	{
	case Di_Bright:
		switch (ym2Disp)
		{
		case 0:
			par.brightness = RangeAdd(par.brightness, kRight * sp, 0, 100);
			kc.setDac = 1;  break;
		case 1:
			par.brightOff = RangeAdd(par.brightOff, kRight * sp, 0, 100);  break;
		case 2:
			par.startScreen = RangeAdd(par.startScreen, kRight, 0, ST_ALL-1);  break;
		}	break;

	case Di_Key:
		switch (ym2Disp)
		{
		case 0:
			par.krDelay = RangeAdd(par.krDelay, kRight, 0,255);  break;
		case 1:
			par.krRepeat = RangeAdd(par.krRepeat, kRight, 0,255);  break;
		}	break;

	case Di_Stats:
		switch (ym2Disp)
		{
		case 0:
			par.time1min = RangeAdd(par.time1min, kRight * sp/2, 0,255);  break;
		case 1:
			par.minInactive = RangeAdd(par.minInactive, kRight *sp/2, 0, 60, 1);  break;
		}	break;

	case Di_Graph:
		switch (ym2Disp)
		{
		case 0:
			par.timeTemp = RangeAdd(par.timeTemp, kRight * sp/2, 0, gIntvMask, 1);  break;
		case 1:
			par.timeTgraph = RangeAdd(par.timeTgraph, kRight * sp/2, 0, gIntvMask, 1);  break;
		case 2:
			par.minTemp = RangeAdd(par.minTemp, kRight * sp/2, 0, 40, 1);  break;
		case 3:
			par.maxTemp = RangeAdd(par.maxTemp, kRight * sp/2, 0, 40, 1);  break;
		}	break;

	case Di_Debug:
		switch (ym2Disp)
		{
		case 0:  // ram info
			iRam = RangeAdd(iRam, kRight, 0, 2);  break;
		case 1:  // fps
			demos.iFps = RangeAdd(demos.iFps, kRight, 0, 2);  break;
		case 2:
			par.tempOfs = RangeAdd(par.tempOfs, kRight * sp, -128, 127, 1);  break;
		}	break;
	}
	if (kAdd || kBckSp)  --mlevel;

	if (kSave)  Save();
	if (kLoad)  Load(kCtrl);
}
