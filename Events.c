/* ###################################################################
**     Filename    : Events.c
**     Project     : Display
**     Processor   : MKE04Z128VLD4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-11-01, 12:57, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Contents    :
**         Cpu_OnNMIINT - void Cpu_OnNMIINT(void);
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"
#include "Init_Config.h"
#include "PDD_Includes.h"
#include "button.h"
#include "display_UART.h"
#include "displayPosition.h"
#include "displayMemory.h"
#include "OLED.h"

#ifdef __cplusplus
extern "C" {
#endif


/* User includes (#include below this line is not maintained by Processor Expert) */
#include "stdbool.h"
#include "PE_Types.h"

#define PENTAIR_ICON_TIMER_COUNTER				40000
#define ICON_ALL_ON_TIMER_COUNTER				24000
#define ALARM_TIMER_COUNTER						8000  // 1 sec
#define PROBE_TIMER_COUNTER						16000
#define VALIDATION_SCREEN_TIMER_COUNTER			24000 // 3sec
#define BRIGHTNESS_TIMER_COUNTER				40000
#define BUTTON_TIMER_RESET_TIME					240000 // 30 sec

//Jason's variables
uint8  rx_cnt = 0;
uint16 second_cnt = 0;
_Bool  second_flag;
uint16 rx_timeout = 0;
uint16 tx_timeout = 0;
uint8  msecond_cnt = 0;
uint8  ti1_seconds = 0;
uint16 sec_1s_cnt = 0;

/*
** ===================================================================
**     Event       :  CI2C1_OnReceiveData (module Events)
**
**     Component   :  CI2C1 [InternalI2C]
**     Description :
**         This event is invoked when I2C finishes the reception of the
**         data successfully. This event is not available for the SLAVE
**         mode and if both RecvChar and RecvBlock are disabled. This
**         event is enabled only if interrupts/events are enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void CI2C1_OnReceiveData(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  CI2C1_OnTransmitData (module Events)
**
**     Component   :  CI2C1 [InternalI2C]
**     Description :
**         This event is invoked when I2C finishes the transmission of
**         the data successfully. This event is not available for the
**         SLAVE mode and if both SendChar and SendBlock are disabled.
**         This event is enabled only if interrupts/events are enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void CI2C1_OnTransmitData(void)
{
	I2CIsBusy = false;
}

/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
**
**     Component   :  TI1_125us [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TI1_OnInterrupt(void)
{
	/* pentair icon timer */
	if (TI1_pentairIconTimer_Flag)
	{
		TI1_pentairIconTimer += 1;
		if (TI1_pentairIconTimer > PENTAIR_ICON_TIMER_COUNTER)
		{
			updateDisplayPosition = true;
			TI1_pentairIconTimer_Flag = false;
			TI1_pentairIconTimer = 0;
		}
	}

	/* icon all on timer */
	if (TI1_iconAllOnTimer_Flag)
	{
		TI1_iconAllOnTimer += 1;
		if (TI1_iconAllOnTimer > ICON_ALL_ON_TIMER_COUNTER)
		{
			updateDisplayPosition = true;
			TI1_iconAllOnTimer_Flag = false;
			TI1_iconAllOnTimer = 0;
		}
	}

	/* Update Screen Timer */
	if (TI1_updateScreenTimerIsOn)
	{
		TI1_updateScreenTimer += 1;
		if (TI1_updateScreenTimer > updateScreenRate)
		{
			TI1_updateScreenTimerIsOn = false;
			TI1_updateScreenTimer = 0;
			updateScreenTimerDone = true;
		}
	}

	/* OK Button */
	if (TI1_OKButtonTimerIsOn)
	{
		TI1_OKButtonTimer += 1;
		if (TI1_OKButtonTimer > BUTTON_TIMER_RESET_TIME)
		{
			TI1_OKButtonTimer = 0;
		}
	}

	/* Back Button */
	if (TI1_backButtonTimerIsOn)
	{
		TI1_backButtonTimer += 1;
		if (TI1_backButtonTimer > BUTTON_TIMER_RESET_TIME)
		{
			TI1_backButtonTimer = 0;
		}
	}

	/* Up Button */
	if (TI1_upButtonTimerIsOn)
	{
		TI1_upButtonTimer += 1;
		if (TI1_upButtonTimer > BUTTON_TIMER_RESET_TIME)
		{
			TI1_upButtonTimer = 0;
		}
	}

	/* Down Button */
	if (TI1_downButtonTimerIsOn)
	{
		TI1_downButtonTimer += 1;
		if (TI1_downButtonTimer > BUTTON_TIMER_RESET_TIME)
		{
			TI1_downButtonTimer = 0;
		}
	}

	/* Check Pixels Timer */
	if (TI1_checkPixelsTimer_Flag)
	{
		TI1_checkPixelsTimer += 1;
		if (TI1_checkPixelsTimer == BRIGHTNESS_TIMER_COUNTER)
		{
			TI1_checkPixelsTimer_Flag = false;
			TI1_checkPixelsTimer = 0;
			currentPosition.displayLevel = MAIN_SCREEN_POSITION;
			refreshScreen = true;
			updateDisplayPosition = true;
		}
	}

	/* Idle Timer */
	if (TI1_idleTimer_Flag)
	{
		TI1_idleTimer += 1;
		if (TI1_idleTimer > TI1_idleTimer_cnt)
		{
			TI1_idleTimer_Flag = false;
			TI1_idleTimer = 0;
			updateDisplayPosition = true;
			currentPosition.displayLevel = MAIN_SCREEN_POSITION;
			currentPosition.lineNumber = INLET_LINENUM;
		}
	}

	/* Validation Screen Timer */
	if (TI1_validationScreenTimer_Flag)
	{
		TI1_validationScreenTimer += 1;
		if (TI1_validationScreenTimer == VALIDATION_SCREEN_TIMER_COUNTER)
		{
			TI1_validationScreenTimer_Flag = false;
			TI1_validationScreenTimer = 0;
			switch (currentPosition.displayLevel)
			{
			case COOLING_SET_POINT_POSITION:
		        currentPosition.lineNumber = 1;
				currentPosition.displayLevel = USER_INTERFACE_POSITION;
				break;

			case COOLING_DIF_POSITION:
		        currentPosition.lineNumber = 2;
				currentPosition.displayLevel = USER_INTERFACE_POSITION;
				break;

			case HEATING_SET_POINT_POSITION:
		        currentPosition.lineNumber = 3;
				currentPosition.displayLevel = USER_INTERFACE_POSITION;
				break;

			case HEATING_DIF_POSITION:
		        currentPosition.lineNumber = 4;
				currentPosition.displayLevel = USER_INTERFACE_POSITION;
				break;

			case HIGH_TEMP_ALARM_SP_POSITION:
		        currentPosition.lineNumber = 5;
				currentPosition.displayLevel = USER_INTERFACE_POSITION;
				break;

			case LOW_TEMP_ALARM_SP_POSITION:
		        currentPosition.lineNumber = 6;
				currentPosition.displayLevel = USER_INTERFACE_POSITION;
				break;

			case TEMP_SCALE_POSITION:
		        currentPosition.lineNumber = 7;
				currentPosition.displayLevel = USER_INTERFACE_POSITION;
				break;

			case HYSTERESIS_POSITION:
		        currentPosition.lineNumber = 8;
				currentPosition.displayLevel = USER_INTERFACE_POSITION;
				break;

			case PASSWORD_POSITION:
		        currentPosition.lineNumber = 9;
				currentPosition.displayLevel = USER_INTERFACE_POSITION;
				break;

			case DOOR_SMOKE_ALARM_POSITION:
		        currentPosition.lineNumber = 1;
				currentPosition.displayLevel = SYSTEM_INTERFACE_POSITION;
				break;

			case DISABLE_SWITCH_POSITION:
		        currentPosition.lineNumber = 2;
				currentPosition.displayLevel = SYSTEM_INTERFACE_POSITION;
				break;

			case ALARM_RELAY_OUTPUT_POSITION:
		        currentPosition.lineNumber = 3;
				currentPosition.displayLevel = SYSTEM_INTERFACE_POSITION;
				break;

			case COMPRESSOR_RESTART_DELAY_POSITION:
		        currentPosition.lineNumber = 4;
				currentPosition.displayLevel = SYSTEM_INTERFACE_POSITION;
				break;

			default: break;
			}
			refreshScreen = true;
			updateDisplayPosition = true;
		}
	}

	/* Alarm Timer */
	if (TI1_alarmTimer_Flag)
	{
		TI1_alarmTimer += 1;
		if (TI1_alarmTimer == ALARM_TIMER_COUNTER)
		{
			TI1_alarmTimer = 0;
			TI1_alarmTimer_Flag = false;

			while ((!alarmOutput[alarmOutputIndex]) && (alarmOutputIndex < NUMBER_OF_ALARMS))
			{
				alarmOutputIndex += 1;
			}

			if (alarmOutputIndex >= NUMBER_OF_ALARMS)
			{
				alarmOutputIndex = 0;
				currentPosition.displayLevel = MAIN_SCREEN_POSITION;
		        currentPosition.lineNumber = INLET_LINENUM;
			}
			else
			{
				currentPosition.displayLevel = alarmStartPosition + alarmOutputIndex;
				alarmOutputIndex += 1;
			}
			refreshScreen = true;
			updateDisplayPosition = true;
		}
	}

	// Jason's X msecond(s) one shot flag
	if (++msecond_cnt>8)
	{
		rx_timeout++;
		tx_timeout++;
		msecond_cnt = 0;
	}
	if (rx_timeout>RX_MAX_WAIT)	{
		rx_timeout = RX_MAX_WAIT;
	}
	if (tx_timeout>TX_MAX_WAIT)	{
		tx_timeout = TX_MAX_WAIT;
	}

	// Jason's X/16th second(s) one shot flag
	if (++sec_1s_cnt>500/*8000*/)
	{
		ti1_seconds++;
		sec_1s_cnt = 0;
	}
	if (ti1_seconds>TX_DELAY)	{
		ti1_seconds = TX_DELAY;
	}
}

/*
** ===================================================================
**     Event       :  Display_OnBlockReceived (module Events)
**
**     Component   :  Display [Serial_LDD]
*/
/*!
**     @brief
**         This event is called when the requested number of data is
**         moved to the input buffer.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void Display_OnBlockReceived(LDD_TUserData *UserDataPtr)
{
	// set data received flag
	DataReceivedFlg = TRUE;													// indicate new byte received
}

/*
** ===================================================================
**     Event       :  Display_OnBlockSent (module Events)
**
**     Component   :  Display [Serial_LDD]
*/
/*!
**     @brief
**         This event is called after the last character from the
**         output buffer is moved to the transmitter.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer is passed
**                           as the parameter of Init method.
*/
/* ===================================================================*/
void Display_OnBlockSent(LDD_TUserData *UserDataPtr)
{
	// set data sent flag
	DataSentFlg = true;
}

/*
** ===================================================================
**     Event       :  Cpu_OnNMI (module Events)
**
**     Component   :  Cpu [MKE04Z128LD4]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMI(void)
{
  /* Write your code here ... */
}

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
