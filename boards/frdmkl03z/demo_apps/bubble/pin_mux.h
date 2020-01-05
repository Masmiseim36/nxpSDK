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


#define SOPT5_LPUART0RXSRC_LPUART_RX 0x00u /*!<@brief LPUART0 Receive Data Source Select: LPUART_RX pin */

/*! @name PORTB1 (number 13), J1[2]/D1/DEBUG_UART0_TX/CMP0_IN3/UART0_RX/I2C0_SDA
  @{ */
/*!
 * @brief Device name: LPUART0 */
#define BOARD_DEBUG_UART0_TX_PERIPHERAL LPUART0
/*!
 * @brief LPUART0 signal: TX */
#define BOARD_DEBUG_UART0_TX_SIGNAL TX
/*!
 * @brief PORT device name: PORTB */
#define BOARD_DEBUG_UART0_TX_PORT PORTB
/*!
 * @brief PORTB pin index: 1 */
#define BOARD_DEBUG_UART0_TX_PIN 1U
/*!
 * @brief Pin name */
#define BOARD_DEBUG_UART0_TX_PIN_NAME LPUART0_TX
/*!
 * @brief Label */
#define BOARD_DEBUG_UART0_TX_LABEL "J1[2]/D1/DEBUG_UART0_TX/CMP0_IN3/UART0_RX/I2C0_SDA"
/*!
 * @brief Identifier name */
#define BOARD_DEBUG_UART0_TX_NAME "DEBUG_UART0_TX"
/* @} */

/*! @name PORTB2 (number 14), J1[1]/D0/DEBUG_UART0_RX/VREF_CAP/CMP_IN5/UART0_TX
  @{ */
/*!
 * @brief Device name: LPUART0 */
#define BOARD_DEBUG_UART0_RX_PERIPHERAL LPUART0
/*!
 * @brief LPUART0 signal: RX */
#define BOARD_DEBUG_UART0_RX_SIGNAL RX
/*!
 * @brief PORT device name: PORTB */
#define BOARD_DEBUG_UART0_RX_PORT PORTB
/*!
 * @brief PORTB pin index: 2 */
#define BOARD_DEBUG_UART0_RX_PIN 2U
/*!
 * @brief Pin name */
#define BOARD_DEBUG_UART0_RX_PIN_NAME LPUART0_RX
/*!
 * @brief Label */
#define BOARD_DEBUG_UART0_RX_LABEL "J1[1]/D0/DEBUG_UART0_RX/VREF_CAP/CMP_IN5/UART0_TX"
/*!
 * @brief Identifier name */
#define BOARD_DEBUG_UART0_RX_NAME "DEBUG_UART0_RX"
/* @} */

/*! @name PORTB10 (number 9), J2[1]/D8/RGB_RED/SPI0_SS
  @{ */
#define BOARD_LED_RED_PERIPHERAL TPM0                  /*!<@brief Device name: TPM0 */
#define BOARD_LED_RED_SIGNAL CH                        /*!<@brief TPM0 signal: CH */
#define BOARD_LED_RED_PORT PORTB                       /*!<@brief PORT device name: PORTB */
#define BOARD_LED_RED_PIN 10U                          /*!<@brief PORTB pin index: 10 */
#define BOARD_LED_RED_CHANNEL 1                        /*!<@brief TPM0 channel: 1 */
#define BOARD_LED_RED_PIN_NAME TPM0_CH1                /*!<@brief Pin name */
#define BOARD_LED_RED_LABEL "J2[1]/D8/RGB_RED/SPI0_SS" /*!<@brief Label */
#define BOARD_LED_RED_NAME "LED_RED"                   /*!<@brief Identifier name */
                                                       /* @} */

/*! @name PORTB11 (number 10), J2[2]/D9/RGB_GREEN/SPI0_MISO
  @{ */
#define BOARD_LED_GREEN_PERIPHERAL TPM0                      /*!<@brief Device name: TPM0 */
#define BOARD_LED_GREEN_SIGNAL CH                            /*!<@brief TPM0 signal: CH */
#define BOARD_LED_GREEN_PORT PORTB                           /*!<@brief PORT device name: PORTB */
#define BOARD_LED_GREEN_PIN 11U                              /*!<@brief PORTB pin index: 11 */
#define BOARD_LED_GREEN_CHANNEL 0                            /*!<@brief TPM0 channel: 0 */
#define BOARD_LED_GREEN_PIN_NAME TPM0_CH0                    /*!<@brief Pin name */
#define BOARD_LED_GREEN_LABEL "J2[2]/D9/RGB_GREEN/SPI0_MISO" /*!<@brief Label */
#define BOARD_LED_GREEN_NAME "LED_GREEN"                     /*!<@brief Identifier name */
                                                             /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*! @name PORTB3 (number 17), J2[10]/D15/ACCEL_I2C_SCL/UART0_TX
  @{ */
/*!
 * @brief Device name: I2C0 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SCL_PERIPHERAL I2C0
/*!
 * @brief I2C0 signal: SCL */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SCL_SIGNAL SCL
/*!
 * @brief PORT device name: PORTB */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SCL_PORT PORTB
/*!
 * @brief PORTB pin index: 3 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SCL_PIN 3U
/*!
 * @brief Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SCL_PIN_NAME I2C0_SCL
/*!
 * @brief Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SCL_LABEL "J2[10]/D15/ACCEL_I2C_SCL/UART0_TX"
/*!
 * @brief Identifier name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SCL_NAME "ACCEL_I2C_SCL"
/* @} */

/*! @name PORTB4 (number 18), J2[9]/D14/ACCEL_I2C_SDA/UART0_RX
  @{ */
/*!
 * @brief Device name: I2C0 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SDA_PERIPHERAL I2C0
/*!
 * @brief I2C0 signal: SDA */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SDA_SIGNAL SDA
/*!
 * @brief PORT device name: PORTB */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SDA_PORT PORTB
/*!
 * @brief PORTB pin index: 4 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SDA_PIN 4U
/*!
 * @brief Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SDA_PIN_NAME I2C0_SDA
/*!
 * @brief Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SDA_LABEL "J2[9]/D14/ACCEL_I2C_SDA/UART0_RX"
/*!
 * @brief Identifier name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SDA_NAME "ACCEL_I2C_SDA"
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
