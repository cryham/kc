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
	if (Key(gGui))
	{	kbdSend = 1 - kbdSend;  kc.setDac = 1;  }

	if (kbdSend)
		return;

	uint32_t ti = millis();
	uint16_t dt = ti - oldti_kr;
	oldti_kr = ti;

	//  update keys press
	kRight= kr(gRight,dt) - kr(gLeft,dt);
	kUp   = kr(gDown,dt) - kr(gUp,dt);
	kPgUp = kr(gPgUp,dt) - kr(gPgDn,dt);
	kEnd  = kr(gEnd,dt) - kr(gHome,dt);

	kBack = Key(gAdd);  kEnt = Key(gEnt);
	kCtrl = KeyH(gCtrl); kSh  = KeyH(gSh);
	kInf  = Key(gMul);  kFps = Key(gSub);
	//  edit seq
	kIns = Key(gIns);   kDel = kr(gDel,dt);  kBckSp = kr(gBckSp,dt);
	kCopy = Key(gC);  kPaste = Key(gV);  kSwap = Key(gX);
	kLoad = Key(gLoad);  kSave = Key(gSave);

	if (Key(gDiv))  // ram info
		iRam = (iRam + 1 + 3) % 3;


	#define RangeAdd(v,va, a,b) \
		if (va > 0){  if (v+va >= b)  v = b;  else  v += va; } \
		if (va < 0){  if (v+va <= a)  v = a;  else  v += va; }


	if (ym == M_Game)
		game.KeyPress(this, mlevel);


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
			RangeAdd(par.scanFreq, kRight*4, 2, 150);
			Periodic_init( par.scanFreq * 1000 );  break;  // upd
		case 1:
			RangeAdd(par.strobe_delay, kRight, 0, 50);  break;
		case 2:
			RangeAdd(par.debounce, kRight, 0, 50);  break;
		case 3:
			RangeAdd(par.dtSeqDef, kRight, 0, 250);  break;
		}
	}
	//  Testing  Mouse
	//..

	//  Display
	//..............................................
	if (ym == M_Display && mlevel == 1)
	{
		if (kUp)
		{	ym2Disp = (ym2Disp + kUp + 6) % 6;  }  // y max par
		else
		if (kRight)  // adjust values
		switch (ym2Disp)
		{
		case 0:
			RangeAdd(par.brightness, kRight * 2, 0, 100);
			kc.setDac = 1;  break;
		case 1:
			RangeAdd(par.fadeTime, kRight * 2, 0, 100);
			break;
		case 2:
			RangeAdd(par.brightOff, kRight * 2, 0, 100);
			break;

		case 3:
			RangeAdd(par.startScreen, kRight, 0, ST_ALL-1);  break;
		case 4:
			RangeAdd(par.krDelay, kRight, 0,255);  break;
		case 5:
			RangeAdd(par.krRepeat, kRight, 0,255);  break;
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
		KeysSeq();
		return;
	}


	//  Add+  <back global
	if (kBack && mlevel > 0)
		--mlevel;

	if (mlevel == 0)  //  main
	{
		if (kUp){  ym += kUp;  if (ym >= M_All)  ym = 0;  if (ym < 0)  ym = M_All-1;  }
		if (kRight > 0)  mlevel = 1;  // enter>
		return;
	}


	//  mapping edit  ----
	//............................................
	if (mlevel == 1 && ym == M_Mapping)
	{
		#define ADD(v,a,ma,mi)  {  v+=a;  if (v >= ma)  v = mi;  if (v < mi)  v = ma-1;  }

		if (kUp)  // menu up,dn
			ADD(ym1[ym], kUp, YM1[ym], 0)

		if (kRight > 0)
		{	//  enter modes
			if (yy == 3)  pickCode = 1;  else
			if (yy == 0)  pressKey = 1;  else
			if (yy == 1)  moveCur = 1;
		}
		if (kRight)  // chg lay
			if (yy == 2)  ADD(nLay, kRight, 8,0)
		return;
	}


	if (mlevel == 1)  //  sub menu
	{	//  navigate
		if (kRight < 0)	mlevel = 0;  // <back
		if (ym != M_Display)
		if (kRight > 0)	mlevel = 2;  // enter>
		if (kUp){  ym1[ym] += kUp;  Chk_y1();  }
		return;
	}

	#ifdef DEMOS
	if (mlevel == 2 && ym == M_Demos)
	{
		demos.KeyPress((EDemo)ym1[ym], this);
		return;
	}
	#endif
}
