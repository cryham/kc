#pragma once

#include <stdint.h>
#include "gfxfont.h"


class Adafruit_GFX
{
public:
	Adafruit_GFX(int16_t w, int16_t h);

	// This MUST be defined by the subclass:
	virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;

	// These MAY be overridden by the subclass.
	virtual void
		drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color),
		drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
		drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),

		drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
		fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

	// no subclass overrides
	void
		drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),

		drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color),
		fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color),

		drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color/*, uint16_t bg*/),

		setCursor(int16_t x, int16_t y),
		moveCursor(int16_t dx, int16_t dy),
		setTextColor(uint16_t c),

		setFont(const GFXfont *f = 0);

	virtual int write(uint8_t);

	// get current cursor position
	int16_t getCursorX() const;
	int16_t getCursorY() const;

protected:
	int16_t _width, _height,
			cursor_x, cursor_y;
	uint16_t textcolor;

	GFXfont *gfxFont;
};
