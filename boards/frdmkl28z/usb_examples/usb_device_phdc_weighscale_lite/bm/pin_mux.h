/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
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



/* PORTB16 (number 62), J1[2]/U7[25]/D0/UART0_RX/FXIO_D16/UART1_RX_TGTMCU */
#define BOARD_INITPINS_DEBUG_UART_RX_PERIPHERAL                          LPUART0   /*!< Device name: LPUART0 */
#define BOARD_INITPINS_DEBUG_UART_RX_SIGNAL                                   RX   /*!< LPUART0 signal: RX */
#define BOARD_INITPINS_DEBUG_UART_RX_PIN_NAME                         LPUART0_RX   /*!< Pin name */
#define BOARD_INITPINS_DEBUG_UART_RX_LABEL "J1[2]/U7[25]/D0/UART0_RX/FXIO_D16/UART1_RX_TGTMCU" /*!< Label */
#define BOARD_INITPINS_DEBUG_UART_RX_NAME                        "DEBUG_UART_RX"   /*!< Identifier name */

/* PORTB17 (number 63), J1[4]/U7[24]/D1/UART0_TX/FXIO_D17/UART1_TX_TGTMCU */
#define BOARD_INITPINS_DEBUG_UART_TX_PERIPHERAL                          LPUART0   /*!< Device name: LPUART0 */
#define BOARD_INITPINS_DEBUG_UART_TX_SIGNAL                                   TX   /*!< LPUART0 signal: TX */
#define BOARD_INITPINS_DEBUG_UART_TX_PIN_NAME                         LPUART0_TX   /*!< Pin name */
#define BOARD_INITPINS_DEBUG_UART_TX_LABEL "J1[4]/U7[24]/D1/UART0_TX/FXIO_D17/UART1_TX_TGTMCU" /*!< Label */
#define BOARD_INITPINS_DEBUG_UART_TX_NAME                        "DEBUG_UART_TX"   /*!< Identifier name */


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

/*******************************************************************************
 * EOF
 ******************************************************************************/
