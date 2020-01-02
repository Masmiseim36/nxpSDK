/*
 * Copyright 2019 NXP
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
#define BOARD_UART0_RX_PERIPHERAL                                        LPUART0   /*!< Device name: LPUART0 */
#define BOARD_UART0_RX_SIGNAL                                          lpuart_rx   /*!< LPUART0 signal: lpuart_rx */
#define BOARD_UART0_RX_PIN_NAME                                            PTA19   /*!< Pin name */
#define BOARD_UART0_RX_PIN_FUNCTION_ID                   IOMUXC_PTA19_LPUART0_RX   /*!< Pin function id */
#define BOARD_UART0_RX_LABEL                                          "UART0_RX"   /*!< Label */
#define BOARD_UART0_RX_NAME                                           "UART0_RX"   /*!< Identifier name */

/* PTA18 (number AC19), UART0_TX */
#define BOARD_UART0_TX_PERIPHERAL                                        LPUART0   /*!< Device name: LPUART0 */
#define BOARD_UART0_TX_SIGNAL                                          lpuart_tx   /*!< LPUART0 signal: lpuart_tx */
#define BOARD_UART0_TX_PIN_NAME                                            PTA18   /*!< Pin name */
#define BOARD_UART0_TX_PIN_FUNCTION_ID                   IOMUXC_PTA18_LPUART0_TX   /*!< Pin function id */
#define BOARD_UART0_TX_LABEL                                          "UART0_TX"   /*!< Label */
#define BOARD_UART0_TX_NAME                                           "UART0_TX"   /*!< Identifier name */

/* PTA8 (number AB16), UART2_CTS */
#define BOARD_UART2_CTS_PERIPHERAL                                           PTA   /*!< Device name: PTA */
#define BOARD_UART2_CTS_SIGNAL                                              port   /*!< PTA signal: port */
#define BOARD_UART2_CTS_CHANNEL                                                8   /*!< PTA port channel: 8 */
#define BOARD_UART2_CTS_PIN_NAME                                            PTA8   /*!< Pin name */
#define BOARD_UART2_CTS_PIN_FUNCTION_ID                         IOMUXC_PTA8_PTA8   /*!< Pin function id */
#define BOARD_UART2_CTS_LABEL                                        "UART2_CTS"   /*!< Label */
#define BOARD_UART2_CTS_NAME                                         "UART2_CTS"   /*!< Identifier name */
#define BOARD_UART2_CTS_DIRECTION                            kGPIO_DigitalOutput   /*!< Direction */

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
void BOARD_InitPins(void);                                 /*!< Function assigned for the core: Cortex-M4[cm4] */

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
