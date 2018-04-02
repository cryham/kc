#include "kc_data.h"
#include "kbd_layout.h"
#include "usb_keyboard.h"
#include "matrix.h"
#include "keys_usb.h"


//  update layer
//------------------------------------------------
void KC_Main::UpdLay()
{
	//  all matrix scan codes
	uint c,r;  int id;
	for (c=0; c < NumCols; ++c)
	for (r=0; r < NumRows; ++r)
	{
		id = NumCols * r + c;  // scan id
		const KeyState& k = Matrix_scanArray[id];
		//  state
		bool on = k.state == KeyState_Press;
		bool off = k.state == KeyState_Release;
		if (on || off)
		if (id < set.nkeys())
		{
			//  get from kc
			uint8_t code = set.keys[id].get(/*nLayer*/0);

			if (code >= K_Layer0 && code <= K_Layer7)  //15
			{
				//  set layer, hold  todo
				if (on)   nLayer = code - K_Layer0;
				else
				if (off)  nLayer = 0;  // defLay par

				if (nLayer >= KC_MaxLayers-1)
					nLayer = KC_MaxLayers-1;
				//if (nLayer < 0)
				//	nLayer = 0;
			}
		}
	}
}

//  keyboard send
//------------------------------------------------
void KC_Main::Send()
{
	//  all matrix scan codes
	uint c,r;  int id;
	for (c=0; c < NumCols; ++c)
	for (r=0; r < NumRows; ++r)
	{
		id = NumCols * r + c;  // scan id
		const KeyState& k = Matrix_scanArray[id];
		//  state
		bool on = k.state == KeyState_Press;
		bool off = k.state == KeyState_Release;
		if (on || off)
		if (id < set.nkeys())
		{
			//  get from kc
			uint8_t code = set.keys[id].get(nLayer);

			if (code > KEY_NONE && code < KEYS_ALL)
			{
				//  send 1 key
				uint usb = cKeyUsb[code];
				if (on)
				{
					Keyboard.press(usb);
					Keyboard.send_now();
				}
				else if (off)
				{
					Keyboard.release(usb);
					Keyboard.send_now();
				}
			}
		}
	}
}

// . . . . . . . . . . . . . . . . . . . . . . . .
//  get data for layer, if any
uint8_t KC_Key::get(uint32_t lay)
{
	if (lay >= KC_MaxLayers || lay < 0)
		return KEY_NONE;

	//lay = 0;
	//return KEY_NONE;
	if (lay == 0 && hasLay(0) && !data.empty())
		return data[0];

	if (hasLay(lay))
		if (lay < data.size())
			return data[lay];

	return KEY_NONE;
}

//  add data at layer
void KC_Key::add(uint8_t code, uint32_t lay)
{
	layUse |= 1ul << lay;  // set use bit

	if (lay < data.size())
	{	data[lay] = code;  return;  }

	uint32_t i, s = data.size();
	for (i=s; i < lay+1; ++i)
		data.push_back(KEY_NONE);

	if (lay < data.size())  // sure
		data[lay] = code;
//	l 2  012
//	s 1  100
//	i=1 i < 3
}

//  remove layer data
void KC_Key::rem(uint32_t lay)
{
	if (!hasLay(lay))  return;

	layUse &= ~(1ul << lay);  // clear use bit

	data[lay] = KEY_NONE;  // disabled
	// pop last 0s-
}


//  init
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

//  todo  Init  CK1 8x6
//  from drawKeys layout
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
//			kk.layUse = 1;
//			kk.data.clear();
//			kk.data.push_back(dk.code);  //+
			kk.add(dk.code, 0);

			//  manual override
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
//				kk.layUse = 1+2+4;
//				kk.data.push_back(dk.code+1);
//				kk.data.push_back(dk.code+2);
			}else
			if (dk.code < K_Z && i % 3 == 1)
			{
				kk.add(dk.code+1, 2);
//				kk.layUse = 1+2;
//				kk.data.push_back(dk.code+1);
			}else
			if (dk.code < K_UP && i % 3 == 1)
			{
				kk.add(dk.code+1, 1);
//				kk.layUse = 1+4;
//				kk.data.push_back(dk.code+1);
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
