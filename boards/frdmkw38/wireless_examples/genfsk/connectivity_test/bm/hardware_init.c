/*
* Copyright 2019 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
 
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
#if defined(gDebugConsoleEnable_d) && (gDebugConsoleEnable_d == 1)
    BOARD_InitDebugConsole();
#endif
}
/*${function:end}*/
