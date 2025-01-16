/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Set non-secure vector table */
    SCB_NS->VTOR = mainNONSECURE_APP_START_ADDRESS;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
