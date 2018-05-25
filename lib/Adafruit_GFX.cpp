/*
This is the core graphics library for all our displays, providing a common
set of graphics primitives (points, lines, circles, etc.).	It needs to be
paired with a hardware-specific library for each display device we carry
(to handle the lower-level functions).

Adafruit invests time and resources providing this open source code, please
support Adafruit & open-source hardware by purchasing products from Adafruit!
 
Copyright (c) 2013 Adafruit Industries.	All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
	this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
	this list of conditions and the following disclaimer in the documentation
	and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
//	CryHam: threw out stuff I don't need

#include "Adafruit_GFX.h"
#include "WProgram.h"
extern const unsigned char font5x7[];

// Many (but maybe not all) non-AVR board installs define macros
// for compatibility with existing PROGMEM-reading AVR code.
// Do our own checks and defines here for good measure...

#ifndef pgm_read_byte
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_word
 #define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif
#ifndef pgm_read_dword
 #define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif

// Pointers are a peculiar case...typically 16-bit on AVR boards,
// 32 bits elsewhere.	Try to accommodate both...

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
 #define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
 #define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif


Adafruit_GFX::Adafruit_GFX(int16_t w, int16_t h)
{
	_width  = w;
	_height = h;
	cursor_y = cursor_x	= 0;
	textcolor = 0xFFFF;
	gfxFont	= 0;
	wrap = true;
}


// Draw a circle outline
void Adafruit_GFX::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	drawPixel(x0, y0+r, color);
	drawPixel(x0, y0-r, color);
	drawPixel(x0+r, y0, color);
	drawPixel(x0-r, y0, color);

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		++x;
		ddF_x += 2;
		f += ddF_x;

		drawPixel(x0 + x, y0 + y, color);	drawPixel(x0 - x, y0 + y, color);
		drawPixel(x0 + x, y0 - y, color);	drawPixel(x0 - x, y0 - y, color);
		drawPixel(x0 + y, y0 + x, color);	drawPixel(x0 - y, y0 + x, color);
		drawPixel(x0 + y, y0 - x, color);	drawPixel(x0 - y, y0 - x, color);
	}
}


// Bresenham's algorithm - thx wikpedia
void Adafruit_GFX::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
	bool steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep)
	{
		_swap_int16_t(x0, y0);
		_swap_int16_t(x1, y1);
	}
	if (x0 > x1)
	{
		_swap_int16_t(x0, x1);
		_swap_int16_t(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1)
		ystep = 1;
	else
		ystep = -1;

	for (; x0 <= x1; ++x0)
	{
		if (steep)
			drawPixel(y0, x0, color);
		else
			drawPixel(x0, y0, color);

		err -= dy;
		if (err < 0)
		{
			y0 += ystep;
			err += dx;
		}
	}
}

// Draw a rectangle
void Adafruit_GFX::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	drawFastHLine(x, y, w, color);
	drawFastHLine(x, y+h-1, w, color);
	drawFastVLine(x, y, h, color);
	drawFastVLine(x+w-1, y, h, color);
}

void Adafruit_GFX::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
	// Update in subclasses if desired!
	drawLine(x, y, x, y+h-1, color);
}

void Adafruit_GFX::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
	// Update in subclasses if desired!
	drawLine(x, y, x+w-1, y, color);
}

void Adafruit_GFX::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	// Update in subclasses if desired!
	for (int16_t i=x; i < x+w; ++i)
		drawFastVLine(i, y, h, color);
}

// Draw a triangle
void Adafruit_GFX::drawTriangle(int16_t x0, int16_t y0,
		int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
	drawLine(x0, y0, x1, y1, color);
	drawLine(x1, y1, x2, y2, color);
	drawLine(x2, y2, x0, y0, color);
}

// Fill a triangle
void Adafruit_GFX::fillTriangle(int16_t x0, int16_t y0,
		int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
	int16_t a, b, y, last;

	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1) {
		_swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
	}
	if (y1 > y2) {
		_swap_int16_t(y2, y1); _swap_int16_t(x2, x1);
	}
	if (y0 > y1) {
		_swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
	}

	if (y0 == y2)  // Handle awkward all-on-same-line case as its own thing
	{
		a = b = x0;
		if (x1 < a)		a = x1;
		else if(x1 > b) b = x1;
		if (x2 < a)		a = x2;
		else if(x2 > b) b = x2;
		drawFastHLine(a, y0, b-a+1, color);
		return;
	}

	int16_t
		dx01 = x1 - x0,
		dy01 = y1 - y0,
		dx02 = x2 - x0,
		dy02 = y2 - y0,
		dx12 = x2 - x1,
		dy12 = y2 - y1;
	int32_t
		sa	 = 0,
		sb	 = 0;

	// For upper part of triangle, find scanline crossings for segments
	// 0-1 and 0-2.	If y1=y2 (flat-bottomed triangle), the scanline y1
	// is included here (and second loop will be skipped, avoiding a /0
	// error there), otherwise scanline y1 is skipped here and handled
	// in the second loop...which also avoids a /0 error here if y0=y1
	// (flat-topped triangle).
	if (y1 == y2)	last = y1;	 // Include y1 scanline
	else			last = y1-1; // Skip it

	for (y=y0; y <= last; ++y)
	{
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		/* longhand:
		a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
		b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		*/
		if (a > b)  _swap_int16_t(a,b);
		drawFastHLine(a, y, b-a+1, color);
	}

	// For lower part of triangle, find scanline crossings for segments
	// 0-2 and 1-2.	This loop is skipped if y1=y2.
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y <= y2; ++y)
	{
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		/* longhand:
		a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
		b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		*/
		if (a > b)  _swap_int16_t(a,b);
		drawFastHLine(a, y, b-a+1, color);
	}
}


int Adafruit_GFX::write(uint8_t c)
{
	if(!gfxFont)  // Built-in font
	{
		if (c == '\n')
		{
			cursor_y += 8;
			cursor_x = 0;
		}
		// else if (c == '\r') { }	// skip
		else if (c < 128)  // half
		{
			if (wrap && cursor_x + 6 >= _width)
			{	// Heading off edge?
				cursor_x = 0;	// Reset x to zero
				cursor_y += 8;	// Advance y one line
			}
			drawChar(cursor_x, cursor_y, c, textcolor/*, textbgcolor*/);
			cursor_x += 6;
		}
	}
	else  // Custom font
	{
		int yo = (uint8_t)pgm_read_byte(&gfxFont->yAdvance) / 2 + 1;
		if (c == '\n')
		{
			cursor_x = 0;
			cursor_y += (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
		}
		else if (c != '\r')
		{
			uint8_t first = pgm_read_byte(&gfxFont->first);
			if (c >= first && c <= (uint8_t)pgm_read_byte(&gfxFont->last))
			{
				uint8_t	 c2	= c - pgm_read_byte(&gfxFont->first);
				GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c2]);
				uint8_t	 w	= pgm_read_byte(&glyph->width),
						h	= pgm_read_byte(&glyph->height);
				if (w > 0 && h > 0)  // Is there an associated bitmap?
				{
					int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset); // sic
					if (wrap && cursor_x + (xo + w) >= _width)
					{
						// Drawing character would go off right edge; wrap to new line
						cursor_x = 0;
						cursor_y += (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
					}
					drawChar(cursor_x, cursor_y + yo /**/, c, textcolor/*, textbgcolor*/);
				}
				cursor_x += pgm_read_byte(&glyph->xAdvance);
			}
		}
		return 1;
	}
	return 0;
}

//  Draw a character
void Adafruit_GFX::drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color/*, uint16_t bg*/)
{
	if (!gfxFont)  //  Built-in font
	{
		if (x >= _width		|| // Clip right
			y >= _height	|| // Clip bottom
			x + 6 - 1 < 0 || // Clip left
			y + 8 - 1 < 0)   // Clip top
		return;

		for (int8_t i=0; i < 6; ++i)
		{
			uint8_t line = i < 5 ? pgm_read_byte(font5x7 + (c*5)+i) : 0;
			for (int8_t j=0; j < 8; ++j, line >>= 1)
			{
				if (line & 0x1)
					drawPixel(x+i, y+j, color);
				//else if (bg != color)
				//	drawPixel(x+i, y+j, bg);
			}
		}
	}
	else  //  Custom font
	{
		// Character is assumed previously filtered by write() to eliminate
		// newlines, returns, non-printable characters, etc.	Calling drawChar()
		// directly with 'bad' characters of font may cause mayhem!

		c -= pgm_read_byte(&gfxFont->first);
		GFXglyph *glyph	= &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
		uint8_t	*bitmap = (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);

		uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
		uint8_t	 w	= pgm_read_byte(&glyph->width),
				 h	= pgm_read_byte(&glyph->height);
				 //xa = pgm_read_byte(&glyph->xAdvance);
		int8_t	 xo = pgm_read_byte(&glyph->xOffset),
				 yo = pgm_read_byte(&glyph->yOffset);
		uint8_t	xx, yy, bits = 0, bit = 0;
		//int16_t	xo16=0, yo16=0;

		for (yy=0; yy < h; ++yy)
		{
			for (xx=0; xx < w; ++xx)
			{
				if (!(bit++ & 7))
					bits = pgm_read_byte(&bitmap[bo++]);
				if (bits & 0x80)
					drawPixel(x+xo+xx, y+yo+yy, color);
				bits <<= 1;
			}
		}
	}
}


void Adafruit_GFX::setCursor(int16_t x, int16_t y)
{
	cursor_x = x;
	cursor_y = y;
}
void Adafruit_GFX::moveCursor(int16_t dx, int16_t dy)
{
	cursor_x += dx;
	cursor_y += dy;
}

int16_t Adafruit_GFX::getCursorX() const
{
	return cursor_x;
}
int16_t Adafruit_GFX::getCursorY() const
{
	return cursor_y;
}

void Adafruit_GFX::setColor(uint16_t c)
{
	textcolor = c;
}

void Adafruit_GFX::setFont(const GFXfont *f)
{
	gfxFont = (GFXfont *)f;
}
void Adafruit_GFX::setWrap(bool w)
{
	wrap = w;
}
