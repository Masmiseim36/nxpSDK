/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_InputMux);
    CLOCK_EnableClock(kCLOCK_HsGpio0);

    RESET_PeripheralReset(kHSGPIO0_RST_SHIFT_RSTn);

    /* DMIC uses 48MHz FRO clock */
    CLOCK_AttachClk(kFRO_DIV4_to_DMIC);
    /*48MHz divided by 60 = 800 KHz PDM clock --> gives 16kHz sample rate */
    CLOCK_SetClkDiv(kCLOCK_DivDmicClk, 60);
}
/*${function:end}*/
