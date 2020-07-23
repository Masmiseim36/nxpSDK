/*
 * Copyright 2019-2020 NXP
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
        .gpioPin = 18,
        .pullSelect = pinPull_Up_c,
        .interruptSelect = pinInt_FallingEdge_c
    },
    {
        .gpioPort = gpioPort_C_c,
        .gpioPin = 2,
        .pullSelect = pinPull_Up_c,
        .interruptSelect = pinInt_FallingEdge_c
    }
};

/* Declare Output GPIO pins */
gpioOutputPinConfig_t ledPins[] = {
    {
        .gpioPort = gpioPort_B_c,
        .gpioPin = 3,
        .outputLogic = 0,
        .slewRate = pinSlewRate_Slow_c,
        .driveStrength = pinDriveStrength_Low_c
    },
    {
        .gpioPort = gpioPort_C_c,
        .gpioPin = 1,
        .outputLogic = 0,
        .slewRate = pinSlewRate_Slow_c,
        .driveStrength = pinDriveStrength_Low_c
    },
    {
        .gpioPort = gpioPort_A_c,
        .gpioPin = 16,
        .outputLogic = 0,
        .slewRate = pinSlewRate_Slow_c,
        .driveStrength = pinDriveStrength_Low_c
    },
    {
        .gpioPort = gpioPort_B_c,
        .gpioPin = 2,
        .outputLogic = 0,
        .slewRate = pinSlewRate_Slow_c,
        .driveStrength = pinDriveStrength_Low_c
    }
};

