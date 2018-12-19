/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

/*! @name PORTE0 (coord E4), UART1_TX_PKYK
  @{ */
#define BOARD_DEBUG_UART_TX_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_DEBUG_UART_TX_PIN 0U     /*!<@brief PORTE pin index: 0 */
                                       /* @} */

/*! @name PORTE1 (coord E3), UART1_RX_PKYK
  @{ */
#define BOARD_DEBUG_UART_RX_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_DEBUG_UART_RX_PIN 1U     /*!<@brief PORTE pin index: 1 */
                                       /* @} */

/*! @name PORTD7 (coord A1), J16[7]/J24[A56]/J32[4]/J34[1]/USB_K22_ID/LED_J_PTD7
  @{ */
#define BOARD_LED_BLUE_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_LED_BLUE_PIN 7U     /*!<@brief PORTD pin index: 7 */
                                  /* @} */

/*! @name PORTD6 (coord B2), J16[5]/J24[A80]/J24[B38]/LED_J_PTD6
  @{ */
#define BOARD_LED_ORANGE_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_LED_ORANGE_PIN 6U     /*!<@brief PORTD pin index: 6 */
                                    /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_I2C_ConfigurePins(void);

/*! @name PORTC10 (coord C6), J5[3]/J24[A75]/J24[B50]/J24[B52/J9[1]/U1[4]/I2C1_SCL
  @{ */
#define BOARD_ACCEL_I2C_SCL_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_ACCEL_I2C_SCL_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_ACCEL_I2C_SCL_PIN 10U    /*!<@brief PORTC pin index: 10 */
                                       /* @} */

/*! @name PORTC11 (coord C5), J5[4]/J7[1]/J24[A60]/J24[B51]/J24[B71]/U1[6]/I2C1_SDA
  @{ */
#define BOARD_ACCEL_I2C_SDA_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_ACCEL_I2C_SDA_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_ACCEL_I2C_SDA_PIN 11U    /*!<@brief PORTC pin index: 11 */
                                       /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_Init_I2C_GPIO_pins(void);

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
