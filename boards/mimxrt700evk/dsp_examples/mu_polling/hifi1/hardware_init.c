/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${macro:start}*/
/*${macro:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    CLOCK_SetXtalFreq(BOARD_XTAL_SYS_CLK_HZ); /* Note: need tell clock driver the frequency of OSC. */
    BOARD_InitBootPins();
    RESET_ClearPeripheralReset(kGPIO8_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_Gpio8);
}
void LED_INIT()
{
    LED_RED_INIT(LOGIC_LED_OFF);
}
void LED_TOGGLE()
{
    LED_RED_TOGGLE();
}
/*${function:end}*/
