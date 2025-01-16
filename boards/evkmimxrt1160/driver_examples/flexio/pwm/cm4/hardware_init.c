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
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    clock_root_config_t rootCfg = {0};
    /* Configure flexio using Osc48MDiv2 */
    rootCfg.mux = 0;
    rootCfg.div = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Flexio2, &rootCfg);
}
/*${function:end}*/
