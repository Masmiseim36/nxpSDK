/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();

    /* Clock was initialized in mpi_loader_extram_loader, just need to set global variables here. */
    POWER_UpdateOscSettlingTime(BOARD_SYSOSC_SETTLING_US); /* Updated XTAL oscillator settling time */
    CLOCK_SetXtalFreq(BOARD_XTAL_SYS_CLK_HZ);              /* Sets external XTAL OSC freq */
    SystemCoreClock = BOARD_BOOTCLOCKRUN_CORE_CLOCK;

    BOARD_InitDebugConsole();
}
/*${function:end}*/
