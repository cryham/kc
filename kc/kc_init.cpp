#include "kc_data.h"
#include "kbd_layout.h"
#include "usb_keyboard.h"
#include "matrix.h"
#include "keys_usb.h"
#include "gui.h"
#include "kc_params.h"


//  ctor init
//------------------------------------------------
void KC_Main::SeqModClear()
{
	//  clear modif
	for (int i=0; i <= K_ModLast; ++i)
		seqMod[i] = 0;
}

KC_Main::KC_Main()
{
	err=E_ok;  memSize = 0;

	setDac = 1;

	SeqModClear();
}

//  clear evth
void KC_Setup::Clear()
{
	//  header  ver
	h1='k';  h2='c';  ver = 2;  //+ up on changes

	//  default  matrix
	rows = 8;  cols = 18;  scanKeys = rows * cols;
	seqSlots = 60;

	keys.clear();
	seqs.clear();

	KC_Sequence sq;  // empty
	//  const size
	for (int i=0; i < seqSlots; ++i)
		seqs.push_back(sq);
}

KC_Setup::KC_Setup()
{
	//Clear();
	InitCK();
}

//  Init  CK1 8x6
//------------------------------------------------
void KC_Setup::InitCK()
{
	Clear();
#if defined(CK1)
	rows = 6;  cols = 8;
#elif defined(CK3)
	rows = 8;  cols = 18;
#endif
	scanKeys = rows * cols;
	seqSlots = 60;

	int i;
	KC_Key k;
	for (i=0; i < scanKeys; ++i)
		keys.push_back(k);


	//  from draw layout
	for (i=0; i < nDrawKeys; ++i)
	{
		const DrawKey& dk = drawKeys[i];
		if (dk.sc != NO) // && dk.code != K_F12)
		{
			KC_Key& kk = keys[dk.sc];
			kk.add(dk.code, 0);

		#if 1  //  override  --*
		#ifdef CK1
				 if (dk.code == K_INS)		kk.add(K_Layer1, 0);
			else if (dk.code == K_SPACE)	kk.add(K_Layer2, 0);
		#else
				 if (dk.code == K_RCTRL)	kk.add(K_Layer1, 0);
			else if (dk.code == K_CAPS)		kk.add(K_Layer2, 0);
		#endif
			else if (dk.code == K_T)	kk.add(K_Seq0, 2);
			else if (dk.code == K_H)	kk.add(K_S2, 2);
			else if (dk.code < K_Z && i % 3 == 0)
			{	kk.add(dk.code+1, 1);
				kk.add(dk.code+2, 2);
			}else if (dk.code < K_Z && i % 3 == 1)
			{	kk.add(dk.code+1, 2);
			}else if (dk.code < K_UP && i % 3 == 1)
			{	kk.add(dk.code+1, 1);
			}	// mouse
			if (dk.code == K_UP)    kk.add(KM_Up, 2);  else
			if (dk.code == K_DOWN)  kk.add(KM_Down, 2);  else
			if (dk.code == K_LEFT)  kk.add(KM_Left, 2);  else
			if (dk.code == K_RIGHT) kk.add(KM_Right, 2);  else
			if (dk.code == K_HOME)  kk.add(KM_LMB, 2);  else
			if (dk.code == K_PGUP)  kk.add(KM_RMB, 2);  else
			if (dk.code == K_DEL)   kk.add(KM_MMB, 2);  else
			if (dk.code == K_END)   kk.add(KM_WhlUp, 2);  else
			if (dk.code == K_PGDN)  kk.add(KM_WhlDown, 2);
		#endif
	}	}

	#if 1  //  examples  --*
	KC_Sequence sq;
	sq.add(K_A);  sq.add(K_LSHIFT);
	sq.add(K_B);  sq.add(K_LSHIFT);  sq.add(K_C);
	seqs[0] = sq;  sq.data.clear();

	sq.add(K_RCTRL);  sq.add(K_RSHIFT);
	sq.add(K_LEFT);  sq.add(K_LEFT);  sq.add(K_LEFT);
	seqs[2] = sq;  sq.data.clear();

	sq.add(K_1);  sq.add(K_2);
	sq.add(K_3);  sq.add(K_4);
	seqs[5] = sq;  sq.data.clear();

	sq.add(K_Q);  sq.add(K_E);  sq.add(K_LSHIFT);
	sq.add(K_F);  sq.add(K_G);
	seqs[7] = sq;
	#endif
}
