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
#define BOARD_INITPINS_USB_OTG1_ID_PERIPHERAL                               ADC1   /*!< Device name: ADC1 */
#define BOARD_INITPINS_USB_OTG1_ID_SIGNAL                                adc_in0   /*!< ADC1 signal: adc_in0 */
#define BOARD_INITPINS_USB_OTG1_ID_PIN_NAME                           GPIO1_IO00   /*!< Pin name */
#define BOARD_INITPINS_USB_OTG1_ID_LABEL                    "GPIO_0/USB_OTG1_ID"   /*!< Label */
#define BOARD_INITPINS_USB_OTG1_ID_NAME                            "USB_OTG1_ID"   /*!< Identifier name */

/* GPIO1_IO01 (coord L15), GPIO_1/USB_OTG1_OC */
#define BOARD_INITPINS_USB_OTG1_OC_PERIPHERAL                               ADC1   /*!< Device name: ADC1 */
#define BOARD_INITPINS_USB_OTG1_OC_SIGNAL                                adc_in1   /*!< ADC1 signal: adc_in1 */
#define BOARD_INITPINS_USB_OTG1_OC_PIN_NAME                           GPIO1_IO01   /*!< Pin name */
#define BOARD_INITPINS_USB_OTG1_OC_LABEL                    "GPIO_1/USB_OTG1_OC"   /*!< Label */
#define BOARD_INITPINS_USB_OTG1_OC_NAME                            "USB_OTG1_OC"   /*!< Identifier name */

/* GPIO1_IO02 (coord L14), GPIO_2/USB_OTG2_PWR */
#define BOARD_INITPINS_USB_OTG2_PWR_PERIPHERAL                              ADC1   /*!< Device name: ADC1 */
#define BOARD_INITPINS_USB_OTG2_PWR_SIGNAL                               adc_in2   /*!< ADC1 signal: adc_in2 */
#define BOARD_INITPINS_USB_OTG2_PWR_PIN_NAME                          GPIO1_IO02   /*!< Pin name */
#define BOARD_INITPINS_USB_OTG2_PWR_LABEL                  "GPIO_2/USB_OTG2_PWR"   /*!< Label */
#define BOARD_INITPINS_USB_OTG2_PWR_NAME                          "USB_OTG2_PWR"   /*!< Identifier name */

/* GPIO1_IO03 (coord L17), GPIO_3/USB_OTG2_OC */
#define BOARD_INITPINS_USB_OTG2_OC_PERIPHERAL                               ADC1   /*!< Device name: ADC1 */
#define BOARD_INITPINS_USB_OTG2_OC_SIGNAL                                adc_in3   /*!< ADC1 signal: adc_in3 */
#define BOARD_INITPINS_USB_OTG2_OC_PIN_NAME                           GPIO1_IO03   /*!< Pin name */
#define BOARD_INITPINS_USB_OTG2_OC_LABEL                    "GPIO_3/USB_OTG2_OC"   /*!< Label */
#define BOARD_INITPINS_USB_OTG2_OC_NAME                            "USB_OTG2_OC"   /*!< Identifier name */

/* GPIO1_IO04 (coord M16), GPIO_4/USB_OTG1_PWR */
#define BOARD_INITPINS_USB_OTG1_PWR_PERIPHERAL                              ADC1   /*!< Device name: ADC1 */
#define BOARD_INITPINS_USB_OTG1_PWR_SIGNAL                               adc_in4   /*!< ADC1 signal: adc_in4 */
#define BOARD_INITPINS_USB_OTG1_PWR_PIN_NAME                          GPIO1_IO04   /*!< Pin name */
#define BOARD_INITPINS_USB_OTG1_PWR_LABEL                  "GPIO_4/USB_OTG1_PWR"   /*!< Label */
#define BOARD_INITPINS_USB_OTG1_PWR_NAME                          "USB_OTG1_PWR"   /*!< Identifier name */

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
