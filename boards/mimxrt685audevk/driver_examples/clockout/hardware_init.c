/*
 * Copyright 2021 NXP
 * All rights reserved.
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

    CLOCK_InitSysPfd(kCLOCK_Pfd1, 19U); /* Enable DSP PLL clock */
    CLOCK_InitSysPfd(kCLOCK_Pfd3, 19U); /* Enable AUX1 PLL clock */
}
/*${function:end}*/
