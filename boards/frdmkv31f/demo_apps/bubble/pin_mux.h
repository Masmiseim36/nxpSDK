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


#define SOPT5_UART0TXSRC_UART_TX 0x00u /*!<@brief UART 0 transmit data source select: UART0_TX pin */

/*! @name PORTB16 (number 62), U7[4]/UART0_RX_TGTMCU
  @{ */
#define BOARD_DEBUG_UART_RX_PERIPHERAL UART0              /*!<@brief Device name: UART0 */
#define BOARD_DEBUG_UART_RX_SIGNAL RX                     /*!<@brief UART0 signal: RX */
#define BOARD_DEBUG_UART_RX_PORT PORTB                    /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_RX_PIN 16U                       /*!<@brief PORTB pin index: 16 */
#define BOARD_DEBUG_UART_RX_PIN_NAME UART0_RX             /*!<@brief Pin name */
#define BOARD_DEBUG_UART_RX_LABEL "U7[4]/UART0_RX_TGTMCU" /*!<@brief Label */
#define BOARD_DEBUG_UART_RX_NAME "DEBUG_UART_RX"          /*!<@brief Identifier name */
                                                          /* @} */

/*! @name PORTB17 (number 63), U10[1]/UART0_TX_TGTMCU
  @{ */
#define BOARD_DEBUG_UART_TX_PERIPHERAL UART0               /*!<@brief Device name: UART0 */
#define BOARD_DEBUG_UART_TX_SIGNAL TX                      /*!<@brief UART0 signal: TX */
#define BOARD_DEBUG_UART_TX_PORT PORTB                     /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_TX_PIN 17U                        /*!<@brief PORTB pin index: 17 */
#define BOARD_DEBUG_UART_TX_PIN_NAME UART0_TX              /*!<@brief Pin name */
#define BOARD_DEBUG_UART_TX_LABEL "U10[1]/UART0_TX_TGTMCU" /*!<@brief Label */
#define BOARD_DEBUG_UART_TX_NAME "DEBUG_UART_TX"           /*!<@brief Identifier name */
                                                           /* @} */

/*! @name PORTD7 (number 100), D4[4]/LEDRGB_GREEN
  @{ */
#define BOARD_LED_GREEN_PERIPHERAL FTM0            /*!<@brief Device name: FTM0 */
#define BOARD_LED_GREEN_SIGNAL CH                  /*!<@brief FTM0 signal: CH */
#define BOARD_LED_GREEN_PORT PORTD                 /*!<@brief PORT device name: PORTD */
#define BOARD_LED_GREEN_PIN 7U                     /*!<@brief PORTD pin index: 7 */
#define BOARD_LED_GREEN_CHANNEL 7                  /*!<@brief FTM0 channel: 7 */
#define BOARD_LED_GREEN_PIN_NAME FTM0_CH7          /*!<@brief Pin name */
#define BOARD_LED_GREEN_LABEL "D4[4]/LEDRGB_GREEN" /*!<@brief Label */
#define BOARD_LED_GREEN_NAME "LED_GREEN"           /*!<@brief Identifier name */
                                                   /* @} */

/*! @name PORTE25 (number 32), D4[3]/LEDRGB_BLUE
  @{ */
#define BOARD_LED_BLUE_PERIPHERAL FTM0           /*!<@brief Device name: FTM0 */
#define BOARD_LED_BLUE_SIGNAL CH                 /*!<@brief FTM0 signal: CH */
#define BOARD_LED_BLUE_PORT PORTE                /*!<@brief PORT device name: PORTE */
#define BOARD_LED_BLUE_PIN 25U                   /*!<@brief PORTE pin index: 25 */
#define BOARD_LED_BLUE_CHANNEL 1                 /*!<@brief FTM0 channel: 1 */
#define BOARD_LED_BLUE_PIN_NAME FTM0_CH1         /*!<@brief Pin name */
#define BOARD_LED_BLUE_LABEL "D4[3]/LEDRGB_BLUE" /*!<@brief Label */
#define BOARD_LED_BLUE_NAME "LED_BLUE"           /*!<@brief Identifier name */
                                                 /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

#define PORT_DFER_DFE_2_MASK 0x04u /*!<@brief Digital Filter Enable Mask for item 2. */
#define PORT_DFER_DFE_3_MASK 0x08u /*!<@brief Digital Filter Enable Mask for item 3. */

/*! @name PORTD2 (number 95), U8[4]/I2C0_SCL
  @{ */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_PERIPHERAL I2C0        /*!<@brief Device name: I2C0 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_SIGNAL SCL             /*!<@brief I2C0 signal: SCL */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_PORT PORTD             /*!<@brief PORT device name: PORTD */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_PIN 2U                 /*!<@brief PORTD pin index: 2 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_PIN_NAME I2C0_SCL      /*!<@brief Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_LABEL "U8[4]/I2C0_SCL" /*!<@brief Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_NAME "ACCEL_SCL"       /*!<@brief Identifier name */
                                                                 /* @} */

/*! @name PORTD3 (number 96), U8[6]/I2C0_SDA
  @{ */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_PERIPHERAL I2C0        /*!<@brief Device name: I2C0 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_SIGNAL SDA             /*!<@brief I2C0 signal: SDA */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_PORT PORTD             /*!<@brief PORT device name: PORTD */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_PIN 3U                 /*!<@brief PORTD pin index: 3 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_PIN_NAME I2C0_SDA      /*!<@brief Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_LABEL "U8[6]/I2C0_SDA" /*!<@brief Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_NAME "ACCEL_SDA"       /*!<@brief Identifier name */
                                                                 /* @} */

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
