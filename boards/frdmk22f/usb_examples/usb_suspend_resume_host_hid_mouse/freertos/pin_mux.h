/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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


/* PORTE1 (number 2), J2[20]/UART1_RX_TGTMCU */
#define BOARD_INITPINS_DEBUG_UART_RX_PERIPHERAL                            UART1   /*!< Device name: UART1 */
#define BOARD_INITPINS_DEBUG_UART_RX_SIGNAL                                   RX   /*!< UART1 signal: RX */
#define BOARD_INITPINS_DEBUG_UART_RX_PIN_NAME                           UART1_RX   /*!< Pin name */
#define BOARD_INITPINS_DEBUG_UART_RX_LABEL              "J2[20]/UART1_RX_TGTMCU"   /*!< Label */
#define BOARD_INITPINS_DEBUG_UART_RX_NAME                        "DEBUG_UART_RX"   /*!< Identifier name */

/* PORTC1 (number 44), J24[6]/LLWU_P6/ADC0_SE15/PUSH_BUTTON2 */
#define BOARD_INITPINS_SW2_GPIO                                            GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_INITPINS_SW2_PORT                                            PORTC   /*!< PORT device name: PORTC */
#define BOARD_INITPINS_SW2_GPIO_PIN                                           1U   /*!< PORTC pin index: 1 */
#define BOARD_INITPINS_SW2_PIN_NAME                                         PTC1   /*!< Pin name */
#define BOARD_INITPINS_SW2_LABEL         "J24[6]/LLWU_P6/ADC0_SE15/PUSH_BUTTON2"   /*!< Label */
#define BOARD_INITPINS_SW2_NAME                                            "SW2"   /*!< Identifier name */
#define BOARD_INITPINS_SW2_DIRECTION                     kPIN_MUX_DirectionInput   /*!< Direction */


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

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
