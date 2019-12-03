/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

#include "board.h"

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/* UART0_RX (number AB32), FTDI_UART0_RX */
#define LPUART_INITPINS_FTDI_UART0_RX_PERIPHERAL                     ADMA__UART0   /*!< Device name: ADMA__UART0 */
#define LPUART_INITPINS_FTDI_UART0_RX_SIGNAL                             uart_rx   /*!< ADMA__UART0 signal: uart_rx */
#define LPUART_INITPINS_FTDI_UART0_RX_PIN_NAME                          UART0_RX   /*!< Pin name */
#define LPUART_INITPINS_FTDI_UART0_RX_PIN_FUNCTION_ID              SC_P_UART0_RX   /*!< Pin function id */
#define LPUART_INITPINS_FTDI_UART0_RX_LABEL                      "FTDI_UART0_RX"   /*!< Label */
#define LPUART_INITPINS_FTDI_UART0_RX_NAME                       "FTDI_UART0_RX"   /*!< Identifier name */

/* UART0_TX (number AA29), FTDI_UART0_TX */
#define LPUART_INITPINS_FTDI_UART0_TX_PERIPHERAL                     ADMA__UART0   /*!< Device name: ADMA__UART0 */
#define LPUART_INITPINS_FTDI_UART0_TX_SIGNAL                             uart_tx   /*!< ADMA__UART0 signal: uart_tx */
#define LPUART_INITPINS_FTDI_UART0_TX_PIN_NAME                          UART0_TX   /*!< Pin name */
#define LPUART_INITPINS_FTDI_UART0_TX_PIN_FUNCTION_ID              SC_P_UART0_TX   /*!< Pin function id */
#define LPUART_INITPINS_FTDI_UART0_TX_LABEL                      "FTDI_UART0_TX"   /*!< Label */
#define LPUART_INITPINS_FTDI_UART0_TX_NAME                       "FTDI_UART0_TX"   /*!< Identifier name */

/* UART0_TX (number AA29), FTDI_UART0_TX */
#define LPUART_DEINITPINS_FTDI_UART0_TX_GPIO                         LSIO__GPIO1   /*!< GPIO device name: LSIO__GPIO1 */
#define LPUART_DEINITPINS_FTDI_UART0_TX_GPIO_PIN                             22U   /*!< LSIO__GPIO1 pin index: 22 */
#define LPUART_DEINITPINS_FTDI_UART0_TX_PIN_NAME                        UART0_TX   /*!< Pin name */
#define LPUART_DEINITPINS_FTDI_UART0_TX_PIN_FUNCTION_ID            SC_P_UART0_TX   /*!< Pin function id */
#define LPUART_DEINITPINS_FTDI_UART0_TX_LABEL                    "FTDI_UART0_TX"   /*!< Label */
#define LPUART_DEINITPINS_FTDI_UART0_TX_NAME                     "FTDI_UART0_TX"   /*!< Identifier name */

/* UART0_RX (number AB32), FTDI_UART0_RX */
#define LPUART_DEINITPINS_FTDI_UART0_RX_GPIO                         LSIO__GPIO1   /*!< GPIO device name: LSIO__GPIO1 */
#define LPUART_DEINITPINS_FTDI_UART0_RX_GPIO_PIN                             21U   /*!< LSIO__GPIO1 pin index: 21 */
#define LPUART_DEINITPINS_FTDI_UART0_RX_PIN_NAME                        UART0_RX   /*!< Pin name */
#define LPUART_DEINITPINS_FTDI_UART0_RX_PIN_FUNCTION_ID            SC_P_UART0_RX   /*!< Pin function id */
#define LPUART_DEINITPINS_FTDI_UART0_RX_LABEL                    "FTDI_UART0_RX"   /*!< Label */
#define LPUART_DEINITPINS_FTDI_UART0_RX_NAME                     "FTDI_UART0_RX"   /*!< Identifier name */

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
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 * @param ipc scfw ipchandle.
 *
 */
void BOARD_InitPins(sc_ipc_t ipc);                         /*!< Function assigned for the core: Cortex-M4F[m4] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 * @param ipc scfw ipchandle.
 *
 */
void LPUART_InitPins(sc_ipc_t ipc);                        /*!< Function assigned for the core: Cortex-M4F[m4] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 * @param ipc scfw ipchandle.
 *
 */
void LPUART_DeinitPins(sc_ipc_t ipc);                      /*!< Function assigned for the core: Cortex-M4F[m4] */

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
