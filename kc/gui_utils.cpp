#include "gui.h"
#include "RamMonitor.h"
#include "matrix.h"

#include "Ada4_ST7735.h"
#include "FreeSans9pt7b.h"
#include "TomThumb.h"

extern RamMonitor ram;
extern KC_Main kc;


//  menu colors
const uint8_t Gui::Mclr[Gui::Cl][2][3] = {
	{{16,26,31},{4,3,1}},  // 0 main
	{{27,26,31},{2,3,1}},  // 1 demos
	{{20,30,26},{6,3,4}},  //  2 test
	{{22,31,18},{6,3,7}},  //  3 map
	{{17,31,31},{5,4,3}},  //  4 seqs
	{{31,31,21},{1,3,5}},  // 5 display
	{{26,28,28},{4,3,3}},  // 6 scan, mouse-
};


//  Draw End
//....................................................................................
void Gui::DrawEnd()
{
	char a[64];
	//  ram info  ---------
	if (iRam)
	{
		int16_t	x = W-1 - (iRam == 2 ? 18*4 : 8*6),
				y = H-1 - (iRam == 2 ? 5*6 : 4*9);
		d->fillRect(x-1,y-1, W-x+1,H-y+1, RGB(3,2,2));
		d->setCursor(x,y);
		std::vector<uint8_t> vv;

		if (iRam == 2)
		{	//  long
			d->setTextColor(RGB(31,28,28));
			d->setFont(&TomThumb);  // 3x5
			sprintf(a,"%s %d %d", ram.warning_crash() ? "crsh" : ram.warning_lowmem() ? "low" : "ok",
					kc.GetSize(), sizeof(vv));
			d->println(a);  y+=6;
			d->setCursor(x, y);  y+=6;
			sprintf(a,"un  %lu %lu",   // space between heap and stack
					ram.adj_unallocd(), ram.unallocated());  d->println(a);
			d->setCursor(x, y);  y+=6;
			sprintf(a,"fr  %ld %ld", //100 * ram.adj_free() / ram.total(),
					ram.adj_free(), ram.free());  d->println(a);
			d->setCursor(x, y);  y+=6;
			sprintf(a,"hp  %lu%%  %lu %lu", 100 * ram.heap_free() / ram.heap_total(),
					ram.heap_free(), ram.heap_total());  d->println(a);
			d->setCursor(x, y);  y+=6;
			sprintf(a,"st  %lu%%  %lu %lu", 100 * ram.stack_free() / ram.stack_total(),
					ram.stack_free(), ram.stack_total());  d->println(a);
			d->setFont(0);
		}else
		{	//  short
			d->setTextColor(RGB(31,23,23));
			sprintf(a,"%s %d", ram.warning_crash() ? "c" : ram.warning_lowmem() ? "l" : "o",
					kc.GetSize());
			d->println(a);  y+=9;
			d->setCursor(x, y);  y+=9;
			sprintf(a,"un %ld", ram.adj_unallocd());  d->println(a);
			d->setCursor(x, y);  y+=9;
			sprintf(a,"hp %lu", ram.heap_free());  d->println(a);
			d->setCursor(x, y);  y+=9;
			sprintf(a,"st %lu", ram.stack_free());  d->println(a);
		}
	}

	#ifdef DEMOS
	//  fps  ---------
	if (mlevel == 2)
	if ((demos.iFps && ym == M_Demos) ||
		(ym == M_Testing && yy == T_ScanSetup))
	{
		uint32_t ti = millis();
		float fr = 1000.f / (ti - oldti);
		int ff = fr;
		oldti = ti;

		d->setTextColor(RGB(24,28,31));
		d->setCursor(W-14,0);
		sprintf(a,"%d", ff);
		d->print(a);

		d->setTextColor(RGB(20,26,31));
		d->setCursor(W-14,9);
		sprintf(a,"%lu", tdemo);
		d->print(a);
	}
	#endif

	//if (!offDisp)
		d->display();  // 58 Fps, 15ms @144MHz
}


//  draw utils
//....................................................................................
void Gui::DrawMenu(int cnt, const char** str, EFadeClr ec, uint16_t curClr,
					int16_t yadd, int16_t nextCol, int16_t numGap)
{
	const int16_t my = mlevel==0 ? ym : yy;
	const int16_t xw = 70, y1 = 32;  // par

	int16_t x=0, y=y1, c;
	for (int i=0; i < cnt; ++i)
	{
		d->setCursor(x,y);
		d->setTextColor(curClr);
		d->print(i == my ? " \x10 ":"   ");  // >

		c = abs(i - my);  // dist dim
		FadeClr(ec, 4, c, 1);
		d->print(str[i]);

		//  next, extras
		y += yadd;

		if (i == numGap)  y += 4;

		if (i+1 == nextCol)
		{	x += xw;  y = y1;  }
	}
}

void Gui::FadeClr(EFadeClr ec, const uint8_t mi, const uint8_t mul, const uint8_t div)
{
	const uint8_t* clr = &Mclr[ec][0][0];
	const uint8_t* cmu = &Mclr[ec][1][0];

	d->setTextColor(RGB(
		max(mi, clr[0] - cmu[0] * mul / div),
		max(mi, clr[1] - cmu[1] * mul / div),
		max(mi, clr[2] - cmu[2] * mul / div) ));
}
void Gui::FadeGrp(uint8_t g, const uint8_t mi, const uint8_t mul, const uint8_t div)
{
	const uint8_t* clr = &cGrpRgb[g][0][0];
	const uint8_t* cmu = &cGrpRgb[g][1][0];

	d->setTextColor(RGB(
		max(mi, clr[0] - cmu[0] * mul / div),
		max(mi, clr[1] - cmu[1] * mul / div),
		max(mi, clr[2] - cmu[2] * mul / div) ));
}


//  key utils
void Gui::Chk_y1()
{
	if (ym1[ym] >= YM1[ym])  ym1[ym] = 0;
	if (ym1[ym] < 0)  ym1[ym] = YM1[ym]-1;
}

//  key auto repeat,  row, col, ms
int8_t Gui::kr(uint8_t sc, uint16_t dt)
{
	KeyPosition st = Matrix_scanArray[sc].state;
	int16_t& m = Matrix_scanArray[sc].autoRepeat;

	if (st == KeyState_Press)
	{
		m = -1;  // start delay
		return 1;
	}
	else if (st == KeyState_Hold)
	{
		if (m < 0)
		{	m -= dt;
			if (m < -par.krDelay*5)  // delay ms
			{	m = 0;  return 1;  }
		}else
		{	m += dt;
			if (m > par.krRepeat*5)  // repeat freq
			{	m = 0;  return 1;  }
		}
	}
	return 0;
}
