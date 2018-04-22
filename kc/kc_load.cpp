#include "WProgram.h"
#include "kc_data.h"
//extern KC_Params par;

//  load, save in eeprom
//.............................................
#define EOfs 0  //  offset and max size to use
#define ESize 2048

const char* erSize = "> Max size";

#define Erd(a)    eeprom_read_byte((uint8_t*)a);      ++a;  memSize = a;  if (a >= ESize) {  err=E_size;  return;  }
#define Ewr(a,b)  eeprom_write_byte((uint8_t*)a, b);  ++a;  memSize = a;  if (a >= ESize) {  err=E_size;  return;  }


//  default params  ----
void ParInit()
{
	par.debounce = 1;  // ms?
	par.strobe_delay = 4;
	par.scanFreq = 50;  // mul by 1 kHz

	par.valDac = 4000;
	par.startScreen = 0;
	par.verCounter = 0;

	par.krDelay = 250/5;  par.krRepeat = 80/5;  // ms

	par.mkSpeed = 100;  par.mkAccel = 100;

	par.dtSeqDef = 20;
}

//  errors
const char* KCerrStr[E_max] = {
	"ok", "> Max size",  "Hdr1 !k", "Hdr2 !c", "Hdr3 ver <",
	"Scan rows >8", "Scan cols >18",  "Seq slots >60", "Key lay >8",
	"r * c != sc", "nkeys != sc",
};


//  get ram used
uint16_t KC_Main::GetSize()
{
	uint16_t s=0;
	s += sizeof(KC_Main);
	s += sizeof(KC_Setup);
	int i;
	for (i=0; i < set.nkeys(); ++i)
	{
		s += sizeof(KC_Key);
		s += set.keys[i].data.capacity();  //size();
	}
	for (i=0; i < set.nseqs(); ++i)
	{
		s += sizeof(KC_Sequence);
		s += set.seqs[i].data.capacity();
	}
	return s;
}


//  Load
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
void KC_Main::Load()
{
	err = E_ok;
	//set.Clear();

	int a = 0, i, n;  uint8_t b;
	//  header
	set.h1 = Erd(a);  if (set.h1 != 'k') {  err=E_H1;  return;  }
	set.h2 = Erd(a);  if (set.h2 != 'c') {  err=E_H2;  return;  }
	set.ver = Erd(a);  if (set.ver > 2) {  err=E_ver;  return;  }

	//  matrix
	set.rows = Erd(a);  if (set.rows > 8) {  err=E_rows;  return;  }
	set.cols = Erd(a);  if (set.cols > 18) {  err=E_cols;  return;  }
	set.scanKeys = set.rows * set.cols;
	set.seqSlots = Erd(a);  if (set.seqSlots > 60) {  err=E_slots;  return;  }


	//  params  ----
	ParInit();  // defaults
	n = Erd(a);  // size
	eeprom_read_block((void*)&par, (void*)a, n);  a+=n;
	if (a >= ESize) {  err=E_size;  return;  }
	setDac = 1;  // upd


	//  Keys  ---
	for (i=0; i < set.scanKeys; ++i)
	{
		uint8_t len = Erd(a);
		if (len > KC_MaxLayers)
		{	err=E_lay;  return;  }

		KC_Key k;
		for (n=0; n < len; ++n)
		{
			b = Erd(a);
			if (b != KEY_NONE)
				k.add(b, n);
		}
		set.keys[i] = k;
	}

	//  Seqs  ---
	for (i=0; i < set.seqSlots; ++i)
	{
		uint8_t len = Erd(a);

		KC_Sequence s;
		for (n=0; n < len; ++n)
		{
			b = Erd(a);
			s.add(b);
		}
		set.seqs[i] = s;
	}

	memSize = a;
}

//  Save
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
void KC_Main::Save()
{
	err = E_ok;
	//  sth very wrong
	if (set.rows * set.cols != set.scanKeys)
	{	err=E_rcEq;  return;  }

	if (set.nkeys() != int(set.scanKeys))
	{	err=E_nkeys;  return;  }

	int a = 0, i, n;

	//  header
	Ewr(a, set.h1);  Ewr(a, set.h2);  Ewr(a, set.ver);

	//  matrix
	Ewr(a, set.rows);  Ewr(a, set.cols);  Ewr(a, set.seqSlots);


	//  params  ----
	++par.verCounter;  // inc ver
	n = sizeof(par);
	Ewr(a, n);  // size
	eeprom_write_block((void*)&par, (void*)a, n);  a+=n;
	if (a >= ESize) {  err=E_size;  return;  }


	//  Keys  ---
	for (i=0; i < set.scanKeys; ++i)
	{
		const KC_Key& k = set.keys[i];
		uint8_t len = k.data.size();
		Ewr(a, len);

		for (n=0; n < len; ++n)
		{	Ewr(a, k.data[n]);  }
	}

	//  Seqs  ---
	for (i=0; i < set.seqSlots; ++i)
	{
		const KC_Sequence& s = set.seqs[i];
		uint8_t len = s.len();
		Ewr(a, len);

		for (n=0; n < len; ++n)
		{	Ewr(a, s.data[n]);  }
	}

	memSize = a;
}
