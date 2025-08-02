/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_debug_console.h"
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    clock_root_config_t rootCfg = {0};

    /* Board pin, clock, debug console init */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    rootCfg.mux = kCLOCK_FLEXIO2_ClockRoot_MuxOscRc400M;
    rootCfg.div = 5;
    CLOCK_SetRootClock(DEMO_FLEXIO_CLOCK, &rootCfg);
}
/*${function:end}*/
