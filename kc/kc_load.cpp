#include "WProgram.h"
#include "kc_data.h"
//extern KC_Params par;

//  load, save in eeprom
//.............................................
#define EOfs 0  //  offset and max size to use
#define ESize 2048

const char* erSize = "> Max size";

#define Erd(a)    eeprom_read_byte((uint8_t*)a);      ++a;  memSize = a;  if (a >= ESize) {  strcpy(err, erSize);  return;  }
#define Ewr(a,b)  eeprom_write_byte((uint8_t*)a, b);  ++a;  memSize = a;  if (a >= ESize) {  strcpy(err, erSize);  return;  }


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

//  Load
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
void KC_Main::Load()
{
	err[0]=0;
	//set.Clear();

	int a = 0, i, n;  uint8_t b;
	//  header
	set.h1 = Erd(a);  if (set.h1 != 'k') {  strcpy(err, "Hdr1 !k");  return;  }
	set.h2 = Erd(a);  if (set.h2 != 'c') {  strcpy(err, "Hdr2 !c");  return;  }
	set.ver = Erd(a);  if (set.ver > 2) {  strcpy(err, "Hdr3 ver >1");  return;  }

	//  matrix
	set.rows = Erd(a);  if (set.rows > 8) {  strcpy(err, "Scan rows >8");  return;  }
	set.cols = Erd(a);  if (set.cols > 18) {  strcpy(err, "Scan cols >18");  return;  }
	set.scanKeys = set.rows * set.cols;
	set.seqSlots = Erd(a);  if (set.seqSlots > 60) {  strcpy(err, "Seq slots >60");  return;  }


	//  params  ----
	ParInit();  // defaults
	n = Erd(a);  // size
	eeprom_read_block((void*)&par, (void*)a, n);  a+=n;
	if (a >= ESize) {  strcpy(err, erSize);  return;  }


	//  Keys  ---
	for (i=0; i < set.scanKeys; ++i)
	{
		uint8_t len = Erd(a);
		if (len > KC_MaxLayers)
		{	strcpy(err, "Key lay >8");  return;  }

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
	strcpy(err, "ok");
}

//  Save
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
void KC_Main::Save()
{
	err[0]=0;
	//  sth very wrong
	if (set.rows * set.cols != set.scanKeys)
	{	strcpy(err, "r * c != sc");  return;  }

	if (set.nkeys() != int(set.scanKeys))
	{	strcpy(err, "nkeys != sc");  return;  }

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
	if (a >= ESize) {  strcpy(err, erSize);  return;  }


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
	if (memSize >= ESize)
		strcpy(err, "Too BIG");
}
