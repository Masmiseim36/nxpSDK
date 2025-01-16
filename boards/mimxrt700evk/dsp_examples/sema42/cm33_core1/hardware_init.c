/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    BOARD_InitAHBSC();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    /* Clear SEMA42 reset */
    RESET_PeripheralReset(kSEMA423_RST_SHIFT_RSTn);
    /* Clear MUA reset */
    RESET_PeripheralReset(kMU3_RST_SHIFT_RSTn);
}
uint8_t APP_GetMCoreDomainID(void)
{
    return 5U;
}
void LED_INIT()
{
    RESET_ClearPeripheralReset(kGPIO8_RST_SHIFT_RSTn);
    CLOCK_EnableClock(kCLOCK_Gpio8);
    LED_RED_INIT(LOGIC_LED_ON);
    /* Enable GPIO access from DSP side */
    GPIO_EnablePinControlNonSecure(BOARD_LED_RED_GPIO, 1 << BOARD_LED_RED_GPIO_PIN);
    GPIO_EnablePinControlNonPrivilege(BOARD_LED_RED_GPIO, 1 << BOARD_LED_RED_GPIO_PIN);
}

/*${function:end}*/
