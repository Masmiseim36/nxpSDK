/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* attach MAIN PLL clock to SCTimer input7 (396MHz / 11 = 36MHz). */
    CLOCK_AttachClk(kMAIN_PLL_to_SCT_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivSctClk, 10);
}
/*${function:end}*/
