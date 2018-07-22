/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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
#define BOARD_NAME "TWR-K65F180M"

/*! @brief The UART to use for debug messages. */
#define BOARD_USE_UART
#define BOARD_DEBUG_UART_TYPE DEBUG_CONSOLE_DEVICE_TYPE_UART
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) UART2
#define BOARD_DEBUG_UART_CLKSRC BUS_CLK
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetBusClkFreq()
#define BOARD_UART_IRQ UART2_RX_TX_IRQn
#define BOARD_UART_IRQ_HANDLER UART2_RX_TX_IRQHandler

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/*! @brief The CAN instance used for board */
#define BOARD_CAN_BASEADDR CAN0

/*! @brief The i2c instance used for i2c connection by default */
#define BOARD_I2C_BASEADDR I2C0
#define BOARD_ACCEL_I2C_BASEADDR I2C0

/*! @brief The Enet instance used for board. */
#define BOARD_ENET_BASEADDR ENET

/*! @brief The FlexBus instance used for board.*/
#define BOARD_FLEXBUS_BASEADDR FB

#define BOARD_TSI_ELECTRODE_CNT 2U

/*! @brief Indexes of the TSI channels for on board electrodes */
#define BOARD_TSI_ELECTRODE_1 11U
#define BOARD_TSI_ELECTRODE_2 12U

/*! @brief The SDHC instance/channel used for board */
#define BOARD_SDHC_BASEADDR SDHC
#define BOARD_SDHC_CD_GPIO_IRQ_HANDLER PORTA_IRQHandler

/*! @brief The CMP instance/channel used for board. */
#define BOARD_CMP_BASEADDR CMP2
#define BOARD_CMP_CHANNEL 2U

/*! @brief The i2c instance used for sai demo */
#define BOARD_SAI_DEMO_I2C_BASEADDR I2C0

/*! @brief The rtc instance used for rtc_func */
#define BOARD_RTC_FUNC_BASEADDR RTC

/*! @brief If connected the TWR_MEM, this is spi sd card */
#define BOARD_SDCARD_CARD_DETECTION_GPIO GPIOD
#define BOARD_SDCARD_CARD_DETECTION_GPIO_PORT PORTD
#define SDCARD_CARD_DETECTION_GPIO_PIN 15U
#define SDCARD_CARD_WRITE_PROTECTION_GPIO GPIOC
#define SDCARD_CARD_WRITE_PROTECTION_GPIO_PORT PORTC
#define SDCARD_CARD_WRITE_PROTECTION_GPIO_PIN 13U
#define SDCARD_SPI_HW_BASEADDR SPI1
#define SDCARD_CARD_INSERTED 0U

/*! @brief Define the port interrupt number for the board switches */
#define BOARD_SW3_GPIO GPIOA
#define BOARD_SW3_PORT PORTA
#define BOARD_SW3_GPIO_PIN 10
#define BOARD_SW3_IRQ PORTA_IRQn
#define BOARD_SW3_IRQ_HANDLER PORTA_IRQHandler
#define BOARD_SW3_NAME "SW3"

#define BOARD_DEMO_I2C_CLKSRC kCLOCK_BusClk
#define BOARD_DEMO_SAI_CLKSRC kCLOCK_CoreSysClk
#define BOARD_DEMO_SAI I2S0
#define LLWU_SW_GPIO BOARD_SW3_GPIO
#define LLWU_SW_PORT BOARD_SW3_PORT
#define LLWU_SW_GPIO_PIN BOARD_SW3_GPIO_PIN
#define LLWU_SW_IRQ BOARD_SW3_IRQ
#define LLWU_SW_IRQ_HANDLER BOARD_SW3_IRQ_HANDLER
#define LLWU_SW_NAME BOARD_SW3_NAME

/* Board led color mapping */
#define LOGIC_LED_ON 0U
#define LOGIC_LED_OFF 1U
#define BOARD_LED_YELLOW_GPIO GPIOB
#define BOARD_LED_YELLOW_GPIO_PORT PORTB
#define BOARD_LED_YELLOW_GPIO_PIN 4U
#define BOARD_LED_ORANGE_GPIO GPIOB
#define BOARD_LED_ORANGE_GPIO_PORT PORTB
#define BOARD_LED_ORANGE_GPIO_PIN 5U
#define BOARD_LED_GREEN_GPIO GPIOA
#define BOARD_LED_GREEN_GPIO_PORT PORTA
#define BOARD_LED_GREEN_GPIO_PIN 28U
#define BOARD_LED_BLUE_GPIO GPIOA
#define BOARD_LED_BLUE_GPIO_PORT PORTA
#define BOARD_LED_BLUE_GPIO_PIN 29U

#define LED_YELLOW_INIT(output)                                                    \
    GPIO_WritePinOutput(BOARD_LED_YELLOW_GPIO, BOARD_LED_YELLOW_GPIO_PIN, output); \
    BOARD_LED_YELLOW_GPIO->PDDR |= (1U << BOARD_LED_YELLOW_GPIO_PIN) /*!< Enable target LED_YELLOW */
#define LED_YELLOW_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_YELLOW_GPIO, 1U << BOARD_LED_YELLOW_GPIO_PIN) /*!< Turn on target LED_YELLOW */
#define LED_YELLOW_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_YELLOW_GPIO, 1U << BOARD_LED_YELLOW_GPIO_PIN) /*!< Turn off target LED_YELLOW */
#define LED_YELLOW_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_YELLOW_GPIO, 1U << BOARD_LED_YELLOW_GPIO_PIN) /*!< Toggle on target LED_YELLOW */

#define LED_ORANGE_INIT(output)                                                    \
    GPIO_WritePinOutput(BOARD_LED_ORANGE_GPIO, BOARD_LED_ORANGE_GPIO_PIN, output); \
    BOARD_LED_ORANGE_GPIO->PDDR |= (1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Enable target LED_ORANGE */
#define LED_ORANGE_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_ORANGE_GPIO, 1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Turn on target LED_ORANGE */
#define LED_ORANGE_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_ORANGE_GPIO, 1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Turn off target LED_ORANGE */
#define LED_ORANGE_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_ORANGE_GPIO, 1U << BOARD_LED_ORANGE_GPIO_PIN) /*!< Toggle on target LED_ORANGE */

#define LED_GREEN_INIT(output)                                                   \
    GPIO_WritePinOutput(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, output); \
    BOARD_LED_GREEN_GPIO->PDDR |= (1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Enable target LED_GREEN */
#define LED_GREEN_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn on target LED_GREEN */
#define LED_GREEN_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn off target LED_GREEN */
#define LED_GREEN_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Toggle on target LED_GREEN */

#define LED_BLUE_INIT(output)                                                  \
    GPIO_WritePinOutput(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, output); \
    BOARD_LED_BLUE_GPIO->PDDR |= (1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Enable target LED_BLUE */
#define LED_BLUE_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn on target LED_BLUE */
#define LED_BLUE_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn off target LED_BLUE */
#define LED_BLUE_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Toggle on target LED_BLUE */

/*! @brief Define the port interrupt number for the usb id gpio pin */
#define BOARD_ID_GPIO GPIOE
#define BOARD_ID_PORT PORTE
#define BOARD_ID_GPIO_PIN 10U
#define BOARD_ID_IRQ PORTE_IRQn

/* SDHC base address, clock and card detection pin */
#define BOARD_SDHC_BASEADDR SDHC
#define BOARD_SDHC_CLKSRC kCLOCK_CoreSysClk
#define BOARD_SDHC_IRQ SDHC_IRQn
#define BOARD_SDHC_CD_GPIO_BASE GPIOA
#define BOARD_SDHC_CD_GPIO_PIN 9U
#define BOARD_SDHC_CD_PORT_BASE PORTA
#define BOARD_SDHC_CD_PORT_IRQ PORTA_IRQn
#define BOARD_SDHC_CD_PORT_IRQ_HANDLER PORTA_IRQHandler

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
