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

#define SOPT5_UART0TXSRC_UART_TX 0x00u /*!<@brief UART 0 transmit data source select: UART0_TX pin */

/*! @name PORTB16 (coord E10), U7[4]/UART0_RX
  @{ */
#define BOARD_DEBUG_UART_RX_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_RX_PIN 16U    /*!<@brief PORTB pin index: 16 */
                                       /* @} */

/*! @name PORTB17 (coord E9), U10[1]/UART0_TX
  @{ */
#define BOARD_DEBUG_UART_TX_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_TX_PIN 17U    /*!<@brief PORTB pin index: 17 */
                                       /* @} */

/*! @name PORTB18 (coord D12), J2[2]/FTM2_CH0
  @{ */
#define BOARD_FTM2_CH0_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_FTM2_CH0_PIN 18U    /*!<@brief PORTB pin index: 18 */
                                  /* @} */

/*! @name PORTB19 (coord D11), J2[4]/FTM2_CH1
  @{ */
#define BOARD_FTM2_CH1_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_FTM2_CH1_PIN 19U    /*!<@brief PORTB pin index: 19 */
                                  /* @} */

/*! @name PORTA2 (coord K6), J9[6]/JTAG_TDO/TRACE_SWO/EZP_DO
  @{ */
#define BOARD_TRACE_SWO_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_TRACE_SWO_PIN 2U     /*!<@brief PORTA pin index: 2 */
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
