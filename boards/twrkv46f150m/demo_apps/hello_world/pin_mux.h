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


#define SOPT5_UART1RXSRC_UART_RX 0x00u /*!<@brief UART 1 receive data source select: UART1_RX pin */
#define SOPT5_UART1TXSRC_UART_TX 0x00u /*!<@brief UART 1 transmit data source select: UART1_TX pin */

/*! @name PORTE0 (number 1), J500[A44]/J501[5]/J505[4]/GPIOE0/TXD1
  @{ */
#define BOARD_DEBUG_UART_TX_PERIPHERAL UART1                              /*!<@brief Device name: UART1 */
#define BOARD_DEBUG_UART_TX_SIGNAL TX                                     /*!<@brief UART1 signal: TX */
#define BOARD_DEBUG_UART_TX_PORT PORTE                                    /*!<@brief PORT device name: PORTE */
#define BOARD_DEBUG_UART_TX_PIN 0U                                        /*!<@brief PORTE pin index: 0 */
#define BOARD_DEBUG_UART_TX_PIN_NAME UART1_TX                             /*!<@brief Pin name */
#define BOARD_DEBUG_UART_TX_LABEL "J500[A44]/J501[5]/J505[4]/GPIOE0/TXD1" /*!<@brief Label */
#define BOARD_DEBUG_UART_TX_NAME "DEBUG_UART_TX"                          /*!<@brief Identifier name */
                                                                          /* @} */

/*! @name PORTE1 (number 2), J500[A43]/J501[7]/J506[4]/GPIOE1/RXD1
  @{ */
#define BOARD_DEBUG_UART_RX_PERIPHERAL UART1                              /*!<@brief Device name: UART1 */
#define BOARD_DEBUG_UART_RX_SIGNAL RX                                     /*!<@brief UART1 signal: RX */
#define BOARD_DEBUG_UART_RX_PORT PORTE                                    /*!<@brief PORT device name: PORTE */
#define BOARD_DEBUG_UART_RX_PIN 1U                                        /*!<@brief PORTE pin index: 1 */
#define BOARD_DEBUG_UART_RX_PIN_NAME UART1_RX                             /*!<@brief Pin name */
#define BOARD_DEBUG_UART_RX_LABEL "J500[A43]/J501[7]/J506[4]/GPIOE1/RXD1" /*!<@brief Label */
#define BOARD_DEBUG_UART_RX_NAME "DEBUG_UART_RX"                          /*!<@brief Identifier name */
                                                                          /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

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
