/*
 * Copyright 2019 NXP
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
    /* Use MAIN clock for the Ctimer2 */
    CLOCK_AttachClk(kMAIN_CLK_to_CTIMER2);

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
