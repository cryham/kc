#include "kc_data.h"
#include "kbd_layout.h"


KC_Setup::KC_Setup()
{
	//Clear();
	InitCK1_8x6();
}

//  todo  Init  CK1 8x6
//  from drawKeys layout
void KC_Setup::InitCK1_8x6()
{
	Clear();
	rows = 6;  cols = 8;  maxKeys = rows * cols;

	int i;
	KC_Key k;
	for (i=0; i < maxKeys; ++i)
		keys.push_back(k);

	//  from draw layout
	for (i=0; i < numKeys; ++i)
	{
		const SKey& dk = drawKeys[i];
		if (dk.sc != NO)
		{
			KC_Key& kk = keys[dk.sc];
			kk.layUse = 1;
			kk.data.clear();
			kk.data.push_back(dk.code);  //-
		}
	}
}

//  clear evth
void KC_Setup::Clear()
{
	kk='k';  ver = 1;  // header
	//  rows * cols = maxKeys,  matrix setup
	rows = 8;  cols = 18;  maxKeys = rows * cols;
	// ver num or date saved-

	keys.clear();
	seqs.clear();
}



#if 0
//.............................................
//  load, save seq in eeprom
//  offset and max size to use
#define EOfs 0
#define ESize 2048

void Gui::Load()  //  Load
{
	Clear();
	int i,n,a;  uint8_t b, er=0;
	//  lengths
	a = EOfs;
	for (i=0; i < iSlots; ++i,++a)
	{	b = eeprom_read_byte((uint8_t*)a);
		seql[i] = b < iSeqLen ? b : 0;
		if (b >= iSeqLen)  ++er;
	}
	//  error, not inited
	if (er > 0)
	{	Clear();  return;  }
	
	//  data
	a = EOfs + iSlots;
	for (i=0; i < iSlots; ++i)
	for (n=0; n < seql[i] && a < ESize; ++n)
	{
		seq[i][n] = eeprom_read_byte((uint8_t*)a);  ++a;
	}
	memSize = a;  infType = 1;
}

void Gui::Save()  //  Save
{
	int i,n,a;
	//  lengths
	a = 0;  // ofs
	for (i=0; i < iSlots; ++i,++a)
		eeprom_write_byte((uint8_t*)a, seql[i]);

	//  data
	a = EOfs + iSlots;
	for (i=0; i < iSlots; ++i)  //if (seql[i])
	for (n=0; n < seql[i] && a < ESize; ++n)
	{
		eeprom_write_byte((uint8_t*)a, seq[i][n]);  ++a;
	}
	memSize = a;  infType = 2;
}
#endif
