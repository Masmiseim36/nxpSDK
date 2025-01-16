/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_utick.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define UTICK_TIME_1S (1000000UL)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile bool utickExpired;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
static void UTickCallback(void)
{
    utickExpired = true;
}

static void UTickDelay(uint32_t usec)
{
    /* Set the UTICK timer to wake up the device from reduced power mode */
    UTICK_SetTick(EXAMPLE_UTICK, kUTICK_Onetime, usec - 1, UTickCallback);

    while (!utickExpired)
    {
    }

    utickExpired = false;
}

int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    PRINTF("Utick example start...\r\n");

    /* Intiialize UTICK */
    UTICK_Init(EXAMPLE_UTICK);

    while (true)
    {
        PRINTF("UTICK delay 1 second...\r\n");
        UTickDelay(UTICK_TIME_1S);
    }
}
