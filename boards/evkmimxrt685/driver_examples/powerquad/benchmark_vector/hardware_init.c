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
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Power up PQ RAM. */
    SYSCTL0->PDRUNCFG1_CLR = SYSCTL0_PDRUNCFG1_PQ_SRAM_APD_MASK | SYSCTL0_PDRUNCFG1_PQ_SRAM_PPD_MASK;
    /* Apply power setting. */
    POWER_ApplyPD();
}
/*${function:end}*/
