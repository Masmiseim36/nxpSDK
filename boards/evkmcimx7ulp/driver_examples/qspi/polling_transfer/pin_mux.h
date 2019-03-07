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
#define BOARD_INITPINS_UART0_RX_PERIPHERAL                               LPUART0   /*!< Device name: LPUART0 */
#define BOARD_INITPINS_UART0_RX_SIGNAL                                 lpuart_rx   /*!< LPUART0 signal: lpuart_rx */
#define BOARD_INITPINS_UART0_RX_PIN_NAME                                   PTA19   /*!< Pin name */
#define BOARD_INITPINS_UART0_RX_PIN_FUNCTION_ID          IOMUXC_PTA19_LPUART0_RX   /*!< Pin function id */
#define BOARD_INITPINS_UART0_RX_LABEL                                 "UART0_RX"   /*!< Label */
#define BOARD_INITPINS_UART0_RX_NAME                                  "UART0_RX"   /*!< Identifier name */

/* PTA18 (number AC19), UART0_TX */
#define BOARD_INITPINS_UART0_TX_PERIPHERAL                               LPUART0   /*!< Device name: LPUART0 */
#define BOARD_INITPINS_UART0_TX_SIGNAL                                 lpuart_tx   /*!< LPUART0 signal: lpuart_tx */
#define BOARD_INITPINS_UART0_TX_PIN_NAME                                   PTA18   /*!< Pin name */
#define BOARD_INITPINS_UART0_TX_PIN_FUNCTION_ID          IOMUXC_PTA18_LPUART0_TX   /*!< Pin function id */
#define BOARD_INITPINS_UART0_TX_LABEL                                 "UART0_TX"   /*!< Label */
#define BOARD_INITPINS_UART0_TX_NAME                                  "UART0_TX"   /*!< Identifier name */

/* PTB19 (number AD10), QSPIA_DATA0 */
#define BOARD_INITPINS_QSPIA_DATA0_PERIPHERAL                              QSPIA   /*!< Device name: QSPIA */
#define BOARD_INITPINS_QSPIA_DATA0_SIGNAL                             qspia_data   /*!< QSPIA signal: qspia_data */
#define BOARD_INITPINS_QSPIA_DATA0_CHANNEL                                     0   /*!< QSPIA qspia_data channel: 0 */
#define BOARD_INITPINS_QSPIA_DATA0_PIN_NAME                                PTB19   /*!< Pin name */
#define BOARD_INITPINS_QSPIA_DATA0_PIN_FUNCTION_ID      IOMUXC_PTB19_QSPIA_DATA0   /*!< Pin function id */
#define BOARD_INITPINS_QSPIA_DATA0_LABEL                           "QSPIA_DATA0"   /*!< Label */
#define BOARD_INITPINS_QSPIA_DATA0_NAME                            "QSPIA_DATA0"   /*!< Identifier name */

/* PTB18 (number AG10), QSPIA_DATA1 */
#define BOARD_INITPINS_QSPIA_DATA1_PERIPHERAL                              QSPIA   /*!< Device name: QSPIA */
#define BOARD_INITPINS_QSPIA_DATA1_SIGNAL                             qspia_data   /*!< QSPIA signal: qspia_data */
#define BOARD_INITPINS_QSPIA_DATA1_CHANNEL                                     1   /*!< QSPIA qspia_data channel: 1 */
#define BOARD_INITPINS_QSPIA_DATA1_PIN_NAME                                PTB18   /*!< Pin name */
#define BOARD_INITPINS_QSPIA_DATA1_PIN_FUNCTION_ID      IOMUXC_PTB18_QSPIA_DATA1   /*!< Pin function id */
#define BOARD_INITPINS_QSPIA_DATA1_LABEL                           "QSPIA_DATA1"   /*!< Label */
#define BOARD_INITPINS_QSPIA_DATA1_NAME                            "QSPIA_DATA1"   /*!< Identifier name */

/* PTB17 (number AF10), QSPIA_DATA2 */
#define BOARD_INITPINS_QSPIA_DATA2_PERIPHERAL                              QSPIA   /*!< Device name: QSPIA */
#define BOARD_INITPINS_QSPIA_DATA2_SIGNAL                             qspia_data   /*!< QSPIA signal: qspia_data */
#define BOARD_INITPINS_QSPIA_DATA2_CHANNEL                                     2   /*!< QSPIA qspia_data channel: 2 */
#define BOARD_INITPINS_QSPIA_DATA2_PIN_NAME                                PTB17   /*!< Pin name */
#define BOARD_INITPINS_QSPIA_DATA2_PIN_FUNCTION_ID      IOMUXC_PTB17_QSPIA_DATA2   /*!< Pin function id */
#define BOARD_INITPINS_QSPIA_DATA2_LABEL                           "QSPIA_DATA2"   /*!< Label */
#define BOARD_INITPINS_QSPIA_DATA2_NAME                            "QSPIA_DATA2"   /*!< Identifier name */

/* PTB16 (number AF9), QSPIA_DATA3 */
#define BOARD_INITPINS_QSPIA_DATA3_PERIPHERAL                              QSPIA   /*!< Device name: QSPIA */
#define BOARD_INITPINS_QSPIA_DATA3_SIGNAL                             qspia_data   /*!< QSPIA signal: qspia_data */
#define BOARD_INITPINS_QSPIA_DATA3_CHANNEL                                     3   /*!< QSPIA qspia_data channel: 3 */
#define BOARD_INITPINS_QSPIA_DATA3_PIN_NAME                                PTB16   /*!< Pin name */
#define BOARD_INITPINS_QSPIA_DATA3_PIN_FUNCTION_ID      IOMUXC_PTB16_QSPIA_DATA3   /*!< Pin function id */
#define BOARD_INITPINS_QSPIA_DATA3_LABEL                           "QSPIA_DATA3"   /*!< Label */
#define BOARD_INITPINS_QSPIA_DATA3_NAME                            "QSPIA_DATA3"   /*!< Identifier name */

/* PTB15 (number AF8), QSPIA_SCLK */
#define BOARD_INITPINS_QSPIA_SCLK_PERIPHERAL                               QSPIA   /*!< Device name: QSPIA */
#define BOARD_INITPINS_QSPIA_SCLK_SIGNAL                              qspia_sclk   /*!< QSPIA signal: qspia_sclk */
#define BOARD_INITPINS_QSPIA_SCLK_PIN_NAME                                 PTB15   /*!< Pin name */
#define BOARD_INITPINS_QSPIA_SCLK_PIN_FUNCTION_ID        IOMUXC_PTB15_QSPIA_SCLK   /*!< Pin function id */
#define BOARD_INITPINS_QSPIA_SCLK_LABEL                             "QSPIA_SCLK"   /*!< Label */
#define BOARD_INITPINS_QSPIA_SCLK_NAME                              "QSPIA_SCLK"   /*!< Identifier name */

/* PTB8 (number AG6), QSPIA_SS0_B */
#define BOARD_INITPINS_QSPIA_SS0_B_PERIPHERAL                              QSPIA   /*!< Device name: QSPIA */
#define BOARD_INITPINS_QSPIA_SS0_B_SIGNAL                             qspia_ss_b   /*!< QSPIA signal: qspia_ss_b */
#define BOARD_INITPINS_QSPIA_SS0_B_CHANNEL                                     0   /*!< QSPIA qspia_ss_b channel: 0 */
#define BOARD_INITPINS_QSPIA_SS0_B_PIN_NAME                                 PTB8   /*!< Pin name */
#define BOARD_INITPINS_QSPIA_SS0_B_PIN_FUNCTION_ID       IOMUXC_PTB8_QSPIA_SS0_B   /*!< Pin function id */
#define BOARD_INITPINS_QSPIA_SS0_B_LABEL                           "QSPIA_SS0_B"   /*!< Label */
#define BOARD_INITPINS_QSPIA_SS0_B_NAME                            "QSPIA_SS0_B"   /*!< Identifier name */

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
