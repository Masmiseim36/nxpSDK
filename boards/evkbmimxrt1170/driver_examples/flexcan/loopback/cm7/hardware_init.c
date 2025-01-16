/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "app.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /*Clock setting for FLEXCAN*/
    clock_root_config_t rootCfg = {0};
    rootCfg.mux                 = FLEXCAN_CLOCK_SOURCE_SELECT;
    rootCfg.div                 = FLEXCAN_CLOCK_SOURCE_DIVIDER;
    CLOCK_SetRootClock(kCLOCK_Root_Can3, &rootCfg);
}
/*${function:end}*/
