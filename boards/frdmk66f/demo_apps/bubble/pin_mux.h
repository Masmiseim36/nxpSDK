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

/*! @name PORTB16 (coord E10), U7[4]/UART0_RX
  @{ */
#define BOARD_DEBUG_UART_RX_PERIPHERAL UART0       /*!<@brief Device name: UART0 */
#define BOARD_DEBUG_UART_RX_SIGNAL RX              /*!<@brief UART0 signal: RX */
#define BOARD_DEBUG_UART_RX_PORT PORTB             /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_RX_PIN 16U                /*!<@brief PORTB pin index: 16 */
#define BOARD_DEBUG_UART_RX_PIN_NAME UART0_RX      /*!<@brief Pin name */
#define BOARD_DEBUG_UART_RX_LABEL "U7[4]/UART0_RX" /*!<@brief Label */
#define BOARD_DEBUG_UART_RX_NAME "DEBUG_UART_RX"   /*!<@brief Identifier name */
                                                   /* @} */

/*! @name PORTB17 (coord E9), U10[1]/UART0_TX
  @{ */
#define BOARD_DEBUG_UART_TX_PERIPHERAL UART0        /*!<@brief Device name: UART0 */
#define BOARD_DEBUG_UART_TX_SIGNAL TX               /*!<@brief UART0 signal: TX */
#define BOARD_DEBUG_UART_TX_PORT PORTB              /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_TX_PIN 17U                 /*!<@brief PORTB pin index: 17 */
#define BOARD_DEBUG_UART_TX_PIN_NAME UART0_TX       /*!<@brief Pin name */
#define BOARD_DEBUG_UART_TX_LABEL "U10[1]/UART0_TX" /*!<@brief Label */
#define BOARD_DEBUG_UART_TX_NAME "DEBUG_UART_TX"    /*!<@brief Identifier name */
                                                    /* @} */

/*! @name PORTC9 (coord D7), LEDRGB_RED
  @{ */
#define BOARD_LED_RED_PERIPHERAL FTM3    /*!<@brief Device name: FTM3 */
#define BOARD_LED_RED_SIGNAL CH          /*!<@brief FTM3 signal: CH */
#define BOARD_LED_RED_PORT PORTC         /*!<@brief PORT device name: PORTC */
#define BOARD_LED_RED_PIN 9U             /*!<@brief PORTC pin index: 9 */
#define BOARD_LED_RED_CHANNEL 5          /*!<@brief FTM3 channel: 5 */
#define BOARD_LED_RED_PIN_NAME FTM3_CH5  /*!<@brief Pin name */
#define BOARD_LED_RED_LABEL "LEDRGB_RED" /*!<@brief Label */
#define BOARD_LED_RED_NAME "LED_RED"     /*!<@brief Identifier name */
                                         /* @} */

/*! @name PORTE6 (coord E1), LEDRGB_GREEN
  @{ */
#define BOARD_LED_GREEN_PERIPHERAL FTM3      /*!<@brief Device name: FTM3 */
#define BOARD_LED_GREEN_SIGNAL CH            /*!<@brief FTM3 signal: CH */
#define BOARD_LED_GREEN_PORT PORTE           /*!<@brief PORT device name: PORTE */
#define BOARD_LED_GREEN_PIN 6U               /*!<@brief PORTE pin index: 6 */
#define BOARD_LED_GREEN_CHANNEL 1            /*!<@brief FTM3 channel: 1 */
#define BOARD_LED_GREEN_PIN_NAME FTM3_CH1    /*!<@brief Pin name */
#define BOARD_LED_GREEN_LABEL "LEDRGB_GREEN" /*!<@brief Label */
#define BOARD_LED_GREEN_NAME "LED_GREEN"     /*!<@brief Identifier name */
                                             /* @} */

/*! @name PORTD8 (coord C9), U8[4]/U19[11]/I2C0_SCL
  @{ */
#define BOARD_GYRO_SCL_PERIPHERAL I2C0                /*!<@brief Device name: I2C0 */
#define BOARD_GYRO_SCL_SIGNAL SCL                     /*!<@brief I2C0 signal: SCL */
#define BOARD_GYRO_SCL_PORT PORTD                     /*!<@brief PORT device name: PORTD */
#define BOARD_GYRO_SCL_PIN 8U                         /*!<@brief PORTD pin index: 8 */
#define BOARD_GYRO_SCL_PIN_NAME I2C0_SCL              /*!<@brief Pin name */
#define BOARD_GYRO_SCL_LABEL "U8[4]/U19[11]/I2C0_SCL" /*!<@brief Label */
#define BOARD_GYRO_SCL_NAME "GYRO_SCL"                /*!<@brief Identifier name */
                                                      /* @} */

/*! @name PORTD9 (coord B9), U8[6]/U19[12]/I2C0_SDA
  @{ */
#define BOARD_GYRO_SDA_PERIPHERAL I2C0                /*!<@brief Device name: I2C0 */
#define BOARD_GYRO_SDA_SIGNAL SDA                     /*!<@brief I2C0 signal: SDA */
#define BOARD_GYRO_SDA_PORT PORTD                     /*!<@brief PORT device name: PORTD */
#define BOARD_GYRO_SDA_PIN 9U                         /*!<@brief PORTD pin index: 9 */
#define BOARD_GYRO_SDA_PIN_NAME I2C0_SDA              /*!<@brief Pin name */
#define BOARD_GYRO_SDA_LABEL "U8[6]/U19[12]/I2C0_SDA" /*!<@brief Label */
#define BOARD_GYRO_SDA_NAME "GYRO_SDA"                /*!<@brief Identifier name */
                                                      /* @} */

/*!
 *
 */
void BOARD_InitPins(void);

#define PORT_DFER_DFE_8_MASK 0x0100u /*!<@brief Digital Filter Enable Mask for item 8. */
#define PORT_DFER_DFE_9_MASK 0x0200u /*!<@brief Digital Filter Enable Mask for item 9. */

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
