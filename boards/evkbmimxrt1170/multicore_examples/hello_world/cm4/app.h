/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_CORE1_H_
#define _APP_CORE1_H_

/*${header:start}*/
#include "fsl_gpio.h"
volatile bool g_pinSet = false;
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define LED_INIT()                   \
    gpio_pin_config_t led_config = { \
        kGPIO_DigitalOutput,         \
        0,                           \
    };                               \
    GPIO_PinInit(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, &led_config);

// #define LED_TOGGLE() GPIO_PortToggle(BOARD_USER_LED_GPIO, 1u << BOARD_USER_LED_GPIO_PIN);
#define LED_TOGGLE()                                                     \
    if (g_pinSet)                                                        \
    {                                                                    \
        GPIO_PinWrite(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, 0U); \
        g_pinSet = false;                                                \
    }                                                                    \
    else                                                                 \
    {                                                                    \
        GPIO_PinWrite(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, 1U); \
        g_pinSet = true;                                                 \
    }
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_CORE1_H_ */
