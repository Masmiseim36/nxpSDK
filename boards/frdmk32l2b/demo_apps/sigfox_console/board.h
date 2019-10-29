/*
 * Copyright 2019 NXP
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

/* The board name */
#define BOARD_NAME "FRDM-K32L2B"

/* The LPUART to use for debug messages. */
#define BOARD_DEBUG_UART_TYPE kSerialPort_Uart
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART0
#define BOARD_DEBUG_UART_INSTANCE 0U
#define BOARD_DEBUG_UART_CLKSRC kCLOCK_McgIrc48MClk
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetFreq(kCLOCK_McgIrc48MClk)
#define BOARD_UART_IRQ LPUART0_IRQn
#define BOARD_UART_IRQ_HANDLER LPUART0_IRQHandler

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

#define BOARD_ADC_USE_ALT_VREF 1U

#define BOARD_TPM_BASEADDR TPM0
#define BOARD_TPM_CHANNEL 5U

/*! @brief The bubble level demo information */
#define BOARD_BUBBLE_TPM_BASEADDR TPM0
#define BOARD_TPM_X_CHANNEL 4U
#define BOARD_TPM_Y_CHANNEL 5U
#define BOARD_MMA8451_ADDR 0x1D
#define BOARD_ACCEL_ADDR BOARD_MMA8451_ADDR
#define BOARD_ACCEL_BAUDRATE 100
#define BOARD_ACCEL_I2C_BASEADDR I2C0

/*! @brief The rtc instance used for board. */
#define BOARD_RTC_FUNC_BASEADDR RTC

/*! @brief The Compare instance/channel used for board */
#define BOARD_CMP_BASEADDR CMP0
#define BOARD_CMP_CHANNEL 0U

/*! @brief The i2c instance used for i2c connection by default */
#define BOARD_I2C_BASEADDR I2C1

/*! @brief The spi instance used for board. */
#define BOARD_SPI_BASEADDR SPI1

/* Board led color mapping */
#define LOGIC_LED_ON 0U
#define LOGIC_LED_OFF 1U
#ifndef BOARD_LED_RED_GPIO
#define BOARD_LED_RED_GPIO GPIOE
#endif
#define BOARD_LED_RED_GPIO_PORT PORTE
#ifndef BOARD_LED_RED_GPIO_PIN
#define BOARD_LED_RED_GPIO_PIN 31U
#endif
#ifndef BOARD_LED_GREEN_GPIO
#define BOARD_LED_GREEN_GPIO GPIOD
#endif
#define BOARD_LED_GREEN_GPIO_PORT PORTD
#ifndef BOARD_LED_GREEN_GPIO_PIN
#define BOARD_LED_GREEN_GPIO_PIN 5U
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

/* ERPC SPI configuration */
#define ERPC_BOARD_SPI_BASEADDR SPI1
#define ERPC_BOARD_SPI_BAUDRATE 500000U
#define ERPC_BOARD_SPI_CLKSRC SPI0_CLK_SRC
#define ERPC_BOARD_SPI_CLK_FREQ CLOCK_GetFreq(SPI0_CLK_SRC)
#define ERPC_BOARD_SPI_INT_GPIO GPIOC
#define ERPC_BOARD_SPI_INT_PORT PORTC
#define ERPC_BOARD_SPI_INT_PIN 1U
#define ERPC_BOARD_SPI_INT_PIN_IRQ PORTC_PORTD_IRQn
#define ERPC_BOARD_SPI_INT_PIN_IRQ_HANDLER PORTC_PORTD_IRQHandler

/* DAC base address */
#define BOARD_DAC_BASEADDR DAC0

/* Board accelerometer driver */
#define BOARD_ACCEL_MMA

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
