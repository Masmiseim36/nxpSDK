/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Direction type  */
typedef enum _pin_mux_direction
{
  kPIN_MUX_DirectionInput = 0U,         /* Input direction */
  kPIN_MUX_DirectionOutput = 1U,        /* Output direction */
  kPIN_MUX_DirectionInputOrOutput = 2U  /* Input or output direction */
} pin_mux_direction_t;

/*!
 * @addtogroup pin_mux
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/* PORTA1 (number 35), J1[2]/D0/UART1_RX_TGTMCU */
#define BOARD_DEBUG_UART_RX_PERIPHERAL                                     UART0   /*!< Device name: UART0 */
#define BOARD_DEBUG_UART_RX_SIGNAL                                            RX   /*!< UART0 signal: RX */
#define BOARD_DEBUG_UART_RX_PIN_NAME                                    UART0_RX   /*!< Pin name */
#define BOARD_DEBUG_UART_RX_LABEL                     "J1[2]/D0/UART1_RX_TGTMCU"   /*!< Label */
#define BOARD_DEBUG_UART_RX_NAME                                 "DEBUG_UART_RX"   /*!< Identifier name */

/* PORTA2 (number 36), J1[4]/D1/UART1_TX_TGTMCU */
#define BOARD_DEBUG_UART_TX_PERIPHERAL                                     UART0   /*!< Device name: UART0 */
#define BOARD_DEBUG_UART_TX_SIGNAL                                            TX   /*!< UART0 signal: TX */
#define BOARD_DEBUG_UART_TX_PIN_NAME                                    UART0_TX   /*!< Pin name */
#define BOARD_DEBUG_UART_TX_LABEL                     "J1[4]/D1/UART1_TX_TGTMCU"   /*!< Label */
#define BOARD_DEBUG_UART_TX_NAME                                 "DEBUG_UART_TX"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void SPI0_InitPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void SPI0_DeinitPins(void);

/* PORTD5 (number 98), J2[12]/D13/LED_GREEN */
#define SPI1_INITPINS_LED_GREEN_PERIPHERAL                                  SPI1   /*!< Device name: SPI1 */
#define SPI1_INITPINS_LED_GREEN_SIGNAL                                       SCK   /*!< SPI1 signal: SCK */
#define SPI1_INITPINS_LED_GREEN_PIN_NAME                                SPI1_SCK   /*!< Pin name */
#define SPI1_INITPINS_LED_GREEN_LABEL                     "J2[12]/D13/LED_GREEN"   /*!< Label */
#define SPI1_INITPINS_LED_GREEN_NAME                                 "LED_GREEN"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void SPI1_InitPins(void);

/* PORTD5 (number 98), J2[12]/D13/LED_GREEN */
#define SPI1_DEINITPINS_LED_GREEN_PERIPHERAL                                ADC0   /*!< Device name: ADC0 */
#define SPI1_DEINITPINS_LED_GREEN_SIGNAL                                      SE   /*!< ADC0 signal: SE */
#define SPI1_DEINITPINS_LED_GREEN_CHANNEL                                     6b   /*!< ADC0 SE channel: 6b */
#define SPI1_DEINITPINS_LED_GREEN_PIN_NAME                             ADC0_SE6b   /*!< Pin name */
#define SPI1_DEINITPINS_LED_GREEN_LABEL                   "J2[12]/D13/LED_GREEN"   /*!< Label */
#define SPI1_DEINITPINS_LED_GREEN_NAME                               "LED_GREEN"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void SPI1_DeinitPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
