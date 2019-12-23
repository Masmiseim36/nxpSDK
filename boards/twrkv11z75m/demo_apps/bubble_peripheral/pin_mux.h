/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/*! @brief Direction type  */
typedef enum _pin_mux_direction
{
    kPIN_MUX_DirectionInput = 0U,        /* Input direction */
    kPIN_MUX_DirectionOutput = 1U,       /* Output direction */
    kPIN_MUX_DirectionInputOrOutput = 2U /* Input or output direction */
} pin_mux_direction_t;

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
 * @brief UART 0 Receive Data Source Select: UART0_RX pin */
#define SOPT5_UART0RXSRC_UART_RX 0x00u
/*!
 * @brief UART 0 Transmit Data Source Select: UART0_TX pin */
#define SOPT5_UART0TXSRC_UART_TX 0x00u
/*!
 * @brief FTM0 channel 0 output PWM/OCMP mode source select: FTM0CH0 pin is the output of FTM0 channel 0 PWM/OCMP */
#define SOPT8_FTM0OCH0SRC_NO_MODULATION 0x00u
/*!
 * @brief FTM0 channel 1 output PWM/OCMP mode source select: FTM0CH1 pin is the output of FTM0 channel 1 PWM/OCMP */
#define SOPT8_FTM0OCH1SRC_NO_MODULATION 0x00u

/*! @name PORTB16 (number 39), J24[9]/J500[7]/J506[2]/U512[2]/RXD0
  @{ */
#define BOARD_DEBUG_UART_RX_PERIPHERAL UART0                            /*!<@brief Device name: UART0 */
#define BOARD_DEBUG_UART_RX_SIGNAL RX                                   /*!<@brief UART0 signal: RX */
#define BOARD_DEBUG_UART_RX_PORT PORTB                                  /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_RX_PIN 16U                                     /*!<@brief PORTB pin index: 16 */
#define BOARD_DEBUG_UART_RX_PIN_NAME UART0_RX                           /*!<@brief Pin name */
#define BOARD_DEBUG_UART_RX_LABEL "J24[9]/J500[7]/J506[2]/U512[2]/RXD0" /*!<@brief Label */
#define BOARD_DEBUG_UART_RX_NAME "DEBUG_UART_RX"                        /*!<@brief Identifier name */
                                                                        /* @} */

/*! @name PORTB17 (number 40), J24[10]/J500[5]/J505[2]/U513[4]/TXD0
  @{ */
#define BOARD_DEBUG_UART_TX_PERIPHERAL UART0                             /*!<@brief Device name: UART0 */
#define BOARD_DEBUG_UART_TX_SIGNAL TX                                    /*!<@brief UART0 signal: TX */
#define BOARD_DEBUG_UART_TX_PORT PORTB                                   /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_TX_PIN 17U                                      /*!<@brief PORTB pin index: 17 */
#define BOARD_DEBUG_UART_TX_PIN_NAME UART0_TX                            /*!<@brief Pin name */
#define BOARD_DEBUG_UART_TX_LABEL "J24[10]/J500[5]/J505[2]/U513[4]/TXD0" /*!<@brief Label */
#define BOARD_DEBUG_UART_TX_NAME "DEBUG_UART_TX"                         /*!<@brief Identifier name */
                                                                         /* @} */

/*! @name PORTC1 (number 44), J18[14]/J22[A40]/J500[9]/U2[3]/FTM0_CH0/LED0
  @{ */
#define BOARD_LED_GREEN1_PERIPHERAL FTM0                                      /*!<@brief Device name: FTM0 */
#define BOARD_LED_GREEN1_SIGNAL CH                                            /*!<@brief FTM0 signal: CH */
#define BOARD_LED_GREEN1_PORT PORTC                                           /*!<@brief PORT device name: PORTC */
#define BOARD_LED_GREEN1_PIN 1U                                               /*!<@brief PORTC pin index: 1 */
#define BOARD_LED_GREEN1_CHANNEL 0                                            /*!<@brief FTM0 channel: 0 */
#define BOARD_LED_GREEN1_PIN_NAME FTM0_CH0                                    /*!<@brief Pin name */
#define BOARD_LED_GREEN1_LABEL "J18[14]/J22[A40]/J500[9]/U2[3]/FTM0_CH0/LED0" /*!<@brief Label */
#define BOARD_LED_GREEN1_NAME "LED_GREEN1"                                    /*!<@brief Identifier name */
#define BOARD_LED_GREEN1_DIRECTION kPIN_MUX_DirectionOutput                   /*!<@brief Direction */
                                                                              /* @} */

/*! @name PORTC2 (number 45), J18[15]/J22[A39]/J24[15]/J500[11]/U2[4]/FTM0_CH1/LED1
  @{ */
/*!
 * @brief Device name: FTM0 */
#define BOARD_LED_YELLOW1_PERIPHERAL FTM0
/*!
 * @brief FTM0 signal: CH */
#define BOARD_LED_YELLOW1_SIGNAL CH
/*!
 * @brief PORT device name: PORTC */
#define BOARD_LED_YELLOW1_PORT PORTC
/*!
 * @brief PORTC pin index: 2 */
#define BOARD_LED_YELLOW1_PIN 2U
/*!
 * @brief FTM0 channel: 1 */
#define BOARD_LED_YELLOW1_CHANNEL 1
/*!
 * @brief Pin name */
#define BOARD_LED_YELLOW1_PIN_NAME FTM0_CH1
/*!
 * @brief Label */
#define BOARD_LED_YELLOW1_LABEL "J18[15]/J22[A39]/J24[15]/J500[11]/U2[4]/FTM0_CH1/LED1"
/*!
 * @brief Identifier name */
#define BOARD_LED_YELLOW1_NAME "LED_YELLOW1"
/*!
 * @brief Direction */
#define BOARD_LED_YELLOW1_DIRECTION kPIN_MUX_DirectionOutput
/* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*! @name PORTC6 (number 51), J4[2]/J22[A7]/J22[D50]/J24[19]/J500[23]/I2C_SCL_SNSR
  @{ */
/*!
 * @brief Device name: I2C0 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_PERIPHERAL I2C0
/*!
 * @brief I2C0 signal: SCL */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_SIGNAL SCL
/*!
 * @brief PORT device name: PORTC */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_PORT PORTC
/*!
 * @brief PORTC pin index: 6 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_PIN 6U
/*!
 * @brief Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_PIN_NAME I2C0_SCL
/*!
 * @brief Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_LABEL "J4[2]/J22[A7]/J22[D50]/J24[19]/J500[23]/I2C_SCL_SNSR"
/*!
 * @brief Identifier name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_NAME "ACCEL_SCL"
/* @} */

/*! @name PORTC7 (number 52), J11[2]/J22[A8]/J22[D62]/J24[20]/J500[21]/I2C_SDA_SNSR
  @{ */
/*!
 * @brief Device name: I2C0 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_PERIPHERAL I2C0
/*!
 * @brief I2C0 signal: SDA */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_SIGNAL SDA
/*!
 * @brief PORT device name: PORTC */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_PORT PORTC
/*!
 * @brief PORTC pin index: 7 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_PIN 7U
/*!
 * @brief Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_PIN_NAME I2C0_SDA
/*!
 * @brief Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_LABEL "J11[2]/J22[A8]/J22[D62]/J24[20]/J500[21]/I2C_SDA_SNSR"
/*!
 * @brief Identifier name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_NAME "ACCEL_SDA"
/* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_I2C_ConfigurePins(void);

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
