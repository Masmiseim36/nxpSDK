/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "board_fusionf1.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    CLOCK_SetXtalFreq(BOARD_XTAL_SYS_CLK_HZ);
    BOARD_InitDebugConsole();
}
/*${function:end}*/
