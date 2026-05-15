/*
 * Copyright 2018 NXP
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
    BOARD_CommonSetting();
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitFLEXIO_PWMPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    clock_root_config_t rootCfg = {0};
    /* Configure flexio using OSC 24M */
    rootCfg.mux = kCLOCK_FLEXIO1_ClockRoot_MuxOscRc24M;
    rootCfg.div = 4;
    CLOCK_SetRootClock(kCLOCK_Root_Flexio1, &rootCfg);
}
/*${function:end}*/
