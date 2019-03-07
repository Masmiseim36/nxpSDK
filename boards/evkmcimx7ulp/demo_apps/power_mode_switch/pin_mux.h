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

/* PTA25 (number AG20), A7_POW_EN# */
#define BOARD_INITPINS_A7_POW_EN_PERIPHERAL                                  PTA   /*!< Device name: PTA */
#define BOARD_INITPINS_A7_POW_EN_SIGNAL                                     port   /*!< PTA signal: port */
#define BOARD_INITPINS_A7_POW_EN_CHANNEL                                      25   /*!< PTA port channel: 25 */
#define BOARD_INITPINS_A7_POW_EN_PIN_NAME                                  PTA25   /*!< Pin name */
#define BOARD_INITPINS_A7_POW_EN_PIN_FUNCTION_ID              IOMUXC_PTA25_PTA25   /*!< Pin function id */
#define BOARD_INITPINS_A7_POW_EN_LABEL                              "A7_POW_EN#"   /*!< Label */
#define BOARD_INITPINS_A7_POW_EN_NAME                                "A7_POW_EN"   /*!< Identifier name */

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

/* PTA4 (number AD15), I2S0_MCLK */
#define BOARD_INITPINS_I2S0_MCLK_PERIPHERAL                                 I2S0   /*!< Device name: I2S0 */
#define BOARD_INITPINS_I2S0_MCLK_SIGNAL                                 i2s_mclk   /*!< I2S0 signal: i2s_mclk */
#define BOARD_INITPINS_I2S0_MCLK_PIN_NAME                                   PTA4   /*!< Pin name */
#define BOARD_INITPINS_I2S0_MCLK_PIN_FUNCTION_ID           IOMUXC_PTA4_I2S0_MCLK   /*!< Pin function id */
#define BOARD_INITPINS_I2S0_MCLK_LABEL                               "I2S0_MCLK"   /*!< Label */
#define BOARD_INITPINS_I2S0_MCLK_NAME                                "I2S0_MCLK"   /*!< Identifier name */

/* PTA2 (number AG14), I2S0_RXD0 */
#define BOARD_INITPINS_I2S0_RXD0_PERIPHERAL                                 I2S0   /*!< Device name: I2S0 */
#define BOARD_INITPINS_I2S0_RXD0_SIGNAL                                  i2s_rxd   /*!< I2S0 signal: i2s_rxd */
#define BOARD_INITPINS_I2S0_RXD0_CHANNEL                                       0   /*!< I2S0 i2s_rxd channel: 0 */
#define BOARD_INITPINS_I2S0_RXD0_PIN_NAME                                   PTA2   /*!< Pin name */
#define BOARD_INITPINS_I2S0_RXD0_PIN_FUNCTION_ID           IOMUXC_PTA2_I2S0_RXD0   /*!< Pin function id */
#define BOARD_INITPINS_I2S0_RXD0_LABEL                               "I2S0_RXD0"   /*!< Label */
#define BOARD_INITPINS_I2S0_RXD0_NAME                                "I2S0_RXD0"   /*!< Identifier name */

/* PTA5 (number AC15), I2S0_TX_BCLK */
#define BOARD_INITPINS_I2S0_TX_BCLK_PERIPHERAL                              I2S0   /*!< Device name: I2S0 */
#define BOARD_INITPINS_I2S0_TX_BCLK_SIGNAL                           i2s_tx_bclk   /*!< I2S0 signal: i2s_tx_bclk */
#define BOARD_INITPINS_I2S0_TX_BCLK_PIN_NAME                                PTA5   /*!< Pin name */
#define BOARD_INITPINS_I2S0_TX_BCLK_PIN_FUNCTION_ID     IOMUXC_PTA5_I2S0_TX_BCLK   /*!< Pin function id */
#define BOARD_INITPINS_I2S0_TX_BCLK_LABEL                         "I2S0_TX_BCLK"   /*!< Label */
#define BOARD_INITPINS_I2S0_TX_BCLK_NAME                          "I2S0_TX_BCLK"   /*!< Identifier name */

/* PTA6 (number AB15), I2S0_TX_FS */
#define BOARD_INITPINS_I2S0_TX_FS_PERIPHERAL                                I2S0   /*!< Device name: I2S0 */
#define BOARD_INITPINS_I2S0_TX_FS_SIGNAL                               i2s_tx_fs   /*!< I2S0 signal: i2s_tx_fs */
#define BOARD_INITPINS_I2S0_TX_FS_PIN_NAME                                  PTA6   /*!< Pin name */
#define BOARD_INITPINS_I2S0_TX_FS_PIN_FUNCTION_ID         IOMUXC_PTA6_I2S0_TX_FS   /*!< Pin function id */
#define BOARD_INITPINS_I2S0_TX_FS_LABEL                             "I2S0_TX_FS"   /*!< Label */
#define BOARD_INITPINS_I2S0_TX_FS_NAME                              "I2S0_TX_FS"   /*!< Identifier name */

/* PTA7 (number AD14), I2S0_TXD0 */
#define BOARD_INITPINS_I2S0_TXD0_PERIPHERAL                                 I2S0   /*!< Device name: I2S0 */
#define BOARD_INITPINS_I2S0_TXD0_SIGNAL                                  i2s_txd   /*!< I2S0 signal: i2s_txd */
#define BOARD_INITPINS_I2S0_TXD0_CHANNEL                                       0   /*!< I2S0 i2s_txd channel: 0 */
#define BOARD_INITPINS_I2S0_TXD0_PIN_NAME                                   PTA7   /*!< Pin name */
#define BOARD_INITPINS_I2S0_TXD0_PIN_FUNCTION_ID           IOMUXC_PTA7_I2S0_TXD0   /*!< Pin function id */
#define BOARD_INITPINS_I2S0_TXD0_LABEL                               "I2S0_TXD0"   /*!< Label */
#define BOARD_INITPINS_I2S0_TXD0_NAME                                "I2S0_TXD0"   /*!< Identifier name */

/* PTA3 (number AF14), VOL+ */
#define BOARD_INITPINS_VOL_UP_PERIPHERAL                                     PTA   /*!< Device name: PTA */
#define BOARD_INITPINS_VOL_UP_SIGNAL                                        port   /*!< PTA signal: port */
#define BOARD_INITPINS_VOL_UP_CHANNEL                                          3   /*!< PTA port channel: 3 */
#define BOARD_INITPINS_VOL_UP_PIN_NAME                                      PTA3   /*!< Pin name */
#define BOARD_INITPINS_VOL_UP_PIN_FUNCTION_ID                   IOMUXC_PTA3_PTA3   /*!< Pin function id */
#define BOARD_INITPINS_VOL_UP_LABEL                                       "VOL+"   /*!< Label */
#define BOARD_INITPINS_VOL_UP_NAME                                      "VOL_UP"   /*!< Identifier name */

/* PTA13 (number AF16), VOL- */
#define BOARD_INITPINS_VOL_DOWN_PERIPHERAL                                   PTA   /*!< Device name: PTA */
#define BOARD_INITPINS_VOL_DOWN_SIGNAL                                      port   /*!< PTA signal: port */
#define BOARD_INITPINS_VOL_DOWN_CHANNEL                                       13   /*!< PTA port channel: 13 */
#define BOARD_INITPINS_VOL_DOWN_PIN_NAME                                   PTA13   /*!< Pin name */
#define BOARD_INITPINS_VOL_DOWN_PIN_FUNCTION_ID               IOMUXC_PTA13_PTA13   /*!< Pin function id */
#define BOARD_INITPINS_VOL_DOWN_LABEL                                     "VOL-"   /*!< Label */
#define BOARD_INITPINS_VOL_DOWN_NAME                                  "VOL_DOWN"   /*!< Identifier name */

/* PTA14 (number AF17), WL_REG_ON */
#define BOARD_INITPINS_WL_REG_ON_PERIPHERAL                                  PTA   /*!< Device name: PTA */
#define BOARD_INITPINS_WL_REG_ON_SIGNAL                                     port   /*!< PTA signal: port */
#define BOARD_INITPINS_WL_REG_ON_CHANNEL                                      14   /*!< PTA port channel: 14 */
#define BOARD_INITPINS_WL_REG_ON_PIN_NAME                                  PTA14   /*!< Pin name */
#define BOARD_INITPINS_WL_REG_ON_PIN_FUNCTION_ID              IOMUXC_PTA14_PTA14   /*!< Pin function id */
#define BOARD_INITPINS_WL_REG_ON_LABEL                               "WL_REG_ON"   /*!< Label */
#define BOARD_INITPINS_WL_REG_ON_NAME                                "WL_REG_ON"   /*!< Identifier name */

/* PTA15 (number AF18), BT_REG_ON */
#define BOARD_INITPINS_BT_REG_ON_PERIPHERAL                                  PTA   /*!< Device name: PTA */
#define BOARD_INITPINS_BT_REG_ON_SIGNAL                                     port   /*!< PTA signal: port */
#define BOARD_INITPINS_BT_REG_ON_CHANNEL                                      15   /*!< PTA port channel: 15 */
#define BOARD_INITPINS_BT_REG_ON_PIN_NAME                                  PTA15   /*!< Pin name */
#define BOARD_INITPINS_BT_REG_ON_PIN_FUNCTION_ID              IOMUXC_PTA15_PTA15   /*!< Pin function id */
#define BOARD_INITPINS_BT_REG_ON_LABEL                               "BT_REG_ON"   /*!< Label */
#define BOARD_INITPINS_BT_REG_ON_NAME                                "BT_REG_ON"   /*!< Identifier name */

/* PTA31 (number AF24), WL_HOST_WAKE */
#define BOARD_INITPINS_WL_HOST_WAKE_PERIPHERAL                               PTA   /*!< Device name: PTA */
#define BOARD_INITPINS_WL_HOST_WAKE_SIGNAL                                  port   /*!< PTA signal: port */
#define BOARD_INITPINS_WL_HOST_WAKE_CHANNEL                                   31   /*!< PTA port channel: 31 */
#define BOARD_INITPINS_WL_HOST_WAKE_PIN_NAME                               PTA31   /*!< Pin name */
#define BOARD_INITPINS_WL_HOST_WAKE_PIN_FUNCTION_ID           IOMUXC_PTA31_PTA31   /*!< Pin function id */
#define BOARD_INITPINS_WL_HOST_WAKE_LABEL                         "WL_HOST_WAKE"   /*!< Label */
#define BOARD_INITPINS_WL_HOST_WAKE_NAME                          "WL_HOST_WAKE"   /*!< Identifier name */

/* PTB1 (number AF2), I2S1_TX_BCLK */
#define BOARD_INITPINS_I2S1_TX_BCLK_PERIPHERAL                               PTB   /*!< Device name: PTB */
#define BOARD_INITPINS_I2S1_TX_BCLK_SIGNAL                                  port   /*!< PTB signal: port */
#define BOARD_INITPINS_I2S1_TX_BCLK_CHANNEL                                    1   /*!< PTB port channel: 1 */
#define BOARD_INITPINS_I2S1_TX_BCLK_PIN_NAME                                PTB1   /*!< Pin name */
#define BOARD_INITPINS_I2S1_TX_BCLK_PIN_FUNCTION_ID             IOMUXC_PTB1_PTB1   /*!< Pin function id */
#define BOARD_INITPINS_I2S1_TX_BCLK_LABEL                         "I2S1_TX_BCLK"   /*!< Label */
#define BOARD_INITPINS_I2S1_TX_BCLK_NAME                          "I2S1_TX_BCLK"   /*!< Identifier name */

/* PTB2 (number AE3), I2S1_TX_FS */
#define BOARD_INITPINS_I2S1_TX_FS_PERIPHERAL                                 PTB   /*!< Device name: PTB */
#define BOARD_INITPINS_I2S1_TX_FS_SIGNAL                                    port   /*!< PTB signal: port */
#define BOARD_INITPINS_I2S1_TX_FS_CHANNEL                                      2   /*!< PTB port channel: 2 */
#define BOARD_INITPINS_I2S1_TX_FS_PIN_NAME                                  PTB2   /*!< Pin name */
#define BOARD_INITPINS_I2S1_TX_FS_PIN_FUNCTION_ID               IOMUXC_PTB2_PTB2   /*!< Pin function id */
#define BOARD_INITPINS_I2S1_TX_FS_LABEL                             "I2S1_TX_FS"   /*!< Label */
#define BOARD_INITPINS_I2S1_TX_FS_NAME                              "I2S1_TX_FS"   /*!< Identifier name */

/* PTB3 (number AE4), I2S1_TXD0 */
#define BOARD_INITPINS_I2S1_TXD0_PERIPHERAL                                  PTB   /*!< Device name: PTB */
#define BOARD_INITPINS_I2S1_TXD0_SIGNAL                                     port   /*!< PTB signal: port */
#define BOARD_INITPINS_I2S1_TXD0_CHANNEL                                       3   /*!< PTB port channel: 3 */
#define BOARD_INITPINS_I2S1_TXD0_PIN_NAME                                   PTB3   /*!< Pin name */
#define BOARD_INITPINS_I2S1_TXD0_PIN_FUNCTION_ID                IOMUXC_PTB3_PTB3   /*!< Pin function id */
#define BOARD_INITPINS_I2S1_TXD0_LABEL                               "I2S1_TXD0"   /*!< Label */
#define BOARD_INITPINS_I2S1_TXD0_NAME                                "I2S1_TXD0"   /*!< Identifier name */

/* PTB10 (number AC7), I2S1_RXD0 */
#define BOARD_INITPINS_I2S1_RXD0_PERIPHERAL                                  PTB   /*!< Device name: PTB */
#define BOARD_INITPINS_I2S1_RXD0_SIGNAL                                     port   /*!< PTB signal: port */
#define BOARD_INITPINS_I2S1_RXD0_CHANNEL                                      10   /*!< PTB port channel: 10 */
#define BOARD_INITPINS_I2S1_RXD0_PIN_NAME                                  PTB10   /*!< Pin name */
#define BOARD_INITPINS_I2S1_RXD0_PIN_FUNCTION_ID              IOMUXC_PTB10_PTB10   /*!< Pin function id */
#define BOARD_INITPINS_I2S1_RXD0_LABEL                               "I2S1_RXD0"   /*!< Label */
#define BOARD_INITPINS_I2S1_RXD0_NAME                                "I2S1_RXD0"   /*!< Identifier name */

/* PTB4 (number AG4), PTB4 */
#define BOARD_INITPINS_PTB4_PERIPHERAL                                       PTB   /*!< Device name: PTB */
#define BOARD_INITPINS_PTB4_SIGNAL                                          port   /*!< PTB signal: port */
#define BOARD_INITPINS_PTB4_CHANNEL                                            4   /*!< PTB port channel: 4 */
#define BOARD_INITPINS_PTB4_PIN_NAME                                        PTB4   /*!< Pin name */
#define BOARD_INITPINS_PTB4_PIN_FUNCTION_ID                     IOMUXC_PTB4_PTB4   /*!< Pin function id */
#define BOARD_INITPINS_PTB4_LABEL                                         "PTB4"   /*!< Label */
#define BOARD_INITPINS_PTB4_NAME                                          "PTB4"   /*!< Identifier name */

/* PTB5 (number AF4), PTB5 */
#define BOARD_INITPINS_PTB5_PERIPHERAL                                       PTB   /*!< Device name: PTB */
#define BOARD_INITPINS_PTB5_SIGNAL                                          port   /*!< PTB signal: port */
#define BOARD_INITPINS_PTB5_CHANNEL                                            5   /*!< PTB port channel: 5 */
#define BOARD_INITPINS_PTB5_PIN_NAME                                        PTB5   /*!< Pin name */
#define BOARD_INITPINS_PTB5_PIN_FUNCTION_ID                     IOMUXC_PTB5_PTB5   /*!< Pin function id */
#define BOARD_INITPINS_PTB5_LABEL                                         "PTB5"   /*!< Label */
#define BOARD_INITPINS_PTB5_NAME                                          "PTB5"   /*!< Identifier name */

/* PTB6 (number AF5), DDR_SW_EN# */
#define BOARD_INITPINS_DDR_SW_EN_PERIPHERAL                                  PTB   /*!< Device name: PTB */
#define BOARD_INITPINS_DDR_SW_EN_SIGNAL                                     port   /*!< PTB signal: port */
#define BOARD_INITPINS_DDR_SW_EN_CHANNEL                                       6   /*!< PTB port channel: 6 */
#define BOARD_INITPINS_DDR_SW_EN_PIN_NAME                                   PTB6   /*!< Pin name */
#define BOARD_INITPINS_DDR_SW_EN_PIN_FUNCTION_ID                IOMUXC_PTB6_PTB6   /*!< Pin function id */
#define BOARD_INITPINS_DDR_SW_EN_LABEL                              "DDR_SW_EN#"   /*!< Label */
#define BOARD_INITPINS_DDR_SW_EN_NAME                                "DDR_SW_EN"   /*!< Identifier name */

/* PTB7 (number AF6), BT_HOST_WAKE */
#define BOARD_INITPINS_BT_HOST_WAKE_PERIPHERAL                               PTB   /*!< Device name: PTB */
#define BOARD_INITPINS_BT_HOST_WAKE_SIGNAL                                  port   /*!< PTB signal: port */
#define BOARD_INITPINS_BT_HOST_WAKE_CHANNEL                                    7   /*!< PTB port channel: 7 */
#define BOARD_INITPINS_BT_HOST_WAKE_PIN_NAME                                PTB7   /*!< Pin name */
#define BOARD_INITPINS_BT_HOST_WAKE_PIN_FUNCTION_ID             IOMUXC_PTB7_PTB7   /*!< Pin function id */
#define BOARD_INITPINS_BT_HOST_WAKE_LABEL                         "BT_HOST_WAKE"   /*!< Label */
#define BOARD_INITPINS_BT_HOST_WAKE_NAME                          "BT_HOST_WAKE"   /*!< Identifier name */

/* PTB9 (number AD7), SEN_INT# */
#define BOARD_INITPINS_SEN_INT_PERIPHERAL                                    PTB   /*!< Device name: PTB */
#define BOARD_INITPINS_SEN_INT_SIGNAL                                       port   /*!< PTB signal: port */
#define BOARD_INITPINS_SEN_INT_CHANNEL                                         9   /*!< PTB port channel: 9 */
#define BOARD_INITPINS_SEN_INT_PIN_NAME                                     PTB9   /*!< Pin name */
#define BOARD_INITPINS_SEN_INT_PIN_FUNCTION_ID                  IOMUXC_PTB9_PTB9   /*!< Pin function id */
#define BOARD_INITPINS_SEN_INT_LABEL                                  "SEN_INT#"   /*!< Label */
#define BOARD_INITPINS_SEN_INT_NAME                                    "SEN_INT"   /*!< Identifier name */

/* PTB11 (number AB7), PMIC_INT# */
#define BOARD_INITPINS_PMIC_INT_PERIPHERAL                                   PTB   /*!< Device name: PTB */
#define BOARD_INITPINS_PMIC_INT_SIGNAL                                      port   /*!< PTB signal: port */
#define BOARD_INITPINS_PMIC_INT_CHANNEL                                       11   /*!< PTB port channel: 11 */
#define BOARD_INITPINS_PMIC_INT_PIN_NAME                                   PTB11   /*!< Pin name */
#define BOARD_INITPINS_PMIC_INT_PIN_FUNCTION_ID               IOMUXC_PTB11_PTB11   /*!< Pin function id */
#define BOARD_INITPINS_PMIC_INT_LABEL                                "PMIC_INT#"   /*!< Label */
#define BOARD_INITPINS_PMIC_INT_NAME                                  "PMIC_INT"   /*!< Identifier name */

/* PTB14 (number AG8), RTC_CLK */
#define BOARD_INITPINS_RTC_CLK_PERIPHERAL                                    RTC   /*!< Device name: RTC */
#define BOARD_INITPINS_RTC_CLK_SIGNAL                                 rtc_clkout   /*!< RTC signal: rtc_clkout */
#define BOARD_INITPINS_RTC_CLK_PIN_NAME                                    PTB14   /*!< Pin name */
#define BOARD_INITPINS_RTC_CLK_PIN_FUNCTION_ID           IOMUXC_PTB14_RTC_CLKOUT   /*!< Pin function id */
#define BOARD_INITPINS_RTC_CLK_LABEL                                   "RTC_CLK"   /*!< Label */
#define BOARD_INITPINS_RTC_CLK_NAME                                    "RTC_CLK"   /*!< Identifier name */

/* PTA25 (number AG20), A7_POW_EN# */
#define BOARD_INITPINSNOQSPI_A7_POW_EN_PERIPHERAL                            PTA   /*!< Device name: PTA */
#define BOARD_INITPINSNOQSPI_A7_POW_EN_SIGNAL                               port   /*!< PTA signal: port */
#define BOARD_INITPINSNOQSPI_A7_POW_EN_CHANNEL                                25   /*!< PTA port channel: 25 */
#define BOARD_INITPINSNOQSPI_A7_POW_EN_PIN_NAME                            PTA25   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_A7_POW_EN_PIN_FUNCTION_ID        IOMUXC_PTA25_PTA25   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_A7_POW_EN_LABEL                        "A7_POW_EN#"   /*!< Label */
#define BOARD_INITPINSNOQSPI_A7_POW_EN_NAME                          "A7_POW_EN"   /*!< Identifier name */

/* PTA19 (number AB19), UART0_RX */
#define BOARD_INITPINSNOQSPI_UART0_RX_PERIPHERAL                         LPUART0   /*!< Device name: LPUART0 */
#define BOARD_INITPINSNOQSPI_UART0_RX_SIGNAL                           lpuart_rx   /*!< LPUART0 signal: lpuart_rx */
#define BOARD_INITPINSNOQSPI_UART0_RX_PIN_NAME                             PTA19   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_UART0_RX_PIN_FUNCTION_ID    IOMUXC_PTA19_LPUART0_RX   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_UART0_RX_LABEL                           "UART0_RX"   /*!< Label */
#define BOARD_INITPINSNOQSPI_UART0_RX_NAME                            "UART0_RX"   /*!< Identifier name */

/* PTA18 (number AC19), UART0_TX */
#define BOARD_INITPINSNOQSPI_UART0_TX_PERIPHERAL                         LPUART0   /*!< Device name: LPUART0 */
#define BOARD_INITPINSNOQSPI_UART0_TX_SIGNAL                           lpuart_tx   /*!< LPUART0 signal: lpuart_tx */
#define BOARD_INITPINSNOQSPI_UART0_TX_PIN_NAME                             PTA18   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_UART0_TX_PIN_FUNCTION_ID    IOMUXC_PTA18_LPUART0_TX   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_UART0_TX_LABEL                           "UART0_TX"   /*!< Label */
#define BOARD_INITPINSNOQSPI_UART0_TX_NAME                            "UART0_TX"   /*!< Identifier name */

/* PTA4 (number AD15), I2S0_MCLK */
#define BOARD_INITPINSNOQSPI_I2S0_MCLK_PERIPHERAL                           I2S0   /*!< Device name: I2S0 */
#define BOARD_INITPINSNOQSPI_I2S0_MCLK_SIGNAL                           i2s_mclk   /*!< I2S0 signal: i2s_mclk */
#define BOARD_INITPINSNOQSPI_I2S0_MCLK_PIN_NAME                             PTA4   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_I2S0_MCLK_PIN_FUNCTION_ID     IOMUXC_PTA4_I2S0_MCLK   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_I2S0_MCLK_LABEL                         "I2S0_MCLK"   /*!< Label */
#define BOARD_INITPINSNOQSPI_I2S0_MCLK_NAME                          "I2S0_MCLK"   /*!< Identifier name */

/* PTA2 (number AG14), I2S0_RXD0 */
#define BOARD_INITPINSNOQSPI_I2S0_RXD0_PERIPHERAL                           I2S0   /*!< Device name: I2S0 */
#define BOARD_INITPINSNOQSPI_I2S0_RXD0_SIGNAL                            i2s_rxd   /*!< I2S0 signal: i2s_rxd */
#define BOARD_INITPINSNOQSPI_I2S0_RXD0_CHANNEL                                 0   /*!< I2S0 i2s_rxd channel: 0 */
#define BOARD_INITPINSNOQSPI_I2S0_RXD0_PIN_NAME                             PTA2   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_I2S0_RXD0_PIN_FUNCTION_ID     IOMUXC_PTA2_I2S0_RXD0   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_I2S0_RXD0_LABEL                         "I2S0_RXD0"   /*!< Label */
#define BOARD_INITPINSNOQSPI_I2S0_RXD0_NAME                          "I2S0_RXD0"   /*!< Identifier name */

/* PTA5 (number AC15), I2S0_TX_BCLK */
#define BOARD_INITPINSNOQSPI_I2S0_TX_BCLK_PERIPHERAL                        I2S0   /*!< Device name: I2S0 */
#define BOARD_INITPINSNOQSPI_I2S0_TX_BCLK_SIGNAL                     i2s_tx_bclk   /*!< I2S0 signal: i2s_tx_bclk */
#define BOARD_INITPINSNOQSPI_I2S0_TX_BCLK_PIN_NAME                          PTA5   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_I2S0_TX_BCLK_PIN_FUNCTION_ID IOMUXC_PTA5_I2S0_TX_BCLK /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_I2S0_TX_BCLK_LABEL                   "I2S0_TX_BCLK"   /*!< Label */
#define BOARD_INITPINSNOQSPI_I2S0_TX_BCLK_NAME                    "I2S0_TX_BCLK"   /*!< Identifier name */

/* PTA6 (number AB15), I2S0_TX_FS */
#define BOARD_INITPINSNOQSPI_I2S0_TX_FS_PERIPHERAL                          I2S0   /*!< Device name: I2S0 */
#define BOARD_INITPINSNOQSPI_I2S0_TX_FS_SIGNAL                         i2s_tx_fs   /*!< I2S0 signal: i2s_tx_fs */
#define BOARD_INITPINSNOQSPI_I2S0_TX_FS_PIN_NAME                            PTA6   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_I2S0_TX_FS_PIN_FUNCTION_ID   IOMUXC_PTA6_I2S0_TX_FS   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_I2S0_TX_FS_LABEL                       "I2S0_TX_FS"   /*!< Label */
#define BOARD_INITPINSNOQSPI_I2S0_TX_FS_NAME                        "I2S0_TX_FS"   /*!< Identifier name */

/* PTA7 (number AD14), I2S0_TXD0 */
#define BOARD_INITPINSNOQSPI_I2S0_TXD0_PERIPHERAL                           I2S0   /*!< Device name: I2S0 */
#define BOARD_INITPINSNOQSPI_I2S0_TXD0_SIGNAL                            i2s_txd   /*!< I2S0 signal: i2s_txd */
#define BOARD_INITPINSNOQSPI_I2S0_TXD0_CHANNEL                                 0   /*!< I2S0 i2s_txd channel: 0 */
#define BOARD_INITPINSNOQSPI_I2S0_TXD0_PIN_NAME                             PTA7   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_I2S0_TXD0_PIN_FUNCTION_ID     IOMUXC_PTA7_I2S0_TXD0   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_I2S0_TXD0_LABEL                         "I2S0_TXD0"   /*!< Label */
#define BOARD_INITPINSNOQSPI_I2S0_TXD0_NAME                          "I2S0_TXD0"   /*!< Identifier name */

/* PTA3 (number AF14), VOL+ */
#define BOARD_INITPINSNOQSPI_VOL_UP_PERIPHERAL                               PTA   /*!< Device name: PTA */
#define BOARD_INITPINSNOQSPI_VOL_UP_SIGNAL                                  port   /*!< PTA signal: port */
#define BOARD_INITPINSNOQSPI_VOL_UP_CHANNEL                                    3   /*!< PTA port channel: 3 */
#define BOARD_INITPINSNOQSPI_VOL_UP_PIN_NAME                                PTA3   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_VOL_UP_PIN_FUNCTION_ID             IOMUXC_PTA3_PTA3   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_VOL_UP_LABEL                                 "VOL+"   /*!< Label */
#define BOARD_INITPINSNOQSPI_VOL_UP_NAME                                "VOL_UP"   /*!< Identifier name */

/* PTA13 (number AF16), VOL- */
#define BOARD_INITPINSNOQSPI_VOL_DOWN_PERIPHERAL                             PTA   /*!< Device name: PTA */
#define BOARD_INITPINSNOQSPI_VOL_DOWN_SIGNAL                                port   /*!< PTA signal: port */
#define BOARD_INITPINSNOQSPI_VOL_DOWN_CHANNEL                                 13   /*!< PTA port channel: 13 */
#define BOARD_INITPINSNOQSPI_VOL_DOWN_PIN_NAME                             PTA13   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_VOL_DOWN_PIN_FUNCTION_ID         IOMUXC_PTA13_PTA13   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_VOL_DOWN_LABEL                               "VOL-"   /*!< Label */
#define BOARD_INITPINSNOQSPI_VOL_DOWN_NAME                            "VOL_DOWN"   /*!< Identifier name */

/* PTA14 (number AF17), WL_REG_ON */
#define BOARD_INITPINSNOQSPI_WL_REG_ON_PERIPHERAL                            PTA   /*!< Device name: PTA */
#define BOARD_INITPINSNOQSPI_WL_REG_ON_SIGNAL                               port   /*!< PTA signal: port */
#define BOARD_INITPINSNOQSPI_WL_REG_ON_CHANNEL                                14   /*!< PTA port channel: 14 */
#define BOARD_INITPINSNOQSPI_WL_REG_ON_PIN_NAME                            PTA14   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_WL_REG_ON_PIN_FUNCTION_ID        IOMUXC_PTA14_PTA14   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_WL_REG_ON_LABEL                         "WL_REG_ON"   /*!< Label */
#define BOARD_INITPINSNOQSPI_WL_REG_ON_NAME                          "WL_REG_ON"   /*!< Identifier name */

/* PTA15 (number AF18), BT_REG_ON */
#define BOARD_INITPINSNOQSPI_BT_REG_ON_PERIPHERAL                            PTA   /*!< Device name: PTA */
#define BOARD_INITPINSNOQSPI_BT_REG_ON_SIGNAL                               port   /*!< PTA signal: port */
#define BOARD_INITPINSNOQSPI_BT_REG_ON_CHANNEL                                15   /*!< PTA port channel: 15 */
#define BOARD_INITPINSNOQSPI_BT_REG_ON_PIN_NAME                            PTA15   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_BT_REG_ON_PIN_FUNCTION_ID        IOMUXC_PTA15_PTA15   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_BT_REG_ON_LABEL                         "BT_REG_ON"   /*!< Label */
#define BOARD_INITPINSNOQSPI_BT_REG_ON_NAME                          "BT_REG_ON"   /*!< Identifier name */

/* PTA31 (number AF24), WL_HOST_WAKE */
#define BOARD_INITPINSNOQSPI_WL_HOST_WAKE_PERIPHERAL                         PTA   /*!< Device name: PTA */
#define BOARD_INITPINSNOQSPI_WL_HOST_WAKE_SIGNAL                            port   /*!< PTA signal: port */
#define BOARD_INITPINSNOQSPI_WL_HOST_WAKE_CHANNEL                             31   /*!< PTA port channel: 31 */
#define BOARD_INITPINSNOQSPI_WL_HOST_WAKE_PIN_NAME                         PTA31   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_WL_HOST_WAKE_PIN_FUNCTION_ID     IOMUXC_PTA31_PTA31   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_WL_HOST_WAKE_LABEL                   "WL_HOST_WAKE"   /*!< Label */
#define BOARD_INITPINSNOQSPI_WL_HOST_WAKE_NAME                    "WL_HOST_WAKE"   /*!< Identifier name */

/* PTB1 (number AF2), I2S1_TX_BCLK */
#define BOARD_INITPINSNOQSPI_I2S1_TX_BCLK_PERIPHERAL                         PTB   /*!< Device name: PTB */
#define BOARD_INITPINSNOQSPI_I2S1_TX_BCLK_SIGNAL                            port   /*!< PTB signal: port */
#define BOARD_INITPINSNOQSPI_I2S1_TX_BCLK_CHANNEL                              1   /*!< PTB port channel: 1 */
#define BOARD_INITPINSNOQSPI_I2S1_TX_BCLK_PIN_NAME                          PTB1   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_I2S1_TX_BCLK_PIN_FUNCTION_ID       IOMUXC_PTB1_PTB1   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_I2S1_TX_BCLK_LABEL                   "I2S1_TX_BCLK"   /*!< Label */
#define BOARD_INITPINSNOQSPI_I2S1_TX_BCLK_NAME                    "I2S1_TX_BCLK"   /*!< Identifier name */

/* PTB2 (number AE3), I2S1_TX_FS */
#define BOARD_INITPINSNOQSPI_I2S1_TX_FS_PERIPHERAL                           PTB   /*!< Device name: PTB */
#define BOARD_INITPINSNOQSPI_I2S1_TX_FS_SIGNAL                              port   /*!< PTB signal: port */
#define BOARD_INITPINSNOQSPI_I2S1_TX_FS_CHANNEL                                2   /*!< PTB port channel: 2 */
#define BOARD_INITPINSNOQSPI_I2S1_TX_FS_PIN_NAME                            PTB2   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_I2S1_TX_FS_PIN_FUNCTION_ID         IOMUXC_PTB2_PTB2   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_I2S1_TX_FS_LABEL                       "I2S1_TX_FS"   /*!< Label */
#define BOARD_INITPINSNOQSPI_I2S1_TX_FS_NAME                        "I2S1_TX_FS"   /*!< Identifier name */

/* PTB3 (number AE4), I2S1_TXD0 */
#define BOARD_INITPINSNOQSPI_I2S1_TXD0_PERIPHERAL                            PTB   /*!< Device name: PTB */
#define BOARD_INITPINSNOQSPI_I2S1_TXD0_SIGNAL                               port   /*!< PTB signal: port */
#define BOARD_INITPINSNOQSPI_I2S1_TXD0_CHANNEL                                 3   /*!< PTB port channel: 3 */
#define BOARD_INITPINSNOQSPI_I2S1_TXD0_PIN_NAME                             PTB3   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_I2S1_TXD0_PIN_FUNCTION_ID          IOMUXC_PTB3_PTB3   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_I2S1_TXD0_LABEL                         "I2S1_TXD0"   /*!< Label */
#define BOARD_INITPINSNOQSPI_I2S1_TXD0_NAME                          "I2S1_TXD0"   /*!< Identifier name */

/* PTB10 (number AC7), I2S1_RXD0 */
#define BOARD_INITPINSNOQSPI_I2S1_RXD0_PERIPHERAL                            PTB   /*!< Device name: PTB */
#define BOARD_INITPINSNOQSPI_I2S1_RXD0_SIGNAL                               port   /*!< PTB signal: port */
#define BOARD_INITPINSNOQSPI_I2S1_RXD0_CHANNEL                                10   /*!< PTB port channel: 10 */
#define BOARD_INITPINSNOQSPI_I2S1_RXD0_PIN_NAME                            PTB10   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_I2S1_RXD0_PIN_FUNCTION_ID        IOMUXC_PTB10_PTB10   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_I2S1_RXD0_LABEL                         "I2S1_RXD0"   /*!< Label */
#define BOARD_INITPINSNOQSPI_I2S1_RXD0_NAME                          "I2S1_RXD0"   /*!< Identifier name */

/* PTB4 (number AG4), PTB4 */
#define BOARD_INITPINSNOQSPI_PTB4_PERIPHERAL                                 PTB   /*!< Device name: PTB */
#define BOARD_INITPINSNOQSPI_PTB4_SIGNAL                                    port   /*!< PTB signal: port */
#define BOARD_INITPINSNOQSPI_PTB4_CHANNEL                                      4   /*!< PTB port channel: 4 */
#define BOARD_INITPINSNOQSPI_PTB4_PIN_NAME                                  PTB4   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_PTB4_PIN_FUNCTION_ID               IOMUXC_PTB4_PTB4   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_PTB4_LABEL                                   "PTB4"   /*!< Label */
#define BOARD_INITPINSNOQSPI_PTB4_NAME                                    "PTB4"   /*!< Identifier name */

/* PTB5 (number AF4), PTB5 */
#define BOARD_INITPINSNOQSPI_PTB5_PERIPHERAL                                 PTB   /*!< Device name: PTB */
#define BOARD_INITPINSNOQSPI_PTB5_SIGNAL                                    port   /*!< PTB signal: port */
#define BOARD_INITPINSNOQSPI_PTB5_CHANNEL                                      5   /*!< PTB port channel: 5 */
#define BOARD_INITPINSNOQSPI_PTB5_PIN_NAME                                  PTB5   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_PTB5_PIN_FUNCTION_ID               IOMUXC_PTB5_PTB5   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_PTB5_LABEL                                   "PTB5"   /*!< Label */
#define BOARD_INITPINSNOQSPI_PTB5_NAME                                    "PTB5"   /*!< Identifier name */

/* PTB6 (number AF5), DDR_SW_EN# */
#define BOARD_INITPINSNOQSPI_DDR_SW_EN_PERIPHERAL                            PTB   /*!< Device name: PTB */
#define BOARD_INITPINSNOQSPI_DDR_SW_EN_SIGNAL                               port   /*!< PTB signal: port */
#define BOARD_INITPINSNOQSPI_DDR_SW_EN_CHANNEL                                 6   /*!< PTB port channel: 6 */
#define BOARD_INITPINSNOQSPI_DDR_SW_EN_PIN_NAME                             PTB6   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_DDR_SW_EN_PIN_FUNCTION_ID          IOMUXC_PTB6_PTB6   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_DDR_SW_EN_LABEL                        "DDR_SW_EN#"   /*!< Label */
#define BOARD_INITPINSNOQSPI_DDR_SW_EN_NAME                          "DDR_SW_EN"   /*!< Identifier name */

/* PTB7 (number AF6), BT_HOST_WAKE */
#define BOARD_INITPINSNOQSPI_BT_HOST_WAKE_PERIPHERAL                         PTB   /*!< Device name: PTB */
#define BOARD_INITPINSNOQSPI_BT_HOST_WAKE_SIGNAL                            port   /*!< PTB signal: port */
#define BOARD_INITPINSNOQSPI_BT_HOST_WAKE_CHANNEL                              7   /*!< PTB port channel: 7 */
#define BOARD_INITPINSNOQSPI_BT_HOST_WAKE_PIN_NAME                          PTB7   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_BT_HOST_WAKE_PIN_FUNCTION_ID       IOMUXC_PTB7_PTB7   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_BT_HOST_WAKE_LABEL                   "BT_HOST_WAKE"   /*!< Label */
#define BOARD_INITPINSNOQSPI_BT_HOST_WAKE_NAME                    "BT_HOST_WAKE"   /*!< Identifier name */

/* PTB9 (number AD7), SEN_INT# */
#define BOARD_INITPINSNOQSPI_SEN_INT_PERIPHERAL                              PTB   /*!< Device name: PTB */
#define BOARD_INITPINSNOQSPI_SEN_INT_SIGNAL                                 port   /*!< PTB signal: port */
#define BOARD_INITPINSNOQSPI_SEN_INT_CHANNEL                                   9   /*!< PTB port channel: 9 */
#define BOARD_INITPINSNOQSPI_SEN_INT_PIN_NAME                               PTB9   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_SEN_INT_PIN_FUNCTION_ID            IOMUXC_PTB9_PTB9   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_SEN_INT_LABEL                            "SEN_INT#"   /*!< Label */
#define BOARD_INITPINSNOQSPI_SEN_INT_NAME                              "SEN_INT"   /*!< Identifier name */

/* PTB11 (number AB7), PMIC_INT# */
#define BOARD_INITPINSNOQSPI_PMIC_INT_PERIPHERAL                             PTB   /*!< Device name: PTB */
#define BOARD_INITPINSNOQSPI_PMIC_INT_SIGNAL                                port   /*!< PTB signal: port */
#define BOARD_INITPINSNOQSPI_PMIC_INT_CHANNEL                                 11   /*!< PTB port channel: 11 */
#define BOARD_INITPINSNOQSPI_PMIC_INT_PIN_NAME                             PTB11   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_PMIC_INT_PIN_FUNCTION_ID         IOMUXC_PTB11_PTB11   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_PMIC_INT_LABEL                          "PMIC_INT#"   /*!< Label */
#define BOARD_INITPINSNOQSPI_PMIC_INT_NAME                            "PMIC_INT"   /*!< Identifier name */

/* PTB14 (number AG8), RTC_CLK */
#define BOARD_INITPINSNOQSPI_RTC_CLK_PERIPHERAL                              RTC   /*!< Device name: RTC */
#define BOARD_INITPINSNOQSPI_RTC_CLK_SIGNAL                           rtc_clkout   /*!< RTC signal: rtc_clkout */
#define BOARD_INITPINSNOQSPI_RTC_CLK_PIN_NAME                              PTB14   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_RTC_CLK_PIN_FUNCTION_ID     IOMUXC_PTB14_RTC_CLKOUT   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_RTC_CLK_LABEL                             "RTC_CLK"   /*!< Label */
#define BOARD_INITPINSNOQSPI_RTC_CLK_NAME                              "RTC_CLK"   /*!< Identifier name */

/* PTB8 (number AG6), QSPIA_SS0_B */
#define BOARD_INITPINSNOQSPI_QSPIA_SS0_B_PERIPHERAL                          PTB   /*!< Device name: PTB */
#define BOARD_INITPINSNOQSPI_QSPIA_SS0_B_SIGNAL                             port   /*!< PTB signal: port */
#define BOARD_INITPINSNOQSPI_QSPIA_SS0_B_CHANNEL                               8   /*!< PTB port channel: 8 */
#define BOARD_INITPINSNOQSPI_QSPIA_SS0_B_PIN_NAME                           PTB8   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_QSPIA_SS0_B_PIN_FUNCTION_ID        IOMUXC_PTB8_PTB8   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_QSPIA_SS0_B_LABEL                     "QSPIA_SS0_B"   /*!< Label */
#define BOARD_INITPINSNOQSPI_QSPIA_SS0_B_NAME                      "QSPIA_SS0_B"   /*!< Identifier name */

/* PTB15 (number AF8), QSPIA_SCLK */
#define BOARD_INITPINSNOQSPI_QSPIA_SCLK_PERIPHERAL                           PTB   /*!< Device name: PTB */
#define BOARD_INITPINSNOQSPI_QSPIA_SCLK_SIGNAL                              port   /*!< PTB signal: port */
#define BOARD_INITPINSNOQSPI_QSPIA_SCLK_CHANNEL                               15   /*!< PTB port channel: 15 */
#define BOARD_INITPINSNOQSPI_QSPIA_SCLK_PIN_NAME                           PTB15   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_QSPIA_SCLK_PIN_FUNCTION_ID       IOMUXC_PTB15_PTB15   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_QSPIA_SCLK_LABEL                       "QSPIA_SCLK"   /*!< Label */
#define BOARD_INITPINSNOQSPI_QSPIA_SCLK_NAME                        "QSPIA_SCLK"   /*!< Identifier name */

/* PTB16 (number AF9), QSPIA_DATA3 */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA3_PERIPHERAL                          PTB   /*!< Device name: PTB */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA3_SIGNAL                             port   /*!< PTB signal: port */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA3_CHANNEL                              16   /*!< PTB port channel: 16 */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA3_PIN_NAME                          PTB16   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA3_PIN_FUNCTION_ID      IOMUXC_PTB16_PTB16   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA3_LABEL                     "QSPIA_DATA3"   /*!< Label */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA3_NAME                      "QSPIA_DATA3"   /*!< Identifier name */

/* PTB17 (number AF10), QSPIA_DATA2 */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA2_PERIPHERAL                          PTB   /*!< Device name: PTB */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA2_SIGNAL                             port   /*!< PTB signal: port */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA2_CHANNEL                              17   /*!< PTB port channel: 17 */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA2_PIN_NAME                          PTB17   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA2_PIN_FUNCTION_ID      IOMUXC_PTB17_PTB17   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA2_LABEL                     "QSPIA_DATA2"   /*!< Label */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA2_NAME                      "QSPIA_DATA2"   /*!< Identifier name */

/* PTB18 (number AG10), QSPIA_DATA1 */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA1_PERIPHERAL                          PTB   /*!< Device name: PTB */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA1_SIGNAL                             port   /*!< PTB signal: port */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA1_CHANNEL                              18   /*!< PTB port channel: 18 */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA1_PIN_NAME                          PTB18   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA1_PIN_FUNCTION_ID      IOMUXC_PTB18_PTB18   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA1_LABEL                     "QSPIA_DATA1"   /*!< Label */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA1_NAME                      "QSPIA_DATA1"   /*!< Identifier name */

/* PTB19 (number AD10), QSPIA_DATA0 */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA0_PERIPHERAL                          PTB   /*!< Device name: PTB */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA0_SIGNAL                             port   /*!< PTB signal: port */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA0_CHANNEL                              19   /*!< PTB port channel: 19 */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA0_PIN_NAME                          PTB19   /*!< Pin name */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA0_PIN_FUNCTION_ID      IOMUXC_PTB19_PTB19   /*!< Pin function id */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA0_LABEL                     "QSPIA_DATA0"   /*!< Label */
#define BOARD_INITPINSNOQSPI_QSPIA_DATA0_NAME                      "QSPIA_DATA0"   /*!< Identifier name */

/* PTA16 (number AG18), I2C0_SCL */
#define BOARD_I2C_CONFIGUREPINS_I2C0_SCL_PERIPHERAL                       LPI2C0   /*!< Device name: LPI2C0 */
#define BOARD_I2C_CONFIGUREPINS_I2C0_SCL_SIGNAL                        lpi2c_scl   /*!< LPI2C0 signal: lpi2c_scl */
#define BOARD_I2C_CONFIGUREPINS_I2C0_SCL_PIN_NAME                          PTA16   /*!< Pin name */
#define BOARD_I2C_CONFIGUREPINS_I2C0_SCL_PIN_FUNCTION_ID IOMUXC_PTA16_LPI2C0_SCL   /*!< Pin function id */
#define BOARD_I2C_CONFIGUREPINS_I2C0_SCL_LABEL                        "I2C0_SCL"   /*!< Label */
#define BOARD_I2C_CONFIGUREPINS_I2C0_SCL_NAME                         "I2C0_SCL"   /*!< Identifier name */

/* PTA17 (number AD19), I2C0_SDA */
#define BOARD_I2C_CONFIGUREPINS_I2C0_SDA_PERIPHERAL                       LPI2C0   /*!< Device name: LPI2C0 */
#define BOARD_I2C_CONFIGUREPINS_I2C0_SDA_SIGNAL                        lpi2c_sda   /*!< LPI2C0 signal: lpi2c_sda */
#define BOARD_I2C_CONFIGUREPINS_I2C0_SDA_PIN_NAME                          PTA17   /*!< Pin name */
#define BOARD_I2C_CONFIGUREPINS_I2C0_SDA_PIN_FUNCTION_ID IOMUXC_PTA17_LPI2C0_SDA   /*!< Pin function id */
#define BOARD_I2C_CONFIGUREPINS_I2C0_SDA_LABEL                        "I2C0_SDA"   /*!< Label */
#define BOARD_I2C_CONFIGUREPINS_I2C0_SDA_NAME                         "I2C0_SDA"   /*!< Identifier name */

/* PTB12 (number AC8), I2C3_SCL */
#define BOARD_I2C_CONFIGUREPINS_I2C3_SCL_PERIPHERAL                       LPI2C3   /*!< Device name: LPI2C3 */
#define BOARD_I2C_CONFIGUREPINS_I2C3_SCL_SIGNAL                        lpi2c_scl   /*!< LPI2C3 signal: lpi2c_scl */
#define BOARD_I2C_CONFIGUREPINS_I2C3_SCL_PIN_NAME                          PTB12   /*!< Pin name */
#define BOARD_I2C_CONFIGUREPINS_I2C3_SCL_PIN_FUNCTION_ID IOMUXC_PTB12_LPI2C3_SCL   /*!< Pin function id */
#define BOARD_I2C_CONFIGUREPINS_I2C3_SCL_LABEL                        "I2C3_SCL"   /*!< Label */
#define BOARD_I2C_CONFIGUREPINS_I2C3_SCL_NAME                         "I2C3_SCL"   /*!< Identifier name */

/* PTB13 (number AD8), I2C3_SDA */
#define BOARD_I2C_CONFIGUREPINS_I2C3_SDA_PERIPHERAL                       LPI2C3   /*!< Device name: LPI2C3 */
#define BOARD_I2C_CONFIGUREPINS_I2C3_SDA_SIGNAL                        lpi2c_sda   /*!< LPI2C3 signal: lpi2c_sda */
#define BOARD_I2C_CONFIGUREPINS_I2C3_SDA_PIN_NAME                          PTB13   /*!< Pin name */
#define BOARD_I2C_CONFIGUREPINS_I2C3_SDA_PIN_FUNCTION_ID IOMUXC_PTB13_LPI2C3_SDA   /*!< Pin function id */
#define BOARD_I2C_CONFIGUREPINS_I2C3_SDA_LABEL                        "I2C3_SDA"   /*!< Label */
#define BOARD_I2C_CONFIGUREPINS_I2C3_SDA_NAME                         "I2C3_SDA"   /*!< Identifier name */

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
void BOARD_InitPinsNoQSPI(void);                           /*!< Function assigned for the core: Cortex-M4[cm4] */

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
