/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
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


#define SOPT5_LPUART0RXSRC_LPUART_RX 0x00u /*!<@brief LPUART 0 receive data source select: LPUART0_RX pin */
#define SOPT5_LPUART0TXSRC_LPUART_TX 0x00u /*!<@brief LPUART0 transmit data source select: LPUART0_TX pin */

/*! @name PORTB16 (number 51), J1[2]/Arduino_D0_UART0_RX/UART1_RX_TGTMCU
  @{ */
#define BOARD_DEBUG_UART_RX_PERIPHERAL LPUART0                                /*!<@brief Device name: LPUART0 */
#define BOARD_DEBUG_UART_RX_SIGNAL RX                                         /*!<@brief LPUART0 signal: RX */
#define BOARD_DEBUG_UART_RX_PORT PORTB                                        /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_RX_PIN 16U                                           /*!<@brief PORTB pin index: 16 */
#define BOARD_DEBUG_UART_RX_PIN_NAME LPUART0_RX                               /*!<@brief Pin name */
#define BOARD_DEBUG_UART_RX_LABEL "J1[2]/Arduino_D0_UART0_RX/UART1_RX_TGTMCU" /*!<@brief Label */
#define BOARD_DEBUG_UART_RX_NAME "DEBUG_UART_RX"                              /*!<@brief Identifier name */
                                                                              /* @} */

/*! @name PORTB17 (number 52), J1[4]/Arduino_D1_UART0_TX/UART1_TX_TGTMCU
  @{ */
#define BOARD_DEBUG_UART_TX_PERIPHERAL LPUART0                                /*!<@brief Device name: LPUART0 */
#define BOARD_DEBUG_UART_TX_SIGNAL TX                                         /*!<@brief LPUART0 signal: TX */
#define BOARD_DEBUG_UART_TX_PORT PORTB                                        /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_TX_PIN 17U                                           /*!<@brief PORTB pin index: 17 */
#define BOARD_DEBUG_UART_TX_PIN_NAME LPUART0_TX                               /*!<@brief Pin name */
#define BOARD_DEBUG_UART_TX_LABEL "J1[4]/Arduino_D1_UART0_TX/UART1_TX_TGTMCU" /*!<@brief Label */
#define BOARD_DEBUG_UART_TX_NAME "DEBUG_UART_TX"                              /*!<@brief Identifier name */
                                                                              /* @} */

/*! @name PORTC1 (number 56), J1[8]/Arduino_D3/Arduino_D3_LED1/LEDRGB_RED
  @{ */
#define BOARD_LED_RED_PERIPHERAL TPM0                                     /*!<@brief Device name: TPM0 */
#define BOARD_LED_RED_SIGNAL CH                                           /*!<@brief TPM0 signal: CH */
#define BOARD_LED_RED_PORT PORTC                                          /*!<@brief PORT device name: PORTC */
#define BOARD_LED_RED_PIN 1U                                              /*!<@brief PORTC pin index: 1 */
#define BOARD_LED_RED_CHANNEL 0                                           /*!<@brief TPM0 channel: 0 */
#define BOARD_LED_RED_PIN_NAME TPM0_CH0                                   /*!<@brief Pin name */
#define BOARD_LED_RED_LABEL "J1[8]/Arduino_D3/Arduino_D3_LED1/LEDRGB_RED" /*!<@brief Label */
#define BOARD_LED_RED_NAME "LED_RED"                                      /*!<@brief Identifier name */
                                                                          /* @} */

/*! @name PORTC2 (number 57), J1[12]/Arduino_D5/Arduino_D5_LED2/LEDRGB_GREEN
  @{ */
#define BOARD_LED_GREEN_PERIPHERAL TPM0                                        /*!<@brief Device name: TPM0 */
#define BOARD_LED_GREEN_SIGNAL CH                                              /*!<@brief TPM0 signal: CH */
#define BOARD_LED_GREEN_PORT PORTC                                             /*!<@brief PORT device name: PORTC */
#define BOARD_LED_GREEN_PIN 2U                                                 /*!<@brief PORTC pin index: 2 */
#define BOARD_LED_GREEN_CHANNEL 1                                              /*!<@brief TPM0 channel: 1 */
#define BOARD_LED_GREEN_PIN_NAME TPM0_CH1                                      /*!<@brief Pin name */
#define BOARD_LED_GREEN_LABEL "J1[12]/Arduino_D5/Arduino_D5_LED2/LEDRGB_GREEN" /*!<@brief Label */
#define BOARD_LED_GREEN_NAME "LED_GREEN"                                       /*!<@brief Identifier name */
                                                                               /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*! @name PORTD2 (number 75), J12[3]/U6[4]/Camera_I2C_SCL/I2C_SCL_FXOS8700CQ
  @{ */
/*!
 * @brief Device name: I2C0 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SCL_PERIPHERAL I2C0
/*!
 * @brief I2C0 signal: SCL */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SCL_SIGNAL SCL
/*!
 * @brief PORT device name: PORTD */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SCL_PORT PORTD
/*!
 * @brief PORTD pin index: 2 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SCL_PIN 2U
/*!
 * @brief Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SCL_PIN_NAME I2C0_SCL
/*!
 * @brief Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SCL_LABEL "J12[3]/U6[4]/Camera_I2C_SCL/I2C_SCL_FXOS8700CQ"
/*!
 * @brief Identifier name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SCL_NAME "ACCEL_I2C_SCL"
/* @} */

/*! @name PORTD3 (number 76), J12[4]/U6[6]/I2C0_SDA/Camera_I2C_SDA/I2C_SDA_FXOS8700CQ
  @{ */
/*!
 * @brief Device name: I2C0 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SDA_PERIPHERAL I2C0
/*!
 * @brief I2C0 signal: SDA */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SDA_SIGNAL SDA
/*!
 * @brief PORT device name: PORTD */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SDA_PORT PORTD
/*!
 * @brief PORTD pin index: 3 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SDA_PIN 3U
/*!
 * @brief Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SDA_PIN_NAME I2C0_SDA
/*!
 * @brief Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SDA_LABEL "J12[4]/U6[6]/I2C0_SDA/Camera_I2C_SDA/I2C_SDA_FXOS8700CQ"
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
