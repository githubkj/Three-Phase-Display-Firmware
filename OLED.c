#include "OLED.h"
#include "Cpu.h"
#include "Events.h"
#include "RES.h"
#include "BitIoLdd1.h"
#include "R_W.h"
#include "BitIoLdd2.h"
#include "E_RD.h"
#include "BitIoLdd3.h"
#include "D_C.h"
#include "BitIoLdd4.h"
#include "CS.h"
#include "BitIoLdd5.h"
#include "CI2C1.h"
#include "IntI2cLdd1.h"
#include "TI1_125us.h"
#include "TimerIntLdd1.h"
#include "TU1.h"
#include "PE_Types.h"
#include "string.h"
#include "stdbool.h"
#include "digit.h"
#include "icon.h"

/* Function Prototypes */
static void OLED_WriteCommand (uint8 commandToWrite);
static void setHorizontalMode(void);

void clearDisplay(void)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 128; j++)
		{
			I2CDataBuffer[i][j] = 0;
		}
	}
}

void fillDisplay(void)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 128; j++)
		{
			I2CDataBuffer[i][j] = 0xFF;
		}
	}
}

void clearArea(int startPage, int startColumn, int endPage, int endColumn)
{
	for (int i = startPage; i <= endPage; i++)
	{
		for (int j = startColumn; j <= endColumn; j++)
		{
			I2CDataBuffer[i][j] = 0;
		}
	}
}

void OLED_init(void)
{
	/* Reset OLED */
	RES_ClrVal();

	/* OLED MCU interface selection: I2C */
	E_RD_ClrVal();
	R_W_ClrVal();
	CS_ClrVal();
	D_C_PutVal(SA0);
	RES_SetVal();									// clearing this line resets the display driver
	CI2C1_SelectSlave(0x3Cu);						// set the slave 7-bit address

	OLED_WriteCommand(OLED_DISPLAY_OFF);
	OLED_WriteCommand(OLED_SET_DISPLAY_CLOCK_DIV);
	OLED_WriteCommand(0x80);  // the suggested ratio 0x80
    OLED_WriteCommand(OLED_SETMULTIPLEX); // 0xA8
    OLED_WriteCommand(0x3F);  // 0x3F 1/64 duty
	OLED_WriteCommand(OLED_SETDISPLAYOFFSET); // 0xD3
	OLED_WriteCommand(0x0); // no offset
	OLED_WriteCommand(OLED_SETSTARTLINE); // line #0
	OLED_WriteCommand(OLED_SEGREMAP_SET); //0xA0
	OLED_WriteCommand(OLED_COMSCANDEC_MAP); //0xC0
	OLED_WriteCommand(OLED_SETCOMPINS); // 0xDA
	OLED_WriteCommand(0x12); // disable COM left/right remap
	OLED_WriteCommand(OLED_SETCONTRAST); //0X81
	OLED_WriteCommand(0x80); //RESET VALUE
	OLED_WriteCommand(OLED_SETPRECHARGE); // 0xd9
	OLED_WriteCommand(0xF1);
	OLED_WriteCommand(OLED_SETVCOMDETECT); // 0xDB
	OLED_WriteCommand(0x34);//RESET
	OLED_WriteCommand(OLED_DISPLAYALLON_RESUME); // 0xA4
	OLED_WriteCommand(OLED_NORMALDISPLAY); // 0xA6
	OLED_WriteCommand(OLED_DISPLAYON);//--turn on oled panel

	setHorizontalMode();
}

void displayAllOn(void)
{
	// symbols at bottom
	uint8 column = 15;
	showIcon(bellIcon, 14, 2, 6, column);
	column+=19;
	showIcon(resisterIcon, 14, 2, 6, column);
	column+=19;
	showIcon(condensorIcon, 14, 2, 6, column);
	column+=19;
	showIcon(fanIcon, 14, 2, 6, column);
	column+=19;
	showIcon(evapIcon, 14, 2, 6, column);
	column+=19;
	showIcon(compIcon, 14, 2, 6, column);
	// symbols at left side
	showIcon(sunIcon, 14, 2, 0, 0);
	showIcon(snowFlakeIcon, 14, 2, 4, 0);
	// Digits
	showIntegerRightAligned(digitCalibriLight4x28, 888, 4, 28, 1, 110);
	showIcon(decimal_point, DECIMAL_POINT_WIDTH, 1, 4, 110-28);
	showIcon(degreeF, 8, 2, 1, 115);
	showIcon(degreeC, 8, 2, 3, 115);
}

void OLED_WriteCommand(uint8 commandToWrite)
{
	word tx_cnt;
	byte error;
	uint8 dataToSend[2];
	dataToSend[0] = 0x80;
	dataToSend[1] = commandToWrite;
	I2CIsBusy = true;
	error =  CI2C1_SendBlock( &dataToSend, 2, &tx_cnt );
	while(I2CIsBusy);
	int i = 0;
	while(i<16000)
	{
		i++;
	}
}

void I2CSendData(void)
{
    if (updateScreen)
    {
        if (!I2CIsBusy)
        {
            word tx_cnt;
            byte error;
            int j = 1;
            for (int i = 0; i < 8; i++)
            {
                for (int x = 0; x < 128; x++)
                {
                    I2CDataToSend[j++] = I2CDataBuffer[i][x];
                }
            }
            I2CIsBusy = TRUE;
            error = CI2C1_SendBlock(&I2CDataToSend, I2CBufferSize, &tx_cnt);
            
            updateScreen = FALSE;
        }
    }
}

void setHorizontalMode(void)
{
	word tx_cnt;
	byte error;
	uint8 dataToSend[16] = {0x80, 0x20, 0x80, 0x00, 0x80, 0x21, 0x80, 0, 0x80, 127, 0x80, 0x22, 0x80, 0, 0x80, 7};
	I2CIsBusy = true;
	error = CI2C1_SendBlock(&dataToSend, 16, &tx_cnt);
	while(I2CIsBusy);
}
