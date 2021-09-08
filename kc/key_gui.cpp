#include "gui.h"
#include "matrix.h"
#include "kc_params.h"
#include "kc_data.h"
#include "periodic.h"


//  Key press
//....................................................................................
void Gui::KeyPress()
{
	uint32_t ti = millis();
	uint16_t dt = ti - oldti_kr;
	oldti_kr = ti;

	//  update keys press  _k_
	//kRight= kr(0,dt) - kr(1,dt);
	kUp   = kr(3,dt) - kr(2,dt);
	/*kRight = Key(0);*/  kBckSp = kr(1,dt);
	
	//  scroll
	static int8_t old = KeyH(5);
	int8_t scr = KeyH(5);
	if (scr && !old)
		kRight = KeyH(4) > 0 ? -1 : 1;
	else
		kRight = Key(0);
	old = scr;

	//kPgUp = kr(gPgDn,dt) - kr(gPgUp,dt);
	//kEnd  = kr(gEnd,dt) - kr(gHome,dt);

	/*kEnt = Key(gEnt);  kBckSp = kr(gBckSp,dt);
	kCtrl = KeyH(gCtrl); kSh  = KeyH(gSh);
	kMul  = Key(gMul);  kSub = Key(gSub);  kDiv = Key(gDiv);
	kLoad = Key(gLoad);  kSave = Key(gSave);*/


	int sp = 2; //(kCtrl ? 10 : kSh ? 1 : 2);  // mul


	//  Setup
	if (ym == M_Setup && mlevel == 2)
	{
		KeysParSetup(sp);
	}

	//  Info
	if (ym == M_Info && mlevel == 2)
	{
		KeysParInfo(sp);
	}

	//  Display
	if (ym == M_Display && mlevel == 1)
	{
		KeysParDisplay(sp);  return;
	}

	//  Clock
	if (ym == M_Clock && mlevel == 1)
	{
		KeysClock();  return;
	}


	if (mlevel == 0)  //  main menu
	{
		if (kUp){  ym += kUp;  if (ym >= M_All)  ym = 0;  if (ym < 0)  ym = M_All-1;  }
		if (kRight > 0)  mlevel = 1;  // enter>
		return;
	}


	//  <back global
	if (kBckSp && mlevel > 0)  --mlevel;


	//  Help
	if (ym == M_Help && mlevel == 1)
	{
		if (kUp || kPgUp)
			hpage = RangeAdd(hpage, kUp+kPgUp, 0,HAll-1, 1);
		if (kRight < 0 || kBckSp)
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
	if (ym == M_Demos && mlevel == 2)
	{
		demos.KeyPress((EDemo)ym1[ym], this);
		return;
	}
	#endif
}
