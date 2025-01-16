/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    /* Clock setting for Flexio */
    CLOCK_SetMux(kCLOCK_Flexio2Mux, FLEXIO_CLOCK_SELECT);
    CLOCK_SetDiv(kCLOCK_Flexio2PreDiv, FLEXIO_CLOCK_PRE_DIVIDER);
    CLOCK_SetDiv(kCLOCK_Flexio2Div, FLEXIO_CLOCK_DIVIDER);
}
/*${function:end}*/
