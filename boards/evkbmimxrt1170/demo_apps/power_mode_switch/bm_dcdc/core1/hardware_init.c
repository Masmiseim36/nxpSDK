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
/*${header:end}*/

/********************** End of forword declaration. ***************/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    clock_root_config_t rootCfg = {0};

    BOARD_ConfigMPU();
    BOARD_InitPins();

    rootCfg.mux = kCLOCK_LPUART12_ClockRoot_MuxOscRc16M;
    rootCfg.div = 1;
    CLOCK_SetRootClock(kCLOCK_Root_Lpuart12, &rootCfg);

    BOARD_InitDebugConsole();
}
/*${function:end}*/
