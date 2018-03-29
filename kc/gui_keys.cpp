#include "gui.h"


//  Key press
//....................................................................................
void Gui::KeyPress(int8_t right, int8_t up, int8_t pgup, int8_t back, int8_t inf)
{

	//  global  ---------
	if (back > 0)
		mlevel = max(0, mlevel-1);  // <back global


	//  level 0  ---------
	if (mlevel == 0)  //  main
	{
		if (up < 0){  ++ym;  if (ym >= M_All)  ym = 0;  }
		if (up > 0){  --ym;  if (ym < 0)  ym = M_All-1;  }
		
		if (right > 0)  mlevel = 1;  // enter>
		return;
	}


	//  level 1  ---------
	/*if (mlevel == 1 && ym == M_Mapping)
	{
		if (up > 0) {  kc.data.push_back(random(100));  }
		if (up < 0) {  if (!kc.data.empty())  kc.data.pop_back();  }
		return;
	}*/

	if (mlevel == 1)
	{
		if (right < 0)	mlevel = 0;  // <back
		if (right > 0)	mlevel = 2;  // enter>

		//  navigate
		if (up < 0){  ++ym1[ym];  if (ym1[ym] >= YM1[ym])  ym1[ym] = 0;  }
		if (up > 0){  --ym1[ym];  if (ym1[ym] < 0)  ym1[ym] = YM1[ym]-1;  }
		return;
	}


	//  level 2  ---------
	if (mlevel == 2 && ym == M_Testing)
	{

	}

	#ifdef DEMOS
	if (mlevel == 2 && ym == M_Demos)
	{
//		demo += pgup;  if (demo < 0) demo = D_All-1;
//			if (demo >= D_All) demo = D_None;
		demos.KeyPress((EDemo)ym1[ym], right, up, pgup, inf);  //Key(3,4), Key(5,1));  // ins ct, - inf
		return;
	}
	#endif

//	if (pgup)
//	{	demo += pgup;  if (demo < 0) demo = D_All-1;
//		if (demo >= D_All) demo = D_None;
//	}
}
