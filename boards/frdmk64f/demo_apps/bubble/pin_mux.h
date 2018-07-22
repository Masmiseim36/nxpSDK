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


/* PORTB16 (number 62), U7[4]/UART0_RX */
#define BOARD_DEBUG_UART_RX_PERIPHERAL                                     UART0   /*!< Device name: UART0 */
#define BOARD_DEBUG_UART_RX_SIGNAL                                            RX   /*!< UART0 signal: RX */
#define BOARD_DEBUG_UART_RX_PIN_NAME                                    UART0_RX   /*!< Pin name */
#define BOARD_DEBUG_UART_RX_LABEL                               "U7[4]/UART0_RX"   /*!< Label */
#define BOARD_DEBUG_UART_RX_NAME                                 "DEBUG_UART_RX"   /*!< Identifier name */

/* PORTB17 (number 63), U10[1]/UART0_TX */
#define BOARD_DEBUG_UART_TX_PERIPHERAL                                     UART0   /*!< Device name: UART0 */
#define BOARD_DEBUG_UART_TX_SIGNAL                                            TX   /*!< UART0 signal: TX */
#define BOARD_DEBUG_UART_TX_PIN_NAME                                    UART0_TX   /*!< Pin name */
#define BOARD_DEBUG_UART_TX_LABEL                              "U10[1]/UART0_TX"   /*!< Label */
#define BOARD_DEBUG_UART_TX_NAME                                 "DEBUG_UART_TX"   /*!< Identifier name */

/* PORTB22 (number 68), D12[1]/LEDRGB_RED */
#define BOARD_LED_RED_GPIO                                                 GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_LED_RED_PORT                                                 PORTB   /*!< PORT device name: PORTB */
#define BOARD_LED_RED_GPIO_PIN                                               22U   /*!< PORTB pin index: 22 */
#define BOARD_LED_RED_PIN_NAME                                             PTB22   /*!< Pin name */
#define BOARD_LED_RED_LABEL                                  "D12[1]/LEDRGB_RED"   /*!< Label */
#define BOARD_LED_RED_NAME                                             "LED_RED"   /*!< Identifier name */

/* PORTE26 (number 33), J2[1]/D12[4]/LEDRGB_GREEN */
#define BOARD_LED_GREEN_GPIO                                               GPIOE   /*!< GPIO device name: GPIOE */
#define BOARD_LED_GREEN_PORT                                               PORTE   /*!< PORT device name: PORTE */
#define BOARD_LED_GREEN_GPIO_PIN                                             26U   /*!< PORTE pin index: 26 */
#define BOARD_LED_GREEN_PIN_NAME                                           PTE26   /*!< Pin name */
#define BOARD_LED_GREEN_LABEL                        "J2[1]/D12[4]/LEDRGB_GREEN"   /*!< Label */
#define BOARD_LED_GREEN_NAME                                         "LED_GREEN"   /*!< Identifier name */


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/* PORTE24 (number 31), J2[20]/U8[4]/I2C0_SCL */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_PERIPHERAL                        I2C0   /*!< Device name: I2C0 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_SIGNAL                             SCL   /*!< I2C0 signal: SCL */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_PIN_NAME                      I2C0_SCL   /*!< Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_LABEL          "J2[20]/U8[4]/I2C0_SCL"   /*!< Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_NAME                       "ACCEL_SCL"   /*!< Identifier name */

/* PORTE25 (number 32), J2[18]/U8[6]/I2C0_SDA */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_PERIPHERAL                        I2C0   /*!< Device name: I2C0 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_SIGNAL                             SDA   /*!< I2C0 signal: SDA */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_PIN_NAME                      I2C0_SDA   /*!< Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_LABEL          "J2[18]/U8[6]/I2C0_SDA"   /*!< Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_NAME                       "ACCEL_SDA"   /*!< Identifier name */


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

/*******************************************************************************
 * EOF
 ******************************************************************************/
