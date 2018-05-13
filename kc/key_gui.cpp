#include "gui.h"
#include "matrix.h"
#include "kbd_layout.h"
#include "keys_usb.h"
#include "kc_params.h"
#include "kc_data.h"
#include "periodic.h"


//  Key press
//....................................................................................
void Gui::KeyPress()
{
	if (Key(par.keyGui))  // toggle Gui **
	{	kbdSend = 1 - kbdSend;  kc.setDac = 1;  }

	if (kbdSend)
		return;

	uint32_t ti = millis();
	uint16_t dt = ti - oldti_kr;
	oldti_kr = ti;

	//  update keys press  _k_
	kRight= kr(gRight,dt) - kr(gLeft,dt);
	kUp   = kr(gDown,dt) - kr(gUp,dt);  kDnH = KeyH(gDown);
	kPgUp = kr(gPgDn,dt) - kr(gPgUp,dt);
	kEnd  = kr(gEnd,dt) - kr(gHome,dt);

	kAdd = Key(gAdd);  kEnt = Key(gEnt);  kEnt2 = Key(gEnt2);
	kCtrl = KeyH(gCtrl); kSh  = KeyH(gSh);  kEsc = Key(gEsc);
	kMul  = Key(gMul);  kSub = Key(gSub);  kDiv = Key(gDiv);
	//  edit seq
	kIns = Key(gIns);   kDel = kr(gDel,dt);  kBckSp = kr(gBckSp,dt);
	kCopy = Key(gC);  kPaste = Key(gV);  kSwap = Key(gX);
	kLoad = Key(gLoad);  kSave = Key(gSave);

	kF1=Key(gF1); kF2=Key(gF2); kF3=Key(gF3); kF6=Key(gF6); kF7=Key(gF7);
	kF8=Key(gF8); kF9=Key(gF9); kF10=Key(gF10); kF11=Key(gF11); kF12=Key(gF12);


	//  quick access keys
	if (par.quickKeys && !(pressGui || pressKey ||
		(edit && mlevel == 1 && ym == M_Sequences) ||
		(mlevel == 2 && ym == M_Testing)))
	{
		if (kEsc)  SetScreen(ST_Main0);
		if (kF1)  SetScreen(ST_Map);
		if (kF2)  SetScreen(ST_Seqs);
		//if (kF3)  SetScreen(ST_Test);
		if (kF6)  SetScreen(ST_Test2+T_Pressed);
		if (kF7)  SetScreen(ST_Test2+T_Layout);
		if (kF8)  SetScreen(ST_Setup2+S_Mouse);
		if (kF9)  SetScreen(ST_Displ);
		if (kF10)  SetScreen(ST_Clock);
		if (kF11)  SetScreen(ST_Help);
		if (kF12)  SetScreen(ST_Demos2+D_Plasma);
	}

	//  Game  ------
	#ifdef GAME
	if (ym == M_Game && mlevel == 1)
	{
		if (game.KeyPress(mlevel))
		{	// goto help
			ym = M_Help;  mlevel = 1;  hpage = 8;
		}
		return;
	}
	#endif

	int sp = (kCtrl ? 10 : kSh ? 1 : 2);  // mul


	//  Setup
	if (ym == M_Setup && mlevel == 2)
	{
		KeysParSetup(sp);
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


	//  Mapping kbd
	if (ym == M_Mapping && mlevel == 1)
	{
		KeysMap();  return;
	}

	//  Sequences
	if (ym == M_Sequences && mlevel == 1)
	{
		KeysSeq();  return;
	}


	//  Add+  <back global
	if ((kAdd || kBckSp) && mlevel > 0)  --mlevel;


	if (mlevel == 0)  //  main menu
	{
		if (kUp){  ym += kUp;  if (ym >= M_All)  ym = 0;  if (ym < 0)  ym = M_All-1;  }
		if (kRight > 0 || kEnt2)  mlevel = 1;  // enter>
		return;
	}


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
		if (kRight < 0 || kBckSp)  mlevel = 0;  // <back
		if (kRight > 0 || kEnt2)
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
