#include "gui.h"
#include "matrix.h"


//  Key press
//....................................................................................
void Gui::KeyPress()
{
	int8_t right = Key(1,1) - Key(1,2),  // ->  <-
		up = Key(2,4) - Key(0,4);    // Dn  Up
		//pgup = Key(2,1) - Key(0,1);  // PgDn  PgUp
		//f12 = Key(5,0) - Key(3,0);  // F12  F11
		//ent = Key(4,0) - Key(2,0);  // Ent  \|
		//end = Key(2,2) - Key(0,2);  // End  Hom

/*	    0    1     2    3  4    5  6    7
	0        PgUp  Hom  U  ^    R  +    E
	1   Bck  ->    <-   Y  Del  T       F3
	2   \    PgDn  End  J  v    F  Ent  D
	3   F11  Del.  Spc  H  Ins  G  Up   F4
	4   Ent  *     Num  M  /    V       C
	5   F12  -          N       B			*/

	if (Key(0,6))  // Add+  <back global
		mlevel = max(0, mlevel-1);

	if (mlevel == 0)  //  main
	{
		if (up){  ym += up;  if (ym >= M_All)  ym = 0;  if (ym < 0)  ym = M_All-1;  }
		if (right > 0)  mlevel = 1;  // enter>
		return;
	}

	if (mlevel == 1 && ym == M_Mapping)
	{	// kc todo ..
		if (up < 0) {  kc.data.push_back(random(100));  }
		if (up > 0) {  if (!kc.data.empty())  kc.data.pop_back();  }
		return;
	}/**/

	if (mlevel == 1)  //  sub menu
	{	//  navigate
		if (right < 0)	mlevel = 0;  // <back
		if (right > 0)	mlevel = 2;  // enter>
		if (up){  ym1[ym] += up;  Chk_y1();  }
		return;
	}

	if (mlevel == 2 && ym == M_Testing)
	{
	}

	#ifdef DEMOS
	if (mlevel == 2 && ym == M_Demos)
	{
		//if (pgup){  ym1[ym] += pgup;  Chk_y1();  einit demos.Init(); mthr-  }

		demos.KeyPress((EDemo)ym1[ym], right, -up,
		//  Ins Ctrl  Sub- inf  Mul* fps
			Key(3,4), Key(5,1), Key(4,1));
		return;
	}
	#endif
}

void Gui::Chk_y1()
{
	if (ym1[ym] >= YM1[ym])  ym1[ym] = 0;
	if (ym1[ym] < 0)  ym1[ym] = YM1[ym]-1;
}
