#include "gui.h"
#include "kc_data.h"
#include "matrix.h"

extern KC_Main kc;


int8_t Gui::KeysSeq()
{
	bool lay2 = kc.nLayer == 2;
	int q = seqId();
	int len = kc.set.seqs[q].len();
	std::vector<uint8_t>& dt = kc.set.seqs[q].data;

	//  toggle edit  ----
	if (kEnt && (!edit || lay2))
	{
		edit = 1-edit;
		if (edit)  // enter edit
		{
			//if (edpos > len)  // if
			edpos = len;  // end
		}
		return 1;
	}

	if (edit)
	{	//  edit sequence  ----
		//uint8_t fun = 0;  // none
		if (lay2)  //` switch..
		{	//  move cursor
			if (kEnd < 0)  edpos = 0;
			if (kEnd > 0)  edpos = len;

			if (kRight < 0)  if (edpos > 0)  --edpos;
			if (kRight > 0)  if (edpos < len)  ++edpos;

			if ((kDel || kBckSp) && len > 0)
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
			//if (kEnt)  SeqClear(q);  // erase

			//if (key(F1))  fun = 1;  // set delay cmd todo
			//if (key(F2))  fun = 2;  // wait cmd
		}
		if (!lay2)  // || fun > 0)
		{
			// manual pick list?
			//  find key, if none
			int ii = -1;
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
			if (ii >= 0) {  // && edpos < iSeqLen-1)
			if (edpos >= len)  // at end
			{
				dt.push_back(ii);
				edpos++;  //len++;
			}else
			if (edins)  // insert
			{
				dt.push_back(0);  ++len;
				for (int i=len-1; i > edpos; --i)
					dt[i] = dt[i-1];
				dt[edpos] = ii;
				edpos++;  //len++;
			}
			else  // overwrite
			{	dt[edpos] = ii;
				if (edpos < len)  ++edpos;
			}
		}	}
	}else
	{	//  save  load
		if (kSave)  Save();
		if (kLoad)  Load(kCtrl);

		if (kUp > 0)  // move
		{	++slot;  if (slot >= iPage) {  slot = 0;
			++page;  if (page >= kc.set.seqSlots/iPage)  page = 0;
		}	}
		if (kUp < 0)
		{	--slot;  if (slot < 0) {  slot = iPage-1;
			--page;  if (page < 0)  page = kc.set.seqSlots/iPage-1;
		}	}
		if (kPgUp > 0)  // page
		{	++page;  if (page >= kc.set.seqSlots/iPage)  page = 0;
		}
		if (kPgUp < 0)
		{	--page;  if (page < 0)  page = kc.set.seqSlots/iPage-1;
		}

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
		}
		if (kBack)
			--mlevel;
	}
	return 0;
}
