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

/* DRAM_SDCLK0_P (coord P1), DRAM_SDCLK0_P */
#define BOARD_INITPINS_DRAM_SDCLK0_P_PERIPHERAL                             MMDC   /*!< Device name: MMDC */
#define BOARD_INITPINS_DRAM_SDCLK0_P_SIGNAL                        mmdc_sdclk0_p   /*!< MMDC signal: mmdc_sdclk0_p */
#define BOARD_INITPINS_DRAM_SDCLK0_P_PIN_NAME                      DRAM_SDCLK0_P   /*!< Pin name */
#define BOARD_INITPINS_DRAM_SDCLK0_P_LABEL                       "DRAM_SDCLK0_P"   /*!< Label */
#define BOARD_INITPINS_DRAM_SDCLK0_P_NAME                        "DRAM_SDCLK0_P"   /*!< Identifier name */

/* DRAM_CAS_B (coord J2), DRAM_CAS_B */
#define BOARD_INITPINS_DRAM_CAS_B_PERIPHERAL                                MMDC   /*!< Device name: MMDC */
#define BOARD_INITPINS_DRAM_CAS_B_SIGNAL                              mmdc_cas_b   /*!< MMDC signal: mmdc_cas_b */
#define BOARD_INITPINS_DRAM_CAS_B_PIN_NAME                            DRAM_CAS_B   /*!< Pin name */
#define BOARD_INITPINS_DRAM_CAS_B_LABEL                             "DRAM_CAS_B"   /*!< Label */
#define BOARD_INITPINS_DRAM_CAS_B_NAME                              "DRAM_CAS_B"   /*!< Identifier name */

/* DRAM_RAS_B (coord M5), DRAM_RAS_B */
#define BOARD_INITPINS_DRAM_RAS_B_PERIPHERAL                                MMDC   /*!< Device name: MMDC */
#define BOARD_INITPINS_DRAM_RAS_B_SIGNAL                              mmdc_ras_b   /*!< MMDC signal: mmdc_ras_b */
#define BOARD_INITPINS_DRAM_RAS_B_PIN_NAME                            DRAM_RAS_B   /*!< Pin name */
#define BOARD_INITPINS_DRAM_RAS_B_LABEL                             "DRAM_RAS_B"   /*!< Label */
#define BOARD_INITPINS_DRAM_RAS_B_NAME                              "DRAM_RAS_B"   /*!< Identifier name */

/* DRAM_RESET (coord G4), DRAM_RESET */
#define BOARD_INITPINS_DRAM_RESET_PERIPHERAL                                MMDC   /*!< Device name: MMDC */
#define BOARD_INITPINS_DRAM_RESET_SIGNAL                              mmdc_reset   /*!< MMDC signal: mmdc_reset */
#define BOARD_INITPINS_DRAM_RESET_PIN_NAME                            DRAM_RESET   /*!< Pin name */
#define BOARD_INITPINS_DRAM_RESET_LABEL                             "DRAM_RESET"   /*!< Label */
#define BOARD_INITPINS_DRAM_RESET_NAME                              "DRAM_RESET"   /*!< Identifier name */

/* DRAM_SDBA2 (coord K2), DRAM_SDBA2 */
#define BOARD_INITPINS_DRAM_SDBA2_PERIPHERAL                                MMDC   /*!< Device name: MMDC */
#define BOARD_INITPINS_DRAM_SDBA2_SIGNAL                               mmdc_sdba   /*!< MMDC signal: mmdc_sdba */
#define BOARD_INITPINS_DRAM_SDBA2_CHANNEL                                      2   /*!< MMDC mmdc_sdba channel: 2 */
#define BOARD_INITPINS_DRAM_SDBA2_PIN_NAME                            DRAM_SDBA2   /*!< Pin name */
#define BOARD_INITPINS_DRAM_SDBA2_LABEL                             "DRAM_SDBA2"   /*!< Label */
#define BOARD_INITPINS_DRAM_SDBA2_NAME                              "DRAM_SDBA2"   /*!< Identifier name */

/* DRAM_DQM0 (coord T7), DRAM_DQM0 */
#define BOARD_INITPINS_DRAM_DQM0_PERIPHERAL                                 MMDC   /*!< Device name: MMDC */
#define BOARD_INITPINS_DRAM_DQM0_SIGNAL                                 mmdc_dqm   /*!< MMDC signal: mmdc_dqm */
#define BOARD_INITPINS_DRAM_DQM0_CHANNEL                                       0   /*!< MMDC mmdc_dqm channel: 0 */
#define BOARD_INITPINS_DRAM_DQM0_PIN_NAME                              DRAM_DQM0   /*!< Pin name */
#define BOARD_INITPINS_DRAM_DQM0_LABEL                               "DRAM_DQM0"   /*!< Label */
#define BOARD_INITPINS_DRAM_DQM0_NAME                                "DRAM_DQM0"   /*!< Identifier name */

/* DRAM_DQM1 (coord T3), DRAM_DQM1 */
#define BOARD_INITPINS_DRAM_DQM1_PERIPHERAL                                 MMDC   /*!< Device name: MMDC */
#define BOARD_INITPINS_DRAM_DQM1_SIGNAL                                 mmdc_dqm   /*!< MMDC signal: mmdc_dqm */
#define BOARD_INITPINS_DRAM_DQM1_CHANNEL                                       1   /*!< MMDC mmdc_dqm channel: 1 */
#define BOARD_INITPINS_DRAM_DQM1_PIN_NAME                              DRAM_DQM1   /*!< Pin name */
#define BOARD_INITPINS_DRAM_DQM1_LABEL                               "DRAM_DQM1"   /*!< Label */
#define BOARD_INITPINS_DRAM_DQM1_NAME                                "DRAM_DQM1"   /*!< Identifier name */

/* DRAM_ODT0 (coord N1), DRAM_ODT0 */
#define BOARD_INITPINS_DRAM_ODT0_PERIPHERAL                                 MMDC   /*!< Device name: MMDC */
#define BOARD_INITPINS_DRAM_ODT0_SIGNAL                                 mmdc_odt   /*!< MMDC signal: mmdc_odt */
#define BOARD_INITPINS_DRAM_ODT0_CHANNEL                                       0   /*!< MMDC mmdc_odt channel: 0 */
#define BOARD_INITPINS_DRAM_ODT0_PIN_NAME                              DRAM_ODT0   /*!< Pin name */
#define BOARD_INITPINS_DRAM_ODT0_LABEL                               "DRAM_ODT0"   /*!< Label */
#define BOARD_INITPINS_DRAM_ODT0_NAME                                "DRAM_ODT0"   /*!< Identifier name */

/* DRAM_ODT1 (coord F1), DRAM_ODT1 */
#define BOARD_INITPINS_DRAM_ODT1_PERIPHERAL                                 MMDC   /*!< Device name: MMDC */
#define BOARD_INITPINS_DRAM_ODT1_SIGNAL                                 mmdc_odt   /*!< MMDC signal: mmdc_odt */
#define BOARD_INITPINS_DRAM_ODT1_CHANNEL                                       1   /*!< MMDC mmdc_odt channel: 1 */
#define BOARD_INITPINS_DRAM_ODT1_PIN_NAME                              DRAM_ODT1   /*!< Pin name */
#define BOARD_INITPINS_DRAM_ODT1_LABEL                               "DRAM_ODT1"   /*!< Label */
#define BOARD_INITPINS_DRAM_ODT1_NAME                                "DRAM_ODT1"   /*!< Identifier name */

/* DRAM_SDQS0_P (coord P6), DRAM_SDQS0_P */
#define BOARD_INITPINS_DRAM_SDQS0_P_PERIPHERAL                              MMDC   /*!< Device name: MMDC */
#define BOARD_INITPINS_DRAM_SDQS0_P_SIGNAL                           mmdc_sdqs_p   /*!< MMDC signal: mmdc_sdqs_p */
#define BOARD_INITPINS_DRAM_SDQS0_P_CHANNEL                                    0   /*!< MMDC mmdc_sdqs_p channel: 0 */
#define BOARD_INITPINS_DRAM_SDQS0_P_PIN_NAME                        DRAM_SDQS0_P   /*!< Pin name */
#define BOARD_INITPINS_DRAM_SDQS0_P_LABEL                         "DRAM_SDQS0_P"   /*!< Label */
#define BOARD_INITPINS_DRAM_SDQS0_P_NAME                          "DRAM_SDQS0_P"   /*!< Identifier name */

/* DRAM_SDQS1_P (coord T1), DRAM_SDQS1_P */
#define BOARD_INITPINS_DRAM_SDQS1_P_PERIPHERAL                              MMDC   /*!< Device name: MMDC */
#define BOARD_INITPINS_DRAM_SDQS1_P_SIGNAL                           mmdc_sdqs_p   /*!< MMDC signal: mmdc_sdqs_p */
#define BOARD_INITPINS_DRAM_SDQS1_P_CHANNEL                                    1   /*!< MMDC mmdc_sdqs_p channel: 1 */
#define BOARD_INITPINS_DRAM_SDQS1_P_PIN_NAME                        DRAM_SDQS1_P   /*!< Pin name */
#define BOARD_INITPINS_DRAM_SDQS1_P_LABEL                         "DRAM_SDQS1_P"   /*!< Label */
#define BOARD_INITPINS_DRAM_SDQS1_P_NAME                          "DRAM_SDQS1_P"   /*!< Identifier name */

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
