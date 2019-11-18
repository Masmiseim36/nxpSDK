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

/* QSPI0A_DATA0 (coord G13), QSPI0_DAT0 */
#define BOARD_INITPINS_QSPI0_DAT0_PERIPHERAL                         LSIO__QSPI0   /*!< Device name: LSIO__QSPI0 */
#define BOARD_INITPINS_QSPI0_DAT0_SIGNAL                             qspi_a_data   /*!< LSIO__QSPI0 signal: qspi_a_data */
#define BOARD_INITPINS_QSPI0_DAT0_CHANNEL                                      0   /*!< LSIO__QSPI0 qspi_a_data channel: 0 */
#define BOARD_INITPINS_QSPI0_DAT0_PIN_NAME                          QSPI0A_DATA0   /*!< Pin name */
#define BOARD_INITPINS_QSPI0_DAT0_PIN_FUNCTION_ID              SC_P_QSPI0A_DATA0   /*!< Pin function id */
#define BOARD_INITPINS_QSPI0_DAT0_LABEL                             "QSPI0_DAT0"   /*!< Label */
#define BOARD_INITPINS_QSPI0_DAT0_NAME                              "QSPI0_DAT0"   /*!< Identifier name */

/* QSPI0A_DATA1 (coord F14), QSPI0_DAT1 */
#define BOARD_INITPINS_QSPI0_DAT1_PERIPHERAL                         LSIO__QSPI0   /*!< Device name: LSIO__QSPI0 */
#define BOARD_INITPINS_QSPI0_DAT1_SIGNAL                             qspi_a_data   /*!< LSIO__QSPI0 signal: qspi_a_data */
#define BOARD_INITPINS_QSPI0_DAT1_CHANNEL                                      1   /*!< LSIO__QSPI0 qspi_a_data channel: 1 */
#define BOARD_INITPINS_QSPI0_DAT1_PIN_NAME                          QSPI0A_DATA1   /*!< Pin name */
#define BOARD_INITPINS_QSPI0_DAT1_PIN_FUNCTION_ID              SC_P_QSPI0A_DATA1   /*!< Pin function id */
#define BOARD_INITPINS_QSPI0_DAT1_LABEL                             "QSPI0_DAT1"   /*!< Label */
#define BOARD_INITPINS_QSPI0_DAT1_NAME                              "QSPI0_DAT1"   /*!< Identifier name */

/* QSPI0A_DATA2 (coord H14), QSPI0_DAT2 */
#define BOARD_INITPINS_QSPI0_DAT2_PERIPHERAL                         LSIO__QSPI0   /*!< Device name: LSIO__QSPI0 */
#define BOARD_INITPINS_QSPI0_DAT2_SIGNAL                             qspi_a_data   /*!< LSIO__QSPI0 signal: qspi_a_data */
#define BOARD_INITPINS_QSPI0_DAT2_CHANNEL                                      2   /*!< LSIO__QSPI0 qspi_a_data channel: 2 */
#define BOARD_INITPINS_QSPI0_DAT2_PIN_NAME                          QSPI0A_DATA2   /*!< Pin name */
#define BOARD_INITPINS_QSPI0_DAT2_PIN_FUNCTION_ID              SC_P_QSPI0A_DATA2   /*!< Pin function id */
#define BOARD_INITPINS_QSPI0_DAT2_LABEL                             "QSPI0_DAT2"   /*!< Label */
#define BOARD_INITPINS_QSPI0_DAT2_NAME                              "QSPI0_DAT2"   /*!< Identifier name */

/* QSPI0A_DATA3 (coord H16), QSPI0_DAT3 */
#define BOARD_INITPINS_QSPI0_DAT3_PERIPHERAL                         LSIO__QSPI0   /*!< Device name: LSIO__QSPI0 */
#define BOARD_INITPINS_QSPI0_DAT3_SIGNAL                             qspi_a_data   /*!< LSIO__QSPI0 signal: qspi_a_data */
#define BOARD_INITPINS_QSPI0_DAT3_CHANNEL                                      3   /*!< LSIO__QSPI0 qspi_a_data channel: 3 */
#define BOARD_INITPINS_QSPI0_DAT3_PIN_NAME                          QSPI0A_DATA3   /*!< Pin name */
#define BOARD_INITPINS_QSPI0_DAT3_PIN_FUNCTION_ID              SC_P_QSPI0A_DATA3   /*!< Pin function id */
#define BOARD_INITPINS_QSPI0_DAT3_LABEL                             "QSPI0_DAT3"   /*!< Label */
#define BOARD_INITPINS_QSPI0_DAT3_NAME                              "QSPI0_DAT3"   /*!< Identifier name */

/* QSPI0A_DQS (coord G17), QSPI0_DQS */
#define BOARD_INITPINS_QSPI0_DQS_PERIPHERAL                          LSIO__QSPI0   /*!< Device name: LSIO__QSPI0 */
#define BOARD_INITPINS_QSPI0_DQS_SIGNAL                               qspi_a_dqs   /*!< LSIO__QSPI0 signal: qspi_a_dqs */
#define BOARD_INITPINS_QSPI0_DQS_PIN_NAME                             QSPI0A_DQS   /*!< Pin name */
#define BOARD_INITPINS_QSPI0_DQS_PIN_FUNCTION_ID                 SC_P_QSPI0A_DQS   /*!< Pin function id */
#define BOARD_INITPINS_QSPI0_DQS_LABEL                               "QSPI0_DQS"   /*!< Label */
#define BOARD_INITPINS_QSPI0_DQS_NAME                                "QSPI0_DQS"   /*!< Identifier name */

/* QSPI0A_SCLK (coord E17), QSPI0A_CLK */
#define BOARD_INITPINS_QSPI0A_CLK_PERIPHERAL                         LSIO__QSPI0   /*!< Device name: LSIO__QSPI0 */
#define BOARD_INITPINS_QSPI0A_CLK_SIGNAL                             qspi_a_sclk   /*!< LSIO__QSPI0 signal: qspi_a_sclk */
#define BOARD_INITPINS_QSPI0A_CLK_PIN_NAME                           QSPI0A_SCLK   /*!< Pin name */
#define BOARD_INITPINS_QSPI0A_CLK_PIN_FUNCTION_ID               SC_P_QSPI0A_SCLK   /*!< Pin function id */
#define BOARD_INITPINS_QSPI0A_CLK_LABEL                             "QSPI0A_CLK"   /*!< Label */
#define BOARD_INITPINS_QSPI0A_CLK_NAME                              "QSPI0A_CLK"   /*!< Identifier name */

/* QSPI0A_SS0_B (coord E15), QSPI0A_CS_B */
#define BOARD_INITPINS_QSPI0A_CS_B_PERIPHERAL                        LSIO__QSPI0   /*!< Device name: LSIO__QSPI0 */
#define BOARD_INITPINS_QSPI0A_CS_B_SIGNAL                           qspi_a_ss0_b   /*!< LSIO__QSPI0 signal: qspi_a_ss0_b */
#define BOARD_INITPINS_QSPI0A_CS_B_PIN_NAME                         QSPI0A_SS0_B   /*!< Pin name */
#define BOARD_INITPINS_QSPI0A_CS_B_PIN_FUNCTION_ID             SC_P_QSPI0A_SS0_B   /*!< Pin function id */
#define BOARD_INITPINS_QSPI0A_CS_B_LABEL                           "QSPI0A_CS_B"   /*!< Label */
#define BOARD_INITPINS_QSPI0A_CS_B_NAME                            "QSPI0A_CS_B"   /*!< Identifier name */

/* QSPI0B_DATA0 (coord H18), QSPI0_DAT4 */
#define BOARD_INITPINS_QSPI0_DAT4_PERIPHERAL                         LSIO__QSPI0   /*!< Device name: LSIO__QSPI0 */
#define BOARD_INITPINS_QSPI0_DAT4_SIGNAL                             qspi_b_data   /*!< LSIO__QSPI0 signal: qspi_b_data */
#define BOARD_INITPINS_QSPI0_DAT4_CHANNEL                                      0   /*!< LSIO__QSPI0 qspi_b_data channel: 0 */
#define BOARD_INITPINS_QSPI0_DAT4_PIN_NAME                          QSPI0B_DATA0   /*!< Pin name */
#define BOARD_INITPINS_QSPI0_DAT4_PIN_FUNCTION_ID              SC_P_QSPI0B_DATA0   /*!< Pin function id */
#define BOARD_INITPINS_QSPI0_DAT4_LABEL                             "QSPI0_DAT4"   /*!< Label */
#define BOARD_INITPINS_QSPI0_DAT4_NAME                              "QSPI0_DAT4"   /*!< Identifier name */

/* QSPI0B_DATA1 (coord H20), QSPI0_DAT5 */
#define BOARD_INITPINS_QSPI0_DAT5_PERIPHERAL                         LSIO__QSPI0   /*!< Device name: LSIO__QSPI0 */
#define BOARD_INITPINS_QSPI0_DAT5_SIGNAL                             qspi_b_data   /*!< LSIO__QSPI0 signal: qspi_b_data */
#define BOARD_INITPINS_QSPI0_DAT5_CHANNEL                                      1   /*!< LSIO__QSPI0 qspi_b_data channel: 1 */
#define BOARD_INITPINS_QSPI0_DAT5_PIN_NAME                          QSPI0B_DATA1   /*!< Pin name */
#define BOARD_INITPINS_QSPI0_DAT5_PIN_FUNCTION_ID              SC_P_QSPI0B_DATA1   /*!< Pin function id */
#define BOARD_INITPINS_QSPI0_DAT5_LABEL                             "QSPI0_DAT5"   /*!< Label */
#define BOARD_INITPINS_QSPI0_DAT5_NAME                              "QSPI0_DAT5"   /*!< Identifier name */

/* QSPI0B_DATA2 (coord G19), QSPI0_DAT6 */
#define BOARD_INITPINS_QSPI0_DAT6_PERIPHERAL                         LSIO__QSPI0   /*!< Device name: LSIO__QSPI0 */
#define BOARD_INITPINS_QSPI0_DAT6_SIGNAL                             qspi_b_data   /*!< LSIO__QSPI0 signal: qspi_b_data */
#define BOARD_INITPINS_QSPI0_DAT6_CHANNEL                                      2   /*!< LSIO__QSPI0 qspi_b_data channel: 2 */
#define BOARD_INITPINS_QSPI0_DAT6_PIN_NAME                          QSPI0B_DATA2   /*!< Pin name */
#define BOARD_INITPINS_QSPI0_DAT6_PIN_FUNCTION_ID              SC_P_QSPI0B_DATA2   /*!< Pin function id */
#define BOARD_INITPINS_QSPI0_DAT6_LABEL                             "QSPI0_DAT6"   /*!< Label */
#define BOARD_INITPINS_QSPI0_DAT6_NAME                              "QSPI0_DAT6"   /*!< Identifier name */

/* QSPI0B_DATA3 (coord F20), QSPI0_DAT7 */
#define BOARD_INITPINS_QSPI0_DAT7_PERIPHERAL                         LSIO__QSPI0   /*!< Device name: LSIO__QSPI0 */
#define BOARD_INITPINS_QSPI0_DAT7_SIGNAL                             qspi_b_data   /*!< LSIO__QSPI0 signal: qspi_b_data */
#define BOARD_INITPINS_QSPI0_DAT7_CHANNEL                                      3   /*!< LSIO__QSPI0 qspi_b_data channel: 3 */
#define BOARD_INITPINS_QSPI0_DAT7_PIN_NAME                          QSPI0B_DATA3   /*!< Pin name */
#define BOARD_INITPINS_QSPI0_DAT7_PIN_FUNCTION_ID              SC_P_QSPI0B_DATA3   /*!< Pin function id */
#define BOARD_INITPINS_QSPI0_DAT7_LABEL                             "QSPI0_DAT7"   /*!< Label */
#define BOARD_INITPINS_QSPI0_DAT7_NAME                              "QSPI0_DAT7"   /*!< Identifier name */

/* QSPI0B_SCLK (coord F18), QSPI0B_CLK */
#define BOARD_INITPINS_QSPI0B_CLK_PERIPHERAL                         LSIO__QSPI0   /*!< Device name: LSIO__QSPI0 */
#define BOARD_INITPINS_QSPI0B_CLK_SIGNAL                             qspi_b_sclk   /*!< LSIO__QSPI0 signal: qspi_b_sclk */
#define BOARD_INITPINS_QSPI0B_CLK_PIN_NAME                           QSPI0B_SCLK   /*!< Pin name */
#define BOARD_INITPINS_QSPI0B_CLK_PIN_FUNCTION_ID               SC_P_QSPI0B_SCLK   /*!< Pin function id */
#define BOARD_INITPINS_QSPI0B_CLK_LABEL                             "QSPI0B_CLK"   /*!< Label */
#define BOARD_INITPINS_QSPI0B_CLK_NAME                              "QSPI0B_CLK"   /*!< Identifier name */

/* QSPI0B_SS0_B (coord F22), QSPI0B_CS_B */
#define BOARD_INITPINS_QSPI0B_CS_B_PERIPHERAL                        LSIO__QSPI0   /*!< Device name: LSIO__QSPI0 */
#define BOARD_INITPINS_QSPI0B_CS_B_SIGNAL                           qspi_b_ss0_b   /*!< LSIO__QSPI0 signal: qspi_b_ss0_b */
#define BOARD_INITPINS_QSPI0B_CS_B_PIN_NAME                         QSPI0B_SS0_B   /*!< Pin name */
#define BOARD_INITPINS_QSPI0B_CS_B_PIN_FUNCTION_ID             SC_P_QSPI0B_SS0_B   /*!< Pin function id */
#define BOARD_INITPINS_QSPI0B_CS_B_LABEL                           "QSPI0B_CS_B"   /*!< Label */
#define BOARD_INITPINS_QSPI0B_CS_B_NAME                            "QSPI0B_CS_B"   /*!< Identifier name */

/* UART0_RTS_B (number AU45), BB_UART2_RX/J20A[28] */
#define BOARD_INITPINS_BB_UART2_RX_PERIPHERAL                         DMA__UART2   /*!< Device name: DMA__UART2 */
#define BOARD_INITPINS_BB_UART2_RX_SIGNAL                                uart_rx   /*!< DMA__UART2 signal: uart_rx */
#define BOARD_INITPINS_BB_UART2_RX_PIN_NAME                          UART0_RTS_B   /*!< Pin name */
#define BOARD_INITPINS_BB_UART2_RX_PIN_FUNCTION_ID              SC_P_UART0_RTS_B   /*!< Pin function id */
#define BOARD_INITPINS_BB_UART2_RX_LABEL                  "BB_UART2_RX/J20A[28]"   /*!< Label */
#define BOARD_INITPINS_BB_UART2_RX_NAME                            "BB_UART2_RX"   /*!< Identifier name */

/* UART0_CTS_B (number AW49), BB_UART2_TX/J20A[29] */
#define BOARD_INITPINS_BB_UART2_TX_PERIPHERAL                         DMA__UART2   /*!< Device name: DMA__UART2 */
#define BOARD_INITPINS_BB_UART2_TX_SIGNAL                                uart_tx   /*!< DMA__UART2 signal: uart_tx */
#define BOARD_INITPINS_BB_UART2_TX_PIN_NAME                          UART0_CTS_B   /*!< Pin name */
#define BOARD_INITPINS_BB_UART2_TX_PIN_FUNCTION_ID              SC_P_UART0_CTS_B   /*!< Pin function id */
#define BOARD_INITPINS_BB_UART2_TX_LABEL                  "BB_UART2_TX/J20A[29]"   /*!< Label */
#define BOARD_INITPINS_BB_UART2_TX_NAME                            "BB_UART2_TX"   /*!< Identifier name */

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
void BOARD_InitPins(sc_ipc_t ipc);                         /*!< Function assigned for the core: Cortex-M4F[cm4_1] */

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
