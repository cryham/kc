#include "gui.h"
#include "RamMonitor.h"
#include "matrix.h"
#include "kc_data.h"

#include "Ada4_ST7735.h"
#include "FreeSans9pt7b.h"
#include "TomThumb.h"

extern RamMonitor ram;


//  Draw End
//....................................................................................
void Gui::DrawEnd()
{
	DrawOperInfo();

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
			d->setClr(31,28,28);
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
			d->setClr(31,23,23);
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

	//  fps  ---------
	bool sc = ym == M_Setup && yy == S_Scan;
	if (demos.iFps == 2 ||
		(mlevel == 2 && (sc || (demos.iFps && ym == M_Demos))))
	{
		uint32_t ti = millis();
		float fr = 1000.f / (ti - oldti);
		int ff = fr;
		oldti = ti;

		d->setClr(24,28,31);
		d->setCursor(W-14,0);
		sprintf(a,"%d", ff);
		d->print(a);
	}

	//if (!offDisp)
		d->display();  // 58 Fps, 15ms @144MHz
}


//  draw utils
//....................................................................................
void Gui::DrawMenu(int cnt, const char** str, EFadeClr ec, uint16_t curClr,
	uint16_t bckClr, int16_t yadd, int16_t nextCol, int16_t numGap)
{
	const int16_t my = mlevel==0 ? ym : yy;
	const int16_t xw = 70, y1 = 32;  // par

	int16_t x=0, y=y1, c;
	for (int i=0; i < cnt; ++i)
	{
		d->setCursor(x,y);
		d->setColor(curClr);
		if (i == my)
			d->fillRect(x, y-1, x+W/2, 10, bckClr);
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

	d->setClr(
		max(mi, clr[0] - cmu[0] * mul / div),
		max(mi, clr[1] - cmu[1] * mul / div),
		max(mi, clr[2] - cmu[2] * mul / div) );
}
void Gui::FadeGrp(uint8_t g, const uint8_t mi, const uint8_t mul, const uint8_t div)
{
	const uint8_t* clr = &cGrpRgb[g][0][0];
	const uint8_t* cmu = &cGrpRgb[g][1][0];

	d->setClr(
		max(mi, clr[0] - cmu[0] * mul / div),
		max(mi, clr[1] - cmu[1] * mul / div),
		max(mi, clr[2] - cmu[2] * mul / div) );
}


//  key utils
//....................................................................................
void Gui::Chk_y1()
{
	if (ym1[ym] >= YM1[ym])  ym1[ym] = 0;
	if (ym1[ym] < 0)  ym1[ym] = YM1[ym]-1;
}

//  returns pressed scan id or -1
int Gui::PressKey(int8_t& pressKey)
{
	if (!pressKey)  return -1;

	int c = 0, ii = -1;
	for (uint i=0; i < ScanKeys; ++i)
	{	if (Matrix_scanArray[i].state != KeyState_Off)  ++c;
		if (Matrix_scanArray[i].state == KeyState_Press)  ii = i;
	}
	if (pressKey == 1)
	{	//  wait until all keys are off
		if (c == 0)
			pressKey = 2;
		return -1;
	}
	else if (pressKey == 2 && ii != -1)
		return ii;

	return -1;
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

//  value add, range
int16_t Gui::RangeAdd(int16_t val, int16_t add, int16_t vmin, int16_t vmax, int8_t cycle)
{
	int16_t v = val;
	if (cycle)
	{	int16_t r = vmax - vmin + 1;
		v = (v + add + r) % r;
	}else
	{	v += add;
		v = min(vmax, max(vmin, v));
	}
	return v;
}

//  save
void Gui::Save()
{
	kc.Save();  infType = 2;  tInfo = -1;
}
//  load
void Gui::Load(int8_t reset)
{
	if (reset)
	{	kc.set.InitCK();  infType = 0;  }
	else
	{	kc.Load();  infType = 1;  }  tInfo = -1;
}
