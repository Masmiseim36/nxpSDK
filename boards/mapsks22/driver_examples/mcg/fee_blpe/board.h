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
/* The board name */
#define BOARD_NAME "MAPS-KS22"

/* The UART to use for debug messages. */
#define BOARD_DEBUG_UART_TYPE DEBUG_CONSOLE_DEVICE_TYPE_UART
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) UART1
#define BOARD_DEBUG_UART_INSTANCE 1U
#define BOARD_DEBUG_UART_CLKSRC kCLOCK_PllFllSelClk
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetPllFllSelClkFreq()
#define BOARD_UART_IRQ UART1_RX_TX_IRQn
#define BOARD_UART_IRQ_HANDLER UART1_RX_TX_IRQHandler

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/* Define the port interrupt number for the board switches */
/* Switch K2 Wakeup on MAPS-KS22F256 board */
#ifndef BOARD_SW2_GPIO
#define BOARD_SW2_GPIO GPIOD
#endif
#ifndef BOARD_SW2_PORT
#define BOARD_SW2_PORT PORTD
#endif
#ifndef BOARD_SW2_GPIO_PIN
#define BOARD_SW2_GPIO_PIN 0
#endif

#define BOARD_SW2_IRQ PORTD_IRQn
#define BOARD_SW2_IRQ_HANDLER PORTD_IRQHandler
#define BOARD_SW2_NAME "K2 Wakeup"

/* Switch 3 on MAPS-DOCK board */
#ifndef BOARD_SW3_GPIO
#define BOARD_SW3_GPIO GPIOC
#endif
#ifndef BOARD_SW3_PORT
#define BOARD_SW3_PORT PORTC
#endif
#ifndef BOARD_SW3_GPIO_PIN
#define BOARD_SW3_GPIO_PIN 14U
#endif

#define BOARD_SW3_IRQ PORTC_IRQn
#define BOARD_SW3_IRQ_HANDLER PORTC_IRQHandler
#define BOARD_SW3_NAME "Dock K3"

/* Board led color mapping */
#define LOGIC_LED_ON 0U
#define LOGIC_LED_OFF 1U
#define BOARD_LED_RED1_GPIO GPIOB
#define BOARD_LED_RED1_GPIO_PORT PORTB
#define BOARD_LED_RED1_GPIO_PIN 3U
#define BOARD_LED_RED2_GPIO GPIOB
#define BOARD_LED_RED2_GPIO_PORT PORTB
#define BOARD_LED_RED2_GPIO_PIN 9U
#define BOARD_LED_RED3_GPIO GPIOB
#define BOARD_LED_RED3_GPIO_PORT PORTB
#define BOARD_LED_RED3_GPIO_PIN 10U
#ifndef BOARD_LED_GREEN_GPIO
#define BOARD_LED_GREEN_GPIO GPIOB
#endif
#define BOARD_LED_GREEN_GPIO_PORT PORTB
#ifndef BOARD_LED_GREEN_GPIO_PIN
#define BOARD_LED_GREEN_GPIO_PIN 11U
#endif

#define LED_RED1_INIT(output)                                                  \
    GPIO_PinWrite(BOARD_LED_RED1_GPIO, BOARD_LED_RED1_GPIO_PIN, output); \
    BOARD_LED_RED1_GPIO->PDDR |= (1U << BOARD_LED_RED1_GPIO_PIN) /*!< Enable target LED_RED1 */
#define LED_RED1_ON() \
    GPIO_PortClear(BOARD_LED_RED1_GPIO, 1U << BOARD_LED_RED1_GPIO_PIN) /*!< Turn on target LED_RED1 */
#define LED_RED1_OFF() \
    GPIO_PortSet(BOARD_LED_RED1_GPIO, 1U << BOARD_LED_RED1_GPIO_PIN) /*!< Turn off target LED_RED1 */
#define LED_RED1_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_RED1_GPIO, 1U << BOARD_LED_RED1_GPIO_PIN) /*!< Toggle on target LED_RED1 */

#define LED_RED2_INIT(output)                                                  \
    GPIO_PinWrite(BOARD_LED_RED2_GPIO, BOARD_LED_RED2_GPIO_PIN, output); \
    BOARD_LED_RED2_GPIO->PDDR |= (1U << BOARD_LED_RED2_GPIO_PIN) /*!< Enable target LED_RED2 */
#define LED_RED2_ON() \
    GPIO_PortClear(BOARD_LED_RED2_GPIO, 1U << BOARD_LED_RED2_GPIO_PIN) /*!< Turn on target LED_RED2 */
#define LED_RED2_OFF() \
    GPIO_PortSet(BOARD_LED_RED2_GPIO, 1U << BOARD_LED_RED2_GPIO_PIN) /*!< Turn off target LED_RED2 */
#define LED_RED2_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_RED2_GPIO, 1U << BOARD_LED_RED2_GPIO_PIN) /*!< Toggle on target LED_RED2 */

#define LED_RED3_INIT(output)                                                  \
    GPIO_PinWrite(BOARD_LED_RED3_GPIO, BOARD_LED_RED3_GPIO_PIN, output); \
    BOARD_LED_RED3_GPIO->PDDR |= (1U << BOARD_LED_RED3_GPIO_PIN) /*!< Enable target LED_RED3 */
#define LED_RED3_ON() \
    GPIO_PortClear(BOARD_LED_RED3_GPIO, 1U << BOARD_LED_RED3_GPIO_PIN) /*!< Turn on target LED_RED3 */
#define LED_RED3_OFF() \
    GPIO_PortSet(BOARD_LED_RED3_GPIO, 1U << BOARD_LED_RED3_GPIO_PIN) /*!< Turn off target LED_RED3 */
#define LED_RED3_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_RED3_GPIO, 1U << BOARD_LED_RED3_GPIO_PIN) /*!< Toggle on target LED_RED3 */

#define LED_GREEN_INIT(output)                                                   \
    GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, output); \
    BOARD_LED_GREEN_GPIO->PDDR |= (1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Enable target LED_GREEN */
#define LED_GREEN_ON() \
    GPIO_PortClear(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn on target LED_GREEN */
#define LED_GREEN_OFF() \
    GPIO_PortSet(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn off target LED_GREEN */
#define LED_GREEN_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Toggle on target LED_GREEN */
/* @brief The SMARTCARD PHY configuration. */
#define BOARD_SMARTCARD_MODULE (UART0)                 /*!< SMARTCARD communicational module instance */
#define BOARD_SMARTCARD_MODULE_IRQ (UART0_RX_TX_IRQn)  /*!< SMARTCARD communicational module IRQ handler */
#define BOARD_SMARTCARD_MODULE_ERRIRQ (UART0_ERR_IRQn) /*!< SMARTCARD communicational module IRQ error handler */
#define BOARD_SMARTCARD_CLOCK_MODULE (1u)              /*!< SMARTCARD clock generation module instance (TPM1) */
#define BOARD_SMARTCARD_CLOCK_MODULE_CHANNEL (1u)      /*!< SMARTCARD clock generation module channel */
#define BOARD_SMARTCARD_CLOCK_MODULE_SOURCE_CLK \
    (kCLOCK_PllFllSelClk) /*!< SMARTCARD clock module peripheral source clock */
#define BOARD_SMARTCARD_CLOCK_MODULE_CLK_FREQ CLOCK_GetFreq(kCLOCK_PllFllSelClk)
#define BOARD_SMARTCARD_CLOCK_VALUE \
    (4000000u)                            /*!< SMARTCARD clock frequency (4Mhz clock because of HSRUN freq is 72MHz ) */
#define BOARD_SMARTCARD_CONTROL_PORT (2u) /*!< SMARTCARD control pin port instance number (PORTC) */
#define BOARD_SMARTCARD_CONTROL_PIN (11u) /*!< SMARTCARD control pin number */
#ifndef BOARD_SMARTCARD_RST_PORT
#define BOARD_SMARTCARD_RST_PORT (2u) /*!< SMARTCARD reset pin port instance number (PORTC) */
#endif
#ifndef BOARD_SMARTCARD_RST_PIN
#define BOARD_SMARTCARD_RST_PIN (10u) /*!< SMARTCARD reset pin number */
#endif
#define BOARD_SMARTCARD_TS_TIMER_ID \
    (0) /*!< SMARTCARD initial char detection timer instance number (PIT0 timer channel 0) */
#define BOARD_SMARTCARD_TS_TIMER_IRQ (PIT0_IRQn) /*!< SMARTCARD initial char detection timer interrupt handler */
#define BOARD_SMARTCARD_DATA_PORT (0u)           /*!< SMARTCARD data pin port instance number (PORTA) */
#define BOARD_SMARTCARD_DATA_PIN (2u)            /*!< SMARTCARD data pin number */
#define BOARD_SMARTCARD_DATA_PIN_MUX (2u)        /*!< SMARTCARD data pin mux selection to operate UART0_TX (ATL2) */

/* @brief The SDSPI disk PHY configuration. */
#define BOARD_SDSPI_SPI_BASE (SPI0_BASE) /*!< SPI base address for SDSPI */
#define BOARD_SDSPI_SPI_PCS_NUMBER (1U)  /*!< SPI instance for SDSPI */
#define BOARD_SDSPI_CD_GPIO_BASE (GPIOC) /*!< Port related to card detect pin for SDSPI */
#ifndef BOARD_SDSPI_CD_PIN
#define BOARD_SDSPI_CD_PIN (0U) /*!< Card detect pin for SDSPI */
#endif
#define BOARD_SDSPI_CD_LOGIC_RISING /*!< Logic of card detect pin for SDSPI */

#define BOARD_CODEC_I2C_BASEADDR LPI2C0
#define BOARD_CODEC_I2C_CLOCK_FREQ CLOCK_GetFreq(kCLOCK_McgIrc48MClk)

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

void BOARD_InitDebugConsole(void);

#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
void BOARD_InitDebugConsole(void);
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
