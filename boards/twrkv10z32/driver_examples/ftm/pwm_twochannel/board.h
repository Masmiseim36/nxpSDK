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
#define BOARD_NAME "TWR-KV10Z32"

/*! @brief The UART to use for debug messages. */
#define BOARD_USE_UART
#define BOARD_DEBUG_UART_TYPE DEBUG_CONSOLE_DEVICE_TYPE_UART
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) UART0
#define BOARD_DEBUG_UART_CLKSRC kCLOCK_CoreSysClk
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetCoreSysClkFreq()
#define BOARD_UART_IRQ UART0_IRQn
#define BOARD_UART_IRQ_HANDLER UART0_IRQHandler

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

#define BOARD_ADC_USE_ALT_VREF 1U

/*! @brief The i2c instance used for board. */
#define BOARD_I2C_COMM_BASEADDR I2C0

/*! @brief The i2c instance used for i2c connection by default */
#define BOARD_I2C_BASEADDR I2C0

/*! @brief ADC1 input channel for board. */
#define BOARD_ADC_HW_TRIGGER_DAC_BASEADDR DAC0
#define BOARD_ADC_HW_TRIGGER_ADC_BASEADDR ADC1
#define BOARD_ADC_HW_TRIGGER_CHANNEL 4U

#define BOARD_CAN_BASEADDR CAN0

/*! @brief The CMP instance/channel used for board. */
#define BOARD_CMP_BASEADDR CMP0
#define BOARD_CMP_CHANNEL 0U

/*! @brief Define the port interrupt number for the board switches */
#define BOARD_SW1_GPIO GPIOA
#define BOARD_SW1_PORT PORTA
#define BOARD_SW1_GPIO_PIN 4U
#define BOARD_SW1_IRQ PORTA_IRQn
#define BOARD_SW1_IRQ_HANDLER PORTA_IRQHandler
#define BOARD_SW1_NAME "SW1"

/* Board led color mapping */
#define LOGIC_LED_ON 0U
#define LOGIC_LED_OFF 1U
#define BOARD_LED_GREEN1_GPIO GPIOC
#define BOARD_LED_GREEN1_GPIO_PORT PORTC
#define BOARD_LED_GREEN1_GPIO_PIN 1U
#define BOARD_LED_YELLOW1_GPIO GPIOE
#define BOARD_LED_YELLOW1_GPIO_PORT PORTE
#define BOARD_LED_YELLOW1_GPIO_PIN 25U
#define BOARD_LED_GREEN2_GPIO GPIOC
#define BOARD_LED_GREEN2_GPIO_PORT PORTC
#define BOARD_LED_GREEN2_GPIO_PIN 3U
#define BOARD_LED_YELLOW2_GPIO GPIOC
#define BOARD_LED_YELLOW2_GPIO_PORT PORTC
#define BOARD_LED_YELLOW2_GPIO_PIN 4U
#define BOARD_LED_GREEN3_GPIO GPIOD
#define BOARD_LED_GREEN3_GPIO_PORT PORTD
#define BOARD_LED_GREEN3_GPIO_PIN 4U
#define BOARD_LED_YELLOW3_GPIO GPIOD
#define BOARD_LED_YELLOW3_GPIO_PORT PORTD
#define BOARD_LED_YELLOW3_GPIO_PIN 5U
#define BOARD_LED_ORANGE_GPIO GPIOD
#define BOARD_LED_ORANGE_GPIO_PORT PORTD
#define BOARD_LED_ORANGE_GPIO_PIN 6U
#define BOARD_LED_RED_GPIO GPIOD
#define BOARD_LED_RED_GPIO_PORT PORTD
#define BOARD_LED_RED_GPIO_PIN 7U

#define LED_GREEN1_INIT(output)                                    \
    GPIO_WritePinOutput(BOARD_LED_GREEN1_GPIO, BOARD_LED_GREEN1_GPIO_PIN, output);\
    BOARD_LED_GREEN1_GPIO->PDDR |= (1U << BOARD_LED_GREEN1_GPIO_PIN)  /*!< Enable target LED_GREEN1 */
#define LED_GREEN1_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_GREEN1_GPIO, 1U << BOARD_LED_GREEN1_GPIO_PIN) /*!< Turn on target LED_GREEN1 */
#define LED_GREEN1_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_GREEN1_GPIO, 1U << BOARD_LED_GREEN1_GPIO_PIN) /*!< Turn off target LED_GREEN1 */
#define LED_GREEN1_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_GREEN1_GPIO, 1U << BOARD_LED_GREEN1_GPIO_PIN) /*!< Toggle on target LED_GREEN1 */

#define LED_YELLOW1_INIT(output)                                     \
    GPIO_WritePinOutput(BOARD_LED_YELLOW1_GPIO, BOARD_LED_YELLOW1_GPIO_PIN, output);\
    BOARD_LED_YELLOW1_GPIO->PDDR |= (1U << BOARD_LED_YELLOW1_GPIO_PIN)  /*!< Enable target LED_YELLOW1 */
#define LED_YELLOW1_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_YELLOW1_GPIO, 1U << BOARD_LED_YELLOW1_GPIO_PIN) /*!< Turn on target LED_YELLOW1 */
#define LED_YELLOW1_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_YELLOW1_GPIO, 1U << BOARD_LED_YELLOW1_GPIO_PIN) /*!< Turn off target LED_YELLOW1 */
#define LED_YELLOW1_TOGGLE() GPIO_TogglePinsOutput(BOARD_LED_YELLOW1_GPIO, 1U << BOARD_LED_YELLOW1_GPIO_PIN)

#define LED_GREEN2_INIT(output)                                    \
    GPIO_WritePinOutput(BOARD_LED_GREEN2_GPIO, BOARD_LED_GREEN2_GPIO_PIN, output);\
    BOARD_LED_GREEN2_GPIO->PDDR |= (1U << BOARD_LED_GREEN2_GPIO_PIN)  /*!< Enable target LED_GREEN2 */
#define LED_GREEN2_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_GREEN2_GPIO, 1U << BOARD_LED_GREEN2_GPIO_PIN) /*!< Turn on target LED_GREEN2 */
#define LED_GREEN2_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_GREEN2_GPIO, 1U << BOARD_LED_GREEN2_GPIO_PIN) /*!< Turn off target LED_GREEN2 */
#define LED_GREEN2_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_GREEN2_GPIO, 1U << BOARD_LED_GREEN2_GPIO_PIN) /*!< Toggle on target LED_GREEN2 */

#define LED_YELLOW2_INIT(output)                                     \
    GPIO_WritePinOutput(BOARD_LED_YELLOW2_GPIO, BOARD_LED_YELLOW2_GPIO_PIN, output);\
    BOARD_LED_YELLOW2_GPIO->PDDR |= (1U << BOARD_LED_YELLOW2_GPIO_PIN)  /*!< Enable target LED_YELLOW2 */
#define LED_YELLOW2_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_YELLOW2_GPIO, 1U << BOARD_LED_YELLOW2_GPIO_PIN) /*!< Turn on target LED_YELLOW2 */
#define LED_YELLOW2_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_YELLOW2_GPIO, 1U << BOARD_LED_YELLOW2_GPIO_PIN) /*!< Turn off target LED_YELLOW2 */
#define LED_YELLOW2_TOGGLE() GPIO_TogglePinsOutput(BOARD_LED_YELLOW2_GPIO, 1U << BOARD_LED_YELLOW2_GPIO_PIN)

#define LED_GREEN3_INIT(output)                                    \
    GPIO_WritePinOutput(BOARD_LED_GREEN3_GPIO, BOARD_LED_GREEN3_GPIO_PIN, output);\
    BOARD_LED_GREEN3_GPIO->PDDR |= (1U << BOARD_LED_GREEN3_GPIO_PIN)  /*!< Enable target LED_GREEN3 */
#define LED_GREEN3_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_GREEN3_GPIO, 1U << BOARD_LED_GREEN3_GPIO_PIN) /*!< Turn on target LED_GREEN3 */
#define LED_GREEN3_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_GREEN3_GPIO, 1U << BOARD_LED_GREEN3_GPIO_PIN) /*!< Turn off target LED_GREEN3 */
#define LED_GREEN3_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_GREEN3_GPIO, 1U << BOARD_LED_GREEN3_GPIO_PIN) /*!< Toggle on target LED_GREEN3 */

#define LED_YELLOW3_INIT(output)                                     \
    GPIO_WritePinOutput(BOARD_LED_YELLOW3_GPIO, BOARD_LED_YELLOW3_GPIO_PIN, output);\
    BOARD_LED_YELLOW3_GPIO->PDDR |= (1U << BOARD_LED_YELLOW3_GPIO_PIN)  /*!< Enable target LED_YELLOW3 */
#define LED_YELLOW3_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_YELLOW3_GPIO, 1U << BOARD_LED_YELLOW3_GPIO_PIN) /*!< Turn on target LED_YELLOW3 */
#define LED_YELLOW3_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_YELLOW3_GPIO, 1U << BOARD_LED_YELLOW3_GPIO_PIN) /*!< Turn off target LED_YELLOW3 */
#define LED_YELLOW3_TOGGLE() GPIO_TogglePinsOutput(BOARD_LED_YELLOW3_GPIO, 1U << BOARD_LED_YELLOW3_GPIO_PIN)
#define LED_ORANGE_INIT(output)                                    \
    GPIO_WritePinOutput(BOARD_LED_ORANGE_GPIO, BOARD_LED_ORANGE_GPIO_PIN, output);\
    BOARD_LED_ORANGE_GPIO->PDDR |= (1U << BOARD_LED_ORANGE_GPIO_PIN)  /*!< Enable target LED_ORANGE */
#define LED_ORANGE_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_ORANGE_GPIO, 1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Turn on target LED_ORANGE */
#define LED_ORANGE_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_ORANGE_GPIO, 1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Turn off target LED_ORANGE */
#define LED_ORANGE_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_ORANGE_GPIO, 1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Toggle on target LED_ORANGE */

#define LED_RED_INIT(output)                                 \
    GPIO_WritePinOutput(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, output);\
    BOARD_LED_RED_GPIO->PDDR |= (1U << BOARD_LED_RED_GPIO_PIN)  /*!< Enable target LED_RED */
#define LED_RED_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Turn on target LED_RED */
#define LED_RED_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Turn off target LED_RED */
#define LED_RED_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Toggle on target LED_RED */

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
