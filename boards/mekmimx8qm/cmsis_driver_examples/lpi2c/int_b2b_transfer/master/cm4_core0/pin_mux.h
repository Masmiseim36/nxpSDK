/*
 * Copyright 2017-2020 NXP
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
/* Routed pin properties */
#define BOARD_INITPINS_FTDI_M40_UART0_RX_PERIPHERAL                   M40__UART0   /*!< Peripheral name */
#define BOARD_INITPINS_FTDI_M40_UART0_RX_SIGNAL                          uart_rx   /*!< Signal name */
#define BOARD_INITPINS_FTDI_M40_UART0_RX_PIN_NAME                   M40_I2C0_SCL   /*!< Routed pin name */
#define BOARD_INITPINS_FTDI_M40_UART0_RX_PIN_FUNCTION_ID       SC_P_M40_I2C0_SCL   /*!< Pin function id */
#define BOARD_INITPINS_FTDI_M40_UART0_RX_LABEL               "FTDI_M40_UART0_RX"   /*!< Label */
#define BOARD_INITPINS_FTDI_M40_UART0_RX_NAME                "FTDI_M40_UART0_RX"   /*!< Identifier */

/* M40_I2C0_SDA (number AU51), FTDI_M40_UART0_TX */
/* Routed pin properties */
#define BOARD_INITPINS_FTDI_M40_UART0_TX_PERIPHERAL                   M40__UART0   /*!< Peripheral name */
#define BOARD_INITPINS_FTDI_M40_UART0_TX_SIGNAL                          uart_tx   /*!< Signal name */
#define BOARD_INITPINS_FTDI_M40_UART0_TX_PIN_NAME                   M40_I2C0_SDA   /*!< Routed pin name */
#define BOARD_INITPINS_FTDI_M40_UART0_TX_PIN_FUNCTION_ID       SC_P_M40_I2C0_SDA   /*!< Pin function id */
#define BOARD_INITPINS_FTDI_M40_UART0_TX_LABEL               "FTDI_M40_UART0_TX"   /*!< Label */
#define BOARD_INITPINS_FTDI_M40_UART0_TX_NAME                "FTDI_M40_UART0_TX"   /*!< Identifier */

/* HDMI_TX0_TS_SCL (number BN9), I2C0_3V3_SCL */
/* Routed pin properties */
#define LPI2C2_INITPINSEX_I2C0_3V3_SCL_PERIPHERAL                      DMA__I2C0   /*!< Peripheral name */
#define LPI2C2_INITPINSEX_I2C0_3V3_SCL_SIGNAL                            i2c_scl   /*!< Signal name */
#define LPI2C2_INITPINSEX_I2C0_3V3_SCL_PIN_NAME                  HDMI_TX0_TS_SCL   /*!< Routed pin name */
#define LPI2C2_INITPINSEX_I2C0_3V3_SCL_PIN_FUNCTION_ID      SC_P_HDMI_TX0_TS_SCL   /*!< Pin function id */
#define LPI2C2_INITPINSEX_I2C0_3V3_SCL_LABEL                      "I2C0_3V3_SCL"   /*!< Label */
#define LPI2C2_INITPINSEX_I2C0_3V3_SCL_NAME                       "I2C0_3V3_SCL"   /*!< Identifier */

/* HDMI_TX0_TS_SDA (number BN7), I2C0_3V3_SDA */
/* Routed pin properties */
#define LPI2C2_INITPINSEX_I2C0_3V3_SDA_PERIPHERAL                      DMA__I2C0   /*!< Peripheral name */
#define LPI2C2_INITPINSEX_I2C0_3V3_SDA_SIGNAL                            i2c_sda   /*!< Signal name */
#define LPI2C2_INITPINSEX_I2C0_3V3_SDA_PIN_NAME                  HDMI_TX0_TS_SDA   /*!< Routed pin name */
#define LPI2C2_INITPINSEX_I2C0_3V3_SDA_PIN_FUNCTION_ID      SC_P_HDMI_TX0_TS_SDA   /*!< Pin function id */
#define LPI2C2_INITPINSEX_I2C0_3V3_SDA_LABEL                      "I2C0_3V3_SDA"   /*!< Label */
#define LPI2C2_INITPINSEX_I2C0_3V3_SDA_NAME                       "I2C0_3V3_SDA"   /*!< Identifier */

/* HDMI_TX0_TS_SCL (number BN9), I2C0_3V3_SCL */
/* Routed pin properties */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SCL_PERIPHERAL                  LSIO__GPIO2   /*!< Peripheral name */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SCL_SIGNAL                          gpio_io   /*!< Signal name */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SCL_CHANNEL                              02   /*!< Signal channel */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SCL_PIN_NAME                HDMI_TX0_TS_SCL   /*!< Routed pin name */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SCL_PIN_FUNCTION_ID    SC_P_HDMI_TX0_TS_SCL   /*!< Pin function id */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SCL_LABEL                    "I2C0_3V3_SCL"   /*!< Label */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SCL_NAME                     "I2C0_3V3_SCL"   /*!< Identifier */

/* Symbols to be used with GPIO driver */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SCL_GPIO                        LSIO__GPIO2   /*!< GPIO peripheral base pointer */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SCL_GPIO_PIN                             2U   /*!< GPIO pin number */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SCL_GPIO_PIN_MASK                (1U << 2U)   /*!< GPIO pin mask */

/* HDMI_TX0_TS_SDA (number BN7), I2C0_3V3_SDA */
/* Routed pin properties */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SDA_PERIPHERAL                  LSIO__GPIO2   /*!< Peripheral name */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SDA_SIGNAL                          gpio_io   /*!< Signal name */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SDA_CHANNEL                              03   /*!< Signal channel */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SDA_PIN_NAME                HDMI_TX0_TS_SDA   /*!< Routed pin name */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SDA_PIN_FUNCTION_ID    SC_P_HDMI_TX0_TS_SDA   /*!< Pin function id */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SDA_LABEL                    "I2C0_3V3_SDA"   /*!< Label */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SDA_NAME                     "I2C0_3V3_SDA"   /*!< Identifier */

/* Symbols to be used with GPIO driver */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SDA_GPIO                        LSIO__GPIO2   /*!< GPIO peripheral base pointer */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SDA_GPIO_PIN                             3U   /*!< GPIO pin number */
#define LPI2C2_DEINITPINSEX_I2C0_3V3_SDA_GPIO_PIN_MASK                (1U << 3U)   /*!< GPIO pin mask */

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
void LPI2C2_InitPinsEx(sc_ipc_t ipc);                      /*!< Function assigned for the core: Cortex-M4F[cm4_core0] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 * @param ipc scfw ipchandle.
 *
 */
void LPI2C2_DeinitPinsEx(sc_ipc_t ipc);                    /*!< Function assigned for the core: Cortex-M4F[cm4_core0] */

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
