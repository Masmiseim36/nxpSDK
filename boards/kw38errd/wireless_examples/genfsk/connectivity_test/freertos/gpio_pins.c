/*
* Copyright 2020 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "GPIO_Adapter.h"
#include "gpio_pins.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

gpioInputPinConfig_t switchPins[] = {
    {
        .gpioPort = gpioPort_B_c,
        .gpioPin = 2,
        .pullSelect = pinPull_Up_c,
        .interruptSelect = pinInt_FallingEdge_c
    },
#if !defined(gBoard_ExtPaChlCtrl_d) || (gBoard_ExtPaChlCtrl_d <= 0)
    {   /* SW4 */
        .gpioPort = gpioPort_B_c,
        .gpioPin = 18,
        .pullSelect = pinPull_Up_c,
        .interruptSelect = pinInt_FallingEdge_c
    }
#endif
};

/* Declare Output GPIO pins */
gpioOutputPinConfig_t ledPins[] = {
    {
        .gpioPort = gpioPort_B_c,
        .gpioPin = 3,
        .outputLogic = 0,
        .slewRate = pinSlewRate_Slow_c,
        .driveStrength = pinDriveStrength_Low_c
    }
};

#if defined(gBoard_ExtPaSupport_d) && (gBoard_ExtPaSupport_d > 0)
/* Declare sky66112 GPIO pins - shall be in the order given by structure BOARD_ExtPaCtrlPins_t in board_extPa.h*/
gpioOutputPinConfig_t ExtPaCtrlPins[] = {
    {
        .gpioPort = gpioPort_C_c,
        .gpioPin = 4,
        .outputLogic = 1,                                           /* CSD default 1, active */
        .slewRate = pinSlewRate_Slow_c,
        .driveStrength = pinDriveStrength_Low_c
    },
    {
        .gpioPort = gpioPort_C_c,
        .gpioPin = 5,
        .outputLogic = 0,                                           /* CPS, default 0, bypass disable */
        .slewRate = pinSlewRate_Slow_c,
        .driveStrength = pinDriveStrength_Low_c
    },
#if defined(gBoard_ExtPaChlCtrl_d) && (gBoard_ExtPaChlCtrl_d > 0)
    {
        .gpioPort = gpioPort_B_c,
        .gpioPin = 18,
        .outputLogic = 1,                                           /* CHL, default 1, Tx in linear mode */
        .slewRate = pinSlewRate_Slow_c,
        .driveStrength = pinDriveStrength_Low_c
    },
#endif
    {
        .gpioPort = gpioPort_C_c,
        .gpioPin = 1,
        .outputLogic = 1,                                           /* ANT_SEL, default 1, ANT2 port enabled */
        .slewRate = pinSlewRate_Slow_c,
        .driveStrength = pinDriveStrength_Low_c
    },
};
#endif
