/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
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
#define BOARD_NAME "TWR-KL28Z72M"

/*! @brief The UART to use for debug messages. */
#define BOARD_USE_UART
#define BOARD_DEBUG_UART_TYPE kSerialPort_Uart
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART0
#define BOARD_DEBUG_UART_INSTANCE 0U
#define BOARD_DEBUG_UART_CLKSRC kCLOCK_ScgSysOscAsyncDiv3Clk
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetIpFreq(kCLOCK_Lpuart0)
#define BOARD_UART_IRQ LPUART0_IRQn
#define BOARD_UART_IRQ_HANDLER LPUART0_IRQHandler

/* @Brief Board accelerator sensor configuration */
#define BOARD_ACCEL_I2C_BASEADDR LPI2C1
#define BOARD_ACCEL_I2C_CLOCK_FREQ (CLOCK_GetIpFreq(kCLOCK_Lpi2c1))
#define BOARD_DEBUG_UART_PCC_ADDRESS kCLOCK_Lpuart0

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/*! @brief The tsi instance used for board. */
#define BOARD_TSI_ELECTRODE_CNT 2
#ifndef BOARD_TSI_ELECTRODE_1
#define BOARD_TSI_ELECTRODE_1 15
#endif
#ifndef BOARD_TSI_ELECTRODE_2
#define BOARD_TSI_ELECTRODE_2 13
#endif

/* @brief The EMVSIM SMARTCARD interface. */
#define BOARD_SMARTCARD_MODULE (EMVSIM0)          /*!< SMARTCARD communicational module instance */
#define BOARD_SMARTCARD_MODULE_IRQ (EMVSIM0_IRQn) /*!< SMARTCARD communicational module IRQ handler */
#define BOARD_SMARTCARD_CLOCK_MODULE (0U)         /*!< SMARTCARD clock generation module instance (EMVSIM0) */
#define BOARD_SMARTCARD_CLOCK_MODULE_CHANNEL (0U) /*!< SMARTCARD clock generation module channel */
#define BOARD_SMARTCARD_CLOCK_MODULE_SOURCE_CLK \
    (kCLOCK_ScgFircAsyncDiv3Clk) /*!< SMARTCARD clock module peripheral source clock */
#define BOARD_SMARTCARD_CLOCK_MODULE_CLK_FREQ CLOCK_GetFreq(kCLOCK_ScgFircAsyncDiv3Clk)
#define BOARD_SMARTCARD_CLOCK_VALUE (4000000U) /*!< SMARTCARD clock frequency */
#define BOARD_SMARTCARD_CONTROL_PORT (2U)      /*!< SMARTCARD control pin port instance number (PORTC) */
#define BOARD_SMARTCARD_CONTROL_PIN (16U)      /*!< SMARTCARD control pin number */
#ifndef BOARD_SMARTCARD_RST_PORT
#define BOARD_SMARTCARD_RST_PORT (2U) /*!< SMARTCARD reset pin port instance number (PORTC) */
#endif
#ifndef BOARD_SMARTCARD_RST_PIN
#define BOARD_SMARTCARD_RST_PIN (15U) /*!< SMARTCARD reset pin number */
#endif
#define BOARD_SMARTCARD_IRQ_PORT (2U)            /*!< SMARTCARD irq port pin instance number (PORTC) */
#define BOARD_SMARTCARD_IRQ_PIN (4U)             /*!< SMARTCARD irq pin number */
#define BOARD_SMARTCARD_IRQ_PIN_IRQ (PORTC_IRQn) /*!< SMARTCARD irq port handler */
#define BOARD_SMARTCARD_VSEL0_PORT (4U)          /*!< SMARTCARD Voltage selection pin0 port instance (PORTE) */
#define BOARD_SMARTCARD_VSEL0_PIN (19U)          /*!< SMARTCARD Voltage selection pin0 pin number */
#define BOARD_SMARTCARD_VSEL1_PORT (4U)          /*!< SMARTCARD Voltage selection pin1 port instance (PORTE) */
#define BOARD_SMARTCARD_VSEL1_PIN (18U)          /*!< SMARTCARD Voltage selection pin1 pin number */

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

#ifndef BOARD_SW3_GPIO
#define BOARD_SW3_GPIO GPIOA
#endif
#ifndef BOARD_SW3_PORT
#define BOARD_SW3_PORT PORTA
#endif
#ifndef BOARD_SW3_GPIO_PIN
#define BOARD_SW3_GPIO_PIN 4U
#endif
#define BOARD_SW3_IRQ PORTA_IRQn
#define BOARD_SW3_IRQ_HANDLER PORTA_IRQHandler
#define BOARD_SW3_NAME "SW3"

/* Board led color mapping */
#define LOGIC_LED_ON 0U
#define LOGIC_LED_OFF 1U
#ifndef BOARD_LED_RED_GPIO
#define BOARD_LED_RED_GPIO GPIOA
#endif
#define BOARD_LED_RED_GPIO_PORT PORTA
#ifndef BOARD_LED_RED_GPIO_PIN
#define BOARD_LED_RED_GPIO_PIN 16U
#endif
#ifndef BOARD_LED_GREEN_GPIO
#define BOARD_LED_GREEN_GPIO GPIOA
#endif
#define BOARD_LED_GREEN_GPIO_PORT PORTA
#ifndef BOARD_LED_GREEN_GPIO_PIN
#define BOARD_LED_GREEN_GPIO_PIN 14U
#endif
#ifndef BOARD_LED_ORANGE_GPIO
#define BOARD_LED_ORANGE_GPIO GPIOC
#endif
#define BOARD_LED_ORANGE_GPIO_PORT PORTC
#ifndef BOARD_LED_ORANGE_GPIO_PIN
#define BOARD_LED_ORANGE_GPIO_PIN 17U
#endif
#ifndef BOARD_LED_YELLOW_GPIO
#define BOARD_LED_YELLOW_GPIO GPIOC
#endif
#define BOARD_LED_YELLOW_GPIO_PORT PORTC
#ifndef BOARD_LED_YELLOW_GPIO_PIN
#define BOARD_LED_YELLOW_GPIO_PIN 18U
#endif

#define LED_RED_INIT(output)                                           \
    GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, output); \
    BOARD_LED_RED_GPIO->PDDR |= (1U << BOARD_LED_RED_GPIO_PIN)                        /*!< Enable target LED_RED */
#define LED_RED_ON() GPIO_PortClear(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Turn on target LED_RED */
#define LED_RED_OFF() GPIO_PortSet(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN)  /*!< Turn off target LED_RED */
#define LED_RED_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Toggle on target LED_RED */

#define LED_GREEN_INIT(output)                                             \
    GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, output); \
    BOARD_LED_GREEN_GPIO->PDDR |= (1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Enable target LED_GREEN */
#define LED_GREEN_ON() \
    GPIO_PortClear(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn on target LED_GREEN */
#define LED_GREEN_OFF() \
    GPIO_PortSet(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn off target LED_GREEN */
#define LED_GREEN_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Toggle on target LED_GREEN */

#define LED_ORANGE_INIT(output)                                              \
    GPIO_PinWrite(BOARD_LED_ORANGE_GPIO, BOARD_LED_ORANGE_GPIO_PIN, output); \
    BOARD_LED_ORANGE_GPIO->PDDR |= (1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Enable target LED_ORANGE */
#define LED_ORANGE_ON() \
    GPIO_PortClear(BOARD_LED_ORANGE_GPIO, 1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Turn on target LED_ORANGE */
#define LED_ORANGE_OFF() \
    GPIO_PortSet(BOARD_LED_ORANGE_GPIO, 1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Turn off target LED_ORANGE */
#define LED_ORANGE_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_ORANGE_GPIO, 1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Toggle on target LED_ORANGE */

#define LED_YELLOW_INIT(output)                                              \
    GPIO_PinWrite(BOARD_LED_YELLOW_GPIO, BOARD_LED_YELLOW_GPIO_PIN, output); \
    BOARD_LED_YELLOW_GPIO->PDDR |= (1U << BOARD_LED_YELLOW_GPIO_PIN) /*!< Enable target LED_YELLOW */
#define LED_YELLOW_ON() \
    GPIO_PortClear(BOARD_LED_YELLOW_GPIO, 1U << BOARD_LED_YELLOW_GPIO_PIN) /*!< Turn on target LED_YELLOW */
#define LED_YELLOW_OFF() \
    GPIO_PortSet(BOARD_LED_YELLOW_GPIO, 1U << BOARD_LED_YELLOW_GPIO_PIN) /*!< Turn off target LED_YELLOW */
#define LED_YELLOW_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_YELLOW_GPIO, 1U << BOARD_LED_YELLOW_GPIO_PIN) /*!< Toggle on target LED_YELLOW */

#define BOARD_CODEC_I2C_BASEADDR LPI2C0
#define BOARD_CODEC_I2C_CLOCK_FREQ CLOCK_GetFreq(kCLOCK_ScgFircClk)

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

void BOARD_InitDebugConsole(void);
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
void BOARD_LPI2C_Init(LPI2C_Type *base, uint32_t clkSrc_Hz);
status_t BOARD_LPI2C_Send(LPI2C_Type *base,
                          uint8_t deviceAddress,
                          uint32_t subAddress,
                          uint8_t subaddressSize,
                          uint8_t *txBuff,
                          uint8_t txBuffSize);
status_t BOARD_LPI2C_Receive(LPI2C_Type *base,
                             uint8_t deviceAddress,
                             uint32_t subAddress,
                             uint8_t subaddressSize,
                             uint8_t *rxBuff,
                             uint8_t rxBuffSize);
void BOARD_Accel_I2C_Init(void);
status_t BOARD_Accel_I2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint32_t txBuff);
status_t BOARD_Accel_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
void BOARD_Codec_I2C_Init(void);
status_t BOARD_Codec_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_Codec_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
#endif /* SDK_I2C_BASED_COMPONENT_USED */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
