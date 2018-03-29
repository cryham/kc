#include "gui.h"


//  Key press
//....................................................................................
void Gui::KeyPress(int8_t right, int8_t up, int8_t pgup, int8_t esc)
{
//	if (!menu)  return;
/*
	//  global  --------
	if (key(F1) || key(SCROLL_LOCK))
	{	help = 1-help;  if (help)  status = 0;  }  // H

	if (key(F2) || key(PAUSE))
	{	status = 1-status;  if (status)  help = 0;  }  // L

	if (help || status)  // <back
	if (key(LEFT) || key(ESC))
	{	help = 0;  status = 0;  }
*/
	if (esc > 0)
		mlevel = max(0, mlevel-1);  // <back global

	if (mlevel == 0)  //  main
	{
		if (up < 0){  ++ym;  if (ym >= M_All)  ym = 0;  }
		if (up > 0){  --ym;  if (ym < 0)  ym = M_All-1;  }
		
		if (right > 0)  mlevel = 1;  // enter>
		return;
	}
	if (mlevel == 1)
	{
		if (right < 0)	mlevel = 0;  // <back
		if (right > 0)	mlevel = 2;  // enter>

		//  navigate
		if (up < 0){  ++ym2[ym];  if (ym2[ym] >= YM2[ym])  ym2[ym] = 0;  }
		if (up > 0){  --ym2[ym];  if (ym2[ym] < 0)  ym2[ym] = YM2[ym]-1;  }
		return;
	}

	if (mlevel == 2 && ym == M_Demos)
	{
		demos.KeyPress((EDemo)ym2[ym], right, up, pgup, 0);  //Key(3,4), Key(5,1));  // ins ct, - inf
		return;
	}

//	if (pgup)
//	{	demo += pgup;  if (demo < 0) demo = D_All-1;
//		if (demo >= D_All) demo = D_None;
//	}
}
