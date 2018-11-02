/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "peripherals.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "FRDM-KV31F"

/*! @brief The UART to use for debug messages. */
#define BOARD_USE_UART
#define BOARD_DEBUG_UART_TYPE DEBUG_CONSOLE_DEVICE_TYPE_UART
#define BOARD_DEBUG_UART_PERIPHERAL (uint32_t) UART0
#define BOARD_DEBUG_UART_INSTANCE 0U
#define BOARD_DEBUG_UART_CLKSRC kCLOCK_CoreSysClk
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetCoreSysClkFreq()
#define BOARD_UART_IRQ UART0_RX_TX_IRQn
#define BOARD_UART_IRQ_HANDLER UART0_RX_TX_IRQHandler

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */


#define LED_RED_ON() \
    GPIO_PortClear(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_PIN) /*!< Turn on target LED_RED */
#define LED_RED_OFF() \
    GPIO_PortSet(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_PIN) /*!< Turn off target LED_RED */
#define LED_RED_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_PIN) /*!< Toggle on target LED_RED */

#define LED_GREEN_ON() \
    GPIO_PortClear(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_PIN) /*!< Turn on target LED_GREEN */
#define LED_GREEN_OFF() \
    GPIO_PortSet(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_PIN) /*!< Turn off target LED_GREEN */
#define LED_GREEN_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_PIN) /*!< Toggle on target LED_GREEN */


#define LED_BLUE_ON() \
    GPIO_PortClear(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_PIN) /*!< Turn on target LED_BLUE */
#define LED_BLUE_OFF() \
    GPIO_PortSet(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_PIN) /*!< Turn off target LED_BLUE */
#define LED_BLUE_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_PIN) /*!< Toggle on target LED_BLUE */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

void BOARD_InitDebugConsole(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
