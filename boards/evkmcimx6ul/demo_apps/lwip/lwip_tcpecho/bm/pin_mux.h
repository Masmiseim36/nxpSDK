/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

#include "board.h"

/* UART1_RX_DATA (coord K16), UART1_RXD */
#define BOARD_INITPINS_UART1_RXD_PERIPHERAL                                UART1   /*!< Device name: UART1 */
#define BOARD_INITPINS_UART1_RXD_SIGNAL                                  uart_rx   /*!< UART1 signal: uart_rx */
#define BOARD_INITPINS_UART1_RXD_PIN_NAME                          UART1_RX_DATA   /*!< Pin name */
#define BOARD_INITPINS_UART1_RXD_LABEL                               "UART1_RXD"   /*!< Label */
#define BOARD_INITPINS_UART1_RXD_NAME                                "UART1_RXD"   /*!< Identifier name */

/* UART1_TX_DATA (coord K14), UART1_TXD */
#define BOARD_INITPINS_UART1_TXD_PERIPHERAL                                UART1   /*!< Device name: UART1 */
#define BOARD_INITPINS_UART1_TXD_SIGNAL                                  uart_tx   /*!< UART1 signal: uart_tx */
#define BOARD_INITPINS_UART1_TXD_PIN_NAME                          UART1_TX_DATA   /*!< Pin name */
#define BOARD_INITPINS_UART1_TXD_LABEL                               "UART1_TXD"   /*!< Label */
#define BOARD_INITPINS_UART1_TXD_NAME                                "UART1_TXD"   /*!< Identifier name */

/* GPIO1_IO00 (coord K13), GPIO_0/USB_OTG1_ID */
#define BOARD_INITPINS_USB_OTG1_ID_PERIPHERAL                              ENET1   /*!< Device name: ENET1 */
#define BOARD_INITPINS_USB_OTG1_ID_SIGNAL                    enet_1588_event0_in   /*!< ENET1 signal: enet_1588_event0_in */
#define BOARD_INITPINS_USB_OTG1_ID_PIN_NAME                           GPIO1_IO00   /*!< Pin name */
#define BOARD_INITPINS_USB_OTG1_ID_LABEL                    "GPIO_0/USB_OTG1_ID"   /*!< Label */
#define BOARD_INITPINS_USB_OTG1_ID_NAME                            "USB_OTG1_ID"   /*!< Identifier name */

/* GPIO1_IO01 (coord L15), GPIO_1/USB_OTG1_OC */
#define BOARD_INITPINS_USB_OTG1_OC_PERIPHERAL                              ENET1   /*!< Device name: ENET1 */
#define BOARD_INITPINS_USB_OTG1_OC_SIGNAL                   enet_1588_event0_out   /*!< ENET1 signal: enet_1588_event0_out */
#define BOARD_INITPINS_USB_OTG1_OC_PIN_NAME                           GPIO1_IO01   /*!< Pin name */
#define BOARD_INITPINS_USB_OTG1_OC_LABEL                    "GPIO_1/USB_OTG1_OC"   /*!< Label */
#define BOARD_INITPINS_USB_OTG1_OC_NAME                            "USB_OTG1_OC"   /*!< Identifier name */

/* GPIO1_IO07 (coord L16), GPIO_7/ENET_MDC */
#define BOARD_INITPINS_ENET_MDC_PERIPHERAL                                 ENET1   /*!< Device name: ENET1 */
#define BOARD_INITPINS_ENET_MDC_SIGNAL                                  enet_mdc   /*!< ENET1 signal: enet_mdc */
#define BOARD_INITPINS_ENET_MDC_PIN_NAME                              GPIO1_IO07   /*!< Pin name */
#define BOARD_INITPINS_ENET_MDC_LABEL                          "GPIO_7/ENET_MDC"   /*!< Label */
#define BOARD_INITPINS_ENET_MDC_NAME                                  "ENET_MDC"   /*!< Identifier name */

/* GPIO1_IO06 (coord K17), GPIO_6/ENET_MDIO */
#define BOARD_INITPINS_ENET_MDIO_PERIPHERAL                                ENET1   /*!< Device name: ENET1 */
#define BOARD_INITPINS_ENET_MDIO_SIGNAL                                enet_mdio   /*!< ENET1 signal: enet_mdio */
#define BOARD_INITPINS_ENET_MDIO_PIN_NAME                             GPIO1_IO06   /*!< Pin name */
#define BOARD_INITPINS_ENET_MDIO_LABEL                        "GPIO_6/ENET_MDIO"   /*!< Label */
#define BOARD_INITPINS_ENET_MDIO_NAME                                "ENET_MDIO"   /*!< Identifier name */

/* ENET1_RX_DATA0 (coord F16), ENET1_RXD0/TP402 */
#define BOARD_INITPINS_ENET1_RXD0_PERIPHERAL                               ENET1   /*!< Device name: ENET1 */
#define BOARD_INITPINS_ENET1_RXD0_SIGNAL                              enet_rdata   /*!< ENET1 signal: enet_rdata */
#define BOARD_INITPINS_ENET1_RXD0_CHANNEL                                      0   /*!< ENET1 enet_rdata channel: 0 */
#define BOARD_INITPINS_ENET1_RXD0_PIN_NAME                        ENET1_RX_DATA0   /*!< Pin name */
#define BOARD_INITPINS_ENET1_RXD0_LABEL                       "ENET1_RXD0/TP402"   /*!< Label */
#define BOARD_INITPINS_ENET1_RXD0_NAME                              "ENET1_RXD0"   /*!< Identifier name */

/* ENET1_RX_DATA1 (coord E17), ENET1_RXD1 */
#define BOARD_INITPINS_ENET1_RXD1_PERIPHERAL                               ENET1   /*!< Device name: ENET1 */
#define BOARD_INITPINS_ENET1_RXD1_SIGNAL                              enet_rdata   /*!< ENET1 signal: enet_rdata */
#define BOARD_INITPINS_ENET1_RXD1_CHANNEL                                      1   /*!< ENET1 enet_rdata channel: 1 */
#define BOARD_INITPINS_ENET1_RXD1_PIN_NAME                        ENET1_RX_DATA1   /*!< Pin name */
#define BOARD_INITPINS_ENET1_RXD1_LABEL                             "ENET1_RXD1"   /*!< Label */
#define BOARD_INITPINS_ENET1_RXD1_NAME                              "ENET1_RXD1"   /*!< Identifier name */

/* ENET1_RX_EN (coord E16), ENET1_CRS_DV */
#define BOARD_INITPINS_ENET1_CRS_DV_PERIPHERAL                             ENET1   /*!< Device name: ENET1 */
#define BOARD_INITPINS_ENET1_CRS_DV_SIGNAL                            enet_rx_en   /*!< ENET1 signal: enet_rx_en */
#define BOARD_INITPINS_ENET1_CRS_DV_PIN_NAME                         ENET1_RX_EN   /*!< Pin name */
#define BOARD_INITPINS_ENET1_CRS_DV_LABEL                         "ENET1_CRS_DV"   /*!< Label */
#define BOARD_INITPINS_ENET1_CRS_DV_NAME                          "ENET1_CRS_DV"   /*!< Identifier name */

/* ENET1_RX_ER (coord D15), ENET1_RX_ER */
#define BOARD_INITPINS_ENET1_RX_ER_PERIPHERAL                              ENET1   /*!< Device name: ENET1 */
#define BOARD_INITPINS_ENET1_RX_ER_SIGNAL                             enet_rx_er   /*!< ENET1 signal: enet_rx_er */
#define BOARD_INITPINS_ENET1_RX_ER_PIN_NAME                          ENET1_RX_ER   /*!< Pin name */
#define BOARD_INITPINS_ENET1_RX_ER_LABEL                           "ENET1_RX_ER"   /*!< Label */
#define BOARD_INITPINS_ENET1_RX_ER_NAME                            "ENET1_RX_ER"   /*!< Identifier name */

/* ENET1_TX_DATA0 (coord E15), ENET1_TXD0 */
#define BOARD_INITPINS_ENET1_TXD0_PERIPHERAL                               ENET1   /*!< Device name: ENET1 */
#define BOARD_INITPINS_ENET1_TXD0_SIGNAL                              enet_tdata   /*!< ENET1 signal: enet_tdata */
#define BOARD_INITPINS_ENET1_TXD0_CHANNEL                                      0   /*!< ENET1 enet_tdata channel: 0 */
#define BOARD_INITPINS_ENET1_TXD0_PIN_NAME                        ENET1_TX_DATA0   /*!< Pin name */
#define BOARD_INITPINS_ENET1_TXD0_LABEL                             "ENET1_TXD0"   /*!< Label */
#define BOARD_INITPINS_ENET1_TXD0_NAME                              "ENET1_TXD0"   /*!< Identifier name */

/* ENET1_TX_DATA1 (coord E14), ENET1_TXD1 */
#define BOARD_INITPINS_ENET1_TXD1_PERIPHERAL                               ENET1   /*!< Device name: ENET1 */
#define BOARD_INITPINS_ENET1_TXD1_SIGNAL                              enet_tdata   /*!< ENET1 signal: enet_tdata */
#define BOARD_INITPINS_ENET1_TXD1_CHANNEL                                      1   /*!< ENET1 enet_tdata channel: 1 */
#define BOARD_INITPINS_ENET1_TXD1_PIN_NAME                        ENET1_TX_DATA1   /*!< Pin name */
#define BOARD_INITPINS_ENET1_TXD1_LABEL                             "ENET1_TXD1"   /*!< Label */
#define BOARD_INITPINS_ENET1_TXD1_NAME                              "ENET1_TXD1"   /*!< Identifier name */

/* ENET1_TX_CLK (coord F14), ENET1_TX_CLK */
#define BOARD_INITPINS_ENET1_TX_CLK_PERIPHERAL                             ENET1   /*!< Device name: ENET1 */
#define BOARD_INITPINS_ENET1_TX_CLK_SIGNAL                          enet_ref_clk   /*!< ENET1 signal: enet_ref_clk */
#define BOARD_INITPINS_ENET1_TX_CLK_CHANNEL                                    1   /*!< ENET1 enet_ref_clk channel: 1 */
#define BOARD_INITPINS_ENET1_TX_CLK_PIN_NAME                        ENET1_TX_CLK   /*!< Pin name */
#define BOARD_INITPINS_ENET1_TX_CLK_LABEL                         "ENET1_TX_CLK"   /*!< Label */
#define BOARD_INITPINS_ENET1_TX_CLK_NAME                          "ENET1_TX_CLK"   /*!< Identifier name */

/* ENET1_TX_EN (coord F15), ENET1_TXEN */
#define BOARD_INITPINS_ENET1_TXEN_PERIPHERAL                               ENET1   /*!< Device name: ENET1 */
#define BOARD_INITPINS_ENET1_TXEN_SIGNAL                              enet_tx_en   /*!< ENET1 signal: enet_tx_en */
#define BOARD_INITPINS_ENET1_TXEN_PIN_NAME                           ENET1_TX_EN   /*!< Pin name */
#define BOARD_INITPINS_ENET1_TXEN_LABEL                             "ENET1_TXEN"   /*!< Label */
#define BOARD_INITPINS_ENET1_TXEN_NAME                              "ENET1_TXEN"   /*!< Identifier name */

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif

#endif /* _PIN_MUX_H_ */
