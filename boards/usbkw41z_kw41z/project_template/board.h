/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
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

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "USB-KW41Z"

/*! @brief The UART to use for debug messages. */
#define BOARD_USE_LPUART
#define BOARD_DEBUG_UART_TYPE DEBUG_CONSOLE_DEVICE_TYPE_LPUART
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART0
#define BOARD_DEBUG_UART_CLKSRC kCLOCK_Osc0ErClk
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetOsc0ErClkFreq()
#define BOARD_UART_IRQ LPUART0_IRQn
#define BOARD_UART_IRQ_HANDLER LPUART0_IRQHandler

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/*! @brief The i2c instance used for i2c connection by default */
#define BOARD_I2C_BASEADDR I2C1

/*! @brief The CMP instance used for board. */
#define BOARD_CMP_BASEADDR CMP0
#define BOARD_CMP_CHANNEL 3U

/*! @brief The rtc instance used for board. */
#define BOARD_RTC_FUNC_BASEADDR RTC

/* Board led color mapping */
#define LOGIC_LED_ON 0U
#define LOGIC_LED_OFF 1U
#define BOARD_LED_RED1_GPIO GPIOB
#define BOARD_LED_RED1_GPIO_PORT PORTB
#define BOARD_LED_RED1_GPIO_PIN 3U
#define BOARD_LED_RED2_GPIO GPIOB
#define BOARD_LED_RED2_GPIO_PORT PORTB
#define BOARD_LED_RED2_GPIO_PIN 18U

#define LED_RED1_INIT(output)                                  \
    GPIO_PinInit(BOARD_LED_RED1_GPIO, BOARD_LED_RED1_GPIO_PIN, \
                 &(gpio_pin_config_t){kGPIO_DigitalOutput, (output)}) /*!< Enable target LED_RED1 */
#define LED_RED1_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_RED1_GPIO, 1U << BOARD_LED_RED1_GPIO_PIN) /*!< Turn on target LED_RED1 */
#define LED_RED1_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_RED1_GPIO, 1U << BOARD_LED_RED1_GPIO_PIN) /*!< Turn off target LED_RED1 */
#define LED_RED1_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_RED1_GPIO, 1U << BOARD_LED_RED1_GPIO_PIN) /*!< Toggle on target LED_RED1 */

#define LED_RED2_INIT(output)                                  \
    GPIO_PinInit(BOARD_LED_RED2_GPIO, BOARD_LED_RED2_GPIO_PIN, \
                 &(gpio_pin_config_t){kGPIO_DigitalOutput, (output)}) /*!< Enable target LED_RED2 */
#define LED_RED2_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_RED2_GPIO, 1U << BOARD_LED_RED2_GPIO_PIN) /*!< Turn on target LED_RED2 */
#define LED_RED2_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_RED2_GPIO, 1U << BOARD_LED_RED2_GPIO_PIN) /*!< Turn off target LED_RED2 */
#define LED_RED2_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_RED2_GPIO, 1U << BOARD_LED_RED2_GPIO_PIN) /*!< Toggle on target LED_RED2 */

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
