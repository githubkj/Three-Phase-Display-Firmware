#ifndef __DISPLAY_POSITION_H
#define __DISPLAY_POSITION_H

/********************* Positions ***********************/
#define PENTAIR_ICON_POSITION					-1
#define ICON_ALL_ON_POSITION					-2
#define MAIN_SCREEN_POSITION					0
#define MAIN_MENU_POSITION						1
#define LANGUAGE_MENU_POSITION					2
#define USER_INTERFACE_POSITION					3
#define SYSTEM_INTERFACE_POSITION				4
#define USER_PASSWORD_POSITION					5
#define SYSTEM_PASSWORD_POSITION				6
/*************** User Interface *****************/
#define COOLING_SET_POINT_POSITION  			7
#define COOLING_DIF_POSITION					8
#define HEATING_SET_POINT_POSITION  			9
#define HEATING_DIF_POSITION					10
#define TEMP_SCALE_POSITION						11
#define HYSTERESIS_POSITION						12
#define PASSWORD_POSITION						13
/*************** System Interface ****************/
#define DOOR_SMOKE_ALARM_POSITION   			14
#define DISABLE_SWITCH_POSITION					15
#define ALARM_RELAY_OUTPUT_POSITION				16
#define ABOUT_POSITION							17
#define COMPRESSOR_RESTART_DELAY_POSITION    	18
#define HIGH_TEMP_ALARM_SP_POSITION 			19
#define LOW_TEMP_ALARM_SP_POSITION  			20
#define COMPRESSOR_CURRENT_POSITION   			21
#define DISPLAY_SYS_SETTINGS_MENU_POSITION   	22

/************ Display System Settings ***************/
#define CURRENT_LIMIT_SETTINGS_POSITION			24
#define MALF_ACTIVATE_POSITION					25
#define MALF_DEACTIVATE_POSITION				26
#define HPC_FAN_ON_POSITION						27
#define HPC_FAN_OFF_POSITION					28

#define THREE_PHASE_CURRENT_MENU_POSITION       29

#define EVAP_MI_TEST				1
#define COND_MI_TEST				2
#define COMP_MI_TEST				3
#define HEATER_MI_TEST				4

/******************** Alarm Display **********************/
#define HIGH_TEMP_ALARM_POSITION						50
#define LOW_TEMP_ALARM_POSITION							51
#define DOOR_ALARM_POSITION								52
#define PHASE_MISSING_ALARM_POSITION					53
#define EVAP_MI_THERMAL_OVERLOAD_ALARM_POSITION			54
#define CONDENSOR_MI_THERMAL_OVERLOAD_ALARM_POSITION	55
#define COMPRESSOR_THERMAL_OVERLOAD_ALARM_POSITION		56
#define OVER_VOLTAGE_ALARM_POSITION						57
#define UNDER_VOLTAGE_ALARM_POSITION					58
#define BAD_BOARD_ALARM_POSITION						59
#define HIGH_PRESSURE_ALARM_POSITION					60
#define LOW_PRESSURE_ALARM_POSITION						61
#define FROST_ALARM_POSITION							62
#define COIL_TEMP_SENSOR_FAULT_ALARM_POSITION			63
#define INLET_TEMP_SENSOR_FAULT_ALARM_POSITION			64
#define OUTLET_TEMP_SENSOR_FAULT_ALARM_POSITION			65
#define OVER_CURRENT_ALARM_POSITION						66
#define DISABLE_SWITCH_ALARM_POSITION					67
#define COMPRESSOR_RESTART_DELAY_COUNT_DOWN_POSITION	68
#define COMPRESSOR_RESTART_DELAY_TIMER_POSITION			69

#define CHECK_ALL_PIXELS								70

/**************** Alarm Output ******************/
#define HIGH_TEMP							0
#define LOW_TEMP							1
#define DOOR_SMOKE							2
#define PHASE_MISSING						3
#define EVAP_MI_THERMAL_OVERLOAD			4
#define CONDENSOR_MI_THERMAL_OVERLOAD		5
#define COMPRESSOR_THERMAL_OVERLOAD			6
#define OVER_VOLTAGE						7
#define UNDER_VOLTAGE						8
#define BAD_BOARD							9
#define HIGH_PRESSURE						10
#define LOW_PRESSURE						11
#define FROST								12
#define COIL_TEMP_SENSOR_FAULT				13
#define INLET_TEMP_SENSOR_FAULT				14
#define OUTLET_TEMP_SENSOR_FAULT			15
#define OVER_CURRENT						16
#define DISABLE_SWITCH_ALARM				17
#define COMPRESSOR_COUNT_DOWN				18
#define COMPRESSOR_COUNT_DOWN_TIMER			19

#define NUMBER_OF_ALARMS					20

/******************** Line Numbers **************************/
// Probe #1 -- #3
#define INLET_LINENUM	1
#define OUTLET_LINENUM	2
#define COIL_LINENUM	3

struct Location {
	short displayLevel;
	short lineNumber;
};

extern struct Location currentPosition;
extern int alarmOutput[];
extern int alarmOutputIndex;
extern int alarmStartPosition;

extern _Bool updateDisplayPosition;

void checkDisplayPosition(void);

#endif /* __DISPLAY_POSITION_H */
