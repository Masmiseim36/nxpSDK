/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief The board name */
#define BOARD_NAME "TWR-K24F120M"

/*! @brief The UART to use for debug messages. */
#define BOARD_USE_UART
#define BOARD_DEBUG_UART_TYPE DEBUG_CONSOLE_DEVICE_TYPE_UART
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) UART1
#define BOARD_DEBUG_UART_INSTANCE 1U
#define BOARD_DEBUG_UART_CLKSRC kCLOCK_CoreSysClk
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetCoreSysClkFreq()
#define BOARD_UART_IRQ UART1_RX_TX_IRQn
#define BOARD_UART_IRQ_HANDLER UART1_RX_TX_IRQHandler

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/*! @brief The i2c instance used for i2c connection by default */
#define BOARD_I2C_BASEADDR I2C0

/*! @brief The CMP instance/channel used for board. */
#define BOARD_CMP_BASEADDR CMP0
#define BOARD_CMP_CHANNEL 0U

/*! @brief The rtc instance used for board. */
#define BOARD_RTC_FUNC_BASEADDR RTC

/*! @brief The i2c instance used for sai demo */
#define BOARD_SAI_DEMO_I2C_BASEADDR I2C0

/*! @brief If connected the TWR_MEM, this is spi sd card */
#define SDCARD_CARD_WRITE_PROTECTION_GPIO GPIOA
#define SDCARD_CARD_WRITE_PROTECTION_GPIO_PORT PORTA
#define SDCARD_CARD_WRITE_PROTECTION_GPIO_PIN 17U
#define SDCARD_SPI_HW_BASEADDR SPI1
#define SDCARD_CD_GPIO_IRQ_HANDLER PORTB_IRQHandler

/*! @brief Define the port interrupt number for the board switches */
#ifndef BOARD_SW1_GPIO
#define BOARD_SW1_GPIO GPIOC
#endif
#ifndef BOARD_SW1_PORT
#define BOARD_SW1_PORT PORTC
#endif
#ifndef BOARD_SW1_GPIO_PIN
#define BOARD_SW1_GPIO_PIN 7U
#endif
#define BOARD_SW1_IRQ PORTC_IRQn
#define BOARD_SW1_IRQ_HANDLER PORTC_IRQHandler
#define BOARD_SW1_NAME "SW1"

/* Board led color mapping */
#define LOGIC_LED_ON 0U
#define LOGIC_LED_OFF 1U
#ifndef BOARD_LED_RED_GPIO
#define BOARD_LED_RED_GPIO GPIOD
#endif
#define BOARD_LED_RED_GPIO_PORT PORTD
#ifndef BOARD_LED_RED_GPIO_PIN
#define BOARD_LED_RED_GPIO_PIN 5U
#endif
#ifndef BOARD_LED_GREEN_GPIO
#define BOARD_LED_GREEN_GPIO GPIOD
#endif
#define BOARD_LED_GREEN_GPIO_PORT PORTD
#ifndef BOARD_LED_GREEN_GPIO_PIN
#define BOARD_LED_GREEN_GPIO_PIN 7U
#endif
#ifndef BOARD_LED_ORANGE_GPIO
#define BOARD_LED_ORANGE_GPIO GPIOD
#endif
#define BOARD_LED_ORANGE_GPIO_PORT PORTD
#ifndef BOARD_LED_ORANGE_GPIO_PIN
#define BOARD_LED_ORANGE_GPIO_PIN 6U
#endif
#ifndef BOARD_LED_YELLOW_GPIO
#define BOARD_LED_YELLOW_GPIO GPIOD
#endif
#define BOARD_LED_YELLOW_GPIO_PORT PORTD
#ifndef BOARD_LED_YELLOW_GPIO_PIN
#define BOARD_LED_YELLOW_GPIO_PIN 4U
#endif

#define LED_GREEN_INIT(output)                                             \
    GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, output); \
    BOARD_LED_GREEN_GPIO->PDDR |= (1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Enable target LED_GREEN */
#define LED_GREEN_ON() \
    GPIO_PortClear(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn on target LED_GREEN */
#define LED_GREEN_OFF() \
    GPIO_PortSet(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn off target LED_GREEN */
#define LED_GREEN_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Toggle on target LED_GREEN */

#define LED_RED_INIT(output)                                           \
    GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, output); \
    BOARD_LED_RED_GPIO->PDDR |= (1U << BOARD_LED_RED_GPIO_PIN)                        /*!< Enable target LED_RED */
#define LED_RED_ON() GPIO_PortClear(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Turn on target LED_RED */
#define LED_RED_OFF() GPIO_PortSet(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN)  /*!< Turn off target LED_RED */
#define LED_RED_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Toggle on target LED_RED */

#define LED_YELLOW_INIT(output)                                              \
    GPIO_PinWrite(BOARD_LED_YELLOW_GPIO, BOARD_LED_YELLOW_GPIO_PIN, output); \
    BOARD_LED_YELLOW_GPIO->PDDR |= (1U << BOARD_LED_YELLOW_GPIO_PIN) /*!< Enable target LED_YELLOW */
#define LED_YELLOW_ON() \
    GPIO_PortClear(BOARD_LED_YELLOW_GPIO, 1U << BOARD_LED_YELLOW_GPIO_PIN) /*!< Turn on target LED_YELLOW */
#define LED_YELLOW_OFF() \
    GPIO_PortSet(BOARD_LED_YELLOW_GPIO, 1U << BOARD_LED_YELLOW_GPIO_PIN) /*!< Turn off target LED_YELLOW */
#define LED_YELLOW_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_YELLOW_GPIO, 1U << BOARD_LED_YELLOW_GPIO_PIN) /*!< Toggle on target LED_YELLOW */

#define LED_ORANGE_INIT(output)                                              \
    GPIO_PinWrite(BOARD_LED_ORANGE_GPIO, BOARD_LED_ORANGE_GPIO_PIN, output); \
    BOARD_LED_ORANGE_GPIO->PDDR |= (1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Enable target LED_ORANGE */
#define LED_ORANGE_ON() \
    GPIO_PortClear(BOARD_LED_ORANGE_GPIO, 1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Turn on target LED_ORANGE */
#define LED_ORANGE_OFF() \
    GPIO_PortSet(BOARD_LED_ORANGE_GPIO, 1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Turn off target LED_ORANGE */
#define LED_ORANGE_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_ORANGE_GPIO, 1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Toggle on target LED_ORANGE */

/* @brief The SDSPI disk PHY configuration. */
#define BOARD_SDSPI_SPI_BASE SPI1_BASE /*!< SPI base address for SDSPI */
#define BOARD_SDSPI_CD_GPIO_BASE GPIOE /*!< Port related to card detect pin for SDSPI */
#ifndef BOARD_SDSPI_CD_PIN
#define BOARD_SDSPI_CD_PIN 6U /*!< Card detect pin for SDSPI */
#endif

#define BOARD_ACCEL_I2C_BASEADDR I2C0
/* clang-format on */
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
