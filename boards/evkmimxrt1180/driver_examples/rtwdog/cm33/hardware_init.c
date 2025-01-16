/*
 * Copyright 2022 NXP
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
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    /* Enable RTWDOG clock */
    CLOCK_EnableClock(kCLOCK_Wdog1);
    NVIC_EnableIRQ(RTWDOG1_IRQn);
    /* Misc config to enable the watchdog */
    SRC_GENERAL_REG->SRMASK         = 0;
    BLK_CTRL_S_AONMIX->LP_HANDSHAKE = 0xFFFFFFFF;
    /* Clear the SRSR manually*/
    SRC_GENERAL_REG->SRSR = SRC_GENERAL_REG->SRSR;
}
/*${function:end}*/
