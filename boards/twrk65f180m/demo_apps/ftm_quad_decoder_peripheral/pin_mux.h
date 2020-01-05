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

/*! @name PORTB19 (coord D11), J26[B72]/D8[1]/BRD_PAD1
  @{ */
#define BOARD_TSI_ELECTRODE_2_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_TSI_ELECTRODE_2_PIN 19U    /*!<@brief PORTB pin index: 19 */
                                         /* @} */

/*! @name PORTB18 (coord D12), J26[B66]/D9[1]/BRD_PAD2
  @{ */
#define BOARD_TSI_ELECTRODE_1_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_TSI_ELECTRODE_1_PIN 18U    /*!<@brief PORTB pin index: 18 */
                                         /* @} */

/*! @name PORTE16 (coord H3), J33[2]/U5[4]/UART2_TX
  @{ */
#define BOARD_DEBUG_UART_TX_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_DEBUG_UART_TX_PIN 16U    /*!<@brief PORTE pin index: 16 */
                                       /* @} */

/*! @name PORTE17 (coord F5), J34[2]/U6[2]/UART2_RX
  @{ */
#define BOARD_DEBUG_UART_RX_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_DEBUG_UART_RX_PIN 17U    /*!<@brief PORTE pin index: 17 */
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
