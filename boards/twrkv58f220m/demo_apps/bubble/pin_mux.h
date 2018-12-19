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


/*!
 * @brief UART 0 receive data source select: UART0_RX pin */
#define SOPT5_UART0RXSRC_UART_RX 0x00u
/*!
 * @brief UART 0 transmit data source select: UART0_TX pin */
#define SOPT5_UART0TXSRC_UART_TX 0x00u
/*!
 * @brief FTM3 channel 6 output source: FTM3_CH6 pin is output of FTM3 channel 6 output */
#define SOPT8_FTM3OCH6SRC_FTM 0x00u
/*!
 * @brief FTM3 channel 7 output source: FTM3_CH7 pin is output of FTM3 channel 7 output */
#define SOPT8_FTM3OCH7SRC_FTM 0x00u

/*! @name PORTB0 (number 81), J15[A43]/J31[7]/UART0_RX
  @{ */
#define BOARD_DEBUG_UART_RX_PERIPHERAL UART0                 /*!<@brief Device name: UART0 */
#define BOARD_DEBUG_UART_RX_SIGNAL RX                        /*!<@brief UART0 signal: RX */
#define BOARD_DEBUG_UART_RX_PORT PORTB                       /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_RX_PIN 0U                           /*!<@brief PORTB pin index: 0 */
#define BOARD_DEBUG_UART_RX_PIN_NAME UART0_RX                /*!<@brief Pin name */
#define BOARD_DEBUG_UART_RX_LABEL "J15[A43]/J31[7]/UART0_RX" /*!<@brief Label */
#define BOARD_DEBUG_UART_RX_NAME "DEBUG_UART_RX"             /*!<@brief Identifier name */
                                                             /* @} */

/*! @name PORTB1 (number 82), J15[A44]/J31[5]/UART0_TX
  @{ */
#define BOARD_DEBUG_UART_TX_PERIPHERAL UART0                 /*!<@brief Device name: UART0 */
#define BOARD_DEBUG_UART_TX_SIGNAL TX                        /*!<@brief UART0 signal: TX */
#define BOARD_DEBUG_UART_TX_PORT PORTB                       /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_TX_PIN 1U                           /*!<@brief PORTB pin index: 1 */
#define BOARD_DEBUG_UART_TX_PIN_NAME UART0_TX                /*!<@brief Pin name */
#define BOARD_DEBUG_UART_TX_LABEL "J15[A44]/J31[5]/UART0_TX" /*!<@brief Label */
#define BOARD_DEBUG_UART_TX_NAME "DEBUG_UART_TX"             /*!<@brief Identifier name */
                                                             /* @} */

/*! @name PORTE11 (number 14), J30[1]/LED_J_PTE11
  @{ */
#define BOARD_LED_RED_PERIPHERAL FTM3                    /*!<@brief Device name: FTM3 */
#define BOARD_LED_RED_SIGNAL CH                          /*!<@brief FTM3 signal: CH */
#define BOARD_LED_RED_PORT PORTE                         /*!<@brief PORT device name: PORTE */
#define BOARD_LED_RED_PIN 11U                            /*!<@brief PORTE pin index: 11 */
#define BOARD_LED_RED_CHANNEL 6                          /*!<@brief FTM3 channel: 6 */
#define BOARD_LED_RED_PIN_NAME FTM3_CH6                  /*!<@brief Pin name */
#define BOARD_LED_RED_LABEL "J30[1]/LED_J_PTE11"         /*!<@brief Label */
#define BOARD_LED_RED_NAME "LED_RED"                     /*!<@brief Identifier name */
#define BOARD_LED_RED_DIRECTION kPIN_MUX_DirectionOutput /*!<@brief Direction */
                                                         /* @} */

/*! @name PORTE12 (number 15), J30[3]/LED_J_PTE12
  @{ */
#define BOARD_LED_GREEN_PERIPHERAL FTM3                    /*!<@brief Device name: FTM3 */
#define BOARD_LED_GREEN_SIGNAL CH                          /*!<@brief FTM3 signal: CH */
#define BOARD_LED_GREEN_PORT PORTE                         /*!<@brief PORT device name: PORTE */
#define BOARD_LED_GREEN_PIN 12U                            /*!<@brief PORTE pin index: 12 */
#define BOARD_LED_GREEN_CHANNEL 7                          /*!<@brief FTM3 channel: 7 */
#define BOARD_LED_GREEN_PIN_NAME FTM3_CH7                  /*!<@brief Pin name */
#define BOARD_LED_GREEN_LABEL "J30[3]/LED_J_PTE12"         /*!<@brief Label */
#define BOARD_LED_GREEN_NAME "LED_GREEN"                   /*!<@brief Identifier name */
#define BOARD_LED_GREEN_DIRECTION kPIN_MUX_DirectionOutput /*!<@brief Direction */
                                                           /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

#define PORT_DFER_DFE_8_MASK 0x0100u /*!<@brief Digital Filter Enable Mask for item 8. */
#define PORT_DFER_DFE_9_MASK 0x0200u /*!<@brief Digital Filter Enable Mask for item 9. */

/*! @name PORTD8 (number 137), J15[A7]/U1[4]/I2C1_SCL
  @{ */
#define BOARD_I2C_CONFIGUREPINS_I2C_SCL_PERIPHERAL I2C1                /*!<@brief Device name: I2C1 */
#define BOARD_I2C_CONFIGUREPINS_I2C_SCL_SIGNAL SCL                     /*!<@brief I2C1 signal: SCL */
#define BOARD_I2C_CONFIGUREPINS_I2C_SCL_PORT PORTD                     /*!<@brief PORT device name: PORTD */
#define BOARD_I2C_CONFIGUREPINS_I2C_SCL_PIN 8U                         /*!<@brief PORTD pin index: 8 */
#define BOARD_I2C_CONFIGUREPINS_I2C_SCL_PIN_NAME I2C1_SCL              /*!<@brief Pin name */
#define BOARD_I2C_CONFIGUREPINS_I2C_SCL_LABEL "J15[A7]/U1[4]/I2C1_SCL" /*!<@brief Label */
#define BOARD_I2C_CONFIGUREPINS_I2C_SCL_NAME "I2C_SCL"                 /*!<@brief Identifier name */
                                                                       /* @} */

/*! @name PORTD9 (number 138), J15[A8]/U1[6]/I2C1_SDA
  @{ */
#define BOARD_I2C_CONFIGUREPINS_I2C_SDA_PERIPHERAL I2C1                /*!<@brief Device name: I2C1 */
#define BOARD_I2C_CONFIGUREPINS_I2C_SDA_SIGNAL SDA                     /*!<@brief I2C1 signal: SDA */
#define BOARD_I2C_CONFIGUREPINS_I2C_SDA_PORT PORTD                     /*!<@brief PORT device name: PORTD */
#define BOARD_I2C_CONFIGUREPINS_I2C_SDA_PIN 9U                         /*!<@brief PORTD pin index: 9 */
#define BOARD_I2C_CONFIGUREPINS_I2C_SDA_PIN_NAME I2C1_SDA              /*!<@brief Pin name */
#define BOARD_I2C_CONFIGUREPINS_I2C_SDA_LABEL "J15[A8]/U1[6]/I2C1_SDA" /*!<@brief Label */
#define BOARD_I2C_CONFIGUREPINS_I2C_SDA_NAME "I2C_SDA"                 /*!<@brief Identifier name */
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
