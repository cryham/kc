#include "TFT_ST7735.h"


/*********************************************************
********************** constructors **********************
**********************************************************/


	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
	TFT_ST7735::TFT_ST7735(const uint8_t cspin,const uint8_t dcpin,const uint8_t rstpin,const uint8_t mosi,const uint8_t sclk)
	{
		_cs   = cspin;
		_dc   = dcpin;
		_rst  = rstpin;
		_mosi = mosi;
		_sclk = sclk;
	}
	#endif


/*********************************************************
************************ backlight ***********************
**********************************************************/
void TFT_ST7735::useBacklight(const uint8_t pin)
{
	_bklPin = pin;
	pinMode(_bklPin, OUTPUT);
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		digitalWriteFast(_bklPin,LOW);
	#endif
}

void TFT_ST7735::backlight(bool state)
{
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		if (_bklPin != 255) {
			digitalWriteFast(_bklPin,!state);
			_backlight = state;
		}
	#endif
}

/*********************************************************
*******************    some SPI stuff     ****************
**********************************************************/
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
//-----------------------------------------Teensy 3.0 & 3.1 & 3.2
	#if !defined (SPI_HAS_TRANSACTION)
	void TFT_ST7735::setBitrate(uint32_t n)
	{
		//nop
	}
	#endif
#endif

/*********************************************************
************** Var init and SPI preparation **************
**********************************************************/
void TFT_ST7735::begin(bool avoidSPIinit) 
{
//initialize Vars
	uint8_t i;
	_currentMode = 0b00000000;
	_backlight = 1;
	_portrait = false;
	_initError = 0b00000000;
	_scrollTop = 0;
	_scrollBottom = 0;
	_defaultBgColor = _ST7735_BACKGROUND;
	_defaultFgColor = _ST7735_FOREGROUND;
	_arcAngleMax = 360;
	_arcAngleOffset = -90;
	_bklPin = 255;
	_Mactrl_Data = 0b11000000;//0b11000000 //0b11001000
	#if defined(SPI_HAS_TRANSACTION)
		_ST7735SPI = SPISettings(TFT_ST7735_SPI_SPEED, MSBFIRST, SPI_MODE0);
	#endif
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
	if ((_mosi == 11 || _mosi == 7) && (_sclk == 13 || _sclk == 14)) {
        SPI.setMOSI(_mosi);
        SPI.setSCK(_sclk);
	} else {
		bitSet(_initError,0);
		return;
	}
	if (!avoidSPIinit) SPI.begin();
	if (SPI.pinIsChipSelect(_cs, _dc)) {
		pcs_data = SPI.setCS(_cs);
		pcs_command = pcs_data | SPI.setCS(_dc);
	} else {
		pcs_data = 0;
		pcs_command = 0;
		bitSet(_initError,1);
		return;
	}
#endif
	if (_rst != 255) {
		pinMode(_rst, OUTPUT);
		digitalWrite(_rst, HIGH);
		delay(10);
		digitalWrite(_rst, LOW);
		delay(10);
		digitalWrite(_rst, HIGH);
		delay(10);
	}
	/* -----------------------------------------------------------
	------------------- Chip Initialization ----------------------
	-------------------------------------------------------------*/
	startTransaction();
	//Software reset -------------------------
	writecommand_cont(CMD_SWRESET); delay(122);//500
	//Exit sleep -----------------------------
	writecommand_cont(CMD_SLPOUT);  delay(5);
	//Frame Rate Control (In normal mode/Full colors)
	writecommand_cont(CMD_FRMCTR1); for (i=0;i<3;i++) writedata8_cont(TFT_ST7735_FRMCTR1[i]);
	writecommand_cont(CMD_FRMCTR2); for (i=0;i<3;i++) writedata8_cont(TFT_ST7735_FRMCTR2[i]);
	writecommand_cont(CMD_FRMCTR3); for (i=0;i<3;i++) writedata8_cont(TFT_ST7735_FRMCTR3[i]);
	writecommand_cont(CMD_DINVCTR); writedata8_cont(TFT_ST7735_DINVCTR);
	//Power Control 1 ------------------------
	writecommand_cont(CMD_PWCTR1); 	for (i=0;i<3;i++) writedata8_cont(TFT_ST7735_PWCTR1[i]);
	//Power Control 2 ------------------------
	writecommand_cont(CMD_PWCTR2); 	writedata8_cont(TFT_ST7735_PWCTR2);
	writecommand_cont(CMD_PWCTR3); 	for (i=0;i<2;i++) writedata8_cont(TFT_ST7735_PWCTR3[i]);
	writecommand_cont(CMD_PWCTR4); 	for (i=0;i<2;i++) writedata8_cont(TFT_ST7735_PWCTR4[i]);
	writecommand_cont(CMD_PWCTR5); 	for (i=0;i<2;i++) writedata8_cont(TFT_ST7735_PWCTR5[i]);
	//VCOM control 1 -------------------------
	writecommand_cont(CMD_VCOMCTR1);for (i=0;i<2;i++) writedata8_cont(TFT_ST7735_VCOMCTR1[i]);
	writecommand_cont(CMD_DINVOF);
	//How many bits per pixel are used?
	writecommand_cont(CMD_PIXFMT); 	 writedata8_cont(TFT_ST7735_PIXFMT);
	//Default gamma curve?
	writecommand_cont(CMD_GAMMASET); writedata8_cont(TFT_ST7735_GAMMACURV);
	//compensate gamma
	if (TFT_ST7735_GAMMASET < 5){
		//Positive Gamma Correction Setting
		writecommand_cont(CMD_PGAMMAC); for (i=0;i<15;i++){writedata8_cont(pGammaSet[i]);}
		//Negative Gamma Correction Setting
		writecommand_cont(CMD_NGAMMAC); for (i=0;i<15;i++){writedata8_cont(nGammaSet[i]);}
	}
	//Exit idle mode
	writecommand_cont(CMD_IDLEOF); 
	//Set Column Address
	writecommand_cont(CMD_CLMADRS);  writedata16_cont(0x0000); writedata16_cont(TFT_ST7735_TFTWIDTH);
	//Set Page Address
	writecommand_cont(CMD_PGEADRS);  writedata16_cont(0x0000); writedata16_cont(TFT_ST7735_TFTHEIGHT);
	//Display inversion 
	//writecommand_cont(CMD_DINVCTR);  writedata8_cont(0x07);
	//Normal Display ON
	writecommand_cont(CMD_NORML);    writecommand_last(CMD_DISPON); delay(1);
	endTransaction();
	delay(1);
	//default screen rotation
	setRotation(_ST7735_ROTATION);
	clearMemory();
	//scroll area (all screen)
	//defineScrollArea(0,0);
	setScrollDirection(0);//default
	//Fill background with default color
	fillScreen(_defaultBgColor);
	//Backlight ON
	backlight(1);
	delay(30);
}

/*********************************************************
***************** Basic display commands *****************
**********************************************************/
/*
A simple helper to detect some error in initialization.
Since this library doesn't use MISO it can detect only
very basic errors:
0b00000001: Wrong MOSI,SCLK pin (only for some MCU's)
0b00000010: Wrong CS pin (only for some MCU's)
0b10000000: Display NOT supported (Only for multi-instances)
*/
uint8_t TFT_ST7735::getErrorCode(void) 
{
	return _initError;
}

void TFT_ST7735::clearMemory(void)
{
	startTransaction();
	setAddrWindow_cont(
				0,
				0,
				TFT_ST7735_TFTWIDTH,
				TFT_ST7735_TFTHEIGHT
	);
	_pushColors_cont(_defaultBgColor,TFT_ST7735_CGRAM);
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	
	endTransaction();
}

/*
This change the mode of the display as:
	NORMAL: Normal mode, backlight on automatically
	PARTIAL: Enable partial mode to work in portions of display (NOTE: Not all display are able to 
	set the non-partial mode BLACK, this due hardware problem on some display)
	IDLE: This is confusing (ILI designer delire), IDLE consume less current and shows less color
	SLEEP: Put display driver section in sleep but rest of the logic works, it also turn off display (if
	you have connected to backlight circuit drive pin)
	INVERT: It invert the display
	DISP_ON: Turn on display (if off) and enable backlight pin if used
	DISP_OFF: The opposite of above
*/
void TFT_ST7735::changeMode(const enum ST7735_modes m) 
{
	if (m != _currentMode){
		bool checkBacklight = false;
		startTransaction();
		switch(m){
			case NORMAL:
				if (_currentMode == 6) {//was in off display?
					writecommand_cont(CMD_DISPON);
					checkBacklight = true;
				}
				if (_currentMode == 2) {//was in idle?
					writecommand_cont(CMD_IDLEOF);
				}
				if (_currentMode == 3) {//was in sleep?
					writecommand_cont(CMD_SLPOUT);
					checkBacklight = true;
					delay(120);//needed
				}
				if (_currentMode == 4){//was inverted?
					_currentMode = 0;
					writecommand_cont(CMD_DINVOF);
				}
				writecommand_cont(CMD_NORML);
				_currentMode = 0;
			break;
			case PARTIAL:
				writecommand_cont(CMD_PTLON);
				_currentMode = 1;
			break;
			case IDLE:
				writecommand_cont(CMD_IDLEON);
				_currentMode = 2;
			break;
			case SLEEP:
				writecommand_last(CMD_SLPIN);
				endTransaction();
				_currentMode = 3;
				delay(5);//needed
				backlight(0);
			return;
			case INVERT:
				writecommand_cont(CMD_DINVON);
				_currentMode = 4;
			break;
			case DISP_ON:
				writecommand_cont(CMD_DISPON);
				_backlight = 0;
				checkBacklight = true;
				_currentMode = 5;
			break;
			case DISP_OFF:
				writecommand_cont(CMD_DISPOFF);
				_backlight = 1;
				checkBacklight = true;
				_currentMode = 6;
			break;
			default:
				endTransaction();
				return;
			break;
		}
		
		#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
			writecommand_last(CMD_NOP);
		#endif
		endTransaction();
		if (checkBacklight) backlight(!_backlight);
	}
}


uint8_t TFT_ST7735::getMode(void) 
{
	return _currentMode;
}

void TFT_ST7735::setArea(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) 
{
	startTransaction();
	setAddrWindow_cont(x0,y0,x1,y1);
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}

void TFT_ST7735::setPartialArea(uint16_t top,uint16_t bott) 
{
	startTransaction();
	writecommand_cont(CMD_PARTAREA);
	writedata16_cont(top);
	writedata16_last(bott);
	endTransaction();
}



uint8_t TFT_ST7735::getRotation(void)  
{
	return _rotation;
}

void TFT_ST7735::setRotation(uint8_t m) 
{
	_rotation = m % 4; // can't be higher than 3
	_Mactrl_Data &= ~(0xF8);//clear bit 3..7,  F0 -> clear bit 4...7
	if (_rotation == 0){
		_portrait = false;
		_width  = TFT_ST7735_TFTWIDTH;
		_height = TFT_ST7735_TFTHEIGHT;
		defineScrollArea(_scrollTop,_scrollBottom);
	} else if (_rotation == 1){
		bitSet(_Mactrl_Data,6); 
		bitSet(_Mactrl_Data,5);
		_width  = TFT_ST7735_TFTHEIGHT;
		_height = TFT_ST7735_TFTWIDTH;
		_portrait = true;
	} else if (_rotation == 2){
		bitSet(_Mactrl_Data,7); 
		bitSet(_Mactrl_Data,6);
		_width  = TFT_ST7735_TFTWIDTH;
		_height = TFT_ST7735_TFTHEIGHT;
		_portrait = false;
		defineScrollArea(_scrollTop,_scrollBottom);
	} else {
		bitSet(_Mactrl_Data,7); 
		bitSet(_Mactrl_Data,5);
		_width  = TFT_ST7735_TFTHEIGHT;
		_height = TFT_ST7735_TFTWIDTH;
		_portrait = true;
	}
	if (TFT_ST7735_CSPACE > 0) bitSet(_Mactrl_Data,3);
	startTransaction();
	 writecommand_cont(CMD_MADCTL);
	 writedata8_last(_Mactrl_Data);
	endTransaction();
	//if (_portrait) swapVals(_width,_height);
}


int16_t TFT_ST7735::width(void) const {
	return _width;
}
 
int16_t TFT_ST7735::height(void) const {
	return _height;
}

int16_t TFT_ST7735::cgWidth(void) const {
	return _width;
}
 
int16_t TFT_ST7735::cgHeight(void) const {
	return _height;
}


/*********************************************************
************************   scroll  ***********************
**********************************************************/

int16_t TFT_ST7735::getScrollTop(void)
{
	return _scrollTop;
}

int16_t TFT_ST7735::getScrollBottom(void)
{
	return _scrollBottom;
}


/*
tfa:Top Fixed Area
bfa:Bottom Fixed Area
*/
void TFT_ST7735::defineScrollArea(int16_t tfa, int16_t bfa)
{
	if (_rotation == 1 || _rotation == 3 || _rotation == 2) return;//no scroll for rot 1,3!
	uint16_t area = 0;
		if (tfa == 0 && bfa == 0) {
			bfa = 0;//special
			//tfa = TFT_ST7735_OFST[_rotation][1];
			area = _height - tfa - bfa;
			_scrollTop = 0;
			_scrollBottom = _height;
		} else {
			_scrollTop = tfa;
			_scrollBottom = bfa;
			//tfa += TFT_ST7735_OFST[_rotation][1];
			bfa = _height - bfa/* - TFT_ST7735_OFST[_rotation][1]*/;
			area = _height - tfa - bfa;
		}

		startTransaction();
		writecommand_cont(CMD_VSCLLDEF);
		writedata16_cont(tfa);
		writedata16_cont(area);
		writedata16_last(bfa);
		endTransaction();
}


uint8_t TFT_ST7735::getScrollDirection(void) 
{
	return _scrollDir;
}

void TFT_ST7735::setScrollDirection(uint8_t dir) 
{
	_scrollDir = dir % 2;
}


/*
Since the value of the Vertical Scrolling Start Address is absolute
it must not enter the fixed area otherwise undesirable image will be displayed
*/
boolean TFT_ST7735::scroll(int16_t pointer) 
{

	if (_rotation == 1 || _rotation == 3 || _rotation == 2) return 0;
    if (pointer >= _scrollTop && pointer <= _scrollBottom) {
		startTransaction();
        writecommand_cont(CMD_VSSTADRS);
		//pointer += TFT_ST7735_OFST[_rotation][1];
        writedata16_last(pointer);
		endTransaction();
		return true;
    }
	return false;
}


/*********************************************************
******************** Color Functions *********************
**********************************************************/

/**************************************************************************/
/*!
	  calculate a gradient color
	  return a spectrum starting from blue to red (0...127)
	  From my RA8875 library
*/
/**************************************************************************/
uint16_t TFT_ST7735::gradient(uint8_t val)
{
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t q = val / 32;
	switch(q){
		case 0: r = 0; g = 2 * (val % 32); b = 31; break;
		case 1: r = 0; g = 63; b = 31 - (val % 32); break;
		case 2: r = val % 32; g = 63; b = 0; break;
		case 3: r = 31; g = 63 - 2 * (val % 32); b = 0; break;
	}
	return (r << 11) + (g << 5) + b;
}

/**************************************************************************/
/*!
	  interpolate 2 16bit colors
	  return a 16bit mixed color between the two
	  Parameters:
	  color1:
	  color2:
	  pos:0...div (mix percentage) (0:color1, div:color2)
	  div:divisions between color1 and color 2
	  From my RA8875 library
	  NOTE: Needs a remake, it's slow! (TODO)
*/
/**************************************************************************/
uint16_t TFT_ST7735::colorInterpolation(uint16_t color1,uint16_t color2,uint16_t pos,uint16_t div)
{
    if (pos == 0) return color1;
    if (pos >= div) return color2;
	uint8_t r1,g1,b1;
	Color565ToRGB(color1,r1,g1,b1);//split in r,g,b
	uint8_t r2,g2,b2;
	Color565ToRGB(color2,r2,g2,b2);//split in r,g,b
	return colorInterpolation(r1,g1,b1,r2,g2,b2,pos,div);
}

/**************************************************************************/
/*!
	  interpolate 2 r,g,b colors
	  return a 16bit mixed color between the two
	  Parameters:
	  r1.
	  g1:
	  b1:
	  r2:
	  g2:
	  b2:
	  pos:0...div (mix percentage) (0:color1, div:color2)
	  div:divisions between color1 and color 2
	  From my RA8875 library
	  NOTE: Needs a remake, it's slow! (TODO)
*/
/**************************************************************************/
uint16_t TFT_ST7735::colorInterpolation(uint8_t r1,uint8_t g1,uint8_t b1,uint8_t r2,uint8_t g2,uint8_t b2,uint16_t pos,uint16_t div)
{
    if (pos == 0) return Color565(r1,g1,b1);
    if (pos >= div) return Color565(r2,g2,b2);
	float pos2 = (float)pos/div;
	return Color565(
				(uint8_t)(((1.0 - pos2) * r1) + (pos2 * r2)),
				(uint8_t)(((1.0 - pos2) * g1) + (pos2 * g2)),
				(uint8_t)(((1.0 - pos2) * b1) + (pos2 * b2))
	);
}

void TFT_ST7735::setBackground(uint16_t color) 
{
	_defaultBgColor = color;
}


void TFT_ST7735::setForeground(uint16_t color) 
{
	_defaultFgColor = color;
}

uint16_t TFT_ST7735::getBackground(void) 
{
	return _defaultBgColor;
}

uint16_t TFT_ST7735::getForeground(void) 
{
	return _defaultFgColor;
}

/*********************************************************
****************** Graphic Functions *********************
**********************************************************/

void TFT_ST7735::drawPixel(int16_t x, int16_t y, uint16_t color) 
{
	if (boundaryCheck(x,y)) return;
	if ((x < 0) || (y < 0)) return;
	startTransaction();
	drawPixel_cont(x,y,color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}



void TFT_ST7735::fillScreen(uint16_t color) 
{
	startTransaction();
	setAddrWindow_cont(
				0,
				0,
				_width - 1,
				_height - 1
	);
	_pushColors_cont(color,_width * _height);
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	
	endTransaction();
}



//with gradient
void TFT_ST7735::fillScreen(uint16_t color1,uint16_t color2) 
{
	startTransaction();
	if (color1 != color2){
		fillRect_cont(0,0,_width-1,_height-1,color1,color2);
	} else {
		setAddrWindow_cont(
				0,
				0,
				_width - 1,
				_height - 1
		);
		_pushColors_cont(color1,_width * _height);
	}
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	
	endTransaction();
}


void TFT_ST7735::clearScreen(void) 
{
	fillScreen(_defaultBgColor);
}

/*
draw fast vertical line
this uses fast contiguos commands method but opens SPi transaction and enable CS
then set CS hi and close transaction
*/
void TFT_ST7735::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) 
{
	// Rudimentary clipping
	if (boundaryCheck(x,y)) return;
	//if (((y + h) - 1) >= _height) h = _height - y;
	h = sizeCheck(y,h,_height);
	startTransaction();
	drawFastVLine_cont(x,y,h,color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}


/*
draw fast horizontal line
this uses fast contiguos commands method but opens SPi transaction and enable CS
then set CS hi and close transaction
*/
void TFT_ST7735::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) 
{
	// Rudimentary clipping
	if (boundaryCheck(x,y)) return;
	//if (((x + w) - 1) >= _width)  w = _width - x;
	w = sizeCheck(x,w,_width);
	startTransaction();
	drawFastHLine_cont(x,y,w,color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}

/*
fill RECT
*/
void TFT_ST7735::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) 
{
	if (boundaryCheck(x,y)) return;
	//if (((x + w) - 1) >= _width)  w = _width  - x;
	//if (((y + h) - 1) >= _height) h = _height - y;
	w = sizeCheck(x,w,_width);
	h = sizeCheck(y,h,_height);
	startTransaction();
	fillRect_cont(x,y,w,h,color,color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}

/*
fill RECT with gradient
*/
void TFT_ST7735::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1,uint16_t color2) 
{
	if (boundaryCheck(x,y)) return;
	//if (((x + w) - 1) >= _width)  w = _width  - x;
	//if (((y + h) - 1) >= _height) h = _height - y;
	w = sizeCheck(x,w,_width);
	h = sizeCheck(y,h,_height);
	startTransaction();
	fillRect_cont(x,y,w,h,color1,color2);
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}


//Updated, new way is fast!!!
void TFT_ST7735::fillRect_cont(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1,uint16_t color2) 
{
	//uint16_t colorTemp;
	if (w < 2 && h < 2){ drawPixel_cont(x,y,color1); return; }
	if (h < 2) { drawFastHLine_cont(x,y,w,color1); return; }
	if (w < 2) { drawFastVLine_cont(x,y,h,color1); return; }
	setAddrWindow_cont(x,y,(x+w)-1,(y+h)-1);
	if (color1 != color2){
		uint16_t pos = 0;
		uint8_t r1,r2,g1,g2,b1,b2;
		Color565ToRGB(color1,r1,g1,b1);
		Color565ToRGB(color2,r2,g2,b2);
		float pos2;
		uint8_t rR,gG,bB;
		uint16_t tot = h;
		uint16_t wtemp = w;
		do { 
			pos2 = (float)pos/tot;
			rR = (((1.0 - pos2) * r1) + (pos2 * r2));
			gG = (((1.0 - pos2) * g1) + (pos2 * g2));
			bB = (((1.0 - pos2) * b1) + (pos2 * b2));
			#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
				do { 
					writedata16_cont(Color565(rR,gG,bB)); 
				} while (--wtemp > 0);
				wtemp = w;
			#endif
			pos++;
		} while (--h > 0);
	} else {
		_pushColors_cont(color1,w*h);
	}
}


/*
draw LINE
*/
void TFT_ST7735::drawLine(int16_t x0, int16_t y0,int16_t x1, int16_t y1, uint16_t color)
{
	startTransaction();
	drawLine_cont(x0,y0,x1,y1,color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}

void TFT_ST7735::drawLine_cont(int16_t x0, int16_t y0,int16_t x1, int16_t y1, uint16_t color)
{
	if (y0 == y1) {
		if (x1 > x0) {
			drawFastHLine_cont(x0, y0, x1 - x0 + 1, color);
		} else if (x1 < x0) {
			drawFastHLine_cont(x1, y0, x0 - x1 + 1, color);
		} else {
			drawPixel_cont(x0, y0, color);
		}
		return;
	} else if (x0 == x1) {
		if (y1 > y0) {
			drawFastVLine_cont(x0, y0, y1 - y0 + 1, color);
		} else {
			drawFastVLine_cont(x0, y1, y0 - y1 + 1, color);
		}
		return;
	}

	bool steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {swapVals(x0, y0); swapVals(x1, y1);}
	if (x0 > x1) {swapVals(x0, x1); swapVals(y0, y1);}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	int16_t xbegin = x0;
	if (steep) {
		for (; x0<=x1; x0++) {
			err -= dy;
			if (err < 0) {
				int16_t len = x0 - xbegin;
				if (len) {
					drawFastVLine_cont(y0, xbegin, len + 1, color);
				} else {
					drawPixel_cont(y0, x0, color);
				}
				xbegin = x0 + 1;
				y0 += ystep;
				err += dx;
			}
		}
		if (x0 > xbegin + 1) drawFastVLine_cont(y0, xbegin, x0 - xbegin, color);
	} else {
		for (; x0<=x1; x0++) {
			err -= dy;
			if (err < 0) {
				int16_t len = x0 - xbegin;
				if (len) {
					drawFastHLine_cont(xbegin, y0, len + 1, color);
				} else {
					drawPixel_cont(x0, y0, color);
				}
				xbegin = x0 + 1;
				y0 += ystep;
				err += dx;
			}
		}
		if (x0 > xbegin + 1) drawFastHLine_cont(xbegin, y0, x0 - xbegin, color);
	}
}

/*
draw rect
*/
void TFT_ST7735::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	startTransaction();
		drawFastHLine_cont(x, y, w, color);
		drawFastHLine_cont(x, (y+h)-1, w, color);
		drawFastVLine_cont(x, y, h, color);
		drawFastVLine_cont((x+w)-1, y, h, color);	
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}


void TFT_ST7735::drawArcHelper(uint16_t cx, uint16_t cy, uint16_t radius, uint16_t thickness, float start, float end, uint16_t color) 
{
	int16_t xmin = 65535;
	int16_t xmax = -32767;
	int16_t ymin = 32767;
	int16_t ymax = -32767;
	float cosStart, sinStart, cosEnd, sinEnd;
	float r, t;
	float startAngle, endAngle;
	int16_t ir2, or2;//int
	int16_t x, y, x2, y2;//int
	int16_t y1s, y1e, y2s;//int
	float sslope, eslope;
	bool y1StartFound, y2StartFound, y1EndFound, y2EndSearching;

	startAngle = (start / _arcAngleMax) * 360;	// 252
	endAngle = (end / _arcAngleMax) * 360;		// 807

	while (startAngle < 0) startAngle += 360;
	while (endAngle < 0) endAngle += 360;
	while (startAngle > 360) startAngle -= 360;
	while (endAngle > 360) endAngle -= 360;

	if (startAngle > endAngle) {
		drawArcHelper(cx, cy, radius, thickness, ((startAngle / 360.0) * _arcAngleMax), _arcAngleMax, color);
		drawArcHelper(cx, cy, radius, thickness, 0, ((endAngle / 360.0) * _arcAngleMax), color);
	} else {
		// Calculate bounding box for the arc to be drawn
		cosStart = cosDeg_helper(startAngle);
		sinStart = sinDeg_helper(startAngle);
		cosEnd = cosDeg_helper(endAngle);
		sinEnd = sinDeg_helper(endAngle);

		r = radius;
		// Point 1: radius & startAngle
		t = r * cosStart;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinStart;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;

		// Point 2: radius & endAngle
		t = r * cosEnd;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinEnd;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;

		r = radius - thickness;
		// Point 3: radius-thickness & startAngle
		t = r * cosStart;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinStart;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;

		// Point 4: radius-thickness & endAngle
		t = r * cosEnd;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinEnd;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;
		// Corrections if arc crosses X or Y axis
		if ((startAngle < 90) && (endAngle > 90)) ymax = radius;
		if ((startAngle < 180) && (endAngle > 180)) xmin = -radius;
		if ((startAngle < 270) && (endAngle > 270)) ymin = -radius;

		// Slopes for the two sides of the arc
		sslope = (float)cosStart / (float)sinStart;
		eslope = (float)cosEnd / (float)sinEnd;
		if (endAngle == 360) eslope = -1000000;
		ir2 = (radius - thickness) * (radius - thickness);
		or2 = radius * radius;
		startTransaction();
		for (x = xmin; x <= xmax; x++) {
			y1StartFound 	= false;
			y2StartFound 	= false;
			y1EndFound 		= false;
			y2EndSearching 	= false;
			y1s = 0;
			y1e = 0;
			y2s = 0;
			for (y = ymin; y <= ymax; y++) {
				x2 = x * x;
				y2 = y * y;
				if (
					(x2 + y2 < or2 && x2 + y2 >= ir2) && (
					(y > 0 && startAngle < 180 && x <= y * sslope) ||
					(y < 0 && startAngle > 180 && x >= y * sslope) ||
					(y < 0 && startAngle <= 180) ||
					(y == 0 && startAngle <= 180 && x < 0) ||
					(y == 0 && startAngle == 0 && x > 0)
					) && (
					(y > 0 && endAngle < 180 && x >= y * eslope) ||
					(y < 0 && endAngle > 180 && x <= y * eslope) ||
					(y > 0 && endAngle >= 180) ||
					(y == 0 && endAngle >= 180 && x < 0) ||
					(y == 0 && startAngle == 0 && x > 0)))
				{
					if (!y1StartFound) {	//start of the higher line found
						y1StartFound = true;
						y1s = y;
					} else if (y1EndFound && !y2StartFound) {//start of the lower line found
						y2StartFound = true;
						y2s = y;
						y += y1e - y1s - 1;	// calculate the most probable end of the lower line (in most cases the length of lower line is equal to length of upper line), in the next loop we will validate if the end of line is really there
						if (y > ymax - 1) {// the most probable end of line 2 is beyond ymax so line 2 must be shorter, thus continue with pixel by pixel search
							y = y2s;	// reset y and continue with pixel by pixel search
							y2EndSearching = true;
						}
					} else if (y2StartFound && !y2EndSearching) {
						// we validated that the probable end of the lower line has a pixel, continue with pixel by pixel search, in most cases next loop with confirm the end of lower line as it will not find a valid pixel
						y2EndSearching = true;
					}
				} else {
					if (y1StartFound && !y1EndFound) {//higher line end found
						y1EndFound = true;
						y1e = y - 1;
						drawFastVLine_cont(cx + x, cy + y1s, y - y1s, color);
						if (y < 0) {
							y = abs(y); // skip the empty middle
						} else {
							break;
						}
					} else if (y2StartFound) {
						if (y2EndSearching) {
							// we found the end of the lower line after pixel by pixel search
							drawFastVLine_cont(cx + x, cy + y2s, y - y2s, color);
							y2EndSearching = false;
							break;
						} else {
							// the expected end of the lower line is not there so the lower line must be shorter
							y = y2s;	// put the y back to the lower line start and go pixel by pixel to find the end
							y2EndSearching = true;
						}
					}
				}
			}
			if (y1StartFound && !y1EndFound){
				y1e = ymax;
				drawFastVLine_cont(cx + x, cy + y1s, y1e - y1s + 1, color);
			} else if (y2StartFound && y2EndSearching)	{// we found start of lower line but we are still searching for the end
														// which we haven't found in the loop so the last pixel in a column must be the end
				drawFastVLine_cont(cx + x, cy + y2s, ymax - y2s + 1, color);
			}
		}
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
		endTransaction();
	}
}

void TFT_ST7735::setArcParams(float arcAngleMax, int arcAngleOffset)
{
	_arcAngleMax = arcAngleMax;
	_arcAngleOffset = arcAngleOffset;
}



/**************************************************************************/
/*!	
		sin e cos helpers
		[private]
		from my RA8875 library
*/
/**************************************************************************/
float TFT_ST7735::cosDeg_helper(float angle)
{
	float rads = angle / (float)360 * 2 * PI;
	return cos(rads);
}


float TFT_ST7735::sinDeg_helper(float angle)
{
	float rads = angle / (float)360 * 2 * PI;
	return sin(rads);
}


//fast
void TFT_ST7735::drawEllipse(int16_t cx,int16_t cy,int16_t radiusW,int16_t radiusH,uint16_t color)
{
	int16_t x,y;
	int16_t twoASquare,twoBSquare;
	int32_t stoppingX,stoppingY;
    int32_t xchange,ychange,ellipseError;
    twoASquare = 2 * (radiusW * radiusW);
    twoBSquare = 2 * (radiusH * radiusH);
    x = radiusW;
    y = 0;
    xchange = (radiusH * radiusH) * (1 - (2 * radiusW));
    ychange = (radiusW * radiusW);
    ellipseError = 0;
    stoppingX = (twoBSquare * radiusW);
    stoppingY = 0;
	startTransaction();
    while (stoppingX >= stoppingY) {
		plot4points_cont(cx,cy,x,y,color);
		y++;
		stoppingY += twoASquare;
		ellipseError += ychange;
		ychange += twoASquare;
		if ((2 * ellipseError) + xchange > 0) {
			x--;
			stoppingX -= twoBSquare;
			ellipseError += xchange;
			xchange += twoBSquare;
		}
    }
    x = 0;
    y = radiusH;
    xchange = (radiusH * radiusH);
    ychange = (radiusW * radiusW) * (1 - (2 * radiusH));
    ellipseError = 0;
    stoppingX = 0;
    stoppingY = (twoASquare * radiusH);
    while (stoppingX <= stoppingY) {
		plot4points_cont(cx,cy,x,y,color);
		x++;
		stoppingX += twoBSquare;
		ellipseError += xchange;
		xchange += twoBSquare;
		if (((2 * ellipseError) + ychange) > 0) {
			y--;
			stoppingY -= twoASquare;
			ellipseError += ychange;
			ychange += twoASquare;
		}
    }
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}

//fast
void TFT_ST7735::plot4points_cont(uint16_t cx, uint16_t cy, uint16_t x, uint16_t y, uint16_t color)
{
	drawPixel_cont(cx + x, cy + y, color);
	if (x != 0) drawPixel_cont(cx - x, cy + y, color);
	if (y != 0) drawPixel_cont(cx + x, cy - y, color);
	if (x != 0 && y != 0) drawPixel_cont(cx - x, cy - y, color);
}

//fast
void TFT_ST7735::drawCircle_cont(int16_t x,int16_t y,int16_t radius,uint8_t cornername,uint16_t color) 
{
	int16_t f			= 1 - radius;
	int16_t ddF_x		= 1;
	int16_t ddF_y 		= -2 * radius;
	int16_t deltax		= 0;
	int16_t deltay		= radius;
	if (cornername > 253){
		drawPixel_cont(x  		, y+radius	, color);
		drawPixel_cont(x  		, y-radius	, color);
		drawPixel_cont(x+radius	, y  		, color);
		drawPixel_cont(x-radius	, y  		, color);
	}
	while (deltax < deltay) {
		if (f >= 0) {
			deltay--;
			ddF_y += 2;
			f     += ddF_y;
		}
		deltax++;
		ddF_x += 2;
		f     += ddF_x;
		if (cornername > 253){
			drawPixel_cont(x + deltax, y + deltay, color);
			drawPixel_cont(x - deltax, y + deltay, color);
			drawPixel_cont(x + deltax, y - deltay, color);
			drawPixel_cont(x - deltax, y - deltay, color);
			drawPixel_cont(x + deltay, y + deltax, color);
			drawPixel_cont(x - deltay, y + deltax, color);
			drawPixel_cont(x + deltay, y - deltax, color);
			drawPixel_cont(x - deltay, y - deltax, color);
		} else {
			if (cornername & 0x4) {
				drawPixel_cont(x + deltax, y + deltay, color);
				drawPixel_cont(x + deltay, y + deltax, color);
			} 
			if (cornername & 0x2) {
				drawPixel_cont(x + deltax, y - deltay, color);
				drawPixel_cont(x + deltay, y - deltax, color);
			}
			if (cornername & 0x8) {
				drawPixel_cont(x - deltay, y + deltax, color);
				drawPixel_cont(x - deltax, y + deltay, color);
			}
			if (cornername & 0x1) {
				drawPixel_cont(x - deltay, y - deltax, color);
				drawPixel_cont(x - deltax, y - deltay, color);
			}
		}
	}
}


//fast
void TFT_ST7735::fillCircle_cont(int16_t x, int16_t y, int16_t radius, uint8_t cornername, int16_t delta, uint16_t color) 
{
	int16_t f     		= 1 - radius;
	int16_t ddF_x 		= 1;
	int16_t ddF_y 		= -2 * radius;
	int16_t deltax		= 0;
	int16_t deltay		= radius;

	while (deltax < deltay) {
		if (f >= 0) {
			deltay--;
			ddF_y += 2;
			f     += ddF_y;
		}
		deltax++;
		ddF_x += 2;
		f     += ddF_x;

		if (cornername & 0x1) {
			drawFastVLine_cont(x+deltax, y-deltay, 2*deltay+1+delta, color);
			drawFastVLine_cont(x+deltay, y-deltax, 2*deltax+1+delta, color);
		}
		if (cornername & 0x2) {
			drawFastVLine_cont(x-deltax, y-deltay, 2*deltay+1+delta, color);
			drawFastVLine_cont(x-deltay, y-deltax, 2*deltax+1+delta, color);
		}
	}
}

//fast
void TFT_ST7735::drawCircle(int16_t x, int16_t y, int16_t radius, uint16_t color)
{
	startTransaction();
	drawCircle_cont(x,y,radius,254,color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}

void TFT_ST7735::fillCircle(int16_t x, int16_t y, int16_t radius,uint16_t color) 
{
	//if (x+(radius*2) > _width || y+(radius*2) > _height) return;
	startTransaction();//open SPI comm
	drawFastVLine_cont(x, y-radius, (2*radius)+1, color);
	fillCircle_cont(x, y, radius, 3, 0, color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();//close SPI comm
}


//fast
void TFT_ST7735::drawRoundRect(int16_t x, int16_t y, int16_t w,int16_t h, int16_t radius, uint16_t color) 
{
	if (x+w >= _width || y+h > _height) return;
	//if (x+w-radius-1 < 0 || y+h-radius-1 < 0) return;
	startTransaction();
	drawFastHLine_cont(x+radius  , y    	 , w-2*radius, color); // Top
	drawFastHLine_cont(x+radius  , y+h-1	 , w-2*radius, color); // Bottom
	drawFastVLine_cont(x    	 , y+radius  , h-2*radius, color); // Left
	drawFastVLine_cont(x+w-1	 , y+radius  , h-2*radius, color); // Right
  // draw four corners
	drawCircle_cont(x+radius    , y+radius    , radius, 1, color);
	drawCircle_cont(x+w-radius-1, y+radius    , radius, 2, color);
	drawCircle_cont(x+w-radius-1, y+h-radius-1, radius, 4, color);
	drawCircle_cont(x+radius    , y+h-radius-1, radius, 8, color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}


//fast
void TFT_ST7735::fillRoundRect(int16_t x, int16_t y, int16_t w,int16_t h, int16_t radius, uint16_t color) 
{
	if (x+w >= _width || y+h > _height) return;
	if (radius < 2) {
		fillRect(x,y,w,h,color);
		return;
	}
	
	startTransaction();
	fillRect_cont(x+radius, y, w-2*radius, h, color, color);
	fillCircle_cont(x+w-radius-1, y+radius, radius, 1, h-2*radius-1, color);
	fillCircle_cont(x+radius    , y+radius, radius, 2, h-2*radius-1, color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}


void TFT_ST7735::drawQuad(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2,int16_t x3, int16_t y3, uint16_t color) 
{
	startTransaction();//open SPI comm
	drawLine_cont(x0, y0, x1, y1, color);//low 1
	drawLine_cont(x1, y1, x2, y2, color);//high 1
	drawLine_cont(x2, y2, x3, y3, color);//high 2
	drawLine_cont(x3, y3, x0, y0, color);//low 2
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();//close SPI comm
}


void TFT_ST7735::fillQuad(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color,bool triangled) 
{
	startTransaction();//open SPI comm
    fillTriangle_cont(x0,y0,x1,y1,x2,y2,color);
	if (triangled) fillTriangle_cont(x2, y2, x3, y3, x0, y0, color);
    fillTriangle_cont(x1,y1,x2,y2,x3,y3,color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();//close SPI comm
}

void TFT_ST7735::drawPolygon(int16_t x, int16_t y, uint8_t sides, int16_t diameter, float rot, uint16_t color)
{ 
	sides = (sides > 2 ? sides : 3);
	const float dtr = (PI/180.0) + PI;
	float rads = 360.0 / sides;//points spacd equally
	uint8_t i;
	startTransaction();
	for (i = 0; i < sides; i++) { 
		drawLine_cont(
			x + (sin((i*rads + rot) * dtr) * diameter),
			y + (cos((i*rads + rot) * dtr) * diameter),
			x + (sin(((i+1)*rads + rot) * dtr) * diameter),
			y + (cos(((i+1)*rads + rot) * dtr) * diameter),
			color);
	}
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();//close SPI comm
}

void TFT_ST7735::drawMesh(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	if (boundaryCheck(x,y)) return;
	//if (((x + w) - 1) >= _width)  w = _width  - x;
	//if (((y + h) - 1) >= _height) h = _height - y;
	w = sizeCheck(x,w,_width);
	h = sizeCheck(y,h,_height);
	
	int16_t n, m;

	if (w < x) {n = w; w = x; x = n;}
	if (h < y) {n = h; h = y; y = n;}
	startTransaction();
	for (m = y; m <= h; m += 2) {
		for (n = x; n <= w; n += 2) {
			drawPixel_cont(n, m, color);
		}
	}
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();//close SPI comm
}

void TFT_ST7735::drawTriangle(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color) 
{
	startTransaction();
	drawLine_cont(x0, y0, x1, y1, color);
	drawLine_cont(x1, y1, x2, y2, color);
	drawLine_cont(x2, y2, x0, y0, color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();//close SPI comm
}

//85% fast
void TFT_ST7735::fillTriangle(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color) 
{
	startTransaction();
	fillTriangle_cont(x0,y0,x1,y1,x2,y2,color);//
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();//close SPI comm
}

void TFT_ST7735::fillTriangle_cont(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color) 
{
	int16_t a, b, y, last;

	if (y0 > y1) {swapVals(y0, y1); swapVals(x0, x1);}
	if (y1 > y2) {swapVals(y2, y1); swapVals(x2, x1);}
	if (y0 > y1) {swapVals(y0, y1); swapVals(x0, x1);}

	if (y0 == y2) {
		a = b = x0;
		if (x1 < a){
			a = x1;
		} else if (x1 > b) {
			b = x1;
		}
		if (x2 < a){
			a = x2;
		} else if (x2 > b) {
			b = x2;
		}
		drawFastHLine_cont(a, y0, b-a+1, color);
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
		sa   = 0,
		sb   = 0;

	if (y1 == y2) {
		last = y1;
	} else { 
		last = y1-1;
	}

	for (y=y0; y<=last; y++) {
		a   = x0 + sa / dy01;
		b   = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		if (a > b) swapVals(a,b);
		drawFastHLine_cont(a, y, b-a+1, color);
	}

	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y <= y2; y++) {
		a   = x1 + sa / dy12;
		b   = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		if (a > b) swapVals(a,b);
		drawFastHLine_cont(a, y, b-a+1, color);
	}
}


/**************************************************************************/
/*!
      Basic line by using Angle as parameter
	  Parameters:
	  x: horizontal start pos
	  y: vertical start
	  angle: the angle of the line
	  length: lenght of the line
	  color: RGB565 color
	  from my RA8875 library
*/
/**************************************************************************/
void TFT_ST7735::drawLineAngle(int16_t x, int16_t y, int angle, uint8_t length, uint16_t color,int offset)
{
	if (length < 2) {//NEW
		drawPixel(x,y,color);
	} else {
		drawLine(
		x,
		y,
		x + (length * cosDeg_helper(angle + offset)),//_angle_offset
		y + (length * sinDeg_helper(angle + offset)), 
		color);
	}
}

/**************************************************************************/
/*!
      Basic line by using Angle as parameter
	  Parameters:
	  x: horizontal start pos
	  y: vertical start
	  angle: the angle of the line
	  start: where line start
	  length: lenght of the line
	  color: RGB565 color
	  from my RA8875 library
*/
/**************************************************************************/
void TFT_ST7735::drawLineAngle(int16_t x, int16_t y, int angle, uint8_t start, uint8_t length, uint16_t color,int offset)
{
	if (start - length < 2) {//NEW
		drawPixel(x,y,color);
	} else {
		drawLine(
		x + start * cosDeg_helper(angle + offset),//_angle_offset
		y + start * sinDeg_helper(angle + offset),
		x + (start + length) * cosDeg_helper(angle + offset),
		y + (start + length) * sinDeg_helper(angle + offset), 
		color);
	}
}

/**************************************************************************/
/*!
      ringMeter 
	  (adapted from Alan Senior (thanks man!))
	  from my RA8875 library
	  it create a ring meter with a lot of personalizations,
	  it return the width of the gauge so you can use this value
	  for positioning other gauges near the one just created easily
	  Parameters:
	  val:  your value
	  minV: the minimum value possible
	  maxV: the max value possible
	  x:    the position on x axis
	  y:    the position on y axis
	  r:    the radius of the gauge (minimum 20)
	  units: a text that shows the units, if "none" all text will be avoided
	  scheme:0...7 or 16 bit color (not BLACK or WHITE)
	  0:red
	  1:green
	  2:blue
	  3:blue->red
	  4:green->red
	  5:red->green
	  6:red->green->blue
	  7:cyan->green->red
	  8:black->white linear interpolation
	  9:violet->yellow linear interpolation
	  or
      RGB565 color (not BLACK or WHITE)
	  backSegColor: the color of the segments not active (default BLACK)
	  angle:		90 -> 180 (the shape of the meter, 90:halfway, 180:full round, 150:default)
	  inc: 			5...20 (5:solid, 20:sparse divisions, default:10)
*/
/**************************************************************************/
void TFT_ST7735::ringMeter(int val, int minV, int maxV, uint8_t x, uint8_t y, uint8_t r, uint16_t colorScheme,uint16_t backSegColor,int angle,uint8_t inc)
{
	if (inc < 5) inc = 5;
	if (inc > 20) inc = 20;
	if (r < 50) r = 20;
	if (angle < 90) angle = 90;
	if (angle > 180) angle = 180;
	int i;
	int curAngle = map(val, minV, maxV, -angle, angle);
	uint16_t colour;
	x += r;
	y += r;   // Calculate coords of centre of ring
	uint8_t w = r / 4;    // Width of outer ring is 1/4 of radius
	const uint8_t seg = 5; // Segments are 5 degrees wide = 60 segments for 300 degrees
	// Draw colour blocks every inc degrees
	for (i = -angle; i < angle; i += inc) {
		colour = BLACK;
		switch (colorScheme) {
			case 0:
				colour = RED;
				break; // Fixed colour
			case 1:
				colour = GREEN;
				break; // Fixed colour
			case 2:
				colour = BLUE;
				break; // Fixed colour
			case 3:
				colour = gradient(map(i, -angle, angle, 0, 127));
				break; // Full spectrum blue to red
			case 4:
				colour = gradient(map(i, -angle, angle, 63, 127));
				break; // Green to red (high temperature etc)
			case 5:
				colour = gradient(map(i, -angle, angle, 127, 63));
				break; // Red to green (low battery etc)
			case 6:
				colour = gradient(map(i, -angle, angle, 127, 0));
				break; // Red to blue (air cond reverse)
			case 7:
				colour = gradient(map(i, -angle, angle, 35, 127));
				break; // cyan to red 
			case 8:
				colour = colorInterpolation(0,0,0,255,255,255,map(i,-angle,angle,0,w),w);
				break; // black to white
			case 9:
				colour = colorInterpolation(0x80,0,0xC0,0xFF,0xFF,0,map(i,-angle,angle,0,w),w);
				break; // violet to yellow
			default:
				if (colorScheme > 9){
					colour = colorScheme;
				} else {
					colour = BLUE;
				}
				break; // Fixed colour
		}
		// Calculate pair of coordinates for segment start
		float xStart = cos((i - 90) * 0.0174532925);
		float yStart = sin((i - 90) * 0.0174532925);
		uint8_t x0 = xStart * (r - w) + x;
		uint8_t y0 = yStart * (r - w) + y;
		uint8_t x1 = xStart * r + x;
		uint8_t y1 = yStart * r + y;

		// Calculate pair of coordinates for segment end
		float xEnd = cos((i + seg - 90) * 0.0174532925);
		float yEnd = sin((i + seg - 90) * 0.0174532925);
		uint8_t x2 = xEnd * (r - w) + x;
		uint8_t y2 = yEnd * (r - w) + y;
		uint8_t x3 = xEnd * r + x;
		uint8_t y3 = yEnd * r + y;

		if (i < curAngle) { // Fill in coloured segments with 2 triangles
			fillQuad(x0, y0, x1, y1, x2, y2, x3, y3, colour, false);
		} else {// Fill in blank segments
			fillQuad(x0, y0, x1, y1, x2, y2, x3, y3, backSegColor, false);
		}
	}
}

//fast
void TFT_ST7735::startPushData(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) 
{
	startTransaction();
	setAddrWindow_cont(x0,y0,x1,y1);
}


//fast
void TFT_ST7735::pushData(uint16_t color) 
{
	writedata16_cont(color);
}

//fast
void TFT_ST7735::endPushData() 
{
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}

//fast
void TFT_ST7735::pushColor(uint16_t color) 
{
	startTransaction();
	writedata16_last(color);
	endTransaction();
}

void TFT_ST7735::drawIcon(int16_t x, int16_t y,const tIcon *icon,uint8_t scale,uint16_t f,uint16_t b,bool inverse)
{
	#if defined(_FORCE_PROGMEM__)
		const _smCharType * iconData 	= PROGMEM_read(&icon->data);
		uint8_t		iWidth				= pgm_read_byte(&(icon->image_width));	//AVR ok
		uint8_t		iHeight				= pgm_read_byte(&(icon->image_height)); //AVR ok
		uint16_t	datalen				= pgm_read_word(&(icon->image_datalen));//AVR ok
		//boolean	dataComp			= PROGMEM_read(&icon->image_comp);//not yet
	#else
//		const _smCharType * iconData	= icon->data;//icon data
		uint8_t		iWidth				= icon->image_width;
		uint8_t		iHeight				= icon->image_height;
//		uint16_t	datalen				= icon->image_datalen;
		//uint8_t		dataComp		= icon->image_comp;//not yet
	#endif
	if (iWidth < 1 || iHeight < 1) return;//cannot be
	if (scale < 1) scale = 1;
	if ((x + iWidth) * scale >= _width || (y + iHeight) * scale >= _height) return;//cannot be
	startTransaction();
	setAddrWindow_cont(x,y,iWidth+x,iHeight+y);
	//LGPO Rendering (uncomp)

	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}

void TFT_ST7735::drawImage(int16_t x, int16_t y,const tPicture *img,const enum ST7735_iconMods m,uint16_t b)
{
	uint16_t px = 0;
	uint16_t color;
	uint8_t currentY = 0;
	uint16_t currentX = 0;
	bool skip = false;
	#if defined(_FORCE_PROGMEM__)
		const uint16_t * imageData  = PROGMEM_read(&img->data);
		uint8_t		iWidth			= pgm_read_byte(&img->image_width);
		uint8_t		iHeight			= pgm_read_byte(&img->image_height);
		uint16_t	datalen			= pgm_read_word(&img->image_datalen);
		//uint8_t		dataDepth	= pgm_read_byte(&img->image_depth);//not yet
		//boolean		dataComp	= pgm_read_byte(&(img->image_comp);//not yet
	#else
		const uint16_t * imageData	= img->data;//image data
		uint8_t		iWidth			= img->image_width;
		uint8_t		iHeight			= img->image_height;
		uint16_t	datalen			= img->image_datalen;
		//uint8_t		dataDepth	= img->image_depth;//not yet
		//uint8_t		dataComp	= img->image_comp;//not yet
	#endif
	
	iWidth -= 1;
	iHeight -= 1;
	if (iWidth < 1 || iHeight < 1) return;//cannot be
	if (x + iWidth >= _width || y + iHeight >= _height) return;//cannot be
	
	startTransaction();
	setAddrWindow_cont(x,y,iWidth+x,iHeight+y);
	
	do { 
		#if defined(_FORCE_PROGMEM__)
			color = pgm_read_word(&imageData[px]);
		#else
			color = imageData[px];
		#endif
		if (m == TRANSPARENT){
			if (color <= b) skip = true;
		} else if (m == REPLACE){
			if (color <= b) color = _defaultBgColor;
		} else if (m == BOTH){
			if (color <= b) {
				color = _defaultBgColor;
			} else {
				color = _defaultFgColor;
			}
		}
		
		if (!skip) {
			writedata16_cont(color); 
		} else {
			setAddrWindow_cont(x+currentX,y+currentY,(iWidth+x),(iHeight+y));//constrain window
			skip = false;
		}
		
		if (currentX == iWidth){
			currentX = 0;
			currentY++;
		} else {
			currentX++;
		}
		
		px++;
		
	} while (--datalen > 0);
	
	#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}


void TFT_ST7735::drawBitmap(int16_t x, int16_t y,const uint8_t *bitmap, int16_t w, int16_t h,uint16_t color) 
{
	int16_t i, j;
	int16_t byteWidth = (w + 7) / 8;
	for (j = 0; j < h; j++) {
		for (i = 0; i < w; i++ ) {
			if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) drawPixel(x + i, y + j, color);
		}
	}
}

void TFT_ST7735::drawBitmap(int16_t x, int16_t y,const uint8_t *bitmap, int16_t w, int16_t h,uint16_t color, uint16_t bg) 
{
	int16_t i, j;
	int16_t byteWidth = (w + 7) / 8;
	for (j = 0; j < h; j++) {
		for (i = 0; i < w; i++ ) {
			if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
				drawPixel(x + i, y + j, color);
			} else {
				drawPixel(x + i, y + j, bg);
		}
    }
  }
}


/*
 ----------------- PushColor stream --------------------------------
*/
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
	void TFT_ST7735::_pushColors_cont(uint16_t data,uint32_t times)
	{
		do { 
			writedata16_cont(data); 
		} while (--times > 0);
	}
#endif
/* -------------------------------------------------------------------------------------------
++++++++++++++++++++++++++++++++ Size Optimizations ++++++++++++++++++++++++++++++++++++++++++
always inline routines dublicate the entire function inside chunks of code for speed during
compiling, this is acceptable for cpu with lot of code space (like Teensy 3.x, DUE) but
can increase dramatically the amount of code for UNO and similar.
For this reason some function are now normal for all CPU, this decrease a fraction of the
overall speed but decrease the amount of space occupied by code.
In addition, there's an user option to decrease a lot the space, enabled for small resources CPU.
I'm sorry for the complicated preprocessor #if #else and the amount of code inside library for 
fix this but is the only 'fast way' I found to acieve this!
--------------------------------------------------------------------------------------------*/


#if defined(_ST7735_SIZEOPTIMIZER)
/* ========================================================================
	-------------------- Common low level commands ------------------------
	Teensy 3.x uses different functions, This are for all the rest of MCU's
   ========================================================================*/
	#if !defined(__MK20DX128__) && !defined(__MK20DX256__) && !defined(__MK64FX512__) && !defined(__MK66FX1M0__)
		void TFT_ST7735::writecommand_cont(const uint8_t c)
		{
			enableCommandStream();
			spiwrite(c);
		}

		void TFT_ST7735::writedata8_cont(uint8_t c)
		{
			enableDataStream();
			spiwrite(c);
		}

		void TFT_ST7735::writedata16_cont(uint16_t d)
		{
			enableDataStream();
			spiwrite16(d);
		}

		void TFT_ST7735::writecommand_last(const uint8_t c)
		{
			enableCommandStream();
			spiwrite(c);
			disableCS();
		}


		void TFT_ST7735::writedata8_last(uint8_t c)
		{
			enableDataStream();
			spiwrite(c);
			disableCS();
		}

		void TFT_ST7735::writedata16_last(uint16_t d)
		{
			enableDataStream();
			spiwrite16(d);
			disableCS();
		}
	#endif
/* ========================================================================
					    Fast Common Graphic Primitives
   ========================================================================*/
	void TFT_ST7735::setAddrWindow_cont(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool disComp)
	{
		if (!disComp){//if false, offset compensate?
			x0 += TFT_ST7735_OFST[_rotation][0];
			x1 += TFT_ST7735_OFST[_rotation][0];
			y0 += TFT_ST7735_OFST[_rotation][1];
			y1 += TFT_ST7735_OFST[_rotation][1];
		}
		writecommand_cont(CMD_CLMADRS); //Column
		writedata16_cont(x0); writedata16_cont(x1);
		writecommand_cont(CMD_PGEADRS); //Page
		writedata16_cont(y0); writedata16_cont(y1);
		writecommand_cont(CMD_RAMWR); //Into ILI Ram
	}
	
	
	void TFT_ST7735::drawFastHLine_cont(int16_t x, int16_t y, int16_t w, uint16_t color)
	{
		setAddrWindow_cont(x, y, x + w - 1, y);
		do { writedata16_cont(color); } while (--w > 0);
	}

	void TFT_ST7735::drawFastVLine_cont(int16_t x, int16_t y, int16_t h, uint16_t color)
	{
		setAddrWindow_cont(x, y, x, y + h - 1);
		do { writedata16_cont(color); } while (--h > 0);
	}

	void TFT_ST7735::drawPixel_cont(int16_t x, int16_t y, uint16_t color)
	{
		setAddrWindow_cont(x, y, x + 1, y + 1);
		writedata16_cont(color);
	}

	bool TFT_ST7735::boundaryCheck(int16_t x,int16_t y)
	{
		if ((x >= _width) || (y >= _height)) return true;
		return false;
	}

	int16_t TFT_ST7735::sizeCheck(int16_t origin,int16_t len,int16_t maxVal)
	{
		if (((origin + len) - 1) >= maxVal) len = maxVal - origin;
		return len;
	}
#endif

/*
void TFT_ST7735::printPacket(word data,uint8_t count){
  for (int i=count-1; i>=0; i--){
    if (bitRead(data,i)==1){
      Serial.print("1");
    } 
    else {
      Serial.print("0");
    }
  }
  Serial.print(" -> 0x");
  if (count == 8){
	  Serial.print((byte)data,HEX);
  } else {
	  Serial.print(data,HEX);
  }
  Serial.print("\n");
}
*/
