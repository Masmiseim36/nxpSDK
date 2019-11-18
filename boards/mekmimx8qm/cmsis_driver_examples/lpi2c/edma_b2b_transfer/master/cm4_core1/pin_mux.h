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

/* HDMI_TX0_TS_SCL (number BN9), I2C0_3V3_SCL */
#define LPI2C2_INITPINSEX_I2C0_3V3_SCL_PERIPHERAL                      DMA__I2C0   /*!< Device name: DMA__I2C0 */
#define LPI2C2_INITPINSEX_I2C0_3V3_SCL_SIGNAL                            i2c_scl   /*!< DMA__I2C0 signal: i2c_scl */
#define LPI2C2_INITPINSEX_I2C0_3V3_SCL_PIN_NAME                  HDMI_TX0_TS_SCL   /*!< Pin name */
#define LPI2C2_INITPINSEX_I2C0_3V3_SCL_PIN_FUNCTION_ID      SC_P_HDMI_TX0_TS_SCL   /*!< Pin function id */
#define LPI2C2_INITPINSEX_I2C0_3V3_SCL_LABEL                      "I2C0_3V3_SCL"   /*!< Label */
#define LPI2C2_INITPINSEX_I2C0_3V3_SCL_NAME                       "I2C0_3V3_SCL"   /*!< Identifier name */

/* HDMI_TX0_TS_SDA (number BN7), I2C0_3V3_SDA */
#define LPI2C2_INITPINSEX_I2C0_3V3_SDA_PERIPHERAL                      DMA__I2C0   /*!< Device name: DMA__I2C0 */
#define LPI2C2_INITPINSEX_I2C0_3V3_SDA_SIGNAL                            i2c_sda   /*!< DMA__I2C0 signal: i2c_sda */
#define LPI2C2_INITPINSEX_I2C0_3V3_SDA_PIN_NAME                  HDMI_TX0_TS_SDA   /*!< Pin name */
#define LPI2C2_INITPINSEX_I2C0_3V3_SDA_PIN_FUNCTION_ID      SC_P_HDMI_TX0_TS_SDA   /*!< Pin function id */
#define LPI2C2_INITPINSEX_I2C0_3V3_SDA_LABEL                      "I2C0_3V3_SDA"   /*!< Label */
#define LPI2C2_INITPINSEX_I2C0_3V3_SDA_NAME                       "I2C0_3V3_SDA"   /*!< Identifier name */

/* HDMI_TX0_TS_SCL (number BN9), I2C0_3V3_SCL */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SCL_GPIO                        LSIO__GPIO2   /*!< GPIO device name: LSIO__GPIO2 */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SCL_GPIO_PIN                             2U   /*!< LSIO__GPIO2 pin index: 2 */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SCL_PIN_NAME                HDMI_TX0_TS_SCL   /*!< Pin name */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SCL_PIN_FUNCTION_ID    SC_P_HDMI_TX0_TS_SCL   /*!< Pin function id */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SCL_LABEL                    "I2C0_3V3_SCL"   /*!< Label */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SCL_NAME                     "I2C0_3V3_SCL"   /*!< Identifier name */

/* HDMI_TX0_TS_SDA (number BN7), I2C0_3V3_SDA */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SDA_GPIO                        LSIO__GPIO2   /*!< GPIO device name: LSIO__GPIO2 */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SDA_GPIO_PIN                             3U   /*!< LSIO__GPIO2 pin index: 3 */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SDA_PIN_NAME                HDMI_TX0_TS_SDA   /*!< Pin name */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SDA_PIN_FUNCTION_ID    SC_P_HDMI_TX0_TS_SDA   /*!< Pin function id */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SDA_LABEL                    "I2C0_3V3_SDA"   /*!< Label */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SDA_NAME                     "I2C0_3V3_SDA"   /*!< Identifier name */

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

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 * @param ipc scfw ipchandle.
 *
 */
void LPI2C2_InitPinsEx(sc_ipc_t ipc);                      /*!< Function assigned for the core: Cortex-M4F[cm4_1] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 * @param ipc scfw ipchandle.
 *
 */
void LPI2C2_DeinitPinsEx(sc_ipc_t ipc);                    /*!< Function assigned for the core: Cortex-M4F[cm4_1] */

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
