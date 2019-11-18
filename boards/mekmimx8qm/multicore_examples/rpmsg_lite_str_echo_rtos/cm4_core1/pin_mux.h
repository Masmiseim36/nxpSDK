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

/* M41_I2C0_SDA (number AU49), BB_M41_I2C0_1V8_SDA/J20A[23] */
#define BOARD_INITPINS_BB_M41_I2C0_1V8_SDA_PERIPHERAL                  M41__I2C0   /*!< Device name: M41__I2C0 */
#define BOARD_INITPINS_BB_M41_I2C0_1V8_SDA_SIGNAL                        i2c_sda   /*!< M41__I2C0 signal: i2c_sda */
#define BOARD_INITPINS_BB_M41_I2C0_1V8_SDA_PIN_NAME                 M41_I2C0_SDA   /*!< Pin name */
#define BOARD_INITPINS_BB_M41_I2C0_1V8_SDA_PIN_FUNCTION_ID     SC_P_M41_I2C0_SDA   /*!< Pin function id */
#define BOARD_INITPINS_BB_M41_I2C0_1V8_SDA_LABEL  "BB_M41_I2C0_1V8_SDA/J20A[23]"   /*!< Label */
#define BOARD_INITPINS_BB_M41_I2C0_1V8_SDA_NAME            "BB_M41_I2C0_1V8_SDA"   /*!< Identifier name */

/* M41_I2C0_SCL (number AR45), BB_M41_I2C0_1V8_SCL/J20A[22] */
#define BOARD_INITPINS_BB_M41_I2C0_1V8_SCL_PERIPHERAL                  M41__I2C0   /*!< Device name: M41__I2C0 */
#define BOARD_INITPINS_BB_M41_I2C0_1V8_SCL_SIGNAL                        i2c_scl   /*!< M41__I2C0 signal: i2c_scl */
#define BOARD_INITPINS_BB_M41_I2C0_1V8_SCL_PIN_NAME                 M41_I2C0_SCL   /*!< Pin name */
#define BOARD_INITPINS_BB_M41_I2C0_1V8_SCL_PIN_FUNCTION_ID     SC_P_M41_I2C0_SCL   /*!< Pin function id */
#define BOARD_INITPINS_BB_M41_I2C0_1V8_SCL_LABEL  "BB_M41_I2C0_1V8_SCL/J20A[22]"   /*!< Label */
#define BOARD_INITPINS_BB_M41_I2C0_1V8_SCL_NAME            "BB_M41_I2C0_1V8_SCL"   /*!< Identifier name */

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
