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


/*! @name PORTA4 (number 38), BUTTON2
  @{ */
#define BOARD_SW2_PERIPHERAL TSI0   /*!<@brief Device name: TSI0 */
#define BOARD_SW2_SIGNAL CH         /*!<@brief TSI0 signal: CH */
#define BOARD_SW2_PORT PORTA        /*!<@brief PORT device name: PORTA */
#define BOARD_SW2_PIN 4U            /*!<@brief PORTA pin index: 4 */
#define BOARD_SW2_CHANNEL 5         /*!<@brief TSI0 channel: 5 */
#define BOARD_SW2_PIN_NAME TSI0_CH5 /*!<@brief Pin name */
#define BOARD_SW2_LABEL "BUTTON2"   /*!<@brief Label */
#define BOARD_SW2_NAME "SW2"        /*!<@brief Identifier name */
                                    /* @} */

/*! @name PORTC4 (number 76), LEDRGB_GREEN
  @{ */
#define BOARD_LED_GREEN_PERIPHERAL TPM0      /*!<@brief Device name: TPM0 */
#define BOARD_LED_GREEN_SIGNAL CH            /*!<@brief TPM0 signal: CH */
#define BOARD_LED_GREEN_PORT PORTC           /*!<@brief PORT device name: PORTC */
#define BOARD_LED_GREEN_PIN 4U               /*!<@brief PORTC pin index: 4 */
#define BOARD_LED_GREEN_CHANNEL 3            /*!<@brief TPM0 channel: 3 */
#define BOARD_LED_GREEN_PIN_NAME TPM0_CH3    /*!<@brief Pin name */
#define BOARD_LED_GREEN_LABEL "LEDRGB_GREEN" /*!<@brief Label */
#define BOARD_LED_GREEN_NAME "LED_GREEN"     /*!<@brief Identifier name */
                                             /* @} */

/*! @name PORTE29 (number 26), LEDRGB_RED
  @{ */
#define BOARD_LED_RED_PERIPHERAL TPM0    /*!<@brief Device name: TPM0 */
#define BOARD_LED_RED_SIGNAL CH          /*!<@brief TPM0 signal: CH */
#define BOARD_LED_RED_PORT PORTE         /*!<@brief PORT device name: PORTE */
#define BOARD_LED_RED_PIN 29U            /*!<@brief PORTE pin index: 29 */
#define BOARD_LED_RED_CHANNEL 2          /*!<@brief TPM0 channel: 2 */
#define BOARD_LED_RED_PIN_NAME TPM0_CH2  /*!<@brief Pin name */
#define BOARD_LED_RED_LABEL "LEDRGB_RED" /*!<@brief Label */
#define BOARD_LED_RED_NAME "LED_RED"     /*!<@brief Identifier name */
                                         /* @} */

/*! @name PORTB16 (number 62), J1[2]/U7[25]/D0/UART0_RX/FXIO_D16/UART1_RX_TGTMCU
  @{ */
/*!
 * @brief Device name: LPUART0 */
#define BOARD_DEBUG_UART_RX_PERIPHERAL LPUART0
/*!
 * @brief LPUART0 signal: RX */
#define BOARD_DEBUG_UART_RX_SIGNAL RX
/*!
 * @brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_RX_PORT PORTB
/*!
 * @brief PORTB pin index: 16 */
#define BOARD_DEBUG_UART_RX_PIN 16U
/*!
 * @brief Pin name */
#define BOARD_DEBUG_UART_RX_PIN_NAME LPUART0_RX
/*!
 * @brief Label */
#define BOARD_DEBUG_UART_RX_LABEL "J1[2]/U7[25]/D0/UART0_RX/FXIO_D16/UART1_RX_TGTMCU"
/*!
 * @brief Identifier name */
#define BOARD_DEBUG_UART_RX_NAME "DEBUG_UART_RX"
/* @} */

/*! @name PORTB17 (number 63), J1[4]/U7[24]/D1/UART0_TX/FXIO_D17/UART1_TX_TGTMCU
  @{ */
/*!
 * @brief Device name: LPUART0 */
#define BOARD_DEBUG_UART_TX_PERIPHERAL LPUART0
/*!
 * @brief LPUART0 signal: TX */
#define BOARD_DEBUG_UART_TX_SIGNAL TX
/*!
 * @brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_TX_PORT PORTB
/*!
 * @brief PORTB pin index: 17 */
#define BOARD_DEBUG_UART_TX_PIN 17U
/*!
 * @brief Pin name */
#define BOARD_DEBUG_UART_TX_PIN_NAME LPUART0_TX
/*!
 * @brief Label */
#define BOARD_DEBUG_UART_TX_LABEL "J1[4]/U7[24]/D1/UART0_TX/FXIO_D17/UART1_TX_TGTMCU"
/*!
 * @brief Identifier name */
#define BOARD_DEBUG_UART_TX_NAME "DEBUG_UART_TX"
/* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

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
