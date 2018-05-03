#include "gui.h"
#include "Ada4_ST7735.h"
#include "matrix.h"
#include "kc_data.h"

extern KC_Main kc;


//  Testing
//....................................................................................
void Gui::DrawTesting()
{
	//  menu
	if (mlevel == 1)
	{
		d->setTextColor(RGB(16,26,16));
		d->print(strMain[ym]);  d->setFont(0);

		DrawMenu(T_All,strTest, C_Test,RGB(22,31,14),RGB(5,9,6), 10, -1, 2);
		return;
	}
	char a[64];

	//  title
	d->setTextColor(RGB(12,20,28));
	d->print(strTest[yy]);
	d->setFont(0);
	d->setTextColor(RGB(21,26,31));

	switch (yy)
	{
	//-----------------------------------------------------
	case T_Pressed:
	{
		d->setCursor(0,32);
		//  layer  -
		d->setTextColor(RGB(20,23,31));
		sprintf(a,"Layer: %d", kc.nLayer);
		d->setTextColor(RGB(24,27,31));
		d->print(a);
		d->println("");  d->moveCursor(0,4);

		//  todo locks  -
		//d->setTextColor(RGB(18,21,24));
		//d->print("Locks: ");  // Num Caps Scrl
		//d->println("");  d->moveCursor(0,2);

		//  modifiers  -
		d->setTextColor(RGB(18,21,24));
		d->print("Modif:");
		d->setTextColor(RGB(20,25,28));
		for (uint i = 0; i < ScanKeys; ++i)
		{
			const KeyState& k = Matrix_scanArray[i];
			if (kc.set.nkeys() >= int(ScanKeys))
			if (k.state == KeyState_Hold)
			{
				uint8_t code = kc.set.key[kc.nLayer][i];
				if (code > KEY_NONE && code <= K_ModLast)
				{	sprintf(a," %s",cKeyStr[code]);
					d->print(a);
		}	}	}
		d->println("");  d->moveCursor(0,4);


		//  scan codes  - - -
		d->setTextColor(RGB(17,23,26));
		d->print("Scan:");
		d->setTextColor(RGB(15,20,25));
		int c = 0;
		for (uint i = 0; i < ScanKeys; ++i)
		{
			const KeyState& k = Matrix_scanArray[i];
			if (k.state == KeyState_Hold)
			{
				sprintf(a," %d",i);  // scan code
				d->print(a);  ++c;
		}	}
		d->println("");  d->moveCursor(0,4);


		//  keys  - - - -
		DrawPressed();

		//  held count
		d->setCursor(0, H-1-8);
		d->setTextColor(RGB(16+c, min(31,24+c), 31));
		sprintf(a,"Held: %d", c);
		d->print(a);

		if (ghost_cols)  // ghost
		{	d->setCursor(9*6, H-1-20);
			d->setTextColor(RGB(min(31,24+c), 18+c, 31));
			sprintf(a,"Ghost: %d %d", ghost_cols, ghost_rows);
			d->print(a);
		}
		//  press count -
		d->setCursor(9*6, H-1-8);
		d->setTextColor(RGB(21,21,27));
		sprintf(a,"Press: %d", cnt_press); //, cnt_hold % 1000);
		d->print(a);

	}	break;


	//-----------------------------------------------------
	case T_Layout:
	{
		d->setCursor(0,26);
		//  layer  -
		d->setTextColor(RGB(20,23,31));
		sprintf(a,"Layer: %d", kc.nLayer);
		d->print(a);
		d->setCursor(0,38);

		//  keys  - - - -
		DrawPressed();

		DrawLayout(false);
	}	break;


	//-----------------------------------------------------
	case T_Matrix:
	{
		//  matrix  :::
		const static int16_t x1 = 12, y1 = 64;
		uint c,r;

		bool ghost = false;
		for (c=0; c < NumCols; ++c)
		for (r=0; r < NumRows; ++r)
		{
			d->setCursor(x1 + c*8, y1 + r*8);
			const KeyState& k = Matrix_scanArray[NumCols * r + c];

			//  color from ghost, use
			#define CGh(gh,u)  d->setTextColor(gh ? RGB(31,26,12) : \
				RGB( min(31,7+u*5), min(31, 14+u*8), max(4, 24-u*2) ))

			a[1]=0;
			if (k.state == KeyState_Off)
			{	//sprintf(a,".");
				a[0]=249;
				bool gh = col_ghost[c] || row_ghost[r];
				int u = max(col_use[c], row_use[r]);
				CGh(gh, u);
				ghost |= gh;
			}else{
				a[0]='*';
				//sprintf(a,"%d", k.state);
				d->setTextColor(RGB(24,28,31));
			}
			d->print(a);
		}

		//  col| row- use
		for (c=0; c < NumCols; ++c)
		{
			CGh(col_ghost[c], col_use[c]);
			d->setCursor(x1 + c*8, y1 - 1*8);
			sprintf(a,"%d", col_use[c]);
			d->print(a);
		}
		for (r=0; r < NumRows; ++r)
		{
			CGh(row_ghost[r], row_use[r]);
			d->setCursor(0, y1 + r*8);
			sprintf(a,"%d", row_use[r]);
			d->print(a);
		}

		//  held  ---
		d->setCursor(0,26);
		d->setTextColor(RGB(24,24,31));
		sprintf(a,"Held %d  press %d", cnt_press-cnt_rel, cnt_press);
		d->print(a);

		//  ghosting  ---
		d->setCursor(0,40);
		d->setTextColor(ghost ? RGB(31,26,12) : RGB(16,21,26));
		sprintf(a,"Ghost col %d row %d", ghost_cols, ghost_rows);
		d->print(a);

	}	break;
	}
}

//  pressed keys list
//....................................................................................
void Gui::DrawPressed()
{
	int8_t seq=-1, fun=-1;
	d->setTextColor(RGB(20,25,28));
	d->print("Keys:");

	if (kc.set.nkeys() >= int(ScanKeys))
	for (uint i = 0; i < ScanKeys; ++i)
	{
		const KeyState& ks = Matrix_scanArray[i];
		if (ks.state == KeyState_Hold)
		{
			uint8_t k = kc.set.key[kc.nLayer][i];
			if (k > K_ModLast && k < KEYS_ALL_EXT/*KEYS_ALL*/)
			{
				const uint8_t* c = &cGrpRgb[cKeyGrp[k]][0][0];
				d->setTextColor(RGB(c[0],c[1],c[2]));
				d->print(" ");  d->print(cKeyStr[k]);

				if (k >= K_Seq0 && k <= K_SeqLast)  seq = k - K_Seq0;  else
				if (k >= K_Fun0 && k <= K_Fun9   )  fun = k - K_Fun0;
	}	}	}

	d->setCursor(0, d->getCursorY()+12);

	if (seq >= 0)  //  seq preview  ---
		DrawSeq(seq, 2);
	else
	if (fun >= 0)
		d->print(cFunStr[fun]);
}
