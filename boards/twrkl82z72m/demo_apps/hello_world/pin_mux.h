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


#define SOPT5_LPUART1RXSRC_LPUART_RX 0x00u /*!<@brief LPUART1 receive data source select: LPUART1_RX pin */
#define SOPT5_LPUART1TXSRC_LPUART_TX 0x00u /*!<@brief LPUART1 transmit data source select: LPUART1_TX pin */

/*! @name PORTC3 (coord B8), J13[A43]/J6[2]/U2[38]/LPUART1_RX/SDRAM_CLK
  @{ */
#define BOARD_DEBUG_UART_RX_PERIPHERAL LPUART1                                 /*!<@brief Device name: LPUART1 */
#define BOARD_DEBUG_UART_RX_SIGNAL RX                                          /*!<@brief LPUART1 signal: RX */
#define BOARD_DEBUG_UART_RX_PORT PORTC                                         /*!<@brief PORT device name: PORTC */
#define BOARD_DEBUG_UART_RX_PIN 3U                                             /*!<@brief PORTC pin index: 3 */
#define BOARD_DEBUG_UART_RX_PIN_NAME LPUART1_RX                                /*!<@brief Pin name */
#define BOARD_DEBUG_UART_RX_LABEL "J13[A43]/J6[2]/U2[38]/LPUART1_RX/SDRAM_CLK" /*!<@brief Label */
#define BOARD_DEBUG_UART_RX_NAME "DEBUG_UART_RX"                               /*!<@brief Identifier name */
                                                                               /* @} */

/*! @name PORTC4 (coord A8), J13[A44]/J13[A69]/J8[2]/U2[22]/LPUART1_TX/SDRAM_A19
  @{ */
/*!
 * @brief Device name: LPUART1 */
#define BOARD_DEBUG_UART_TX_PERIPHERAL LPUART1
/*!
 * @brief LPUART1 signal: TX */
#define BOARD_DEBUG_UART_TX_SIGNAL TX
/*!
 * @brief PORT device name: PORTC */
#define BOARD_DEBUG_UART_TX_PORT PORTC
/*!
 * @brief PORTC pin index: 4 */
#define BOARD_DEBUG_UART_TX_PIN 4U
/*!
 * @brief Pin name */
#define BOARD_DEBUG_UART_TX_PIN_NAME LPUART1_TX
/*!
 * @brief Label */
#define BOARD_DEBUG_UART_TX_LABEL "J13[A44]/J13[A69]/J8[2]/U2[22]/LPUART1_TX/SDRAM_A19"
/*!
 * @brief Identifier name */
#define BOARD_DEBUG_UART_TX_NAME "DEBUG_UART_TX"
/* @} */

/*!
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
