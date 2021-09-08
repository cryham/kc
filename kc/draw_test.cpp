#include "gui.h"
#include "Ada4_ST7735.h"
#include "matrix.h"
#include "kc_data.h"


//  Testing
//....................................................................................
void Gui::DrawTesting()
{
	//  menu
	if (mlevel == 1)
	{
		d->setClr(16,26,16);
		d->print(strMain[ym]);  d->setFont(0);

		DrawMenu(T_All,strTest, C_Test,RGB(22,31,14),RGB(5,9,6), 10, -1, 2);
		return;
	}
	char a[64];

	//  title
	d->setClr(12,20,28);
	d->print(strTest[yy]);
	d->setFont(0);
	d->setClr(21,26,31);

	switch (yy)
	{

	//-----------------------------------------------------
	case T_Matrix:
	{
		const static int16_t x1 = 10, y1 = 64;
		uint c,r, hc=99,hr=99;  //held col,row
		//const uint X = (W-x1) / (NumCols);
		const uint X = NumCols < 12 ? 9 : NumCols < 20 ? 8 : 7;

		//  grid #
		const uint16_t cl[3] = {RGB(9,10,10), RGB(13,14,14), RGB(16,20,20)};
		int w = NumCols*X;  if (w > W-1-x1) w = W-1-x1;
		for (r=0; r < NumRows; ++r)
			if (r % 2 == 1)
				d->drawFastHLine(x1, y1 + r*8 +4, w, cl[r % 4 == 3 ? 1 : 0]);
		for (c=0; c < NumCols; ++c)
			if (c % 2 == 1)
				d->drawFastVLine(x1 + c*X +3, y1, H-1-y1, cl[c%8==7 ? 2 : c%4==3 ? 1 : 0]);

		//  matrix  :::  *
		for (c=0; c < NumCols; ++c)
		for (r=0; r < NumRows; ++r)
		{
			d->setCursor(x1 + c*X, y1 + r*8);
			const KeyState& k = Matrix_scanArray[NumCols * r + c];

			a[1]=0;
			if (k.state == KeyState_Off)
			{	//sprintf(a,".");
				a[0]=2;
			}else{
				hc=c; hr=r;  // held
				a[0]='*';
				//sprintf(a,"%d", k.state);
				d->setClr(24,28,31);
			}
			d->print(a);
		}

		//  held  ---
		d->setCursor(0,26);
		d->setClr(24,24,31);
		sprintf(a,"Held %d  press %d ", cnt_press-cnt_rel, cnt_press);
		d->print(a);
		if  (hc<99 && hr<99)
		{
			d->setCursor(W/2, 6);
			sprintf(a,"R%d C%d", hr+1, hc+1);
			d->print(a);
		}

	}	break;
	}
}
