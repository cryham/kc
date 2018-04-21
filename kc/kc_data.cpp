#include "kc_data.h"
#include "kbd_layout.h"
#include "usb_keyboard.h"
#include "usb_mouse.h"
#include "matrix.h"
#include "keys_usb.h"
#include "WProgram.h"

KC_Params par;

//  update layers  (always)
//------------------------------------------------
void KC_Main::UpdLay()
{
	//  dac led
	if (setDac)
	{	setDac = 0;
		analogWriteDAC0(par.valDac);
	}

	//  all matrix scan codes  ----
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

//  keyboard send  (to pc usb)
//------------------------------------------------
void KC_Main::Send(uint32_t ms)
{
	//  in sequence  ***
	if (inSeq >= 0 && (ms - tiSeq > dtSeq || ms < tiSeq))  // par
	{
		tiSeq = ms;
		const KC_Sequence& sq = set.seqs[inSeq];
		uint8_t code = sq.data[seqPos];
		uint usb = cKeyUsb[code];

		//  modifiers
		if (code > KEY_NONE && code <= K_ModLast)
		{
			if (seqMod[code] == 0)
			{
				Keyboard.press(usb);
				Keyboard.send_now();
				seqMod[code] = 1;
			}else
			{	Keyboard.release(usb);
				Keyboard.send_now();
				seqMod[code] = 0;
			}
			seqRel = 2;  // next key
		}
		//  keys
		else if (seqRel == 0)
		{
			Keyboard.press(usb);
			Keyboard.send_now();
			++seqRel;  // next step
		}
		else if (seqRel == 1)
		{
			Keyboard.release(usb);
			Keyboard.send_now();
			++seqRel;
		}
		//  both
		if (seqRel >= 2)
		{
			seqRel = 0;
			++seqPos;  // next seq byte
			if (seqPos >= sq.len())
			{
				Keyboard.releaseAll();
				SeqModClear();
				inSeq = -1;  // end
			}
		}
	}
	//  deny keys during seq
	//if (inSeq >= 0)
	//	return;


	//  all matrix scan codes  ----
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
			{
				//  get code for current layer
				uint8_t code = set.keys[id].get(nLayer);
				if (code > KEY_NONE && code < KEYS_ALL)
				{
					//  if 1 key, send press
					uint usb = cKeyUsb[code];
					k.layerOn = nLayer;  // save layer of press
					Keyboard.press(usb);
					Keyboard.send_now();
				}
				//  mouse  * * *
				else
				if (code >= KM_Left && code <= KM_Down)
				{
					switch (code)
					{
					case KM_Left:  Mouse_input_x -= 8;  break;
					case KM_Right: Mouse_input_x += 8;  break;
					case KM_Up:    Mouse_input_y -= 8;  break;
					case KM_Down:  Mouse_input_y += 8;  break;
					}
					k.layerOn = nLayer;
				}
				else if (code >= KM_WhlUp && code <= KM_WhlRight)
				{
					switch (code)
					{
					case KM_WhlLeft:  Mouse_wheel_x =-1;  break;
					case KM_WhlRight: Mouse_wheel_x = 1;  break;
					case KM_WhlUp:    Mouse_wheel_y =-1;  break;
					case KM_WhlDown:  Mouse_wheel_y = 1;  break;
					}
					k.layerOn = nLayer;
				}
				else if (code >= KM_LMB && code <= KM_Forw)
				{
					uint8_t b = MOUSE_LEFT;
					switch (code)
					{
					case KM_LMB:  break;
					case KM_RMB:  b = MOUSE_RIGHT;  break;
					case KM_MMB:  b = MOUSE_MIDDLE;  break;
					case KM_Back: b = MOUSE_BACK;   break;
					case KM_Forw: b = MOUSE_FORWARD;  break;
					}
					Mouse.press(b);
					k.layerOn = nLayer;
				}
				else  //  sequences
				if (code >= K_Seq0 && code <= K_SeqLast
					&& inSeq < 0)
				{
					int8_t sq = code - K_Seq0;
					if (set.nseqs() > sq &&
						set.seqs[sq].len() > 0)
					{
						//  start seq  ***
						Keyboard.releaseAll();
						SeqModClear();
						inSeq = sq;  tiSeq = ms;
						seqPos = 0;  seqRel = 0;
						dtSeq = par.dtSeqDef;
					}
					else  inSeq = -1;
				}
			}
			else if (off)
			{
				//  send for layer it was pressed on
				uint8_t code = set.keys[id].get(k.layerOn);
				if (code > KEY_NONE && code < KEYS_ALL)
				{
					//  release 1 key
					uint usb = cKeyUsb[code];
					Keyboard.release(usb);
					Keyboard.send_now();
				}
				//  mouse  * * *
				else
				if (code >= KM_Left && code <= KM_Down)
				{
					switch (code)
					{
					case KM_Left:  Mouse_input_x += 8;  break;
					case KM_Right: Mouse_input_x -= 8;  break;
					case KM_Up:    Mouse_input_y += 8;  break;
					case KM_Down:  Mouse_input_y -= 8;  break;
					}
				}
				else if (code >= KM_LMB && code <= KM_Forw)
				{
					uint8_t b = MOUSE_LEFT;
					switch (code)
					{
					case KM_LMB:  break;
					case KM_RMB:  b = MOUSE_RIGHT;  break;
					case KM_MMB:  b = MOUSE_MIDDLE;  break;
					case KM_Back: b = MOUSE_BACK;   break;
					case KM_Forw: b = MOUSE_FORWARD;  break;
					}
					Mouse.release(b);
				}
			}
		}
	}

	//  mouse move and send  * * *

	Mouse_shift = KeyH( 94);  // par..
	usb_mouse_idle();
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
