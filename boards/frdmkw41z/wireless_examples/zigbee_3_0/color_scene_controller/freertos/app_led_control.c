/*
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
