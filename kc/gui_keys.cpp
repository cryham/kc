#include "gui.h"
#include "matrix.h"
#include "kbd_layout.h"


//  todo key auto repeat,  row, col, ms
int8_t Gui::kr(int8_t c, int8_t r, uint16_t dt)
{
	int i = c * NumCols + r;
	KeyPosition st = Matrix_scanArray[i].state;
	int16_t& m = Matrix_autoRepeat[i];

	if (st == KeyState_Press)
	{
		m = -1;  // start delay
		return 1;
	}
	else if (st == KeyState_Hold)
	{
		if (m < 0)
		{	m -= dt;
			if (m < -250)  // delay ms
			{	m = 0;  return 1;  }
		}else
		{	m += dt;
			if (m > 100)  // repeat freq
			{	m = 0;  return 1;  }
		}
	}
	return 0;
}

//  Key press
//....................................................................................
void Gui::KeyPress()
{
	uint32_t ti = millis();
	uint16_t dt = ti - oldti_kr;
	oldti_kr = ti;

#if 1
	int8_t right = kr(1,1,dt) - kr(1,2,dt),  // ->  <-
		up = kr(2,4,dt) - kr(0,4,dt),    // Dn  Up
		pgup = kr(2,1,dt) - kr(0,1,dt);  // PgDn  PgUp
#else
	int8_t right = Key(1,1) - Key(1,2),  // ->  <-
		up = Key(2,4) - Key(0,4),    // Dn  Up
		pgup = Key(2,1) - Key(0,1);  // PgDn  PgUp
		//f12 = Key(5,0) - Key(3,0);  // F12  F11
		//ent = Key(4,0) - Key(2,0);  // Ent  \|
		//end = Key(2,2) - Key(0,2);  // End  Hom
#endif

	//  mapping  press key ..
	if (pressKey)
	{
		int c = 0, ii = -1;
		for (uint i=0; i < MaxKeys; ++i)
		{	if (Matrix_scanArray[i].state != KeyState_Off)  ++c;
			if (Matrix_scanArray[i].state == KeyState_Press)  ii = i;
		}
		if (pressKey == 1)
		{	// wait until all keys are off
			if (c == 0)
				pressKey = 2;
		}
		else if (pressKey == 2 && ii != -1)
		{	// pick
			pressKey = 0;  scId = ii;
		}
		return;
	}

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

	//  mapping edit  ----
	if (mlevel == 1 && ym == M_Mapping)
	{
		// todo kc keyboard mapping ...
		#define ADD(v,a,ma,mi)  {  v+=a;  if (v >= ma)  v = mi;  if (v < mi)  v = ma-1;  }

		if (up)  // menu up,dn
			ADD(ym1[ym], up, YM1[ym], 0)

		if (yy == 0 && right > 0)
			pressKey = 1;  // pick..
		else
		if (pgup)  // adj val
		{
			switch (yy)
			{
			case 1: ADD(nLay, pgup, 8,0);  break;
			case 2: ADD(scId, pgup, int16_t(MaxKeys),-1);  break;
			case 3: ADD(drawId, pgup, numKeys,-1);  break;
			case 4: ADD(keyCode, pgup, 255,0);  break;
			}
			// update
			//keyCode = -1;
		}

	/*	TODO BIG mapping modes: ..
			*press key to pick
			*browse all list (+by draw id, -by scan) up,dn,x4,16,
			=move [] cursor to pick from layout
			-pick
			..auto repeat keys
		display:
			layer change, or all
		layout keys colored by:
			1 bound cnt /or not, 2 sequence id, 3 layer use
		status page:
			cnt all mapped, unmapped scan codes, duplicates
	*/
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
