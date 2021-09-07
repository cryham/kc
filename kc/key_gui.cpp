#include "gui.h"
#include "matrix.h"
#include "kc_params.h"
#include "kc_data.h"
#include "periodic.h"


//  scan codes for gui keys
const uint8_t gLoad=15,gSave=31,gDiv=36, gEsc=0,gAdd=6,gEnt=22,
	gRight=9,gLeft=10,gDown=20,gUp=4, gPgDn=17,gPgUp=1,gEnd=18,gHome=2,
	gCtrl=26,gSh=16,gMul=33,gSub=41, gIns=28,gDel=25, gBckSp=8,gSpc=26;


//  Key press
//....................................................................................
void Gui::KeyPress()
{
	uint32_t ti = millis();
	uint16_t dt = ti - oldti_kr;
	oldti_kr = ti;

	//  update keys press  _k_
	/*kRight= kr(gRight,dt) - kr(gLeft,dt);
	kUp   = kr(gDown,dt) - kr(gUp,dt);
	kPgUp = kr(gPgDn,dt) - kr(gPgUp,dt);
	kEnd  = kr(gEnd,dt) - kr(gHome,dt);

	kAdd = Key(gAdd);  kEnt = Key(gEnt);
	kCtrl = KeyH(gCtrl); kSh  = KeyH(gSh);
	kMul  = Key(gMul);  kSub = Key(gSub);  kDiv = Key(gDiv);
	//  edit seq
	kBckSp = kr(gBckSp,dt);
	kLoad = Key(gLoad);  kSave = Key(gSave);*/


	int sp = (kCtrl ? 10 : kSh ? 1 : 2);  // mul


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


	//  Add+  <back global
	if ((kAdd || kBckSp) && mlevel > 0)  --mlevel;


	//  Help
	if (ym == M_Help && mlevel == 1)
	{
		if (kUp || kPgUp)
			hpage = RangeAdd(hpage, kUp+kPgUp, 0,HAll-1, 1);
		if (kRight < 0 || kAdd || kBckSp)
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
