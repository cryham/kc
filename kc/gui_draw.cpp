#include "gui.h"
#include "Ada4_ST7735.h"
#include "FreeSans9pt7b.h"

extern KC_Main kc;

//  menu draw  util
//------------------------------------------------------
const uint8_t Gui::Mclr[Gui::Cl][2][3] = {
	{{16,26,31},{3,2,0}},  // 0 main
	{{27,26,31},{2,3,1}},  // 1 demos
	{{20,30,26},{6,3,4}},  //  2 test
	{{24,30,18},{6,3,6}},  //  3 map
	{{15,31,31},{4,5,4}},  //  4 seqs
	{{31,31,23},{1,2,4}},  // 5 display
	{{26,28,28},{4,3,3}},  // 6 scan, mouse-
};

void Gui::DrawMenu(int cnt, const char** str, EFadeClr ec,
					int16_t yadd, int16_t nextCol, int16_t numGap)
{
	const int16_t my = mlevel==0 ? ym : yy;
	const int16_t xw = 70, y1 = 32;  // par

	int16_t x=0, y=y1, c;
	for (int i=0; i < cnt; ++i)
	{
		d->setCursor(x,y);
		d->setTextColor(RGB(18,18,31));
		d->print(i == my ? " \x10 ":"   ");

		c = abs(i - my);  // dist dim
		FadeClr(ec, 4, c, 1);
		d->print(str[i]);

		//  next, extras
		y += yadd;

		if (i == numGap)  y += 4;

		if (i == nextCol)
		{	x += xw;  y = y1;  }
	}
}

void Gui::FadeClr(EFadeClr ec, const uint8_t minRGB, const uint8_t mul, const uint8_t div)
{
	const uint8_t* clr = &Mclr[ec][0][0];
	const uint8_t* cmu = &Mclr[ec][1][0];

	d->setTextColor(RGB(
		max(minRGB, clr[0] - cmu[0] * mul / div),
		max(minRGB, clr[1] - cmu[1] * mul / div),
		max(minRGB, clr[2] - cmu[2] * mul / div) ));
}
void Gui::FadeGrp(uint8_t g, const uint8_t minRGB, const uint8_t mul, const uint8_t div)
{
	const uint8_t* clr = &cGrpRgb[g][0][0];
	const uint8_t* cmu = &cGrpRgb[g][1][0];

	d->setTextColor(RGB(
		max(minRGB, clr[0] - cmu[0] * mul / div),
		max(minRGB, clr[1] - cmu[1] * mul / div),
		max(minRGB, clr[2] - cmu[2] * mul / div) ));
}

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
		d->setTextColor(RGB(25,22,30));
		d->print("Main Menu");  d->setFont(0);

		DrawMenu(M_All,strMain,C_Main, 10, -1,2);

		//d->setCursor(W-1 -7*6, H-8);
		//d->print("F1 Help");
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
				case D_Plasma:   demos.Plasma();  break;  // clr 55, 40 ms
				case D_Wave:     demos.Wave();  break;
				case D_Hedrons:  demos.Hedrons();  break;  // 5-9ms

				case D_Ngons:    demos.Ngons();  break;  // 12 8ms 14 10ms
				case D_CK_Logo:  demos.CK_logo();  break;  // 7
				case D_Fonts:	 demos.Fonts();  break;

				case D_Fountain: demos.Fountain();  break;
				case D_Balls:    demos.Balls();  break;
				case D_Space:    demos.Space();  break;
				case D_Rain:     demos.Rain();  break;
			}
			tdemo = millis() - oti1;
		}else
		{	//  menu
			d->print(strMain[ym]);  d->setFont(0);

			DrawMenu(D_All,strDemo,C_Demos, 10, D_Next);
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
		d->print(strMain[ym]);  //d->setFont(0);

		//  time  ---
		tm = rtc_get();
		int h = tm/3600%24, m = tm/60%60, s = tm%60;

		d->setCursor(W/2, 0);
		d->setTextColor(RGB(10,28,18));

		sprintf(a,"%2d:%02d:%02d", h,m,s);
		d->print(a);

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
			h = 4;
			switch(i)
			{
			case 0:
				sprintf(a,"Brightness: %d  %d %%",
					par.valDac-3595, 100*(par.valDac-3600)/(4095-3600));  break;
			case 1:
				sprintf(a,"Fade time: ");  h = 2;  break;
			case 2:
				sprintf(a,"Fade brightness: ");  break;

			case 3:
				sprintf(a,"Start scren: %d", par.startScreen);  break;
			//ck demo time, bright..

			case 4:
				sprintf(a,"Key delay:  %d ms", par.krDelay*5);  h = 2;  break;
			case 5:
				sprintf(a,"Key repeat: %d ms", par.krRepeat*5);  break;

			//int8_t startScreen
			//uint8_t mkSpeed, mkAccel;
			}
			d->println(a);  d->moveCursor(0,h);
		}
	}
}


//  Draw End
//....................................................................................
void Gui::DrawEnd()
{

	#ifdef DEMOS
	//  fps  ---------
	if (mlevel == 2)
	if ((demos.iFps && ym == M_Demos) ||
		(ym == M_Testing && yy == T_ScanSetup))
	{
		uint32_t ti = millis();
		float fr = 1000.f / (ti - oldti);
		int ff = fr;
		oldti = ti;

		d->setTextColor(RGB(24,28,31));
		d->setCursor(W-14,0);
		sprintf(a,"%d", ff);
		d->print(a);

		d->setTextColor(RGB(20,26,31));
		d->setCursor(W-14,9);
		sprintf(a,"%lu", tdemo);
		d->print(a);
	}
	#endif

	//if (!offDisp)
		d->display();  // 58 Fps, 15ms @144MHz
}
