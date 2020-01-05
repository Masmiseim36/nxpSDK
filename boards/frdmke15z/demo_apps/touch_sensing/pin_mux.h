/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
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

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

/* PORTD0 (number 4), D4[1]/RGB_RED 
  @{ */
#define BOARD_LED_RED_PERIPHERAL                                            FTM0   /*!< Device name: FTM0 */
#define BOARD_LED_RED_SIGNAL                                                  CH   /*!< FTM0 signal: CH */
#define BOARD_LED_RED_CHANNEL                                                  2   /*!< FTM0 channel: 2 */
#define BOARD_LED_RED_PIN_NAME                                          FTM0_CH2   /*!< Pin name */
#define BOARD_LED_RED_LABEL                                      "D4[1]/RGB_RED"   /*!< Label */
#define BOARD_LED_RED_NAME                                             "LED_RED"   /*!< Identifier name */

/* PORTD16 (number 21), D4[4]/RGB_GREEN 
  @{ */
#define BOARD_LED_GREEN_PERIPHERAL                                          FTM0   /*!< Device name: FTM0 */
#define BOARD_LED_GREEN_SIGNAL                                                CH   /*!< FTM0 signal: CH */
#define BOARD_LED_GREEN_CHANNEL                                                1   /*!< FTM0 channel: 1 */
#define BOARD_LED_GREEN_PIN_NAME                                        FTM0_CH1   /*!< Pin name */
#define BOARD_LED_GREEN_LABEL                                  "D4[4]/RGB_GREEN"   /*!< Label */
#define BOARD_LED_GREEN_NAME                                         "LED_GREEN"   /*!< Identifier name */

/* PORTD15 (number 22), D4[3]/RGB_BLUE 
  @{ */
#define BOARD_LED_BLUE_PERIPHERAL                                           FTM0   /*!< Device name: FTM0 */
#define BOARD_LED_BLUE_SIGNAL                                                 CH   /*!< FTM0 signal: CH */
#define BOARD_LED_BLUE_CHANNEL                                                 0   /*!< FTM0 channel: 0 */
#define BOARD_LED_BLUE_PIN_NAME                                         FTM0_CH0   /*!< Pin name */
#define BOARD_LED_BLUE_LABEL                                    "D4[3]/RGB_BLUE"   /*!< Label */
#define BOARD_LED_BLUE_NAME                                           "LED_BLUE"   /*!< Identifier name */

/* PORTC7 (number 80), UART1_TX_TGTMCU 
  @{ */
#define BOARD_DEBUG_UART_TX_PERIPHERAL                                   LPUART1   /*!< Device name: LPUART1 */
#define BOARD_DEBUG_UART_TX_SIGNAL                                            TX   /*!< LPUART1 signal: TX */
#define BOARD_DEBUG_UART_TX_PIN_NAME                                  LPUART1_TX   /*!< Pin name */
#define BOARD_DEBUG_UART_TX_LABEL                              "UART1_TX_TGTMCU"   /*!< Label */
#define BOARD_DEBUG_UART_TX_NAME                                 "DEBUG_UART_TX"   /*!< Identifier name */


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
