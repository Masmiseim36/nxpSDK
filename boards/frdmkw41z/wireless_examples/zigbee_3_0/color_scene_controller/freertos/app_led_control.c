/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include "dbg.h"
#include "app_led_control.h"
#include "app_serial_interface.h"
#include "ZTimer.h"
#include "app_main.h"
#include "LED.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define DIO_LED_MASK  0x03UL
#define LED_BLINK_TIME ZTIMER_TIME_MSEC(50)    /*  0.05sec blink time */

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct
{
    uint32 u32DioDirection;
    uint32 u32DioOutput;
    const volatile uint32 u32DioInput;
} tsDio;

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE void vAppSetLedState(uint32 u32LedMask, bool_t bOn );

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
PRIVATE teShiftLevel eAppShiftLevel;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void APP_vInitLeds(void)
{
}


PUBLIC void APP_vBlinkLeds(teShiftLevel eShiftLevel)
{
    eAppShiftLevel = eShiftLevel;
    APP_vSetLeds((~eShiftLevel) & DIO_LED_MASK);
    ZTIMER_eStop(u8TimerLedBlink);
    ZTIMER_eStart(u8TimerLedBlink, LED_BLINK_TIME);
}

PUBLIC void APP_vSetLeds(teShiftLevel eShiftLevel)
{
    (eShiftLevel & 0x01) ? vAppSetLedState(1<<0, 1): vAppSetLedState(1<<0, 0);
    (eShiftLevel & 0x02) ? vAppSetLedState(1<<1, 1): vAppSetLedState(1<<1, 0);
#ifdef DR1199
    uint8 u8Leds = (uint8)eShiftLevel;
    /* this serial protocol message will cause what looks like garbage debug output on a terminal */
    vSL_WriteMessage(SERIAL_MSG_TYPE_LEDS, SERIAL_MSG_LENGTH_LEDS, &u8Leds);
#endif
}

PRIVATE void vAppSetLedState(uint32 u32LedMask, bool_t bOn )
{
    if (bOn)
    {
        Led_TurnOn(u32LedMask);
    }
    else
    {
        Led_TurnOff(u32LedMask);
    }
}

PUBLIC void APP_cbTimerLedBlink(void *pvParam)
{
    APP_vSetLeds(eAppShiftLevel);
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
