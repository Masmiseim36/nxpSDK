/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP__H_
#define aPP__NODEH_

#include "app_led_control.h"
//#ifdef DR1159
#include "app_captouch_buttons.h"
//#endif

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
#ifdef DR1199
/*
 * Alturnative key codes for use with the GUI
 */
typedef enum
{
    BUTTON_1,
    BUTTON_SW5,
    BUTTON_SW4,
    BUTTON_SW3,
    BUTTON_SW2,
    KEY_1 = 0x10,
    KEY_2,
    KEY_3,
    KEY_4,

    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,

    KEY_9,
    KEY_10,
    KEY_11,
    KEY_12,

    KEY_13,
    KEY_14,
    KEY_15,
    KEY_16,

} teUserKeyCodes;
#endif


/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void APP_vHandleFactoryNewKeyPress(teUserKeyCodes eKeyCode);
PUBLIC void APP_vHandleKeyPress(teUserKeyCodes eKeyCode);
PUBLIC void APP_vHandleKeyRelease(teUserKeyCodes eKeyCode);


/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/
extern PUBLIC teShiftLevel eShiftLevel;


/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /*APP___H_*/
