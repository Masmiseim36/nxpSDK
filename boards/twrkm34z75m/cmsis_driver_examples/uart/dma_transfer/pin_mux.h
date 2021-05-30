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
void UART0_InitPins(void);

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

/* PORTI6 (number 6), UART2_RX */
#define BOARD_DEBUG_UART_RX_PERIPHERAL                                     UART2   /*!< Device name: UART2 */
#define BOARD_DEBUG_UART_RX_SIGNAL                                            RX   /*!< UART2 signal: RX */
#define BOARD_DEBUG_UART_RX_PIN_NAME                                    UART2_RX   /*!< Pin name */
#define BOARD_DEBUG_UART_RX_LABEL                                     "UART2_RX"   /*!< Label */
#define BOARD_DEBUG_UART_RX_NAME                                 "DEBUG_UART_RX"   /*!< Identifier name */

/* PORTI7 (number 7), UART2_TX */
#define BOARD_DEBUG_UART_TX_PERIPHERAL                                     UART2   /*!< Device name: UART2 */
#define BOARD_DEBUG_UART_TX_SIGNAL                                            TX   /*!< UART2 signal: TX */
#define BOARD_DEBUG_UART_TX_PIN_NAME                                    UART2_TX   /*!< Pin name */
#define BOARD_DEBUG_UART_TX_LABEL                                     "UART2_TX"   /*!< Label */
#define BOARD_DEBUG_UART_TX_NAME                                 "DEBUG_UART_TX"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART2_InitPins(void);

/* PORTI6 (number 6), UART2_RX */
#define UART2_DEINITPINS_DEBUG_UART_RX_PERIPHERAL                            LCD   /*!< Device name: LCD */
#define UART2_DEINITPINS_DEBUG_UART_RX_SIGNAL                                  P   /*!< LCD signal: P */
#define UART2_DEINITPINS_DEBUG_UART_RX_CHANNEL                                46   /*!< LCD P channel: 46 */
#define UART2_DEINITPINS_DEBUG_UART_RX_PIN_NAME                          LCD_P46   /*!< Pin name */
#define UART2_DEINITPINS_DEBUG_UART_RX_LABEL                          "UART2_RX"   /*!< Label */
#define UART2_DEINITPINS_DEBUG_UART_RX_NAME                      "DEBUG_UART_RX"   /*!< Identifier name */

/* PORTI7 (number 7), UART2_TX */
#define UART2_DEINITPINS_DEBUG_UART_TX_PERIPHERAL                            LCD   /*!< Device name: LCD */
#define UART2_DEINITPINS_DEBUG_UART_TX_SIGNAL                                  P   /*!< LCD signal: P */
#define UART2_DEINITPINS_DEBUG_UART_TX_CHANNEL                                47   /*!< LCD P channel: 47 */
#define UART2_DEINITPINS_DEBUG_UART_TX_PIN_NAME                          LCD_P47   /*!< Pin name */
#define UART2_DEINITPINS_DEBUG_UART_TX_LABEL                          "UART2_TX"   /*!< Label */
#define UART2_DEINITPINS_DEBUG_UART_TX_NAME                      "DEBUG_UART_TX"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART2_DeinitPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART3_InitPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART3_DeinitPins(void);

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
