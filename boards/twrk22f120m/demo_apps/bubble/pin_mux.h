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

/* PORTE1 (coord E3), UART1_RX_PKYK */
#define BOARD_INITPINS_DEBUG_UART_RX_PERIPHERAL                            UART1   /*!< Device name: UART1 */
#define BOARD_INITPINS_DEBUG_UART_RX_SIGNAL                                   RX   /*!< UART1 signal: RX */
#define BOARD_INITPINS_DEBUG_UART_RX_PIN_NAME                           UART1_RX   /*!< Pin name */
#define BOARD_INITPINS_DEBUG_UART_RX_LABEL                       "UART1_RX_PKYK"   /*!< Label */
#define BOARD_INITPINS_DEBUG_UART_RX_NAME                        "DEBUG_UART_RX"   /*!< Identifier name */

/* PORTD4 (coord A3), J16[1]/J24[B47]/J24[A78]/LED_J_PTD4 */
#define BOARD_INITPINS_LED_GREEN_PERIPHERAL                                 FTM0   /*!< Device name: FTM0 */
#define BOARD_INITPINS_LED_GREEN_SIGNAL                                       CH   /*!< FTM0 signal: CH */
#define BOARD_INITPINS_LED_GREEN_CHANNEL                                       4   /*!< FTM0 channel: 4 */
#define BOARD_INITPINS_LED_GREEN_PIN_NAME                               FTM0_CH4   /*!< Pin name */
#define BOARD_INITPINS_LED_GREEN_LABEL     "J16[1]/J24[B47]/J24[A78]/LED_J_PTD4"   /*!< Label */
#define BOARD_INITPINS_LED_GREEN_NAME                                "LED_GREEN"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/* PORTC10 (coord C6), J5[3]/J24[A75]/J24[B50]/J24[B52/J9[1]/U1[4]/I2C1_SCL */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_PERIPHERAL                        I2C1   /*!< Device name: I2C1 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_SIGNAL                             SCL   /*!< I2C1 signal: SCL */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_PIN_NAME                      I2C1_SCL   /*!< Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_LABEL "J5[3]/J24[A75]/J24[B50]/J24[B52/J9[1]/U1[4]/I2C1_SCL" /*!< Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_NAME                       "ACCEL_SCL"   /*!< Identifier name */

/* PORTC11 (coord C5), J5[4]/J7[1]/J24[A60]/J24[B51]/J24[B71]/U1[6]/I2C1_SDA */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_PERIPHERAL                        I2C1   /*!< Device name: I2C1 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_SIGNAL                             SDA   /*!< I2C1 signal: SDA */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_PIN_NAME                      I2C1_SDA   /*!< Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_LABEL "J5[4]/J7[1]/J24[A60]/J24[B51]/J24[B71]/U1[6]/I2C1_SDA" /*!< Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_NAME                       "ACCEL_SDA"   /*!< Identifier name */

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
