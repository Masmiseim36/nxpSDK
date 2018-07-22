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

/*! @name PORTA2 (number 24), J1[8]/GREEN_LED
  @{ */
#define BOARD_LEDRGB_GREEN_PERIPHERAL GPIOA        /*!<@brief Device name: GPIOA */
#define BOARD_LEDRGB_GREEN_SIGNAL GPIO             /*!<@brief GPIOA signal: GPIO */
#define BOARD_LEDRGB_GREEN_GPIO GPIOA              /*!<@brief GPIO device name: GPIOA */
#define BOARD_LEDRGB_GREEN_GPIO_PIN 2U             /*!<@brief PORTA pin index: 2 */
#define BOARD_LEDRGB_GREEN_PORT PORTA              /*!<@brief PORT device name: PORTA */
#define BOARD_LEDRGB_GREEN_PIN 2U                  /*!<@brief PORTA pin index: 2 */
#define BOARD_LEDRGB_GREEN_CHANNEL 2               /*!<@brief GPIOA GPIO channel: 2 */
#define BOARD_LEDRGB_GREEN_PIN_NAME PTA2           /*!<@brief Pin name */
#define BOARD_LEDRGB_GREEN_LABEL "J1[8]/GREEN_LED" /*!<@brief Label */
#define BOARD_LEDRGB_GREEN_NAME "LEDRGB_GREEN"     /*!<@brief Identifier name */
                                                   /* @} */

/*! @name PORTA1 (number 23), J2[4]/RED_LED
  @{ */
#define BOARD_LEDRGB_RED_PERIPHERAL FTM0       /*!<@brief Device name: FTM0 */
#define BOARD_LEDRGB_RED_SIGNAL CH             /*!<@brief FTM0 signal: CH */
#define BOARD_LEDRGB_RED_PORT PORTA            /*!<@brief PORT device name: PORTA */
#define BOARD_LEDRGB_RED_PIN 1U                /*!<@brief PORTA pin index: 1 */
#define BOARD_LEDRGB_RED_CHANNEL 6             /*!<@brief FTM0 channel: 6 */
#define BOARD_LEDRGB_RED_PIN_NAME FTM0_CH6     /*!<@brief Pin name */
#define BOARD_LEDRGB_RED_LABEL "J2[4]/RED_LED" /*!<@brief Label */
#define BOARD_LEDRGB_RED_NAME "LEDRGB_RED"     /*!<@brief Identifier name */
                                               /* @} */

/*! @name PORTD5 (number 62), J2[12]/BLUE_LED
  @{ */
#define BOARD_LEDRGB_BLUE_PERIPHERAL FTM0         /*!<@brief Device name: FTM0 */
#define BOARD_LEDRGB_BLUE_SIGNAL CH               /*!<@brief FTM0 signal: CH */
#define BOARD_LEDRGB_BLUE_PORT PORTD              /*!<@brief PORT device name: PORTD */
#define BOARD_LEDRGB_BLUE_PIN 5U                  /*!<@brief PORTD pin index: 5 */
#define BOARD_LEDRGB_BLUE_CHANNEL 5               /*!<@brief FTM0 channel: 5 */
#define BOARD_LEDRGB_BLUE_PIN_NAME FTM0_CH5       /*!<@brief Pin name */
#define BOARD_LEDRGB_BLUE_LABEL "J2[12]/BLUE_LED" /*!<@brief Label */
#define BOARD_LEDRGB_BLUE_NAME "LEDRGB_BLUE"      /*!<@brief Identifier name */
                                                  /* @} */

/*!
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
