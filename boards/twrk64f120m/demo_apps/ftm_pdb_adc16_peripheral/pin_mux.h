/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

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

#define SOPT5_UART1TXSRC_UART_TX 0x00u /*!<@brief UART 1 transmit data source select: UART1_TX pin */

/*! @name PORTC3 (coord A11), J10[2]/J11[A38]/J11[A64]/UART1_RX
  @{ */
#define BOARD_DEBUG_UART_RX_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_DEBUG_UART_RX_PIN 3U     /*!<@brief PORTC pin index: 3 */
                                       /* @} */

/*! @name PORTC4 (coord A9), J11[A37]/J15[2]/J11[A69]/UART1_TX
  @{ */
#define BOARD_DEBUG_UART_TX_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_DEBUG_UART_TX_PIN 4U     /*!<@brief PORTC pin index: 4 */
                                       /* @} */

/*! @name PORTA6 (coord J7), J8[2]/U6[11]/ACCEL_INT1
  @{ */
#define BOARD_ACCEL_INT1_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_ACCEL_INT1_PIN 6U     /*!<@brief PORTA pin index: 6 */
                                    /* @} */

/*! @name PORTA7 (coord J8), J11[B40]
  @{ */
#define BOARD_FTM0_CH4_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_FTM0_CH4_PIN 7U     /*!<@brief PORTA pin index: 7 */
                                  /* @} */

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

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
