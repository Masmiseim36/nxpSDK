/*
 * Copyright 2023 NXP
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
void APP_InitCore1Domain(void)
{
}

uint8_t APP_GetCore1DomainID(void)
{
    return 5U;
}

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    RESET_ClearPeripheralReset(kMU1_RST_SHIFT_RSTn);
}
/*${function:end}*/
