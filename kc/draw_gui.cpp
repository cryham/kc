#include "gui.h"
#include "Ada4_ST7735.h"
#include "FreeSans9pt7b.h"


//  Draw  main
//....................................................................................
void Gui::Draw()
{
	yy = ym1[ym];

	//  Clear  if not rain
	#ifdef DEMOS
	if (ym != M_Demos || mlevel != 2 || yy != D_Rain)
	#endif
		d->clear();

	d->setFont(&FreeSans9pt7b);
	d->setCursor(0,0);


	//  Main menu
	//------------------------------------------------------
	if (mlevel==0)
	{
		d->setTextColor(RGB(6,19,31));
		d->print("Main Menu");  d->setFont(0);

		DrawMenu(M_All,strMain, C_Main,RGB(20,25,29), 10, -1,2);

		//d->setCursor(W-1 -7*6, H-8);
		//todo d->print("F1 Help");
		return;
	}
	d->setTextColor(RGB(12,22,31));


	//  Demos
	//------------------------------------------------------
	#ifdef DEMOS
	if (ym == M_Demos)
	{
		if (mlevel == 2)
		{
			d->setFont(0);

			uint32_t oti1 = millis();
			switch (yy)
			{
			#ifdef DEMOS_PLASMA
				case D_Plasma:   demos.Plasma();  break;  // clr 55, 40 ms
			#endif
				case D_Wave:     demos.Wave();  break;
				case D_Fire:     demos.Fire();  break;
				case D_CK_Logo:  demos.CK_logo();  break;  // 7
			#ifdef DEMOS_3D
				case D_Hedrons:  demos.Hedrons();  break;  // 5-9ms
			#endif
				case D_Ngons:    demos.Ngons();  break;  // 12 8ms 14 10ms
				case D_Fonts:	 demos.Fonts();  break;

			#ifdef DEMOS_OLD_PAR
				case D_Fountain: demos.Fountain();  break;
				case D_Balls:    demos.Balls();  break;
				case D_Space:    demos.Space();  break;
			#endif
				case D_Rain:     demos.Rain();  break;
			}
			tdemo = millis() - oti1;
		}else
		{	//  menu
			d->setTextColor(RGB(25,16,28));
			d->print(strMain[ym]);  d->setFont(0);

			DrawMenu(D_All,strDemo, C_Demos,RGB(27,24,30), 10, D_Next);
		}
		return;
	}
	#endif

	//  Testing, Mappings,  kbd
	//------------------------------------------------------
	if (ym == M_Testing)
	{
		DrawTesting();  return;
	}

	if (ym == M_Mapping)
	{
		DrawMapping();  return;
	}

	if (ym == M_Sequences)
	{
		DrawSequences();  return;
	}


	//  Display
	//------------------------------------------------------
	if (ym == M_Display)
	{
		char a[64];
		d->setTextColor(RGB(29,23,16));
		d->print(strMain[ym]);  //d->setFont(0);

		//  time  ---
		tm = rtc_get();
		if (tm)
		{	int h = tm/3600%24, m = tm/60%60, s = tm%60;

			d->setCursor(W/2, 0);
			d->setTextColor(RGB(10,28,18));

			sprintf(a,"%2d:%02d:%02d", h,m,s);
			d->print(a);
		}
		d->setFont(0);

		//  brightness, dac  ---
		d->setCursor(0, 32);
		for (int i=0; i < 6; ++i)
		{
			int c = abs(i - ym2Disp);  // dist dim
			if (!c)
			{	d->setTextColor(RGB(31,22,6));
				d->print("\x10 ");  // >
			}else
				d->print("  ");

			FadeClr(C_Disp, 4, c, 1);
			int8_t h = 4;
			switch(i)
			{
			case 0:
				sprintf(a,"Brightness: %d %%", par.brightness);  break;

			case 1:
				sprintf(a,"Fade time: %d", par.fadeTime);  h = 2;  break;
			case 2:
				sprintf(a,"Fade bright.: %d %%", par.brightOff);  break;

			case 3:
				sprintf(a,"Start scren: %s", StrScreen(par.startScreen));  break;

			case 4:
				sprintf(a,"Key delay:  %d ms", par.krDelay*5);  h = 2;  break;
			case 5:
				sprintf(a,"Key repeat: %d ms", par.krRepeat*5);  break;

			//uint8_t mkSpeed, mkAccel;  todo scroll
			//start demo time
			}
			d->println(a);  d->moveCursor(0,h);
		}
		return;
	}
}
