/*
 * Copyright 2024 NXP
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
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();
    
    CLOCK_EnableClock(kCLOCK_Gpio0);
    RESET_ClearPeripheralReset(kGPIO0_RST_SHIFT_RSTn);
}

void Led_Init(void)
{
    LED_1_INIT();
    LED_2_INIT();
}
/*${function:end}*/
