/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef APP_SWITCH_STATE_MACHINE_H_
#define APP_SWITCH_STATE_MACHINE_H_

 /*!
\file       app_switch_state_machine.h
\brief      ZLO Controller & Switch Demo: Remote Key Press Behaviour (Interface)
*/

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include "zcl.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define BUTTON_DELAY_TIME_IN_MS 250

#define ON   (1 << APP_BUTTONS_BUTTON_SW1)
#define OFF  (1 << APP_BUTTONS_BUTTON_SW2 )
#define UP   (1 << APP_BUTTONS_BUTTON_SW3 )
#define DOWN (1 << APP_BUTTONS_BUTTON_SW4 )
#define NTAG_FD (1 << APP_BUTTONS_NFC_FD)
#define COMM (1 << APP_BUTTONS_BUTTON_1)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef enum{
    SCENE_1=1,
    SCENE_2
}te_ScenesId;

typedef enum{
    LIGHT_CONTROL_MODE,
    INDIVIDUAL_CONTROL_MODE,
    COMMISSIONING_MODE,
    NUMBER_OF_MODES
}te_SwitchState;

/* Kindly Maintain the order as the button numbers are assigned directly */

typedef enum{
    COMM_BUTTON_PRESSED,                // 0
    ON_PRESSED,                         // 1
    OFF_PRESSED,                        // 2
    UP_PRESSED,                         // 3
    DOWN_PRESSED,                       // 4
    NTAG_FD_PRESSED,                    // 5
    UP_AND_ON_TOGETHER_PRESSED,         // 6
    UP_AND_OFF_TOGETHER_PRESSED,        // 7
    DOWN_AND_OFF_TOGETHER_PRESSED,      // 8
    DOWN_AND_ON_TOGETHER_PRESSED,       // 9
    COMM_AND_ON_TOGETHER_PRESSED,       // 10
    COMM_AND_OFF_TOGETHER_PRESSED,      // 11
    COMM_AND_UP_TOGETHER_PRESSED,       // 12
    COMM_AND_DOWN_TOGETHER_PRESSED,     // 13
    COMM_BUTTON_RELEASED,               // 14
    UP_RELEASED,                        // 15
    DOWN_RELEASED,                      // 16
    NTAG_FD_RELEASED,                   // 17
    NUMBER_OF_TRANSITION_CODE           // 18
}te_TransitionCode;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void vApp_ProcessKeyCombination(APP_tsEvent sButton);
PUBLIC te_SwitchState eGetSwitchState(void);
PUBLIC void APP_cbTimerButtonDelay(void *pvParam);
PUBLIC void vStopStartModeChangeTimer( uint32 u32Ticks );
PUBLIC void APP_cbTimerChangeMode(void *pvParam);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

#endif /* APP_SWITCH_STATE_MACHINE_H_ */
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
