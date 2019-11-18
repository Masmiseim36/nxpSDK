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

/* M40_I2C0_SCL (number AM44), FTDI_M40_UART0_RX */
#define BOARD_INITPINS_FTDI_M40_UART0_RX_PERIPHERAL                   M40__UART0   /*!< Device name: M40__UART0 */
#define BOARD_INITPINS_FTDI_M40_UART0_RX_SIGNAL                          uart_rx   /*!< M40__UART0 signal: uart_rx */
#define BOARD_INITPINS_FTDI_M40_UART0_RX_PIN_NAME                   M40_I2C0_SCL   /*!< Pin name */
#define BOARD_INITPINS_FTDI_M40_UART0_RX_PIN_FUNCTION_ID       SC_P_M40_I2C0_SCL   /*!< Pin function id */
#define BOARD_INITPINS_FTDI_M40_UART0_RX_LABEL               "FTDI_M40_UART0_RX"   /*!< Label */
#define BOARD_INITPINS_FTDI_M40_UART0_RX_NAME                "FTDI_M40_UART0_RX"   /*!< Identifier name */

/* M40_I2C0_SDA (number AU51), FTDI_M40_UART0_TX */
#define BOARD_INITPINS_FTDI_M40_UART0_TX_PERIPHERAL                   M40__UART0   /*!< Device name: M40__UART0 */
#define BOARD_INITPINS_FTDI_M40_UART0_TX_SIGNAL                          uart_tx   /*!< M40__UART0 signal: uart_tx */
#define BOARD_INITPINS_FTDI_M40_UART0_TX_PIN_NAME                   M40_I2C0_SDA   /*!< Pin name */
#define BOARD_INITPINS_FTDI_M40_UART0_TX_PIN_FUNCTION_ID       SC_P_M40_I2C0_SDA   /*!< Pin function id */
#define BOARD_INITPINS_FTDI_M40_UART0_TX_LABEL               "FTDI_M40_UART0_TX"   /*!< Label */
#define BOARD_INITPINS_FTDI_M40_UART0_TX_NAME                "FTDI_M40_UART0_TX"   /*!< Identifier name */

/* HDMI_TX0_TS_SCL (number BN9), I2C0_3V3_SCL */
#define BOARD_INITPINS_I2C0_3V3_SCL_PERIPHERAL                         DMA__I2C0   /*!< Device name: DMA__I2C0 */
#define BOARD_INITPINS_I2C0_3V3_SCL_SIGNAL                               i2c_scl   /*!< DMA__I2C0 signal: i2c_scl */
#define BOARD_INITPINS_I2C0_3V3_SCL_PIN_NAME                     HDMI_TX0_TS_SCL   /*!< Pin name */
#define BOARD_INITPINS_I2C0_3V3_SCL_PIN_FUNCTION_ID         SC_P_HDMI_TX0_TS_SCL   /*!< Pin function id */
#define BOARD_INITPINS_I2C0_3V3_SCL_LABEL                         "I2C0_3V3_SCL"   /*!< Label */
#define BOARD_INITPINS_I2C0_3V3_SCL_NAME                          "I2C0_3V3_SCL"   /*!< Identifier name */

/* HDMI_TX0_TS_SDA (number BN7), I2C0_3V3_SDA */
#define BOARD_INITPINS_I2C0_3V3_SDA_PERIPHERAL                         DMA__I2C0   /*!< Device name: DMA__I2C0 */
#define BOARD_INITPINS_I2C0_3V3_SDA_SIGNAL                               i2c_sda   /*!< DMA__I2C0 signal: i2c_sda */
#define BOARD_INITPINS_I2C0_3V3_SDA_PIN_NAME                     HDMI_TX0_TS_SDA   /*!< Pin name */
#define BOARD_INITPINS_I2C0_3V3_SDA_PIN_FUNCTION_ID         SC_P_HDMI_TX0_TS_SDA   /*!< Pin function id */
#define BOARD_INITPINS_I2C0_3V3_SDA_LABEL                         "I2C0_3V3_SDA"   /*!< Label */
#define BOARD_INITPINS_I2C0_3V3_SDA_NAME                          "I2C0_3V3_SDA"   /*!< Identifier name */

/* ENET1_MDC (coord A13), I2C4_3V3_SCL */
#define BOARD_INITPINS_I2C4_3V3_SCL_PERIPHERAL                         DMA__I2C4   /*!< Device name: DMA__I2C4 */
#define BOARD_INITPINS_I2C4_3V3_SCL_SIGNAL                               i2c_scl   /*!< DMA__I2C4 signal: i2c_scl */
#define BOARD_INITPINS_I2C4_3V3_SCL_PIN_NAME                           ENET1_MDC   /*!< Pin name */
#define BOARD_INITPINS_I2C4_3V3_SCL_PIN_FUNCTION_ID               SC_P_ENET1_MDC   /*!< Pin function id */
#define BOARD_INITPINS_I2C4_3V3_SCL_LABEL                         "I2C4_3V3_SCL"   /*!< Label */
#define BOARD_INITPINS_I2C4_3V3_SCL_NAME                          "I2C4_3V3_SCL"   /*!< Identifier name */

/* ENET1_MDIO (coord C13), I2C4_3V3_SDA */
#define BOARD_INITPINS_I2C4_3V3_SDA_PERIPHERAL                         DMA__I2C4   /*!< Device name: DMA__I2C4 */
#define BOARD_INITPINS_I2C4_3V3_SDA_SIGNAL                               i2c_sda   /*!< DMA__I2C4 signal: i2c_sda */
#define BOARD_INITPINS_I2C4_3V3_SDA_PIN_NAME                          ENET1_MDIO   /*!< Pin name */
#define BOARD_INITPINS_I2C4_3V3_SDA_PIN_FUNCTION_ID              SC_P_ENET1_MDIO   /*!< Pin function id */
#define BOARD_INITPINS_I2C4_3V3_SDA_LABEL                         "I2C4_3V3_SDA"   /*!< Label */
#define BOARD_INITPINS_I2C4_3V3_SDA_NAME                          "I2C4_3V3_SDA"   /*!< Identifier name */

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
void BOARD_InitPins(sc_ipc_t ipc);                         /*!< Function assigned for the core: Cortex-M4F[cm4_0] */

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
