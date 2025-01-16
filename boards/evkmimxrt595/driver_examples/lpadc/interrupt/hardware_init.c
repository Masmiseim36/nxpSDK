/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
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
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    SYSCTL0->PDRUNCFG0_CLR = SYSCTL0_PDRUNCFG0_ADC_PD_MASK;
    SYSCTL0->PDRUNCFG0_CLR = SYSCTL0_PDRUNCFG0_ADC_LP_MASK;
    RESET_PeripheralReset(kADC0_RST_SHIFT_RSTn);
    CLOCK_AttachClk(kFRO_DIV4_to_ADC_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivAdcClk, 1);
}
/*${function:end}*/
