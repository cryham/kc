#include "TFT_ST7735.h"


/********************** constructos **********************/

#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)

TFT_ST7735::TFT_ST7735(const uint8_t cspin, const uint8_t dcpin,
	const uint8_t rstpin, const uint8_t mosi, const uint8_t sclk)
{
	_cs   = cspin;
	_dc   = dcpin;
	_rst  = rstpin;
	_mosi = mosi;
	_sclk = sclk;
}
#endif


/*********************************************************
************** Var init and SPI preparation **************
**********************************************************/
void TFT_ST7735::begin(bool avoidSPIinit) 
{
	//  initialize Vars
	uint8_t i;
	_currentMode = 0b00000000;
	_portrait = false;
	_initError = 0b00000000;
	_defaultBgColor = _ST7735_BACKGROUND;
	_defaultFgColor = _ST7735_FOREGROUND;
	_Mactrl_Data = 0b11000000; //0b11000000 //0b11001000

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
	if (_rst != 255)
	{
		pinMode(_rst, OUTPUT);
		digitalWrite(_rst, HIGH);  delay(10);
		digitalWrite(_rst, LOW);   delay(10);
		digitalWrite(_rst, HIGH);  delay(10);
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
	if (TFT_ST7735_GAMMASET < 5)
	{	//Positive Gamma Correction Setting
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

	//Fill background with default color
	fillScreen(_defaultBgColor);
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
uint8_t TFT_ST7735::getErrorCode()
{
	return _initError;
}

void TFT_ST7735::clearMemory()
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
	if (m != _currentMode)
	{
		startTransaction();
		switch(m)
		{
		case NORMAL:
			if (_currentMode == 6) {//was in off display?
				writecommand_cont(CMD_DISPON);
			}
			if (_currentMode == 2) {//was in idle?
				writecommand_cont(CMD_IDLEOF);
			}
			if (_currentMode == 3) {//was in sleep?
				writecommand_cont(CMD_SLPOUT);
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
			return;
		case INVERT:
			writecommand_cont(CMD_DINVON);
			_currentMode = 4;
			break;
		case DISP_ON:
			writecommand_cont(CMD_DISPON);
			_currentMode = 5;
			break;
		case DISP_OFF:
			writecommand_cont(CMD_DISPOFF);
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
	}
}


uint8_t TFT_ST7735::getMode()
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


uint8_t TFT_ST7735::getRotation()
{
	return _rotation;
}

void TFT_ST7735::setRotation(uint8_t m) 
{
	_rotation = m % 4; // can't be higher than 3
	_Mactrl_Data &= ~(0xF8); //clear bit 3..7,  F0 -> clear bit 4...7
	if (_rotation == 0){
		_portrait = false;
		_width  = TFT_ST7735_TFTWIDTH;
		_height = TFT_ST7735_TFTHEIGHT;
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


int16_t TFT_ST7735::width() const {		return _width;	}
int16_t TFT_ST7735::height() const {	return _height;	}


/*********************************************************
******************** Color Functions *********************
**********************************************************/
#if 0
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
	  r1.  g1:  b1:
	  r2:  g2:  b2:
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
#endif

void TFT_ST7735::setBackground(uint16_t color) 
{
	_defaultBgColor = color;
}


void TFT_ST7735::setForeground(uint16_t color) 
{
	_defaultFgColor = color;
}

uint16_t TFT_ST7735::getBackground()
{
	return _defaultBgColor;
}

uint16_t TFT_ST7735::getForeground()
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


void TFT_ST7735::clearScreen()
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
