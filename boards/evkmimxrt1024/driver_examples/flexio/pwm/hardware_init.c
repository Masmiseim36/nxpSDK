/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Clock setting for Flexio */
    CLOCK_SetMux(kCLOCK_Flexio1Mux, FLEXIO_CLOCK_SELECT);
    CLOCK_SetDiv(kCLOCK_Flexio1PreDiv, FLEXIO_CLOCK_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Flexio1Div, FLEXIO_CLOCK_DIVIDER);
}
/*${function:end}*/
