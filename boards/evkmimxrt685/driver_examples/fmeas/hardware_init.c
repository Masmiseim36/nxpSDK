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
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_InputMux);
    CLOCK_EnableClock(kCLOCK_Freqme);
    RESET_PeripheralReset(kFREQME_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kINPUTMUX_RST_SHIFT_RSTn);

    /* Attach clock out to SYSPLL PFD0 with divider 100. */
    CLOCK_AttachClk(kMAIN_PLL_to_CLKOUT);
    CLOCK_SetClkDiv(kCLOCK_DivClockOut, 100);
}
/*${function:end}*/
