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
    BOARD_InitLPUART();
    BOARD_InitI2C();
    BOARD_InitSPI();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /* Turn on RTC clock */
    CLOCK_EnableClock(kCLOCK_Rtc0);
}
/*${function:end}*/
