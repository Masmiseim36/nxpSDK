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
#include "board_hifi4.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
}
void LED_INIT()
{
    CLOCK_EnableClock(kCLOCK_HsGpio0);
    RESET_PeripheralReset(kHSGPIO0_RST_SHIFT_RSTn);
    LED_RED_INIT(LOGIC_LED_OFF);
}
void LED_TOGGLE()
{
    LED_RED_TOGGLE();
}
/*${function:end}*/
