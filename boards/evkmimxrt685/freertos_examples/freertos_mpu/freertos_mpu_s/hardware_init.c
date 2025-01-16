/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    gpio_pin_config_t xLedConfig = {.pinDirection = kGPIO_DigitalOutput, .outputLogic = 1};

    /* Initialize GPIO for LEDs. */
    GPIO_PortInit(GPIO, LED_PORT);
    GPIO_PinInit(GPIO, LED_PORT, GREEN_LED_PIN, &(xLedConfig));
    GPIO_PinInit(GPIO, LED_PORT, BLUE_LED_PIN, &(xLedConfig));

    /* Set non-secure vector table */
    SCB_NS->VTOR = mainNONSECURE_APP_START_ADDRESS;

    /* attach main clock divide to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
