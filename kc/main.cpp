#include "WProgram.h"
#include "usb_keyboard.h"
#include "matrix.h"
#include "plasma.h"
#include "kbd_draw.h"
#include "Ada4_ST7735.h"

//  vars
unsigned long tm;
uint32_t oldti = 0;  // dt
uint c=0;   // frame counter
//#define pin 6


//------------------------------------------------------------------------------------
int main()
{
	Ada4_ST7735 tft;
	tft.begin();

	PlasmaInit(tft.getBuffer(),160,128);
	tm = rtc_get();

	#ifdef pin
	pinMode(pin, OUTPUT);
	digitalWriteFast(pin, LOW);
	#endif
	char a[128];


	//uint16_t scanNum = 0; // K
	Matrix_setup();

	int text = 3, key = 1, plasma = 0;


	while(1)
	{
		Matrix_scan( 0 );  // K

		#ifdef pin
		digitalWriteFast(pin, LOW);
		#endif


		// C 11- 13fps, 0 37fps, >2 20fps  txt 55fps 3ln
		//------------------------------------------------
		if (plasma)
			Plasma(plasma);
		else
			tft.clear();

		//------------------------------------------------
		if (text)
		{
			//  kbd draw  ---------
			if (text == 3)
			{
				int x = 0, y = 0;
				for (int i=0; i < numKeys; ++i)
				{
					const SKey& k = keys[i];
					if (k.x >=0)  x = k.x;  else  x -= k.x;
					if (k.y > 0)  y = k.y + 60; /*Y*/  else  y -= k.y;
	
					tft.drawRect(x, y, k.w+1, k.h+1, clrRect[k.o]);
					tft.setCursor(
						k.o==5 || k.o==7 ? x + k.w - 6 :
								k.o==3 ? x+1 : x+1,
						k.h < H ? y :
								k.o==3 ? y+1 : y+1);
					tft.setTextColor(clrText[k.o]);
					sprintf(a,"%c", k.c);
					tft.print(a);
				}
			}
			tft.setTextColor(0xFFFF);

			//  matrix  -------
			if (text == 2)
			for (uint c=0; c < Matrix_colsNum; ++c)
			for (uint r=0; r < Matrix_rowsNum; ++r)
			{
				tft.setCursor(c*16, 30 + r*8);
				const KeyState& k = Matrix_scanArray[Matrix_colsNum * r + c];
				sprintf(a,"%d", k.curState);
				tft.print(a);
			}
			//Matrix_scan( 1 );  // K
			//scanNum = 0;


			//  fps  ---------
			//if (text)
			{
			uint32_t ti = millis();
			tft.setCursor(0,0); //hh-12);
			float fr = 1000.f / (ti - oldti);
			int ff = fr;
			//sprintf(a,"%4.1f", fr);
			sprintf(a,"%d", ff);
			tft.print(a);
			oldti = ti;

			//  time  ---
			tm = rtc_get();
			int h = tm/3600%24, m = tm/60%60, s = tm%60;
			tft.setCursor(0,8); //hh-20);
			sprintf(a,"%2d:%02d:%02d  %d", h,m,s, c);
			tft.print(a);

			//  keys  ***
			tft.setCursor(0,20); //hh-20);
			sprintf(a,"P %d  R %d  H %d", cnt_press, cnt_rel, cnt_hold);
			tft.print(a);

			/*tft.setCursor(0,50); //hh-20);
			sprintf(a,"txt %d  pls %d  key %d", text, plasma, key);
			//sprintf(a,"%f", fr);
			tft.print(a);*/
			}

			if (!plasma)
			{
				//delay(1);  // delay
				//tft.display();
			}
		}
		#ifdef pin
		digitalWriteFast(pin, HIGH);
		#endif
		++c;


		//  kbd
		//------------------------------------------------
		const int rr = Matrix_colsNum;
		KeyState& k = Matrix_scanArray[0 * rr + 0];
		k = Matrix_scanArray[1 * rr + 1];
		if (k.curState == KeyState_Press)
		{
			++plasma;
			if (plasma > 6)
				plasma = 0;
		}
		k = Matrix_scanArray[0 * rr + 1];
		if (k.curState == KeyState_Press)
		{
			++text;
			if (text > 3)
				text = 0;
		}

		k = Matrix_scanArray[1 * rr + 0];
		if (k.curState == KeyState_Press)
			key = 1 - key;

		//------------------------------------------------
		if (key)
		{
			//int u = 0;
			for (uint c=2; c < Matrix_colsNum; ++c)
			for (uint r=0; r < Matrix_rowsNum; ++r)
			{
				int id = rr * r + c;
				k = Matrix_scanArray[id];
				if (k.curState == KeyState_Press)
				{	Keyboard.press(KEY_A + id);
					Keyboard.send_now();
				}
				else if (k.curState == KeyState_Release)
				{	Keyboard.release(KEY_A + id);
					Keyboard.send_now();
				}
			}
		}

		if (text || plasma)
			tft.display();
	}
}
