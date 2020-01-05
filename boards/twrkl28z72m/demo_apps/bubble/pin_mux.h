/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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


/*! @name PORTA4 (number 38), J24[3]/NMI0/TSI0_CH5/SW3
  @{ */
#define BOARD_SW3_PERIPHERAL TSI0                  /*!<@brief Device name: TSI0 */
#define BOARD_SW3_SIGNAL CH                        /*!<@brief TSI0 signal: CH */
#define BOARD_SW3_PORT PORTA                       /*!<@brief PORT device name: PORTA */
#define BOARD_SW3_PIN 4U                           /*!<@brief PORTA pin index: 4 */
#define BOARD_SW3_CHANNEL 5                        /*!<@brief TSI0 channel: 5 */
#define BOARD_SW3_PIN_NAME TSI0_CH5                /*!<@brief Pin name */
#define BOARD_SW3_LABEL "J24[3]/NMI0/TSI0_CH5/SW3" /*!<@brief Label */
#define BOARD_SW3_NAME "SW3"                       /*!<@brief Identifier name */
                                                   /* @} */

/*! @name PORTA1 (number 35), U11[3]/UART0_RX
  @{ */
#define BOARD_DEBUG_UART_RX_PERIPHERAL LPUART0      /*!<@brief Device name: LPUART0 */
#define BOARD_DEBUG_UART_RX_SIGNAL RX               /*!<@brief LPUART0 signal: RX */
#define BOARD_DEBUG_UART_RX_PORT PORTA              /*!<@brief PORT device name: PORTA */
#define BOARD_DEBUG_UART_RX_PIN 1U                  /*!<@brief PORTA pin index: 1 */
#define BOARD_DEBUG_UART_RX_PIN_NAME LPUART0_RX     /*!<@brief Pin name */
#define BOARD_DEBUG_UART_RX_LABEL "U11[3]/UART0_RX" /*!<@brief Label */
#define BOARD_DEBUG_UART_RX_NAME "DEBUG_UART_RX"    /*!<@brief Identifier name */
                                                    /* @} */

/*! @name PORTA2 (number 36), U12[4]/UART0_TX
  @{ */
#define BOARD_DEBUG_UART_TX_PERIPHERAL LPUART0      /*!<@brief Device name: LPUART0 */
#define BOARD_DEBUG_UART_TX_SIGNAL TX               /*!<@brief LPUART0 signal: TX */
#define BOARD_DEBUG_UART_TX_PORT PORTA              /*!<@brief PORT device name: PORTA */
#define BOARD_DEBUG_UART_TX_PIN 2U                  /*!<@brief PORTA pin index: 2 */
#define BOARD_DEBUG_UART_TX_PIN_NAME LPUART0_TX     /*!<@brief Pin name */
#define BOARD_DEBUG_UART_TX_LABEL "U12[4]/UART0_TX" /*!<@brief Label */
#define BOARD_DEBUG_UART_TX_NAME "DEBUG_UART_TX"    /*!<@brief Identifier name */
                                                    /* @} */

/*! @name PORTC6 (number 78), J17[1]/J31[B57]/U15[9]/INT2
  @{ */
#define BOARD_ACCEL_I2C_INT2_PERIPHERAL CMP0                     /*!<@brief Device name: CMP0 */
#define BOARD_ACCEL_I2C_INT2_SIGNAL IN                           /*!<@brief CMP0 signal: IN */
#define BOARD_ACCEL_I2C_INT2_PORT PORTC                          /*!<@brief PORT device name: PORTC */
#define BOARD_ACCEL_I2C_INT2_PIN 6U                              /*!<@brief PORTC pin index: 6 */
#define BOARD_ACCEL_I2C_INT2_CHANNEL 0                           /*!<@brief CMP0 IN channel: 0 */
#define BOARD_ACCEL_I2C_INT2_PIN_NAME CMP0_IN0                   /*!<@brief Pin name */
#define BOARD_ACCEL_I2C_INT2_LABEL "J17[1]/J31[B57]/U15[9]/INT2" /*!<@brief Label */
#define BOARD_ACCEL_I2C_INT2_NAME "ACCEL_I2C_INT2"               /*!<@brief Identifier name */
                                                                 /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*! @name PORTC11 (number 83), J15[1]/U15[6]/I2C1_SDA
  @{ */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SDA_PERIPHERAL LPI2C1              /*!<@brief Device name: LPI2C1 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SDA_SIGNAL SDA                     /*!<@brief LPI2C1 signal: SDA */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SDA_PORT PORTC                     /*!<@brief PORT device name: PORTC */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SDA_PIN 11U                        /*!<@brief PORTC pin index: 11 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SDA_PIN_NAME LPI2C1_SDA            /*!<@brief Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SDA_LABEL "J15[1]/U15[6]/I2C1_SDA" /*!<@brief Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SDA_NAME "ACCEL_I2C_SDA"           /*!<@brief Identifier name */
                                                                             /* @} */

/*! @name PORTC10 (number 82), J14[1]/J26[24]/U15[4]/I2C1_SCL/FXIO0_D23
  @{ */
/*!
 * @brief Device name: LPI2C1 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SCL_PERIPHERAL LPI2C1
/*!
 * @brief LPI2C1 signal: SCL */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SCL_SIGNAL SCL
/*!
 * @brief PORT device name: PORTC */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SCL_PORT PORTC
/*!
 * @brief PORTC pin index: 10 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SCL_PIN 10U
/*!
 * @brief Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SCL_PIN_NAME LPI2C1_SCL
/*!
 * @brief Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SCL_LABEL "J14[1]/J26[24]/U15[4]/I2C1_SCL/FXIO0_D23"
/*!
 * @brief Identifier name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_I2C_SCL_NAME "ACCEL_I2C_SCL"
/* @} */

/*!
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
