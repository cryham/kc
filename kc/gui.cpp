#include "gui.h"
#include "def.h"
#include "kbd_layout.h"
#include "kc_data.h"


//  Main  ----
Gui::Gui() : d(0)
{
	Init(0);
}

void Gui::Init(Ada4_ST7735* tft)
{
	d = tft;
	kbdSend = 0;  //-

	mlevel = 0;
	ym = 0;  yy = 0;
	for (int i=0; i < M_All; ++i)
		ym1[i]=0;


//	mlevel = 2;  //0
//	ym = M_Testing;
//	ym1[M_Testing] = T_Pressed;
//	SetScreen(ST_Main0);

	
	oldti=0;  oldti_kr=0;
	iRam = 0;  hpage = 0;

#ifdef DEMOS
	demos.Init(d);
#endif
#ifdef GAME
	game.Init(this);
#endif

	ym2Scan=0; ym2Keyb=0; ym2Mouse=0; pressGui=0;
	ym2Disp=0; pgDisp=0;

	nLay=0;  scId=NO;  drawId=54;  drawX=0; drawY=0;
	nLayCpy=0;  scIdCpy=0;  
	keyCode=0;  pressKey=0;  pickCode=0;

	edit = 0;  edins = 1;  copyId = -1;
	slot=0; page=0; edpos=0;
	tBlnk=0; tInfo=0;  infType=0;


	//  init Group Starts and Ends
	for (int g=0; g < grpMax; ++g)
	{
		grpStart[g] = 0;  // all
		grpEnd[g] = KEYS_ALL_EXT;

		//  find first and last key, for this group
		for (int i=0; i < KEYS_ALL_EXT; ++i)
			if (cKeyGrp[i] == g)
			{
				if (grpStart[g] == 0)  grpStart[g] = i;
				grpEnd[g] = i;
			}
	}
}

//  start screen  ---
void Gui::SetScreen(int8_t s)
{
	if (s == ST_Main0)
	{	mlevel = 0;  ym = 0;  return;  }
	else  mlevel = 1;

	switch (s)
	{
	case ST_Map:   ym = M_Mapping;  break;
	case ST_Seqs:  ym = M_Sequences;  break;
	case ST_Test:  ym = M_Testing;  break;

	case ST_Displ: ym = M_Display;  break;
	case ST_Setup: ym = M_Setup;  break;
	case ST_Help:  ym = M_Help;  break;
#ifdef DEMOS
	case ST_Demos: ym = M_Demos;  break;
	}
	if (s >= ST_Demos2)
	{	mlevel = 2;  ym = M_Demos;
		ym1[ym] = s - ST_Demos2;
	}
	else
#else
	}
#endif
	if (s >= ST_Setup2 && s < ST_Setup2Max)
	{	mlevel = 2;  ym = M_Setup;
		ym1[ym] = s - ST_Setup2;
	}else
	if (s >= ST_Test2 && s < ST_Test2Max)
	{	mlevel = 2;  ym = M_Testing;
		ym1[ym] = s - ST_Test2;
	}
}

const char* Gui::StrScreen(int8_t s)
{
	// todo main, pressed, layout, version, demos
	switch (s)
	{
	case ST_Main0:  return "Main";
	case ST_Map:    return strMain[M_Mapping];
	case ST_Seqs:   return strMain[M_Sequences];
	case ST_Test:   return strMain[M_Testing];

	case ST_Displ:  return strMain[M_Display];
	case ST_Setup:  return strMain[M_Setup];
	case ST_Help:   return strMain[M_Help];
#ifdef DEMOS
	case ST_Demos:  return strMain[M_Demos];
	}
	if (s >= ST_Demos2)
		return strDemo[s - ST_Demos2];
#else
	}
#endif
	if (s >= ST_Setup2 && s < ST_Setup2Max)
		return strSetup[s - ST_Setup2];
	if (s >= ST_Test2 && s < ST_Test2Max)
		return strTest[s - ST_Test2];
	return "";
}
