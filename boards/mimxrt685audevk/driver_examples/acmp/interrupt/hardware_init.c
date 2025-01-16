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
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Let acmp run on main clock with divider 2 (250Mhz). */
    CLOCK_AttachClk(kMAIN_CLK_to_ACMP_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivAcmpClk, 2);

    SYSCTL0->PDRUNCFG0_CLR = SYSCTL0_PDRUNCFG0_ACMP_PD_MASK;
    RESET_PeripheralReset(kACMP0_RST_SHIFT_RSTn);
    /* Make sure ACMP voltage reference available*/
    POWER_SetAnalogBuffer(true);
}
/*${function:end}*/
