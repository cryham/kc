#include "gui.h"
#include "matrix.h"
#include "kbd_layout.h"
#include "keys_usb.h"

extern KC_Main kc;


//  Key press
//....................................................................................
void Gui::KeyPress()
{
	uint32_t ti = millis();
	uint16_t dt = ti - oldti_kr;
	oldti_kr = ti;

	//  update keys press
	kRight= kr(1,1,dt) - kr(1,2,dt);  // ->  <-
	kUp   = kr(2,4,dt) - kr(0,4,dt);  // Dn  Up
	kPgUp = kr(2,1,dt) - kr(0,1,dt);  // PgDn  PgUp
	kEnd  = kr(2,2,dt) - kr(0,2,dt);  // End  Hom

	kBack = Key(0,6);  kEnt = Key(4,0);  // Add+  Ent
	kCtrl = KeyH(3,4); kSh  = 0;  // Ins
	kInf  = Key(4,1);  kFps = Key(5,1);  // Mul*  Sub-

	//kF12 = Key(5,0) - Key(3,0);  // F12  F11  //Key(2,0);  \|

	/*	    0    1     2    3  4    5  6    7
		0        PgUp  Hom  U  ^    R  +    E
		1   Bck  ->    <-   Y  Del  T       F3
		2   \    PgDn  End  J  v    F  Ent  D
		3   F11  Del.  Spc  H  Ins  G  Up   F4
		4   Ent  *     Num  M  /    V       C
		5   F12  -          N       B			*/

	//  Mapping  edit modes
	//............................................
	if (ym == M_Mapping && mlevel == 1)
	{
		if (pressKey)  // press key
		{
			int c = 0, ii = -1;
			for (uint i=0; i < ScanKeys; ++i)
			{	if (Matrix_scanArray[i].state != KeyState_Off)  ++c;
				if (Matrix_scanArray[i].state == KeyState_Press)  ii = i;
			}
			if (pressKey == 1)
			{	//  wait until all keys are off
				if (c == 0)
					pressKey = 2;
			}
			else if (pressKey == 2 && ii != -1)
			{	//  pick
				pressKey = 0;  scId = ii;
				//  find draw key for scId
				for (int i=0; i < nDrawKeys; ++i)
					if (drawKeys[i].sc == scId)
					{	drawId = i;  break;  }
			}
			return;
		}
		if (moveCur)  // move cursor
		{
			if (kBack)
			{	//  set if key exists in matrix
				uint8_t sc = drawKeys[drawId].sc;
				if (sc < ScanKeys && sc != NO)
					scId = sc;
				moveCur = 0;  return;
			}

			if (kRight)	drawId += kRight;
			if (kUp)	drawId += kUp *  // y meh-
				(drawId < 40 ? 14 : drawId < 53 ? 13 : /*drawId < 57 ? */12 /*: 11*/);

			if (drawId >= nDrawKeys) drawId -= nDrawKeys;
			if (drawId < 0)			 drawId += nDrawKeys;
			return;
		}
		if (pickCode)  // pick code
		{
			if (kBack)
			{	//  apply in kc
				if (scId >= 0 && scId < kc.set.nkeys())
				{
					KC_Key& k = kc.set.keys[scId];
					//  if has no layer, add
					if (k.data.empty())
						k.data.push_back(keyCode);
					else
						k.data[0] = keyCode;
				}
				pickCode = 0;  return;
			}
			if (kFps)
			{	pickCode = 0;  return;  }

			if (kRight) { // <- ->
			if (grpFilt)
				keyGroup += kRight;
			else
				keyCode += kRight * 12;
			}
			if (kUp)	keyCode += kUp;  else
			if (kPgUp)	keyCode += kPgUp * 4;  else

			if (kEnd < 0)  // home filter
				grpFilt = 1-grpFilt;

			//  range
			if (keyCode < 0)			  keyCode += KEYS_ALL_EXT;
			if (keyCode >= KEYS_ALL_EXT)  keyCode -= KEYS_ALL_EXT;
			if (keyGroup < 0)		 keyGroup = grpMax-1;
			if (keyGroup >= grpMax)	 keyGroup = 0;

			if (grpFilt)
				if (cKeyGrp[keyCode] != keyGroup)
					keyCode = kc.grpStart[keyGroup];
			return;
		}
	}
	//............................................

	//  Add+  <back global
	if (kBack && mlevel > 0)
		--mlevel;

	if (mlevel == 0)  //  main
	{
		if (kUp){  ym += kUp;  if (ym >= M_All)  ym = 0;  if (ym < 0)  ym = M_All-1;  }
		if (kRight > 0)  mlevel = 1;  // enter>
		return;
	}

	//  mapping edit  ----
	if (mlevel == 1 && ym == M_Mapping)
	{
		#define ADD(v,a,ma,mi)  {  v+=a;  if (v >= ma)  v = mi;  if (v < mi)  v = ma-1;  }

		if (kUp)  // menu up,dn
			ADD(ym1[ym], kUp, YM1[ym], 0)

		if (kRight > 0)
		{	//  enter modes
			if (yy == 0)  pressKey = 1;  else
			if (yy == 1)  moveCur = 1;  else
			if (yy == 3)  pickCode = 1;
		}else
		if (kPgUp && yy == 2)  // adj val
			ADD(nLay, kPgUp, 8,0)
		return;
	}
	/*	BIG mapping ..
		display:
			layer change, or all
		layout keys colored by:
			1 bound cnt /or not, 2 sequence id, 3 layer use
		status page:
			cnt all mapped, unmapped scan codes, duplicates
	*/


	if (mlevel == 1)  //  sub menu
	{	//  navigate
		if (kRight < 0)	mlevel = 0;  // <back
		if (kRight > 0)	mlevel = 2;  // enter>
		if (kUp){  ym1[ym] += kUp;  Chk_y1();  }
		return;
	}

	#ifdef DEMOS
	if (mlevel == 2 && ym == M_Demos)
	{
		demos.KeyPress((EDemo)ym1[ym], this);
		return;
	}
	#endif
}
//....................................................................................



//  utils
void Gui::Chk_y1()
{
	if (ym1[ym] >= YM1[ym])  ym1[ym] = 0;
	if (ym1[ym] < 0)  ym1[ym] = YM1[ym]-1;
}

//  key auto repeat,  row, col, ms
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
			if (m < -krDelay)  // delay ms
			{	m = 0;  return 1;  }
		}else
		{	m += dt;
			if (m > krRepeat)  // repeat freq
			{	m = 0;  return 1;  }
		}
	}
	return 0;
}
