#include "gui.h"
#include "matrix.h"
#include "kbd_layout.h"
#include "keys_usb.h"
#include "kc_params.h"
#include "periodic.h"

extern KC_Main kc;


//  Key press
//....................................................................................
void Gui::KeyPress()
{
	uint32_t ti = millis();
	uint16_t dt = ti - oldti_kr;
	oldti_kr = ti;

	//  update keys press
	kRight= kr(101,dt) - kr( 99,dt);  // ->  <-
	kUp   = kr( 64,dt) - kr( 82,dt);  // Dn  Up
	kPgUp = kr( 65,dt) - kr( 83,dt);  // PgDn  PgUp
	kEnd  = kr( 63,dt) - kr( 81,dt);  // End  Home

	kBack = Key( 84);  kEnt = Key( 66);  // Add+  Ent
	kCtrl = KeyH(128); kSh  = KeyH( 94);  //LCt LSh  -Ins  \|
	kInf  = Key( 47);  kFps = Key( 29);  // Mul*  Sub-
	//  edit seq
	kIns = Key( 10);   kDel = kr( 11,dt);  kBckSp = kr( 91,dt);
	kCopy = Key( 49);  kPaste = Key( 44);  kSwap = Key( 50);  // C V B
	kLoad = Key( 95);  kSave = Key(110);  //F5 F4  -F3  F4

	if (Key( 46))  // Div/
		iRam = (iRam + 1 + 3) % 3;

	//kF12 = Key(5,0) - Key(3,0);  // F12  F11

	// -- PgUp,Home,Up,R  test strobe delay

	/*	    0    1     2    3  4    5  6    7
		0        PgUp  Hom  U  ^    R  +    E
		1   Bck  ->    <-   Y  Del  T       F3
		2   \    PgDn  End  J  v    F  Ent  D
		3   F11  Del.  Spc  H  Ins  G  Up   F4
		4   Ent  *     Num  M  /    V       C
		5   F12  -          N       B			*/


	#define RangeAdd(v,va, a,b) \
		if (va > 0){  if (v+va >= b)  v = b;  else  v += va; } \
		if (va < 0){  if (v+va <= a)  v = a;  else  v += va;}


	//  Testing  Scan Setup
	//..............................................
	if (ym == M_Testing && mlevel == 2 && yy == T_ScanSetup)
	{
		if (kUp)
		{	ym2Test = (ym2Test + kUp + 4) % 4;  }  // y max par
		else
		if (kRight)  // adjust values
		switch (ym2Test)
		{
		case 0:
			RangeAdd(par.scanFreq, kRight*4, 2, 150);
			Periodic_init( par.scanFreq * 1000 );  break;  // upd
		case 1:
			RangeAdd(par.strobe_delay, kRight, 0, 50);  break;
		case 2:
			RangeAdd(par.debounce, kRight, 0, 50);  break;
		case 3:
			RangeAdd(par.dtSeqDef, kRight, 0, 250);  break;
		}
	}
	//  Testing  Mouse
	//..

	//  Display
	//..............................................
	if (ym == M_Display && mlevel == 1)
	{
		if (kUp)
		{	ym2Disp = (ym2Disp + kUp + 6) % 6;  }  // y max par
		else
		if (kRight)  // adjust values
		switch (ym2Disp)
		{
		case 0:
			RangeAdd(par.valDac, kRight * 10, 3600, 4095);
			kc.setDac = 1;  break;
		case 1:
			break;
		case 2:
			break;

		case 3:
			RangeAdd(par.startScreen, kRight, 0, 15);  break;
		case 4:
			RangeAdd(par.krDelay, kRight, 0,255);  break;
		case 5:
			RangeAdd(par.krRepeat, kRight, 0,255);  break;
		}

		if (kBack)  --mlevel;
		return;
	}


	//  Mapping  edit modes
	//...............................................................................
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

			//  find closest next in move direction
			int x=2, y=0, ii = -1, B = 2000,
				mxl = -B, mxr = B, myu = B, myd = B, axd = B, axu = B;

			for (int i = 0; i < nDrawKeys; ++i)
			{
				const DrawKey& k = drawKeys[i];

				//  set coords or advance
				if (k.x >=0)  x = k.x;  else  x -= k.x;
				if (k.y > 0)  y = k.y + yPosLay;  else  y -= k.y;

				//  distance to cursor key center
				int dx = x + k.w/2 - drawX,
					dy = y + k.h/2 - drawY, ay = abs(dy), ax = abs(dx);

				if (i != drawId)
				{
					if (kRight > 0 && dx > 0 && abs(dy) < 8)  // >
					{
						if (dx < mxr)
						{	mxr = dx;  ii = i;  }
					}else
					if (kRight < 0 && dx < 0 && abs(dy) < 8)  // <
					{
						if (dx > mxl)
						{	mxl = dx;  ii = i;  }
					}else
					if (kUp < 0 && dy < 0 && ax < 6)  // ^
					{
						if (ay < myu || (ay == myu && ax < axu))
						{	myu = ay;  axu = ax;  ii = i;  }
					}else
					if (kUp > 0 && dy > 0 && ax < 6)  // v
					{
						if (dy <= myd && ax < axd)
						{	myd = dy;  axd = ax;  ii = i;  }
					}
				}
			}
			if (ii != -1)
				drawId = ii;

			if (drawId >= nDrawKeys) drawId -= nDrawKeys;
			if (drawId < 0)			 drawId += nDrawKeys;
			return;
		}

		if (pickCode)  // pick code  ----
		{
			if (kBack)
			{	//  apply in kc
				if (scId >= 0 && scId < kc.set.nkeys())
				{
					KC_Key& k = kc.set.keys[scId];
					if (keyCode != KEY_NONE)
						k.add(keyCode, nLay);
					else
						k.rem(nLay);
				}
				pickCode = 0;  return;
			}
			if (kFps)
			{	pickCode = 0;  return;  }

			if (kRight)  // <- ->
			{	if (grpFilt)
				{	keyGroup += kRight;
					if (keyGroup < 0)		 keyGroup = grpMax-1;
					if (keyGroup >= grpMax)	 keyGroup = 0;
					keyCode = grpStart[keyGroup];
				}else
					keyCode += kRight * 12;
			}else
			if (kUp)	keyCode += kUp;  else
			if (kPgUp)	keyCode += kPgUp * 4;  else

			if (kEnd < 0)  // home  filter
				grpFilt = 1-grpFilt;

			//  grp range
			if (grpFilt)
			{	if (keyCode < grpStart[keyGroup])
					keyCode = grpEnd[keyGroup];
				if (keyCode > grpEnd[keyGroup])
					keyCode = grpStart[keyGroup];
			}
			//  all range
			if (keyCode < 0)			  keyCode += KEYS_ALL_EXT;
			if (keyCode >= KEYS_ALL_EXT)  keyCode -= KEYS_ALL_EXT;
			return;
		}
	}


	//  sequence list
	//-------------------------------------------------------------------------------
	if (mlevel == 1 && ym == M_Sequences)
	{
		bool lay2 = kc.nLayer == 2;
		int q = seqId();
		int len = kc.set.seqs[q].len();
		std::vector<uint8_t>& dt = kc.set.seqs[q].data;

		//  toggle edit  ----
		if (kEnt && (!edit || lay2))
		{
			edit = 1-edit;
			if (edit)  // enter edit
			{
				//if (edpos > len)  // if
				edpos = len;  // end
			}
			return;
		}

		if (edit)
		{			//  edit sequence  ----
			//uint8_t fun = 0;  // none
			if (lay2)  //` switch..
			{	//  move cursor
				if (kEnd < 0)  edpos = 0;
				if (kEnd > 0)  edpos = len;

				if (kRight < 0)  if (edpos > 0)  --edpos;
				if (kRight > 0)  if (edpos < len)  ++edpos;

				if ((kDel || kBckSp) && len > 0)
				{
					int i = edpos;  // del>
					if (kBckSp)
					{	i = max(0, edpos-1);  // <del
						edpos = i;  //
					}
					for (; i < len-1; ++i)
						dt[i] = dt[i+1];
					dt.pop_back();
					--len;
					if (edpos > len)
						edpos = len;
				}
				if (kIns)
					edins = 1 - edins;  // ins/ovr
				//if (kEnt)  SeqClear(q);  // erase

				//if (key(F1))  fun = 1;  // set delay cmd todo
				//if (key(F2))  fun = 2;  // wait cmd
			}
			if (!lay2)  // || fun > 0)
			{
				// manual pick list?
				//  find key, if none
				int ii = -1;
				for (uint i=0; i < ScanKeys; ++i)
				{
					if (Matrix_scanArray[i].state == KeyState_Press
						&& int(i) < kc.set.nkeys())
					{
						ii = kc.set.keys[i].get(kc.nLayer);  //0.
						if (ii == KEY_NONE || ii >= KEYS_ALL)  ii = -1;  // norm only
						break;
					}
				}
				//  add key to sequence
				if (ii >= 0) {  // && edpos < iSeqLen-1)
				if (edpos >= len)  // at end
				{
					dt.push_back(ii);
					edpos++;  //len++;
				}else
				if (edins)  // insert
				{
					dt.push_back(0);  ++len;
					for (int i=len-1; i > edpos; --i)
						dt[i] = dt[i-1];
					dt[edpos] = ii;
					edpos++;  //len++;
				}
				else  // overwrite
				{	dt[edpos] = ii;
					if (edpos < len)  ++edpos;
				}
			}	}
		}else
		{	if (kSave)  // save
			{	kc.Save();  infType = 2;  tInfo = -1;
			}
			if (kLoad)  // load
			{	kc.Load();  infType = 1;  tInfo = -1;
			}

			if (kUp > 0)  // move
			{	++slot;  if (slot >= iPage) {  slot = 0;
				++page;  if (page >= kc.set.seqSlots/iPage)  page = 0;
			}	}
			if (kUp < 0)
			{	--slot;  if (slot < 0) {  slot = iPage-1;
				--page;  if (page < 0)  page = kc.set.seqSlots/iPage-1;
			}	}
			if (kPgUp > 0)  // page
			{	++page;  if (page >= kc.set.seqSlots/iPage)  page = 0;
			}
			if (kPgUp < 0)
			{	--page;  if (page < 0)  page = kc.set.seqSlots/iPage-1;
			}

			if (kCopy)  // copy
			{	kc.set.copy.data = dt;  cpId = q;
				infType = 3;  tInfo = -1;
			}
			//  paste, set
			if (kPaste)
			{	dt = kc.set.copy.data;
				infType = 4;  tInfo = -1;
			}
			//  swap, xchg
			if (kSwap && cpId != -1)
			{
				KC_Sequence cp = kc.set.seqs[cpId];
				kc.set.seqs[cpId] = kc.set.seqs[q];
				kc.set.seqs[q] = cp;
			}
			if (kBack)
				--mlevel;
		}
		return;
	}


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
	//............................................
	if (mlevel == 1 && ym == M_Mapping)
	{
		#define ADD(v,a,ma,mi)  {  v+=a;  if (v >= ma)  v = mi;  if (v < mi)  v = ma-1;  }

		if (kUp)  // menu up,dn
			ADD(ym1[ym], kUp, YM1[ym], 0)

		if (kRight > 0)
		{	//  enter modes
			if (yy == 3)  pickCode = 1;  else
			if (yy == 0)  pressKey = 1;  else
			if (yy == 1)  moveCur = 1;
		}
		if (kRight)  // chg lay
			if (yy == 2)  ADD(nLay, kRight, 8,0)
		return;
	}


	if (mlevel == 1)  //  sub menu
	{	//  navigate
		if (kRight < 0)	mlevel = 0;  // <back
		if (ym != M_Display)
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
