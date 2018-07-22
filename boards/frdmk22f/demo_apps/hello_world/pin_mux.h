/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/*! @brief Direction type  */
typedef enum _pin_mux_direction
{
    kPIN_MUX_DirectionInput = 0U,        /* Input direction */
    kPIN_MUX_DirectionOutput = 1U,       /* Output direction */
    kPIN_MUX_DirectionInputOrOutput = 2U /* Input or output direction */
} pin_mux_direction_t;

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif


#define SOPT5_UART1TXSRC_UART_TX 0x00u /*!<@brief UART 1 transmit data source select: UART1_TX pin */

/*! @name PORTE0 (number 1), J2[18]/UART1_TX_TGTMCU
  @{ */
#define BOARD_DEBUG_UART_TX_PERIPHERAL UART1               /*!<@brief Device name: UART1 */
#define BOARD_DEBUG_UART_TX_SIGNAL TX                      /*!<@brief UART1 signal: TX */
#define BOARD_DEBUG_UART_TX_PORT PORTE                     /*!<@brief PORT device name: PORTE */
#define BOARD_DEBUG_UART_TX_PIN 0U                         /*!<@brief PORTE pin index: 0 */
#define BOARD_DEBUG_UART_TX_PIN_NAME UART1_TX              /*!<@brief Pin name */
#define BOARD_DEBUG_UART_TX_LABEL "J2[18]/UART1_TX_TGTMCU" /*!<@brief Label */
#define BOARD_DEBUG_UART_TX_NAME "DEBUG_UART_TX"           /*!<@brief Identifier name */
                                                           /* @} */

/*! @name PORTE1 (number 2), J2[20]/UART1_RX_TGTMCU
  @{ */
#define BOARD_DEBUG_UART_RX_PERIPHERAL UART1               /*!<@brief Device name: UART1 */
#define BOARD_DEBUG_UART_RX_SIGNAL RX                      /*!<@brief UART1 signal: RX */
#define BOARD_DEBUG_UART_RX_PORT PORTE                     /*!<@brief PORT device name: PORTE */
#define BOARD_DEBUG_UART_RX_PIN 1U                         /*!<@brief PORTE pin index: 1 */
#define BOARD_DEBUG_UART_RX_PIN_NAME UART1_RX              /*!<@brief Pin name */
#define BOARD_DEBUG_UART_RX_LABEL "J2[20]/UART1_RX_TGTMCU" /*!<@brief Label */
#define BOARD_DEBUG_UART_RX_NAME "DEBUG_UART_RX"           /*!<@brief Identifier name */
                                                           /* @} */

/*!
 *
 */
void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
