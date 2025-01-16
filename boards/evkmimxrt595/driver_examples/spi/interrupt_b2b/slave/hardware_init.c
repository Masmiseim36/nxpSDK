/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
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
#if USE_HS_SPI
    /* Use 48 MHz clock for the FLEXCOMM14 */
    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM14);
#else
    /* Use 48 MHz clock for the FLEXCOMM5 */
    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM5);
#endif

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
