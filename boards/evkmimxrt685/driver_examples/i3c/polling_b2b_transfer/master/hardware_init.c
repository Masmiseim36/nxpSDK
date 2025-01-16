/*
 * Copyright 2019, 2022, 2024 NXP
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
    /* Attach FFRO clock to I3C, 48MHz / 2 = 24MHz. */
    CLOCK_AttachClk(kFFRO_to_I3C_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivI3cClk, 2);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
