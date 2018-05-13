#include "gui.h"
#include "matrix.h"
#include "kbd_layout.h"
#include "keys_usb.h"
#include "kc_params.h"
#include "kc_data.h"
#include "periodic.h"

const uint8_t Gui::DispPages[Gui::Disp_All] = {2,4,5};
const uint8_t Gui::ScanPages[S_All-1] = {2,3,4};


//  Key press
//....................................................................................
void Gui::KeysParSetup(int sp)
{
	switch (yy)
	{
	case S_Scan:
		if (kUp)
		{	ym2Scan = RangeAdd(ym2Scan, kUp, 0, ScanPages[yy], 1);  }
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

	case S_Keyboard:
		if (pressGui)
		{
			int ii = PressKey(pressGui);
			if (ii != -1)
			switch (ym2Keyb)
			{
			case 3:  par.keyGui = ii;  pressGui = 0;  break;
			}
		}
		else if (kUp)
		{	ym2Keyb  = RangeAdd(ym2Keyb , kUp, 0, ScanPages[yy], 1);  }
		else if (kRight)
		switch (ym2Keyb)
		{
		case 0:
			par.dtSeqDef = RangeAdd(par.dtSeqDef, kRight * sp, 0, 250);  break;
		case 1:
			par.defLayer = RangeAdd(par.defLayer, kRight, 0, KC_MaxLayers-1);  break;
		case 2:
			par.editLayer = RangeAdd(par.editLayer, kRight, 0, KC_MaxLayers-1);  break;
		case 3:
			pressGui = 1;  break;
		}	break;

	case S_Mouse:
		if (pressGui)
		{
			int ii = PressKey(pressGui);
			if (ii != -1)
			switch (ym2Mouse)
			{
			case 2:  par.keyMouseSlow = ii;  pressGui = 0;  break;
			}
		}
		else if (kUp)
		{	ym2Mouse = RangeAdd(ym2Mouse, kUp, 0, ScanPages[yy], 1);  }
		else if (kRight)
		switch (ym2Mouse)
		{
		case 0:
			par.mkSpeed = RangeAdd(par.mkSpeed, kRight * sp, 0, 250);  break;
		case 1:
			par.mkAccel = RangeAdd(par.mkAccel, kRight * sp, 0, 250);  break;
		case 2:
			pressGui = 1;  break;
		case 3:
			par.mkWhSpeed = RangeAdd(par.mkWhSpeed, kRight * sp, 0, 250);  break;
		case 4:
			par.mkWhAccel = RangeAdd(par.mkWhAccel, kRight * sp, 0, 250);  break;
		}	break;

	//case S_Version:
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
	{	pgDisp = RangeAdd(pgDisp, kPgUp, 0, Disp_All-1, 1);
		ym2Disp = RangeAdd(ym2Disp, 0, 0, DispPages[pgDisp], 1);
	}else
	if (kRight)  // adjust values
	switch (pgDisp)
	{
	case 0:
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

	case 1:
		switch (ym2Disp)
		{
		case 0:
			par.krDelay = RangeAdd(par.krDelay, kRight, 0,255);  break;
		case 1:
			par.krRepeat = RangeAdd(par.krRepeat, kRight, 0,255);  break;
		case 2:  // ram info
			iRam = RangeAdd(iRam, kRight, 0, 2);  break;
		case 3:  // fps
			demos.iFps = RangeAdd(demos.iFps, kRight, 0, 2);  break;
		case 4:
			par.quickKeys = RangeAdd(par.quickKeys, kRight, 0, 2);  break;
		}	break;

	case 2:
	{	unsigned long tm = rtc_get(), td=0;
		//rtc_compensate(int adjust)
		int a = kRight * (kCtrl ? 10 : 1);
		if (a)
		{	switch (ym2Disp)
			{
			case 0:  td = a * 3600;  break;  // h
			case 1:  td = a * 60;  break;  // m
			case 2:  td = a * 1;  break;  // s
			case 3:  td = a * 3600*24;  break;  // day
			case 4:  td = a * 3600*24*30;  break;  // mth-
			case 5:  td = a * 3600*24*365;  break;  // yr
			}
			if (tm + td >= 0)
			{	tm += td;  if (tm_on + td >= 0)  tm_on += td;
				rtc_set(tm);  }
		}
	}
	}
	if (kAdd || kBckSp)  --mlevel;

	if (kSave)  Save();
	if (kLoad)  Load(kCtrl);
}
