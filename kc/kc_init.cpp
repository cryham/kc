#include "kc_data.h"
#include "kbd_layout.h"
#include "usb_keyboard.h"
#include "matrix.h"
#include "keys_usb.h"


//  ctor init
//------------------------------------------------
KC_Main::KC_Main()
{
	//  init grp Start and End
	for (int g=0; g < grpMax; ++g)
	{
		grpStart[g] = 0;  // all
		grpEnd[g] = KEYS_ALL_EXT;

		//  find first and last key, for this group
		for (int i=0; i < KEYS_ALL_EXT; ++i)
			if (cKeyGrp[i] == g)
			{
				if (grpStart[g] == 0)  grpStart[g] = i;
				grpEnd[g] = i;
			}
	}
}

KC_Setup::KC_Setup()
{
	//Clear();
	InitCK1_8x6();
}

//  Init  CK1 8x6
//------------------------------------------------
void KC_Setup::InitCK1_8x6()
{
	Clear();
	rows = 6;  cols = 8;  scanKeys = rows * cols;

	int i;
	KC_Key k;
	for (i=0; i < scanKeys; ++i)
		keys.push_back(k);


	//  from draw layout
	for (i=0; i < nDrawKeys; ++i)
	{
		const SKey& dk = drawKeys[i];
		if (dk.sc != NO)
		{
			KC_Key& kk = keys[dk.sc];
			kk.add(dk.code, 0);


			//  override  --*
			if (dk.code == K_INS)
				kk.add(K_Layer1, 0);
			else
			if (dk.code == K_SPACE)
				kk.add(K_Layer2, 0);
			else
			if (dk.code < K_Z && i % 3 == 0)
			{
				kk.add(dk.code+1, 1);
				kk.add(dk.code+2, 2);
			}else
			if (dk.code < K_Z && i % 3 == 1)
			{
				kk.add(dk.code+1, 2);
			}else
			if (dk.code < K_UP && i % 3 == 1)
			{
				kk.add(dk.code+1, 1);
			}/**/
	}	}
}

//  clear evth
void KC_Setup::Clear()
{
	kk='k';  ver = 1;  // header

	//  default  matrix setup
	rows = 8;  cols = 18;  scanKeys = rows * cols;
	// ver num or date saved-

	keys.clear();
	seqs.clear();

	KC_Sequence sq;  // empty
	//  const size
	for (int i=0; i < /*iSlots*/60; ++i)
		seqs.push_back(sq);


	//  examples  --*
	sq.data.push_back(K_A);
	sq.data.push_back(K_B);
	sq.data.push_back(K_C);
	seqs[2] = sq;

	sq.data.clear();
	sq.data.push_back(K_1);
	sq.data.push_back(K_2);
	seqs[5] = sq;

	sq.data.clear();
	sq.data.push_back(K_Q);
	sq.data.push_back(K_E);
	sq.data.push_back(K_LSHIFT);
	sq.data.push_back(K_F);
	sq.data.push_back(K_G);
	seqs[7] = sq;
}
