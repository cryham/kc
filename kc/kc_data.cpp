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

			if (code >= K_Layer1 && code <= K_Layer15)  //7 16-
			{
				//  set layer, hold
				if (on)   nLayer = code - K_Layer1 + 1;
				else
				if (off)  nLayer = 0;  // todo defLay par

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
void KC_Main::Send(uint32_t ms)
{
	//  in sequence
	if (inSeq != -1 && (ms - tiSeq > 20 || ms < tiSeq))  // par
	{
		tiSeq = ms;
		KC_Sequence sq = set.seqs[inSeq];
		uint8_t code = sq.data[seqPos];
		uint usb = cKeyUsb[code];
		if (seqRel == 0)
		{
			Keyboard.press(usb);
			Keyboard.send_now();
			++seqRel;
		}
		else if (seqRel == 1)
		{
			Keyboard.release(usb);
			Keyboard.send_now();
			++seqRel;
		}
		else if (seqRel >= 2)
		{
			seqRel = 0;
			++seqPos;  // next seq byte
			if (seqPos >= sq.len())
				inSeq = -1;  // end
		}
	}

	//  all matrix scan codes
	uint c,r;  int id;
	for (c=0; c < NumCols; ++c)
	for (r=0; r < NumRows; ++r)
	{
		id = NumCols * r + c;  // scan id
		KeyState& k = Matrix_scanArray[id];
		//  state
		bool on = k.state == KeyState_Press;
		bool off = k.state == KeyState_Release;

		if (on || off)
		if (id < set.nkeys())
		{
			if (on)
			{	//  get code for current layer
				uint8_t code = set.keys[id].get(nLayer);
				if (code > KEY_NONE && code < KEYS_ALL)
				{
					//  if 1 key, send
					uint usb = cKeyUsb[code];
					if (on)
					{	//  save layer of press
						k.layerOn = nLayer;
						Keyboard.press(usb);
						Keyboard.send_now();
					}
				}else  // special
				if (code >= K_Seq0 && code <= K_SeqLast
					&& inSeq < 0)
				{
					inSeq = code - K_Seq0;
					if (set.nseqs() > code - K_Seq0
					&& set.seqs[inSeq].len() > 0)
					{
						//  start seq
						tiSeq = ms;
						Keyboard.releaseAll();
						seqPos = 0;  seqRel = 0;
					}
					else  inSeq = -1;
				}
			}
			else if (off)
			{	//  send for layer it was pressed on
				uint8_t code = set.keys[id].get(k.layerOn);
				uint usb = cKeyUsb[code];
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

//  add data at layer,  code > KEY_NONE
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
}

//  remove layer data
void KC_Key::rem(uint32_t lay)
{
	if (!hasLay(lay))  return;

	layUse &= ~(1ul << lay);  // clear use bit

	data[lay] = KEY_NONE;  // disabled
	// pop last 0s-
}
