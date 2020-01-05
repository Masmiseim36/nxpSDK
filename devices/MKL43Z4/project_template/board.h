/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "TWR-KL43Z48M"

/*! @brief The UART to use for debug messages. */
#define BOARD_USE_UART
#define BOARD_DEBUG_UART_TYPE DEBUG_CONSOLE_DEVICE_TYPE_UART
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) UART2
#define BOARD_DEBUG_UART_INSTANCE    2U
#define BOARD_DEBUG_UART_CLKSRC BUS_CLK
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetBusClkFreq()
#define BOARD_UART_IRQ UART2_FLEXIO_IRQn
#define BOARD_UART_IRQ_HANDLER UART2_FLEXIO_IRQHandler

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

#define BOARD_TPM_BASEADDR TPM1
#define BOARD_TPM_CHANNEL 1U

#define BOARD_CMP_BASEADDR CMP0
#define BOARD_CMP_CHANNEL 0U

/*! @brief The i2c instance used for i2c connection by default */
#define BOARD_I2C_BASEADDR I2C1

/*! @brief The spi instance used for board. */
#define BOARD_SPI_BASEADDR SPI0

/*! @brief The rtc instance used for board. */
#define BOARD_RTC_FUNC_BASEADDR RTC

/*! @brief The i2c instance used for board. */
#define BOARD_SAI_DEMO_I2C_BASEADDR I2C1

/* Board led color mapping */
#define LOGIC_LED_ON 0U
#define LOGIC_LED_OFF 1U
#define BOARD_LED_GREEN1_GPIO GPIOA
#define BOARD_LED_GREEN1_GPIO_PORT PORTA
#define BOARD_LED_GREEN1_GPIO_PIN 12U
#define BOARD_LED_RED1_GPIO GPIOA
#define BOARD_LED_RED1_GPIO_PORT PORTA
#define BOARD_LED_RED1_GPIO_PIN 13U
#define BOARD_LED_GREEN2_GPIO GPIOB
#define BOARD_LED_GREEN2_GPIO_PORT PORTB
#define BOARD_LED_GREEN2_GPIO_PIN 0U
#define BOARD_LED_RED2_GPIO GPIOB
#define BOARD_LED_RED2_GPIO_PORT PORTB
#define BOARD_LED_RED2_GPIO_PIN 19U

#define LED_RED1_INIT(output)                                  \
    GPIO_PinWrite(BOARD_LED_RED1_GPIO, BOARD_LED_RED1_GPIO_PIN, output);\
    BOARD_LED_RED1_GPIO->PDDR |= (1U << BOARD_LED_RED1_GPIO_PIN)  /*!< Enable target LED_RED1 */
#define LED_RED1_ON() \
    GPIO_PortClear(BOARD_LED_RED1_GPIO, 1U << BOARD_LED_RED1_GPIO_PIN) /*!< Turn on target LED_RED1 */
#define LED_RED1_OFF() \
    GPIO_PortSet(BOARD_LED_RED1_GPIO, 1U << BOARD_LED_RED1_GPIO_PIN) /*!< Turn off target LED_RED1 */
#define LED_RED1_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_RED1_GPIO, 1U << BOARD_LED_RED1_GPIO_PIN) /*!< Toggle on target LED_RED1 */

#define LED_GREEN1_INIT(output)                                    \
    GPIO_PinWrite(BOARD_LED_GREEN1_GPIO, BOARD_LED_GREEN1_GPIO_PIN, output);\
    BOARD_LED_GREEN1_GPIO->PDDR |= (1U << BOARD_LED_GREEN1_GPIO_PIN)  /*!< Enable target LED_GREEN1 */
#define LED_GREEN1_ON() \
    GPIO_PortClear(BOARD_LED_GREEN1_GPIO, 1U << BOARD_LED_GREEN1_GPIO_PIN) /*!< Turn on target LED_GREEN1 */
#define LED_GREEN1_OFF() \
    GPIO_PortSet(BOARD_LED_GREEN1_GPIO, 1U << BOARD_LED_GREEN1_GPIO_PIN) /*!< Turn off target LED_GREEN1 */
#define LED_GREEN1_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_GREEN1_GPIO, 1U << BOARD_LED_GREEN1_GPIO_PIN) /*!< Toggle on target LED_GREEN1 */

#define LED_RED2_INIT(output)                                  \
    GPIO_PinWrite(BOARD_LED_RED2_GPIO, BOARD_LED_RED2_GPIO_PIN, output);\
    BOARD_LED_RED2_GPIO->PDDR |= (1U << BOARD_LED_RED2_GPIO_PIN)  /*!< Enable target LED_RED2 */
#define LED_RED2_ON() \
    GPIO_PortClear(BOARD_LED_RED2_GPIO, 1U << BOARD_LED_RED2_GPIO_PIN) /*!< Turn on target LED_RED2 */
#define LED_RED2_OFF() \
    GPIO_PortSet(BOARD_LED_RED2_GPIO, 1U << BOARD_LED_RED2_GPIO_PIN) /*!< Turn off target LED_RED2 */
#define LED_RED2_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_RED2_GPIO, 1U << BOARD_LED_RED2_GPIO_PIN) /*!< Toggle on target LED_RED2 */

#define LED_GREEN2_INIT(output)                                    \
    GPIO_PinWrite(BOARD_LED_GREEN2_GPIO, BOARD_LED_GREEN2_GPIO_PIN, output);\
    BOARD_LED_GREEN2_GPIO->PDDR |= (1U << BOARD_LED_GREEN2_GPIO_PIN)  /*!< Enable target LED_GREEN2 */
#define LED_GREEN2_ON() \
    GPIO_PortClear(BOARD_LED_GREEN2_GPIO, 1U << BOARD_LED_GREEN2_GPIO_PIN) /*!< Turn on target LED_GREEN2 */
#define LED_GREEN2_OFF() \
    GPIO_PortSet(BOARD_LED_GREEN2_GPIO, 1U << BOARD_LED_GREEN2_GPIO_PIN) /*!< Turn off target LED_GREEN2 */
#define LED_GREEN2_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_GREEN2_GPIO, 1U << BOARD_LED_GREEN2_GPIO_PIN) /*!< Toggle on target LED_GREEN2 */

#define BOARD_ACCEL_I2C_BASEADDR I2C1

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
