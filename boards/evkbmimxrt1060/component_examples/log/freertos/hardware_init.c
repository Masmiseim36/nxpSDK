/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "app.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Enable clock gate for GPIO1 */
    CLOCK_EnableClock(kCLOCK_Gpio1);
}

void Led_Init(void)
{
    LED_1_INIT();
}
/*${function:end}*/
