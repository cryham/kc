#include "WProgram.h"
#include "kc_data.h"

//  load, save in eeprom
//.............................................
#define EOfs 0  //  offset and max size to use
#define ESize 2048
#define Erd(a)    eeprom_read_byte((uint8_t*)a)
#define Ewr(a,b)  eeprom_write_byte((uint8_t*)a, b)


//  Load
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
void KC_Main::Load()
{
	err[0]=0;
	//set.Clear();
	//KC_Setup s;  // load into temp !crash

	int a = 0, i, n;  uint8_t b;
	//  header
	set.h1 = Erd(a);  if (set.h1 != 'k') {  strcpy(err, "Hdr1 !k");  return;  }  ++a;
	set.h2 = Erd(a);  if (set.h2 != 'c') {  strcpy(err, "Hdr2 !c");  return;  }  ++a;
	set.ver = Erd(a);  if (set.ver > 1) {  strcpy(err, "Hdr3 ver >1");  return;  }  ++a;

	//  matrix
	set.rows = Erd(a);  if (set.rows > 8) {  strcpy(err, "Scan rows >8");  return;  }  ++a;
	set.cols = Erd(a);  if (set.cols > 18) {  strcpy(err, "Scan cols >18");  return;  }  ++a;
	set.scanKeys = set.rows * set.cols;
	set.iSlots = Erd(a);  if (set.iSlots > 60) {  strcpy(err, "Seq slots >60");  return;  }  ++a;

	// inc num when saved, set ver
	// params here, size..

	//  keys
	for (i=0; i < set.scanKeys; ++i)
	{
		uint8_t len = Erd(a);  ++a;
		if (len > KC_MaxLayers)
		{	strcpy(err, "Key lay >8");  return;  }
		KC_Key k;
		for (n=0; n < len; ++n)
		{
			b = Erd(a);  ++a;
			if (b != KEY_NONE)
				k.add(b, n);
		}
		set.keys[i] = k;
	}

	//  seq
	//	KC_Sequence sq;  // empty
/*	for (i=0; i < s.iSlots; ++i)
	{	seqs.push_back(sq);
	}
*/
	memSize = a;
	strcpy(err, "ok");

	//  if evth ok, full set
	//set = s;
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
	Ewr(a, set.h1);  ++a;
	Ewr(a, set.h2);  ++a;
	Ewr(a, set.ver);  ++a;

	//  matrix
	Ewr(a, set.rows);  ++a;
	Ewr(a, set.cols);  ++a;
	Ewr(a, set.iSlots);  ++a;

	//  keys
	for (i=0; i < set.scanKeys; ++i)
	{
		const KC_Key& k = set.keys[i];
		uint8_t len = k.data.size();
		Ewr(a, len);  ++a;
		for (n=0; n < len; ++n)
		{	Ewr(a, k.data[n]);  ++a;  }
	}

	//  seq

	memSize = a;
	if (memSize >= ESize)
		strcpy(err, "Too BIG");
}
