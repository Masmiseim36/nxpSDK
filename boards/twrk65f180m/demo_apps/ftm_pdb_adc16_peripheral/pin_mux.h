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

/*! @name PORTE17 (coord F5), J34[2]/U6[2]/UART2_RX
  @{ */
#define BOARD_DEBUG_UART_RX_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_DEBUG_UART_RX_PIN 17U    /*!<@brief PORTE pin index: 17 */
                                       /* @} */

/*! @name PORTE16 (coord H3), J33[2]/U5[4]/UART2_TX
  @{ */
#define BOARD_DEBUG_UART_TX_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_DEBUG_UART_TX_PIN 16U    /*!<@brief PORTE pin index: 16 */
                                       /* @} */

/*! @name PORTC1 (coord B12), J26[A67]/U14[22]/FB_AD13/SDRAM_A21
  @{ */
#define BOARD_SDRAM_A21_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_A21_PIN 1U     /*!<@brief PORTC pin index: 1 */
                                   /* @} */

/*! @name PORTC2 (coord A13), J26[A68]/U14[24]/FB_AD12/SDRAM_A20
  @{ */
#define BOARD_SDRAM_A20_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_A20_PIN 2U     /*!<@brief PORTC pin index: 2 */
                                   /* @} */

/*! @name PORTA6 (coord M10), J26[B27]/U14[68]/CLKOUT
  @{ */
#define BOARD_SDRAM_CLKOUT_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SDRAM_CLKOUT_PIN 6U     /*!<@brief PORTA pin index: 6 */
                                      /* @} */

/*! @name PORTA7 (coord L10), J26[A14]/RMII0_MDIO
  @{ */
#define BOARD_RMII0_MDIO_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_MDIO_PIN 7U     /*!<@brief PORTA pin index: 7 */
                                    /* @} */

/*! @name PORTD5 (coord C4), J26[A79]/U14[63]/FB_AD1/SDRAM_A9
  @{ */
#define BOARD_SDRAM_A9_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_SDRAM_A9_PIN 5U     /*!<@brief PORTD pin index: 5 */
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
