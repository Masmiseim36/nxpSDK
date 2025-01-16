/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitDebugConsole();
    BOARD_InitBootPins();
    CLOCK_EnableClock(kCLOCK_Gpio2);
}
/*${function:end}*/
