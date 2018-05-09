#include "kc_data.h"
#include "kbd_layout.h"
#include "usb_keyboard.h"
#include "usb_mouse.h"
#include "matrix.h"
#include "keys_usb.h"
#include "gui.h"
#include "WProgram.h"

KC_Params par;
extern Gui gui;


//  update layers  (always)
//------------------------------------------------------------------------
void KC_Main::UpdLay(uint32_t ms)
{
	//  brightness dac led  ~~~
	if (setDac)
	{	setDac = 0;
		int bri = gui.kbdSend ? par.brightOff : par.brightness;
		const int minBri = 3580;
		int val = bri == 0 ? 0 : bri * (4095 - minBri) / 100 + minBri;
		analogWriteDAC0(val);
	}

	//  all matrix scan codes  ----
	uint c,r;  int id;
	for (c=0; c < NumCols; ++c)
	for (r=0; r < NumRows; ++r)
	{
		id = NumCols * r + c;  // scan id
		if (id >= set.nkeys())  continue;

		//  state
		const KeyState& k = Matrix_scanArray[id];
		bool on = k.state == KeyState_Press;
		bool off = k.state == KeyState_Release;

		bool hold = k.state == KeyState_Hold;
		uint8_t codeL = set.key[nLayer][id];
		bool fun = codeL >= K_Fun0 && codeL <= K_Fun9;

		if (on || off)
		{
			//  get from kc
			uint8_t code0 = set.key[0][id];
			uint8_t codeL = set.key[nLayer][id];

			if (code0 >= K_Layer1 && code0 < K_Layer1+KC_MaxLayers)
			{
				//  set layer, hold
				if (on)   nLayer = code0 - K_Layer1 + 1;
				else  // default layer
				if (off)  nLayer = par.defLayer;

				if (nLayer >= KC_MaxLayers-1)
					nLayer = KC_MaxLayers-1;
			}
			else  //  display, internal functions  ***
			//..........................................................
			if (on && fun)
			{
				switch (codeL)
				{
				case K_Fun0:  // send, Gui toggle
					gui.kbdSend = 1-gui.kbdSend;
					setDac = 1;  break;

				case K_Fun1:
				case K_Fun2:  // brightness -+
					tiFun = ms;  break;  // delay no par

				case K_Fun3:  // soft reset  //NVIC_SystemReset();
					#define SCB_AIRCR (*(volatile uint32_t *)0xE000ED0C)
					SCB_AIRCR = 0x05FA0004;  break;
				}
			}
		}else if (hold)
		{
			if (fun)
			if (ms - tiFun > par.krRepeat*5 || ms < tiFun)
			{	tiFun = ms;
				uint8_t& br = gui.kbdSend ? par.brightOff : par.brightness;
				switch (codeL)
				{
				case K_Fun1:  // brightness -+
					br = gui.RangeAdd(br, (gui.kCtrl ?-10 :-2), 0, 100);
					setDac = 1;  break;
				case K_Fun2:
					br = gui.RangeAdd(br, (gui.kCtrl ? 10 : 2), 0, 100);
					setDac = 1;  break;
				}
			}
		}
	}
}

//  keyboard send  (to pc usb)
//------------------------------------------------------------------------------------------------
bool KC_Main::SeqEnd(const KC_Sequence& sq)
{
	if (seqPos >= sq.len())
	{
		Keyboard.releaseAll();
		SeqModClear();
		inSeq = -1;  // end
		return true;
	}
	return false;
}

//  const
const static uint8_t parMBtn[6]={
		MOUSE_LEFT, MOUSE_MIDDLE, MOUSE_RIGHT, MOUSE_BACK, MOUSE_FORWARD};

void KC_Main::Send(uint32_t ms)
{
	//  in sequence  ***
	if (inSeq >= 0 && (ms - tiSeq > dtSeq || ms < tiSeq))
	{
		tiSeq = ms;

		if (seqWait)
		{	seqWait = 0;  // restore, dtSeqOwn-
			dtSeq = par.dtSeqDef;
		}
		const KC_Sequence& sq = set.seqs[inSeq];
		uint8_t code = sq.data[seqPos];
		uint usb = cKeyUsb[code];

		//  commands ___ execute
		bool isCmd = code >= K_Cmd0 && code <= K_CmdLast;
		if (isCmd)
		{
			int cmd = code - K_Cmd0;
			++seqPos;  if (!SeqEnd(sq))
			{
				int cp = sq.data[seqPos], cm = cp-128;
				++seqPos;  SeqEnd(sq);

				switch (cmd)
				{
				case CMD_SetDelay:  // ms
					dtSeq = cp;  // dtSeqOwn = par;
					break;

				case CMD_Wait:  // 0.1s
					dtSeq = 100*cp;  seqWait = 1;
					break;

				case CMD_Comment:  //  move until over next Cmt
					#define Cmt  K_Cmd0 + CMD_Comment
					seqPos += 2;
					while (!SeqEnd(sq) && sq.data[seqPos-2] != Cmt)
						++seqPos;
					break;

				case CMD_Hide:  // ignore
					break;

				case CMD_RunSeq:  // todo inSeq2 =
					break;

				//  _mouse commands_ execute
				case CM_x:  usb_mouse_move(cm,0, 0,0);  break;
				case CM_y:  usb_mouse_move(0,cm, 0,0);  break;

				case CM_BtnOn:   Mouse.press(parMBtn[cp]);  break;
				case CM_BtnOff:  Mouse.release(parMBtn[cp]);  break;

				case CM_Btn2x:  Mouse.click(parMBtn[cp]);
				case CM_Btn:  Mouse.click(parMBtn[cp]);  break;

				case CM_WhX:  Mouse.scroll(cm,0);  break;  // wheels
				case CM_WhY:  Mouse.scroll(0,cm);  break;

				case CM_xbig:  usb_mouse_move(cm*100,0, 0,0);  break;
				case CM_ybig:  usb_mouse_move(0,cm*100, 0,0);  break;

				case CM_xset:  xm=cm*100;  break;  // absolute pos
				case CM_yset:  ym=cm*100;  break;
				case CM_mset:  Mouse.moveTo(xm,ym);  break;
			}	}
		}
		//  todo hold any key-
		//  modifiers
		else if (code > KEY_NONE && code <= K_ModLast)
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
		if (seqRel >= 2 && !isCmd)
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
				uint8_t code = set.key[nLayer][id];
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
				{	switch (code)
					{
					case KM_Left:  Mouse_input_x -= 8;  break;  case KM_Right: Mouse_input_x += 8;  break;
					case KM_Up:    Mouse_input_y -= 8;  break;  case KM_Down:  Mouse_input_y += 8;  break;
					}
					k.layerOn = nLayer;
				}
				else if (code >= KM_WhlUp && code <= KM_WhlRight)
				{	switch (code)
					{
					case KM_WhlUp:    Mouse_wheel_y -= 1;  break;  case KM_WhlDown:  Mouse_wheel_y += 1;  break;
					case KM_WhlLeft:  Mouse_wheel_x -= 1;  break;  case KM_WhlRight: Mouse_wheel_x += 1;  break;
					}
					k.layerOn = nLayer;
				}
				else if (code >= KM_LMB && code <= KM_Forw)
				{
					uint8_t b = MOUSE_LEFT;
					switch (code)
					{  case KM_LMB:  break;
					case KM_RMB:  b = MOUSE_RIGHT;  break;  case KM_MMB:  b = MOUSE_MIDDLE;  break;
					case KM_Back: b = MOUSE_BACK;   break;  case KM_Forw: b = MOUSE_FORWARD;  break;
					}
					Mouse.press(b);
					k.layerOn = nLayer;
				}
				//  sequences  * * *
				else
				if (code >= K_Seq0 && code <= K_SeqLast
					&& inSeq < 0)
				{
					int8_t sq = code - K_Seq0;
					if (set.nseqs() > sq && sq < KC_MaxSeqs
						&& set.seqs[sq].len() > 0)
					{
						//  start seq  ***
						Keyboard.releaseAll();
						SeqModClear();
						inSeq = sq;  tiSeq = ms;  seqPos = 0;  seqRel = 0;
						dtSeq = par.dtSeqDef;  seqWait = 0;
					}
					else  inSeq = -1;
				}
			}
			else if (off)
			{
				//  send for layer it was pressed on
				uint8_t code = set.key[k.layerOn][id];
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
				{	switch (code)
					{
					case KM_Left:  Mouse_input_x += 8;  break;  case KM_Right: Mouse_input_x -= 8;  break;
					case KM_Up:    Mouse_input_y += 8;  break;  case KM_Down:  Mouse_input_y -= 8;  break;
					}
				}
				else if (code >= KM_WhlUp && code <= KM_WhlRight)
				{	switch (code)
					{
					case KM_WhlUp:    Mouse_wheel_y += 1;  break;  case KM_WhlDown:  Mouse_wheel_y -= 1;  break;
					case KM_WhlLeft:  Mouse_wheel_x += 1;  break;  case KM_WhlRight: Mouse_wheel_x -= 1;  break;
					}
					k.layerOn = nLayer;
				}
				else if (code >= KM_LMB && code <= KM_Forw)
				{
					uint8_t b = MOUSE_LEFT;
					switch (code)
					{	case KM_LMB:  break;
					case KM_RMB:  b = MOUSE_RIGHT;  break;  case KM_MMB:  b = MOUSE_MIDDLE;  break;
					case KM_Back: b = MOUSE_BACK;   break;  case KM_Forw: b = MOUSE_FORWARD;  break;
					}
					Mouse.release(b);
				}
			}
		}
	}

	//  mouse move and send  * * *

	Mouse_shift = KeyH(par.keyMouseSlow);
	usb_mouse_idle();
}
