/*
 * Copyright 2020-2021 NXP
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

    CLOCK_EnableClock(kCLOCK_Usdhc1);
    CLOCK_EnableClock(kCLOCK_Usdhc2);
    CLOCK_EnableClock(kCLOCK_Sai1);
    CLOCK_EnableClock(kCLOCK_Sai2);
    CLOCK_EnableClock(kCLOCK_FlexSpi);
    CLOCK_EnableClock(kCLOCK_Spdif);
    CLOCK_EnableClock(kCLOCK_Can2S);
    CLOCK_EnableClock(kCLOCK_Enet);
    CLOCK_EnableClock(kCLOCK_Lpspi1);
    CLOCK_EnableClock(kCLOCK_Trace);
    CLOCK_EnableClock(kCLOCK_Pit);
    CLOCK_EnableClock(kCLOCK_Lpi2c1);

    /* Set Spdif clock source. */
    CLOCK_SetMux(kCLOCK_SpdifMux, 0);
}
/*${function:end}*/
