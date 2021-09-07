
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))

#include <util/delay.h>
#include <stdlib.h>

#include "Adafruit_GFX.h"
#include "Ada4_ST7735.h"

//  screen dimensions  160x128  TFT ST7735
const uint16_t ww = 160, hh = 128;


#include "TFT_ST7735.h"

extern TFT_ST7735 tft;

//  screen buffer 40 kB
static uint16_t buffer[ww * hh] = {0};

tPicture tp {buffer, ww,hh, ww*hh, 16, RLE_no};


#define CS1 10
#define DC1  9
#define RST  8

TFT_ST7735 tft = TFT_ST7735(CS1, DC1, RST);
//TFT_ST7735 tft = TFT_ST7735(CS1, DC1);


uint16_t* Ada4_ST7735::getBuffer()
{
	return buffer;
}

// the most basic function, set a single pixel
void Ada4_ST7735::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	if (x < 0 || x >= ww || y < 0 || y >= hh)
		return;

	buffer[x + ww*y] = color;
}


//  constructor
Ada4_ST7735::Ada4_ST7735()
	: Adafruit_GFX(ww, hh)
{
}

void Ada4_ST7735::begin()
{
	tft.begin();
}

void Ada4_ST7735::display()
{
	tft.drawImage(0,0,&tp,NONE);  // draw buffer to screen
}

//  clear buffer
void Ada4_ST7735::clear()
{
//	memset(buffer, 0, ww * hh * 2);
	memset(buffer, 0, sizeof(buffer));
}


void Ada4_ST7735::print(const char* str)
{
	char c;
	int i=0;
	while ((c = str[i++]) != 0)
	{
		write(c);
	}
}

void Ada4_ST7735::println(const char* str)
{
	print(str);
	cursor_x = 0;
	cursor_y += 8;
}

void Ada4_ST7735::print(char c)
{
	write(c);
}

void Ada4_ST7735::println(int i)
{
	char ch[32];
	sprintf(ch,"%d",i);
	print(ch);
	cursor_x = 0;
	cursor_y += 8;
}


void Ada4_ST7735::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
	int a = y*ww+x, i;
	for (i=0; i < w; ++i,++a)
		buffer[a] = color;
}

void Ada4_ST7735::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
	int a = y*ww+x, i;
	for (i=0; i < h; ++i,a+=ww)
		buffer[a] = color;
}

void Ada4_ST7735::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	int i,j,a;
	for (j=y; j < y+h; ++j)
	{	a = j*ww+x;
		for (i=0; i < w; ++i,++a)
			buffer[a] = color;
	}
}
