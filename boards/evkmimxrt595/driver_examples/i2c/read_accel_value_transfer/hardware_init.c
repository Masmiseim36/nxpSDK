/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "app.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_gpio.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Use 48 MHz clock for the FLEXCOMM4 */
    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM4);

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
