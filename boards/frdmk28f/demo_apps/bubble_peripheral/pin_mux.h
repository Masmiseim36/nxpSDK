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
 * @brief LPUART0 receive data source select: LPUART0_RX pin */
#define SOPT5_LPUART0RXSRC_LPUART_RX 0x00u
/*!
 * @brief LPUART0 transmit data source select: LPUART0_TX pin */
#define SOPT5_LPUART0TXSRC_LPUART_TX 0x00u
/*!
 * @brief FTM3 channel 1 output source: FTM3_CH1 pin is output of FTM3 channel 1 output */
#define SOPT8_FTM3OCH1SRC_FTM 0x00u
/*!
 * @brief FTM3 channel 2 output source: FTM3_CH2 pin is output of FTM3 channel 2 output */
#define SOPT8_FTM3OCH2SRC_FTM 0x00u

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

/*! @name PORTE6 (coord E2), J1[7]/D1[1]/LEDRGB_RED
  @{ */
#define BOARD_LED_RED_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_LED_RED_PIN 6U     /*!<@brief PORTE pin index: 6 */
                                 /* @} */

/*! @name PORTE7 (coord E3), J1[15]/D1[4]/LEDRGB_GREEN
  @{ */
#define BOARD_LED_GREEN_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_LED_GREEN_PIN 7U     /*!<@brief PORTE pin index: 7 */
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

/*! @name PORTC28 (coord C6), U6[6]/I2C SDA
  @{ */
#define BOARD_ACCEL_I2C_SDA_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_ACCEL_I2C_SDA_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_ACCEL_I2C_SDA_PIN 28U    /*!<@brief PORTC pin index: 28 */
                                       /* @} */

/*! @name PORTC29 (coord B6), U6[4]/I2C_SCL
  @{ */
#define BOARD_ACCEL_I2C_SCL_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_ACCEL_I2C_SCL_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_ACCEL_I2C_SCL_PIN 29U    /*!<@brief PORTC pin index: 29 */
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
