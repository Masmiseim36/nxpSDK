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

/* GPIO1_IO08 (coord N17), GPIO_8/BLT_PWM */
#define BOARD_INITPINS_BLT_PWM_GPIO                                        GPIO1   /*!< GPIO device name: GPIO1 */
#define BOARD_INITPINS_BLT_PWM_GPIO_PIN                                       8U   /*!< GPIO1 pin index: 8 */
#define BOARD_INITPINS_BLT_PWM_PIN_NAME                               GPIO1_IO08   /*!< Pin name */
#define BOARD_INITPINS_BLT_PWM_LABEL                            "GPIO_8/BLT_PWM"   /*!< Label */
#define BOARD_INITPINS_BLT_PWM_NAME                                    "BLT_PWM"   /*!< Identifier name */

/* LCD_CLK (coord A8), LCD_PCLK */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_PCLK_PERIPHERAL                      LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_PCLK_SIGNAL                      lcdif_clk   /*!< LCDIF signal: lcdif_clk */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_PCLK_PIN_NAME                      LCD_CLK   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_PCLK_LABEL                      "LCD_PCLK"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_PCLK_NAME                       "LCD_PCLK"   /*!< Identifier name */

/* LCD_DATA00 (coord B9), LCD_DATA0/BT_CFG1[0] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA0_PERIPHERAL                     LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA0_SIGNAL                    lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA0_CHANNEL                            0   /*!< LCDIF lcdif_data channel: 0 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA0_PIN_NAME                  LCD_DATA00   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA0_LABEL         "LCD_DATA0/BT_CFG1[0]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA0_NAME                     "LCD_DATA0"   /*!< Identifier name */

/* LCD_DATA01 (coord A9), LCD_DATA1/BT_CFG1[1] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA1_PERIPHERAL                     LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA1_SIGNAL                    lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA1_CHANNEL                            1   /*!< LCDIF lcdif_data channel: 1 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA1_PIN_NAME                  LCD_DATA01   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA1_LABEL         "LCD_DATA1/BT_CFG1[1]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA1_NAME                     "LCD_DATA1"   /*!< Identifier name */

/* LCD_DATA02 (coord E10), LCD_DATA2/BT_CFG1[2] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA2_PERIPHERAL                     LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA2_SIGNAL                    lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA2_CHANNEL                            2   /*!< LCDIF lcdif_data channel: 2 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA2_PIN_NAME                  LCD_DATA02   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA2_LABEL         "LCD_DATA2/BT_CFG1[2]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA2_NAME                     "LCD_DATA2"   /*!< Identifier name */

/* LCD_DATA03 (coord D10), LCD_DATA3/BT_CFG1[3] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA3_PERIPHERAL                     LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA3_SIGNAL                    lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA3_CHANNEL                            3   /*!< LCDIF lcdif_data channel: 3 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA3_PIN_NAME                  LCD_DATA03   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA3_LABEL         "LCD_DATA3/BT_CFG1[3]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA3_NAME                     "LCD_DATA3"   /*!< Identifier name */

/* LCD_DATA04 (coord C10), LCD_DATA4/BT_CFG1[4] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA4_PERIPHERAL                     LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA4_SIGNAL                    lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA4_CHANNEL                            4   /*!< LCDIF lcdif_data channel: 4 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA4_PIN_NAME                  LCD_DATA04   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA4_LABEL         "LCD_DATA4/BT_CFG1[4]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA4_NAME                     "LCD_DATA4"   /*!< Identifier name */

/* LCD_DATA05 (coord B10), LCD_DATA5/BT_CFG1[5] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA5_PERIPHERAL                     LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA5_SIGNAL                    lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA5_CHANNEL                            5   /*!< LCDIF lcdif_data channel: 5 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA5_PIN_NAME                  LCD_DATA05   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA5_LABEL         "LCD_DATA5/BT_CFG1[5]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA5_NAME                     "LCD_DATA5"   /*!< Identifier name */

/* LCD_DATA06 (coord A10), LCD_DATA6/BT_CFG1[6] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA6_PERIPHERAL                     LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA6_SIGNAL                    lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA6_CHANNEL                            6   /*!< LCDIF lcdif_data channel: 6 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA6_PIN_NAME                  LCD_DATA06   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA6_LABEL         "LCD_DATA6/BT_CFG1[6]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA6_NAME                     "LCD_DATA6"   /*!< Identifier name */

/* LCD_DATA07 (coord D11), LCD_DATA7/BT_CFG1[7] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA7_PERIPHERAL                     LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA7_SIGNAL                    lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA7_CHANNEL                            7   /*!< LCDIF lcdif_data channel: 7 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA7_PIN_NAME                  LCD_DATA07   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA7_LABEL         "LCD_DATA7/BT_CFG1[7]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA7_NAME                     "LCD_DATA7"   /*!< Identifier name */

/* LCD_DATA08 (coord B11), LCD_DATA8/BT_CFG2[0] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA8_PERIPHERAL                     LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA8_SIGNAL                    lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA8_CHANNEL                            8   /*!< LCDIF lcdif_data channel: 8 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA8_PIN_NAME                  LCD_DATA08   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA8_LABEL         "LCD_DATA8/BT_CFG2[0]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA8_NAME                     "LCD_DATA8"   /*!< Identifier name */

/* LCD_DATA09 (coord A11), LCD_DATA9/BT_CFG2[1] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA9_PERIPHERAL                     LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA9_SIGNAL                    lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA9_CHANNEL                            9   /*!< LCDIF lcdif_data channel: 9 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA9_PIN_NAME                  LCD_DATA09   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA9_LABEL         "LCD_DATA9/BT_CFG2[1]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA9_NAME                     "LCD_DATA9"   /*!< Identifier name */

/* LCD_DATA10 (coord E12), LCD_DATA10/BT_CFG2[2] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA10_PERIPHERAL                    LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA10_SIGNAL                   lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA10_CHANNEL                          10   /*!< LCDIF lcdif_data channel: 10 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA10_PIN_NAME                 LCD_DATA10   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA10_LABEL       "LCD_DATA10/BT_CFG2[2]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA10_NAME                   "LCD_DATA10"   /*!< Identifier name */

/* LCD_DATA11 (coord D12), LCD_DATA11/BT_CFG2[3] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA11_PERIPHERAL                    LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA11_SIGNAL                   lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA11_CHANNEL                          11   /*!< LCDIF lcdif_data channel: 11 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA11_PIN_NAME                 LCD_DATA11   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA11_LABEL       "LCD_DATA11/BT_CFG2[3]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA11_NAME                   "LCD_DATA11"   /*!< Identifier name */

/* LCD_DATA12 (coord C12), LCD_DATA12/BT_CFG2[4] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA12_PERIPHERAL                    LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA12_SIGNAL                   lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA12_CHANNEL                          12   /*!< LCDIF lcdif_data channel: 12 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA12_PIN_NAME                 LCD_DATA12   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA12_LABEL       "LCD_DATA12/BT_CFG2[4]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA12_NAME                   "LCD_DATA12"   /*!< Identifier name */

/* LCD_DATA13 (coord B12), LCD_DATA13/BT_CFG2[5] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA13_PERIPHERAL                    LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA13_SIGNAL                   lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA13_CHANNEL                          13   /*!< LCDIF lcdif_data channel: 13 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA13_PIN_NAME                 LCD_DATA13   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA13_LABEL       "LCD_DATA13/BT_CFG2[5]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA13_NAME                   "LCD_DATA13"   /*!< Identifier name */

/* LCD_DATA14 (coord A12), LCD_DATA14/BT_CFG2[6] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA14_PERIPHERAL                    LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA14_SIGNAL                   lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA14_CHANNEL                          14   /*!< LCDIF lcdif_data channel: 14 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA14_PIN_NAME                 LCD_DATA14   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA14_LABEL       "LCD_DATA14/BT_CFG2[6]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA14_NAME                   "LCD_DATA14"   /*!< Identifier name */

/* LCD_DATA15 (coord D13), LCD_DATA15/BT_CFG2[7] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA15_PERIPHERAL                    LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA15_SIGNAL                   lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA15_CHANNEL                          15   /*!< LCDIF lcdif_data channel: 15 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA15_PIN_NAME                 LCD_DATA15   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA15_LABEL       "LCD_DATA15/BT_CFG2[7]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA15_NAME                   "LCD_DATA15"   /*!< Identifier name */

/* LCD_DATA16 (coord C13), LCD_DATA16/BT_CFG4[0] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA16_PERIPHERAL                    LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA16_SIGNAL                   lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA16_CHANNEL                          16   /*!< LCDIF lcdif_data channel: 16 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA16_PIN_NAME                 LCD_DATA16   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA16_LABEL       "LCD_DATA16/BT_CFG4[0]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA16_NAME                   "LCD_DATA16"   /*!< Identifier name */

/* LCD_DATA17 (coord B13), LCD_DATA17/BT_CFG4[1] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA17_PERIPHERAL                    LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA17_SIGNAL                   lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA17_CHANNEL                          17   /*!< LCDIF lcdif_data channel: 17 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA17_PIN_NAME                 LCD_DATA17   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA17_LABEL       "LCD_DATA17/BT_CFG4[1]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA17_NAME                   "LCD_DATA17"   /*!< Identifier name */

/* LCD_DATA18 (coord A13), LCD_DATA18/BT_CFG4[2] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA18_PERIPHERAL                    LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA18_SIGNAL                   lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA18_CHANNEL                          18   /*!< LCDIF lcdif_data channel: 18 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA18_PIN_NAME                 LCD_DATA18   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA18_LABEL       "LCD_DATA18/BT_CFG4[2]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA18_NAME                   "LCD_DATA18"   /*!< Identifier name */

/* LCD_DATA19 (coord D14), LCD_DATA19/BT_CFG4[3] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA19_PERIPHERAL                    LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA19_SIGNAL                   lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA19_CHANNEL                          19   /*!< LCDIF lcdif_data channel: 19 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA19_PIN_NAME                 LCD_DATA19   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA19_LABEL       "LCD_DATA19/BT_CFG4[3]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA19_NAME                   "LCD_DATA19"   /*!< Identifier name */

/* LCD_DATA20 (coord C14), LCD_DATA20/BT_CFG4[4] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA20_PERIPHERAL                    LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA20_SIGNAL                   lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA20_CHANNEL                          20   /*!< LCDIF lcdif_data channel: 20 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA20_PIN_NAME                 LCD_DATA20   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA20_LABEL       "LCD_DATA20/BT_CFG4[4]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA20_NAME                   "LCD_DATA20"   /*!< Identifier name */

/* LCD_DATA21 (coord B14), LCD_DATA21/BT_CFG4[5] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA21_PERIPHERAL                    LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA21_SIGNAL                   lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA21_CHANNEL                          21   /*!< LCDIF lcdif_data channel: 21 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA21_PIN_NAME                 LCD_DATA21   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA21_LABEL       "LCD_DATA21/BT_CFG4[5]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA21_NAME                   "LCD_DATA21"   /*!< Identifier name */

/* LCD_DATA22 (coord A14), LCD_DATA22/BT_CFG4[6] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA22_PERIPHERAL                    LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA22_SIGNAL                   lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA22_CHANNEL                          22   /*!< LCDIF lcdif_data channel: 22 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA22_PIN_NAME                 LCD_DATA22   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA22_LABEL       "LCD_DATA22/BT_CFG4[6]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA22_NAME                   "LCD_DATA22"   /*!< Identifier name */

/* LCD_DATA23 (coord B16), LCD_DATA23/BT_CFG4[7] */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA23_PERIPHERAL                    LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA23_SIGNAL                   lcdif_data   /*!< LCDIF signal: lcdif_data */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA23_CHANNEL                          23   /*!< LCDIF lcdif_data channel: 23 */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA23_PIN_NAME                 LCD_DATA23   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA23_LABEL       "LCD_DATA23/BT_CFG4[7]"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DATA23_NAME                   "LCD_DATA23"   /*!< Identifier name */

/* LCD_ENABLE (coord B8), LCD_DE */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DE_PERIPHERAL                        LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DE_SIGNAL                     lcdif_enable   /*!< LCDIF signal: lcdif_enable */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DE_PIN_NAME                     LCD_ENABLE   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DE_LABEL                          "LCD_DE"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_DE_NAME                           "LCD_DE"   /*!< Identifier name */

/* LCD_HSYNC (coord D9), LCD_HSYNC */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_HSYNC_PERIPHERAL                     LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_HSYNC_SIGNAL                   lcdif_hsync   /*!< LCDIF signal: lcdif_hsync */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_HSYNC_PIN_NAME                   LCD_HSYNC   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_HSYNC_LABEL                    "LCD_HSYNC"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_HSYNC_NAME                     "LCD_HSYNC"   /*!< Identifier name */

/* LCD_VSYNC (coord C9), LCD_VSYNC */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_VSYNC_PERIPHERAL                     LCDIF   /*!< Device name: LCDIF */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_VSYNC_SIGNAL                   lcdif_vsync   /*!< LCDIF signal: lcdif_vsync */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_VSYNC_PIN_NAME                   LCD_VSYNC   /*!< Pin name */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_VSYNC_LABEL                    "LCD_VSYNC"   /*!< Label */
#define BOARD_LCDIF_CONFIGUREPINS_LCD_VSYNC_NAME                     "LCD_VSYNC"   /*!< Identifier name */

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*!
 * @brief Initialize eLCDIF pins.
 *
 */
void BOARD_LCDIF_ConfigurePins(void);

#if defined(__cplusplus)
}
#endif

#endif /* _PIN_MUX_H_ */
