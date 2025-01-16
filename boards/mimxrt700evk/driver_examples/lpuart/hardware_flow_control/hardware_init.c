/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_clock.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_InitBootClocks();
    
    BOARD_InitDebugConsole();

    CLOCK_AttachClk(kMAIN_PLL_PFD3_to_FCCLK1);
    CLOCK_SetClkDiv(kCLOCK_DivFcclk1Clk, 11U);
    CLOCK_AttachClk(kFCCLK1_to_FLEXCOMM4);
}
/*${function:end}*/
