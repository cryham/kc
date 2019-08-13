#include "gui.h"
#include "matrix.h"
#include "kbd_layout.h"
#include "keys_usb.h"
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
	case S_Layer:
		if (kUp)
		{	ym2Lay  = RangeAdd(ym2Lay , kUp, 0, ysp, 1);  }
		else if (kRight)
		switch (ym2Lay)
		{
		case 0:
			par.defLayer = RangeAdd(par.defLayer, kRight, 0, KC_MaxLayers-1);  break;
		case 1:
			par.editLayer = RangeAdd(par.editLayer, kRight, 0, KC_MaxLayers-1);  break;
		case 2:
			par.msLLTapMax = RangeAdd(par.msLLTapMax, kRight, 0, 250);  break;
		case 3:
			par.msLLHoldMin = RangeAdd(par.msLLHoldMin, kRight, 0, 90);  break;
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
		{	ym2Keyb  = RangeAdd(ym2Keyb , kUp, 0, ysp, 1);  }
		else if (kRight)
		switch (ym2Keyb)
		{
		case 0:
			par.dtSeqDef = RangeAdd(par.dtSeqDef, kRight * sp, 0, 250);  break;
		case 1:
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
		{	ym2Mouse = RangeAdd(ym2Mouse, kUp, 0, ysp, 1);  }
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
		case 2:
			par.quickKeys = RangeAdd(par.quickKeys, kRight, 0, 1);  break;
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
