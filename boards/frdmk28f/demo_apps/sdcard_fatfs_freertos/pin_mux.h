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

#define SOPT5_LPUART0RXSRC_LPUART_RX 0x00u /*!<@brief LPUART0 receive data source select: LPUART0_RX pin */
#define SOPT5_LPUART0TXSRC_LPUART_TX 0x00u /*!<@brief LPUART0 transmit data source select: LPUART0_TX pin */

/*! @name PORTA28 (coord H12), J19[P2]/SDHC0_D3
  @{ */
#define BOARD_SDHC0_D3_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SDHC0_D3_PIN 28U    /*!<@brief PORTA pin index: 28 */
                                  /* @} */

/*! @name PORTA29 (coord H11), J19[P1]/SDHC0_D2
  @{ */
#define BOARD_SDHC0_D2_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SDHC0_D2_PIN 29U    /*!<@brief PORTA pin index: 29 */
                                  /* @} */

/*! @name PORTA24 (coord K11), J19[P8]/SDHC0_D1
  @{ */
#define BOARD_SDHC0_D1_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SDHC0_D1_PIN 24U    /*!<@brief PORTA pin index: 24 */
                                  /* @} */

/*! @name PORTA25 (coord J11), J19[P7]/SDHC0_D0
  @{ */
#define BOARD_SDHC0_D0_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SDHC0_D0_PIN 25U    /*!<@brief PORTA pin index: 25 */
                                  /* @} */

/*! @name PORTA27 (coord H13), J19[P3]/SDHC0_CMD
  @{ */
#define BOARD_SDHC0_CMD_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SDHC0_CMD_PIN 27U    /*!<@brief PORTA pin index: 27 */
                                   /* @} */

/*! @name PORTA26 (coord J10), J19[P5]/SDHC0_DCLK
  @{ */
#define BOARD_SDHC0_DCLK_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SDHC0_DCLK_PIN 26U    /*!<@brief PORTA pin index: 26 */
                                    /* @} */

/*! @name PORTC25 (coord A7), LPUART0_RX_TGTMCU
  @{ */
#define BOARD_DEBUG_UART_RX_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_DEBUG_UART_RX_PIN 25U    /*!<@brief PORTC pin index: 25 */
                                       /* @} */

/*! @name PORTC24 (coord B7), LPUART0_TX_TGTMCU
  @{ */
#define BOARD_DEBUG_UART_TX_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_DEBUG_UART_TX_PIN 24U    /*!<@brief PORTC pin index: 24 */
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
