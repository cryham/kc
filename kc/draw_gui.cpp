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

		DrawMenu(M_All,strMain, C_Main,RGB(20,25,29),RGB(5,7,9), 10, M_Next, 2);
		return;
	}
	d->setTextColor(RGB(12,22,31));


	switch (ym)
	{
	//  Demos
	//------------------------------------------------------
	#ifdef DEMOS
	case M_Demos:
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

			DrawMenu(D_All,strDemo, C_Demos,RGB(27,27,30),RGB(6,6,9), 10, D_Next);
		}
		return;
	#endif

	#ifdef GAME   // game
	case M_Game:  game.Draw();  return;
	#endif

	//  Testing, Mappings,  kbd
	case M_Testing:   DrawTesting();  return;
	case M_Mapping:   DrawMapping();  return;
	case M_Sequences: DrawSequences();  return;

	//  Display, Help
	case M_Display: DrawDisplay();  return;
	case M_Help:    DrawHelp();  return;
	}
}
