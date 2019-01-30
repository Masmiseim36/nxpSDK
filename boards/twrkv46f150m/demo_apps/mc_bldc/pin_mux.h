/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
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



/* PORTE0 (number 1), J500[A44]/J501[5]/J505[4]/GPIOE0/TXD1 */
#define BOARD_INITPINS_DEBUG_UART_TX_PERIPHERAL                            UART1   /*!< Device name: UART1 */
#define BOARD_INITPINS_DEBUG_UART_TX_SIGNAL                                   TX   /*!< UART1 signal: TX */
#define BOARD_INITPINS_DEBUG_UART_TX_PIN_NAME                           UART1_TX   /*!< Pin name */
#define BOARD_INITPINS_DEBUG_UART_TX_LABEL "J500[A44]/J501[5]/J505[4]/GPIOE0/TXD1" /*!< Label */
#define BOARD_INITPINS_DEBUG_UART_TX_NAME                        "DEBUG_UART_TX"   /*!< Identifier name */

/* PORTE1 (number 2), J500[A43]/J501[7]/J506[4]/GPIOE1/RXD1 */
#define BOARD_INITPINS_DEBUG_UART_RX_PERIPHERAL                            UART1   /*!< Device name: UART1 */
#define BOARD_INITPINS_DEBUG_UART_RX_SIGNAL                                   RX   /*!< UART1 signal: RX */
#define BOARD_INITPINS_DEBUG_UART_RX_PIN_NAME                           UART1_RX   /*!< Pin name */
#define BOARD_INITPINS_DEBUG_UART_RX_LABEL "J500[A43]/J501[7]/J506[4]/GPIOE1/RXD1" /*!< Label */
#define BOARD_INITPINS_DEBUG_UART_RX_NAME                        "DEBUG_UART_RX"   /*!< Identifier name */

/* PORTE6 (number 7), J5[1]/J502[11]/GPIOE6/FTM3_CH1/SW2 */
#define BOARD_INITPINS_SW2_GPIO                                            GPIOE   /*!< GPIO device name: GPIOE */
#define BOARD_INITPINS_SW2_PORT                                            PORTE   /*!< PORT device name: PORTE */
#define BOARD_INITPINS_SW2_GPIO_PIN                                           6U   /*!< PORTE pin index: 6 */
#define BOARD_INITPINS_SW2_PIN_NAME                                         PTE6   /*!< Pin name */
#define BOARD_INITPINS_SW2_LABEL            "J5[1]/J502[11]/GPIOE6/FTM3_CH1/SW2"   /*!< Label */
#define BOARD_INITPINS_SW2_NAME                                            "SW2"   /*!< Identifier name */

/* PORTD0 (number 93), J500[A40]/J501[9]/GPIOD0/PWM_A0/LED0 */
#define BOARD_INITPINS_LED_GREEEN1_PERIPHERAL                               FTM0   /*!< Device name: FTM0 */
#define BOARD_INITPINS_LED_GREEEN1_SIGNAL                                     CH   /*!< FTM0 signal: CH */
#define BOARD_INITPINS_LED_GREEEN1_CHANNEL                                     0   /*!< FTM0 channel: 0 */
#define BOARD_INITPINS_LED_GREEEN1_PIN_NAME                             FTM0_CH0   /*!< Pin name */
#define BOARD_INITPINS_LED_GREEEN1_LABEL  "J500[A40]/J501[9]/GPIOD0/PWM_A0/LED0"   /*!< Label */
#define BOARD_INITPINS_LED_GREEEN1_NAME                            "LED_GREEEN1"   /*!< Identifier name */
#define BOARD_INITPINS_LED_GREEEN1_DIRECTION            kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTD1 (number 94), J500[A39]/J501[11]/GPIOD1/PWM_B0/LED1 */
#define BOARD_INITPINS_LED_YELLOW1_PERIPHERAL                               FTM0   /*!< Device name: FTM0 */
#define BOARD_INITPINS_LED_YELLOW1_SIGNAL                                     CH   /*!< FTM0 signal: CH */
#define BOARD_INITPINS_LED_YELLOW1_CHANNEL                                     1   /*!< FTM0 channel: 1 */
#define BOARD_INITPINS_LED_YELLOW1_PIN_NAME                             FTM0_CH1   /*!< Pin name */
#define BOARD_INITPINS_LED_YELLOW1_LABEL "J500[A39]/J501[11]/GPIOD1/PWM_B0/LED1"   /*!< Label */
#define BOARD_INITPINS_LED_YELLOW1_NAME                            "LED_YELLOW1"   /*!< Identifier name */
#define BOARD_INITPINS_LED_YELLOW1_DIRECTION            kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTD2 (number 95), J500[A38]/J501[30]/GPIOD2/PWM_A1/LED2 */
#define BOARD_INITPINS_LED_GREEN2_PERIPHERAL                                FTM0   /*!< Device name: FTM0 */
#define BOARD_INITPINS_LED_GREEN2_SIGNAL                                      CH   /*!< FTM0 signal: CH */
#define BOARD_INITPINS_LED_GREEN2_CHANNEL                                      2   /*!< FTM0 channel: 2 */
#define BOARD_INITPINS_LED_GREEN2_PIN_NAME                              FTM0_CH2   /*!< Pin name */
#define BOARD_INITPINS_LED_GREEN2_LABEL  "J500[A38]/J501[30]/GPIOD2/PWM_A1/LED2"   /*!< Label */
#define BOARD_INITPINS_LED_GREEN2_NAME                              "LED_GREEN2"   /*!< Identifier name */
#define BOARD_INITPINS_LED_GREEN2_DIRECTION             kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTD3 (number 96), J500[A37]/J501[32]/GPIOD3/PWM_B1/LED3 */
#define BOARD_INITPINS_LED_YELLOW2_PERIPHERAL                               FTM0   /*!< Device name: FTM0 */
#define BOARD_INITPINS_LED_YELLOW2_SIGNAL                                     CH   /*!< FTM0 signal: CH */
#define BOARD_INITPINS_LED_YELLOW2_CHANNEL                                     3   /*!< FTM0 channel: 3 */
#define BOARD_INITPINS_LED_YELLOW2_PIN_NAME                             FTM0_CH3   /*!< Pin name */
#define BOARD_INITPINS_LED_YELLOW2_LABEL "J500[A37]/J501[32]/GPIOD3/PWM_B1/LED3"   /*!< Label */
#define BOARD_INITPINS_LED_YELLOW2_NAME                            "LED_YELLOW2"   /*!< Identifier name */
#define BOARD_INITPINS_LED_YELLOW2_DIRECTION            kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTD4 (number 97), J500[B40]/J501[34]/GPIOD4/PWM_A2/LED4 */
#define BOARD_INITPINS_LED_GREEN3_PERIPHERAL                                FTM0   /*!< Device name: FTM0 */
#define BOARD_INITPINS_LED_GREEN3_SIGNAL                                      CH   /*!< FTM0 signal: CH */
#define BOARD_INITPINS_LED_GREEN3_CHANNEL                                      4   /*!< FTM0 channel: 4 */
#define BOARD_INITPINS_LED_GREEN3_PIN_NAME                              FTM0_CH4   /*!< Pin name */
#define BOARD_INITPINS_LED_GREEN3_LABEL  "J500[B40]/J501[34]/GPIOD4/PWM_A2/LED4"   /*!< Label */
#define BOARD_INITPINS_LED_GREEN3_NAME                              "LED_GREEN3"   /*!< Identifier name */
#define BOARD_INITPINS_LED_GREEN3_DIRECTION             kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTD5 (number 98), J500[B39]/J501[36]/GPIOD5/PWM_B2/LED5 */
#define BOARD_INITPINS_LED_YELLOW3_PERIPHERAL                               FTM0   /*!< Device name: FTM0 */
#define BOARD_INITPINS_LED_YELLOW3_SIGNAL                                     CH   /*!< FTM0 signal: CH */
#define BOARD_INITPINS_LED_YELLOW3_CHANNEL                                     5   /*!< FTM0 channel: 5 */
#define BOARD_INITPINS_LED_YELLOW3_PIN_NAME                             FTM0_CH5   /*!< Pin name */
#define BOARD_INITPINS_LED_YELLOW3_LABEL "J500[B39]/J501[36]/GPIOD5/PWM_B2/LED5"   /*!< Label */
#define BOARD_INITPINS_LED_YELLOW3_NAME                            "LED_YELLOW3"   /*!< Identifier name */
#define BOARD_INITPINS_LED_YELLOW3_DIRECTION            kPIN_MUX_DirectionOutput   /*!< Direction */


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
