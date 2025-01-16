/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
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
    rootCfg.mux                 = kCLOCK_BUS_WAKEUP_ClockRoot_MuxSysPll2Out;
    rootCfg.div                 = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Bus_Wakeup, &rootCfg);
}
/*${function:end}*/
