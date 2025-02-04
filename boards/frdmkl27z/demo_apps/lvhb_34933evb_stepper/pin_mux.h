/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

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

/*! @name PORTE24 (number 20), J1[12]/D5-TPM0_CH0
  @{ */
#define BOARD_LVHB_IN1A_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_LVHB_IN1A_PIN 24U    /*!<@brief PORTE pin index: 24 */
                                   /* @} */

/*! @name PORTC9 (number 54), J1[14]/D6-TPM0_CH5/CMP0_IN3
  @{ */
#define BOARD_LVHB_IN1B_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_LVHB_IN1B_PIN 9U     /*!<@brief PORTC pin index: 9 */
                                   /* @} */

/*! @name PORTE31 (number 19), J2[2]/D8-TPM0_CH4
  @{ */
#define BOARD_LVHB_IN2A_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_LVHB_IN2A_PIN 31U    /*!<@brief PORTE pin index: 31 */
                                   /* @} */

/*! @name PORTA5 (number 27), J2[4]/D9-TPM0_CH2
  @{ */
#define BOARD_LVHB_IN2B_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_LVHB_IN2B_PIN 5U     /*!<@brief PORTA pin index: 5 */
                                   /* @} */

/*! @name PORTA2 (number 24), J1[4]/J26[1]/D1-UART0_TX
  @{ */
#define BOARD_LVHB_EN_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define BOARD_LVHB_EN_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_LVHB_EN_PIN 2U     /*!<@brief PORTA pin index: 2 */
                                 /* @} */

/*! @name PORTB18 (number 41), J2[11]/D11[1]/LED_RED
  @{ */
#define BOARD_LED_RED_GPIO GPIOB /*!<@brief GPIO device name: GPIOB */
#define BOARD_LED_RED_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_LED_RED_PIN 18U    /*!<@brief PORTB pin index: 18 */
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
