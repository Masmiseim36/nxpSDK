/*
 * Copyright 2019-2020 NXP
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

#define SOPT5_LPUART1RXSRC_0b0 0x00u  /*!<@brief LPUART1 Receive Data Source Select: LPUART1_RX pin */
#define SOPT5_LPUART1TXSRC_0b00 0x00u /*!<@brief LPUART1 Transmit Data Source Select: LPUART1_TX pin */

/*! @name PORTA18 (number 6), LPUART1_TX
  @{ */
#define BOARD_INITPINS_LPUART1_TX_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_INITPINS_LPUART1_TX_PIN 18U    /*!<@brief PORTA pin index: 18 */
                                             /* @} */

/*! @name PORTA17 (number 5), LPUART1_RX
  @{ */
#define BOARD_INITPINS_LPUART1_RX_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_INITPINS_LPUART1_RX_PIN 17U    /*!<@brief PORTA pin index: 17 */
                                             /* @} */
#define SOPT5_LPUART0RXSRC_0b0 0x00u  /*!<@brief LPUART0 Receive Data Source Select: LPUART_RX pin */
#define SOPT5_LPUART0TXSRC_0b00 0x00u /*!<@brief LPUART0 Transmit Data Source Select: LPUART0_TX pin */

/*! @name PORTC6 (number 42), LPUART0_RX
  @{ */
#define BOARD_INITLPUART_LPUART0_RX_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_INITLPUART_LPUART0_RX_PIN 6U     /*!<@brief PORTC pin index: 6 */
                                               /* @} */

/*! @name PORTC7 (number 43), LPUART0_TX
  @{ */
#define BOARD_INITLPUART_LPUART0_TX_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_INITLPUART_LPUART0_TX_PIN 7U     /*!<@brief PORTC pin index: 7 */
                                               /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLPUART(void);

/*! @name PORTB3 (number 19), LED
  @{ */
#define BOARD_INITLEDS_LED_GPIO GPIOB /*!<@brief GPIO device name: GPIOB */
#define BOARD_INITLEDS_LED_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_INITLEDS_LED_PIN 3U     /*!<@brief PORTB pin index: 3 */
                                      /* @} */

/*! @name PORTC1 (number 37), LED_R
  @{ */
#define BOARD_INITLEDS_LED_R_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_INITLEDS_LED_R_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_INITLEDS_LED_R_PIN 1U     /*!<@brief PORTC pin index: 1 */
                                        /* @} */

/*! @name PORTB2 (number 18), LED_B
  @{ */
#define BOARD_INITLEDS_LED_B_GPIO GPIOB /*!<@brief GPIO device name: GPIOB */
#define BOARD_INITLEDS_LED_B_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_INITLEDS_LED_B_PIN 2U     /*!<@brief PORTB pin index: 2 */
                                        /* @} */

/*! @name PORTA16 (number 4), LED_G
  @{ */
#define BOARD_INITLEDS_LED_G_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define BOARD_INITLEDS_LED_G_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_INITLEDS_LED_G_PIN 16U    /*!<@brief PORTA pin index: 16 */
                                        /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDs(void);

/*! @name PORTB18 (number 23), SW2
  @{ */
#define BOARD_INITBUTTONS_SW2_GPIO GPIOB /*!<@brief GPIO device name: GPIOB */
#define BOARD_INITBUTTONS_SW2_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_INITBUTTONS_SW2_PIN 18U    /*!<@brief PORTB pin index: 18 */
                                         /* @} */

/*! @name PORTC2 (number 38), SW3
  @{ */
#define BOARD_INITBUTTONS_SW3_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_INITBUTTONS_SW3_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_INITBUTTONS_SW3_PIN 2U     /*!<@brief PORTC pin index: 2 */
                                         /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitButtons(void);

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
