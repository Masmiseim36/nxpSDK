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


/*!
 * @brief LPUART0 receive data source select: LPUART0_RX pin */
#define SOPT5_LPUART0RXSRC_LPUART_RX 0x00u
/*!
 * @brief LPUART0 transmit data source select: LPUART0_TX pin */
#define SOPT5_LPUART0TXSRC_LPUART_TX 0x00u
/*!
 * @brief FTM3 channel 1 output source: FTM3_CH1 pin is output of FTM3 channel 1 output */
#define SOPT8_FTM3OCH1SRC_FTM 0x00u
/*!
 * @brief FTM3 channel 2 output source: FTM3_CH2 pin is output of FTM3 channel 2 output */
#define SOPT8_FTM3OCH2SRC_FTM 0x00u

/*! @name PORTC25 (coord A7), LPUART0_RX_TGTMCU
  @{ */
#define BOARD_DEBUG_UART_RX_PERIPHERAL LPUART0        /*!<@brief Device name: LPUART0 */
#define BOARD_DEBUG_UART_RX_SIGNAL RX                 /*!<@brief LPUART0 signal: RX */
#define BOARD_DEBUG_UART_RX_PORT PORTC                /*!<@brief PORT device name: PORTC */
#define BOARD_DEBUG_UART_RX_PIN 25U                   /*!<@brief PORTC pin index: 25 */
#define BOARD_DEBUG_UART_RX_PIN_NAME LPUART0_RX       /*!<@brief Pin name */
#define BOARD_DEBUG_UART_RX_LABEL "LPUART0_RX_TGTMCU" /*!<@brief Label */
#define BOARD_DEBUG_UART_RX_NAME "DEBUG_UART_RX"      /*!<@brief Identifier name */
                                                      /* @} */

/*! @name PORTC24 (coord B7), LPUART0_TX_TGTMCU
  @{ */
#define BOARD_DEBUG_UART_TX_PERIPHERAL LPUART0        /*!<@brief Device name: LPUART0 */
#define BOARD_DEBUG_UART_TX_SIGNAL TX                 /*!<@brief LPUART0 signal: TX */
#define BOARD_DEBUG_UART_TX_PORT PORTC                /*!<@brief PORT device name: PORTC */
#define BOARD_DEBUG_UART_TX_PIN 24U                   /*!<@brief PORTC pin index: 24 */
#define BOARD_DEBUG_UART_TX_PIN_NAME LPUART0_TX       /*!<@brief Pin name */
#define BOARD_DEBUG_UART_TX_LABEL "LPUART0_TX_TGTMCU" /*!<@brief Label */
#define BOARD_DEBUG_UART_TX_NAME "DEBUG_UART_TX"      /*!<@brief Identifier name */
                                                      /* @} */

/*! @name PORTE7 (coord E3), J1[15]/D1[4]/LEDRGB_GREEN
  @{ */
#define BOARD_LED_GREEN_PERIPHERAL FTM3                   /*!<@brief Device name: FTM3 */
#define BOARD_LED_GREEN_SIGNAL CH                         /*!<@brief FTM3 signal: CH */
#define BOARD_LED_GREEN_PORT PORTE                        /*!<@brief PORT device name: PORTE */
#define BOARD_LED_GREEN_PIN 7U                            /*!<@brief PORTE pin index: 7 */
#define BOARD_LED_GREEN_CHANNEL 2                         /*!<@brief FTM3 channel: 2 */
#define BOARD_LED_GREEN_PIN_NAME FTM3_CH2                 /*!<@brief Pin name */
#define BOARD_LED_GREEN_LABEL "J1[15]/D1[4]/LEDRGB_GREEN" /*!<@brief Label */
#define BOARD_LED_GREEN_NAME "LED_GREEN"                  /*!<@brief Identifier name */
                                                          /* @} */

/*! @name PORTE6 (coord E2), J1[7]/D1[1]/LEDRGB_RED
  @{ */
#define BOARD_LED_RED_PERIPHERAL FTM3                /*!<@brief Device name: FTM3 */
#define BOARD_LED_RED_SIGNAL CH                      /*!<@brief FTM3 signal: CH */
#define BOARD_LED_RED_PORT PORTE                     /*!<@brief PORT device name: PORTE */
#define BOARD_LED_RED_PIN 6U                         /*!<@brief PORTE pin index: 6 */
#define BOARD_LED_RED_CHANNEL 1                      /*!<@brief FTM3 channel: 1 */
#define BOARD_LED_RED_PIN_NAME FTM3_CH1              /*!<@brief Pin name */
#define BOARD_LED_RED_LABEL "J1[7]/D1[1]/LEDRGB_RED" /*!<@brief Label */
#define BOARD_LED_RED_NAME "LED_RED"                 /*!<@brief Identifier name */
                                                     /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*! @name PORTC29 (coord B6), U6[4]/I2C_SCL
  @{ */
#define BOARD_I2C_CONFIGUREPINS_I2C_SCL_PERIPHERAL I2C3       /*!<@brief Device name: I2C3 */
#define BOARD_I2C_CONFIGUREPINS_I2C_SCL_SIGNAL SCL            /*!<@brief I2C3 signal: SCL */
#define BOARD_I2C_CONFIGUREPINS_I2C_SCL_PORT PORTC            /*!<@brief PORT device name: PORTC */
#define BOARD_I2C_CONFIGUREPINS_I2C_SCL_PIN 29U               /*!<@brief PORTC pin index: 29 */
#define BOARD_I2C_CONFIGUREPINS_I2C_SCL_PIN_NAME I2C3_SCL     /*!<@brief Pin name */
#define BOARD_I2C_CONFIGUREPINS_I2C_SCL_LABEL "U6[4]/I2C_SCL" /*!<@brief Label */
#define BOARD_I2C_CONFIGUREPINS_I2C_SCL_NAME "I2C_SCL"        /*!<@brief Identifier name */
                                                              /* @} */

/*! @name PORTC28 (coord C6), U6[6]/I2C SDA
  @{ */
#define BOARD_I2C_CONFIGUREPINS_I2C_SDA_PERIPHERAL I2C3       /*!<@brief Device name: I2C3 */
#define BOARD_I2C_CONFIGUREPINS_I2C_SDA_SIGNAL SDA            /*!<@brief I2C3 signal: SDA */
#define BOARD_I2C_CONFIGUREPINS_I2C_SDA_PORT PORTC            /*!<@brief PORT device name: PORTC */
#define BOARD_I2C_CONFIGUREPINS_I2C_SDA_PIN 28U               /*!<@brief PORTC pin index: 28 */
#define BOARD_I2C_CONFIGUREPINS_I2C_SDA_PIN_NAME I2C3_SDA     /*!<@brief Pin name */
#define BOARD_I2C_CONFIGUREPINS_I2C_SDA_LABEL "U6[6]/I2C SDA" /*!<@brief Label */
#define BOARD_I2C_CONFIGUREPINS_I2C_SDA_NAME "I2C_SDA"        /*!<@brief Identifier name */
                                                              /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
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
