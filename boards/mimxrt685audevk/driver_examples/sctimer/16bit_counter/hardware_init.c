/*
 * Copyright 2021 NXP
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

    /* attach FFRO clock to SCTimer input7 (48MHz / 1 = 48MHz). */
    CLOCK_AttachClk(kFFRO_to_SCT_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivSctClk, 1);
}
/*${function:end}*/
