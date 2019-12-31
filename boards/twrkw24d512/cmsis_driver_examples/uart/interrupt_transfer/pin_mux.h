/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Direction type  */
typedef enum _pin_mux_direction
{
  kPIN_MUX_DirectionInput = 0U,         /* Input direction */
  kPIN_MUX_DirectionOutput = 1U,        /* Output direction */
  kPIN_MUX_DirectionInputOrOutput = 2U  /* Input or output direction */
} pin_mux_direction_t;

/*!
 * @addtogroup pin_mux
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/* PORTD6 (number 13), J21[2]/UART0_RX/PTD6_LED_BLUE */
#define UART0_INITPINS_PTD6_LED_BLUE_PERIPHERAL                            UART0   /*!< Device name: UART0 */
#define UART0_INITPINS_PTD6_LED_BLUE_SIGNAL                                   RX   /*!< UART0 signal: RX */
#define UART0_INITPINS_PTD6_LED_BLUE_PIN_NAME                           UART0_RX   /*!< Pin name */
#define UART0_INITPINS_PTD6_LED_BLUE_LABEL       "J21[2]/UART0_RX/PTD6_LED_BLUE"   /*!< Label */
#define UART0_INITPINS_PTD6_LED_BLUE_NAME                        "PTD6_LED_BLUE"   /*!< Identifier name */

/* PORTD7 (number 14), J24[2]/UART0_TX/PTD7_LED_BLUE */
#define UART0_INITPINS_PTD7_LED_BLUE_PERIPHERAL                            UART0   /*!< Device name: UART0 */
#define UART0_INITPINS_PTD7_LED_BLUE_SIGNAL                                   TX   /*!< UART0 signal: TX */
#define UART0_INITPINS_PTD7_LED_BLUE_PIN_NAME                           UART0_TX   /*!< Pin name */
#define UART0_INITPINS_PTD7_LED_BLUE_LABEL       "J24[2]/UART0_TX/PTD7_LED_BLUE"   /*!< Label */
#define UART0_INITPINS_PTD7_LED_BLUE_NAME                        "PTD7_LED_BLUE"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART0_InitPins(void);

/* PORTD6 (number 13), J21[2]/UART0_RX/PTD6_LED_BLUE */
#define UART0_DEINITPINS_PTD6_LED_BLUE_PERIPHERAL                           ADC0   /*!< Device name: ADC0 */
#define UART0_DEINITPINS_PTD6_LED_BLUE_SIGNAL                                 SE   /*!< ADC0 signal: SE */
#define UART0_DEINITPINS_PTD6_LED_BLUE_CHANNEL                                7b   /*!< ADC0 SE channel: 7b */
#define UART0_DEINITPINS_PTD6_LED_BLUE_PIN_NAME                        ADC0_SE7b   /*!< Pin name */
#define UART0_DEINITPINS_PTD6_LED_BLUE_LABEL     "J21[2]/UART0_RX/PTD6_LED_BLUE"   /*!< Label */
#define UART0_DEINITPINS_PTD6_LED_BLUE_NAME                      "PTD6_LED_BLUE"   /*!< Identifier name */

/* PORTD7 (number 14), J24[2]/UART0_TX/PTD7_LED_BLUE */
#define UART0_DEINITPINS_PTD7_LED_BLUE_PERIPHERAL                           ADC0   /*!< Device name: ADC0 */
#define UART0_DEINITPINS_PTD7_LED_BLUE_SIGNAL                                 SE   /*!< ADC0 signal: SE */
#define UART0_DEINITPINS_PTD7_LED_BLUE_CHANNEL                                22   /*!< ADC0 SE channel: 22 */
#define UART0_DEINITPINS_PTD7_LED_BLUE_PIN_NAME                        ADC0_SE22   /*!< Pin name */
#define UART0_DEINITPINS_PTD7_LED_BLUE_LABEL     "J24[2]/UART0_TX/PTD7_LED_BLUE"   /*!< Label */
#define UART0_DEINITPINS_PTD7_LED_BLUE_NAME                      "PTD7_LED_BLUE"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART0_DeinitPins(void);

/* PORTE0 (number 15), U6[24]/J11[12]/J16[A44]/J18[2]/TRACE_CLKOUT/UART1_TX */
#define UART1_INITPINS_DEBUG_UART_TX_PERIPHERAL                            UART1   /*!< Device name: UART1 */
#define UART1_INITPINS_DEBUG_UART_TX_SIGNAL                                   TX   /*!< UART1 signal: TX */
#define UART1_INITPINS_DEBUG_UART_TX_PIN_NAME                           UART1_TX   /*!< Pin name */
#define UART1_INITPINS_DEBUG_UART_TX_LABEL "U6[24]/J11[12]/J16[A44]/J18[2]/TRACE_CLKOUT/UART1_TX" /*!< Label */
#define UART1_INITPINS_DEBUG_UART_TX_NAME                        "DEBUG_UART_TX"   /*!< Identifier name */

/* PORTE1 (number 16), U6[25]/J11[20]/J16[A43]/J18[5]/TRACE_D3/UART1_RX */
#define UART1_INITPINS_DEBUG_UART_RX_PERIPHERAL                            UART1   /*!< Device name: UART1 */
#define UART1_INITPINS_DEBUG_UART_RX_SIGNAL                                   RX   /*!< UART1 signal: RX */
#define UART1_INITPINS_DEBUG_UART_RX_PIN_NAME                           UART1_RX   /*!< Pin name */
#define UART1_INITPINS_DEBUG_UART_RX_LABEL "U6[25]/J11[20]/J16[A43]/J18[5]/TRACE_D3/UART1_RX" /*!< Label */
#define UART1_INITPINS_DEBUG_UART_RX_NAME                        "DEBUG_UART_RX"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART1_InitPins(void);

/* PORTE0 (number 15), U6[24]/J11[12]/J16[A44]/J18[2]/TRACE_CLKOUT/UART1_TX */
#define UART1_DEINITPINS_DEBUG_UART_TX_PERIPHERAL                           ADC0   /*!< Device name: ADC0 */
#define UART1_DEINITPINS_DEBUG_UART_TX_SIGNAL                                 SE   /*!< ADC0 signal: SE */
#define UART1_DEINITPINS_DEBUG_UART_TX_CHANNEL                                10   /*!< ADC0 SE channel: 10 */
#define UART1_DEINITPINS_DEBUG_UART_TX_PIN_NAME                        ADC0_SE10   /*!< Pin name */
#define UART1_DEINITPINS_DEBUG_UART_TX_LABEL "U6[24]/J11[12]/J16[A44]/J18[2]/TRACE_CLKOUT/UART1_TX" /*!< Label */
#define UART1_DEINITPINS_DEBUG_UART_TX_NAME                      "DEBUG_UART_TX"   /*!< Identifier name */

/* PORTE1 (number 16), U6[25]/J11[20]/J16[A43]/J18[5]/TRACE_D3/UART1_RX */
#define UART1_DEINITPINS_DEBUG_UART_RX_PERIPHERAL                           ADC0   /*!< Device name: ADC0 */
#define UART1_DEINITPINS_DEBUG_UART_RX_SIGNAL                                 SE   /*!< ADC0 signal: SE */
#define UART1_DEINITPINS_DEBUG_UART_RX_CHANNEL                                11   /*!< ADC0 SE channel: 11 */
#define UART1_DEINITPINS_DEBUG_UART_RX_PIN_NAME                        ADC0_SE11   /*!< Pin name */
#define UART1_DEINITPINS_DEBUG_UART_RX_LABEL "U6[25]/J11[20]/J16[A43]/J18[5]/TRACE_D3/UART1_RX" /*!< Label */
#define UART1_DEINITPINS_DEBUG_UART_RX_NAME                      "DEBUG_UART_RX"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART1_DeinitPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART2_InitPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART2_DeinitPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
