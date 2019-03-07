/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2017 NXP
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
 *
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

#include "board.h"

/* UART1_RX_DATA (coord K16), UART1_RXD */
#define BOARD_INITPINS_UART1_RXD_PERIPHERAL                                UART1   /*!< Device name: UART1 */
#define BOARD_INITPINS_UART1_RXD_SIGNAL                                  uart_rx   /*!< UART1 signal: uart_rx */
#define BOARD_INITPINS_UART1_RXD_PIN_NAME                          UART1_RX_DATA   /*!< Pin name */
#define BOARD_INITPINS_UART1_RXD_LABEL                               "UART1_RXD"   /*!< Label */
#define BOARD_INITPINS_UART1_RXD_NAME                                "UART1_RXD"   /*!< Identifier name */

/* UART1_TX_DATA (coord K14), UART1_TXD */
#define BOARD_INITPINS_UART1_TXD_PERIPHERAL                                UART1   /*!< Device name: UART1 */
#define BOARD_INITPINS_UART1_TXD_SIGNAL                                  uart_tx   /*!< UART1 signal: uart_tx */
#define BOARD_INITPINS_UART1_TXD_PIN_NAME                          UART1_TX_DATA   /*!< Pin name */
#define BOARD_INITPINS_UART1_TXD_LABEL                               "UART1_TXD"   /*!< Label */
#define BOARD_INITPINS_UART1_TXD_NAME                                "UART1_TXD"   /*!< Identifier name */

/* SNVS_TAMPER7 (coord N10), SNVS_TAMPER7/SHIFT_STCP/P801A[24] */
#define BOARD_INITPINS_SHIFT_STCP_GPIO                                     GPIO5   /*!< GPIO device name: GPIO5 */
#define BOARD_INITPINS_SHIFT_STCP_GPIO_PIN                                    7U   /*!< GPIO5 pin index: 7 */
#define BOARD_INITPINS_SHIFT_STCP_PIN_NAME                          SNVS_TAMPER7   /*!< Pin name */
#define BOARD_INITPINS_SHIFT_STCP_LABEL      "SNVS_TAMPER7/SHIFT_STCP/P801A[24]"   /*!< Label */
#define BOARD_INITPINS_SHIFT_STCP_NAME                              "SHIFT_STCP"   /*!< Identifier name */

/* SNVS_TAMPER8 (coord N9), SNVS_TAMPER8/SHIFT_nOE/P801A[18] */
#define BOARD_INITPINS_SHIFT_nOE_GPIO                                      GPIO5   /*!< GPIO device name: GPIO5 */
#define BOARD_INITPINS_SHIFT_nOE_GPIO_PIN                                     8U   /*!< GPIO5 pin index: 8 */
#define BOARD_INITPINS_SHIFT_nOE_PIN_NAME                           SNVS_TAMPER8   /*!< Pin name */
#define BOARD_INITPINS_SHIFT_nOE_LABEL        "SNVS_TAMPER8/SHIFT_nOE/P801A[18]"   /*!< Label */
#define BOARD_INITPINS_SHIFT_nOE_NAME                                "SHIFT_nOE"   /*!< Identifier name */

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif

#endif /* _PIN_MUX_H_ */
