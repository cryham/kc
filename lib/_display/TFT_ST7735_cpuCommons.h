#ifndef __TFT_ST7735_CPU_H
	#define __TFT_ST7735_CPU_H
	#include <stdint.h>
	//this are for the LCD Image Converter as workaround
	#define RLE_no  (0)
	#define RLE_yes (1)
	#define RLE_proportional (0)
	#define RLE_monospaced (1)
	/*
		_SPI_MULTITRANSFER
		Spi burst multy byte transfer support (experimental)
	*/
	
	#if defined(__MKL26Z64__) || defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
		//#include <avr/pgmspace.h>
		//#define _FORCE_PROGMEM__
		#define _smCharType	unsigned char
		static const uint32_t TFT_ST7735_SPI_SPEED 		= 30000000;
		#if !defined(__MKL26Z64__)
			#define _SPI_MULTITRANSFER	//enable burst multy byte transfer
		#endif
	#endif

	#if defined(_FORCE_PROGMEM__)
		template <typename T> T PROGMEM_read (const T * sce) { static T temp; memcpy_P (&temp, sce, sizeof (T)); return temp; }
	#endif
#endif
