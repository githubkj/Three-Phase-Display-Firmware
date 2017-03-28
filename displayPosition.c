#include "displayPosition.h"
#include "icon.h"
#include "digit.h"
#include "OLED.h"
#include "PE_Types.h"
#include "button.h"
#include "text.h"
#include "Events.h"
#include "display_UART.h"
#include "stdbool.h"
#include "displayMemory.h"

uint16_union modbus_ro_reg_rcv[RO_REG_LEN];					// read only registers controller copy
byte modbus_rw_coil_rcv[RW_COIL_LEN/8+1];					// read/write coils controller copy

#define USER_LEVEL_PASSWORD		22
#define SYSTEM_LEVEL_PASSWORD	66

/* Function Prototypes */
static void mainScreenPosition(void);
static void mainMenuPosition(void);
static void userInterfacePosition(void);
static void systemInterfacePosition(void);
static void enterPasswordPosition(void);
static void aboutMenuPosition(void);

static void turnOnAllPixelsPosition(void);

static void alarmPosition(void);

static void coolingSetPointPosition(void);
static void coolingDifferentialPosition(void);
static void heatingSetPointPosition(void);
static void heatingDifferentialPosition(void);
static void highTempAlarmPosition(void);
static void lowTempAlarmPosition(void);
static void tempScalePosition(void);
static void hysteresisPosition(void);
static void languagePosition(void);
static void passwordPosition(void);

static void doorSmokeAlarmPosition(void);
static void disableSwitchAlarmPosition(void);
static void alarmRelayOutputPosition(void);
static void compressorRestartDelayPosition(void);
static void compressorCurrentPosition(void);
static void systemSettingsPosition(void);
static void currentLimitSettingsPosition(void);
static void malfDeactivatePosition(void);
static void malfActivatePosition(void);
static void hpcFanOnPosition(void);
static void hpcFanOffPosition(void);

void checkDisplayPosition(void)
{
	if (updateDisplayPosition)
	{
		updateDisplayPosition = false;
		updateDisplayMemory = true;
		switch (currentPosition.displayLevel)
		{
		/******************** Startup Screens **************************/
			/* Display Pentair Logo for 1 second at Start-up */
			case PENTAIR_ICON_POSITION:
				refreshScreen = true;
				currentPosition.displayLevel = ICON_ALL_ON_POSITION;
				break;

			/* Display All Icons for 1 second at Start-up */
			case ICON_ALL_ON_POSITION:
				refreshScreen = true;
				currentPosition.displayLevel = MAIN_SCREEN_POSITION;
				currentPosition.lineNumber = INLET_LINENUM;
				break;
		/******************** Startup Screens **************************/

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

			// Main Screen
			case MAIN_SCREEN_POSITION:
				mainScreenPosition();
				break;

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

			/******************** Main Menu **************************/
			case MAIN_MENU_POSITION:
				mainMenuPosition();
				break;

			case USER_INTERFACE_POSITION:
				userInterfacePosition();
				break;

			case SYSTEM_INTERFACE_POSITION:
				systemInterfacePosition();
				break;

			case USER_PASSWORD_POSITION: case SYSTEM_PASSWORD_POSITION:
				enterPasswordPosition();
				break;

			case ABOUT_POSITION:
				aboutMenuPosition();
				break;
			/******************** Main Menu **************************/

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

			/******************** User Interface **************************/
			case COOLING_SET_POINT_POSITION:
				coolingSetPointPosition();
				break;

			case COOLING_DIF_POSITION:
				coolingDifferentialPosition();
				break;

			case HEATING_SET_POINT_POSITION:
				heatingSetPointPosition();
				break;

			case HEATING_DIF_POSITION:
				heatingDifferentialPosition();
				break;

			case HIGH_TEMP_ALARM_SP_POSITION:
				highTempAlarmPosition();
				break;

			case LOW_TEMP_ALARM_SP_POSITION:
				lowTempAlarmPosition();
				break;

			case TEMP_SCALE_POSITION:
				tempScalePosition();
				break;

			case HYSTERESIS_POSITION:
				hysteresisPosition();
				break;

			case PASSWORD_POSITION:
				passwordPosition();
				break;

			case LANGUAGE_MENU_POSITION:
				languagePosition();
				break;
			/******************** User Interface **************************/

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

			/******************** System Interface ************************/
			case DOOR_SMOKE_ALARM_POSITION:
				doorSmokeAlarmPosition();
				break;

			case DISABLE_SWITCH_POSITION:
				disableSwitchAlarmPosition();
				break;

			case ALARM_RELAY_OUTPUT_POSITION:
				alarmRelayOutputPosition();
				break;

			case COMPRESSOR_RESTART_DELAY_POSITION:
				compressorRestartDelayPosition();
				break;

			case COMPRESSOR_CURRENT_POSITION:
				compressorCurrentPosition();
				break;

			// System Settings
			case DISPLAY_SYS_SETTINGS_MENU_POSITION:
				systemSettingsPosition();
				break;

			case CURRENT_LIMIT_SETTINGS_POSITION:
				currentLimitSettingsPosition();
				break;

			case MALF_DEACTIVATE_POSITION:
				malfDeactivatePosition();
				break;

			case MALF_ACTIVATE_POSITION:
				malfActivatePosition();
				break;

			case HPC_FAN_ON_POSITION:
				hpcFanOnPosition();
				break;

			case HPC_FAN_OFF_POSITION:
				hpcFanOffPosition();
				break;
			/******************** System Interface **************************/

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

			// Push four buttons at same time
			case CHECK_ALL_PIXELS:
				turnOnAllPixelsPosition();
				break;

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

			/******************** Alarms ***********************/
			case HIGH_TEMP_ALARM_POSITION: case LOW_TEMP_ALARM_POSITION:
			case DOOR_ALARM_POSITION: case PHASE_MISSING_ALARM_POSITION: case EVAP_MI_THERMAL_OVERLOAD_ALARM_POSITION:
			case CONDENSOR_MI_THERMAL_OVERLOAD_ALARM_POSITION: case COMPRESSOR_THERMAL_OVERLOAD_ALARM_POSITION:
			case OVER_VOLTAGE_ALARM_POSITION: case UNDER_VOLTAGE_ALARM_POSITION:
			case BAD_BOARD_ALARM_POSITION: case HIGH_PRESSURE_ALARM_POSITION:
			case LOW_PRESSURE_ALARM_POSITION: case FROST_ALARM_POSITION:
			case DISABLE_SWITCH_ALARM_POSITION: case COIL_TEMP_SENSOR_FAULT_ALARM_POSITION:
			case INLET_TEMP_SENSOR_FAULT_ALARM_POSITION: case OUTLET_TEMP_SENSOR_FAULT_ALARM_POSITION:
			case OVER_CURRENT_ALARM_POSITION: case COMPRESSOR_RESTART_DELAY_COUNT_DOWN_POSITION: case COMPRESSOR_RESTART_DELAY_TIMER_POSITION:
				alarmPosition();
				break;
			/******************** Alarms ***********************/

			default: break;
		}
	}
}

void mainScreenPosition(void)
{
	if (releasedOK)
	{
		releasedOK = false;
		// Turn off alarm timer and clear its counter
		TI1_alarmTimer_Flag = false;
		TI1_alarmTimer = 0;
		if (currentPosition.lineNumber == INLET_LINENUM)
		{
			currentPosition.displayLevel = MAIN_MENU_POSITION;
			currentPosition.lineNumber   = 1;
		}
		else
		{
			currentPosition.lineNumber   = INLET_LINENUM;
		}
	}
	else if (releasedBack)
	{
		releasedBack = false;
		if (currentPosition.lineNumber != INLET_LINENUM)
		{
			currentPosition.lineNumber   = INLET_LINENUM;
		}
	}
	else if (releasedUp)
	{
		releasedUp = false;
		refreshScreen = true;
		if (currentPosition.lineNumber == INLET_LINENUM)
		{
			currentPosition.lineNumber = COIL_LINENUM;
		}
		else if (currentPosition.lineNumber == COIL_LINENUM)
		{
			if (modbus_rw_coil_rcv[PROBE2_PRESENT/8] & PROBE2_PRESENT_F)
			{
				currentPosition.lineNumber = OUTLET_LINENUM;
			}
			else
			{
				currentPosition.lineNumber = INLET_LINENUM;
			}
		}
		else if (currentPosition.lineNumber == OUTLET_LINENUM)
		{
			currentPosition.lineNumber = INLET_LINENUM;
		}
	}
	else if (releasedDown)
	{
		releasedDown = false;
		refreshScreen = true;
		if (currentPosition.lineNumber == COIL_LINENUM)
		{
			currentPosition.lineNumber = INLET_LINENUM;
		}
		else if (currentPosition.lineNumber == INLET_LINENUM)
		{
			if (modbus_rw_coil_rcv[PROBE2_PRESENT/8] & PROBE2_PRESENT_F)
			{
				currentPosition.lineNumber = OUTLET_LINENUM;
			}
			else
			{
				currentPosition.lineNumber = COIL_LINENUM;
			}
		}
		else if (currentPosition.lineNumber == OUTLET_LINENUM)
		{
			currentPosition.lineNumber = COIL_LINENUM;
		}
	}
}

void mainMenuPosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = MAIN_SCREEN_POSITION;
        currentPosition.lineNumber   = INLET_LINENUM;
		refreshScreen = true;
    }
    else if (releasedOK)
    {
    	releasedOK = false;
        switch (currentPosition.lineNumber)
        {
			case 1:
				if (modbus_rw_coil_rcv[PASSWORD_ENABLED/8] & PASSWORD_ENABLED_F)
				{
					userInput = 0;
					currentPosition.displayLevel = USER_PASSWORD_POSITION;
					refreshScreen = true;
				}
				else
				{
					currentPosition.displayLevel = USER_INTERFACE_POSITION;
				}
				break;

			case 2:
				userInput = 0;
				currentPosition.displayLevel = SYSTEM_PASSWORD_POSITION;
				refreshScreen = true;
				break;

			case 3:
				currentPosition.displayLevel = ABOUT_POSITION;
				break;

			default: break;
        }
        currentPosition.lineNumber = 1;
    }
    else if (releasedUp)
    {
    	releasedUp = false;
		switch (currentPosition.lineNumber)
		{
			case 1: case 2:
				currentPosition.lineNumber = 1;
				break;

			case 3:
				currentPosition.lineNumber = 2;
				break;

			default: break;
		}
    }
    else if (releasedDown)
    {
    	releasedDown = false;
		switch (currentPosition.lineNumber)
		{
			case 1:
				currentPosition.lineNumber = 2;
				break;

			case 2: case 3:
				currentPosition.lineNumber = 3;
				break;

			default: break;
		}
    }
}

void enterPasswordPosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = MAIN_MENU_POSITION;
        currentPosition.lineNumber   = 1;
    }
    else if (releasedOK)
    {
    	releasedOK = false;
    	if ((userInput == USER_LEVEL_PASSWORD) && (currentPosition.displayLevel == USER_PASSWORD_POSITION))
    	{
            currentPosition.displayLevel = USER_INTERFACE_POSITION;
            currentPosition.lineNumber   = 1;
    	}
    	else if ((userInput == SYSTEM_LEVEL_PASSWORD) && (currentPosition.displayLevel == SYSTEM_PASSWORD_POSITION))
    	{
            currentPosition.displayLevel = SYSTEM_INTERFACE_POSITION;
            currentPosition.lineNumber   = 1;
    	}
    	else
    	{
            currentPosition.displayLevel = MAIN_MENU_POSITION;
            currentPosition.lineNumber   = 1;
    	}
    }
    else if (releasedUp)
    {
    	releasedUp = false;
		userInput += 1;
    	if (userInput > 99)
    	{
    		userInput = 0;
    	}
    }
    else if (releasedDown)
    {
    	releasedDown = false;
		userInput -= 1;
    	if (userInput < 0)
    	{
    		userInput = 99;
    	}
    }
	else if (heldUp)
	{
		if (updateScreenTimerDone)
		{
			updateScreenTimerDone = false;

			userInput += 1;
	    	if (userInput > 99)
	    	{
	    		userInput = 0;
	    	}
		}
		else
		{
			if (!TI1_updateScreenTimerIsOn)
			{
				updateScreenRate = BUTTON_HELD_REFRESH_TIME;
				TI1_updateScreenTimerIsOn = true;
			}
		}
	}
	else if (heldDown)
	{
		if (updateScreenTimerDone)
		{
			updateScreenTimerDone = false;

			userInput -= 1;
	    	if (userInput < 0)
	    	{
	    		userInput = 99;
	    	}
		}
		else
		{
			if (!TI1_updateScreenTimerIsOn)
			{
				updateScreenRate = BUTTON_HELD_REFRESH_TIME;
				TI1_updateScreenTimerIsOn = true;
			}
		}
	}
}

void userInterfacePosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = MAIN_MENU_POSITION;
        currentPosition.lineNumber   = 1;
    }
    else if (releasedOK)
    {
    	releasedOK = false;
		refreshScreen = true;
    	parameterIsSet = false;
        switch (currentPosition.lineNumber)
        {
            case 1:
                currentPosition.displayLevel = COOLING_SET_POINT_POSITION;
            	userInput = modbus_rw_reg_rcv[COOLING_SP].ivalue;
                break;

            case 2:
                currentPosition.displayLevel = COOLING_DIF_POSITION;
            	userInput = modbus_rw_reg_rcv[COOLING_DIF_1].ivalue;
                break;

            case 3:
                currentPosition.displayLevel = HEATING_SET_POINT_POSITION;
            	userInput = modbus_rw_reg_rcv[HEATING_SP].ivalue;
                break;

            case 4:
                currentPosition.displayLevel = HEATING_DIF_POSITION;
            	userInput = modbus_rw_reg_rcv[HEATING_DIF].ivalue;
                break;

            case 5:
                currentPosition.displayLevel = HIGH_TEMP_ALARM_SP_POSITION;
            	userInput = modbus_rw_reg_rcv[HIGH_TEMP_ALARM_SP].ivalue;
                break;

            case 6:
                currentPosition.displayLevel = LOW_TEMP_ALARM_SP_POSITION;
            	userInput = modbus_rw_reg_rcv[LOW_TEMP_ALARM_SP].ivalue;
                break;

            case 7:
                currentPosition.displayLevel = TEMP_SCALE_POSITION;
				if (modbus_rw_coil_rcv[UNIT_OF_MEASURE/8] & UNIT_OF_MEASURE_F)
				{
					currentPosition.lineNumber = 1; // Degree F
				}
				else
				{
					currentPosition.lineNumber = 2; // Degree C
				}
                break;

            case 8:
            	currentPosition.displayLevel = HYSTERESIS_POSITION;
				if (modbus_rw_coil_rcv[COOL_HYSTERESIS/8] & COOL_HYSTERESIS_F)
				{
					currentPosition.lineNumber = 2; // Negative
				}
				else
				{
					currentPosition.lineNumber = 1; // Positive
				}
                break;

            case 9:
            	currentPosition.displayLevel = LANGUAGE_MENU_POSITION;
				currentPosition.lineNumber = 1;
            	break;

            case 10:
            	currentPosition.displayLevel = PASSWORD_POSITION;
				if (modbus_rw_coil_rcv[PASSWORD_ENABLED/8] & PASSWORD_ENABLED_F)
				{
					currentPosition.lineNumber = 2;
				}
				else
				{
					currentPosition.lineNumber = 1;
				}
                break;

            default:
                break;
        }
    }
    else if (releasedUp)
    {
    	releasedUp = false;
		if (currentPosition.lineNumber > 1)
		{
			currentPosition.lineNumber -= 1;
		}
		else
		{
			currentPosition.lineNumber = 1;
		}
    }
    else if (releasedDown)
    {
    	releasedDown = false;
		if (currentPosition.lineNumber < 10)
		{
			currentPosition.lineNumber += 1;
		}
		else
		{
			currentPosition.lineNumber = 10;
		}
    }
}

void systemInterfacePosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = MAIN_MENU_POSITION;
        currentPosition.lineNumber   = 2;
    }
    else if (releasedOK)
    {
    	releasedOK = false;
		refreshScreen = true;
    	parameterIsSet = false;
        switch (currentPosition.lineNumber)
        {
            case 1:
                currentPosition.displayLevel = DOOR_SMOKE_ALARM_POSITION;
				if (modbus_rw_coil_rcv[DOOR_SWITCH/8] & DOOR_SWITCH_F)
				{
					currentPosition.lineNumber = 2;
				}
				else
				{
					currentPosition.lineNumber = 1;
				}
                break;

            case 2:
                currentPosition.displayLevel = DISABLE_SWITCH_POSITION;
				if (modbus_rw_coil_rcv[DISABLE_SWITCH/8] & DISABLE_SWITCH_F)
				{
					currentPosition.lineNumber = 2;
				}
				else
				{
					currentPosition.lineNumber = 1;
				}
                break;

            case 3:
                currentPosition.displayLevel = ALARM_RELAY_OUTPUT_POSITION;
				if (modbus_rw_coil_rcv[ALARM_OUTPUT/8] & ALARM_OUTPUT_F)
				{
					currentPosition.lineNumber = 2;
				}
				else
				{
					currentPosition.lineNumber = 1;
				}
                break;

            case 4:
            	currentPosition.displayLevel = COMPRESSOR_RESTART_DELAY_POSITION;
            	userInput = modbus_rw_reg_rcv[COMP_MIN_STOP_TIME].ivalue;
                break;

            case 5:
                currentPosition.displayLevel = COMPRESSOR_CURRENT_POSITION;
                currentPosition.lineNumber = 1;
                break;

            case 6:
                currentPosition.displayLevel = DISPLAY_SYS_SETTINGS_MENU_POSITION;
                currentPosition.lineNumber = 1;
            	break;

            default:
                break;
        }
    }
    else if (releasedUp)
    {
    	releasedUp = false;
    	if (currentPosition.lineNumber > 1)
    	{
            currentPosition.lineNumber -= 1;
    	}
    	else
    	{
            currentPosition.lineNumber = 1;
        }
    }
    else if (releasedDown)
    {
    	releasedDown = false;
    	if (currentPosition.lineNumber < 6)
    	{
            currentPosition.lineNumber += 1;
    	}
    	else
    	{
            currentPosition.lineNumber = 6;
        }
    }
}

void aboutMenuPosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = MAIN_MENU_POSITION;
        currentPosition.lineNumber   = 3;
    }
    else if (releasedOK)
    {
    	releasedOK = false;
    }
    else if (releasedUp)
    {
    	releasedUp = false;
    }
    else if (releasedDown)
    {
    	releasedDown = false;
    }
}

void turnOnAllPixelsPosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
    }
    else if (releasedOK)
    {
    	releasedOK = false;
    }
    else if (releasedUp)
    {
    	releasedUp = false;
    }
    else if (releasedDown)
    {
    	releasedDown = false;
    }
}

void alarmPosition(void)
{
	if (releasedOK)
	{
		releasedOK = false;
		// Turn off alarm timer and clear its counter
		TI1_alarmTimer_Flag = false;
		TI1_alarmTimer = 0;
		currentPosition.displayLevel = MAIN_MENU_POSITION;
		currentPosition.lineNumber   = 1;
		refreshScreen = true;
	}
	else if (releasedBack)
	{
		releasedBack = false;
		currentPosition.displayLevel = MAIN_SCREEN_POSITION;
		currentPosition.lineNumber   = INLET_LINENUM;
		refreshScreen = true;
	}
	else if (releasedUp)
	{
		releasedUp = false;
		// Turn off alarm timer and clear its counter
		TI1_alarmTimer_Flag = false;
		TI1_alarmTimer = 0;
		currentPosition.displayLevel = MAIN_SCREEN_POSITION;
		currentPosition.lineNumber   = OUTLET_LINENUM;
		refreshScreen = true;
	}
	else if (releasedDown)
	{
		releasedDown = false;
		// Turn off alarm timer and clear its counter
		TI1_alarmTimer_Flag = false;
		TI1_alarmTimer = 0;
		currentPosition.displayLevel = MAIN_SCREEN_POSITION;
		currentPosition.lineNumber   = COIL_LINENUM;
		refreshScreen = true;
	}
}

void coolingSetPointPosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = USER_INTERFACE_POSITION;
        currentPosition.lineNumber = 1;
    }
    else if (releasedOK)
    {
    	releasedOK = false;
		refreshScreen = true;
    	uint16 uart_write_return;
    	// write to main board to update the coolingSP
    	uart_write_return = display_uart_update(REGISTER, RW_REG_START + COOLING_SP, false, userInput, COOLING_SP_F, 0);
    	// Validation Screen
    	parameterIsSet = true;
    }
    else if (releasedUp)
    {
    	releasedUp = false;
		if (userInput < modbus_rw_reg_rcv[COOLING_SP_MAX].ivalue)
		{
			userInput += 10;
			if (userInput > modbus_rw_reg_rcv[COOLING_SP_MAX].ivalue)
			{
				userInput = modbus_rw_reg_rcv[COOLING_SP_MAX].ivalue;
			}
		}
    }
    else if (releasedDown)
    {
    	releasedDown = false;
		if (userInput > modbus_rw_reg_rcv[COOLING_SP_MIN].ivalue)
		{
			userInput -= 10;
    		if (userInput < modbus_rw_reg_rcv[COOLING_SP_MIN].ivalue)
    		{
				userInput = modbus_rw_reg_rcv[COOLING_SP_MIN].ivalue;
    		}
		}
    }
	else if (heldUp)
	{
		if (updateScreenTimerDone)
		{
			if (userInput < modbus_rw_reg_rcv[COOLING_SP_MAX].ivalue)
			{
				userInput += 10;
				if (userInput > modbus_rw_reg_rcv[COOLING_SP_MAX].ivalue)
				{
					userInput = modbus_rw_reg_rcv[COOLING_SP_MAX].ivalue;
				}
			}
			updateScreenTimerDone = false;
		}
		else
		{
			if (!TI1_updateScreenTimerIsOn)
			{
				updateScreenRate = BUTTON_HELD_REFRESH_TIME;
				TI1_updateScreenTimerIsOn = true;
			}
		}
	}
	else if (heldDown)
	{
		if (updateScreenTimerDone)
		{
			if (userInput > modbus_rw_reg_rcv[COOLING_SP_MIN].ivalue)
			{
				userInput -= 10;
	    		if (userInput < modbus_rw_reg_rcv[COOLING_SP_MIN].ivalue)
	    		{
					userInput = modbus_rw_reg_rcv[COOLING_SP_MIN].ivalue;
	    		}
			}
			updateScreenTimerDone = false;
		}
		else
		{
			if (!TI1_updateScreenTimerIsOn)
			{
				updateScreenRate = BUTTON_HELD_REFRESH_TIME;
				TI1_updateScreenTimerIsOn = true;
			}
		}
	}
}

void coolingDifferentialPosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = USER_INTERFACE_POSITION;
        currentPosition.lineNumber = 2;
    }
    else if (releasedOK)
    {
    	releasedOK = false;
		refreshScreen = true;
    	uint16 uart_write_return;
    	uart_write_return = display_uart_update(REGISTER, RW_REG_START + COOLING_DIF_1, false, userInput, COOLING_DIF_1_F, 0);
    	// Validation Screen
    	parameterIsSet = true;
    }
    else if (releasedUp)
    {
    	releasedUp = false;
		userInput += 10;
    	if (userInput > 999)
    	{
    		userInput = 999;
    	}
    }
    else if (releasedDown)
    {
    	releasedDown = false;
    	userInput -= 10;
    	if (userInput < 0)
    	{
    		userInput = 0;
    	}
    }
	else if (heldUp)
	{
		if (updateScreenTimerDone)
		{
			userInput += 10;
	    	if (userInput > 999)
	    	{
	    		userInput = 999;
	    	}
			updateScreenTimerDone = false;
		}
		else
		{
			if (!TI1_updateScreenTimerIsOn)
			{
				updateScreenRate = BUTTON_HELD_REFRESH_TIME;
				TI1_updateScreenTimerIsOn = true;
			}
		}
	}
	else if (heldDown)
	{
		if (updateScreenTimerDone)
		{
	    	userInput -= 10;
	    	if (userInput < 0)
	    	{
	    		userInput = 0;
	    	}
			updateScreenTimerDone = false;
		}
		else
		{
			if (!TI1_updateScreenTimerIsOn)
			{
				updateScreenRate = BUTTON_HELD_REFRESH_TIME;
				TI1_updateScreenTimerIsOn = true;
			}
		}
	}
}

void heatingSetPointPosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = USER_INTERFACE_POSITION;
        currentPosition.lineNumber = 3;
    }
    else if (releasedOK)
    {
    	releasedOK = false;
		refreshScreen = true;
    	uint16 uart_write_return;
    	uart_write_return = display_uart_update(REGISTER, RW_REG_START + HEATING_SP, false, userInput, HEATING_SP_F, 0);
    	// Validation Screen
    	parameterIsSet = true;
    }
    else if (releasedUp)
    {
    	releasedUp = false;
		if (userInput < modbus_rw_reg_rcv[HEATING_SP_MAX].ivalue)
		{
			userInput += 10;
			if (userInput > modbus_rw_reg_rcv[HEATING_SP_MAX].ivalue)
			{
				userInput = modbus_rw_reg_rcv[HEATING_SP_MAX].ivalue;
			}
		}
    }
    else if (releasedDown)
    {
    	releasedDown = false;
		if (userInput > modbus_rw_reg_rcv[HEATING_SP_MIN].ivalue)
		{
			userInput -= 10;
    		if (userInput < modbus_rw_reg_rcv[HEATING_SP_MIN].ivalue)
    		{
				userInput = modbus_rw_reg_rcv[HEATING_SP_MIN].ivalue;
    		}
		}
    }
	else if (heldUp)
	{
		if (updateScreenTimerDone)
		{
			if (userInput < modbus_rw_reg_rcv[HEATING_SP_MAX].ivalue)
			{
				userInput += 10;
				if (userInput > modbus_rw_reg_rcv[HEATING_SP_MAX].ivalue)
				{
					userInput = modbus_rw_reg_rcv[HEATING_SP_MAX].ivalue;
				}
			}
			updateScreenTimerDone = false;
		}
		else
		{
			if (!TI1_updateScreenTimerIsOn)
			{
				updateScreenRate = BUTTON_HELD_REFRESH_TIME;
				TI1_updateScreenTimerIsOn = true;
			}
		}
	}
	else if (heldDown)
	{
		if (updateScreenTimerDone)
		{
			if (userInput > modbus_rw_reg_rcv[HEATING_SP_MIN].ivalue)
			{
				userInput -= 10;
	    		if (userInput < modbus_rw_reg_rcv[HEATING_SP_MIN].ivalue)
	    		{
					userInput = modbus_rw_reg_rcv[HEATING_SP_MIN].ivalue;
	    		}
			}
			updateScreenTimerDone = false;
		}
		else
		{
			if (!TI1_updateScreenTimerIsOn)
			{
				updateScreenRate = BUTTON_HELD_REFRESH_TIME;
				TI1_updateScreenTimerIsOn = true;
			}
		}
	}
}

void heatingDifferentialPosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = USER_INTERFACE_POSITION;
        currentPosition.lineNumber = 4;
    }
    else if (releasedOK)
    {
    	releasedOK = false;
		refreshScreen = true;
    	uint16 uart_write_return;
    	uart_write_return = display_uart_update(REGISTER, RW_REG_START + HEATING_DIF, false, userInput, HEATING_DIF_F, 0);
    	// Validation Screen
    	parameterIsSet = true;
    }
    else if (releasedUp)
    {
    	releasedUp = false;
    	userInput += 10;
    	if (userInput > 999)
    	{
    		userInput = 999;
    	}
    }
    else if (releasedDown)
    {
    	releasedDown = false;
    	userInput -= 10;
    	if (userInput < 0)
    	{
    		userInput = 0;
    	}
    }
	else if (heldUp)
	{
		if (updateScreenTimerDone)
		{
	    	userInput += 10;
	    	if (userInput > 999)
	    	{
	    		userInput = 999;
	    	}
			updateScreenTimerDone = false;
		}
		else
		{
			if (!TI1_updateScreenTimerIsOn)
			{
				updateScreenRate = BUTTON_HELD_REFRESH_TIME;
				TI1_updateScreenTimerIsOn = true;
			}
		}
	}
	else if (heldDown)
	{
		if (updateScreenTimerDone)
		{
	    	userInput -= 10;
	    	if (userInput < 0)
	    	{
	    		userInput = 0;
	    	}
			updateScreenTimerDone = false;
		}
		else
		{
			if (!TI1_updateScreenTimerIsOn)
			{
				updateScreenRate = BUTTON_HELD_REFRESH_TIME;
				TI1_updateScreenTimerIsOn = true;
			}
		}
	}
}

void highTempAlarmPosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = USER_INTERFACE_POSITION;
        currentPosition.lineNumber = 5;
    }
    else if (releasedOK)
    {
    	releasedOK = false;
		refreshScreen = true;
    	uint16 uart_write_return;
    	uart_write_return = display_uart_update(REGISTER, RW_REG_START + HIGH_TEMP_ALARM_SP, false, userInput, HIGH_TEMP_ALARM_SP_F, 0);
    	// Validation Screen
    	parameterIsSet = true;
    }
    else if (releasedUp)
    {
    	releasedUp = false;
    	userInput += 10;
    	if (userInput > 999)
    	{
    		userInput = 999;
    	}
    }
    else if (releasedDown)
    {
    	releasedDown = false;
		userInput -= 10;
    	if (userInput < -999)
    	{
    		userInput = -999;
    	}
    }
	else if (heldUp)
	{
		if (updateScreenTimerDone)
		{
	    	userInput += 10;
	    	if (userInput > 999)
	    	{
	    		userInput = 999;
	    	}
			updateScreenTimerDone = false;
		}
		else
		{
			if (!TI1_updateScreenTimerIsOn)
			{
				updateScreenRate = BUTTON_HELD_REFRESH_TIME;
				TI1_updateScreenTimerIsOn = true;
			}
		}
	}
	else if (heldDown)
	{
		if (updateScreenTimerDone)
		{
	    	userInput -= 10;
	    	if (userInput < -999)
	    	{
	    		userInput = -999;
	    	}
			updateScreenTimerDone = false;
		}
		else
		{
			if (!TI1_updateScreenTimerIsOn)
			{
				updateScreenRate = BUTTON_HELD_REFRESH_TIME;
				TI1_updateScreenTimerIsOn = true;
			}
		}
	}
}

void lowTempAlarmPosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = USER_INTERFACE_POSITION;
        currentPosition.lineNumber = 6;
    }
    else if (releasedOK)
    {
    	releasedOK = false;
		refreshScreen = true;
    	uint16 uart_write_return;
    	// write to main board to update the coolingSP
    	uart_write_return = display_uart_update(REGISTER, RW_REG_START + LOW_TEMP_ALARM_SP, false, userInput, LOW_TEMP_ALARM_SP_F, 0);
    	// Validation Screen
    	parameterIsSet = true;
    }
    else if (releasedUp)
    {
    	releasedUp = false;
		userInput += 10;
    	if (userInput > 999)
    	{
    		userInput = 999;
    	}
    }
    else if (releasedDown)
    {
    	releasedDown = false;
    	userInput -= 10;
    	if (userInput < -999)
    	{
    		userInput = -999;
    	}
    }
	else if (heldUp)
	{
		if (updateScreenTimerDone)
		{
	    	userInput += 10;
	    	if (userInput > 999)
	    	{
	    		userInput = 999;
	    	}
			updateScreenTimerDone = false;
		}
		else
		{
			if (!TI1_updateScreenTimerIsOn)
			{
				updateScreenRate = BUTTON_HELD_REFRESH_TIME;
				TI1_updateScreenTimerIsOn = true;
			}
		}
	}
	else if (heldDown)
	{
		if (updateScreenTimerDone)
		{
	    	userInput -= 10;
	    	if (userInput < -999)
	    	{
	    		userInput = -999;
	    	}
			updateScreenTimerDone = false;
		}
		else
		{
			if (!TI1_updateScreenTimerIsOn)
			{
				updateScreenRate = BUTTON_HELD_REFRESH_TIME;
				TI1_updateScreenTimerIsOn = true;
			}
		}
	}
}

void tempScalePosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = USER_INTERFACE_POSITION;
        currentPosition.lineNumber = 7;
    }
    else if (releasedOK)
    {
    	releasedOK = false;
		refreshScreen = true;
    	uint16 uart_write_return;
    	switch (currentPosition.lineNumber)
    	{
			case 1: // degree F
				uart_write_return = display_uart_update(COIL, RW_COIL_START + UNIT_OF_MEASURE, true, 0, 0, UNIT_OF_MEASURE_F);
				break;

			case 2: // degree C
				uart_write_return = display_uart_update(COIL, RW_COIL_START + UNIT_OF_MEASURE, false, 0, 0, UNIT_OF_MEASURE_F);
				break;

			default: break;
    	}
    	// Validation Screen
    	parameterIsSet = true;
    }
    else if (releasedUp)
    {
    	releasedUp = false;
		currentPosition.lineNumber = 1;
    }
    else if (releasedDown)
    {
    	releasedDown = false;
		currentPosition.lineNumber = 2;
    }
}

void hysteresisPosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = USER_INTERFACE_POSITION;
        currentPosition.lineNumber = 8;
    }
    else if (releasedOK)
    {
    	releasedOK = false;
		refreshScreen = true;
    	uint16 uart_write_return;
    	switch (currentPosition.lineNumber)
    	{
			case 1: // positive
				uart_write_return = display_uart_update(COIL, RW_COIL_START + COOL_HYSTERESIS, false, 0, 0, COOL_HYSTERESIS_F);
				break;

			case 2: // negative
				uart_write_return = display_uart_update(COIL, RW_COIL_START + COOL_HYSTERESIS, true, 0, 0, COOL_HYSTERESIS_F);
				break;

			default: break;
    	}
    	// Validation Screen
    	parameterIsSet = true;
    }
    else if (releasedUp)
    {
    	releasedUp = false;
		currentPosition.lineNumber = 1;
    }
    else if (releasedDown)
    {
    	releasedDown = false;
		currentPosition.lineNumber = 2;
    }
}

void languagePosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = USER_INTERFACE_POSITION;
        currentPosition.lineNumber   = 9;
    }
    else if (releasedOK)
    {
    	releasedOK = false;
    }
    else if (releasedUp)
    {
    	releasedUp = false;
    	if (currentPosition.lineNumber > 1)
    	{
            currentPosition.lineNumber -= 1;
    	}
    	else
    	{
            currentPosition.lineNumber = 1;
        }
    }
    else if (releasedDown)
    {
    	releasedDown = false;
    	if (currentPosition.lineNumber < 7)
    	{
            currentPosition.lineNumber += 1;
    	}
    	else
    	{
            currentPosition.lineNumber = 7;
        }
    }
}

void passwordPosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = USER_INTERFACE_POSITION;
        currentPosition.lineNumber = 10;
	}
    else if (releasedOK)
    {
    	releasedOK = false;
		refreshScreen = true;
    	uint16 uart_write_return;
		switch (currentPosition.lineNumber)
		{
		case 1:
			uart_write_return = display_uart_update(COIL, RW_COIL_START + PASSWORD_ENABLED, false, 0, 0, PASSWORD_ENABLED_F);
			break;

		case 2:
			uart_write_return = display_uart_update(COIL, RW_COIL_START + PASSWORD_ENABLED, true, 0, 0, PASSWORD_ENABLED_F);
			break;

		default: break;
		}
    	// Validation Screen
    	parameterIsSet = true;
	}
    else if (releasedUp)
    {
    	releasedUp = false;
		currentPosition.lineNumber = 1;
    }
    else if (releasedDown)
    {
    	releasedDown = false;
		currentPosition.lineNumber = 2;
    }
}

void doorSmokeAlarmPosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = SYSTEM_INTERFACE_POSITION;
        currentPosition.lineNumber   = 1;
    }
    else if (releasedOK)
    {
    	releasedOK = false;
		refreshScreen = true;
    	uint16 uart_write_return;
    	switch (currentPosition.lineNumber)
    	{
			case 1: // Normally open
				uart_write_return = display_uart_update(COIL, RW_COIL_START + DOOR_SWITCH, false, 0, 0, DOOR_SWITCH_F);
				break;

			case 2: // Normally close
				uart_write_return = display_uart_update(COIL, RW_COIL_START + DOOR_SWITCH, true, 0, 0, DOOR_SWITCH_F);
				break;

			default: break;
    	}
    	// Validation Screen
    	parameterIsSet = true;
    }
    else if (releasedUp)
    {
    	releasedUp = false;
		currentPosition.lineNumber = 1;
    }
    else if (releasedDown)
    {
    	releasedDown = false;
		currentPosition.lineNumber = 2;
    }
}

void disableSwitchAlarmPosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = SYSTEM_INTERFACE_POSITION;
        currentPosition.lineNumber   = 2;
    }
    else if (releasedOK)
    {
    	releasedOK = false;
		refreshScreen = true;
    	uint16 uart_write_return;
    	switch (currentPosition.lineNumber)
    	{
			case 1: // Normally open
				uart_write_return = display_uart_update(COIL, RW_COIL_START + DISABLE_SWITCH, false, 0, 0, DISABLE_SWITCH_F);
				break;

			case 2: // Normally close
				uart_write_return = display_uart_update(COIL, RW_COIL_START + DISABLE_SWITCH, true, 0, 0, DISABLE_SWITCH_F);
				break;

			default: break;
    	}
    	// Validation Screen
    	parameterIsSet = true;
    }
    else if (releasedUp)
    {
    	releasedUp = false;
		currentPosition.lineNumber = 1;
    }
    else if (releasedDown)
    {
    	releasedDown = false;
		currentPosition.lineNumber = 2;
    }
}

void alarmRelayOutputPosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = SYSTEM_INTERFACE_POSITION;
        currentPosition.lineNumber   = 3;
    }
    else if (releasedOK)
    {
    	releasedOK = false;
		refreshScreen = true;
    	uint16 uart_write_return;
    	switch (currentPosition.lineNumber)
    	{
			case 1: // Normally open
				uart_write_return = display_uart_update(COIL, RW_COIL_START + ALARM_OUTPUT, false, 0, 0, ALARM_OUTPUT_F);
				break;

			case 2: // Normally close
				uart_write_return = display_uart_update(COIL, RW_COIL_START + ALARM_OUTPUT, true, 0, 0, ALARM_OUTPUT_F);
				break;

			default: break;
    	}
    	// Validation Screen
    	parameterIsSet = true;
    }
    else if (releasedUp)
    {
    	releasedUp = false;
		currentPosition.lineNumber = 1;
    }
    else if (releasedDown)
    {
    	releasedDown = false;
		currentPosition.lineNumber = 2;
    }
}

void compressorRestartDelayPosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = SYSTEM_INTERFACE_POSITION;
        currentPosition.lineNumber = 4;
    }
    else if (releasedOK)
    {
    	releasedOK = false;
		refreshScreen = true;
    	uint16 uart_write_return;
    	// write to main board to update the coolingSP
    	uart_write_return = display_uart_update(REGISTER, RW_REG_START + COMP_MIN_STOP_TIME, false, userInput, COMP_MIN_STOP_TIME_F, 0);
    	// Validation Screen
    	parameterIsSet = true;
    }
    else if (releasedUp)
    {
    	releasedUp = false;
    	if (userInput < 5999) // 5999 second is 99 minutes and 59 second
    	{
			userInput += 1;
    	}
    }
    else if (releasedDown)
    {
    	releasedDown = false;
    	if (userInput > 0)
    	{
			userInput -= 1;
    	}
    }
	else if (heldUp)
	{
		if (updateScreenTimerDone)
		{
	    	if (userInput < 5999) // 5999 second is 99 minutes and 59 second
	    	{
				userInput += 1;
	    	}
			updateScreenTimerDone = false;
		}
		else
		{
			if (!TI1_updateScreenTimerIsOn)
			{
				updateScreenRate = BUTTON_HELD_REFRESH_TIME;
				TI1_updateScreenTimerIsOn = true;
			}
		}
	}
	else if (heldDown)
	{
		if (updateScreenTimerDone)
		{
	    	if (userInput > 0)
	    	{
				userInput -= 1;
	    	}
			updateScreenTimerDone = false;
		}
		else
		{
			if (!TI1_updateScreenTimerIsOn)
			{
				updateScreenRate = BUTTON_HELD_REFRESH_TIME;
				TI1_updateScreenTimerIsOn = true;
			}
		}
	}
}

void compressorCurrentPosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = SYSTEM_INTERFACE_POSITION;
        currentPosition.lineNumber   = 5;
    }
    else if (releasedOK)
    {
    	releasedOK = false;
    }
    else if (releasedUp)
    {
    	releasedUp = false;
    }
    else if (releasedDown)
    {
    	releasedDown = false;
    }
}

void systemSettingsPosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = SYSTEM_INTERFACE_POSITION;
        currentPosition.lineNumber   = 6;
	}
    else if (releasedOK)
    {
    	releasedOK = false;
		refreshScreen = true;
        switch (currentPosition.lineNumber)
        {
            case 1:
                currentPosition.displayLevel = CURRENT_LIMIT_SETTINGS_POSITION;
                break;

            case 2:
                currentPosition.displayLevel = MALF_ACTIVATE_POSITION;
                break;

            case 3:
                currentPosition.displayLevel = MALF_DEACTIVATE_POSITION;
                break;

            case 4:
                currentPosition.displayLevel = HPC_FAN_ON_POSITION;
                break;

            case 5:
                currentPosition.displayLevel = HPC_FAN_OFF_POSITION;
                break;

            default: break;
        }
	}
    else if (releasedUp)
    {
    	releasedUp = false;
    	if (currentPosition.lineNumber == 1)
    	{
            currentPosition.lineNumber = 5;
    	}
    	else
    	{
            currentPosition.lineNumber -= 1;
        }
    }
    else if (releasedDown)
    {
    	releasedDown = false;
    	if (currentPosition.lineNumber == 5)
    	{
            currentPosition.lineNumber = 1;
    	}
    	else
    	{
            currentPosition.lineNumber += 1;
        }
    }
}

void currentLimitSettingsPosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = DISPLAY_SYS_SETTINGS_MENU_POSITION;
        currentPosition.lineNumber   = 1;
	}
    else if (releasedOK)
    {
    	releasedOK = false;
        currentPosition.displayLevel = DISPLAY_SYS_SETTINGS_MENU_POSITION;
        currentPosition.lineNumber   = 1;
	}
    else if (releasedUp)
    {
    	releasedUp = false;
	}
    else if (releasedDown)
    {
    	releasedDown = false;
	}
}

void malfDeactivatePosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = DISPLAY_SYS_SETTINGS_MENU_POSITION;
        currentPosition.lineNumber   = 3;
	}
    else if (releasedOK)
    {
    	releasedOK = false;
        currentPosition.displayLevel = DISPLAY_SYS_SETTINGS_MENU_POSITION;
        currentPosition.lineNumber   = 3;
	}
    else if (releasedUp)
    {
    	releasedUp = false;
	}
    else if (releasedDown)
    {
    	releasedDown = false;
	}
}

void malfActivatePosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = DISPLAY_SYS_SETTINGS_MENU_POSITION;
        currentPosition.lineNumber   = 2;
	}
    else if (releasedOK)
    {
    	releasedOK = false;
        currentPosition.displayLevel = DISPLAY_SYS_SETTINGS_MENU_POSITION;
        currentPosition.lineNumber   = 2;
	}
    else if (releasedUp)
    {
    	releasedUp = false;
	}
    else if (releasedDown)
    {
    	releasedDown = false;
	}
}

void hpcFanOnPosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = DISPLAY_SYS_SETTINGS_MENU_POSITION;
        currentPosition.lineNumber   = 4;
	}
    else if (releasedOK)
    {
    	releasedOK = false;
        currentPosition.displayLevel = DISPLAY_SYS_SETTINGS_MENU_POSITION;
        currentPosition.lineNumber   = 4;
	}
    else if (releasedUp)
    {
    	releasedUp = false;
	}
    else if (releasedDown)
    {
    	releasedDown = false;
	}
}

void hpcFanOffPosition(void)
{
    if (releasedBack)
    {
    	releasedBack = false;
        currentPosition.displayLevel = DISPLAY_SYS_SETTINGS_MENU_POSITION;
        currentPosition.lineNumber   = 5;
	}
    else if (releasedOK)
    {
    	releasedOK = false;
        currentPosition.displayLevel = DISPLAY_SYS_SETTINGS_MENU_POSITION;
        currentPosition.lineNumber   = 5;
	}
    else if (releasedUp)
    {
    	releasedUp = false;
	}
    else if (releasedDown)
    {
    	releasedDown = false;
	}

}
