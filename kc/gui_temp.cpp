#include "gui.h"
#include "Ada4_ST7735.h"
#include "kc_params.h"

#ifdef TEMP1

#include <OneWire.h>
#include <DS18B20.h>

//  sensor address
byte addr[8] = {0,};

OneWire onewire(TEMP1);  // pin
DS18B20 sensors(&onewire);


//  Get Temp'C
//....................................................................................
void Gui::GetTemp()
{
	uint32_t ms = millis();

	if (temp1 == 1)
	{	temp1 = 0;  //  first

		//  Look for 1-Wire devices
		if (onewire.search(addr))  // while
		if (OneWire::crc8(addr, 7) == addr[7])
		{
			//onewire.reset_search();
			//  setup
 			sensors.begin(12);  // quality bits
 			sensors.request(addr);
			temp1 = 2;
		}
	}
	if (temp1 >= 2)
	{
		if (ms - msTemp > tTemp(par) || ms < msTemp || temp1 == 2)
		{
			//  if measurement ready
			if (sensors.available())
			{
				msTemp = ms;
				temp1 = 3;
				//?  slower if not in gui, every 20, 10 sec

				fTemp = sensors.readTemperature(addr);
				fTemp += 0.03f * par.tempOfs;  // fix sensor error
				sensors.request(addr);  // next
			}
		}

	#ifdef GRAPHS
		if (ms - msTempG > tTgraph(par) || ms < msTempG)
		{
			msTempG = ms;
			//  graph inc pos
			++grTpos;
			if (grTpos >= W)  grTpos = 0;
			//  add to graph
			int t = 255.f * (fTemp - par.minTemp) / (par.maxTemp - par.minTemp);
			t = t > 255 ? 255 : (t < 0 ? 0 : t);
			grTemp[grTpos] = t;
		}
	#endif
	}
}

#endif
