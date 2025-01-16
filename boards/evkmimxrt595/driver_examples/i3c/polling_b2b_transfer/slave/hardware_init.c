/*
 * Copyright 2019, 2024 NXP
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
    /* Attach 24MHz clock to I3C. */
    CLOCK_AttachClk(kFRO_DIV8_to_I3C_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivI3cClk, 1);

    /* Attach lposc_1m clock to I3C time control, clear halt for slow clock. */
    CLOCK_AttachClk(kLPOSC_to_I3C_TC_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivI3cTcClk, 1);
    CLOCK_SetClkDiv(kCLOCK_DivI3cSlowClk, 1);

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
