/*
 * Copyright 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
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
#define BOARD_NAME "TWR-KV46F150M"

/*! @brief The UART to use for debug messages. */
#define BOARD_USE_UART
#define BOARD_DEBUG_UART_TYPE kSerialPort_Uart
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) UART1
#define BOARD_DEBUG_UART_INSTANCE 1U
#define BOARD_DEBUG_UART_CLKSRC kCLOCK_FastPeriphClk
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetFastPeriphClkFreq()
#define BOARD_UART_IRQ UART1_RX_TX_IRQn
#define BOARD_UART_IRQ_HANDLER UART1_RX_TX_IRQHandler

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/*! @brief The i2c instance used for i2c connection by default */
#define BOARD_I2C_BASEADDR I2C

/*! @brief The dspi instance used for board. */
#define BOARD_DSPI_BASEADDR SPI

#define BOARD_CMP_BASEADDR CMP0
#define BOARD_CMP_CHANNEL 1U

/* PTC7 - CMP0_IN1. */
#define BOARD_CAN_BASEADDR CAN0

/*! @brief Define the port interrupt number for the board switches */
#ifndef BOARD_SW2_GPIO
#define BOARD_SW2_GPIO GPIOE
#endif
#ifndef BOARD_SW2_PORT
#define BOARD_SW2_PORT PORTE
#endif
#ifndef BOARD_SW2_GPIO_PIN
#define BOARD_SW2_GPIO_PIN 6U
#endif
#define BOARD_SW2_IRQ PORTE_IRQn
#define BOARD_SW2_IRQ_HANDLER PORTE_IRQHandler
#define BOARD_SW2_NAME "SW2"

/* Board led color mapping */
#define LOGIC_LED_ON 1U
#define LOGIC_LED_OFF 0U
#define BOARD_LED_GREEN1_GPIO GPIOD
#define BOARD_LED_GREEN1_GPIO_PORT PORTD
#define BOARD_LED_GREEN1_GPIO_PIN 0U
#define BOARD_LED_YELLOW1_GPIO GPIOD
#define BOARD_LED_YELLOW1_GPIO_PORT PORTD
#define BOARD_LED_YELLOW1_GPIO_PIN 1U
#define BOARD_LED_GREEN2_GPIO GPIOD
#define BOARD_LED_GREEN2_GPIO_PORT PORTD
#define BOARD_LED_GREEN2_GPIO_PIN 2U
#define BOARD_LED_YELLOW2_GPIO GPIOD
#define BOARD_LED_YELLOW2_GPIO_PORT PORTD
#define BOARD_LED_YELLOW2_GPIO_PIN 3U
#define BOARD_LED_GREEN3_GPIO GPIOD
#define BOARD_LED_GREEN3_GPIO_PORT PORTD
#define BOARD_LED_GREEN3_GPIO_PIN 4U
#define BOARD_LED_YELLOW3_GPIO GPIOD
#define BOARD_LED_YELLOW3_GPIO_PORT PORTD
#define BOARD_LED_YELLOW3_GPIO_PIN 5U
#define BOARD_LED_GREEN4_GPIO GPIOC
#define BOARD_LED_GREEN4_GPIO_PORT PORTC
#define BOARD_LED_GREEN4_GPIO_PIN 1U
#define BOARD_LED_YELLOW4_GPIO GPIOC
#define BOARD_LED_YELLOW4_GPIO_PORT PORTC
#define BOARD_LED_YELLOW4_GPIO_PIN 2U
#ifndef BOARD_LED_ORANGE_GPIO
#define BOARD_LED_ORANGE_GPIO GPIOB
#endif
#define BOARD_LED_ORANGE_GPIO_PORT PORTB
#ifndef BOARD_LED_ORANGE_GPIO_PIN
#define BOARD_LED_ORANGE_GPIO_PIN 23U
#endif

#define LED_GREEN1_INIT(output)                                              \
    GPIO_PinWrite(BOARD_LED_GREEN1_GPIO, BOARD_LED_GREEN1_GPIO_PIN, output); \
    BOARD_LED_GREEN1_GPIO->PDDR |= (1U << BOARD_LED_GREEN1_GPIO_PIN) /*!< Enable target LED_GREEN1 */
#define LED_GREEN1_OFF() \
    GPIO_PortClear(BOARD_LED_GREEN1_GPIO, 1U << BOARD_LED_GREEN1_GPIO_PIN) /*!< Turn off target LED_GREEN1 */
#define LED_GREEN1_ON() \
    GPIO_PortSet(BOARD_LED_GREEN1_GPIO, 1U << BOARD_LED_GREEN1_GPIO_PIN) /*!< Turn on target LED_GREEN1 */
#define LED_GREEN1_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_GREEN1_GPIO, 1U << BOARD_LED_GREEN1_GPIO_PIN) /*!< Toggle on target LED_GREEN1 */

#define LED_YELLOW1_INIT(output)                                               \
    GPIO_PinWrite(BOARD_LED_YELLOW1_GPIO, BOARD_LED_YELLOW1_GPIO_PIN, output); \
    BOARD_LED_YELLOW1_GPIO->PDDR |= (1U << BOARD_LED_YELLOW1_GPIO_PIN) /*!< Enable target LED_YELLOW1 */
#define LED_YELLOW1_OFF() \
    GPIO_PortClear(BOARD_LED_YELLOW1_GPIO, 1U << BOARD_LED_YELLOW1_GPIO_PIN) /*!< Turn off target LED_YELLOW1 */
#define LED_YELLOW1_ON() \
    GPIO_PortSet(BOARD_LED_YELLOW1_GPIO, 1U << BOARD_LED_YELLOW1_GPIO_PIN) /*!< Turn on target LED_YELLOW1 */
#define LED_YELLOW1_TOGGLE() GPIO_PortToggle(BOARD_LED_YELLOW1_GPIO, 1U << BOARD_LED_YELLOW1_GPIO_PIN)

#define LED_GREEN2_INIT(output)                                              \
    GPIO_PinWrite(BOARD_LED_GREEN2_GPIO, BOARD_LED_GREEN2_GPIO_PIN, output); \
    BOARD_LED_GREEN2_GPIO->PDDR |= (1U << BOARD_LED_GREEN2_GPIO_PIN) /*!< Enable target LED_GREEN2 */
#define LED_GREEN2_OFF() \
    GPIO_PortClear(BOARD_LED_GREEN2_GPIO, 1U << BOARD_LED_GREEN2_GPIO_PIN) /*!< Turn off target LED_GREEN2 */
#define LED_GREEN2_ON() \
    GPIO_PortSet(BOARD_LED_GREEN2_GPIO, 1U << BOARD_LED_GREEN2_GPIO_PIN) /*!< Turn on target LED_GREEN2 */
#define LED_GREEN2_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_GREEN2_GPIO, 1U << BOARD_LED_GREEN2_GPIO_PIN) /*!< Toggle on target LED_GREEN2 */

#define LED_YELLOW2_INIT(output)                                               \
    GPIO_PinWrite(BOARD_LED_YELLOW2_GPIO, BOARD_LED_YELLOW2_GPIO_PIN, output); \
    BOARD_LED_YELLOW2_GPIO->PDDR |= (1U << BOARD_LED_YELLOW2_GPIO_PIN) /*!< Enable target LED_YELLOW2 */
#define LED_YELLOW2_OFF() \
    GPIO_PortClear(BOARD_LED_YELLOW2_GPIO, 1U << BOARD_LED_YELLOW2_GPIO_PIN) /*!< Turn off target LED_YELLOW2 */
#define LED_YELLOW2_ON() \
    GPIO_PortSet(BOARD_LED_YELLOW2_GPIO, 1U << BOARD_LED_YELLOW2_GPIO_PIN) /*!< Turn on target LED_YELLOW2 */
#define LED_YELLOW2_TOGGLE() GPIO_PortToggle(BOARD_LED_YELLOW2_GPIO, 1U << BOARD_LED_YELLOW2_GPIO_PIN)

#define LED_GREEN3_INIT(output)                                              \
    GPIO_PinWrite(BOARD_LED_GREEN3_GPIO, BOARD_LED_GREEN3_GPIO_PIN, output); \
    BOARD_LED_GREEN3_GPIO->PDDR |= (1U << BOARD_LED_GREEN3_GPIO_PIN) /*!< Enable target LED_GREEN3 */
#define LED_GREEN3_OFF() \
    GPIO_PortClear(BOARD_LED_GREEN3_GPIO, 1U << BOARD_LED_GREEN3_GPIO_PIN) /*!< Turn off target LED_GREEN3 */
#define LED_GREEN3_ON() \
    GPIO_PortSet(BOARD_LED_GREEN3_GPIO, 1U << BOARD_LED_GREEN3_GPIO_PIN) /*!< Turn on target LED_GREEN3 */
#define LED_GREEN3_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_GREEN3_GPIO, 1U << BOARD_LED_GREEN3_GPIO_PIN) /*!< Toggle on target LED_GREEN3 */

#define LED_YELLOW3_INIT(output)                                               \
    GPIO_PinWrite(BOARD_LED_YELLOW3_GPIO, BOARD_LED_YELLOW3_GPIO_PIN, output); \
    BOARD_LED_YELLOW3_GPIO->PDDR |= (1U << BOARD_LED_YELLOW3_GPIO_PIN) /*!< Enable target LED_YELLOW3 */
#define LED_YELLOW3_OFF() \
    GPIO_PortClear(BOARD_LED_YELLOW3_GPIO, 1U << BOARD_LED_YELLOW3_GPIO_PIN) /*!< Turn off target LED_YELLOW3 */
#define LED_YELLOW3_ON() \
    GPIO_PortSet(BOARD_LED_YELLOW3_GPIO, 1U << BOARD_LED_YELLOW3_GPIO_PIN) /*!< Turn on target LED_YELLOW3 */
#define LED_YELLOW3_TOGGLE() GPIO_PortToggle(BOARD_LED_YELLOW3_GPIO, 1U << BOARD_LED_YELLOW3_GPIO_PIN)

#define LED_GREEN4_INIT(output)                                              \
    GPIO_PinWrite(BOARD_LED_GREEN4_GPIO, BOARD_LED_GREEN4_GPIO_PIN, output); \
    BOARD_LED_GREEN4_GPIO->PDDR |= (1U << BOARD_LED_GREEN4_GPIO_PIN) /*!< Enable target LED_GREEN4 */
#define LED_GREEN4_OFF() \
    GPIO_PortClear(BOARD_LED_GREEN4_GPIO, 1U << BOARD_LED_GREEN4_GPIO_PIN) /*!< Turn off target LED_GREEN4 */
#define LED_GREEN4_ON() \
    GPIO_PortSet(BOARD_LED_GREEN4_GPIO, 1U << BOARD_LED_GREEN4_GPIO_PIN) /*!< Turn on target LED_GREEN4 */
#define LED_GREEN4_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_GREEN4_GPIO, 1U << BOARD_LED_GREEN4_GPIO_PIN) /*!< Toggle on target LED_GREEN4 */

#define LED_YELLOW4_INIT(output)                                               \
    GPIO_PinWrite(BOARD_LED_YELLOW4_GPIO, BOARD_LED_YELLOW4_GPIO_PIN, output); \
    BOARD_LED_YELLOW4_GPIO->PDDR |= (1U << BOARD_LED_YELLOW4_GPIO_PIN) /*!< Enable target LED_YELLOW4 */
#define LED_YELLOW4_OFF() \
    GPIO_PortClear(BOARD_LED_YELLOW4_GPIO, 1U << BOARD_LED_YELLOW4_GPIO_PIN) /*!< Turn off target LED_YELLOW4 */
#define LED_YELLOW4_ON() \
    GPIO_PortSet(BOARD_LED_YELLOW4_GPIO, 1U << BOARD_LED_YELLOW4_GPIO_PIN) /*!< Turn on target LED_YELLOW4 */
#define LED_YELLOW4_TOGGLE() GPIO_PortToggle(BOARD_LED_YELLOW4_GPIO, 1U << BOARD_LED_YELLOW4_GPIO_PIN)

#define LED_ORANGE_INIT(output)                                              \
    GPIO_PinWrite(BOARD_LED_ORANGE_GPIO, BOARD_LED_ORANGE_GPIO_PIN, output); \
    BOARD_LED_ORANGE_GPIO->PDDR |= (1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Enable target LED_ORANGE */
#define LED_ORANGE_OFF() \
    GPIO_PortClear(BOARD_LED_ORANGE_GPIO, 1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Turn off target LED_ORANGE */
#define LED_ORANGE_ON() \
    GPIO_PortSet(BOARD_LED_ORANGE_GPIO, 1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Turn on target LED_ORANGE */
#define LED_ORANGE_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_ORANGE_GPIO, 1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Toggle on target LED_ORANGE */

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
