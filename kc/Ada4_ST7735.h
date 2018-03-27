/*********************************************************************
   Wrapper from ST7735 to Adafruit GFX
   with single buffering
*********************************************************************/

#ifndef _Ada4_ST7735_H_
#define _Ada4_ST7735_H_

#include <Adafruit_GFX.h>

class Ada4_ST7735 : public Adafruit_GFX
{
 public:
	Ada4_ST7735();

	void begin();

	void clear();
	void display();

	uint16_t* getBuffer();
	void drawPixel(int16_t x, int16_t y, uint16_t color);

	void print(const char* str), println(const char* str);
	void print(char c), println(int i);

//	virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
//	virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

private:

	inline void drawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color) __attribute__((always_inline));
	inline void drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color) __attribute__((always_inline));

};

#endif
