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
	seqSlots = KC_MaxSeqs;

	int i,l;
	for (l=0; l < KC_MaxLayers; ++l)
	for (i=0; i < KC_MaxRows * KC_MaxCols; ++i)
		key[l][i] = KEY_NONE;

	//  const size
	for (i=0; i < KC_MaxSeqs; ++i)
	{	seqs[i].data.clear();
		seqs[i].data.shrink_to_fit();  // free ram
	}
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
	seqSlots = KC_MaxSeqs;

	int i;
	//  from draw layout
	for (i=0; i < nDrawKeys; ++i)
	{
		const DrawKey& dk = drawKeys[i];
		if (dk.sc != NO)
		{
			#define add(code, lay)  key[lay][dk.sc] = code
			add(dk.code, 0);

		#if 1  //  override  --*
		#ifdef CK1
				 if (dk.code == K_INS)	 add(K_Layer1, 0);
			else if (dk.code == K_SPACE) add(K_Layer2, 0);
		#else
				 if (dk.code == K_MENU)	add(K_Layer1, 0);
			else if (dk.code == K_CAPS)	add(K_Layer2, 0);
		#endif
			else if (dk.code == K_1)	add(K_Seq0, 2);
			else if (dk.code == K_Q)	add(K_S1, 2);
			else  // mouse
			if (dk.code == K_UP)    add(KM_Up, 2);  else
			if (dk.code == K_DOWN)  add(KM_Down, 2);  else
			if (dk.code == K_LEFT)  add(KM_Left, 2);  else
			if (dk.code == K_RIGHT) add(KM_Right, 2);  else
			if (dk.code == K_HOME)  add(KM_LMB, 2);  else
			if (dk.code == K_PGUP)  add(KM_RMB, 2);  else
			if (dk.code == K_DEL)   add(KM_MMB, 2);  else
			if (dk.code == K_END)   add(KM_WhlUp, 2);  else
			if (dk.code == K_PGDN)  add(KM_WhlDown, 2);
		#endif
			#undef add
	}	}

	#if 1  //  examples  --*
	KC_Sequence sq;
	sq.add(K_A);  sq.add(K_LSHIFT);
	sq.add(K_B);  sq.add(K_LSHIFT);  sq.add(K_C);
	seqs[0] = sq;  sq.data.clear();

	sq.add(K_RCTRL);  sq.add(K_RSHIFT);
	sq.add(K_LEFT);  sq.add(K_LEFT);  sq.add(K_LEFT);
	seqs[2] = sq;  sq.data.clear();
	#endif
}
