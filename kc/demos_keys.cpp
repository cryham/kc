#include "demos.h"
#include "Ada4_ST7735.h"
#include "gui.h"  // keys


//  Main
Demos::Demos()
{
	Init(0);
}
//....................................................................................
void Demos::Init(Ada4_ST7735* tft)
{
	d = tft;  if (d)  data = d->getBuffer();
	ti = 0;  oti = 0;
	
#ifdef DEMOS  // params
	iFps = 0;  iInfo = 0;  //iPrev = -1;  iInfoOff = 1;

	einit = INone;
	sCnt = 3*sMax/4;  sVel = 10;  //16 stars
	bCnt = min(200,bMax);  bSpd = 100;  bSpRnd = 1;  bRad = 3;  // balls

	r1Int = 2;  r1Size = 2;  // rain
	r2Int = -1;  r2Size = 2;  rCur = 1;
	
	fInt = 5;  fWave = 1;  // fountain

	ngt = 0;  ngCur = 12;  ngRot = 0;  // ngons
	hdtOn = 1;
	hdt = 0;  hdCur = hdA-5;  hdRot = 0;
	hdSpd = 1;  hdDiag = 1;  // hedrons

	ckCur = 0;  ckSpeed = 6;  // logo

	plasma = 2;  t = 3210;
	tadd[0]=7; tadd[1]=5; tadd[2]=8; tadd[3]=4; tadd[4]=6; tadd[5]=9;  tadd[6]=8;

	twv = 6;  // wave
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
void Demos::KeyPress(EDemo demo, Gui* gui)
{
	//  global
	//if (key(R)){  Init(0);  return;  }  // reset all

	//  fps, info txt
	if (gui->kInf){  iInfo = 1 - iInfo;  return;  }
	if (gui->kFps){  iFps = 1 - iFps;   return;  }

	int8_t k = gui->kRight, u = -gui->kUp,
		pgup = gui->kPgUp, end = gui->kEnd, ct = gui->kCtrl;
	int sp = gui->kSh ? 2 : 10;
	
	if (k || u || pgup || end)
	{	//iInfo = -1;
		switch (demo)
		{
		//  full  --------
		case D_Plasma:
			if (k){  plasma += k; if (plasma < 0) plasma = num_plasma-1;  if (plasma >= num_plasma) plasma = 0;  }
			if (u)  PlasmaT(u);
			break;

		case D_Wave:
			if (k)  twv += k;
			if (u)  twv += u*10;
			break;

		case D_Hedrons:  // 3d
			if (pgup)
				hdDiag = (hdDiag + pgup + hdDiagMax) % hdDiagMax;
			if (end)
				hdSpd += end;
			if (k)
			{	hdtOn = 0;  // manual
				hdCur = (hdCur + k + hdA) % hdA;  }
			if (u)
				hdRot = (hdRot + u + hdRotMax) % hdRotMax;
			break;

		case D_Ngons:
			if (k)
			{	ngtOn = 0;  // manual
				ngCur += k;
				if (ngCur < ngMin)  ngCur = ngMax;
				if (ngCur > ngMax)  ngCur = ngMin;  }
			if (u)
			{	++ngRot;
				if (ngRot >= ngRotMax)  ngRot = 0;  }
			break;

		//  old  --------
		case D_Space:
			if (k){  sCnt += k*sp;  sCnt = max(0, min(sMax, sCnt));  einit = INone;  }
			if (u){  sVel += u;     sVel = max(0, min(40, sVel));  einit = INone;  }
			break;
			
		case D_Balls:
			if (ct)
			{	if (k){  bRad += k;      bRad = max(1, min(5, bRad));  einit = INone;  }
				if (u){  bSpRnd += u;  bSpRnd = max(0, min(bSpRMax, bSpRnd));  einit = INone;  }
			}else
			{	if (k){  bCnt += k*sp;    bCnt = max(0, min(bMax, bCnt));  einit = INone;  }
				if (u){  bSpd += u*sp*2;  bSpd = max(0, min(400, bSpd));  einit = INone;  }
			}break;

		case D_Rain:
			if (!ct){  if (!rCur)
			{	if (k){  r1Int += k;  if (r1Int < -6)  r1Int = -6;  }
				if (u)   r1Size += u;  r1Size = max(0, min(4, r1Size));
			}else{
				if (k){  r2Int += k;  if (r2Int < -2)  r2Int = -2;  }
				if (u)   r2Size += u;  r2Size = max(0, min(4, r2Size));
			}	}
			if (ct && k)  rCur = 1-rCur;
			break;

		case D_Fountain:
			if (k)  fInt = (fInt + k + 12) % 12;
			if (u)  fWave += u;
			break;

		//  txt	 --------
		case D_CK_Logo:
			if (k)  ckSpeed += k;
			if (u)  ckCur = (ckCur + u + ckMax) % ckMax;
			break;

		case D_Fonts:
			if (k)  fntCur = (fntCur + k + fntMax) % fntMax;
			break;

		default:
			break;
		}
	}
}
#endif
