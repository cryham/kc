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

	//  1 minute time, stats
	if (par.time1min)
	if (ms - tm_min1 > 1000 * 6 * par.time1min || ms < tm_min1)
	{
		tm_min1 = ms;
		min1_Keys = cnt_press1min * 60 / (6 * par.time1min);
		cnt_press1min = 0;

	#ifdef GRAPHS
		//  graph inc pos
		++grPpos;
		if (grPpos >= W)  grPpos = 0;
		//  add to graph
		grPMin[grPpos] = min1_Keys > 255 ? 255 : min1_Keys;
	#endif
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
		bool fun = codeL >= K_Fun0 && codeL <= K_FunLast;

		if (on || off)
		{
			//  get from kc
			uint8_t code0 = set.key[0][id];
			uint8_t codeL = set.key[nLayer][id];

			if (code0 >= K_Layer1 && code0 < K_Layer1+KC_MaxLayers)
			{
				int8_t lay = code0 - K_Layer1 + 1;

				if (on)
					msKeyLay = ms;
				else
				if (off &&
					(ms - msKeyLay < par.msLLTapMax*10 || ms < msKeyLay ||
					(ms - msKeyLay > par.msLLHoldMin*100 && par.msLLHoldMin > 0)))
				{
					if (nLayerLock == lay)
						nLayerLock = -1;  // unlock
					else
						nLayerLock = lay;  // lock, set sticky
				}

				//  set layer, hold  ------
				if (on)   nLayer = lay;
				else  // default layer
				if (off)  nLayer =
					nLayerLock >=0 ? nLayerLock : par.defLayer;

				if (nLayer >= KC_MaxLayers-1)
					nLayer = KC_MaxLayers-1;
			}
			else
			//  display, internal functions  ***
			//..........................................................
			if (on && fun)
			{
				switch (codeL)
				{
				case K_Fun10:  // send, Gui toggle
					gui.kbdSend = 1 - gui.kbdSend;  QuitSeq();
					setDac = 1;  break;

				case K_Fun1:
				case K_Fun2:  // brightness -+
					tiFun = ms;  break;  // delay no par

				case K_Fun3:  // soft reset  //NVIC_SystemReset();
					#define SCB_AIRCR (*(volatile uint32_t *)0xE000ED0C)
					SCB_AIRCR = 0x05FA0004;  break;

				case K_Fun4:  // light, led toggle
					#ifdef LED
					gui.led = 1 - gui.led;
					digitalWrite(LED, gui.led ? LOW : HIGH);
					#endif
					break;

				case K_Fun5:  // quit seq, stop repeat
					QuitSeq(0);
					break;

				case K_Fun6:  // reset stats
					gui.tm_on = rtc_get();
					cnt_press = 0;
					cnt_press1min = 0;
					break;

				case K_Fun7:  // dec,inc default layer
					if (par.defLayer > 0)
						--par.defLayer;
					break;
				case K_Fun8:
					if (par.defLayer < KC_MaxLayers-1)
						++par.defLayer;
					break;

				case K_FunLast:  // un/lock layer
					if (nLayerLock >= 0)
						nLayerLock = -1;  // unlock
					else
					if (nLayer > 0)  // lock to current
						nLayerLock = nLayer;
					break;

				//todo new funct
				//case K_Fun0, K_Fun11, K_Fun12, K_Fun13, K_Fun14, K_Fun15, K_Fun16, K_Fun17,
				}
			}
			/*else
			//  quit seq, layer lock etc
			if (on && (id == gEsc || id == gSpc))
			{
				nLayerLock = -1;
				if (inSeq[0] >= 0)
					QuitSeq(0);
			}*/
		}
		else if (hold)  // repeat, funct
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

//  seq end check
bool KC_Main::SeqEnd(int lev, const KC_Sequence& sq)
{
	if (seqPos[lev] >= sq.len())
	{
		Keyboard.releaseAll();
		SeqModClear();
		inSeq[lev] = -1;  // end
		return true;
	}
	return false;
}

//  Quit running Sequence on enter Gui
void KC_Main::QuitSeq(int8_t chk)
{
	if (chk && gui.kbdSend)  return;
	if (inSeq[0] < 0 && inSeq[1] < 0)  return;
	inSeq[0] = -1;  inSeq[1] = -1;
	Keyboard.releaseAll();
	SeqModClear();
}

//  const
const static uint8_t parMBtn[6]={
		MOUSE_LEFT, MOUSE_MIDDLE, MOUSE_RIGHT, MOUSE_BACK, MOUSE_FORWARD};


//  keyboard send  (to pc usb)
//------------------------------------------------------------------------------------------------
void KC_Main::Send(uint32_t ms)
{
	//  in sequence  ***
	int lev = -1;  // level, none
	if (inSeq[1] >= 0)  lev = 1;  else  // from seq
	if (inSeq[0] >= 0)  lev = 0;  // base, from key

	if (lev >= 0 && (ms - tiSeq > dtSeq || ms < tiSeq))
	{
		tiSeq = ms;

		if (seqWait)
		{	seqWait = 0;  // restore, dtSeqOwn-
			dtSeq = par.dtSeqDef;
		}
		const int8_t isq = inSeq[lev];
		const KC_Sequence& sq = set.seqs[isq];
		int16_t& sp = seqPos[lev];  int8_t& sr = seqRel[lev];
		uint8_t code = sq.data[sp];

		uint usb = cKeyUsb[code];

		//  commands ___ execute
		bool isCmd = code >= K_Cmd0 && code <= K_CmdLast;
		if (isCmd)
		{
			int cmd = code - K_Cmd0;
			++sp;  if (!SeqEnd(lev,sq))
			{
				int cp = sq.data[sp], cm = cp-128;  // par
				++sp;  SeqEnd(lev,sq);

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
					sp += 2;
					while (!SeqEnd(lev,sq) && sq.data[sp-2] != Cmt)
						++sp;
					break;

				case CMD_Hide:  // ignore
					break;

				case CMD_RunSeq:  //  run seq from seq, next level
					//  lev 1 max, not self
					if (lev == 0 && cp != inSeq[0])
					{
						inSeq[1] = cp;
						seqPos[1]=0;  seqRel[1]=0;
					}
					break;

				case CMD_Repeat:  // repeat
					Keyboard.releaseAll();
					SeqModClear();
					inSeq[lev] = isq;
					//sp = 0;  sr = 0;
					seqPos[0]=0;  seqRel[0]=0;
					seqPos[1]=0;  seqRel[1]=0;
					tiSeq = ms;  dtSeq = par.dtSeqDef;  seqWait = 0;
					break;

				//  _mouse commands_ execute
				case CM_x:  usb_mouse_move(cm,0, 0,0);  break;
				case CM_y:  usb_mouse_move(0,cm, 0,0);  break;

				case CM_BtnOn:   Mouse.press(parMBtn[cp]);  break;
				case CM_BtnOff:  Mouse.release(parMBtn[cp]);  break;

				case CM_Btn2x:  Mouse.click(parMBtn[cp]);
				case CM_Btn:  Mouse.click(parMBtn[cp]);  break;

				case CM_WhX:  Mouse.scroll(0,cm);  break;  // wheels
				case CM_WhY:  Mouse.scroll(-cm,0);  break;

				case CM_xbig:  usb_mouse_move(cm*100,0, 0,0);  break;
				case CM_ybig:  usb_mouse_move(0,cm*100, 0,0);  break;

				case CM_xset:  xm=cm*100;  break;  // absolute pos
				case CM_yset:  ym=cm*100;  break;
				case CM_mset:
					Mouse.moveTo(xm,ym);  break;
			}	}
		}
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
			sr = 2;  // next key
		}
		//  keys
		else if (sr == 0)
		{
			Keyboard.press(usb);
			Keyboard.send_now();
			++sr;  // next step
		}
		else if (sr == 1)
		{
			Keyboard.release(usb);
			Keyboard.send_now();
			++sr;
		}
		//  both
		if (sr >= 2 && !isCmd)
		{
			sr = 0;
			++sp;  // next seq byte
			SeqEnd(lev,sq);
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


					//  key press time, for stats  ---
					tm_keyOld = tm_key;
					tm_key = rtc_get();

					//  prev inactive times
					if (tm_key - tm_keyOld > 60 * par.minInactive)
					{
						tm_keyAct = tm_key;  // reset active start
						tInact2 = tInact1;
						tInact1 = (tm_key - tm_keyOld) / 60;
						tInactSum += tInact1;
					}
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
					&& inSeq[0] < 0)
				{
					int8_t sq = code - K_Seq0;
					if (set.nseqs() > sq && sq < KC_MaxSeqs
						&& set.seqs[sq].len() > 0)
					{
						//  start seq  ***
						Keyboard.releaseAll();
						SeqModClear();
						inSeq[0] = sq;  inSeq[1] = -1;
						seqPos[0]=0;  seqRel[0]=0;
						seqPos[1]=0;  seqRel[1]=0;
						tiSeq = ms;  dtSeq = par.dtSeqDef;  seqWait = 0;
					}
					else  inSeq[0] = -1;
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
