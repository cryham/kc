#include "gui.h"
#include "Ada4_ST7735.h"
#include "FreeSans9pt7b.h"

extern KC_Main kc;

//  menu draw  util
//------------------------------------------------------
const uint8_t  // 0 main  1 demos  2 test  3 seqs
	Gui::Mclr[Gui::Cl][2][3] = {
	{{16,26,31},{3,2,0}},
	{{27,26,31},{2,3,1}},
	{{31,25,31},{3,5,2}},
	{{20,31,25},{3,3,3}}};

void Gui::DrawMenu(int cnt, const char** str, int8_t q, // clrId
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
		FadeClr(&Mclr[q][0][0], &Mclr[q][1][0], 4, c, 1);
		d->print(str[i]);

		//  next, extras
		y += yadd;

		if (i == numGap)  y += 4;

		if (i == nextCol)
		{	x += xw;  y = y1;  }
	}
}

void Gui::FadeClr(const uint8_t* clrRGB, const uint8_t* mulRGB,
					const uint8_t minRGB, const uint8_t mul, const uint8_t div)
{
	d->setTextColor(RGB(
		max(minRGB, clrRGB[0] - mulRGB[0] * mul / div),
		max(minRGB, clrRGB[1] - mulRGB[1] * mul / div),
		max(minRGB, clrRGB[2] - mulRGB[2] * mul / div) ));
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

		DrawMenu(M_All,strMain,0, 10, -1,2);

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

			DrawMenu(D_All,strDemo,1, 10, D_Next);
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
		d->setTextColor(RGB(31,31,12));

		sprintf(a,"Brightness: %d  %d %%",
			kc.valDac-3595, 100*(kc.valDac-3600)/(4095-3600));
		//todo
		// fade time, fade to brightness
		// key auto repeat, delay
		d->print(a);
	}
}


//  Draw End
//....................................................................................
void Gui::DrawEnd()
{

	#ifdef DEMOS
	//  fps  ---------
	if (demos.iFps && mlevel == 2 && ym == M_Demos)
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
