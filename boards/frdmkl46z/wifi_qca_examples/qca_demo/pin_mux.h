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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/*! @brief Direction type  */
typedef enum _pin_mux_direction
{
    kPIN_MUX_DirectionInput = 0U,        /* Input direction */
    kPIN_MUX_DirectionOutput = 1U,       /* Output direction */
    kPIN_MUX_DirectionInputOrOutput = 2U /* Input or output direction */
} pin_mux_direction_t;

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


#define SOPT5_UART0RXSRC_UART_RX 0x00u /*!<@brief UART0 Receive Data Source Select: UART_RX pin */
#define SOPT5_UART0TXSRC_UART_TX 0x00u /*!<@brief UART0 Transmit Data Source Select: UART0_TX pin */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*! @name PORTC9 (number 85), IRQ
  @{ */
#define BOARD_INITGT202SHIELD_IRQ_PERIPHERAL GPIOC                  /*!<@brief Device name: GPIOC */
#define BOARD_INITGT202SHIELD_IRQ_SIGNAL GPIO                       /*!<@brief GPIOC signal: GPIO */
#define BOARD_INITGT202SHIELD_IRQ_GPIO GPIOC                        /*!<@brief GPIO device name: GPIOC */
#define BOARD_INITGT202SHIELD_IRQ_GPIO_PIN 9U                       /*!<@brief PORTC pin index: 9 */
#define BOARD_INITGT202SHIELD_IRQ_PORT PORTC                        /*!<@brief PORT device name: PORTC */
#define BOARD_INITGT202SHIELD_IRQ_PIN 9U                            /*!<@brief PORTC pin index: 9 */
#define BOARD_INITGT202SHIELD_IRQ_CHANNEL 9                         /*!<@brief GPIOC GPIO channel: 9 */
#define BOARD_INITGT202SHIELD_IRQ_PIN_NAME PTC9                     /*!<@brief Pin name */
#define BOARD_INITGT202SHIELD_IRQ_LABEL "IRQ"                       /*!<@brief Label */
#define BOARD_INITGT202SHIELD_IRQ_NAME "IRQ"                        /*!<@brief Identifier name */
#define BOARD_INITGT202SHIELD_IRQ_DIRECTION kPIN_MUX_DirectionInput /*!<@brief Direction */
                                                                    /* @} */

/*! @name PORTA13 (number 43), PWRON
  @{ */
#define BOARD_INITGT202SHIELD_PWRON_PERIPHERAL GPIOA                   /*!<@brief Device name: GPIOA */
#define BOARD_INITGT202SHIELD_PWRON_SIGNAL GPIO                        /*!<@brief GPIOA signal: GPIO */
#define BOARD_INITGT202SHIELD_PWRON_GPIO GPIOA                         /*!<@brief GPIO device name: GPIOA */
#define BOARD_INITGT202SHIELD_PWRON_GPIO_PIN 13U                       /*!<@brief PORTA pin index: 13 */
#define BOARD_INITGT202SHIELD_PWRON_PORT PORTA                         /*!<@brief PORT device name: PORTA */
#define BOARD_INITGT202SHIELD_PWRON_PIN 13U                            /*!<@brief PORTA pin index: 13 */
#define BOARD_INITGT202SHIELD_PWRON_CHANNEL 13                         /*!<@brief GPIOA GPIO channel: 13 */
#define BOARD_INITGT202SHIELD_PWRON_PIN_NAME PTA13                     /*!<@brief Pin name */
#define BOARD_INITGT202SHIELD_PWRON_LABEL "PWRON"                      /*!<@brief Label */
#define BOARD_INITGT202SHIELD_PWRON_NAME "PWRON"                       /*!<@brief Identifier name */
#define BOARD_INITGT202SHIELD_PWRON_DIRECTION kPIN_MUX_DirectionOutput /*!<@brief Direction */
                                                                       /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitGT202Shield(void);

/*! @name PORTD3 (number 96), IRQ
  @{ */
#define BOARD_INITSILEX2401SHIELD_IRQ_PERIPHERAL GPIOD                  /*!<@brief Device name: GPIOD */
#define BOARD_INITSILEX2401SHIELD_IRQ_SIGNAL GPIO                       /*!<@brief GPIOD signal: GPIO */
#define BOARD_INITSILEX2401SHIELD_IRQ_GPIO GPIOD                        /*!<@brief GPIO device name: GPIOD */
#define BOARD_INITSILEX2401SHIELD_IRQ_GPIO_PIN 3U                       /*!<@brief PORTD pin index: 3 */
#define BOARD_INITSILEX2401SHIELD_IRQ_PORT PORTD                        /*!<@brief PORT device name: PORTD */
#define BOARD_INITSILEX2401SHIELD_IRQ_PIN 3U                            /*!<@brief PORTD pin index: 3 */
#define BOARD_INITSILEX2401SHIELD_IRQ_CHANNEL 3                         /*!<@brief GPIOD GPIO channel: 3 */
#define BOARD_INITSILEX2401SHIELD_IRQ_PIN_NAME PTD3                     /*!<@brief Pin name */
#define BOARD_INITSILEX2401SHIELD_IRQ_LABEL "IRQ"                       /*!<@brief Label */
#define BOARD_INITSILEX2401SHIELD_IRQ_NAME "IRQ"                        /*!<@brief Identifier name */
#define BOARD_INITSILEX2401SHIELD_IRQ_DIRECTION kPIN_MUX_DirectionInput /*!<@brief Direction */
                                                                        /* @} */

/*! @name PORTA4 (number 38), PWRON
  @{ */
#define BOARD_INITSILEX2401SHIELD_PWRON_PERIPHERAL GPIOA                   /*!<@brief Device name: GPIOA */
#define BOARD_INITSILEX2401SHIELD_PWRON_SIGNAL GPIO                        /*!<@brief GPIOA signal: GPIO */
#define BOARD_INITSILEX2401SHIELD_PWRON_GPIO GPIOA                         /*!<@brief GPIO device name: GPIOA */
#define BOARD_INITSILEX2401SHIELD_PWRON_GPIO_PIN 4U                        /*!<@brief PORTA pin index: 4 */
#define BOARD_INITSILEX2401SHIELD_PWRON_PORT PORTA                         /*!<@brief PORT device name: PORTA */
#define BOARD_INITSILEX2401SHIELD_PWRON_PIN 4U                             /*!<@brief PORTA pin index: 4 */
#define BOARD_INITSILEX2401SHIELD_PWRON_CHANNEL 4                          /*!<@brief GPIOA GPIO channel: 4 */
#define BOARD_INITSILEX2401SHIELD_PWRON_PIN_NAME PTA4                      /*!<@brief Pin name */
#define BOARD_INITSILEX2401SHIELD_PWRON_LABEL "PWRON"                      /*!<@brief Label */
#define BOARD_INITSILEX2401SHIELD_PWRON_NAME "PWRON"                       /*!<@brief Identifier name */
#define BOARD_INITSILEX2401SHIELD_PWRON_DIRECTION kPIN_MUX_DirectionOutput /*!<@brief Direction */
                                                                           /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitSilex2401Shield(void);

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
