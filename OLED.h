#ifndef __OLED_H
#define __OLED_H

#include "PE_Types.h"
#include "string.h"

/* Defines */

#define SA0								0
#define OLED_DISPLAY_OFF            	0xAE
#define OLED_SET_DISPLAY_CLOCK_DIV		0xD5

	#define OLED_SETSTARTLINE               0x40
	#define OLED_SETCONTRAST                0x81
	#define OLED_SEGREMAP_SET               0xA1
	#define OLED_DISPLAYALLON_RESUME        0xA4
	#define OLED_DISPLAYALLON               0xA5
	#define OLED_NORMALDISPLAY              0xA6
	#define OLED_INVERTDISPLAY              0xA7
	#define OLED_SETMULTIPLEX               0xA8
	#define OLED_DISPLAYON                  0xAF
	#define OLED_COMSCANDEC_MAP             0xC8
	#define OLED_SETDISPLAYOFFSET           0xD3
	#define OLED_SETPRECHARGE               0xD9
	#define OLED_SETCOMPINS                 0xDA
	#define OLED_SETVCOMDETECT              0xDB

	#define I2CBufferSize		1025

/* Global Variables */

	extern uint8 I2CDataBuffer[8][128];
	extern 	uint8 I2CDataToSend[I2CBufferSize]; /* Don't know why increase to 1225 bytes fix the problem */
	extern _Bool updateScreen;

/* Function Prototypes */

	void clearDisplay(void);
	void fillDisplay(void);
	void clearArea(int startPage, int startColumn, int endPage, int endColumn);
	void OLED_init (void);
	void displayAllOn(void);
	void I2CSendData(void);

#endif /* __OLED_H */
