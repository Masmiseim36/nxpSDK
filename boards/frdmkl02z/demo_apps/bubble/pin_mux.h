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


#define SOPT4_TPM1CH0SRC_TPM1 0x00u    /*!<@brief TPM1 channel 0 input capture source select: TPM1_CH0 signal */
#define SOPT5_UART0RXSRC_UART_RX 0x00u /*!<@brief UART0 receive data source select: UART0_RX pin */
#define SOPT5_UART0TXSRC_UART_TX 0x00u /*!<@brief UART0 transmit data source select: UART0_TX pin */

/*! @name PORTB1 (number 17), J8[2]/ADC0_SE5/CMP0_IN3/UART0_TX/UART0_RX
  @{ */
#define BOARD_DEBUG_UART0_TX_PERIPHERAL UART0                                  /*!<@brief Device name: UART0 */
#define BOARD_DEBUG_UART0_TX_SIGNAL TX                                         /*!<@brief UART0 signal: TX */
#define BOARD_DEBUG_UART0_TX_PORT PORTB                                        /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART0_TX_PIN 1U                                            /*!<@brief PORTB pin index: 1 */
#define BOARD_DEBUG_UART0_TX_PIN_NAME UART0_TX                                 /*!<@brief Pin name */
#define BOARD_DEBUG_UART0_TX_LABEL "J8[2]/ADC0_SE5/CMP0_IN3/UART0_TX/UART0_RX" /*!<@brief Label */
#define BOARD_DEBUG_UART0_TX_NAME "DEBUG_UART0_TX"                             /*!<@brief Identifier name */
                                                                               /* @} */

/*! @name PORTB2 (number 18), J8[1]/ADC0_SE4/UART0_RX/UART0_TX
  @{ */
#define BOARD_DEBUG_UART0_RX_PERIPHERAL UART0                         /*!<@brief Device name: UART0 */
#define BOARD_DEBUG_UART0_RX_SIGNAL RX                                /*!<@brief UART0 signal: RX */
#define BOARD_DEBUG_UART0_RX_PORT PORTB                               /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART0_RX_PIN 2U                                   /*!<@brief PORTB pin index: 2 */
#define BOARD_DEBUG_UART0_RX_PIN_NAME UART0_RX                        /*!<@brief Pin name */
#define BOARD_DEBUG_UART0_RX_LABEL "J8[1]/ADC0_SE4/UART0_RX/UART0_TX" /*!<@brief Label */
#define BOARD_DEBUG_UART0_RX_NAME "DEBUG_UART0_RX"                    /*!<@brief Identifier name */
                                                                      /* @} */

/*! @name PORTB6 (number 1), J8[7]/LEDRGB_RED/LPTMR0_ALT3
  @{ */
#define BOARD_LED_RED_PERIPHERAL TPM1                      /*!<@brief Device name: TPM1 */
#define BOARD_LED_RED_SIGNAL CH                            /*!<@brief TPM1 signal: CH */
#define BOARD_LED_RED_PORT PORTB                           /*!<@brief PORT device name: PORTB */
#define BOARD_LED_RED_PIN 6U                               /*!<@brief PORTB pin index: 6 */
#define BOARD_LED_RED_CHANNEL 1                            /*!<@brief TPM1 channel: 1 */
#define BOARD_LED_RED_PIN_NAME TPM1_CH1                    /*!<@brief Pin name */
#define BOARD_LED_RED_LABEL "J8[7]/LEDRGB_RED/LPTMR0_ALT3" /*!<@brief Label */
#define BOARD_LED_RED_NAME "LED_RED"                       /*!<@brief Identifier name */
                                                           /* @} */

/*! @name PORTB7 (number 2), J8[8]/LEDRGB_GREEN
  @{ */
#define BOARD_LED_GREEN_PERIPHERAL TPM1            /*!<@brief Device name: TPM1 */
#define BOARD_LED_GREEN_SIGNAL CH                  /*!<@brief TPM1 signal: CH */
#define BOARD_LED_GREEN_PORT PORTB                 /*!<@brief PORT device name: PORTB */
#define BOARD_LED_GREEN_PIN 7U                     /*!<@brief PORTB pin index: 7 */
#define BOARD_LED_GREEN_CHANNEL 0                  /*!<@brief TPM1 channel: 0 */
#define BOARD_LED_GREEN_PIN_NAME TPM1_CH0          /*!<@brief Pin name */
#define BOARD_LED_GREEN_LABEL "J8[8]/LEDRGB_GREEN" /*!<@brief Label */
#define BOARD_LED_GREEN_NAME "LED_GREEN"           /*!<@brief Identifier name */
                                                   /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*! @name PORTB3 (number 23), J7[10]/I2C0_SCL/UART0_TX
  @{ */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_PERIPHERAL I2C0                  /*!<@brief Device name: I2C0 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_SIGNAL SCL                       /*!<@brief I2C0 signal: SCL */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_PORT PORTB                       /*!<@brief PORT device name: PORTB */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_PIN 3U                           /*!<@brief PORTB pin index: 3 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_PIN_NAME I2C0_SCL                /*!<@brief Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_LABEL "J7[10]/I2C0_SCL/UART0_TX" /*!<@brief Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_NAME "ACCEL_SCL"                 /*!<@brief Identifier name */
                                                                           /* @} */

/*! @name PORTB4 (number 24), J7[9]/I2C0_SDA/UART0_RX
  @{ */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_PERIPHERAL I2C0                 /*!<@brief Device name: I2C0 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_SIGNAL SDA                      /*!<@brief I2C0 signal: SDA */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_PORT PORTB                      /*!<@brief PORT device name: PORTB */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_PIN 4U                          /*!<@brief PORTB pin index: 4 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_PIN_NAME I2C0_SDA               /*!<@brief Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_LABEL "J7[9]/I2C0_SDA/UART0_RX" /*!<@brief Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_NAME "ACCEL_SDA"                /*!<@brief Identifier name */
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
