#include "gui.h"
#include "matrix.h"
#include "kbd_layout.h"
#include "keys_usb.h"
#include "kc_params.h"
#include "periodic.h"

extern KC_Main kc;


//  Key press
//....................................................................................
void Gui::KeyPress()
{
	if (Key(gGui))  // toggle Gui
	{	kbdSend = 1 - kbdSend;  kc.setDac = 1;  }

	if (kbdSend)
		return;

	uint32_t ti = millis();
	uint16_t dt = ti - oldti_kr;
	oldti_kr = ti;

	//  update keys press
	kRight= kr(gRight,dt) - kr(gLeft,dt);
	kUp   = kr(gDown,dt) - kr(gUp,dt);  kDnH = KeyH(gDown);
	kPgUp = kr(gPgDn,dt) - kr(gPgUp,dt);
	kEnd  = kr(gEnd,dt) - kr(gHome,dt);

	kBack = Key(gAdd);  kEnt = Key(gEnt);
	kCtrl = KeyH(gCtrl); kSh  = KeyH(gSh);
	kMul  = Key(gMul);  kSub = Key(gSub);  kDiv = Key(gDiv);
	//  edit seq
	kIns = Key(gIns);   kDel = kr(gDel,dt);  kBckSp = kr(gBckSp,dt);
	kCopy = Key(gC);  kPaste = Key(gV);  kSwap = Key(gX);
	kLoad = Key(gLoad);  kSave = Key(gSave);


	//  Game  ------
	#ifdef GAME
	if (ym == M_Game && mlevel == 1)
	{
		if (game.KeyPress(mlevel))
		{	// goto help
			ym = M_Help;  mlevel = 1;  hpage = 5;
		}
		return;
	}
	#endif


	//  Testing  Scan Setup
	//..............................................
	if (ym == M_Testing && mlevel == 2 && yy == T_ScanSetup)
	{
		if (kUp)
		{	ym2Test = (ym2Test + kUp + 4) % 4;  }  // y max par
		else
		if (kRight)  // adjust values
		switch (ym2Test)
		{
		case 0:
			par.scanFreq = RangeAdd(par.scanFreq, -kRight * (kSh ? 1 : 4), 2, 150);
			Periodic_init( par.scanFreq * 1000 );  break;  // upd
		case 1:
			par.strobe_delay = RangeAdd(par.strobe_delay, kRight, 0, 50);  break;
		case 2:
			par.debounce = RangeAdd(par.debounce, kRight, 0, 50);  break;
		case 3:
			par.dtSeqDef = RangeAdd(par.dtSeqDef, kRight * (kCtrl ? 10 : 1), 0, 250);  break;
		}
	}
	//  Testing  Mouse
	//..

	//  Display
	//..............................................
	if (ym == M_Display && mlevel == 1)
	{
		if (kUp)  // y
		{	ym2Disp = RangeAdd(ym2Disp, kUp, 0, DispPages[pgDisp]-1, 1);  }
		else
		if (kPgUp)  // pg
		{	pgDisp = RangeAdd(pgDisp, kPgUp, 0, Disp_All-1, 1);
			ym2Disp = RangeAdd(ym2Disp, 0, 0, DispPages[pgDisp]-1, 1);
		}else
		if (kRight)  // adjust values
		switch (pgDisp)
		{
		case 0:
			switch (ym2Disp)
			{
			case 0:
				par.brightness = RangeAdd(par.brightness, kRight * (kCtrl ? 10 : 2), 0, 100);
				kc.setDac = 1;  break;
			case 1:
				par.brightOff = RangeAdd(par.brightOff, kRight * (kCtrl ? 10 : 2), 0, 100);  break;
			case 2:
				par.fadeTime = RangeAdd(par.fadeTime, kRight * 2, 0, 100);  break;
			case 3:
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
			}	break;
		}
		if (kBack)  --mlevel;
		return;
	}


	//  Mapping  edit modes
	//.......................
	if (ym == M_Mapping && mlevel == 1)
	{
		if (KeysMap())  return;
	}

	//  sequence list
	//.......................
	if (mlevel == 1 && ym == M_Sequences)
	{
		KeysSeq();  return;
	}


	//  Add+  <back global
	if (kBack && mlevel > 0)  --mlevel;

	if (mlevel == 0)  //  main menu
	{
		if (kUp){  ym += kUp;  if (ym >= M_All)  ym = 0;  if (ym < 0)  ym = M_All-1;  }
		if (kRight > 0)  mlevel = 1;  // enter>
		return;
	}


	//  mapping edit  ----
	//............................................
	if (mlevel == 1 && ym == M_Mapping)
	{
		if (kUp)  // menu up,dn
			ym1[ym] = RangeAdd(ym1[ym], kUp, 0,YM1[ym]-1, 1);

		if (kRight > 0)
		{	//  enter modes
			if (yy == 0)  pressKey = 1;  else
			if (yy == 1)  moveCur = 1;  else
			if (yy == 3)  pickCode = 1;
		}
		if (kPgUp || (yy == 2 && kRight))  // change layer
			nLay = RangeAdd(nLay, kPgUp+kRight, 0,KC_MaxLayers, 1);
			// nLay == KC_MaxLayers shows layer use vis

		//  erase key *
		if (kCtrl && kDel &&
			scId != NO && scId < kc.set.nkeys() && nLay < KC_MaxLayers)
			kc.set.key[nLay][scId] = KEY_NONE;

		//  quick access keys / * -
		if (kDiv)  pressKey = 1;
		if (kMul)  moveCur = 1;
		if (kSub)  pickCode = 1;

		if (kSave)  Save();
		if (kLoad)  Load(kCtrl);
		return;
	}


	//  Help
	if (mlevel == 1 && ym == M_Help)
	{
		if (kUp)
			hpage = RangeAdd(hpage, kUp, 0,HAll-1, 1);
		if (kRight < 0 || kBack)
			mlevel = 0;  // <back
		return;
	}


	if (mlevel == 1)  //  sub menu
	{
		//  navigate
		if (kRight < 0)  mlevel = 0;  // <back
		if (kRight > 0)
			if (ym != M_Display)  mlevel = 2;  // enter>

		if (kUp){  ym1[ym] += kUp;  Chk_y1();  }
		return;
	}


	//  Demos
	#ifdef DEMOS
	if (mlevel == 2 && ym == M_Demos)
	{
		demos.KeyPress((EDemo)ym1[ym], this);
		return;
	}
	#endif
}
