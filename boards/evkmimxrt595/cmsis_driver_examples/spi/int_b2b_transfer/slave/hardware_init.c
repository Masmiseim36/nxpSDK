/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "fsl_inputmux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM5);

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

uint32_t SPI5_GetFreq(void)
{
    return CLOCK_GetFlexcommClkFreq(5U);
}
/*${function:end}*/
