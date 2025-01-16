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
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Clear reset */
    RESET_PeripheralReset(kRNG_RST_SHIFT_RSTn);
    /*Make sure casper ram buffer has power up*/
    POWER_DisablePD(kPDRUNCFG_PPD_CASPER_SRAM);
    POWER_ApplyPD();
}
/*${function:end}*/
