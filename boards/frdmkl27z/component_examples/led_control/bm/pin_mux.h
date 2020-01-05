/*
 * Copyright 2018 NXP
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

#define SOPT5_LPUART0RXSRC_LPUART_RX 0x00u /*!<@brief LPUART0 Receive Data Source Select: LPUART_RX pin */
#define SOPT5_LPUART0TXSRC_LPUART_TX 0x00u /*!<@brief LPUART0 Transmit Data Source Select: LPUART0_TX pin */

/*! @name PORTA1 (number 23), J1[2]/J25[1]/D0-UART0_RX
  @{ */
#define BOARD_DEBUG_UART0_RX_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_DEBUG_UART0_RX_PIN 1U     /*!<@brief PORTA pin index: 1 */
                                        /* @} */

/*! @name PORTA2 (number 24), J1[4]/J26[1]/D1-UART0_TX
  @{ */
#define BOARD_DEBUG_UART0_TX_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_DEBUG_UART0_TX_PIN 2U     /*!<@brief PORTA pin index: 2 */
                                        /* @} */

/*! @name PORTC1 (number 44), J3[13]/SW3
  @{ */
#define BOARD_SW3_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_SW3_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SW3_PIN 1U     /*!<@brief PORTC pin index: 1 */
                             /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*! @name PORTA13 (number 29), J1[10]/D4-TPM1_CH1/D4-LED_BLUE
  @{ */
#define BOARD_LED_BLUE_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define BOARD_LED_BLUE_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_LED_BLUE_PIN 13U    /*!<@brief PORTA pin index: 13 */
                                  /* @} */

/*! @name PORTB18 (number 41), J2[11]/D11[1]/LED_RED
  @{ */
#define BOARD_LED_RED_GPIO GPIOB /*!<@brief GPIO device name: GPIOB */
#define BOARD_LED_RED_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_LED_RED_PIN 18U    /*!<@brief PORTB pin index: 18 */
                                 /* @} */

/*! @name PORTB19 (number 42), J2[13]/D11[4]/LED_GREEN
  @{ */
#define BOARD_LED_GREEN_GPIO GPIOB /*!<@brief GPIO device name: GPIOB */
#define BOARD_LED_GREEN_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_LED_GREEN_PIN 19U    /*!<@brief PORTB pin index: 19 */
                                   /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDs(void);

#define SOPT4_TPM2CH0SRC_TPM2_CH0 0x00u /*!<@brief TPM2 Channel 0 Input Capture Source Select: TPM2_CH0 signal */

/*! @name PORTB18 (number 41), J2[11]/D11[1]/LED_RED
  @{ */
#define BOARD_INITPWM_LED_RED_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_INITPWM_LED_RED_PIN 18U    /*!<@brief PORTB pin index: 18 */
                                         /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPWM(void);

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
