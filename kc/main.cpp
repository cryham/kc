#include "WProgram.h"
#include "usb_keyboard.h"
#include "plasma.h"
#include "Ada4_ST7735.h"


//------------------------------------------------------------------------------------
int main()
{
	Ada4_ST7735 tft;
	tft.begin();

	PlasmaInit(tft.getBuffer(),160,128);
	while(1)
	{
			Plasma(1);
			tft.display();
	}
}
