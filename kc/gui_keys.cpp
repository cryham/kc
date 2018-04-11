#include "gui.h"
#include "matrix.h"
#include "kbd_layout.h"
#include "keys_usb.h"

extern KC_Main kc;
extern volatile int16_t USBMouse_Relative_x;
extern volatile int16_t USBMouse_Relative_y;


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
	kCtrl = KeyH(3,4); kSh  = KeyH(2,0);  // Ins  \|
	kInf  = Key(4,1);  kFps = Key(5,1);  // Mul*  Sub-
	//  edit seq
	kIns = Key(3,4);   kDel = kr(1,4,dt);  kBckSp = kr(1,0,dt);
	kCopy = Key(4,7);  kPaste =  Key(4,5);  kSwap = Key(5,5);  // C V B
	kLoad = Key(1,7);  kSave = Key(3,7);  // F3  F4

	//kF12 = Key(5,0) - Key(3,0);  // F12  F11

	//  todo mouse test
	USBMouse_Relative_x = 8 * (KeyH(1,1) - KeyH(1,2));
	USBMouse_Relative_y = 8 * (KeyH(2,4) - KeyH(0,4));

	/*	    0    1     2    3  4    5  6    7
		0        PgUp  Hom  U  ^    R  +    E
		1   Bck  ->    <-   Y  Del  T       F3
		2   \    PgDn  End  J  v    F  Ent  D
		3   F11  Del.  Spc  H  Ins  G  Up   F4
		4   Ent  *     Num  M  /    V       C
		5   F12  -          N       B			*/


	//  Mapping  edit modes
	//............................................
	if (ym == M_Display && mlevel == 1)
	{
		if (kUp)
		{	kc.valDac += kUp * 10;  kc.setDac = 1;  }
		if (kPgUp)
		{	kc.valDac += kPgUp * 40;  kc.setDac = 1;  }
	}

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
	//............................................


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

				//if (key(F1))  fun = 1;  // set delay cmd
				//if (key(F2))  fun = 2;  // wait cmd
			}
			if (!lay2)  // || fun > 0)
			{
				// todo manual pick list?
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
	int16_t& m = Matrix_scanArray[i].autoRepeat;

	if (st == KeyState_Press)
	{
		m = -1;  // start delay
		return 1;
	}
	else if (st == KeyState_Hold)
	{
		if (m < 0)
		{	m -= dt;
			if (m < -kc.krDelay)  // delay ms
			{	m = 0;  return 1;  }
		}else
		{	m += dt;
			if (m > kc.krRepeat)  // repeat freq
			{	m = 0;  return 1;  }
		}
	}
	return 0;
}
