/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"
#include "app.h"
#include "board.h"
#include "ezhv_support.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    BOARD_InitHardware();

    /* Print a note to terminal. */
    PRINTF("Primary Core: This is a EZHV LED Blinky example\r\n");

    /* Boot EZHV */
    BOARD_EZHV_Init();

    while(1)
    {
    }
}
