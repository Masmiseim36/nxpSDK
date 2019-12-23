/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Direction type  */
typedef enum _pin_mux_direction
{
  kPIN_MUX_DirectionInput = 0U,         /* Input direction */
  kPIN_MUX_DirectionOutput = 1U,        /* Output direction */
  kPIN_MUX_DirectionInputOrOutput = 2U  /* Input or output direction */
} pin_mux_direction_t;

/*!
 * @addtogroup pin_mux
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/* PORTB16 (number 31), J15[7]/J21[2]/J6[9]/RXD0 */
#define BOARD_INITPINS_DEBUG_UART_RX_PERIPHERAL                            UART0   /*!< Device name: UART0 */
#define BOARD_INITPINS_DEBUG_UART_RX_SIGNAL                                   RX   /*!< UART0 signal: RX */
#define BOARD_INITPINS_DEBUG_UART_RX_PIN_NAME                           UART0_RX   /*!< Pin name */
#define BOARD_INITPINS_DEBUG_UART_RX_LABEL            "J15[7]/J21[2]/J6[9]/RXD0"   /*!< Label */
#define BOARD_INITPINS_DEBUG_UART_RX_NAME                        "DEBUG_UART_RX"   /*!< Identifier name */

/* PORTB17 (number 32), J15[5]/J22[2]/J6[10]/TXD0 */
#define BOARD_INITPINS_DEBUG_UART_TX_PERIPHERAL                            UART0   /*!< Device name: UART0 */
#define BOARD_INITPINS_DEBUG_UART_TX_SIGNAL                                   TX   /*!< UART0 signal: TX */
#define BOARD_INITPINS_DEBUG_UART_TX_PIN_NAME                           UART0_TX   /*!< Pin name */
#define BOARD_INITPINS_DEBUG_UART_TX_LABEL           "J15[5]/J22[2]/J6[10]/TXD0"   /*!< Label */
#define BOARD_INITPINS_DEBUG_UART_TX_NAME                        "DEBUG_UART_TX"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/* PORTC6 (number 39), J6[19]/J9[2]/J15[23]/J24[A7]/SCL0 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C0_SCL_PERIPHERAL                   I2C0   /*!< Device name: I2C0 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C0_SCL_SIGNAL                        SCL   /*!< I2C0 signal: SCL */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C0_SCL_PIN_NAME                 I2C0_SCL   /*!< Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C0_SCL_LABEL "J6[19]/J9[2]/J15[23]/J24[A7]/SCL0" /*!< Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C0_SCL_NAME             "ACCEL_I2C0_SCL"   /*!< Identifier name */

/* PORTC7 (number 40), J15[21]/J6[20]/J7[2]/J24[A8]/SDA0 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C0_SDA_PERIPHERAL                   I2C0   /*!< Device name: I2C0 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C0_SDA_SIGNAL                        SDA   /*!< I2C0 signal: SDA */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C0_SDA_PIN_NAME                 I2C0_SDA   /*!< Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C0_SDA_LABEL "J15[21]/J6[20]/J7[2]/J24[A8]/SDA0" /*!< Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C0_SDA_NAME             "ACCEL_I2C0_SDA"   /*!< Identifier name */

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

/*******************************************************************************
 * EOF
 ******************************************************************************/
