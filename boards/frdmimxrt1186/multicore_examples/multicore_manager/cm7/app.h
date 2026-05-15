/*
 * Copyright 2019-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_CORE1_H_
#define _APP_CORE1_H_

/*${header:start}*/
#include "fsl_rgpio.h"
rgpio_pin_config_t led_config = {
    kRGPIO_DigitalOutput,
    0,
};
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define LED_INIT()   RGPIO_PinInit(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, &led_config);
#define LED_TOGGLE() RGPIO_PortToggle(BOARD_USER_LED_GPIO, 1u << BOARD_USER_LED_GPIO_PIN);
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_CORE1_H_ */
