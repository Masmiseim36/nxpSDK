/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
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
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

#define SOPT5_UART0TXSRC_UART_TX 0x00u /*!<@brief UART 0 transmit data source select: UART0_TX pin */

/*! @name PORTB1 (number 54), U13[11]/RMII0_MDC
  @{ */
#define BOARD_RMII0_MDC_PERIPHERAL ENET           /*!<@brief Device name: ENET */
#define BOARD_RMII0_MDC_SIGNAL RMII_MDC           /*!<@brief ENET signal: RMII_MDC */
#define BOARD_RMII0_MDC_PORT PORTB                /*!<@brief PORT device name: PORTB */
#define BOARD_RMII0_MDC_PIN 1U                    /*!<@brief PORTB pin index: 1 */
#define BOARD_RMII0_MDC_PIN_NAME RMII0_MDC        /*!<@brief Pin name */
#define BOARD_RMII0_MDC_LABEL "U13[11]/RMII0_MDC" /*!<@brief Label */
#define BOARD_RMII0_MDC_NAME "RMII0_MDC"          /*!<@brief Identifier name */
                                                  /* @} */

/*! @name PORTB0 (number 53), U13[10]/RMII0_MDIO
  @{ */
#define BOARD_RMII0_MDIO_PERIPHERAL ENET            /*!<@brief Device name: ENET */
#define BOARD_RMII0_MDIO_SIGNAL RMII_MDIO           /*!<@brief ENET signal: RMII_MDIO */
#define BOARD_RMII0_MDIO_PORT PORTB                 /*!<@brief PORT device name: PORTB */
#define BOARD_RMII0_MDIO_PIN 0U                     /*!<@brief PORTB pin index: 0 */
#define BOARD_RMII0_MDIO_PIN_NAME RMII0_MDIO        /*!<@brief Pin name */
#define BOARD_RMII0_MDIO_LABEL "U13[10]/RMII0_MDIO" /*!<@brief Label */
#define BOARD_RMII0_MDIO_NAME "RMII0_MDIO"          /*!<@brief Identifier name */
                                                    /* @} */

/*! @name PORTA13 (number 43), U13[13]/RMII0_RXD_0
  @{ */
#define BOARD_RMII0_RXD0_PERIPHERAL ENET             /*!<@brief Device name: ENET */
#define BOARD_RMII0_RXD0_SIGNAL RMII_RXD0            /*!<@brief ENET signal: RMII_RXD0 */
#define BOARD_RMII0_RXD0_PORT PORTA                  /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_RXD0_PIN 13U                     /*!<@brief PORTA pin index: 13 */
#define BOARD_RMII0_RXD0_PIN_NAME RMII0_RXD0         /*!<@brief Pin name */
#define BOARD_RMII0_RXD0_LABEL "U13[13]/RMII0_RXD_0" /*!<@brief Label */
#define BOARD_RMII0_RXD0_NAME "RMII0_RXD0"           /*!<@brief Identifier name */
                                                     /* @} */

/*! @name PORTA12 (number 42), U13[12]/RMII0_RXD_1
  @{ */
#define BOARD_RMII0_RXD1_PERIPHERAL ENET             /*!<@brief Device name: ENET */
#define BOARD_RMII0_RXD1_SIGNAL RMII_RXD1            /*!<@brief ENET signal: RMII_RXD1 */
#define BOARD_RMII0_RXD1_PORT PORTA                  /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_RXD1_PIN 12U                     /*!<@brief PORTA pin index: 12 */
#define BOARD_RMII0_RXD1_PIN_NAME RMII0_RXD1         /*!<@brief Pin name */
#define BOARD_RMII0_RXD1_LABEL "U13[12]/RMII0_RXD_1" /*!<@brief Label */
#define BOARD_RMII0_RXD1_NAME "RMII0_RXD1"           /*!<@brief Identifier name */
                                                     /* @} */

/*! @name PORTA5 (number 39), U13[17]/RMII0_RXER
  @{ */
#define BOARD_RMII0_RXER_PERIPHERAL ENET            /*!<@brief Device name: ENET */
#define BOARD_RMII0_RXER_SIGNAL RMII_RXER           /*!<@brief ENET signal: RMII_RXER */
#define BOARD_RMII0_RXER_PORT PORTA                 /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_RXER_PIN 5U                     /*!<@brief PORTA pin index: 5 */
#define BOARD_RMII0_RXER_PIN_NAME RMII0_RXER        /*!<@brief Pin name */
#define BOARD_RMII0_RXER_LABEL "U13[17]/RMII0_RXER" /*!<@brief Label */
#define BOARD_RMII0_RXER_NAME "RMII0_RXER"          /*!<@brief Identifier name */
                                                    /* @} */

/*! @name PORTA16 (number 46), U13[20]/RMII0_TXD0
  @{ */
#define BOARD_RMII0_TXD0_PERIPHERAL ENET            /*!<@brief Device name: ENET */
#define BOARD_RMII0_TXD0_SIGNAL RMII_TXD0           /*!<@brief ENET signal: RMII_TXD0 */
#define BOARD_RMII0_TXD0_PORT PORTA                 /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_TXD0_PIN 16U                    /*!<@brief PORTA pin index: 16 */
#define BOARD_RMII0_TXD0_PIN_NAME RMII0_TXD0        /*!<@brief Pin name */
#define BOARD_RMII0_TXD0_LABEL "U13[20]/RMII0_TXD0" /*!<@brief Label */
#define BOARD_RMII0_TXD0_NAME "RMII0_TXD0"          /*!<@brief Identifier name */
                                                    /* @} */

/*! @name PORTA17 (number 47), U13[21]/RMII0_TXD1
  @{ */
#define BOARD_RMII0_TXD1_PERIPHERAL ENET            /*!<@brief Device name: ENET */
#define BOARD_RMII0_TXD1_SIGNAL RMII_TXD1           /*!<@brief ENET signal: RMII_TXD1 */
#define BOARD_RMII0_TXD1_PORT PORTA                 /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_TXD1_PIN 17U                    /*!<@brief PORTA pin index: 17 */
#define BOARD_RMII0_TXD1_PIN_NAME RMII0_TXD1        /*!<@brief Pin name */
#define BOARD_RMII0_TXD1_LABEL "U13[21]/RMII0_TXD1" /*!<@brief Label */
#define BOARD_RMII0_TXD1_NAME "RMII0_TXD1"          /*!<@brief Identifier name */
                                                    /* @} */

/*! @name PORTA15 (number 45), U13[19]/RMII0_TXEN
  @{ */
#define BOARD_RMII0_TXEN_PERIPHERAL ENET            /*!<@brief Device name: ENET */
#define BOARD_RMII0_TXEN_SIGNAL RMII_TXEN           /*!<@brief ENET signal: RMII_TXEN */
#define BOARD_RMII0_TXEN_PORT PORTA                 /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_TXEN_PIN 15U                    /*!<@brief PORTA pin index: 15 */
#define BOARD_RMII0_TXEN_PIN_NAME RMII0_TXEN        /*!<@brief Pin name */
#define BOARD_RMII0_TXEN_LABEL "U13[19]/RMII0_TXEN" /*!<@brief Label */
#define BOARD_RMII0_TXEN_NAME "RMII0_TXEN"          /*!<@brief Identifier name */
                                                    /* @} */

/*! @name PORTA14 (number 44), U13[15]/RMII0_CRS_DV
  @{ */
#define BOARD_RMII0_CRS_DV_PERIPHERAL ENET              /*!<@brief Device name: ENET */
#define BOARD_RMII0_CRS_DV_SIGNAL RMII_CRS_DV           /*!<@brief ENET signal: RMII_CRS_DV */
#define BOARD_RMII0_CRS_DV_PORT PORTA                   /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_CRS_DV_PIN 14U                      /*!<@brief PORTA pin index: 14 */
#define BOARD_RMII0_CRS_DV_PIN_NAME RMII0_CRS_DV        /*!<@brief Pin name */
#define BOARD_RMII0_CRS_DV_LABEL "U13[15]/RMII0_CRS_DV" /*!<@brief Label */
#define BOARD_RMII0_CRS_DV_NAME "RMII0_CRS_DV"          /*!<@brief Identifier name */
                                                        /* @} */

/*! @name PORTC16 (number 90), J1[2]
  @{ */
#define BOARD_TMR_1588_0_PERIPHERAL ENET          /*!<@brief Device name: ENET */
#define BOARD_TMR_1588_0_SIGNAL TMR_1588          /*!<@brief ENET signal: TMR_1588 */
#define BOARD_TMR_1588_0_PORT PORTC               /*!<@brief PORT device name: PORTC */
#define BOARD_TMR_1588_0_PIN 16U                  /*!<@brief PORTC pin index: 16 */
#define BOARD_TMR_1588_0_CHANNEL 0                /*!<@brief ENET TMR_1588 channel: 0 */
#define BOARD_TMR_1588_0_PIN_NAME ENET0_1588_TMR0 /*!<@brief Pin name */
#define BOARD_TMR_1588_0_LABEL "J1[2]"            /*!<@brief Label */
#define BOARD_TMR_1588_0_NAME "TMR_1588_0"        /*!<@brief Identifier name */
                                                  /* @} */

/*! @name PORTC17 (number 91), J1[4]
  @{ */
#define BOARD_TMR_1588_1_PERIPHERAL ENET          /*!<@brief Device name: ENET */
#define BOARD_TMR_1588_1_SIGNAL TMR_1588          /*!<@brief ENET signal: TMR_1588 */
#define BOARD_TMR_1588_1_PORT PORTC               /*!<@brief PORT device name: PORTC */
#define BOARD_TMR_1588_1_PIN 17U                  /*!<@brief PORTC pin index: 17 */
#define BOARD_TMR_1588_1_CHANNEL 1                /*!<@brief ENET TMR_1588 channel: 1 */
#define BOARD_TMR_1588_1_PIN_NAME ENET0_1588_TMR1 /*!<@brief Pin name */
#define BOARD_TMR_1588_1_LABEL "J1[4]"            /*!<@brief Label */
#define BOARD_TMR_1588_1_NAME "TMR_1588_1"        /*!<@brief Identifier name */
                                                  /* @} */

/*! @name PORTB16 (number 62), U7[4]/UART0_RX
  @{ */
#define BOARD_DEBUG_UART_RX_PERIPHERAL UART0       /*!<@brief Device name: UART0 */
#define BOARD_DEBUG_UART_RX_SIGNAL RX              /*!<@brief UART0 signal: RX */
#define BOARD_DEBUG_UART_RX_PORT PORTB             /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_RX_PIN 16U                /*!<@brief PORTB pin index: 16 */
#define BOARD_DEBUG_UART_RX_PIN_NAME UART0_RX      /*!<@brief Pin name */
#define BOARD_DEBUG_UART_RX_LABEL "U7[4]/UART0_RX" /*!<@brief Label */
#define BOARD_DEBUG_UART_RX_NAME "DEBUG_UART_RX"   /*!<@brief Identifier name */
                                                   /* @} */

/*! @name PORTB17 (number 63), U10[1]/UART0_TX
  @{ */
#define BOARD_DEBUG_UART_TX_PERIPHERAL UART0        /*!<@brief Device name: UART0 */
#define BOARD_DEBUG_UART_TX_SIGNAL TX               /*!<@brief UART0 signal: TX */
#define BOARD_DEBUG_UART_TX_PORT PORTB              /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_TX_PIN 17U                 /*!<@brief PORTB pin index: 17 */
#define BOARD_DEBUG_UART_TX_PIN_NAME UART0_TX       /*!<@brief Pin name */
#define BOARD_DEBUG_UART_TX_LABEL "U10[1]/UART0_TX" /*!<@brief Label */
#define BOARD_DEBUG_UART_TX_NAME "DEBUG_UART_TX"    /*!<@brief Identifier name */
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
