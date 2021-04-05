/*
 * Copyright 2020 NXP
 * All rights reserved.
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
#define BOARD_INITPINS_FTDI_M40_UART0_RX_PIN_FUNCTION_ID       SC_P_M40_I2C0_SCL   /*!< Pin function id */

/* M40_I2C0_SDA (number AU51), FTDI_M40_UART0_TX */
#define BOARD_INITPINS_FTDI_M40_UART0_TX_PIN_FUNCTION_ID       SC_P_M40_I2C0_SDA   /*!< Pin function id */

/* ENET0_MDC (coord A9), ENET0_MDC */
#define ENET0_INITPINSEX_ENET0_MDC_PIN_FUNCTION_ID                SC_P_ENET0_MDC   /*!< Pin function id */

/* ENET0_MDIO (coord D10), ENET0_MDIO */
#define ENET0_INITPINSEX_ENET0_MDIO_PIN_FUNCTION_ID              SC_P_ENET0_MDIO   /*!< Pin function id */

/* ENET0_REFCLK_125M_25M (coord B10), BB_ARD_MIK_RST_B/J20D[41] */
#define ENET0_INITPINSEX_BB_ARD_MIK_RST_B_PIN_FUNCTION_ID SC_P_ENET0_REFCLK_125M_25M /*!< Pin function id */

/* ENET0_RGMII_RXC (coord B44), ETH0_RGMII_RXC */
#define ENET0_INITPINSEX_ETH0_RGMII_RXC_PIN_FUNCTION_ID     SC_P_ENET0_RGMII_RXC   /*!< Pin function id */

/* ENET0_RGMII_RXD0 (coord A47), ETH0_RGMII_RXD0 */
#define ENET0_INITPINSEX_ETH0_RGMII_RXD0_PIN_FUNCTION_ID   SC_P_ENET0_RGMII_RXD0   /*!< Pin function id */

/* ENET0_RGMII_RXD1 (coord D44), ETH0_RGMII_RXD1 */
#define ENET0_INITPINSEX_ETH0_RGMII_RXD1_PIN_FUNCTION_ID   SC_P_ENET0_RGMII_RXD1   /*!< Pin function id */

/* ENET0_RGMII_RXD2 (coord C45), ETH0_RGMII_RXD2 */
#define ENET0_INITPINSEX_ETH0_RGMII_RXD2_PIN_FUNCTION_ID   SC_P_ENET0_RGMII_RXD2   /*!< Pin function id */

/* ENET0_RGMII_TX_CTL (coord E41), ETH0_RGMII_TX_CTL */
#define ENET0_INITPINSEX_ETH0_RGMII_TX_CTL_PIN_FUNCTION_ID SC_P_ENET0_RGMII_TX_CTL /*!< Pin function id */

/* ENET0_RGMII_TXD3 (coord D42), ETH0_RGMII_TXD3 */
#define ENET0_INITPINSEX_ETH0_RGMII_TXD3_PIN_FUNCTION_ID   SC_P_ENET0_RGMII_TXD3   /*!< Pin function id */

/* ENET0_RGMII_TXD2 (coord A45), ETH0_RGMII_TXD2 */
#define ENET0_INITPINSEX_ETH0_RGMII_TXD2_PIN_FUNCTION_ID   SC_P_ENET0_RGMII_TXD2   /*!< Pin function id */

/* ENET0_RGMII_TXD1 (coord B42), ETH0_RGMII_TXD1 */
#define ENET0_INITPINSEX_ETH0_RGMII_TXD1_PIN_FUNCTION_ID   SC_P_ENET0_RGMII_TXD1   /*!< Pin function id */

/* ENET0_RGMII_TXD0 (coord A43), ETH0_RGMII_TXD0 */
#define ENET0_INITPINSEX_ETH0_RGMII_TXD0_PIN_FUNCTION_ID   SC_P_ENET0_RGMII_TXD0   /*!< Pin function id */

/* ENET0_RGMII_RXD3 (coord E45), ETH0_RGMII_RXD3 */
#define ENET0_INITPINSEX_ETH0_RGMII_RXD3_PIN_FUNCTION_ID   SC_P_ENET0_RGMII_RXD3   /*!< Pin function id */

/* ENET0_RGMII_RX_CTL (coord E43), ETH0_RGMII_RX_CTL */
#define ENET0_INITPINSEX_ETH0_RGMII_RX_CTL_PIN_FUNCTION_ID SC_P_ENET0_RGMII_RX_CTL /*!< Pin function id */

/* ENET0_RGMII_TXC (coord A41), ETH0_RGMII_TXC */
#define ENET0_INITPINSEX_ETH0_RGMII_TXC_PIN_FUNCTION_ID     SC_P_ENET0_RGMII_TXC   /*!< Pin function id */

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
void BOARD_InitPins(sc_ipc_t ipc);                         /*!< Function assigned for the core: Cortex-M4F[cm4_core0] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 * @param ipc scfw ipchandle.
 *
 */
void ENET0_InitPinsEx(sc_ipc_t ipc);                       /*!< Function assigned for the core: Cortex-M4F[cm4_core0] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 * @param ipc scfw ipchandle.
 *
 */
void ENET0_DeinitPinsEx(sc_ipc_t ipc);                     /*!< Function assigned for the core: Cortex-M4F[cm4_core0] */

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
