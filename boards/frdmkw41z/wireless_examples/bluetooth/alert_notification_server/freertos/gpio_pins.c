/*
* Copyright (c) 2013-2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
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
        .gpioPort = gpioPort_C_c,
        .gpioPin = 5,
        .pullSelect = pinPull_Up_c,
        .interruptSelect = pinInt_FallingEdge_c
    },
    {
        .gpioPort = gpioPort_C_c,
        .gpioPin = 4,
        .pullSelect = pinPull_Up_c,
        .interruptSelect = pinInt_FallingEdge_c
    }
};

/* Declare Output GPIO pins */
gpioOutputPinConfig_t ledPins[] = {
    {
        .gpioPort = gpioPort_B_c,
        .gpioPin = 0,
        .outputLogic = 1,
        .slewRate = pinSlewRate_Slow_c,
        .driveStrength = pinDriveStrength_Low_c
    },
    {
        .gpioPort = gpioPort_C_c,
        .gpioPin = 1,
        .outputLogic = 1,
        .slewRate = pinSlewRate_Slow_c,
        .driveStrength = pinDriveStrength_Low_c
    },
    {
        .gpioPort = gpioPort_A_c,
        .gpioPin = 19,
        .outputLogic = 1,
        .slewRate = pinSlewRate_Slow_c,
        .driveStrength = pinDriveStrength_Low_c
    },
    {
        .gpioPort = gpioPort_A_c,
        .gpioPin = 18,
        .outputLogic = 1,
        .slewRate = pinSlewRate_Slow_c,
        .driveStrength = pinDriveStrength_Low_c
    },
    {
        .gpioPort = gpioPort_C_c,
        .gpioPin = 18,
        .outputLogic = 1,
        .slewRate = pinSlewRate_Slow_c,
        .driveStrength = pinDriveStrength_High_c
    }
};

/* RF_DENY Coexistence Pin */
gpioInputPinConfig_t  gCoexistence_RfDeny = {
    .gpioPort = gpioPort_A_c,
    .gpioPin = 17,
    .pullSelect = pinPull_Disabled_c,
    .interruptSelect = pinInt_EitherEdge_c
    //.interruptSelect = pinInt_Disabled_c
};

/* RF_ACTIVE Coexistence Pin - used only in SW mode (ex: 802.15.4) */
gpioOutputPinConfig_t  gCoexistence_RfActive = {
    .gpioPort = gpioPort_C_c,
    .gpioPin = 1,
    .outputLogic = 0,
    .slewRate = pinSlewRate_Fast_c,
    .driveStrength = pinDriveStrength_Low_c
};

/* RF_STATUS Coexistence Pin - used only in SW mode (ex: 802.15.4) */
gpioOutputPinConfig_t  gCoexistence_RfStatus = {
    .gpioPort = gpioPort_C_c,
    .gpioPin = 3,
    .outputLogic = 0,
    .slewRate = pinSlewRate_Fast_c,
    .driveStrength = pinDriveStrength_Low_c
};

