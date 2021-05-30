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


#define SOPT5_UART0RXSRC_UART_RX 0x00u /*!<@brief UART0 Receive Data Source Select: UART_RX pin */
#define SOPT5_UART0TXSRC_UART_TX 0x00u /*!<@brief UART0 Transmit Data Source Select: UART0_TX pin */

/*! @name PORTA1 (number 35), J1[2]/D0/UART1_RX_TGTMCU
  @{ */
#define BOARD_DEBUG_UART_RX_PERIPHERAL UART0                 /*!<@brief Device name: UART0 */
#define BOARD_DEBUG_UART_RX_SIGNAL RX                        /*!<@brief UART0 signal: RX */
#define BOARD_DEBUG_UART_RX_PORT PORTA                       /*!<@brief PORT device name: PORTA */
#define BOARD_DEBUG_UART_RX_PIN 1U                           /*!<@brief PORTA pin index: 1 */
#define BOARD_DEBUG_UART_RX_PIN_NAME UART0_RX                /*!<@brief Pin name */
#define BOARD_DEBUG_UART_RX_LABEL "J1[2]/D0/UART1_RX_TGTMCU" /*!<@brief Label */
#define BOARD_DEBUG_UART_RX_NAME "DEBUG_UART_RX"             /*!<@brief Identifier name */
                                                             /* @} */

/*! @name PORTA2 (number 36), J1[4]/D1/UART1_TX_TGTMCU
  @{ */
#define BOARD_DEBUG_UART_TX_PERIPHERAL UART0                 /*!<@brief Device name: UART0 */
#define BOARD_DEBUG_UART_TX_SIGNAL TX                        /*!<@brief UART0 signal: TX */
#define BOARD_DEBUG_UART_TX_PORT PORTA                       /*!<@brief PORT device name: PORTA */
#define BOARD_DEBUG_UART_TX_PIN 2U                           /*!<@brief PORTA pin index: 2 */
#define BOARD_DEBUG_UART_TX_PIN_NAME UART0_TX                /*!<@brief Pin name */
#define BOARD_DEBUG_UART_TX_LABEL "J1[4]/D1/UART1_TX_TGTMCU" /*!<@brief Label */
#define BOARD_DEBUG_UART_TX_NAME "DEBUG_UART_TX"             /*!<@brief Identifier name */
                                                             /* @} */

/*! @name PORTE29 (number 26), LED_RED
  @{ */
#define BOARD_LED_RED_PERIPHERAL TPM0   /*!<@brief Device name: TPM0 */
#define BOARD_LED_RED_SIGNAL CH         /*!<@brief TPM0 signal: CH */
#define BOARD_LED_RED_PORT PORTE        /*!<@brief PORT device name: PORTE */
#define BOARD_LED_RED_PIN 29U           /*!<@brief PORTE pin index: 29 */
#define BOARD_LED_RED_CHANNEL 2         /*!<@brief TPM0 channel: 2 */
#define BOARD_LED_RED_PIN_NAME TPM0_CH2 /*!<@brief Pin name */
#define BOARD_LED_RED_LABEL "LED_RED"   /*!<@brief Label */
#define BOARD_LED_RED_NAME "LED_RED"    /*!<@brief Identifier name */
                                        /* @} */

/*! @name PORTD5 (number 98), J2[12]/D13/LED_GREEN
  @{ */
#define BOARD_LED_GREEN_PERIPHERAL TPM0              /*!<@brief Device name: TPM0 */
#define BOARD_LED_GREEN_SIGNAL CH                    /*!<@brief TPM0 signal: CH */
#define BOARD_LED_GREEN_PORT PORTD                   /*!<@brief PORT device name: PORTD */
#define BOARD_LED_GREEN_PIN 5U                       /*!<@brief PORTD pin index: 5 */
#define BOARD_LED_GREEN_CHANNEL 5                    /*!<@brief TPM0 channel: 5 */
#define BOARD_LED_GREEN_PIN_NAME TPM0_CH5            /*!<@brief Pin name */
#define BOARD_LED_GREEN_LABEL "J2[12]/D13/LED_GREEN" /*!<@brief Label */
#define BOARD_LED_GREEN_NAME "LED_GREEN"             /*!<@brief Identifier name */
                                                     /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*!
 *
 */
void BOARD_I2C_ConfigurePins(void);

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
