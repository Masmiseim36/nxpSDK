/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitADCClock(void)
{
    clock_root_config_t adc1ClkRoot;
    adc1ClkRoot.mux      = 1U;
    adc1ClkRoot.div      = 7U;
    adc1ClkRoot.clockOff = false;
    CLOCK_SetRootClock(kCLOCK_Root_Adc1, &adc1ClkRoot);
}

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
