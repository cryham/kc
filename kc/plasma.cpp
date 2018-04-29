#include "WProgram.h"
#include "demos.h"
#include "Ada4_ST7735.h"

#ifdef DEMOS
#ifdef DEMOS_PLASMA


void Demos::Plasma()
{
	// sint in ram 0 37fps 30 flash, >2 20fps
		 if (plasma == 0)  PSlowClrRings();  // 12 fps Clr1 blur
	else if (plasma == 1)  PClrLines();  // 16 fps Clr2 lines

	else if (plasma == 2)  POrnPinkCy();
	else if (plasma == 3)  POrnViolBlue();  // 16 orng small

	else if (plasma == 4)  PSmallWhiteCyVi();
	else if (plasma == 5)  POldBlue();

	else if (plasma == 6)  PFastOrnCy();
	else if (plasma == 7)  PFastBlueYellow();
	else if (plasma == 8)  PFastDiamCyBlR();
	else if (plasma == 9)  PPink();

	if (iInfo > 0)
	{
		d->setCursor(0,0);
		d->println(tadd[plasma]);
	}
}

void Demos::PlasmaT(int8_t dt)
{
	tadd[plasma] += dt;
}
#define S  (SY*SY)


void Demos::PSlowClrRings()  // Clr1 blur
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
{
	uint tt[12]={t*9,t*12,t*5,t*4, t*13,t*4,t*8};
	uint tt2[4]={t*7,t*17,t*8,t*3}, tt3[4]={t*11,t*14,t*7,t*5};
	uint yy[3]={0,0,0}, yy2[6]={0,0,0,0,0,0}, yy3[6]={0,0,0,0,0,0};

	for (uint y=0; y<H; ++y)
	{
		uint a = y*W;
		uint xx[3]={0,0,0}, xx2[6]={0,0,0,0,0,0}, xx3[6]={0,0,0,0,0,0};

		for (uint x=0; x<W; ++x, ++a)
		{
			int c,d,e;
			c = 4*Sin( yy[0] +xx[0]+tt[0]) * Cos( xx[1]        +tt[1]);
			c-= 4*Sin( yy[1]       +tt[2]) * Cos( xx[2] +yy[2] +tt[3]);
			c+= 4*Sin(              tt[2]) * Cos(               tt[4]);
			c-= 4*Sin(              tt[5]) * Cos(               tt[6]);
			c = abs(c);
			xx[0]+=41; xx[1]+=25; xx[2]+=48;
			c /= 3*SY*(SY/11)/2;

			d = 6*Sin( yy2[0] +xx2[0]) * Cos( xx2[1] +tt2[0]);
			d-= 4*Cos( yy2[1] +tt2[1]) * Sin( xx2[2] +tt2[2]);
			d+= 5*Sin( yy2[2] +tt2[3]) * Cos( xx2[3] +yy2[3]);
			d-= 6*Cos( yy2[4] +xx2[4]) * Sin( xx2[5] +yy2[5]);
			d = abs(d);
			xx2[0]+=55; xx2[1]+=62; xx2[2]+=41; xx2[3]+=36; xx2[4]+=47; xx2[5]+=33;
			d /= 3*SY*(SY/12);

			e = 5*Sin( yy3[0] +xx3[0]) * Cos( xx3[1] +tt3[0]);
			e+= 5*Cos( yy3[1] +tt3[1]) * Sin( xx3[2] +tt3[2]);
			e-= 6*Sin( yy3[2] +tt3[3]) * Cos( xx3[3] +yy3[3]);
			e+= 6*Cos( yy3[4] +xx3[4]) * Sin( xx3[5] +yy3[5]);
			e = abs(e);
			xx3[0]+=31; xx3[1]+=48; xx3[2]+=83; xx3[3]+=62; xx3[4]+=37; xx3[5]+=25;
			e /= 3*SY*(SY/11);

			data[a] = (e<<11) + (c<<5) + d;
		}
		yy[0]+=61; yy[1]+=43; yy[2]+=18; yy[3]+=51; yy[4]+=26; yy[5]+=39;
		yy2[0]+=56; yy2[1]+=61; yy2[2]+=87; yy2[3]+=73; yy2[4]+=47; yy2[5]+=51;
		yy3[0]+=73; yy3[1]+=34; yy3[2]+=69; yy3[3]+=40; yy3[4]+=61; yy3[5]+=55;
	}	t += tadd[0];
}


void Demos::PClrLines()  // Clr2 lines
{
	uint tt[16]={t*9,t*12,t*5,t*4, t*14,t*3,t*7,t*12, t*5,t*13,t*4,t*8, t*2,t*6,t*10,t*11};
	uint yy[12]={0,0,0,0,0,0,0,0,0,0,0,0};

	for (uint y=0; y<H; ++y)
	{
		uint a = y*W;
		uint xx[12]={0,0,0,0,0,0,0,0,0,0,0,0};

		for (uint x=0; x<W; ++x, ++a)
		{
			int c,r,b;
			c = 4*Sin( yy[0] +xx[0]+tt[0]) * Cos( xx[1]        +tt[1]);
			c-= 4*Sin( yy[1]       +tt[2]) * Cos( xx[2] +yy[2] +tt[3]);
			c+= 5*Cos( yy[3] +xx[4]+tt[4]) * Sin( xx[3]        +tt[5]);
			c-= 5*Cos( yy[4]       +tt[6]) * Sin( xx[5] +yy[5] +tt[7]);
			c+= 4*Sin( yy[6]       +tt[8]) * Cos( xx[6]        +tt[9]);  r = c;
			c-= 4*Sin( yy[8] +xx[7]+tt[10])* Cos( xx[8]        +tt[11]);
			c+= 5*Cos( yy[9] +xx[9]+tt[12])* Sin( xx[10]+yy[7] +tt[13]);
			c-= 5*Cos( yy[11]      +tt[14])* Sin( xx[11]+yy[10]+tt[15]);  b = c-r;
			c = abs(c);  r = abs(r);  b = abs(b);
			xx[0]+=111; xx[1]+=75; xx[2]+=100; xx[3]+=97; xx[4]+=114; xx[5]+=43;
			xx[6]+=91; xx[7]+=103; xx[8]+=124; xx[9]+=88; xx[10]+=132; xx[11]+=17;

			c /= SY*(SY/8);  c = min(31, max(0, 31-c));  //^
			b /= SY*(SY/8);  b = min(31, max(0, 31-b));
			r /= SY*(SY/16);  //r = max(0, 31 - r);
			data[a] = ((r/4)<<11) + (c<<6) + b;  // 31-r/4
		}
		yy[0]+=71; yy[1]+=136; yy[2]+=128; yy[3]+=82; yy[4]+=78; yy[5]+=149;
		yy[6]+=93; yy[7]+=56; yy[8]+=113; yy[9]+=109; yy[10]+=121; yy[11]+=132;
	}	t += tadd[1];
}


void Demos::POrnPinkCy()  // POrnPinkCy ~~~
//....................................................................................
{
	uint tt[16]={t*9,t*12,t*5,t*4, t*14,t*3,t*7,t*12, t*5,t*13,t*4,t*8, t*2,t*6,t*10,t*11};
	uint yy[12]={0,0,0,0,0,0,0,0,0,0,0,0};

	for (uint y=0; y<H; ++y)
	{
		uint a = y*W;
		uint xx[12]={0,0,0,0,0,0,0,0,0,0,0,0};

		for (uint x=0; x<W; ++x, ++a)
		{
			int c,r,b;
			c = 4*Sin( yy[0] +xx[0]+tt[0]) * Cos( xx[1]        +tt[1]);
			c-= 4*Sin( yy[1]       +tt[2]) * Cos( xx[2] +yy[2] +tt[3]);
			c+= 5*Cos( yy[3] +xx[4]+tt[4]) * Sin( xx[3]        +tt[5]);
			c-= 5*Cos( yy[4]       +tt[6]) * Sin( xx[5] +yy[5] +tt[7]);
			c+= 4*Sin( yy[6]       +tt[8]) * Cos( xx[6]        +tt[9]);  r = c;
			c-= 4*Sin( yy[8] +xx[7]+tt[10])* Cos( xx[8]        +tt[11]);
			c+= 5*Cos( yy[9] +xx[9]+tt[12])* Sin( xx[10]+yy[7] +tt[13]);
			c-= 5*Cos( yy[11]      +tt[14])* Sin( xx[11]+yy[10]+tt[15]);  b = c-r;
			c = abs(c);  r = abs(r);  b = abs(b);
			xx[0]+=111; xx[1]+=75; xx[2]+=100; xx[3]+=97; xx[4]+=114; xx[5]+=43;
			xx[6]+=91; xx[7]+=103; xx[8]+=124; xx[9]+=88; xx[10]+=132; xx[11]+=17;

			c /= SY*(SY/8);  //c = min(31, max(0, 31-c));  //^
			b /= SY*(SY/8);  //b = min(31, max(0, 31-b));
			r /= SY*(SY/16);  //r = max(0, 31 - r);
			data[a] = ((r/4)<<11) + (c/3<<6) + b/3;
		}
		yy[0]+=71; yy[1]+=136; yy[2]+=128; yy[3]+=82; yy[4]+=78; yy[5]+=149;
		yy[6]+=93; yy[7]+=56; yy[8]+=113; yy[9]+=109; yy[10]+=121; yy[11]+=132;
	}	t += tadd[2];
}

void Demos::POrnViolBlue()  // orange viol blue
//....................................................................................
{
	uint tt[20]={t*10,t*10,t*4,t*5, t*5,t*7,t*1,t*5,
				t*3,t*8,t*2,t*6, t*5,t*9,t*2,t*7, t*3,t*8,t*4,t*5};
	uint yy[12]={0,0,0,0, 0,0,0,0, 0,0,0,0};

	for (uint y=0; y<H; ++y)
	{
		uint a = y*W;
		uint xx[12]={0,0,0,0, 0,0,0,0, 0,0,0,0};

		for (uint x=0; x<W; ++x, ++a)
		{
			int c,d,e;
			c = 4*Sin( yy[0]+xx[0]+tt[4]) *Cos( xx[1]       +tt[5]);
			c-= 4*Sin( yy[1]      +tt[6]) *Cos( xx[2]+yy[2] +tt[7]);
			c+= 5*Cos( yy[3]      +tt[8]) *Sin( xx[3]       +tt[9]);  e=c;
			c-= 5*Cos( yy[4]+xx[4]+tt[10])*Sin( xx[5]+yy[5] +tt[11]);
			c+= 4*Sin( yy[6]      +tt[12])*Cos( xx[6]+yy[7] +tt[13]);
			c-= 4*Sin( yy[8]+xx[8]+tt[14])*Cos( xx[8]       +tt[15]);  d=c;
			c+= 5*Cos( yy[9]+xx[9]+tt[16])*Sin( xx[10]+yy[10]+tt[17]);
			c-= 5*Cos( yy[11]     +tt[18])*Sin( xx[11]       +tt[19]);
			c = abs(c);  d = abs(d);  e = abs(e);
			xx[0]+=115; xx[1]+=22; xx[2]+=107; xx[3]+=85;  xx[4]+= 152; xx[5]+= 27;
			xx[6]+=82;  xx[7]+=13; xx[8]+=91;  xx[9]+=177; xx[10]+=135; xx[11]+=31;

			c /= 3*SY*(SY/10);  d /= 3*SY*(SY/10);  e /= 3*SY*(SY/10);
			data[a] = RGB(d, (e+d+c)/3, c);
		}
		yy[0]+=76; yy[1]+=213; yy[2]+=48; yy[3]+=91; yy[4]+=177; yy[5]+= 39;
		yy[6]+=26; yy[7]+=86;  yy[8]+=17; yy[9]+=21; yy[10]+=74; yy[11]+=17;
	}	t += tadd[5];
}


void Demos::PSmallWhiteCyVi()  // small white cyan viol
//....................................................................................
{
	uint tt[16]={t*9,t*12,t*5,t*4, t*14,t*3,t*7,t*12, t*5,t*13,t*4,t*8, t*2,t*6,t*10,t*11};
	uint yy[12]={0,0,0,0,0,0,0,0,0,0,0,0};

	for (uint y=0; y<H; ++y)
	{
		uint a = y*W;
		uint xx[12]={0,0,0,0,0,0,0,0,0,0,0,0};

		for (uint x=0; x<W; ++x, ++a)
		{
			int c,d,e;
			c = 5*Sin( yy[0] +xx[0]+tt[0]) * Cos( xx[1]        +tt[1]);
			c-= 5*Sin( yy[1]       +tt[2]) * Cos( xx[2] +yy[2] +tt[3]);
			c+= 5*Cos( yy[3] +xx[4]+tt[4]) * Sin( xx[3]        +tt[5]);  d=c;
			c-= 5*Cos( yy[4]       +tt[6]) * Sin( xx[5] +yy[5] +tt[7]);
			c+= 5*Sin( yy[6]       +tt[8]) * Cos( xx[6]        +tt[9]);
			c-= 5*Sin( yy[8] +xx[7]+tt[10])* Cos( xx[8]        +tt[11]);  e=c;
			c+= 5*Cos( yy[9] +xx[9]+tt[12])* Sin( xx[10]+yy[7] +tt[13]);
			c-= 5*Cos( yy[11]      +tt[14])* Sin( xx[11]+yy[10]+tt[15]);
			c = abs(c);  d = abs(d-e);  e = abs(c-d);
			xx[0]+=131; xx[1]+=75; xx[2]+=200; xx[3]+=97; xx[4] +=24; xx[5] +=343;
			xx[6]+=151; xx[7]+=63; xx[8]+=224; xx[9]+=88; xx[10]+=22; xx[11]+=337;

			c /= SY*(SY/12);  d /= SY*(SY/11);  e /= SY*(SY/12);
			data[a] = RGB(min(d,c)/3, max(d,e)/3, max(c,d)/3);
		}
		yy[0]+=171; yy[1]+=36; yy[2]+=128; yy[3]+=82; yy[4] +=208; yy[5] +=319;
		yy[6]+=193; yy[7]+=56; yy[8]+=113; yy[9]+=49; yy[10]+=221; yy[11]+=352;
	}	t += tadd[6];
}


void Demos::POldBlue()  // old blue  ---
{
	uint tt[16]={t*9,t*12,t*5,t*4, t*14,t*3,t*7,t*12, t*5,t*13,t*4,t*8, t*2,t*6,t*10,t*11};
	uint yy[12]={0,0,0,0,0,0,0,0,0,0,0,0};

	for (uint y=0; y<H; ++y)
	{
		uint a = y*W;
		uint xx[12]={0,0,0,0,0,0,0,0,0,0,0,0};

		for (uint x=0; x<W; ++x, ++a)
		{
			int c;
			c = 4*Sin( yy[0] +xx[0]+tt[0]) * Cos( xx[1]        +tt[1]);
			c-= 4*Sin( yy[1]       +tt[2]) * Cos( xx[2] +yy[2] +tt[3]);
			c+= 5*Cos( yy[3] +xx[4]+tt[4]) * Sin( xx[3]        +tt[5]);
			c-= 5*Cos( yy[4]       +tt[6]) * Sin( xx[5] +yy[5] +tt[7]);
			c+= 4*Sin( yy[6]       +tt[8]) * Cos( xx[6]        +tt[9]);
			c-= 4*Sin( yy[8] +xx[7]+tt[10])* Cos( xx[8]        +tt[11]);
			c+= 5*Cos( yy[9] +xx[9]+tt[12])* Sin( xx[10]+yy[7] +tt[13]);
			c-= 5*Cos( yy[11]      +tt[14])* Sin( xx[11]+yy[10]+tt[15]);
			c = abs(c);
			xx[0]+=111; xx[1]+=75; xx[2]+=100; xx[3]+=97; xx[4]+=154; xx[5]+=143;
			xx[6]+=131; xx[7]+=53; xx[8]+=144; xx[9]+=88; xx[10]+=132; xx[11]+=17;

			c /= SY*(SY/12);
			data[a] = ((c/5)<<6) + c/3;
		}
		yy[0]+=71; yy[1]+=93; yy[2]+=128; yy[3]+=151; yy[4]+=78; yy[5]+=149;
		yy[6]+=136; yy[7]+=56; yy[8]+=213; yy[9]+=109; yy[10]+=82; yy[11]+=132;
	}	t += tadd[4];
}


void Demos::PFastOrnCy()  // fast1  orng cyan
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
{
	uint tt[4]={t*9,t*17,t*8,t*3};
	uint yy[6]={0,0,0,0,0,0};

	for (uint y=0; y<H; ++y)
	{
		uint a = y*W;
		uint xx[6]={0,0,0,0,0,0};

		for (uint x=0; x<W; ++x, ++a)
		{
			int c,e;
			c = 8*Sin( yy[0] +xx[0]) * Cos( xx[1] +tt[0]);
			c+= 8*Cos( yy[1] +tt[1]) * Sin( xx[2] +tt[2]); e=c;
			c+= 6*Sin( yy[2] +tt[3]) * Cos( xx[3] +yy[3]);
			c+= 6*Cos( yy[4] +xx[4]) * Sin( xx[5] +yy[5]);
			c = abs(c);  e = abs(c-e);
			xx[0]+=15; xx[1]+=62; xx[2]+=85; xx[3]+=92; xx[4]+=77; xx[5]+=115;

			c /= SY*(SY/12);  e /= SY*(SY/12);
			data[a] = RGB(e/4, c/6 + e/6, c/3);
		}
		yy[0]+=66; yy[1]+=51; yy[2]+=166; yy[3]+=6; yy[4]+=151; yy[5]+=4;
	}	t += tadd[3];
}


void Demos::PFastBlueYellow()  // fast2  blue grn ylw
{
	uint tt[8]={t*3,t*7,t*11,t*15,t*19,t*22,t*9,t*13};
	uint yy[4]={0,0,0,0};

	for (uint y=0; y<H; ++y)
	{
		uint a = y*W;
		uint xx[4]={0,0,0,0};

		for (uint x=0; x<W; ++x, ++a)
		{
			#define m 8
			#define Sm SY/m*SY/m
			int c,d,e;
			c = 9*Sin( yy[0] -tt[0])/m * Cos(-xx[0] +tt[1])/m;
			c+= 9*Sin(-yy[1] +tt[2])/m * Cos( xx[1] -tt[3])/m; e=c;
			c-= 9*Sin(-yy[2] -tt[4])/m * Cos(-xx[2] +tt[5])/m; d=c;
			c-= 9*Cos( yy[3] +tt[6])/m * Sin(-xx[3] +tt[7])/m;
			c = abs(10*Sm-abs(c));  d = abs(c-6*Sm -abs(e-6*Sm));  e = abs(12*Sm-d);
			xx[0]+=35; xx[1]+=42; xx[2]+=57; xx[3]+=79;

			c /= SY/m*(SY/m/10);  d /= SY/m*(SY/m/10);  e /= SY/m*(SY/m/10);
			data[a] = RGB(min(31,e/4), min(31,c/4), min(31,d/5));
		}
		yy[0]+=26; yy[1]+=41; yy[2]+=59; yy[3]+=81;
	}	t += tadd[7];
}


void Demos::PFastDiamCyBlR()  // fast3  diam cyan blue red
{
	uint tt[8]={t*3,t*7,t*11,t*15,t*19,t*22,t*9,t*13};
	uint yy[4]={0,0,0,0};

	for (uint y=0; y<H; ++y)
	{
		uint a = y*W;
		uint xx[4]={0,0,0,0};

		for (uint x=0; x<W; ++x, ++a)
		{
			int32_t c,d,e;
			c = 9*Sin( yy[0] -tt[0]) * Cos(-xx[0] +tt[1]);
			c+= 9*Sin(-yy[1] +tt[2]) * Cos( xx[1] -tt[3]); e=c;
			c-= 9*Sin(-yy[2] -tt[4]) * Cos(-xx[2] +tt[5]); d=c;
			c-= 9*Cos( yy[3] +tt[6]) * Sin(-xx[3] +tt[7]);
			c = abs(-S*5+d-abs(S*2+c));  d = abs(c-S*3+abs(e-S*6));  e = abs(S*7-d);
			xx[0]+=25; xx[1]+=32; xx[2]+=47; xx[3]+=69;

			c /= SY*(SY/12);  d /= SY*(SY/12);  e /= SY*(SY/12);
			data[a] = RGB(e/4, c/4, d/4);
		}
		yy[0]+=16; yy[1]+=31; yy[2]+=49; yy[3]+=71;
	}	t += tadd[8];
}


void Demos::PPink()  // fast4  pink
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
{
	uint tt[8]={t*4,t*6,t*3,t*5,t*9,t*8,t*11,t*2};
	uint yy[4]={0,0,0,0};

	for (uint y=0; y<H; ++y)
	{
		uint a = y*W;
		uint xx[4]={0,0,0,0};

		for (uint x=0; x<W; ++x, ++a)
		{
			int c,d,e;
			c = 6*Sin( yy[0]+xx[3] -tt[0]) * Cos(-xx[0] +tt[1]);
			c+= 5*Sin( yy[1]-xx[2] -tt[2]) * Cos( xx[1] -tt[3]); e=c;
			c-= 7*Cos(-yy[2]+xx[1] +tt[4]) * Sin(-xx[2] +tt[5]); d=c;
			c+= 6*Cos(-yy[3]-xx[0] +tt[6]) * Sin( xx[3] -tt[7]);
			c = abs(c);
			d = abs(c-e);
			e = abs(d+3*S-abs(c-6*S));
			xx[0]+=111; xx[1]+=62; xx[2]+=37; xx[3]+=46;

			c /= SY*(SY/12);  d /= SY*(SY/12);  e /= SY*(SY/12);
			data[a] = RGB( max(c/5,d/4), min(d/5,e/5), max(c/4,e/5) );
		}
		yy[0]+=113; yy[1]+=55; yy[2]+=33; yy[3]+=41;
	}	t += tadd[9];
}


#endif
#endif
