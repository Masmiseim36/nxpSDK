/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
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
    /* Use 16 MHz clock for the FLEXCOMM4 */
    CLOCK_AttachClk(kSFRO_to_FLEXCOMM4);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

uint32_t I2C4_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(4U);
}

/*${function:end}*/
