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


/*! @name PORTE16 (coord K3), J14[44]/J20[17]/UART2_TX
  @{ */
#define BOARD_DEBUG_UART_TX_PERIPHERAL UART2                 /*!<@brief Device name: UART2 */
#define BOARD_DEBUG_UART_TX_SIGNAL TX                        /*!<@brief UART2 signal: TX */
#define BOARD_DEBUG_UART_TX_PORT PORTE                       /*!<@brief PORT device name: PORTE */
#define BOARD_DEBUG_UART_TX_PIN 16U                          /*!<@brief PORTE pin index: 16 */
#define BOARD_DEBUG_UART_TX_PIN_NAME UART2_TX                /*!<@brief Pin name */
#define BOARD_DEBUG_UART_TX_LABEL "J14[44]/J20[17]/UART2_TX" /*!<@brief Label */
#define BOARD_DEBUG_UART_TX_NAME "DEBUG_UART_TX"             /*!<@brief Identifier name */
                                                             /* @} */

/*! @name PORTE17 (coord H4), J14[43]/J20[19]/UART2_RX
  @{ */
#define BOARD_DEBUG_UART_RX_PERIPHERAL UART2                 /*!<@brief Device name: UART2 */
#define BOARD_DEBUG_UART_RX_SIGNAL RX                        /*!<@brief UART2 signal: RX */
#define BOARD_DEBUG_UART_RX_PORT PORTE                       /*!<@brief PORT device name: PORTE */
#define BOARD_DEBUG_UART_RX_PIN 17U                          /*!<@brief PORTE pin index: 17 */
#define BOARD_DEBUG_UART_RX_PIN_NAME UART2_RX                /*!<@brief Pin name */
#define BOARD_DEBUG_UART_RX_LABEL "J14[43]/J20[19]/UART2_RX" /*!<@brief Label */
#define BOARD_DEBUG_UART_RX_NAME "DEBUG_UART_RX"             /*!<@brief Identifier name */
                                                             /* @} */

/*! @name PORTD4 (coord A3), J14[A78]/J14[B47]/J15[1]/D5/LED_J_PTD4
  @{ */
#define BOARD_LED_GREEN_PERIPHERAL FTM0                                /*!<@brief Device name: FTM0 */
#define BOARD_LED_GREEN_SIGNAL CH                                      /*!<@brief FTM0 signal: CH */
#define BOARD_LED_GREEN_PORT PORTD                                     /*!<@brief PORT device name: PORTD */
#define BOARD_LED_GREEN_PIN 4U                                         /*!<@brief PORTD pin index: 4 */
#define BOARD_LED_GREEN_CHANNEL 4                                      /*!<@brief FTM0 channel: 4 */
#define BOARD_LED_GREEN_PIN_NAME FTM0_CH4                              /*!<@brief Pin name */
#define BOARD_LED_GREEN_LABEL "J14[A78]/J14[B47]/J15[1]/D5/LED_J_PTD4" /*!<@brief Label */
#define BOARD_LED_GREEN_NAME "LED_GREEN"                               /*!<@brief Identifier name */
                                                                       /* @} */

/*! @name PORTD5 (coord A2), J14[A79]/J15[3]/D6/LED_J_PTD5
  @{ */
#define BOARD_LED_YELLOW_PERIPHERAL FTM0                       /*!<@brief Device name: FTM0 */
#define BOARD_LED_YELLOW_SIGNAL CH                             /*!<@brief FTM0 signal: CH */
#define BOARD_LED_YELLOW_PORT PORTD                            /*!<@brief PORT device name: PORTD */
#define BOARD_LED_YELLOW_PIN 5U                                /*!<@brief PORTD pin index: 5 */
#define BOARD_LED_YELLOW_CHANNEL 5                             /*!<@brief FTM0 channel: 5 */
#define BOARD_LED_YELLOW_PIN_NAME FTM0_CH5                     /*!<@brief Pin name */
#define BOARD_LED_YELLOW_LABEL "J14[A79]/J15[3]/D6/LED_J_PTD5" /*!<@brief Label */
#define BOARD_LED_YELLOW_NAME "LED_YELLOW"                     /*!<@brief Identifier name */
                                                               /* @} */

/*! @name PORTD6 (coord B2), J14[A80]/J14[B38]/J15[5]/D8/LED_J_PTD6
  @{ */
#define BOARD_LED_ORANGE_PERIPHERAL FTM0                                /*!<@brief Device name: FTM0 */
#define BOARD_LED_ORANGE_SIGNAL CH                                      /*!<@brief FTM0 signal: CH */
#define BOARD_LED_ORANGE_PORT PORTD                                     /*!<@brief PORT device name: PORTD */
#define BOARD_LED_ORANGE_PIN 6U                                         /*!<@brief PORTD pin index: 6 */
#define BOARD_LED_ORANGE_CHANNEL 6                                      /*!<@brief FTM0 channel: 6 */
#define BOARD_LED_ORANGE_PIN_NAME FTM0_CH6                              /*!<@brief Pin name */
#define BOARD_LED_ORANGE_LABEL "J14[A80]/J14[B38]/J15[5]/D8/LED_J_PTD6" /*!<@brief Label */
#define BOARD_LED_ORANGE_NAME "LED_ORANGE"                              /*!<@brief Identifier name */
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
