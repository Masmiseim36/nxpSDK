/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "app.h"
#include "board.h"
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

/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize standard SDK demo application pins */
    BOARD_InitHardware();

    /* Print the initial banner */
    PRINTF("\r\nHello World running on core 'Cortex-M33'\r\n");

    /* Copy DSP image to RAM and start DSP core. */
    BOARD_DSP_Init();

    while (1)
        ;
}
