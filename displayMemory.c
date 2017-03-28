#include "displayPosition.h"
#include "icon.h"
#include "digit.h"
#include "OLED.h"
#include "PE_Types.h"
#include "button.h"
#include "text.h"
#include "Events.h"
#include "display_UART.h"
#include <stdbool.h>
#include "displayMemory.h"

uint16_union modbus_ro_reg_rcv[RO_REG_LEN];					// read only registers controller copy
byte modbus_rw_coil_rcv[RW_COIL_LEN/8+1];					// read/write coils controller copy

/* Function Prototypes */
static void mainScreenDisplayMemory(void);
static void mainMenuDisplayMemory(void);
static void userInterfaceDisplayMemory(void);
static void systemInterfaceDisplayMemory(void);
static void enterPasswordDisplayMemory(void);
static void showVersionNumber(int versionNumber, char page, char col);
static void aboutMenuDisplayMemory(void);

static void alarmDisplayMemory(int alarmPosition);

static void coolingSetPointDisplayMemory(void);
static void coolingDifferentialDisplayMemory(void);
static void heatingSetPointDisplayMemory(void);
static void heatingDifferentialDisplayMemory(void);
static void highTempAlarmDisplayMemory(void);
static void lowTempAlarmDisplayMemory(void);
static void tempScaleDisplayMemory(void);
static void hysteresisDisplayMemory(void);
static void passwordDisplayMemory(void);
static void languageDisplayMemory(void);

static void doorSmokeAlarmDisplayMemory(void);
static void disableSwitchAlarmDisplayMemory(void);
static void alarmRelayOutputDisplayMemory(void);
static void compressorRestartDelayDisplayMemory(void);
static void compressorCurrentDisplayMemory(void);
static void systemSettingsDisplayMemory(void);
static void currentLimitSettingsDisplayMemory(void);
static void malfDeactivateDisplayMemory(void);
static void malfActivateDisplayMemory(void);
static void hpcFanOnDisplayMemory(void);
static void hpcFanOffDisplayMemory(void);

static void showWorkingStatus1(void);
static void showWorkingStatus2(void);
static void showDegree(unsigned char page, unsigned char column);
static void showTempReading(int tempReading, unsigned char page, unsigned char column);
static void showTimer(const char font[], unsigned char minute, unsigned char second, unsigned char heightInPages, unsigned char widthInRows, unsigned char page, unsigned char column);

/*
** ===================================================================
**     Method      :  		checkDisplayMemory
**     Description :
**
** ===================================================================
*/
void checkDisplayMemory(void)
{
	if (updateDisplayMemory)
	{
		updateDisplayMemory = false;
		if (refreshScreen)
		{
			refreshScreen = false;
			clearDisplay();
		}
		updateScreen = true;

		switch (currentPosition.displayLevel)
		{
		/******************** Startup Screens **************************/
			/* Display Pentair Logo for 1 second at Start-up */
			case PENTAIR_ICON_POSITION:
				showIcon(Pentair_Logo, 128, 4, 2, 0);
				TI1_pentairIconTimer_Flag = true;
				break;

			/* Display All Icons for 1 second at Start-up */
			case ICON_ALL_ON_POSITION:
				displayAllOn();
				TI1_iconAllOnTimer_Flag = true;
				break;
		/******************** Startup Screens **************************/

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

			// Main Screen
			case MAIN_SCREEN_POSITION:
				mainScreenDisplayMemory();
				updateDisplayMemory = true; // update main screen all the time (add a timer later)
				break;

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

			/******************** Main Menu **************************/
			case MAIN_MENU_POSITION:
				mainMenuDisplayMemory();
				break;

			case USER_INTERFACE_POSITION:
				userInterfaceDisplayMemory();
				break;

			case SYSTEM_INTERFACE_POSITION:
				systemInterfaceDisplayMemory();
				break;

			case USER_PASSWORD_POSITION: case SYSTEM_PASSWORD_POSITION:
				enterPasswordDisplayMemory();
				break;

			case ABOUT_POSITION:
				aboutMenuDisplayMemory();
				break;
			/******************** Main Menu **************************/

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

			/******************** User Interface **************************/
			case COOLING_SET_POINT_POSITION:
				coolingSetPointDisplayMemory();
				break;

			case COOLING_DIF_POSITION:
				coolingDifferentialDisplayMemory();
				break;

			case HEATING_SET_POINT_POSITION:
				heatingSetPointDisplayMemory();
				break;

			case HEATING_DIF_POSITION:
				heatingDifferentialDisplayMemory();
				break;

			case HIGH_TEMP_ALARM_SP_POSITION:
				highTempAlarmDisplayMemory();
				break;

			case LOW_TEMP_ALARM_SP_POSITION:
				lowTempAlarmDisplayMemory();
				break;

			case TEMP_SCALE_POSITION:
				tempScaleDisplayMemory();
				break;

			case HYSTERESIS_POSITION:
				hysteresisDisplayMemory();
				break;

			case PASSWORD_POSITION:
				passwordDisplayMemory();
				break;

			case LANGUAGE_MENU_POSITION:
				languageDisplayMemory();
				break;
			/******************** User Interface **************************/

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

			/******************** System Interface ************************/
			case DOOR_SMOKE_ALARM_POSITION:
				doorSmokeAlarmDisplayMemory();
				break;

			case DISABLE_SWITCH_POSITION:
				disableSwitchAlarmDisplayMemory();
				break;

			case ALARM_RELAY_OUTPUT_POSITION:
				alarmRelayOutputDisplayMemory();
				break;

			case COMPRESSOR_RESTART_DELAY_POSITION:
				compressorRestartDelayDisplayMemory();
				break;

			case COMPRESSOR_CURRENT_POSITION:
				compressorCurrentDisplayMemory();
				break;

			// System Settings
			case DISPLAY_SYS_SETTINGS_MENU_POSITION:
				systemSettingsDisplayMemory();
				break;

			case CURRENT_LIMIT_SETTINGS_POSITION:
				currentLimitSettingsDisplayMemory();
				break;

			case MALF_DEACTIVATE_POSITION:
				malfDeactivateDisplayMemory();
				break;

			case MALF_ACTIVATE_POSITION:
				malfActivateDisplayMemory();
				break;

			case HPC_FAN_ON_POSITION:
				hpcFanOnDisplayMemory();
				break;

			case HPC_FAN_OFF_POSITION:
				hpcFanOffDisplayMemory();
				break;
			/******************** System Interface **************************/

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

			// Push four buttons at same time
			case CHECK_ALL_PIXELS:
				fillDisplay();
				TI1_checkPixelsTimer_Flag = true;
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
				alarmDisplayMemory(currentPosition.displayLevel);
				break;
			/******************** Alarms ***********************/

			default: break;
		}
	}
}

void mainScreenDisplayMemory(void)
{
	/* Probe#1 - #3 */
	switch (currentPosition.lineNumber)
	{
		case INLET_LINENUM:
			/* Power */
			showIcon(snowFlakeIcon, 14, 2, 4, 0);

			/* Inlet Temperature Sensor Reading */
			showTempReading(modbus_ro_reg_rcv[PROBE_1].ivalue, 1, 110);

			/* Working Status 1 */
			showWorkingStatus1();

			/* Working Status 2 */
			showWorkingStatus2();

			TI1_alarmTimer_Flag = true;
			break;

		case OUTLET_LINENUM:
			// Turn off alarm timer and clear its counter
			TI1_alarmTimer_Flag = false;
			TI1_alarmTimer = 0;
			displayString("Outlet Temp", arial_bold14, 0, 30);
			showTempReading(modbus_ro_reg_rcv[PROBE_2].ivalue, 3, 105);
			break;

		case COIL_LINENUM:
			// Turn off alarm timer and clear its counter
			TI1_alarmTimer_Flag = false;
			TI1_alarmTimer = 0;
			displayString("Cond Coil Temp", arial_bold14, 0, 20);
			showTempReading(modbus_ro_reg_rcv[PROBE_3].ivalue, 3, 105);
			break;

		default: break;
	}
}

void mainMenuDisplayMemory(void)
{
	char *mainMenuItems[] = {
			"       Main Menu        ",
			"   User Interface         ",
			"   System Interface       ",
			"   About                  "
	};

	displayHighlightedString(mainMenuItems[0], arial_bold14, 0, 0);
	displayString(mainMenuItems[1], arial14, 2, 0);
	displayString(mainMenuItems[2], arial14, 4, 0);
	displayString(mainMenuItems[3], arial14, 6, 0);

	if (currentPosition.lineNumber%3 == 0)
	{
		displayString(" *", arial14, 6, 0);
	}
	else
	{
		displayString(" *", arial14, (currentPosition.lineNumber%3)*2, 0);
	}
}

void enterPasswordDisplayMemory(void)
{
	displayHighlightedString("    Enter Password    ", arial_bold14, 0, 0);
	if (userInput >= 0 && userInput <= 9)
	{
		showIntegerLeftAligned(digitCalibriLight4x28, userInput, 4, 28, 3, 65);
		showIntegerRightAligned(digitCalibriLight4x28, 0, 4, 28, 3, 65);
	}
	else
	{
		showIntegerLeftAligned(digitCalibriLight4x28, userInput, 4, 28, 3, 35);
	}
}

void userInterfaceDisplayMemory(void)
{
	char *userInterfaceItems[] = {
			"     User Interface       ",
			"   Cooling Set Point      ",
			"   Cooling Differential   ",
			"   Heating Set Point      ",
			"   Heating Differential   ",
			"   High Temp Alarm        ",
			"   Low Temp Alarm         ",
			"   Temperature Scale      ",
			"   Hysteresis             ",
			"   Language               ",
			"   Passcode enable/disable"
	};

	displayHighlightedString(userInterfaceItems[0], arial_bold14, 0, 0);
    switch (currentPosition.lineNumber)
    {
		case 1: case 2: case 3:
			displayString(userInterfaceItems[1], arial14, 2, 0);
			displayString(userInterfaceItems[2], arial14, 4, 0);
			displayString(userInterfaceItems[3], arial14, 6, 0);
			break;

		case 4: case 5: case 6:
			displayString(userInterfaceItems[4], arial14, 2, 0);
			displayString(userInterfaceItems[5], arial14, 4, 0);
			displayString(userInterfaceItems[6], arial14, 6, 0);
			break;

		case 7: case 8: case 9:
			displayString(userInterfaceItems[7], arial14, 2, 0);
			displayString(userInterfaceItems[8], arial14, 4, 0);
			displayString(userInterfaceItems[9], arial14, 6, 0);
			break;

		case 10:
			displayString(userInterfaceItems[10], arial14, 2, 0);
			displayString("                         ", arial14, 4, 0);
			displayString("                         ", arial14, 6, 0);
			break;

		default: break;
    }
	if (currentPosition.lineNumber%3 == 0)
	{
		displayString(" *", arial14, 6, 0);
	}
	else
	{
		displayString(" *", arial14, (currentPosition.lineNumber%3)*2, 0);
	}
}

void systemInterfaceDisplayMemory(void)
{
	char *systemInterfaceItems[] = {
			"   System Interface     ",
			"   Door/Smoke Alarm     ",
			"   Disable Switch       ",
			"   Alarm relay output   ",
			"   Comp Restart Delay   ",
			"   Compressor Current   ",
			"   System Settings      "
	};

	displayHighlightedString(systemInterfaceItems[0], arial_bold14, 0, 0);
    switch (currentPosition.lineNumber)
    {
        case 1: case 2: case 3:
			displayString(systemInterfaceItems[1], arial14, 2, 0);
			displayString(systemInterfaceItems[2], arial14, 4, 0);
			displayString(systemInterfaceItems[3], arial14, 6, 0);
            break;

        case 4: case 5: case 6:
			displayString(systemInterfaceItems[4], arial14, 2, 0);
			displayString(systemInterfaceItems[5], arial14, 4, 0);
			displayString(systemInterfaceItems[6], arial14, 6, 0);
            break;

        default: break;
    }
	if (currentPosition.lineNumber%3 == 0)
	{
		displayString(" *", arial14, 6, 0);
	}
	else
	{
		displayString(" *", arial14, (currentPosition.lineNumber%3)*2, 0);
	}
}

void showVersionNumber(int versionNumber, char page, char col)
{
	char minor = versionNumber & 0xff;
	char major = (versionNumber & 0xff00) >> 8;
	displayChar(major, arial14, page, col);
	displayChar('.', arial14, page, col+8);
	displayChar(minor, arial14, page, col+16);
}

void aboutMenuDisplayMemory(void)
{
	displayString(" ECU Hardware:", arial14, 0, 0);
	showVersionNumber(modbus_ro_reg_rcv[HARDWARE_VERSION].ivalue, 0, 90);
    displayString(" ECU Firmware:", arial14, 2, 0);
	showVersionNumber(modbus_ro_reg_rcv[FIRMWARE_VERSION].ivalue, 2, 90);
	displayString(" DU Hardware:", arial14, 4, 0);
	showVersionNumber(modbus_wo_reg_snd[DISPLAY_HARDWARE_REV].ivalue, 4, 90);
	displayString(" DU Firmware:", arial14, 6, 0);
	showVersionNumber(modbus_wo_reg_snd[DISPLAY_FIRMWARE_REV].ivalue, 6, 90);
}

void alarmDisplayMemory(int alarmPosition)
{
	switch (alarmPosition)
	{
		case HIGH_TEMP_ALARM_POSITION:
			showIcon(attentionIcon, 16, 2, 3, 5);
			displayString("High Temp Alarm", arial_bold14, 3, 23);
			break;

		case LOW_TEMP_ALARM_POSITION:
			showIcon(attentionIcon, 16, 2, 3, 5);
			displayString("Low Temp Alarm", arial_bold14, 3, 23);
			break;

		case DOOR_ALARM_POSITION:
			showIcon(attentionIcon, 16, 2, 3, 5);
			displayString("Door/Smoke Alarm", arial_bold14, 3, 25);
			break;

		case PHASE_MISSING_ALARM_POSITION:
			showIcon(attentionIcon, 16, 2, 3, 15);
			displayString("Phase Missing", arial_bold14, 3, 35);
			break;

		case EVAP_MI_THERMAL_OVERLOAD_ALARM_POSITION:
			showIcon(attentionIcon, 16, 2, 3, 3);
			displayString("Evaporator Fan", arial_bold14, 2, 23);
			displayString("Thermal Overload", arial_bold14, 4, 23);
			break;

		case CONDENSOR_MI_THERMAL_OVERLOAD_ALARM_POSITION:
			showIcon(attentionIcon, 16, 2, 3, 3);
			displayString("Condenser Fan", arial_bold14, 2, 23);
			displayString("Thermal Overload", arial_bold14, 4, 23);
			break;

		case COMPRESSOR_THERMAL_OVERLOAD_ALARM_POSITION:
			showIcon(attentionIcon, 16, 2, 3, 3);
			displayString("Compressor", arial_bold14, 2, 23);
			displayString("Thermal Overload", arial_bold14, 4, 23);
			break;

		case OVER_VOLTAGE_ALARM_POSITION:
			showIcon(attentionIcon, 16, 2, 3, 15);
			displayString("Over Voltage", arial_bold14, 3, 35);
			break;

		case UNDER_VOLTAGE_ALARM_POSITION:
			showIcon(attentionIcon, 16, 2, 3, 15);
			displayString("Under Voltage", arial_bold14, 3, 35);
			break;

		case BAD_BOARD_ALARM_POSITION:
			showIcon(attentionIcon, 16, 2, 3, 5);
			displayString("Controller Board", arial_bold14, 2, 25);
			displayString("Malfunction", arial_bold14, 4, 26);
			break;

		case HIGH_PRESSURE_ALARM_POSITION:
			showIcon(attentionIcon, 16, 2, 3, 5);
			displayString("Unit Malfunction:", arial_bold14, 2, 25);
			displayString("High Pressure", arial_bold14, 4, 25);
			break;

		case LOW_PRESSURE_ALARM_POSITION:
			showIcon(attentionIcon, 16, 2, 3, 15);
			displayString("Low Pressure", arial_bold14, 3, 35);
			break;

		case FROST_ALARM_POSITION:
			showIcon(attentionIcon, 16, 2, 3, 5);
			displayString("Frost Detected", arial_bold14, 0, 25);
			displayString("Compressor and", arial_bold14, 2, 25);
			displayString("Condenser Fan", arial_bold14, 4, 25);
			displayString("Disabled", arial_bold14, 6, 25);
			break;

		case DISABLE_SWITCH_ALARM_POSITION:
			showIcon(attentionIcon, 16, 2, 3, 10);
			displayString("Disable Switch", arial_bold14, 2, 30);
			displayString("Activated", arial_bold14, 4, 30);
			break;

		case COIL_TEMP_SENSOR_FAULT_ALARM_POSITION:
			showIcon(attentionIcon, 16, 2, 3, 5);
			displayString("Condenser Coil", arial_bold14, 1, 25);
			displayString("Temp Sensor", arial_bold14, 3, 25);
			displayString("Malfunction", arial_bold14, 5, 26);
			break;

		case INLET_TEMP_SENSOR_FAULT_ALARM_POSITION:
			showIcon(attentionIcon, 16, 2, 3, 5);
			displayString("Enclosure Inlet", arial_bold14, 1, 25);
			displayString("Air Temp Sensor", arial_bold14, 3, 25);
			displayString("Malfunction", arial_bold14, 5, 26);
			break;

		case OUTLET_TEMP_SENSOR_FAULT_ALARM_POSITION:
			showIcon(attentionIcon, 16, 2, 3, 5);
			displayString("Enclosure Outlet", arial_bold14, 1, 25);
			displayString("Air Temp Sensor", arial_bold14, 3, 25);
			displayString("Malfunction", arial_bold14, 5, 26);
			break;

		case OVER_CURRENT_ALARM_POSITION:
			showIcon(attentionIcon, 16, 2, 3, 3);
			displayString("Compressor", arial_bold14, 2, 23);
			displayString("Overcurrent Alarm", arial_bold14, 4, 23);
			break;

		case COMPRESSOR_RESTART_DELAY_COUNT_DOWN_POSITION:
			displayString("Restart Delay Active", arial_bold14, 3, 5);
			break;

		case COMPRESSOR_RESTART_DELAY_TIMER_POSITION:
			showTimer(digitCalibriLight4x28, modbus_ro_reg_rcv[COMP_DELAY_TIMER].ivalue/60, modbus_ro_reg_rcv[COMP_DELAY_TIMER].ivalue%60, 4, 28, 2, 5);
			break;

		default: break;
	}
	// Turn on alarm timer
	TI1_alarmTimer_Flag = true;
}

void coolingSetPointDisplayMemory(void)
{
	if (parameterIsSet)
	{
    	displayString("Cooling set point", arial_bold14, 2, 10);
    	displayString("is set to", arial_bold14, 4, 10);
		showIntegerLeftAligned(digitCourier2x7, userInput/10, 2, 7, 4, 75);
		showDegree(4, 100);
    	TI1_validationScreenTimer_Flag = true;
	}
	else
	{
		displayHighlightedString("   Cooling Set Point   ", arial_bold14, 0, 0);
		showTempReading(userInput, 3, 110);
	}
}

void coolingDifferentialDisplayMemory(void)
{
	if (parameterIsSet)
	{
    	displayString("Cooling differential", arial_bold14, 2, 10);
    	displayString("is set to", arial_bold14, 4, 10);
		showIntegerLeftAligned(digitCourier2x7, userInput/10, 2, 7, 4, 75);
		showDegree(4, 100);
    	TI1_validationScreenTimer_Flag = true;
	}
	else
	{
		displayHighlightedString("  Cooling Differential ", arial_bold14, 0, 0);
		showTempReading(userInput, 3, 110);
	}
}

void heatingSetPointDisplayMemory(void)
{
	if (parameterIsSet)
	{
    	displayString("Heating set point", arial_bold14, 2, 10);
    	displayString("is set to", arial_bold14, 4, 10);
		showIntegerLeftAligned(digitCourier2x7, userInput/10, 2, 7, 4, 75);
		showDegree(4, 100);
    	TI1_validationScreenTimer_Flag = true;
	}
	else
	{
		displayHighlightedString("   Heating Set Point   ", arial_bold14, 0, 0);
		showTempReading(userInput, 3, 110);
	}
}

void heatingDifferentialDisplayMemory(void)
{
	if (parameterIsSet)
	{
    	displayString("Heating differential", arial_bold14, 2, 10);
    	displayString("is set to", arial_bold14, 4, 10);
		showIntegerLeftAligned(digitCourier2x7, userInput/10, 2, 7, 4, 75);
		showDegree(4, 100);
    	TI1_validationScreenTimer_Flag = true;
	}
	else
	{
		displayHighlightedString("  Heating Differential ", arial_bold14, 0, 0);
		showTempReading(userInput, 3, 110);
	}
}

void highTempAlarmDisplayMemory(void)
{
	if (parameterIsSet)
	{
    	displayString("High temp alarm", arial_bold14, 2, 10);
    	displayString("is set to", arial_bold14, 4, 10);
		showIntegerLeftAligned(digitCourier2x7, userInput/10, 2, 7, 4, 75);
		showDegree(4, 100);
    	TI1_validationScreenTimer_Flag = true;
	}
	else
	{
		displayHighlightedString("   High Temp Alarm   ", arial_bold14, 0, 0);
		showTempReading(userInput, 3, 110);
	}
}

void lowTempAlarmDisplayMemory(void)
{
	if (parameterIsSet)
	{
    	displayString("Low temp alarm", arial_bold14, 2, 10);
    	displayString("is set to", arial_bold14, 4, 10);
		showIntegerLeftAligned(digitCourier2x7, userInput/10, 2, 7, 4, 75);
		showDegree(4, 100);
    	TI1_validationScreenTimer_Flag = true;
	}
	else
	{
		displayHighlightedString("   Low Temp Alarm    ", arial_bold14, 0, 0);
		showTempReading(userInput, 3, 110);
	}
}

void tempScaleDisplayMemory(void)
{
	if (parameterIsSet)
	{
		switch (currentPosition.lineNumber)
		{
			case 1:
	    		displayString("Set to degree F ", arial_bold14, 3, 15);
				break;
			case 2:
	    		displayString("Set to degree C ", arial_bold14, 3, 15);
				break;
			default: break;
		}
    	TI1_validationScreenTimer_Flag = true;
	}
	else
	{
		displayHighlightedString("  Temperature Scale  ", arial_bold14, 0, 0);
		switch (currentPosition.lineNumber)
		{
			case 1:
				displayString(" * Fahrenheit", arial14, 2, 0);
				displayString("   Celsius", arial14, 4, 0);
				break;
			case 2:
				displayString("   Fahrenheit", arial14, 2, 0);
				displayString(" * Celsius", arial14, 4, 0);
				break;
			default: break;
		}
	}
}

void hysteresisDisplayMemory(void)
{
	if (parameterIsSet)
	{
    	switch (currentPosition.lineNumber)
    	{
			case 1: // Positive
	    		displayString("Hysteresis: Positive", arial_bold14, 3, 5);
				break;

			case 2: // Negative
	    		displayString("Hysteresis: Negative", arial_bold14, 3, 5);
				break;

			default: break;
    	}
    	TI1_validationScreenTimer_Flag = true;
	}
	else
	{
		displayHighlightedString("      Hysteresis       ", arial_bold14, 0, 0);
		switch (currentPosition.lineNumber)
		{
			case 1:
				displayString(" * Positive", arial14, 2, 0);
				displayString("   Negative", arial14, 4, 0);
				break;
			case 2:
				displayString("   Positive", arial14, 2, 0);
				displayString(" * Negative", arial14, 4, 0);
				break;
			default: break;
		}
	}
}

void passwordDisplayMemory(void)
{
	if (parameterIsSet)
	{
		if (currentPosition.lineNumber == 2)
		{
    		displayString("  Passcode for User", arial_bold14, 2, 0);
    		displayString("  Interface is ON", arial_bold14, 4, 0);
		}
		else if (currentPosition.lineNumber == 1)
		{
			displayString("  Passcode for User", arial_bold14, 2, 0);
			displayString("  Interface is OFF", arial_bold14, 4, 0);
		}
    	TI1_validationScreenTimer_Flag = true;
	}
	else
	{
		displayHighlightedString("        Passcode        ", arial_bold14, 0, 0);
		switch (currentPosition.lineNumber)
		{
			case 1:
				displayString(" * No Passcode", arial14, 2, 0);
				displayString("   Enable Passcode", arial14, 4, 0);
				break;
			case 2:
				displayString("   No Passcode", arial14, 2, 0);
				displayString(" * Enable Passcode", arial14, 4, 0);
				break;
			default: break;
		}
	}
}

void languageDisplayMemory(void)
{
	char *languageMenuItems[] = {
			"       Language        ",
        	"   English             ",
            "   Chinese             ",
            "   French              ",
			"   German              ",
            "   Italian             ",
            "   Polish              ",
			"   Spanish             "
	};

	displayHighlightedString(languageMenuItems[0], arial_bold14, 0, 0);
    switch (currentPosition.lineNumber)
    {
        case 1: case 2: case 3:
    		displayString(languageMenuItems[1], arial14, 2, 0);
    		displayString(languageMenuItems[2], arial14, 4, 0);
    		displayString(languageMenuItems[3], arial14, 6, 0);
            break;

        case 4: case 5: case 6:
    		displayString(languageMenuItems[4], arial14, 2, 0);
    		displayString(languageMenuItems[5], arial14, 4, 0);
    		displayString(languageMenuItems[6], arial14, 6, 0);
            break;

        case 7:
    		displayString(languageMenuItems[7], arial14, 2, 0);
    		displayString("                       ", arial14, 4, 0);
    		displayString("                       ", arial14, 6, 0);
            break;

        default: break;
    }

	if (currentPosition.lineNumber%3 == 0)
	{
		displayString(" *", arial14, 6, 0);
	}
	else
	{
		displayString(" *", arial14, (currentPosition.lineNumber%3)*2, 0);
	}
}

void doorSmokeAlarmDisplayMemory(void)
{
	if (parameterIsSet)
	{
    	switch (currentPosition.lineNumber)
    	{
			case 1: // Normally Open
	    		displayString("  Door/Smoke Alarm", arial_bold14, 2, 0);
	    		displayString("  is Normally Open", arial_bold14, 4, 0);
				break;

			case 2: // Normally Close
	    		displayString("  Door/Smoke Alarm", arial_bold14, 2, 0);
	    		displayString("  is Normally Close", arial_bold14, 4, 0);
				break;

			default: break;
    	}
    	TI1_validationScreenTimer_Flag = true;
	}
	else
	{
		displayHighlightedString("   Door/Smoke Alarm   ", arial_bold14, 0, 0);
    	switch (currentPosition.lineNumber)
    	{
			case 1:
				displayString(" * Normally Open", arial14, 2, 0);
				displayString("   Normally Close", arial14, 4, 0);
				break;

			case 2:
				displayString("   Normally Open", arial14, 2, 0);
				displayString(" * Normally Close", arial14, 4, 0);
				break;

			default: break;
    	}
	}
}

void disableSwitchAlarmDisplayMemory(void)
{
	if (parameterIsSet)
	{
    	switch (currentPosition.lineNumber)
    	{
			case 1: // Normally Open
	    		displayString("  Disable Switch", arial_bold14, 2, 5);
	    		displayString("  is Normally Open", arial_bold14, 4, 5);
				break;

			case 2: // Normally Close
	    		displayString("  Disable Switch", arial_bold14, 2, 5);
	    		displayString("  is Normally Close", arial_bold14, 4, 5);
				break;

			default: break;
    	}
    	TI1_validationScreenTimer_Flag = true;
	}
	else
	{
		displayHighlightedString("    Disable Switch    ", arial_bold14, 0, 0);
    	switch (currentPosition.lineNumber)
    	{
			case 1:
				displayString(" * Normally Open", arial14, 2, 0);
				displayString("   Normally Close", arial14, 4, 0);
				break;
			case 2:
				displayString("   Normally Open", arial14, 2, 0);
				displayString(" * Normally Close", arial14, 4, 0);
				break;
			default: break;
    	}
	}
}

void alarmRelayOutputDisplayMemory(void)
{
	if (parameterIsSet)
	{
    	switch (currentPosition.lineNumber)
    	{
			case 1: // Normally Open
	    		displayString("Alarm Relay Output", arial_bold14, 2, 10);
	    		displayString("is Normally Open", arial_bold14, 4, 10);
				break;

			case 2: // Normally Close
	    		displayString("Alarm Relay Output", arial_bold14, 2, 10);
	    		displayString("is Normally Close", arial_bold14, 4, 10);
				break;

			default: break;
    	}
    	TI1_validationScreenTimer_Flag = true;
	}
	else
	{
		displayHighlightedString("  Alarm Relay Output  ", arial_bold14, 0, 0);
    	switch (currentPosition.lineNumber)
    	{
			case 1:
				displayString(" * Normally Open", arial14, 2, 0);
				displayString("   Normally Close", arial14, 4, 0);
				break;
			case 2:
				displayString("   Normally Open", arial14, 2, 0);
				displayString(" * Normally Close", arial14, 4, 0);
				break;
			default: break;
    	}
	}
}

void compressorRestartDelayDisplayMemory(void)
{
	if (parameterIsSet)
	{
    	displayString("Comp Restart Delay", arial_bold14, 2, 5);
    	showIntegerLeftAligned(digitCourier2x7, userInput/60, 2, 7, 4, 5);
    	displayString("min", arial_bold14, 4, 25);
    	showIntegerLeftAligned(digitCourier2x7, userInput%60, 2, 7, 4, 60);
    	displayString("sec", arial_bold14, 4, 90);
    	TI1_validationScreenTimer_Flag = true;
	}
	else
	{
		displayHighlightedString(" Comp Restart Delay ", arial_bold14, 0, 0);
		showTimer(digitCalibriLight4x28, userInput/60, userInput%60, 4, 28, 3, 5);
	}
}

void compressorCurrentDisplayMemory(void)
{
	displayHighlightedString(" Compressor Current ", arial_bold14, 0, 0);
	displayString("Phase 1:", arial_bold14, 2, 2);
	displayString("Phase 2:", arial_bold14, 4, 2);
	displayString("Phase 3:", arial_bold14, 6, 2);

	if (modbus_ro_reg_rcv[PHASE_1_CURRENT].ivalue/10 <= 99)
	{
		showIntegerLeftAligned(digitCourier2x7, modbus_ro_reg_rcv[PHASE_1_CURRENT].ivalue/10, 2, 7, 2, 70);
		displayString("mA", arial_bold14, 2, 100);
	}
	else
	{
		showIntegerLeftAligned(digitCourier2x7, modbus_ro_reg_rcv[PHASE_1_CURRENT].ivalue/10000, 2, 7, 2, 70);
		displayString(".", courier7x14, 2, 78); // decimal point
		showIntegerLeftAligned(digitCourier2x7, (modbus_ro_reg_rcv[PHASE_1_CURRENT].ivalue/10-(modbus_ro_reg_rcv[PHASE_1_CURRENT].ivalue/10/1000)*1000)/100, 2, 7, 2, 85);
		displayString("A", arial_bold14, 2, 100);
	}

	if (modbus_ro_reg_rcv[PHASE_2_CURRENT].ivalue/10 <= 99)
	{
		showIntegerLeftAligned(digitCourier2x7, modbus_ro_reg_rcv[PHASE_2_CURRENT].ivalue/10, 2, 7, 4, 70);
		displayString("mA", arial_bold14, 4, 100);
	}
	else
	{
		showIntegerLeftAligned(digitCourier2x7, modbus_ro_reg_rcv[PHASE_2_CURRENT].ivalue/10000, 2, 7, 4, 70);
		displayString(".", courier7x14, 4, 78); // decimal point
		showIntegerLeftAligned(digitCourier2x7, (modbus_ro_reg_rcv[PHASE_2_CURRENT].ivalue/10-(modbus_ro_reg_rcv[PHASE_2_CURRENT].ivalue/10/1000)*1000)/100, 2, 7, 4, 85);
		displayString("A", arial_bold14, 4, 100);
	}

	if (modbus_ro_reg_rcv[PHASE_3_CURRENT].ivalue/10 <= 99)
	{
		showIntegerLeftAligned(digitCourier2x7, modbus_ro_reg_rcv[PHASE_3_CURRENT].ivalue/10, 2, 7, 6, 70);
		displayString("mA", arial_bold14, 6, 100);
	}
	else
	{
		showIntegerLeftAligned(digitCourier2x7, modbus_ro_reg_rcv[PHASE_3_CURRENT].ivalue/10000, 2, 7, 6, 70);
		displayString(".", courier7x14, 6, 78); // decimal point
		showIntegerLeftAligned(digitCourier2x7, (modbus_ro_reg_rcv[PHASE_3_CURRENT].ivalue/10-(modbus_ro_reg_rcv[PHASE_3_CURRENT].ivalue/10/1000)*1000)/100, 2, 7, 6, 85);
		displayString("A", arial_bold14, 6, 100);
	}
}

void systemSettingsDisplayMemory(void)
{
	char *systemSettingItems[] = {
			" Display SYS Settings ",
	    	"   Current limit setting ",
	        "   MALF Activate         ",
	        "   MALF Deactivate       ",
	    	"   HPC Fan ON            ",
	    	"   HPC Fan OFF           "
	};

	displayHighlightedString(systemSettingItems[0], arial_bold14, 0, 0);
	switch (currentPosition.lineNumber)
	{
		case 1: case 2: case 3:
			displayString(systemSettingItems[1], arial14, 2, 0);
			displayString(systemSettingItems[2], arial14, 4, 0);
			displayString(systemSettingItems[3], arial14, 6, 0);
			break;

		case 4: case 5:
			displayString(systemSettingItems[4], arial14, 2, 0);
			displayString(systemSettingItems[5], arial14, 4, 0);
			displayString("                         ", arial14, 6, 0);
			break;

		default: break;
	}
	if (currentPosition.lineNumber%3 == 0)
	{
		displayString(" *", arial14, 6, 0);
	}
	else
	{
		displayString(" *", arial14, (currentPosition.lineNumber%3)*2, 0);
	}
}

void currentLimitSettingsDisplayMemory(void)
{
	displayHighlightedString(" Current limit setting ", arial_bold14, 0, 0);
	int currentLimit = modbus_ero_reg_rcv[4].ivalue/10;

	if (currentLimit <= 99)
	{
		showIntegerLeftAligned(digitCourier2x7, currentLimit, 2, 7, 3, 30);
		displayString("mA", arial_bold14, 3, 60);
	}
	else
	{
		showIntegerLeftAligned(digitCourier2x7, currentLimit/1000, 2, 7, 3, 25);
		displayString(".", courier7x14, 3, 40); // decimal point
		showIntegerLeftAligned(digitCourier2x7, (currentLimit-(currentLimit/1000)*1000)/100, 2, 7, 3, 60);
		displayString("A", arial_bold14, 3, 80);
	}
}

void malfDeactivateDisplayMemory(void)
{
	displayHighlightedString("   MALF Deactivate   ", arial_bold14, 0, 0);
	showTempReading(modbus_ero_reg_rcv[MALF_DEACTIVATE].ivalue, 3, 110);
}

void malfActivateDisplayMemory(void)
{
	displayHighlightedString("    MALF Activate     ", arial_bold14, 0, 0);
	showTempReading(modbus_ero_reg_rcv[MALF_ACTIVATE].ivalue, 3, 110);
}

void hpcFanOnDisplayMemory(void)
{
	displayHighlightedString("      HPC Fan ON      ", arial_bold14, 0, 0);
	showTempReading(modbus_ero_reg_rcv[HPC_DEACTIVATE].ivalue, 3, 110);
}

void hpcFanOffDisplayMemory(void)
{
	displayHighlightedString("      HPC Fan OFF     ", arial_bold14, 0, 0);
	showTempReading(modbus_ero_reg_rcv[HPC_ACTIVATE].ivalue, 3, 110);
}

void showWorkingStatus1(void)
{
	int workingStatus1 = modbus_ro_reg_rcv[WORKING_STATUS_1].ivalue;
//	workingStatus1 = 0x0;
	//	0 - Phase Missing
	if (workingStatus1 & 0x0001)
	{
		alarmOutput[PHASE_MISSING] = 1;
		showIcon(bellIcon, 14, 2, 6, 15);
	}
	else
	{
		alarmOutput[PHASE_MISSING] = 0;
	}

	// 	1 - Evap MI Thermal Overload
	if (workingStatus1 & 0x0002)
	{
		alarmOutput[EVAP_MI_THERMAL_OVERLOAD] = 1;
		showIcon(bellIcon, 14, 2, 6, 15);
	}
	else
	{
		alarmOutput[EVAP_MI_THERMAL_OVERLOAD] = 0;
	}

	//	2 - Imbalance Voltage
	//	3 - Over Voltage
	if (workingStatus1 & 0x0008)
	{
		alarmOutput[OVER_VOLTAGE] = 1;
		showIcon(bellIcon, 14, 2, 6, 15);
	}
	else
	{
		alarmOutput[OVER_VOLTAGE] = 0;
	}

	//	4 - Under Voltage
	if (workingStatus1 & 0x0010)
	{
		alarmOutput[UNDER_VOLTAGE] = 1;
		showIcon(bellIcon, 14, 2, 6, 15);
	}
	else
	{
		alarmOutput[UNDER_VOLTAGE] = 0;
	}

	//	5 - Door/Smoke Alarm
	if (workingStatus1 & 0x0020)
	{
		alarmOutput[DOOR_SMOKE] = 1;
		showIcon(bellIcon, 14, 2, 6, 15);
	}
	else
	{
		alarmOutput[DOOR_SMOKE] = 0;
	}

	//	6 - Coil Temp Sensor Fault
	if (workingStatus1 & 0x0040)
	{
		alarmOutput[COIL_TEMP_SENSOR_FAULT] = 1;
		showIcon(bellIcon, 14, 2, 6, 15);
	}
	else
	{
		alarmOutput[COIL_TEMP_SENSOR_FAULT] = 0;
	}

	//	7 - Low Pressure Alarm
	if (workingStatus1 & 0x0080)
	{
		alarmOutput[LOW_PRESSURE] = 1;
		showIcon(bellIcon, 14, 2, 6, 15);
	}
	else
	{
		alarmOutput[LOW_PRESSURE] = 0;
	}

	//	8 - Compressor Thermal Overload
	if (workingStatus1 & 0x0100)
	{
		showIcon(bellIcon, 14, 2, 6, 15);
		alarmOutput[COMPRESSOR_THERMAL_OVERLOAD] = 1;
	}
	else
	{
		alarmOutput[COMPRESSOR_THERMAL_OVERLOAD] = 0;
	}

	//	9 - Bad Board
	if (workingStatus1 & 0x0200)
	{
		showIcon(bellIcon, 14, 2, 6, 15);
		alarmOutput[BAD_BOARD] = 1;
	}
	else
	{
		alarmOutput[BAD_BOARD] = 0;
	}

	//	10 - Reverse Relay On
	//	11 - High Pressure Alarm
	if (workingStatus1 & 0x0800)
	{
		showIcon(bellIcon, 14, 2, 6, 15);
		alarmOutput[HIGH_PRESSURE] = 1;
	}
	else
	{
		alarmOutput[HIGH_PRESSURE] = 0;
	}

	//	12 - Condensor MI Thermal Overload
	if (workingStatus1 & 0x1000)
	{
		showIcon(bellIcon, 14, 2, 6, 15);
		alarmOutput[CONDENSOR_MI_THERMAL_OVERLOAD] = 1;
	}
	else
	{
		alarmOutput[CONDENSOR_MI_THERMAL_OVERLOAD] = 0;
	}

	//	13 - Forward Relay On
	//	14 - Inlet Temp Sensor Fault
	if (workingStatus1 & 0x4000)
	{
		showIcon(bellIcon, 14, 2, 6, 15);
		alarmOutput[INLET_TEMP_SENSOR_FAULT] = 1;
	}
	else
	{
		alarmOutput[INLET_TEMP_SENSOR_FAULT] = 0;
	}

	//	15 - Outlet Temp Sensor Fault
	if (workingStatus1 & 0x8000)
	{
		alarmOutput[OUTLET_TEMP_SENSOR_FAULT] = 1;
		showIcon(bellIcon, 14, 2, 6, 15);
	}
	else
	{
		alarmOutput[OUTLET_TEMP_SENSOR_FAULT] = 0;
	}
}

void showWorkingStatus2(void)
{
	int workingStatus2 = modbus_ro_reg_rcv[WORKING_STATUS_2].ivalue;
//	workingStatus2 = 0;

	//	0 - Frost Alarm
	if (workingStatus2 & 0x0001)
	{
		alarmOutput[FROST] = 1;
//		showIcon(bellIcon, 14, 2, 6, 15);
	}
	else
	{
		alarmOutput[FROST] = 0;
	}

	//	1 - Degraded mode
	//	2 - Probe 2 Present
	//	3 - Diagnostics Mode

	//	4 - Low Temp Alarm
	if (workingStatus2 & 0x0010)
	{
		alarmOutput[LOW_TEMP] = 1;
		showIcon(bellIcon, 14, 2, 6, 15);
	}
	else
	{
		alarmOutput[LOW_TEMP] = 0;
	}

	//	5 - High Temp Alarm
	if (workingStatus2 & 0x0020)
	{
		alarmOutput[HIGH_TEMP] = 1;
		showIcon(bellIcon, 14, 2, 6, 15);
	}
	else
	{
		alarmOutput[HIGH_TEMP] = 0;
	}

	//	6 - Disable Switch Alarm
	if (workingStatus2 & 0x0040)
	{
		alarmOutput[DISABLE_SWITCH_ALARM] = 1;
		showIcon(bellIcon, 14, 2, 6, 15);
	}
	else
	{
		alarmOutput[DISABLE_SWITCH_ALARM] = 0;
	}

	//	7 - Over Current
	if (workingStatus2 & 0x0080)
	{
		alarmOutput[OVER_CURRENT] = 1;
		showIcon(bellIcon, 14, 2, 6, 15);
	}
	else
	{
		alarmOutput[OVER_CURRENT] = 0;
	}

	//	8 - Cooling On
	if ((workingStatus2 & 0x0100) && modbus_ro_reg_rcv[COMP_DELAY_TIMER].ivalue > 0)
	{
		alarmOutput[COMPRESSOR_COUNT_DOWN] = 1;
		alarmOutput[COMPRESSOR_COUNT_DOWN_TIMER] = 1;
	}
	else
	{
		alarmOutput[COMPRESSOR_COUNT_DOWN] = 0;
		alarmOutput[COMPRESSOR_COUNT_DOWN_TIMER] = 0;
	}

	//	9 - Not Used
	//	10 - Not Used
	//	11 - Not Used
	//	12 - Heater On
	if (workingStatus2 & 0x1000)
	{
		showIcon(resisterIcon, 14, 2, 6, 34);
	}
	else
	{
		hideIcon(14, 6, 34);
	}

	//	13 - Condensor MI On
	if (workingStatus2 & 0x2000)
	{
		showIcon(condensorIcon, 14, 2, 6, 53);
	}
	else
	{
		hideIcon(14, 6, 53);
	}

	//	14 - Evap MI On
	if (workingStatus2 & 0x4000)
	{
		showIcon(evapIcon, 14, 2, 6, 91);
	}
	else
	{
		hideIcon(14, 6, 91);
	}

	//	15 - Compressor On
	if (workingStatus2 & 0x8000)
	{
		showIcon(compIcon, 14, 2, 6, 110);
	}
	else
	{
		hideIcon(14, 6, 110);
	}
}

void showDegree(unsigned char page, unsigned char column)
{
	if (modbus_rw_coil_rcv[UNIT_OF_MEASURE/8] & UNIT_OF_MEASURE_F)
	{
		showIcon(degreeF, 8, 2, page, column);
	}
	else
	{
		showIcon(degreeC, 8, 2, page, column);
	}
}

void showTempReading(int tempReading, unsigned char page, unsigned char column)
{
	if ((tempReading >= 1000) || (tempReading <= -1000))
	{
		showIntegerRightAligned(digitCalibriLight4x28, tempReading/10, 4, 28, page, column);
	}
	else
	{
		showIntegerRightAligned(digitCalibriLight4x28, tempReading, 4, 28, page, column);
		showIcon(decimal_point, DECIMAL_POINT_WIDTH, 1, page+3, column-28);
	}
	showDegree(page,115);
}

void showTimer(const char font[], unsigned char minute, unsigned char second, unsigned char heightInPages, unsigned char widthInRows, unsigned char page, unsigned char column)
{
	if (minute >= 100)
	{
		minute = 99;
	}
	else if (minute <= -1)
	{
		minute = 0;
	}

	if (second >= 60)
	{
		second = 59;
	}
	else if (second <= -1)
	{
		second = 0;
	}

	// Minute
	showDigit(font, minute/10, heightInPages, widthInRows, page, column);
	showDigit(font, minute%10, heightInPages, widthInRows, page, column+widthInRows);

	// :
	showIcon(decimal_point, DECIMAL_POINT_WIDTH, 1, page, column + widthInRows*2+2);
	showIcon(decimal_point, DECIMAL_POINT_WIDTH, 1, page+3, column + widthInRows*2+2);

	// second
	showDigit(font, second/10, heightInPages, widthInRows, page, column + widthInRows*2+6);
	showDigit(font, second%10, heightInPages, widthInRows, page, column + widthInRows*3+6);
}

