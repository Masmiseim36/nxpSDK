/*
 * Copyright 2021 NXP
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
  kPIN_MUX_DirectionInput = 0U,         /* Input direction */
  kPIN_MUX_DirectionOutput = 1U,        /* Output direction */
  kPIN_MUX_DirectionInputOrOutput = 2U  /* Input or output direction */
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

/* PORTI6 (number 6), UART2_RX */
#define BOARD_DEBUG_UART_RX_PERIPHERAL                                     UART2   /*!< Device name: UART2 */
#define BOARD_DEBUG_UART_RX_SIGNAL                                            RX   /*!< UART2 signal: RX */
#define BOARD_DEBUG_UART_RX_PIN_NAME                                    UART2_RX   /*!< Pin name */
#define BOARD_DEBUG_UART_RX_LABEL                                     "UART2_RX"   /*!< Label */
#define BOARD_DEBUG_UART_RX_NAME                                 "DEBUG_UART_RX"   /*!< Identifier name */

/* PORTI7 (number 7), UART2_TX */
#define BOARD_DEBUG_UART_TX_PERIPHERAL                                     UART2   /*!< Device name: UART2 */
#define BOARD_DEBUG_UART_TX_SIGNAL                                            TX   /*!< UART2 signal: TX */
#define BOARD_DEBUG_UART_TX_PIN_NAME                                    UART2_TX   /*!< Pin name */
#define BOARD_DEBUG_UART_TX_LABEL                                     "UART2_TX"   /*!< Label */
#define BOARD_DEBUG_UART_TX_NAME                                 "DEBUG_UART_TX"   /*!< Identifier name */

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

/*!
 * @brief
 * UART2 IrDA Select: Pad RX input PTI[6] or PTE[6] selected for RX input of UART2 and UART2 TX signal is not used
 * for modulation.
 */
#define MISC_CTL_UART2IRSEL_NONE 0x00u

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/* PORTD7 (number 79), J32[A7]/J10[1]/U5[4]/I2C0_SCL */
#define I2C0_INITPINS_ACCEL_SCL_PERIPHERAL                                  I2C0   /*!< Device name: I2C0 */
#define I2C0_INITPINS_ACCEL_SCL_SIGNAL                                       SCL   /*!< I2C0 signal: SCL */
#define I2C0_INITPINS_ACCEL_SCL_PIN_NAME                                I2C0_SCL   /*!< Pin name */
#define I2C0_INITPINS_ACCEL_SCL_LABEL            "J32[A7]/J10[1]/U5[4]/I2C0_SCL"   /*!< Label */
#define I2C0_INITPINS_ACCEL_SCL_NAME                                 "ACCEL_SCL"   /*!< Identifier name */

/* PORTE0 (number 80), J32[A8]/J11[1]/U5[6]/I2C0_SDA */
#define I2C0_INITPINS_ACCEL_SDA_PERIPHERAL                                  I2C0   /*!< Device name: I2C0 */
#define I2C0_INITPINS_ACCEL_SDA_SIGNAL                                       SDA   /*!< I2C0 signal: SDA */
#define I2C0_INITPINS_ACCEL_SDA_PIN_NAME                                I2C0_SDA   /*!< Pin name */
#define I2C0_INITPINS_ACCEL_SDA_LABEL            "J32[A8]/J11[1]/U5[6]/I2C0_SDA"   /*!< Label */
#define I2C0_INITPINS_ACCEL_SDA_NAME                                 "ACCEL_SDA"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void I2C0_InitPins(void);

/* PORTD7 (number 79), J32[A7]/J10[1]/U5[4]/I2C0_SCL */
#define I2C0_DEINITPINS_ACCEL_SCL_PERIPHERAL                                CMP0   /*!< Device name: CMP0 */
#define I2C0_DEINITPINS_ACCEL_SCL_SIGNAL                                      IN   /*!< CMP0 signal: IN */
#define I2C0_DEINITPINS_ACCEL_SCL_CHANNEL                                      4   /*!< CMP0 IN channel: 4 */
#define I2C0_DEINITPINS_ACCEL_SCL_PIN_NAME                              CMP0_IN4   /*!< Pin name */
#define I2C0_DEINITPINS_ACCEL_SCL_LABEL          "J32[A7]/J10[1]/U5[4]/I2C0_SCL"   /*!< Label */
#define I2C0_DEINITPINS_ACCEL_SCL_NAME                               "ACCEL_SCL"   /*!< Identifier name */

/* PORTE0 (number 80), J32[A8]/J11[1]/U5[6]/I2C0_SDA */
#define I2C0_DEINITPINS_ACCEL_SDA_PERIPHERAL                                 n/a   /*!< Device name: n/a */
#define I2C0_DEINITPINS_ACCEL_SDA_SIGNAL                                disabled   /*!< n/a signal: disabled */
#define I2C0_DEINITPINS_ACCEL_SDA_PIN_NAME PTE0/I2C0_SDA/XBAR_OUT4/UART3_TX/CLKOUT /*!< Pin name */
#define I2C0_DEINITPINS_ACCEL_SDA_LABEL          "J32[A8]/J11[1]/U5[6]/I2C0_SDA"   /*!< Label */
#define I2C0_DEINITPINS_ACCEL_SDA_NAME                               "ACCEL_SDA"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void I2C0_DeinitPins(void);

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
