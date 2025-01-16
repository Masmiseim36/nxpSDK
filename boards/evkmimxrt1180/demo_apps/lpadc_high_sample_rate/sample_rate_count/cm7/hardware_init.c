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

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

void BOARD_InitADCClock(void)
{
    clock_root_config_t adc1ClkRoot;

    CLOCK_InitPfd(kCLOCK_PllSys2, kCLOCK_Pfd3, 19U);

    /* The ADC1 module of the RT1180 maximum operating clock frequency is 84MHz, in order to achieve the
       highest conversion rate, set the ADC1 operating clock to the configurable highest frequency 83.375MHz.
     */
    adc1ClkRoot.mux      = kCLOCK_ADC1_ClockRoot_MuxSysPll2Pfd3; /* Set clock source as SYS PLL2 pfd3 CLK. */
    adc1ClkRoot.div      = 6U;
    adc1ClkRoot.clockOff = false;
    CLOCK_SetRootClock(kCLOCK_Root_Adc1, &adc1ClkRoot);
}

/*${function:end}*/
