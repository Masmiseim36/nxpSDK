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
