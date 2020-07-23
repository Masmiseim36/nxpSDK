/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_gpio.h"
#include "mcdrv_periph_init.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * Functions enables the DC-bus brake.
 */
void MCDRV_fsBrakeOn(void)
{
    /* Raise the brake control signal. */
    PIN_MCU_BRAKE_GPIO->PSOR = (0x01UL << PIN_MCU_BRAKE_PIN);
}

/*!
 * Functions disables the DC-bus brake.
 */
void MCDRV_fsBrakeOff(void)
{
    /* Clear the brake control signal. */
    PIN_MCU_BRAKE_GPIO->PCOR = (0x01UL << PIN_MCU_BRAKE_PIN);
}

