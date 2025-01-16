/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "board.h"
#include "camera_support.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitSDRAMPins();
    BOARD_EarlyPrepareCamera();
    BOARD_InitCSIPins();
    BOARD_InitLCDPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

/*${function:end}*/
