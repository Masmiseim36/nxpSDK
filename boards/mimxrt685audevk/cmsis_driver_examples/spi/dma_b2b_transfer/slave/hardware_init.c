/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    CLOCK_AttachClk(kSFRO_to_FLEXCOMM5);
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

uint32_t SPI5_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(5U);
}
/*${function:end}*/
