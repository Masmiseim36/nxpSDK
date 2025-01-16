/*
 * Copyright 2024 NXP
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
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Set PERCLK_CLK source to OSC_CLK*/
    CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
    /* Set PERCLK_CLK divider to 1 */
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);
}
/*${function:end}*/
