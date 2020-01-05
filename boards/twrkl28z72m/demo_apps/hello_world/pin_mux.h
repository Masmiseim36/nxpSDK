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


/*! @name PORTA1 (number 35), U11[3]/UART0_RX
  @{ */
#define BOARD_DEBUG_UART_RX_PERIPHERAL LPUART0      /*!<@brief Device name: LPUART0 */
#define BOARD_DEBUG_UART_RX_SIGNAL RX               /*!<@brief LPUART0 signal: RX */
#define BOARD_DEBUG_UART_RX_PORT PORTA              /*!<@brief PORT device name: PORTA */
#define BOARD_DEBUG_UART_RX_PIN 1U                  /*!<@brief PORTA pin index: 1 */
#define BOARD_DEBUG_UART_RX_PIN_NAME LPUART0_RX     /*!<@brief Pin name */
#define BOARD_DEBUG_UART_RX_LABEL "U11[3]/UART0_RX" /*!<@brief Label */
#define BOARD_DEBUG_UART_RX_NAME "DEBUG_UART_RX"    /*!<@brief Identifier name */
                                                    /* @} */

/*! @name PORTA2 (number 36), U12[4]/UART0_TX
  @{ */
#define BOARD_DEBUG_UART_TX_PERIPHERAL LPUART0      /*!<@brief Device name: LPUART0 */
#define BOARD_DEBUG_UART_TX_SIGNAL TX               /*!<@brief LPUART0 signal: TX */
#define BOARD_DEBUG_UART_TX_PORT PORTA              /*!<@brief PORT device name: PORTA */
#define BOARD_DEBUG_UART_TX_PIN 2U                  /*!<@brief PORTA pin index: 2 */
#define BOARD_DEBUG_UART_TX_PIN_NAME LPUART0_TX     /*!<@brief Pin name */
#define BOARD_DEBUG_UART_TX_LABEL "U12[4]/UART0_TX" /*!<@brief Label */
#define BOARD_DEBUG_UART_TX_NAME "DEBUG_UART_TX"    /*!<@brief Identifier name */
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
