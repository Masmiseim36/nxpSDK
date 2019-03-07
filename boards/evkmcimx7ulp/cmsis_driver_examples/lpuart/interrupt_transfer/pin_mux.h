/*
 * Copyright 2018 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_


/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/* PTA19 (number AB19), UART0_RX */
#define LPUART0_INITPINS_UART0_RX_PERIPHERAL                             LPUART0   /*!< Device name: LPUART0 */
#define LPUART0_INITPINS_UART0_RX_SIGNAL                               lpuart_rx   /*!< LPUART0 signal: lpuart_rx */
#define LPUART0_INITPINS_UART0_RX_PIN_NAME                                 PTA19   /*!< Pin name */
#define LPUART0_INITPINS_UART0_RX_PIN_FUNCTION_ID        IOMUXC_PTA19_LPUART0_RX   /*!< Pin function id */
#define LPUART0_INITPINS_UART0_RX_LABEL                               "UART0_RX"   /*!< Label */
#define LPUART0_INITPINS_UART0_RX_NAME                                "UART0_RX"   /*!< Identifier name */

/* PTA18 (number AC19), UART0_TX */
#define LPUART0_INITPINS_UART0_TX_PERIPHERAL                             LPUART0   /*!< Device name: LPUART0 */
#define LPUART0_INITPINS_UART0_TX_SIGNAL                               lpuart_tx   /*!< LPUART0 signal: lpuart_tx */
#define LPUART0_INITPINS_UART0_TX_PIN_NAME                                 PTA18   /*!< Pin name */
#define LPUART0_INITPINS_UART0_TX_PIN_FUNCTION_ID        IOMUXC_PTA18_LPUART0_TX   /*!< Pin function id */
#define LPUART0_INITPINS_UART0_TX_LABEL                               "UART0_TX"   /*!< Label */
#define LPUART0_INITPINS_UART0_TX_NAME                                "UART0_TX"   /*!< Identifier name */

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
 *
 */
void LPUART0_InitPins(void);                               /*!< Function assigned for the core: Cortex-M4[cm4] */

/*!
 * @brief Deint pin routing and optionally pin electrical features.
 *
 */
void LPUART0_DeinitPins(void);                             /*!< Function assigned for the core: Cortex-M4[cm4] */

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
