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

/*! @name PORTA4 (number 38), CN9[38]/CN2[6]/KEY_SEL
  @{ */
#define BOARD_KEY_SEL_PERIPHERAL n/a                                 /*!<@brief Device name: n/a */
#define BOARD_KEY_SEL_SIGNAL disabled                                /*!<@brief n/a signal: disabled */
#define BOARD_KEY_SEL_PORT PORTA                                     /*!<@brief PORT device name: PORTA */
#define BOARD_KEY_SEL_PIN 4U                                         /*!<@brief PORTA pin index: 4 */
#define BOARD_KEY_SEL_PIN_NAME PTA4/LLWU_P3/TPM0_CH1/I2S0_MCLK/NMI_b /*!<@brief Pin name */
#define BOARD_KEY_SEL_LABEL "CN9[38]/CN2[6]/KEY_SEL"                 /*!<@brief Label */
#define BOARD_KEY_SEL_NAME "KEY_SEL"                                 /*!<@brief Identifier name */
                                                                     /* @} */

/*! @name PORTE0 (number 1), CN9[1]/M1[5]/CN2[24]/UART1_TX/DBG_TXD
  @{ */
#define BOARD_DEBUG_UART_TX_PERIPHERAL UART1                              /*!<@brief Device name: UART1 */
#define BOARD_DEBUG_UART_TX_SIGNAL TX                                     /*!<@brief UART1 signal: TX */
#define BOARD_DEBUG_UART_TX_PORT PORTE                                    /*!<@brief PORT device name: PORTE */
#define BOARD_DEBUG_UART_TX_PIN 0U                                        /*!<@brief PORTE pin index: 0 */
#define BOARD_DEBUG_UART_TX_PIN_NAME UART1_TX                             /*!<@brief Pin name */
#define BOARD_DEBUG_UART_TX_LABEL "CN9[1]/M1[5]/CN2[24]/UART1_TX/DBG_TXD" /*!<@brief Label */
#define BOARD_DEBUG_UART_TX_NAME "DEBUG_UART_TX"                          /*!<@brief Identifier name */
                                                                          /* @} */

/*! @name PORTE1 (number 2), CN9[2]/CN2[31]/M1[6]/UART1_RX/DBG_RXD
  @{ */
#define BOARD_DEBUG_UART_RX_PERIPHERAL UART1                              /*!<@brief Device name: UART1 */
#define BOARD_DEBUG_UART_RX_SIGNAL RX                                     /*!<@brief UART1 signal: RX */
#define BOARD_DEBUG_UART_RX_PORT PORTE                                    /*!<@brief PORT device name: PORTE */
#define BOARD_DEBUG_UART_RX_PIN 1U                                        /*!<@brief PORTE pin index: 1 */
#define BOARD_DEBUG_UART_RX_PIN_NAME UART1_RX                             /*!<@brief Pin name */
#define BOARD_DEBUG_UART_RX_LABEL "CN9[2]/CN2[31]/M1[6]/UART1_RX/DBG_RXD" /*!<@brief Label */
#define BOARD_DEBUG_UART_RX_NAME "DEBUG_UART_RX"                          /*!<@brief Identifier name */
                                                                          /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
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
