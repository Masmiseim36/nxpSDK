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
#include "fsl_reset.h"
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Let acmp run on main clock with divider 2 (198Mhz). */
    CLOCK_AttachClk(kMAIN_CLK_to_ACMP_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivAcmpClk, 2);

    POWER_DisablePD(kPDRUNCFG_PD_ACMP);
    RESET_PeripheralReset(kACMP0_RST_SHIFT_RSTn);
    /* Make sure ACMP voltage reference available*/
    POWER_SetAnalogBuffer(true);
}
/*${function:end}*/
