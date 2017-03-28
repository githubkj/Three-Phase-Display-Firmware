#include "button.h"
#include "Back_Button.h"
#include "BitIoLdd6.h"
#include "OK_Button.h"
#include "BitIoLdd7.h"
#include "Up_Button.h"
#include "BitIoLdd8.h"
#include "Down_Button.h"
#include "BitIoLdd9.h"

#include "displayPosition.h"
#include "stdbool.h"
#include "Events.h"

/*
** ===================================================================
**     Method      :  		scanButton
**     Description :		Scans button states
** ===================================================================
*/
void scanButton(void)
{
	// Back button
	if (!Back_Button_GetVal())
	{
		releasedBack = false;

		if (!TI1_backButtonTimerIsOn)
		{
			TI1_backButtonTimerIsOn = true;
			TI1_backButtonTimer = 0;
		}
		else
		{
			if (TI1_backButtonTimer < BUTTON_HELD_TIME && TI1_backButtonTimer > BUTTON_DEBOUNCE_TIME)
			{
				pressedBack = true;
			}
			if (TI1_backButtonTimer >= BUTTON_HELD_TIME)
			{
				heldBack = true;
			}
		}
	}
	else
	{
		TI1_backButtonTimerIsOn = false;
		TI1_backButtonTimer = 0;

		if (pressedBack || heldBack)
		{
			pressedBack = false;
			heldBack = false;
			releasedBack = true;
		}
	}

	// OK Button
	if (!OK_Button_GetVal())
	{
		releasedOK = false;

		if (!TI1_OKButtonTimerIsOn)
		{
			TI1_OKButtonTimerIsOn = true;
			TI1_OKButtonTimer = 0;
		}
		else
		{
			if (TI1_OKButtonTimer < BUTTON_HELD_TIME && TI1_OKButtonTimer > BUTTON_DEBOUNCE_TIME)
			{
				pressedOK = true;
			}
			if (TI1_OKButtonTimer >= BUTTON_HELD_TIME)
			{
				heldOK = true;
			}
		}
	}
	else
	{
		TI1_OKButtonTimerIsOn = false;
		TI1_OKButtonTimer = 0;

		if (pressedOK || heldOK)
		{
			pressedOK = false;
			heldOK = false;
			releasedOK = true;
		}
	}

	// Up button
	if (!Up_Button_GetVal())
	{
		releasedUp = false;

		if (!TI1_upButtonTimerIsOn)
		{
			TI1_upButtonTimerIsOn = true;
			TI1_upButtonTimer = 0;
		}
		else
		{
			if (TI1_upButtonTimer < BUTTON_HELD_TIME && TI1_upButtonTimer > BUTTON_DEBOUNCE_TIME)
			{
				pressedUp = true;
			}
			if (TI1_upButtonTimer >= BUTTON_HELD_TIME)
			{
				heldUp = true;
			}
		}
	}
	else
	{
		TI1_upButtonTimerIsOn = false;
		TI1_upButtonTimer = 0;

		if (pressedUp || heldUp)
		{
			pressedUp = false;
			heldUp = false;
			releasedUp = true;
		}
	}


	// Down button
	if (!Down_Button_GetVal())
	{
		releasedDown = false;

		if (!TI1_downButtonTimerIsOn)
		{
			TI1_downButtonTimerIsOn = true;
			TI1_downButtonTimer = 0;
		}
		else
		{
			if (TI1_downButtonTimer < BUTTON_HELD_TIME && TI1_downButtonTimer > BUTTON_DEBOUNCE_TIME)
			{
				pressedDown = true;
			}
			if (TI1_downButtonTimer >= BUTTON_HELD_TIME)
			{
				heldDown = true;
			}
		}
	}
	else
	{
		TI1_downButtonTimerIsOn = false;
		TI1_downButtonTimer = 0;

		if (pressedDown || heldDown)
		{
			pressedDown = false;
			heldDown = false;
			releasedDown = true;
		}
	}

	// Press Four Button: Check Pixels
	if (pressedDown && pressedUp && pressedBack && pressedOK)
	{
		currentPosition.displayLevel = CHECK_ALL_PIXELS;
		updateDisplayPosition = true;
	}

	if (pressedBack  || pressedOK  || pressedUp  || pressedDown  ||
		releasedBack || releasedOK || releasedUp || releasedDown ||
        heldBack     || heldOK     || heldUp     || heldDown)
	{
		updateDisplayPosition = true;
	}

	// Idle
	if (!pressedDown && !pressedUp && !pressedBack && !pressedOK)
	{
		// set the idle timer counter
		if ((currentPosition.displayLevel == MAIN_SCREEN_POSITION) && (currentPosition.lineNumber != INLET_LINENUM))
		{
			TI1_idleTimer_cnt = IDLE_TIMER_COUNTER_30_SEC;
		}
		else if (currentPosition.displayLevel != MAIN_SCREEN_POSITION)
		{
			TI1_idleTimer_cnt = IDLE_TIMER_COUNTER_1_MIN;
		}
		TI1_idleTimer_Flag = true;
	}
	else
	{
		TI1_idleTimer_Flag = false;
		TI1_idleTimer = 0;
	}
}
