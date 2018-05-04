#include "gui.h"
#include "kc_data.h"
#include "matrix.h"

extern KC_Main kc;


int8_t Gui::KeysSeq()
{
	bool layEd = kc.nLayer == par.editLayer;
	int q = seqId();
	KC_Sequence& sq = kc.set.seqs[q];
	int len = sq.len();
	std::vector<uint8_t>& dt = sq.data;

	//  toggle edit  ----
	if (kEnt && layEd && (!edit || layEd))
	{
		edit = 1-edit;
		if (edit)  // enter edit
		{
			//if (edpos > len)  // if
			edpos = len;  // end
		}
		return 1;
	}

	//  Edit sequence
	//--------------------------------------------------------------------------
	if (edit)
	{
		int cmd = -1;  // none

		if (layEd)
		{	if (!kSh)  // no shift
			{
				//  start, end
				if (kEnd < 0)  edpos = 0;
				if (kEnd > 0)  edpos = len;

				//  cmd check
				#define CmdAt(i)  (dt[i] >= K_Cmd0 && dt[i] <= K_CmdLast)
				bool cmdR = edpos > 1 && CmdAt(edpos-2);
				bool cmdL = edpos < len-1 && CmdAt(edpos);
				int cmdPar = cmdR ? edpos-1 : cmdL ? edpos+1 : 0;

				//  move, skip cmd  <,>
				if (kRight < 0)
				{	if (cmdR)  edpos-=2;  else
					if (edpos > 0)  --edpos;  }
				if (kRight > 0)
				{	if (cmdL)  edpos+=2;  else
					if (edpos < len)  ++edpos;  }

				//  edit cmd param  ^,v
				if (cmdPar && kUp)
				{
					dt[cmdPar] = RangeAdd(dt[cmdPar],
						(kCtrl ? 10 : 1) * -kUp, 0, 255);
				}

				//  del char key
				int8_t del = kDel || kBckSp ? 1 : 0;
				//  del 2 chars, if cmd
				if (kDel && edpos < len-1 && CmdAt(edpos))  del = 2;
				if (kBckSp && edpos > 1 && CmdAt(edpos-2))  del = 2;

				if (del)
				for (int d=0; d < del; ++d)
				if (len > 0)
				{
					int i = edpos;  // del>
					if (kBckSp)
					{	i = max(0, edpos-1);  // <del
						edpos = i;  //
					}
					for (; i < len-1; ++i)
						dt[i] = dt[i+1];
					dt.pop_back();
					--len;
					if (edpos > len)
						edpos = len;
				}
				if (kIns)
					edins = 1 - edins;  // ins/ovr

				//  commands ___ set, keys
				if (kDiv)   cmd = CMD_SetDelay;
				if (kMul)   cmd = CMD_Wait;
				if (kCopy)  cmd = CMD_Comment;
				if (kSwap)  cmd = CMD_Hide;
				//  todo manual pick key from list-
			}
			else  // shift
			{
				// _mouse commands_ set, keys
				if (kRight < 0)  cmd = CM_x;  // par = 10;
				if (kRight > 0)  cmd = CM_x;
				if (kUp > 0)  cmd = CM_y;
				if (kUp < 0)  cmd = CM_y;
				if (kEnd < 0)  cmd = CM_Btn;
				if (kEnd > 0)  cmd = CM_Btn2x;
				if (kPgUp < 0)  cmd = CM_BtnOff;
				if (kPgUp > 0)  cmd = CM_BtnOn;
				if (kDiv)  cmd = CM_WhX;
				if (kMul)  cmd = CM_WhY;
			}
		}
		if (cmd >= 0)
		{
			//  1 B cmd id, 1 B param
			//  commands use seq codes
			cmd += K_Cmd0;

			//  add command to sequence
			if (edpos >= len)  // at end
			{
				dt.push_back(cmd);  ++edpos;
				dt.push_back(0);  ++edpos;
			}
			else  // insert
			{
				dt.push_back(0);  ++len;
				dt.push_back(0);  ++len;

				for (int i=len-1; i > edpos; --i)
					dt[i] = dt[i-2];

				dt[edpos] = cmd;  ++edpos;
				dt[edpos] = 0;  ++edpos;
			}
		}
		else
		if (!layEd)
		{
			int ii = -1;  // none, find key
			for (uint i=0; i < ScanKeys; ++i)
			{
				if (Matrix_scanArray[i].state == KeyState_Press
					&& int(i) < kc.set.nkeys())
				{
					ii = kc.set.key[kc.nLayer][i];  //0.
					if (ii == KEY_NONE || ii >= KEYS_ALL)  ii = -1;  // norm only
					break;
				}
			}

			//  add key to sequence
			if (ii >= 0) {
			if (edpos >= len)  // at end
			{
				dt.push_back(ii);
				edpos++;
			}else
			if (edins)  // insert
			{
				dt.push_back(0);  ++len;
				for (int i=len-1; i > edpos; --i)
					dt[i] = dt[i-1];
				dt[edpos] = ii;
				edpos++;
			}
			else  // overwrite
			{	dt[edpos] = ii;
				if (edpos < len)  ++edpos;
			}
		}	}
	}else
	{	// View
		//------------------------------------------------------------
		//  save  load
		if (kSave)  Save();
		if (kLoad)  Load(kCtrl);

		//  move
		if (kUp > 0){	++slot;  slotMax();  }
		if (kUp < 0){	--slot;  slotMin();  }
		//  move 3
		if (kEnd > 0){	slot += 3;  slotMax();  }
		if (kEnd < 0){	slot -= 3;  slotMin();  }
		//  page
		if (kPgUp > 0)  pageInc();
		if (kPgUp < 0)  pageDec();

		//  copy
		if (kCopy)
		{	kc.set.copy.data = dt;  cpId = q;
			infType = 3;  tInfo = -1;
		}
		//  paste, set
		if (kPaste)
		{	dt = kc.set.copy.data;
			infType = 4;  tInfo = -1;
		}
		//  swap, xchg
		if (kSwap && cpId != -1)
		{
			KC_Sequence cp = kc.set.seqs[cpId];
			kc.set.seqs[cpId] = kc.set.seqs[q];
			kc.set.seqs[q] = cp;
			infType = 5;  tInfo = -1;
		}
		//  erase seq *
		if (kCtrl && kDel)
		{	sq.data.clear();
			sq.data.shrink_to_fit();  // free ram
		}

		if (kBack)
			--mlevel;
	}
	return 0;
}

void Gui::slotMax()
{
	if (slot >= iPage) {  slot = 0;  pageInc();  }
}
void Gui::slotMin()
{
	if (slot < 0) {  slot = iPage-1;  pageDec();  }
}
void Gui::pageInc()
{
	++page;  if (page >= kc.set.seqSlots/iPage)  page = 0;
}
void Gui::pageDec()
{
	--page;  if (page < 0)  page = kc.set.seqSlots/iPage-1;
}
