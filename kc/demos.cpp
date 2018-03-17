#include "demos.h"

//#include "gui.h"  // menu enum, ym etc
//extern "C" {
//	#include <scan_loop.h>  // keys kk
//	#include <usb_hid->h>  // key_defs
//}

//  Main
Demos::Demos()
{
	Init(0);
}
void Demos::Init(Ada4_ST7735* tft)
{
	fps = 0;
	ti = 0;  oti = 0;
	d = tft;  if (d)  data = d->getBuffer();
	
#ifdef DEMOS
	iPrev = -1;  // params
	iInfo = 1;  iInfoOff = 1;

	einit = INone;
	sCnt = 80;  sVel = 8;  // stars
	bCnt = 80;  bSpd = 120;  bSpRnd = 1;  bRad = 3;  // balls

	r1Int = 0;  r1Size = 0;  // rain
	r2Int = 0;  r2Size = 0;  rCur = 0;
	
	fInt = 5;  fWave = 1;  // fountain

	ngt = 0;  ngCur = ngMin;  ngRot = 0;  // ngons
	hdtOn = 1;
	hdt = 0;  hdCur = hdA-1;  hdRot = 0;  // hedrons

	ckCur = 0;	ckSpeed = 6;  // logo

	plasma = 1;  t = 31210;
	tadd[0]=2; tadd[1]=5; tadd[2]=3; tadd[3]=5; tadd[4]=5; tadd[5]=5;
#endif
}


#if 0  ///--
//  Draw
//....................................................................................
void Demos::Draw(D d, int8_t menu, int8_t y, int8_t y2)
{
#ifdef DEMOS
	#define clD  d->clearDisplay();
	if (menu)
	{
		if (iInfo < 0)
			iInfo = iInfoOff == iOff ? 0 :
					60 + iInfoOff*60;  //par
		if (iInfo > 0)
			--iInfo;

		d->setTextColor(WHITE,BLACK);  // backgr
		if (y == MDemos)
		{
			if (iPrev != y2)  //  Init on change
			switch (y2)
			{
				case 0:  einit = INone;  break;  // Stars
				case 1:  einit = INone;  break;

				case 4:  ngtOn = 1;  break;  // Ngons
				case 5:  hdtOn = 1;  break;  // Hedrons
			}
			switch (y2)
			{
				case 0: clD  Space(d);  break;
				case 1: clD  Balls(d);  break;

				case 2:		 Rain(d);  break;
				case 3: clD  Fountain(d);  break;
				
				case 4: clD  Ngons(d);  break;
				case 5: clD  Hedrons(d);  break;
			}
			iPrev = y2;
		}
		else if (y == MText)
		switch (y2)
		{
			case 0: clD  CK_logo(d);  break;
			case 1: clD  Font_ver(d, false);  break;
			case 2: clD  Chars(d,0);  break;
			case 3: clD  Chars(d,1);  break;
		}
		else if (y == MPlasma)
		switch (y2)
		{
			case 4: Plasma4(d->getBuffer());  break;
			case 3: Plasma3(d->getBuffer());  break;
			case 2: Plasma2(d->getBuffer());  break;
			case 1: Plasma1(d->getBuffer());  break;
			case 0: Plasma0(d->getBuffer());  break;
		}
		d->setTextColor(WHITE);  // transp
	}
#endif

	if (fps)  /// fps
	{
		oti = ti;  ti = micros();
		d->setFont(0);
		d->setCursor(0,0);
		d->setTextColor(WHITE,BLACK);  // backgr
		d->println(int16_t(1000000.f/(ti-oti)));
		d->setTextColor(WHITE);  // transp
	}
}
#endif


#ifdef DEMOS
//  Key Press  demo params
//....................................................................................
void Demos::KeyPress(EDemo demo, int k, int e, int ct, int kinf)
{
//	int k = 0, e = 0;
//	int sh = kk[KEY_RSHIFT], ct = kk[KEY_RCTRL];
	int sh = 0; //, ct = 0;
	int sp = sh ? 2 : 10;

	//  change
//	if (key(PAGE_UP  ))  k = -1;
//	if (key(PAGE_DOWN))  k =  1;
//	if (key(HOME))  e = -1;
//	if (key(END ))  e =  1;

	//  global
//	if (key(R))
//	{
//		Init(0);  return;  //  reset all
//	}

	//  info txt
	if (kinf) //keyp(MINUS) || key(ESC))
//	if (ct)
//		iInfoOff = (iInfoOff + 1) % (iOff+1);  // mode,off
//	else
	{	if (iInfoOff == iOff)  iInfo = 40;
		else
		if (iInfo)  iInfo = 0;
		else  iInfo = -1;
		return;  //  show/hide
	}
	
	if (/*demo &&*/ (k || e))
	{	//iInfo = -1;

		switch (demo)
		{
		case D_Space:
			if (k){  sCnt += k*sp;  sCnt = max(0, min(sMax, sCnt));  einit = INone;  }
			if (e){  sVel += e;  sVel = max(0, min(40, sVel));  einit = INone;  }
			break;
			
		case D_Balls:
			if (ct)
			{	if (k){  bRad += k;  bRad = max(1, min(5, bRad));  einit = INone;  }
				if (e){  bSpRnd += e;  bSpRnd = max(0, min(bSpRMax, bSpRnd));  einit = INone;  }
			}else
			{	if (k){  bCnt += k*sp;  bCnt = max(0, min(bMax, bCnt));  einit = INone;  }
				if (e){  bSpd += e*sp*2;  bSpd = max(0, min(400, bSpd));  einit = INone;  }
			}break;

		case D_Rain:
			if (!ct){  if (!rCur)
			{	if (k){  r1Int += k;  if (r1Int < -6)  r1Int = -6;  }
				if (e)   r1Size += e;  r1Size = max(0, min(4, r1Size));
			}else{
				if (k){  r2Int += k;  if (r2Int < -2)  r2Int = -2;  }
				if (e)   r2Size += e;  r2Size = max(0, min(4, r2Size));
			}	}
			if (ct && k)  rCur = 1-rCur;
			break;

		case D_Fountain:
			if (k)  fInt = (fInt + k + 12) % 12;
			if (e)  fWave += e;
			break;

		case D_Ngons:
			if (k)
			{	ngtOn = 0;  // manual
				ngCur += k;
				if (ngCur < ngMin)  ngCur = ngMax;
				if (ngCur > ngMax)  ngCur = ngMin;  }
			if (e)
			{	++ngRot;
				if (ngRot >= ngRotMax)  ngRot = 0;  }
			break;
			
		case D_Hedrons:
			if (k)
			{	hdtOn = 0;  // manual
				hdCur = (hdCur + k + hdA) % hdA;  }
			if (e)
				hdRot = (hdRot + e + hdRotMax) % hdRotMax;
			break;

		case D_CK_Logo:
			if (k)  ckSpeed += k;
			if (e)  ckCur = (ckCur + e + ckMax) % ckMax;
			break;

		case D_Plasma:
			if (k){  plasma += k; if (plasma < 0) plasma = num_plasma-1;  if (plasma >= num_plasma) plasma = 0;  }
			if (e)  PlasmaT(e);
			break;

		default:
			break;
		}
	}
}
#endif

#if 0
//  display
void Demos::KeyGlob(D d)
{
	if (key(F) || keyp(PLUS))
		fps = 1-fps;

	if (key(SPACE) || key(D))
	{
		dim = 1-dim;
		d->dim(dim);
	}
}

void Demos::Reset(D d)
{
	d->begin(SSD1306_SWITCHCAPVCC);
	d->clearDisplay();
	d->dim(dim);
	d->display();
	d->setTextColor(WHITE);
}
#endif
