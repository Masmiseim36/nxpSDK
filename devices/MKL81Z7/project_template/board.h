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
#define BOARD_NAME "TWR-KL81Z72M"

/*! @brief The UART to use for debug messages. */
#define BOARD_USE_LPUART
#define BOARD_DEBUG_UART_TYPE DEBUG_CONSOLE_DEVICE_TYPE_LPUART
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART1
#define BOARD_DEBUG_UART_INSTANCE 1U
#define BOARD_DEBUG_UART_CLKSRC kCLOCK_Osc0ErClk
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetOsc0ErClkFreq()
#define BOARD_UART_IRQ LPUART1_IRQn
#define BOARD_UART_IRQ_HANDLER LPUART1_IRQHandler

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/*! @brief The i2c instance used for i2c connection by default */
#define BOARD_I2C_BASEADDR I2C0

/*! @brief The TPM instance/channel used for board */
#define BOARD_TPM_BASEADDR TPM2
#define BOARD_TPM_CHANNEL 0U

#define BOARD_TSI_ELECTRODE_CNT 2U

/*! @brief Indexes of the TSI channels for on board electrodes */
#ifndef BOARD_TSI_ELECTRODE_1
#define BOARD_TSI_ELECTRODE_1 9U
#endif
#ifndef BOARD_TSI_ELECTRODE_2
#define BOARD_TSI_ELECTRODE_2 10U
#endif

/*! @brief The i2c instance used for board. */
#define BOARD_SAI_DEMO_I2C_BASEADDR I2C0

/*! @brief The rtc instance used for board. */
#define BOARD_RTC_FUNC_BASEADDR RTC

/* Board led color mapping */
#define LOGIC_LED_ON 0U
#define LOGIC_LED_OFF 1U
#ifndef BOARD_LED_RED_GPIO
#define BOARD_LED_RED_GPIO GPIOD
#endif
#define BOARD_LED_RED_GPIO_PORT PORTD
#ifndef BOARD_LED_RED_GPIO_PIN
#define BOARD_LED_RED_GPIO_PIN 11U
#endif
#ifndef BOARD_LED_GREEN_GPIO
#define BOARD_LED_GREEN_GPIO GPIOD
#endif
#define BOARD_LED_GREEN_GPIO_PORT PORTD
#ifndef BOARD_LED_GREEN_GPIO_PIN
#define BOARD_LED_GREEN_GPIO_PIN 12U
#endif
#ifndef BOARD_LED_BLUE_GPIO
#define BOARD_LED_BLUE_GPIO GPIOD
#endif
#define BOARD_LED_BLUE_GPIO_PORT PORTD
#ifndef BOARD_LED_BLUE_GPIO_PIN
#define BOARD_LED_BLUE_GPIO_PIN 13U
#endif

#define LED_RED_INIT(output)                                                 \
    GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, output); \
    BOARD_LED_RED_GPIO->PDDR |= (1U << BOARD_LED_RED_GPIO_PIN) /*!< Enable target LED_RED */
#define LED_RED_ON() \
    GPIO_PortClear(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Turn on target LED_RED */
#define LED_RED_OFF() \
    GPIO_PortSet(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Turn off target LED_RED */
#define LED_RED_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Toggle on target LED_RED */

#define LED_GREEN_INIT(output)                                                   \
    GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, output); \
    BOARD_LED_GREEN_GPIO->PDDR |= (1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Enable target LED_GREEN */
#define LED_GREEN_ON() \
    GPIO_PortClear(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn on target LED_GREEN */
#define LED_GREEN_OFF() \
    GPIO_PortSet(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn off target LED_GREEN */
#define LED_GREEN_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Toggle on target LED_GREEN */

#define LED_BLUE_INIT(output)                                                  \
    GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, output); \
    BOARD_LED_BLUE_GPIO->PDDR |= (1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Enable target LED_BLUE */
#define LED_BLUE_ON() \
    GPIO_PortClear(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn on target LED_BLUE */
#define LED_BLUE_OFF() \
    GPIO_PortSet(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn off target LED_BLUE */
#define LED_BLUE_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Toggle on target LED_BLUE */

/* @brief The EMVSIM SMARTCARD PHY configuration. */
#define BOARD_SMARTCARD_MODULE (EMVSIM1)          /*!< SMARTCARD communicational module instance */
#define BOARD_SMARTCARD_MODULE_IRQ (EMVSIM1_IRQn) /*!< SMARTCARD communicational module IRQ handler */
#define BOARD_SMARTCARD_CLOCK_MODULE (1u)         /*!< SMARTCARD clock generation module instance (EMVSIM1) */
#define BOARD_SMARTCARD_CLOCK_MODULE_CHANNEL (0u) /*!< SMARTCARD clock generation module channel */
#define BOARD_SMARTCARD_CLOCK_MODULE_SOURCE_CLK \
    (kCLOCK_PllFllSelClk) /*!< SMARTCARD clock module peripheral source clock (HSRUN = 72Mhz) */
#define BOARD_SMARTCARD_CLOCK_MODULE_CLK_FREQ CLOCK_GetFreq(kCLOCK_PllFllSelClk)
#define BOARD_SMARTCARD_CLOCK_VALUE (4000000u) /*!< SMARTCARD clock frequency */
#define BOARD_SMARTCARD_CONTROL_PORT (1u)      /*!< SMARTCARD control pin port instance number (PORTB) */
#define BOARD_SMARTCARD_CONTROL_PIN (6u)       /*!< SMARTCARD control pin number */
#ifndef BOARD_SMARTCARD_RST_PORT
#define BOARD_SMARTCARD_RST_PORT (1u) /*!< SMARTCARD reset pin port instance number (PORTB) */
#endif
#ifndef BOARD_SMARTCARD_RST_PIN
#define BOARD_SMARTCARD_RST_PIN (8u) /*!< SMARTCARD reset pin number */
#endif
#define BOARD_SMARTCARD_IRQ_PORT (0u)            /*!< SMARTCARD irq port pin instance number (PORTA) */
#define BOARD_SMARTCARD_IRQ_PIN (13u)            /*!< SMARTCARD irq pin number */
#define BOARD_SMARTCARD_IRQ_PIN_IRQ (PORTA_IRQn) /*!< SMARTCARD irq port handler */
#define BOARD_SMARTCARD_VSEL0_PORT (2u)          /*!< SMARTCARD Voltage selection pin0 port instance (PORTC) */
#define BOARD_SMARTCARD_VSEL0_PIN (9u)           /*!< SMARTCARD Voltage selection pin0 pin number */
#define BOARD_SMARTCARD_VSEL1_PORT (2u)          /*!< SMARTCARD Voltage selection pin1 port instance (PORTC) */
#define BOARD_SMARTCARD_VSEL1_PIN (2u)           /*!< SMARTCARD Voltage selection pin1 pin number */

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
