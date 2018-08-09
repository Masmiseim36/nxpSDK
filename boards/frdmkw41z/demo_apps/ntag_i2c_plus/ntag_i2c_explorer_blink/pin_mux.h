/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/* PORTC4 (number 40), SW3 */
#define BOARD_INITBUTTONS_SW3_GPIO                                         GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_INITBUTTONS_SW3_PORT                                         PORTC   /*!< PORT device name: PORTC */
#define BOARD_INITBUTTONS_SW3_GPIO_PIN                                        4U   /*!< PORTC pin index: 4 */
#define BOARD_INITBUTTONS_SW3_PIN_NAME                                      PTC4   /*!< Pin name */
#define BOARD_INITBUTTONS_SW3_LABEL                                        "SW3"   /*!< Label */
#define BOARD_INITBUTTONS_SW3_NAME                                         "SW3"   /*!< Identifier name */
#define BOARD_INITBUTTONS_SW3_DIRECTION                  kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTC5 (number 41), SW4 */
#define BOARD_INITBUTTONS_SW4_GPIO                                         GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_INITBUTTONS_SW4_PORT                                         PORTC   /*!< PORT device name: PORTC */
#define BOARD_INITBUTTONS_SW4_GPIO_PIN                                        5U   /*!< PORTC pin index: 5 */
#define BOARD_INITBUTTONS_SW4_PIN_NAME                                      PTC5   /*!< Pin name */
#define BOARD_INITBUTTONS_SW4_LABEL                                        "SW4"   /*!< Label */
#define BOARD_INITBUTTONS_SW4_NAME                                         "SW4"   /*!< Identifier name */
#define BOARD_INITBUTTONS_SW4_DIRECTION                  kPIN_MUX_DirectionInput   /*!< Direction */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitButtons(void);

/* PORTC1 (number 37), LEDRGB_RED */
#define BOARD_INITLEDS_LED_RED_GPIO                                        GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_INITLEDS_LED_RED_PORT                                        PORTC   /*!< PORT device name: PORTC */
#define BOARD_INITLEDS_LED_RED_GPIO_PIN                                       1U   /*!< PORTC pin index: 1 */
#define BOARD_INITLEDS_LED_RED_PIN_NAME                                     PTC1   /*!< Pin name */
#define BOARD_INITLEDS_LED_RED_LABEL                                "LEDRGB_RED"   /*!< Label */
#define BOARD_INITLEDS_LED_RED_NAME                                    "LED_RED"   /*!< Identifier name */
#define BOARD_INITLEDS_LED_RED_DIRECTION                kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTA18 (number 6), LEDRGB_BLUE */
#define BOARD_INITLEDS_LED_BLUE_GPIO                                       GPIOA   /*!< GPIO device name: GPIOA */
#define BOARD_INITLEDS_LED_BLUE_PORT                                       PORTA   /*!< PORT device name: PORTA */
#define BOARD_INITLEDS_LED_BLUE_GPIO_PIN                                     18U   /*!< PORTA pin index: 18 */
#define BOARD_INITLEDS_LED_BLUE_PIN_NAME                                   PTA18   /*!< Pin name */
#define BOARD_INITLEDS_LED_BLUE_LABEL                              "LEDRGB_BLUE"   /*!< Label */
#define BOARD_INITLEDS_LED_BLUE_NAME                                  "LED_BLUE"   /*!< Identifier name */
#define BOARD_INITLEDS_LED_BLUE_DIRECTION               kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTA19 (number 7), LEDRGB_GREEN */
#define BOARD_INITLEDS_LED_GREEN_GPIO                                      GPIOA   /*!< GPIO device name: GPIOA */
#define BOARD_INITLEDS_LED_GREEN_PORT                                      PORTA   /*!< PORT device name: PORTA */
#define BOARD_INITLEDS_LED_GREEN_GPIO_PIN                                    19U   /*!< PORTA pin index: 19 */
#define BOARD_INITLEDS_LED_GREEN_PIN_NAME                                  PTA19   /*!< Pin name */
#define BOARD_INITLEDS_LED_GREEN_LABEL                            "LEDRGB_GREEN"   /*!< Label */
#define BOARD_INITLEDS_LED_GREEN_NAME                                "LED_GREEN"   /*!< Identifier name */
#define BOARD_INITLEDS_LED_GREEN_DIRECTION              kPIN_MUX_DirectionOutput   /*!< Direction */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDs(void);

/* PORTC7 (number 43), UART0_TX */
#define BOARD_INITDEBUG_UART_DEBUG_UART_TX_PERIPHERAL                    LPUART0   /*!< Device name: LPUART0 */
#define BOARD_INITDEBUG_UART_DEBUG_UART_TX_SIGNAL                             TX   /*!< LPUART0 signal: TX */
#define BOARD_INITDEBUG_UART_DEBUG_UART_TX_PIN_NAME                     UART0_TX   /*!< Pin name */
#define BOARD_INITDEBUG_UART_DEBUG_UART_TX_LABEL                      "UART0_TX"   /*!< Label */
#define BOARD_INITDEBUG_UART_DEBUG_UART_TX_NAME                  "DEBUG_UART_TX"   /*!< Identifier name */
#define BOARD_INITDEBUG_UART_DEBUG_UART_TX_DIRECTION    kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTC6 (number 42), UART0_RX */
#define BOARD_INITDEBUG_UART_DEBUG_UART_RX_PERIPHERAL                    LPUART0   /*!< Device name: LPUART0 */
#define BOARD_INITDEBUG_UART_DEBUG_UART_RX_SIGNAL                             RX   /*!< LPUART0 signal: RX */
#define BOARD_INITDEBUG_UART_DEBUG_UART_RX_PIN_NAME                     UART0_RX   /*!< Pin name */
#define BOARD_INITDEBUG_UART_DEBUG_UART_RX_LABEL                      "UART0_RX"   /*!< Label */
#define BOARD_INITDEBUG_UART_DEBUG_UART_RX_NAME                  "DEBUG_UART_RX"   /*!< Identifier name */
#define BOARD_INITDEBUG_UART_DEBUG_UART_RX_DIRECTION     kPIN_MUX_DirectionInput   /*!< Direction */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitDEBUG_UART(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void I2C1_InitPins(void);

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
