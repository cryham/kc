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
//....................................................................................
void Demos::Init(Ada4_ST7735* tft)
{
	fps = 0;
	ti = 0;  oti = 0;
	d = tft;  if (d)  data = d->getBuffer();
	
#ifdef DEMOS
	iPrev = -1;  // params
	iInfo = 0;  iInfoOff = 1;

	einit = INone;
	sCnt = sMax/2;  sVel = 10;  //16 stars
	bCnt = min(200,bMax);  bSpd = 100;  bSpRnd = 1;  bRad = 3;  // balls

	r1Int = 2;  r1Size = 2;  // rain
	r2Int = 3;  r2Size = 3;  rCur = 0;
	
	fInt = 5;  fWave = 1;  // fountain

	ngt = 0;  ngCur = ngMin;  ngRot = 0;  // ngons
	hdtOn = 1;
	hdt = 0;  hdCur = hdA-1;  hdRot = 0;  // hedrons

	ckCur = 0;	ckSpeed = 6;  // logo

	plasma = 1;  t = 131210;
	for (int i=0; i < num_plasma; ++i)
		tadd[i]=5;

	twv = 7;
#endif
}


#if 0  ///--
void Demos::Draw(D d, int8_t menu, int8_t y, int8_t y2)
{
	if (iInfo < 0)
		iInfo = iInfoOff == iOff ? 0 :
				60 + iInfoOff*60;  //par
	if (iInfo > 0)
		--iInfo;
#endif


#ifdef DEMOS
//  Key Press  demo params
//....................................................................................
void Demos::KeyPress(EDemo demo, int k, int e, int ct, int kinf)
{
	int sh = 0; //, ct = 0;
	int sp = sh ? 2 : 10;

	//  change
//	if (key(PAGE_UP  ))  k = -1;
//	if (key(HOME))  e = -1;

	//  global
//	if (key(R))
//	{	Init(0);  return;  }  // reset all

	//  info txt
	if (kinf)
		iInfo = 1 - iInfo;
	/*if (ct)
		iInfoOff = (iInfoOff + 1) % (iOff+1);  // mode,off
	else
	{	if (iInfoOff == iOff)  iInfo = 40;
		else
		if (iInfo)  iInfo = 0;
		else  iInfo = -1;
		return;  //  show/hide
	}*/
	
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

		case D_Wave:
			if (k)  twv += k;
			if (e)  twv += e*10;
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
