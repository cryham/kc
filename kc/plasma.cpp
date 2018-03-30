#include "WProgram.h"
#include "demos.h"

#ifdef DEMOS

//  sin
const int16_t sint[SX] =
{	// flash
	#include "sint8k.h"
	//#include "sint16k.h"
};


void Demos::Plasma()
{
	// sint in ram 0 37fps, >2 20fps
		 if (plasma == 0)  PlasmaC1();  // 12 fps Clr1 blur circles
	else if (plasma == 1)  PlasmaC2();  // 16 fps Clr2 lines
	else if (plasma == 2)  PlasmaC2b(); // 16 fps Clr2
	else if (plasma == 3)  Plasma1();  // 30 viol  was 37
	else if (plasma == 4)  Plasma2();  // 17 blue
	else if (plasma == 5)  Plasma3();  // 16 orng small
	else if (plasma == 6)  Plasma4();  // 16 tiny
}

void Demos::PlasmaT(int8_t dt)
{
	tadd[plasma] += dt;
}

static int yw[W];

void Demos::Wave()
{
	uint tt[16]={t*9,t*7,t*5,t*4, t*14,t*13,t*11,t*12, t*21,t*23,t*25,t*28, t*42,t*46,t*41,t*34};

	uint xx[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	for (uint x=0; x<W; ++x)
	{
		int c;
		c =11*Sin( xx[0] +tt[0]) + 10*Cos( xx[1] +tt[1]);
		c-=12*Sin( xx[2] +tt[2]) + 10*Cos( xx[3] +tt[3]);
		c+= 8*Cos( xx[4] +tt[4]) +  8*Sin( xx[5] +tt[5]);
		c-=10*Cos( xx[6] +tt[6]) +  9*Sin( xx[7] +tt[7]);
		c+= 7*Sin( xx[8] +tt[8]) +  8*Cos( xx[9] +tt[9]);
		c-= 8*Sin( xx[10]+tt[10])+  6*Cos( xx[11]+tt[11]);
		c+= 5*Cos( xx[12]+tt[12])+  3*Sin( xx[13]+tt[13]);
		c-= 4*Cos( xx[14]+tt[14])+  2*Sin( xx[15]+tt[15]);

		xx[0]+=22; xx[1]+=35; xx[2]+=31; xx[3]+=45; xx[4]+=66; xx[5]+=93;
		xx[6]+=131; xx[7]+=293; xx[8]+=458; xx[9]+=229; xx[10]+=388; xx[11]+=1256;
		xx[12]+=633; xx[13]+=1928; xx[14]+=836; xx[15]+=3301;

		//c *= 6*H/2;  c /= SY*SY;
		c /= 80*(SY/(H/2));
		yw[x] = c;
		//yw[x] = Sin(SX*x/W+t)/(SY/32);
	}

	for (uint y=0; y<H; ++y)
	{
		uint a = y*W;
		for (uint x=0; x<W; ++x, ++a)
		{
			int c = yw[x];
			int d = y-H/2-c;  d = H/2-abs(d);  d = max(0,d);
			c = RGB(min(31, d/3), min(31, d/2), min(31, d));
			data[a] = c;
		}
	}
	t+=twv;
}


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
void Demos::PlasmaC1()  // Clr1 blur
{
	uint tt[16]={t*9,t*12,t*5,t*4, t*14,t*3,t*7,t*12, t*5,t*13,t*4,t*8, t*2,t*6,t*10,t*11};
	uint tt2[4]={t*7,t*17,t*8,t*3}, tt3[4]={t*11,t*14,t*7,t*5};
	uint yy[12]={0,0,0,0,0,0,0,0,0,0,0,0}, yy2[6]={0,0,0,0,0,0}, yy3[6]={0,0,0,0,0,0};

	for (uint y=0; y<H; ++y)
	{
		uint a = y*W;
		uint xx[12]={0,0,0,0,0,0,0,0,0,0,0,0}, xx2[6]={0,0,0,0,0,0}, xx3[6]={0,0,0,0,0,0};

		for (uint x=0; x<W; ++x, ++a)
		{
			int c,d,e;
			c = 4*Sin( yy[0] +xx[0]+tt[0]) * Cos( xx[1]        +tt[1]);
			c-= 4*Sin( yy[1]       +tt[2]) * Cos( xx[2] +yy[2] +tt[3]);
			c+= 4*Sin( yy[6]       +tt[8]) * Cos( xx[6]        +tt[9]);
			c-= 4*Sin( yy[8] +xx[7]+tt[10])* Cos( xx[8]        +tt[11]);
			c = abs(c);
			xx[0]+=41; xx[1]+=25; xx[2]+=48; xx[3]+=61; xx[4]+=54; xx[5]+=33;
//			xx[6]+=131; xx[7]+=53; xx[8]+=144; xx[9]+=88; xx[10]+=132; xx[11]+=17;
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
//		yy[6]+=136; yy[7]+=56; yy[8]+=213; yy[9]+=109; yy[10]+=82; yy[11]+=132;

		yy2[0]+=56; yy2[1]+=61; yy2[2]+=87; yy2[3]+=73; yy2[4]+=47; yy2[5]+=51;
		yy3[0]+=73; yy3[1]+=34; yy3[2]+=69; yy3[3]+=40; yy3[4]+=61; yy3[5]+=55;
	}	t+=tadd[0];
}


void Demos::PlasmaC2()  // Clr2 lines
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
	}	t+=tadd[1];
}

void Demos::PlasmaC2b()  // Clr2
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
	}	t+=tadd[2];
}


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
void Demos::Plasma1()  // viol fastest
{
	uint tt[4]={t*7,t*17,t*8,t*3};
	uint yy[6]={0,0,0,0,0,0};

	for (uint y=0; y<H; ++y)
	{
		uint a = y*W;
		uint xx[6]={0,0,0,0,0,0};

		for (uint x=0; x<W; ++x, ++a)
		{
			int c;
			c = 8*Sin( yy[0] +xx[0]) * Cos( xx[1] +tt[0]);
			c+= 8*Cos( yy[1] +tt[1]) * Sin( xx[2] +tt[2]);
			c+= 6*Sin( yy[2] +tt[3]) * Cos( xx[3] +yy[3]);
			c+= 6*Cos( yy[4] +xx[4]) * Sin( xx[5] +yy[5]);
			c = abs(c);
			xx[0]+=15; xx[1]+=62; xx[2]+=85; xx[3]+=92; xx[4]+=77; xx[5]+=115;

			c /= SY*(SY/12);
			data[a] = ((c/5)<<11) + c/3;
		}
		yy[0]+=66; yy[1]+=51; yy[2]+=166; yy[3]+=6; yy[4]+=151; yy[5]+=4;
	}	t+=tadd[3];
}


void Demos::Plasma2()  // blue fast
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
	}	t+=tadd[4];
}


void Demos::Plasma3()  // orange small
{
	uint tt[32]={t*8,t*7,t*13,t*12, t*17,t*8,t*7,t*13, t*5,t*11,t*9,t*7, t*10,t*10,t*4,t*5,
				 t*5,t*7,t*1 ,t*5 , t*3 ,t*8,t*2,t*6 , t*5,t*9 ,t*2,t*7, t*3 ,t*8 ,t*4,t*5};
	uint yy[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	for (uint y=0; y<H; ++y)
	{
		uint a = y*W;
		uint xx[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

		for (uint x=0; x<W; ++x, ++a)
		{
			int c;
			c = 4*Sin( yy[0] +xx[0]+tt[0]) * Cos( xx[1]        +tt[1]);
			c-= 4*Sin( yy[1]       +tt[2]) * Cos( xx[2] +yy[2] +tt[3]);
			c+= 5*Cos( yy[3]       +tt[4]) * Sin( xx[3]        +tt[5]);
			c-= 5*Cos( yy[4] +xx[4]+tt[6]) * Sin( xx[5] +yy[5] +tt[7]);
			c+= 4*Sin( yy[6]       +tt[8]) * Cos( xx[6] +yy[7] +tt[9]);
			c-= 4*Sin( yy[8] +xx[7]+tt[10])* Cos( xx[8]        +tt[11]);
			c+= 5*Cos( yy[9] +xx[9]+tt[12])* Sin( xx[10]+yy[10]+tt[13]);
			c-= 5*Cos( yy[11]      +tt[14])* Sin( xx[11]       +tt[15]);

			c = 4*Sin( yy[12]+xx[12]+tt[16])*Cos( xx[13]       +tt[17]);
			c-= 4*Sin( yy[13]       +tt[18])*Cos( xx[14]+yy[14]+tt[19]);
			c+= 5*Cos( yy[15]       +tt[20])*Sin( xx[15]       +tt[21]);
			c-= 5*Cos( yy[16]+xx[16]+tt[22])*Sin( xx[17]+yy[17]+tt[23]);
			c+= 4*Sin( yy[18]       +tt[24])*Cos( xx[18]+yy[19]+tt[25]);
			c-= 4*Sin( yy[20]+xx[19]+tt[26])*Cos( xx[20]       +tt[27]);
			c+= 5*Cos( yy[21]+xx[21]+tt[28])*Sin( xx[22]+yy[22]+tt[29]);
			c-= 5*Cos( yy[23]       +tt[30])*Sin( xx[23]       +tt[31]);
			c = abs(c);
			xx[0]+=15; xx[1]+=62; xx[2]+=71; xx[3]+=85; xx[4]+=12; xx[5]+=57;
			xx[6]+=92; xx[7]+=63; xx[8]+=81; xx[9]+=77; xx[10]+=115; xx[11]+=107;
			xx[12]+=315; xx[13]+=162; xx[14]+=171; xx[15]+=25; xx[16]+=212; xx[17]+=27;
			xx[18]+=82; xx[19]+=163; xx[20]+=71; xx[21]+=177; xx[22]+=325; xx[23]+=301;

			c /= SY*(SY/12);
			data[a] = ((c/3)<<11) + ((c/5)<<6);
		}
		yy[0]+=66; yy[1]+=83; yy[2]+=8; yy[3]+=51; yy[4]+=77; yy[5]+=9;
		yy[6]+=166; yy[7]+=6; yy[8]+=183; yy[9]+=151; yy[10]+=4; yy[11]+=177;
		yy[12]+=266; yy[13]+=283; yy[14]+=48; yy[15]+=351; yy[16]+=377; yy[17]+=39;
		yy[18]+=16; yy[19]+=86; yy[20]+=13; yy[21]+=11; yy[22]+=74; yy[23]+=17;
	}	t+=tadd[5];
}


void Demos::Plasma4()  // tiny
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
			c = 5*Sin( yy[0] +xx[0]+tt[0]) * Cos( xx[1]        +tt[1]);
			c-= 5*Sin( yy[1]       +tt[2]) * Cos( xx[2] +yy[2] +tt[3]);
			c+= 5*Cos( yy[3] +xx[4]+tt[4]) * Sin( xx[3]        +tt[5]);
			c-= 5*Cos( yy[4]       +tt[6]) * Sin( xx[5] +yy[5] +tt[7]);
			c+= 5*Sin( yy[6]       +tt[8]) * Cos( xx[6]        +tt[9]);
			c-= 5*Sin( yy[8] +xx[7]+tt[10])* Cos( xx[8]        +tt[11]);
			c+= 5*Cos( yy[9] +xx[9]+tt[12])* Sin( xx[10]+yy[7]+tt[13]);
			c-= 5*Cos( yy[11]      +tt[14])* Sin( xx[11]+yy[10]+tt[15]);
			c = abs(c);
			xx[0]+=511; xx[1]+=275; xx[2]+=300; xx[3]+=397; xx[4]+=514; xx[5]+=443;
			xx[6]+=591; xx[7]+=303; xx[8]+=324; xx[9]+=388; xx[10]+=532; xx[11]+=437;

			c /= SY*(SY/12);
			data[a] = ((c/3)<<11) + ((c/4)<<6) + c/3;
		}
		yy[0]+=371; yy[1]+=236; yy[2]+=428; yy[3]+=282; yy[4]+=508; yy[5]+=319;
		yy[6]+=393; yy[7]+=256; yy[8]+=413; yy[9]+=249; yy[10]+=521; yy[11]+=352;
	}	t+=tadd[6];
}
#endif
