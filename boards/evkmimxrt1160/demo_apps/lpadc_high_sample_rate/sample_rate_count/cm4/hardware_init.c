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
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    clock_root_config_t busRootCfg = {0};
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Set bus clock root mux to system PLL3. */
    busRootCfg.mux = kCLOCK_BUS_ClockRoot_MuxSysPll3Out;
    busRootCfg.div = 2;
    CLOCK_SetRootClock(kCLOCK_Root_Bus, &busRootCfg);
}

void BOARD_InitADCClock(void)
{
    clock_root_config_t adc1ClkRoot;
    /* Set ADC1 CLK as 88MHz */
    adc1ClkRoot.mux      = 6U; /* Set clock source as SYS PLL2 CLK. */
    adc1ClkRoot.div      = 6U;
    adc1ClkRoot.clockOff = false;
    CLOCK_SetRootClock(kCLOCK_Root_Adc1, &adc1ClkRoot);
}
/*${function:end}*/
