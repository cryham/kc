#include "gui.h"
#include "Ada4_ST7735.h"
#include "FreeSans9pt7b.h"


//  menu draw  util
//------------------------------------------------------
const int8_t Cl = 4,  // 0 main  1 demos  2 test
	Rclr[Cl] = {16,27,31,10}, Gclr[Cl] = {26,26,23,10}, Bclr[Cl] = {31,31,31,10},
	Rmul[Cl] = {3, 2, 3, 1 }, Gmul[Cl] = {2, 3, 5, 1 }, Bmul[Cl] = {0, 1, 2, 1 };

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
		d->setTextColor(RGB(
			max(0, Rclr[q] - c * Rmul[q]),
			max(0, Gclr[q] - c * Gmul[q]),
			max(0, Bclr[q] - c * Bmul[q]) ));
		d->print(str[i]);

		//  next, extras
		y += yadd;

		if (i == numGap)  y += 4;

		if (i == nextCol)
		{	x += xw;  y = y1;  }
	}
}


//  Draw  main
//....................................................................................
void Gui::Draw()
{
	yy = ym2[ym];

	//  Clear  if not rain
	if (ym != M_Demos || mlevel != 2 || yy != D_Rain)
		d->clear();

	d->setFont(&FreeSans9pt7b);
	d->setCursor(0,0);


	//  Main menu
	//------------------------------------------------------
	if (mlevel==0)
	{
		d->setTextColor(RGB(27,24,31));
		d->print("Main Menu");  d->setFont(0);

		DrawMenu(M_All,strMain,0, 10, -1,2);

		//d->setCursor(W-1 -7*6, H-8);
		//d->print("F1 Help");
		return;
	}
	d->setTextColor(RGB(12,22,31));


	//  Demos
	//------------------------------------------------------
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

	//  Testing, Mappings,  kbd
	//------------------------------------------------------
	if (ym == M_Testing)
	{
		DrawTesting();  return;
	}

	if (ym == M_Mappings)
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

		d->setCursor(0, 32);
		d->setTextColor(RGB(10,28,18));

		sprintf(a,"%2d:%02d:%02d", h,m,s);
		d->print(a);
	}
}


//  Draw End
//....................................................................................
void Gui::DrawEnd()
{

	//  fps  ---------
	if (mlevel == 2 && ym == M_Demos)
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

	//if (!offDisp)
		d->display();  // 58 Fps, 15ms @144MHz
}
