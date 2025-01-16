/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Set LPSPI_PODF. */
    CLOCK_SetDiv(kCLOCK_LpspiDiv, kCLOCK_LpspiDivBy6);
    /* Set Lpspi clock source, PLL2_MAIN_CLK 528MHz. */
    CLOCK_SetMux(kCLOCK_LpspiMux, 2);

    /* Set LPI2C_CLK_PODF. */
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, kCLOCK_MiscDivBy2);
    /* Set Lpi2c clock source, OSC_CLK (24 MHz). */
    CLOCK_SetMux(kCLOCK_Lpi2cMux, 1);
}
/*${function:end}*/
