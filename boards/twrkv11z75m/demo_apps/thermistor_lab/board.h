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
#define BOARD_NAME "TWR-KV11Z75M"

/*! @brief The UART to use for debug messages. */
#define BOARD_USE_UART
#define BOARD_DEBUG_UART_TYPE kSerialPort_Uart
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) UART0
#define BOARD_DEBUG_UART_INSTANCE 0U
#define BOARD_DEBUG_UART_CLKSRC SYS_CLK
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetCoreSysClkFreq()
#define BOARD_UART_IRQ UART0_IRQn
#define BOARD_UART_IRQ_HANDLER UART0_IRQHandler

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/*! @brief The instances of peripherals used for dac_adc_demo */
#define BOARD_DAC_DEMO_DAC_BASEADDR DAC0
#define BOARD_DAC_DEMO_ADC_BASEADDR ADC1
#define BOARD_DAC_DEMO_ADC_CHANNEL 4U
#define BOARD_ADC_USE_ALT_VREF 1U

/*! @brief The i2c instance used for board. */
#define BOARD_I2C_COMM_BASEADDR I2C0

/*! @brief The i2c instance used for i2c connection by default */
#define BOARD_I2C_BASEADDR I2C0

/*! @brief The bubble level demo information */
#define BOARD_FXOS8700_ADDR 0x1C
#define BOARD_ACCEL_ADDR BOARD_FXOS8700_ADDR
#define BOARD_ACCEL_BAUDRATE 100
#define BOARD_ACCEL_I2C_BASEADDR I2C0
#define BOARD_ACCEL_I2C_CLK_SRC I2C0_CLK_SRC
#define BOARD_ACCEL_I2C_CLOCK_FREQ CLOCK_GetFreq(I2C0_CLK_SRC)

/*! @brief ADC1 input channel for board. */
#define BOARD_ADC_HW_TRIGGER_DAC_BASEADDR DAC0
#define BOARD_ADC_HW_TRIGGER_ADC_BASEADDR ADC1
#define BOARD_ADC_HW_TRIGGER_CHANNEL 4U

#define BOARD_CAN_BASEADDR CAN0

/*! @brief The dspi instance used for board. */
#define BOARD_DSPI_BASEADDR SPI0

/*! @brief The CMP instance used for board. */
#define BOARD_CMP_BASEADDR CMP0

/*! @brief The CMP channel used for board. */
#define BOARD_CMP_CHANNEL 0U

/*! @brief Define the port interrupt number for the board switches */
#ifndef BOARD_SW2_GPIO
#define BOARD_SW2_GPIO GPIOA
#endif
#ifndef BOARD_SW2_PORT
#define BOARD_SW2_PORT PORTA
#endif
#ifndef BOARD_SW2_GPIO_PIN
#define BOARD_SW2_GPIO_PIN 4U
#endif
#define BOARD_SW2_IRQ PORTA_IRQn
#define BOARD_SW2_IRQ_HANDLER PORTA_IRQHandler
#define BOARD_SW2_NAME "SW2"

/* Board led color mapping */
#define LOGIC_LED_ON 0U
#define LOGIC_LED_OFF 1U
#define BOARD_LED_GREEN1_GPIO GPIOC
#define BOARD_LED_GREEN1_GPIO_PORT PORTC
#define BOARD_LED_GREEN1_GPIO_PIN 1U
#define BOARD_LED_YELLOW1_GPIO GPIOC
#define BOARD_LED_YELLOW1_GPIO_PORT PORTC
#define BOARD_LED_YELLOW1_GPIO_PIN 2U
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
#ifndef BOARD_LED_ORANGE_GPIO
#define BOARD_LED_ORANGE_GPIO GPIOD
#endif
#define BOARD_LED_ORANGE_GPIO_PORT PORTD
#ifndef BOARD_LED_ORANGE_GPIO_PIN
#define BOARD_LED_ORANGE_GPIO_PIN 6U
#endif
#ifndef BOARD_LED_RED_GPIO
#define BOARD_LED_RED_GPIO GPIOD
#endif
#define BOARD_LED_RED_GPIO_PORT PORTD
#ifndef BOARD_LED_RED_GPIO_PIN
#define BOARD_LED_RED_GPIO_PIN 7U
#endif

#define LED_GREEN1_INIT(output)                                              \
    GPIO_PinWrite(BOARD_LED_GREEN1_GPIO, BOARD_LED_GREEN1_GPIO_PIN, output); \
    BOARD_LED_GREEN1_GPIO->PDDR |= (1U << BOARD_LED_GREEN1_GPIO_PIN) /*!< Enable target LED_GREEN1 */
#define LED_GREEN1_ON() \
    GPIO_PortClear(BOARD_LED_GREEN1_GPIO, 1U << BOARD_LED_GREEN1_GPIO_PIN) /*!< Turn on target LED_GREEN1 */
#define LED_GREEN1_OFF() \
    GPIO_PortSet(BOARD_LED_GREEN1_GPIO, 1U << BOARD_LED_GREEN1_GPIO_PIN) /*!< Turn off target LED_GREEN1 */
#define LED_GREEN1_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_GREEN1_GPIO, 1U << BOARD_LED_GREEN1_GPIO_PIN) /*!< Toggle on target LED_GREEN1 */

#define LED_YELLOW1_INIT(output)                                               \
    GPIO_PinWrite(BOARD_LED_YELLOW1_GPIO, BOARD_LED_YELLOW1_GPIO_PIN, output); \
    BOARD_LED_YELLOW1_GPIO->PDDR |= (1U << BOARD_LED_YELLOW1_GPIO_PIN) /*!< Enable target LED_YELLOW1 */
#define LED_YELLOW1_ON() \
    GPIO_PortClear(BOARD_LED_YELLOW1_GPIO, 1U << BOARD_LED_YELLOW1_GPIO_PIN) /*!< Turn on target LED_YELLOW1 */
#define LED_YELLOW1_OFF() \
    GPIO_PortSet(BOARD_LED_YELLOW1_GPIO, 1U << BOARD_LED_YELLOW1_GPIO_PIN) /*!< Turn off target LED_YELLOW1 */
#define LED_YELLOW1_TOGGLE() GPIO_PortToggle(BOARD_LED_YELLOW1_GPIO, 1U << BOARD_LED_YELLOW1_GPIO_PIN)

#define LED_GREEN2_INIT(output)                                              \
    GPIO_PinWrite(BOARD_LED_GREEN2_GPIO, BOARD_LED_GREEN2_GPIO_PIN, output); \
    BOARD_LED_GREEN2_GPIO->PDDR |= (1U << BOARD_LED_GREEN2_GPIO_PIN) /*!< Enable target LED_GREEN2 */
#define LED_GREEN2_ON() \
    GPIO_PortClear(BOARD_LED_GREEN2_GPIO, 1U << BOARD_LED_GREEN2_GPIO_PIN) /*!< Turn on target LED_GREEN2 */
#define LED_GREEN2_OFF() \
    GPIO_PortSet(BOARD_LED_GREEN2_GPIO, 1U << BOARD_LED_GREEN2_GPIO_PIN) /*!< Turn off target LED_GREEN2 */
#define LED_GREEN2_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_GREEN2_GPIO, 1U << BOARD_LED_GREEN2_GPIO_PIN) /*!< Toggle on target LED_GREEN2 */

#define LED_YELLOW2_INIT(output)                                               \
    GPIO_PinWrite(BOARD_LED_YELLOW2_GPIO, BOARD_LED_YELLOW2_GPIO_PIN, output); \
    BOARD_LED_YELLOW2_GPIO->PDDR |= (1U << BOARD_LED_YELLOW2_GPIO_PIN) /*!< Enable target LED_YELLOW2 */
#define LED_YELLOW2_ON() \
    GPIO_PortClear(BOARD_LED_YELLOW2_GPIO, 1U << BOARD_LED_YELLOW2_GPIO_PIN) /*!< Turn on target LED_YELLOW2 */
#define LED_YELLOW2_OFF() \
    GPIO_PortSet(BOARD_LED_YELLOW2_GPIO, 1U << BOARD_LED_YELLOW2_GPIO_PIN) /*!< Turn off target LED_YELLOW2 */
#define LED_YELLOW2_TOGGLE() GPIO_PortToggle(BOARD_LED_YELLOW2_GPIO, 1U << BOARD_LED_YELLOW2_GPIO_PIN)

#define LED_GREEN3_INIT(output)                                              \
    GPIO_PinWrite(BOARD_LED_GREEN3_GPIO, BOARD_LED_GREEN3_GPIO_PIN, output); \
    BOARD_LED_GREEN3_GPIO->PDDR |= (1U << BOARD_LED_GREEN3_GPIO_PIN) /*!< Enable target LED_GREEN3 */
#define LED_GREEN3_ON() \
    GPIO_PortClear(BOARD_LED_GREEN3_GPIO, 1U << BOARD_LED_GREEN3_GPIO_PIN) /*!< Turn on target LED_GREEN3 */
#define LED_GREEN3_OFF() \
    GPIO_PortSet(BOARD_LED_GREEN3_GPIO, 1U << BOARD_LED_GREEN3_GPIO_PIN) /*!< Turn off target LED_GREEN3 */
#define LED_GREEN3_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_GREEN3_GPIO, 1U << BOARD_LED_GREEN3_GPIO_PIN) /*!< Toggle on target LED_GREEN3 */

#define LED_YELLOW3_INIT(output)                                               \
    GPIO_PinWrite(BOARD_LED_YELLOW3_GPIO, BOARD_LED_YELLOW3_GPIO_PIN, output); \
    BOARD_LED_YELLOW3_GPIO->PDDR |= (1U << BOARD_LED_YELLOW3_GPIO_PIN) /*!< Enable target LED_YELLOW3 */
#define LED_YELLOW3_ON() \
    GPIO_PortClear(BOARD_LED_YELLOW3_GPIO, 1U << BOARD_LED_YELLOW3_GPIO_PIN) /*!< Turn on target LED_YELLOW3 */
#define LED_YELLOW3_OFF() \
    GPIO_PortSet(BOARD_LED_YELLOW3_GPIO, 1U << BOARD_LED_YELLOW3_GPIO_PIN) /*!< Turn off target LED_YELLOW3 */
#define LED_YELLOW3_TOGGLE() GPIO_PortToggle(BOARD_LED_YELLOW3_GPIO, 1U << BOARD_LED_YELLOW3_GPIO_PIN)

#define LED_ORANGE_INIT(output)                                              \
    GPIO_PinWrite(BOARD_LED_ORANGE_GPIO, BOARD_LED_ORANGE_GPIO_PIN, output); \
    BOARD_LED_ORANGE_GPIO->PDDR |= (1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Enable target LED_ORANGE */
#define LED_ORANGE_ON() \
    GPIO_PortClear(BOARD_LED_ORANGE_GPIO, 1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Turn on target LED_ORANGE */
#define LED_ORANGE_OFF() \
    GPIO_PortSet(BOARD_LED_ORANGE_GPIO, 1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Turn off target LED_ORANGE */
#define LED_ORANGE_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_ORANGE_GPIO, 1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Toggle on target LED_ORANGE */

#define LED_RED_INIT(output)                                           \
    GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, output); \
    BOARD_LED_RED_GPIO->PDDR |= (1U << BOARD_LED_RED_GPIO_PIN)                        /*!< Enable target LED_RED */
#define LED_RED_ON() GPIO_PortClear(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Turn on target LED_RED */
#define LED_RED_OFF() GPIO_PortSet(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN)  /*!< Turn off target LED_RED */
#define LED_RED_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Toggle on target LED_RED */

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

void BOARD_InitDebugConsole(void);
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
void BOARD_I2C_Init(I2C_Type *base, uint32_t clkSrc_Hz);
status_t BOARD_I2C_Send(I2C_Type *base,
                        uint8_t deviceAddress,
                        uint32_t subAddress,
                        uint8_t subaddressSize,
                        uint8_t *txBuff,
                        uint8_t txBuffSize);
status_t BOARD_I2C_Receive(I2C_Type *base,
                           uint8_t deviceAddress,
                           uint32_t subAddress,
                           uint8_t subaddressSize,
                           uint8_t *rxBuff,
                           uint8_t rxBuffSize);
void BOARD_Accel_I2C_Init(void);
status_t BOARD_Accel_I2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint32_t txBuff);
status_t BOARD_Accel_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
#endif /* SDK_I2C_BASED_COMPONENT_USED */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
