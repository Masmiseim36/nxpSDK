/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */


#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

#include "board.h"

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/* UART4_RXD (coord F19), UART4_RXD */
#define BOARD_INIT_EXAMPLE_UART4_RXD_PERIPHERAL                            UART4   /*!< Device name: UART4 */
#define BOARD_INIT_EXAMPLE_UART4_RXD_SIGNAL                              uart_rx   /*!< UART4 signal: uart_rx */
#define BOARD_INIT_EXAMPLE_UART4_RXD_PIN_NAME                          UART4_RXD   /*!< Pin name */
#define BOARD_INIT_EXAMPLE_UART4_RXD_LABEL                           "UART4_RXD"   /*!< Label */
#define BOARD_INIT_EXAMPLE_UART4_RXD_NAME                            "UART4_RXD"   /*!< Identifier name */

/* UART4_TXD (coord F18), UART4_TXD */
#define BOARD_INIT_EXAMPLE_UART4_TXD_PERIPHERAL                            UART4   /*!< Device name: UART4 */
#define BOARD_INIT_EXAMPLE_UART4_TXD_SIGNAL                              uart_tx   /*!< UART4 signal: uart_tx */
#define BOARD_INIT_EXAMPLE_UART4_TXD_PIN_NAME                          UART4_TXD   /*!< Pin name */
#define BOARD_INIT_EXAMPLE_UART4_TXD_LABEL                           "UART4_TXD"   /*!< Label */
#define BOARD_INIT_EXAMPLE_UART4_TXD_NAME                            "UART4_TXD"   /*!< Identifier name */

/* ECSPI1_SS0 (coord B6), UART3_RTS */
#define BOARD_INIT_EXAMPLE_UART3_RTS_GPIO                                  GPIO5   /*!< GPIO device name: GPIO5 */
#define BOARD_INIT_EXAMPLE_UART3_RTS_GPIO_PIN                                 9U   /*!< GPIO5 pin index: 9 */
#define BOARD_INIT_EXAMPLE_UART3_RTS_PIN_NAME                         ECSPI1_SS0   /*!< Pin name */
#define BOARD_INIT_EXAMPLE_UART3_RTS_LABEL                           "UART3_RTS"   /*!< Label */
#define BOARD_INIT_EXAMPLE_UART3_RTS_NAME                            "UART3_RTS"   /*!< Identifier name */

/* ECSPI2_MOSI (coord B8), ECSPI2_MOSI */
#define BOARD_INIT_EXAMPLE_ECSPI2_MOSI_GPIO                                GPIO5   /*!< GPIO device name: GPIO5 */
#define BOARD_INIT_EXAMPLE_ECSPI2_MOSI_GPIO_PIN                              11U   /*!< GPIO5 pin index: 11 */
#define BOARD_INIT_EXAMPLE_ECSPI2_MOSI_PIN_NAME                      ECSPI2_MOSI   /*!< Pin name */
#define BOARD_INIT_EXAMPLE_ECSPI2_MOSI_LABEL                       "ECSPI2_MOSI"   /*!< Label */
#define BOARD_INIT_EXAMPLE_ECSPI2_MOSI_NAME                        "ECSPI2_MOSI"   /*!< Identifier name */

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


/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

/*!
 * @brief 
 *
 */
void BOARD_Init_Example(void);                             /*!< Function assigned for the core: Cortex-M4[m4] */

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
