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

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*! @name PORTE4 (number 5), BUTTON1
  @{ */
#define BOARD_SW3_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define BOARD_SW3_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_SW3_PIN 4U     /*!<@brief PORTE pin index: 4 */
                             /* @} */

/*! @name PORTA4 (number 38), BUTTON2
  @{ */
#define BOARD_SW2_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define BOARD_SW2_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SW2_PIN 4U     /*!<@brief PORTA pin index: 4 */
                             /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitBUTTONs(void);

/*! @name PORTE29 (number 26), LEDRGB_RED
  @{ */
#define BOARD_LED_RED_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define BOARD_LED_RED_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_LED_RED_PIN 29U    /*!<@brief PORTE pin index: 29 */
                                 /* @} */

/*! @name PORTE31 (number 28), LEDRGB_BLUE
  @{ */
#define BOARD_LED_BLUE_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define BOARD_LED_BLUE_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_LED_BLUE_PIN 31U    /*!<@brief PORTE pin index: 31 */
                                  /* @} */

/*! @name PORTC4 (number 76), LEDRGB_GREEN
  @{ */
#define BOARD_LED_GREEN_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_LED_GREEN_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_LED_GREEN_PIN 4U     /*!<@brief PORTC pin index: 4 */
                                   /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDs(void);

/*! @name PORTB16 (number 62), J1[2]/U7[25]/D0/UART0_RX/FXIO_D16/UART1_RX_TGTMCU
  @{ */
#define BOARD_DEBUG_UART_RX_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_RX_PIN 16U    /*!<@brief PORTB pin index: 16 */
                                       /* @} */

/*! @name PORTB17 (number 63), J1[4]/U7[24]/D1/UART0_TX/FXIO_D17/UART1_TX_TGTMCU
  @{ */
#define BOARD_DEBUG_UART_TX_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_TX_PIN 17U    /*!<@brief PORTB pin index: 17 */
                                       /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitDEBUG_UART(void);

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
