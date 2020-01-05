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

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

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
void UART0_InitPins(void);

/* PORTA1 (number 35), J1[2]/D0/UART1_RX_TGTMCU */
#define UART0_DEINITPINS_DEBUG_UART_RX_PERIPHERAL                           TSI0   /*!< Device name: TSI0 */
#define UART0_DEINITPINS_DEBUG_UART_RX_SIGNAL                                 CH   /*!< TSI0 signal: CH */
#define UART0_DEINITPINS_DEBUG_UART_RX_CHANNEL                                 2   /*!< TSI0 channel: 2 */
#define UART0_DEINITPINS_DEBUG_UART_RX_PIN_NAME                         TSI0_CH2   /*!< Pin name */
#define UART0_DEINITPINS_DEBUG_UART_RX_LABEL          "J1[2]/D0/UART1_RX_TGTMCU"   /*!< Label */
#define UART0_DEINITPINS_DEBUG_UART_RX_NAME                      "DEBUG_UART_RX"   /*!< Identifier name */

/* PORTA2 (number 36), J1[4]/D1/UART1_TX_TGTMCU */
#define UART0_DEINITPINS_DEBUG_UART_TX_PERIPHERAL                           TSI0   /*!< Device name: TSI0 */
#define UART0_DEINITPINS_DEBUG_UART_TX_SIGNAL                                 CH   /*!< TSI0 signal: CH */
#define UART0_DEINITPINS_DEBUG_UART_TX_CHANNEL                                 3   /*!< TSI0 channel: 3 */
#define UART0_DEINITPINS_DEBUG_UART_TX_PIN_NAME                         TSI0_CH3   /*!< Pin name */
#define UART0_DEINITPINS_DEBUG_UART_TX_LABEL          "J1[4]/D1/UART1_TX_TGTMCU"   /*!< Label */
#define UART0_DEINITPINS_DEBUG_UART_TX_NAME                      "DEBUG_UART_TX"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART0_DeinitPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART1_InitPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART1_DeinitPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART2_InitPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART2_DeinitPins(void);

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
