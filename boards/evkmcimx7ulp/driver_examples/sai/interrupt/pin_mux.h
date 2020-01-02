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

/* PTA4 (number AD15), I2S0_MCLK */
#define BOARD_I2S0_MCLK_PERIPHERAL                                          I2S0   /*!< Device name: I2S0 */
#define BOARD_I2S0_MCLK_SIGNAL                                          i2s_mclk   /*!< I2S0 signal: i2s_mclk */
#define BOARD_I2S0_MCLK_PIN_NAME                                            PTA4   /*!< Pin name */
#define BOARD_I2S0_MCLK_PIN_FUNCTION_ID                    IOMUXC_PTA4_I2S0_MCLK   /*!< Pin function id */
#define BOARD_I2S0_MCLK_LABEL                                        "I2S0_MCLK"   /*!< Label */
#define BOARD_I2S0_MCLK_NAME                                         "I2S0_MCLK"   /*!< Identifier name */

/* PTA5 (number AC15), I2S0_TX_BCLK */
#define BOARD_I2S0_TX_BCLK_PERIPHERAL                                       I2S0   /*!< Device name: I2S0 */
#define BOARD_I2S0_TX_BCLK_SIGNAL                                    i2s_tx_bclk   /*!< I2S0 signal: i2s_tx_bclk */
#define BOARD_I2S0_TX_BCLK_PIN_NAME                                         PTA5   /*!< Pin name */
#define BOARD_I2S0_TX_BCLK_PIN_FUNCTION_ID              IOMUXC_PTA5_I2S0_TX_BCLK   /*!< Pin function id */
#define BOARD_I2S0_TX_BCLK_LABEL                                  "I2S0_TX_BCLK"   /*!< Label */
#define BOARD_I2S0_TX_BCLK_NAME                                   "I2S0_TX_BCLK"   /*!< Identifier name */

/* PTA6 (number AB15), I2S0_TX_FS */
#define BOARD_I2S0_TX_FS_PERIPHERAL                                         I2S0   /*!< Device name: I2S0 */
#define BOARD_I2S0_TX_FS_SIGNAL                                        i2s_tx_fs   /*!< I2S0 signal: i2s_tx_fs */
#define BOARD_I2S0_TX_FS_PIN_NAME                                           PTA6   /*!< Pin name */
#define BOARD_I2S0_TX_FS_PIN_FUNCTION_ID                  IOMUXC_PTA6_I2S0_TX_FS   /*!< Pin function id */
#define BOARD_I2S0_TX_FS_LABEL                                      "I2S0_TX_FS"   /*!< Label */
#define BOARD_I2S0_TX_FS_NAME                                       "I2S0_TX_FS"   /*!< Identifier name */

/* PTA7 (number AD14), I2S0_TXD0 */
#define BOARD_I2S0_TXD0_PERIPHERAL                                          I2S0   /*!< Device name: I2S0 */
#define BOARD_I2S0_TXD0_SIGNAL                                           i2s_txd   /*!< I2S0 signal: i2s_txd */
#define BOARD_I2S0_TXD0_CHANNEL                                                0   /*!< I2S0 i2s_txd channel: 0 */
#define BOARD_I2S0_TXD0_PIN_NAME                                            PTA7   /*!< Pin name */
#define BOARD_I2S0_TXD0_PIN_FUNCTION_ID                    IOMUXC_PTA7_I2S0_TXD0   /*!< Pin function id */
#define BOARD_I2S0_TXD0_LABEL                                        "I2S0_TXD0"   /*!< Label */
#define BOARD_I2S0_TXD0_NAME                                         "I2S0_TXD0"   /*!< Identifier name */

/* PTA2 (number AG14), I2S0_RXD0 */
#define BOARD_I2S0_RXD0_PERIPHERAL                                          I2S0   /*!< Device name: I2S0 */
#define BOARD_I2S0_RXD0_SIGNAL                                           i2s_rxd   /*!< I2S0 signal: i2s_rxd */
#define BOARD_I2S0_RXD0_CHANNEL                                                0   /*!< I2S0 i2s_rxd channel: 0 */
#define BOARD_I2S0_RXD0_PIN_NAME                                            PTA2   /*!< Pin name */
#define BOARD_I2S0_RXD0_PIN_FUNCTION_ID                    IOMUXC_PTA2_I2S0_RXD0   /*!< Pin function id */
#define BOARD_I2S0_RXD0_LABEL                                        "I2S0_RXD0"   /*!< Label */
#define BOARD_I2S0_RXD0_NAME                                         "I2S0_RXD0"   /*!< Identifier name */

/* PTA16 (number AG18), I2C0_SCL */
#define BOARD_I2C0_SCL_PERIPHERAL                                         LPI2C0   /*!< Device name: LPI2C0 */
#define BOARD_I2C0_SCL_SIGNAL                                          lpi2c_scl   /*!< LPI2C0 signal: lpi2c_scl */
#define BOARD_I2C0_SCL_PIN_NAME                                            PTA16   /*!< Pin name */
#define BOARD_I2C0_SCL_PIN_FUNCTION_ID                   IOMUXC_PTA16_LPI2C0_SCL   /*!< Pin function id */
#define BOARD_I2C0_SCL_LABEL                                          "I2C0_SCL"   /*!< Label */
#define BOARD_I2C0_SCL_NAME                                           "I2C0_SCL"   /*!< Identifier name */

/* PTA17 (number AD19), I2C0_SDA */
#define BOARD_I2C0_SDA_PERIPHERAL                                         LPI2C0   /*!< Device name: LPI2C0 */
#define BOARD_I2C0_SDA_SIGNAL                                          lpi2c_sda   /*!< LPI2C0 signal: lpi2c_sda */
#define BOARD_I2C0_SDA_PIN_NAME                                            PTA17   /*!< Pin name */
#define BOARD_I2C0_SDA_PIN_FUNCTION_ID                   IOMUXC_PTA17_LPI2C0_SDA   /*!< Pin function id */
#define BOARD_I2C0_SDA_LABEL                                          "I2C0_SDA"   /*!< Label */
#define BOARD_I2C0_SDA_NAME                                           "I2C0_SDA"   /*!< Identifier name */

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

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_I2C_ConfigurePins(void);                        /*!< Function assigned for the core: Cortex-M4[cm4] */

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
