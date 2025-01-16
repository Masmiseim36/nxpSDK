/*
 * Copyright 2019 NXP
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
    SDK_DelayAtLeastUs(100, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
