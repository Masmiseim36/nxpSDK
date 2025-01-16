/*
 * Copyright 2019 NXP
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
    CLOCK_EnableClock(kCLOCK_HsGpio0);
    CLOCK_EnableClock(kCLOCK_HsGpio1);
    CLOCK_EnableClock(kCLOCK_HsGpio3);
    RESET_PeripheralReset(kHSGPIO0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kHSGPIO1_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kHSGPIO3_RST_SHIFT_RSTn);

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

void Led_Init(void)
{
    LED_1_INIT();
    LED_2_INIT();
    LED_3_INIT();
}
/*${function:end}*/
