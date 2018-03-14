#include "WProgram.h"
#include "usb_keyboard.h"
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


	int text = 3, plasma = 2;


	while(1)
	{
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
			tft.setTextColor(0xFFFF);



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



		if (text || plasma)
			tft.display();
	}
}
