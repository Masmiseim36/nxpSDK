/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=================================================================================================
\file       app_blink_led.c
\brief      ZLO Demo: Blinks an LED a number of pre defined ticks.
==================================================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "jendefs.h"
#include "dbg.h"
#include "App_OccupancySensor.h"
#include "app_blink_led.h"

#include "LED.h"

#include "ZTimer.h"
#include "app_main.h"
#include "pwrm.h"
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#ifdef DEBUG_BLINK_LED
    #define TRACE_BLINK_LED   TRUE
#else
    #define TRACE_BLINK_LED   FALSE
#endif
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
#define BLINK_LED   LED3
/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void vToggleLED(void);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
PRIVATE bool_t bDIO1State = FALSE;
PRIVATE uint32 u32BlinkTickTime = 0;
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: APP_vInitLeds
 *
 * DESCRIPTION:
 * Initialises LED's
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vInitLeds(void)
{
}

/****************************************************************************
 *
 * NAME: APP_vSetLED
 *
 * DESCRIPTION:
 * Initialises LED's
 *
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void APP_vSetLED(uint8 u8Led, bool_t bOn)
{
    if (bOn)
    {
        Led_TurnOn(u8Led);
    }
    else
    {
        Led_TurnOff(u8Led);
    }
}

/****************************************************************************
 *
 * NAME: APP_BlinkLED
 *
 * DESCRIPTION:
 * Toggles LED 2 and restarts the timer.
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vAPP_cbBlinkLED( void *pvParams)
{
    DBG_vPrintf(TRACE_BLINK_LED, "\nAPP Blink LED: Task Started");
    vToggleLED();
    ZTIMER_eStart(u8TimerBlink, u32BlinkTickTime);
}

/****************************************************************************
 *
 * NAME: vStartBlinkTimer
 *
 * DESCRIPTION:
 * Starts the software timer and blinks it the desired amount of ticks. It also
 * stores the time so the Task can restart it with the defined time
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vStartBlinkTimer(uint32 u32Ticks)
{
    DBG_vPrintf(TRACE_BLINK_LED, "\nAPP Blink LED: Starting Blink Timer value = %d", u32Ticks);
    u32BlinkTickTime = u32Ticks;
    ZTIMER_eStart(u8TimerBlink, u32Ticks);
}

/****************************************************************************
 *
 * NAME: vStopBlinkTimer
 *
 * DESCRIPTION:
 * Stops the blink timer and turns the LED off
 * RETURNS:
 * void
 *
 ****************************************************************************/
PUBLIC void vStopBlinkTimer(void)
{
    DBG_vPrintf(TRACE_BLINK_LED, "\nAPP Blink LED: Stopping Blink Timer");
    ZTIMER_eStop(u8TimerBlink);
    u32BlinkTickTime = 0;
    APP_vSetLED(BLINK_LED, FALSE);
}

/****************************************************************************/
/***        Local Function                                                ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME: vToggleLED
 *
 * DESCRIPTION:
 * Changes the state of LED 2
 * RETURNS:
 * void
 *
 ****************************************************************************/
PRIVATE void vToggleLED(void)
{
    DBG_vPrintf(TRACE_BLINK_LED, "\nAPP Blink LED: Toggle LED to %b", !bDIO1State);
    bDIO1State = !bDIO1State;
    APP_vSetLED(BLINK_LED, bDIO1State);
}
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
