/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_CORE1_H_
#define _APP_CORE1_H_

/*${header:start}*/
#include "fsl_rgpio.h"
#include "fsl_phyrtl8211f.h"

volatile bool g_pinSet = false;
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define LED_INIT()                    \
    rgpio_pin_config_t led_config = { \
        kRGPIO_DigitalOutput,         \
        0,                            \
    };                                \
    RGPIO_PinInit(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, &led_config);

// #define LED_TOGGLE() GPIO_PortToggle(BOARD_USER_LED_GPIO, 1u << BOARD_USER_LED_GPIO_PIN);
#define LED_TOGGLE()                                                      \
    if (g_pinSet)                                                         \
    {                                                                     \
        RGPIO_PinWrite(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, 0U); \
        g_pinSet = false;                                                 \
    }                                                                     \
    else                                                                  \
    {                                                                     \
        RGPIO_PinWrite(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, 1U); \
        g_pinSet = true;                                                  \
    }

#define EXAMPLE_EP_RXBD_NUM          8U
#define EXAMPLE_EP_TXBD_NUM          8U
#define EXAMPLE_EP_BD_ALIGN          128U
#define EXAMPLE_EP_BUFF_SIZE_ALIGN   64U
#define EXAMPLE_EP_RXBUFF_SIZE       1518U
#define EXAMPLE_EP_RXBUFF_SIZE_ALIGN SDK_SIZEALIGN(EXAMPLE_EP_RXBUFF_SIZE, EXAMPLE_EP_BUFF_SIZE_ALIGN)
#define EXAMPLE_EP_TEST_FRAME_SIZE   1000U
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_CORE1_H_ */
