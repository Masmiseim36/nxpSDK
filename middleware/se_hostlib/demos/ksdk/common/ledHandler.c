/**
* @file ledHandler.c
* @author NXP Semiconductors
* @version 1.0
* @par License
* Copyright 2017,2018 NXP
*
* This software is owned or controlled by NXP and may only be used
* strictly in accordance with the applicable license terms.  By expressly
* accepting such terms or by downloading, installing, activating and/or
* otherwise using the software, you are agreeing that you have read, and
* that you agree to comply with and are bound by, such license terms.  If
* you do not agree to be bound by the applicable license terms, then you
* may not retain, install, activate or otherwise use the software.
*
* @par Description
* Led Handler
*/

#include "sm_types.h"
#include "nxLog_App.h"
#if AX_EMBEDDED &&  !defined(IMX_RT)
#include "board.h"
#endif
#include "ledHandler.h"

void led_handler(ledColor_t led, ledState_t state)
{
    switch (led) {
    case RED:
        if (state == LED_ON) {
            LED_RED_ON();
        }
        else if (state == LED_OFF) {
            LED_RED_OFF();
        }
        else /*State is an enum so safely making use of else*/
        {
            LED_RED_TOGGLE();
        }
        break;
    case GREEN:
        if (state == LED_ON) {
            LED_GREEN_ON();
        }
        else if (state == LED_OFF) {
            LED_GREEN_OFF();
        }
        else /*State is an enum so safely making use of else*/
        {
            LED_GREEN_TOGGLE();
        }
        break;
    case BLUE:
        if (state == LED_ON) {
            LED_BLUE_ON();
        }
        else if (state == LED_OFF) {
            LED_BLUE_OFF();
        }
        else /*State is an enum so safely making use of else*/
        {
            LED_BLUE_TOGGLE();
        }
        break;
    default:
        LOG_W("wrong LED \n");
    }
}
