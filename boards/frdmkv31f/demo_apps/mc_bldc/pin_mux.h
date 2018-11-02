/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
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



/* PORTA4 (number 38), SW2/NMI_b */
#define BOARD_INITPINS_SW2_GPIO                                            GPIOA   /*!< GPIO device name: GPIOA */
#define BOARD_INITPINS_SW2_PORT                                            PORTA   /*!< PORT device name: PORTA */
#define BOARD_INITPINS_SW2_GPIO_PIN                                           4U   /*!< PORTA pin index: 4 */
#define BOARD_INITPINS_SW2_PIN_NAME                                         PTA4   /*!< Pin name */
#define BOARD_INITPINS_SW2_LABEL                                     "SW2/NMI_b"   /*!< Label */
#define BOARD_INITPINS_SW2_NAME                                            "SW2"   /*!< Identifier name */

/* PORTD1 (number 94), D4[1]/LEDRGB_RED */
#define BOARD_INITPINS_LED_RED_GPIO                                        GPIOD   /*!< GPIO device name: GPIOD */
#define BOARD_INITPINS_LED_RED_PORT                                        PORTD   /*!< PORT device name: PORTD */
#define BOARD_INITPINS_LED_RED_GPIO_PIN                                       1U   /*!< PORTD pin index: 1 */
#define BOARD_INITPINS_LED_RED_PIN_NAME                                     PTD1   /*!< Pin name */
#define BOARD_INITPINS_LED_RED_LABEL                          "D4[1]/LEDRGB_RED"   /*!< Label */
#define BOARD_INITPINS_LED_RED_NAME                                    "LED_RED"   /*!< Identifier name */

/* PORTD7 (number 100), D4[4]/LEDRGB_GREEN */
#define BOARD_INITPINS_LED_GREEN_GPIO                                      GPIOD   /*!< GPIO device name: GPIOD */
#define BOARD_INITPINS_LED_GREEN_PORT                                      PORTD   /*!< PORT device name: PORTD */
#define BOARD_INITPINS_LED_GREEN_GPIO_PIN                                     7U   /*!< PORTD pin index: 7 */
#define BOARD_INITPINS_LED_GREEN_PIN_NAME                                   PTD7   /*!< Pin name */
#define BOARD_INITPINS_LED_GREEN_LABEL                      "D4[4]/LEDRGB_GREEN"   /*!< Label */
#define BOARD_INITPINS_LED_GREEN_NAME                                "LED_GREEN"   /*!< Identifier name */

/* PORTC1 (number 71), J3[15]/FTM0_CH0 */
#define BOARD_INITPINS_FTM0_CH0_PERIPHERAL                                  FTM0   /*!< Device name: FTM0 */
#define BOARD_INITPINS_FTM0_CH0_SIGNAL                                        CH   /*!< FTM0 signal: CH */
#define BOARD_INITPINS_FTM0_CH0_CHANNEL                                        0   /*!< FTM0 channel: 0 */
#define BOARD_INITPINS_FTM0_CH0_PIN_NAME                                FTM0_CH0   /*!< Pin name */
#define BOARD_INITPINS_FTM0_CH0_LABEL                          "J3[15]/FTM0_CH0"   /*!< Label */
#define BOARD_INITPINS_FTM0_CH0_NAME                                  "FTM0_CH0"   /*!< Identifier name */

/* PORTC2 (number 72), J3[13]/FTM0_CH1 */
#define BOARD_INITPINS_FTM0_CH1_PERIPHERAL                                  FTM0   /*!< Device name: FTM0 */
#define BOARD_INITPINS_FTM0_CH1_SIGNAL                                        CH   /*!< FTM0 signal: CH */
#define BOARD_INITPINS_FTM0_CH1_CHANNEL                                        1   /*!< FTM0 channel: 1 */
#define BOARD_INITPINS_FTM0_CH1_PIN_NAME                                FTM0_CH1   /*!< Pin name */
#define BOARD_INITPINS_FTM0_CH1_LABEL                          "J3[13]/FTM0_CH1"   /*!< Label */
#define BOARD_INITPINS_FTM0_CH1_NAME                                  "FTM0_CH1"   /*!< Identifier name */

/* PORTD4 (number 97), J3[7]/FTM0_CH4 */
#define BOARD_INITPINS_FTM0_CH4_PERIPHERAL                                  FTM0   /*!< Device name: FTM0 */
#define BOARD_INITPINS_FTM0_CH4_SIGNAL                                        CH   /*!< FTM0 signal: CH */
#define BOARD_INITPINS_FTM0_CH4_CHANNEL                                        4   /*!< FTM0 channel: 4 */
#define BOARD_INITPINS_FTM0_CH4_PIN_NAME                                FTM0_CH4   /*!< Pin name */
#define BOARD_INITPINS_FTM0_CH4_LABEL                           "J3[7]/FTM0_CH4"   /*!< Label */
#define BOARD_INITPINS_FTM0_CH4_NAME                                  "FTM0_CH4"   /*!< Identifier name */

/* PORTD5 (number 98), J3[5]/FTM0_CH5 */
#define BOARD_INITPINS_FTM0_CH5_PERIPHERAL                                  FTM0   /*!< Device name: FTM0 */
#define BOARD_INITPINS_FTM0_CH5_SIGNAL                                        CH   /*!< FTM0 signal: CH */
#define BOARD_INITPINS_FTM0_CH5_CHANNEL                                        5   /*!< FTM0 channel: 5 */
#define BOARD_INITPINS_FTM0_CH5_PIN_NAME                                FTM0_CH5   /*!< Pin name */
#define BOARD_INITPINS_FTM0_CH5_LABEL                           "J3[5]/FTM0_CH5"   /*!< Label */
#define BOARD_INITPINS_FTM0_CH5_NAME                                  "FTM0_CH5"   /*!< Identifier name */


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
