/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_src.h"
#include "fsl_common.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_SRC SRC

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
 * @brief Main function
 */
int main(void)
{
    uint8_t ch;
    uint32_t flags;

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("Example: SRC Reset Soruce.\r\n");

    flags = SRC_GetResetStatusFlags(DEMO_SRC);
    SRC_ClearResetStatusFlags(DEMO_SRC, flags);

    PRINTF("SRC_GetResetStatusFlags(): 0x%X.\r\n", flags);
    PRINTF("SRC_GetBootModeWord1(): 0x%X.\r\n", SRC_GetBootModeWord1(DEMO_SRC));
    PRINTF("SRC_GetBootModeWord2(): 0x%X.\r\n", SRC_GetBootModeWord2(DEMO_SRC));

    while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }
}
