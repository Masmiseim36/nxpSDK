/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_


/*******************************************************************************
 * Definitions
 ******************************************************************************/

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

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

/* PORTB16 (coord E10), U7[4]/UART0_RX */
#define BOARD_INITPINS_DEBUG_UART_RX_PERIPHERAL                            UART0   /*!< Device name: UART0 */
#define BOARD_INITPINS_DEBUG_UART_RX_SIGNAL                                   RX   /*!< UART0 signal: RX */
#define BOARD_INITPINS_DEBUG_UART_RX_PIN_NAME                           UART0_RX   /*!< Pin name */
#define BOARD_INITPINS_DEBUG_UART_RX_LABEL                      "U7[4]/UART0_RX"   /*!< Label */
#define BOARD_INITPINS_DEBUG_UART_RX_NAME                        "DEBUG_UART_RX"   /*!< Identifier name */

/* PORTB17 (coord E9), U10[1]/UART0_TX */
#define BOARD_INITPINS_DEBUG_UART_TX_PERIPHERAL                            UART0   /*!< Device name: UART0 */
#define BOARD_INITPINS_DEBUG_UART_TX_SIGNAL                                   TX   /*!< UART0 signal: TX */
#define BOARD_INITPINS_DEBUG_UART_TX_PIN_NAME                           UART0_TX   /*!< Pin name */
#define BOARD_INITPINS_DEBUG_UART_TX_LABEL                     "U10[1]/UART0_TX"   /*!< Label */
#define BOARD_INITPINS_DEBUG_UART_TX_NAME                        "DEBUG_UART_TX"   /*!< Identifier name */

/* PORTA12 (coord K9), U13[12]/RMII0_RXD1 */
#define BOARD_INITPINS_RMII0_RXD1_PERIPHERAL                                ENET   /*!< Device name: ENET */
#define BOARD_INITPINS_RMII0_RXD1_SIGNAL                                MII_RXD1   /*!< ENET signal: MII_RXD1 */
#define BOARD_INITPINS_RMII0_RXD1_PIN_NAME                             MII0_RXD1   /*!< Pin name */
#define BOARD_INITPINS_RMII0_RXD1_LABEL                     "U13[12]/RMII0_RXD1"   /*!< Label */
#define BOARD_INITPINS_RMII0_RXD1_NAME                              "RMII0_RXD1"   /*!< Identifier name */

/* PORTA13 (coord J9), U13[13]/RMII0_RXD0 */
#define BOARD_INITPINS_RMII0_RXD0_PERIPHERAL                                ENET   /*!< Device name: ENET */
#define BOARD_INITPINS_RMII0_RXD0_SIGNAL                                MII_RXD0   /*!< ENET signal: MII_RXD0 */
#define BOARD_INITPINS_RMII0_RXD0_PIN_NAME                             MII0_RXD0   /*!< Pin name */
#define BOARD_INITPINS_RMII0_RXD0_LABEL                     "U13[13]/RMII0_RXD0"   /*!< Label */
#define BOARD_INITPINS_RMII0_RXD0_NAME                              "RMII0_RXD0"   /*!< Identifier name */

/* PORTA14 (coord L10), U13[15]/RMII0_CRS_DV */
#define BOARD_INITPINS_RMII0_CRS_DV_PERIPHERAL                              ENET   /*!< Device name: ENET */
#define BOARD_INITPINS_RMII0_CRS_DV_SIGNAL                              MII_RXDV   /*!< ENET signal: MII_RXDV */
#define BOARD_INITPINS_RMII0_CRS_DV_PIN_NAME                           MII0_RXDV   /*!< Pin name */
#define BOARD_INITPINS_RMII0_CRS_DV_LABEL                 "U13[15]/RMII0_CRS_DV"   /*!< Label */
#define BOARD_INITPINS_RMII0_CRS_DV_NAME                          "RMII0_CRS_DV"   /*!< Identifier name */

/* PORTA15 (coord L11), U13[19]/RMII0_TXEN */
#define BOARD_INITPINS_RMII0_TXEN_PERIPHERAL                                ENET   /*!< Device name: ENET */
#define BOARD_INITPINS_RMII0_TXEN_SIGNAL                                MII_TXEN   /*!< ENET signal: MII_TXEN */
#define BOARD_INITPINS_RMII0_TXEN_PIN_NAME                             MII0_TXEN   /*!< Pin name */
#define BOARD_INITPINS_RMII0_TXEN_LABEL                     "U13[19]/RMII0_TXEN"   /*!< Label */
#define BOARD_INITPINS_RMII0_TXEN_NAME                              "RMII0_TXEN"   /*!< Identifier name */

/* PORTA16 (coord K10), U13[20]/RMII0_TXD_0 */
#define BOARD_INITPINS_RMII0_TXD_0_PERIPHERAL                               ENET   /*!< Device name: ENET */
#define BOARD_INITPINS_RMII0_TXD_0_SIGNAL                               MII_TXD0   /*!< ENET signal: MII_TXD0 */
#define BOARD_INITPINS_RMII0_TXD_0_PIN_NAME                            MII0_TXD0   /*!< Pin name */
#define BOARD_INITPINS_RMII0_TXD_0_LABEL                   "U13[20]/RMII0_TXD_0"   /*!< Label */
#define BOARD_INITPINS_RMII0_TXD_0_NAME                            "RMII0_TXD_0"   /*!< Identifier name */

/* PORTA17 (coord K11), U13[21]/RMII0_TXD_1 */
#define BOARD_INITPINS_RMII0_TXD_1_PERIPHERAL                               ENET   /*!< Device name: ENET */
#define BOARD_INITPINS_RMII0_TXD_1_SIGNAL                               MII_TXD1   /*!< ENET signal: MII_TXD1 */
#define BOARD_INITPINS_RMII0_TXD_1_PIN_NAME                            MII0_TXD1   /*!< Pin name */
#define BOARD_INITPINS_RMII0_TXD_1_LABEL                   "U13[21]/RMII0_TXD_1"   /*!< Label */
#define BOARD_INITPINS_RMII0_TXD_1_NAME                            "RMII0_TXD_1"   /*!< Identifier name */

/* PORTA5 (coord M8), U13[17]/RMII0_RXER */
#define BOARD_INITPINS_RMII0_RXER_PERIPHERAL                                ENET   /*!< Device name: ENET */
#define BOARD_INITPINS_RMII0_RXER_SIGNAL                                MII_RXER   /*!< ENET signal: MII_RXER */
#define BOARD_INITPINS_RMII0_RXER_PIN_NAME                             MII0_RXER   /*!< Pin name */
#define BOARD_INITPINS_RMII0_RXER_LABEL                     "U13[17]/RMII0_RXER"   /*!< Label */
#define BOARD_INITPINS_RMII0_RXER_NAME                              "RMII0_RXER"   /*!< Identifier name */

/* PORTB0 (coord H10), U13[10]/RMII0_MDIO */
#define BOARD_INITPINS_RMII0_MDIO_PERIPHERAL                                ENET   /*!< Device name: ENET */
#define BOARD_INITPINS_RMII0_MDIO_SIGNAL                                MII_MDIO   /*!< ENET signal: MII_MDIO */
#define BOARD_INITPINS_RMII0_MDIO_PIN_NAME                             MII0_MDIO   /*!< Pin name */
#define BOARD_INITPINS_RMII0_MDIO_LABEL                     "U13[10]/RMII0_MDIO"   /*!< Label */
#define BOARD_INITPINS_RMII0_MDIO_NAME                              "RMII0_MDIO"   /*!< Identifier name */

/* PORTB1 (coord H9), U13[11]/RMII0_MDC */
#define BOARD_INITPINS_RMII0_MDC_PERIPHERAL                                 ENET   /*!< Device name: ENET */
#define BOARD_INITPINS_RMII0_MDC_SIGNAL                                  MII_MDC   /*!< ENET signal: MII_MDC */
#define BOARD_INITPINS_RMII0_MDC_PIN_NAME                               MII0_MDC   /*!< Pin name */
#define BOARD_INITPINS_RMII0_MDC_LABEL                       "U13[11]/RMII0_MDC"   /*!< Label */
#define BOARD_INITPINS_RMII0_MDC_NAME                                "RMII0_MDC"   /*!< Identifier name */

/* PORTC9 (coord D7), LEDRGB_RED */
#define BOARD_INITPINS_LED_RED_GPIO                                        GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_INITPINS_LED_RED_PORT                                        PORTC   /*!< PORT device name: PORTC */
#define BOARD_INITPINS_LED_RED_GPIO_PIN                                       9U   /*!< PORTC pin index: 9 */
#define BOARD_INITPINS_LED_RED_PIN_NAME                                     PTC9   /*!< Pin name */
#define BOARD_INITPINS_LED_RED_LABEL                                "LEDRGB_RED"   /*!< Label */
#define BOARD_INITPINS_LED_RED_NAME                                    "LED_RED"   /*!< Identifier name */
#define BOARD_INITPINS_LED_RED_DIRECTION                kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTE6 (coord E1), LEDRGB_GREEN */
#define BOARD_INITPINS_LED_GREEN_GPIO                                      GPIOE   /*!< GPIO device name: GPIOE */
#define BOARD_INITPINS_LED_GREEN_PORT                                      PORTE   /*!< PORT device name: PORTE */
#define BOARD_INITPINS_LED_GREEN_GPIO_PIN                                     6U   /*!< PORTE pin index: 6 */
#define BOARD_INITPINS_LED_GREEN_PIN_NAME                                   PTE6   /*!< Pin name */
#define BOARD_INITPINS_LED_GREEN_LABEL                            "LEDRGB_GREEN"   /*!< Label */
#define BOARD_INITPINS_LED_GREEN_NAME                                "LED_GREEN"   /*!< Identifier name */
#define BOARD_INITPINS_LED_GREEN_DIRECTION              kPIN_MUX_DirectionOutput   /*!< Direction */

/* PORTA11 (coord L9), LEDRGB_BLUE */
#define BOARD_INITPINS_LED_BLUE_GPIO                                       GPIOA   /*!< GPIO device name: GPIOA */
#define BOARD_INITPINS_LED_BLUE_PORT                                       PORTA   /*!< PORT device name: PORTA */
#define BOARD_INITPINS_LED_BLUE_GPIO_PIN                                     11U   /*!< PORTA pin index: 11 */
#define BOARD_INITPINS_LED_BLUE_PIN_NAME                                   PTA11   /*!< Pin name */
#define BOARD_INITPINS_LED_BLUE_LABEL                              "LEDRGB_BLUE"   /*!< Label */
#define BOARD_INITPINS_LED_BLUE_NAME                                  "LED_BLUE"   /*!< Identifier name */
#define BOARD_INITPINS_LED_BLUE_DIRECTION               kPIN_MUX_DirectionOutput   /*!< Direction */


/*!
 *
 */
void BOARD_InitPins(void);

/* PORTD9 (coord B9), U8[6]/U19[12]/I2C0_SDA */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_PERIPHERAL                        I2C0   /*!< Device name: I2C0 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_SIGNAL                             SDA   /*!< I2C0 signal: SDA */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_PIN_NAME                      I2C0_SDA   /*!< Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_LABEL         "U8[6]/U19[12]/I2C0_SDA"   /*!< Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_NAME                       "ACCEL_SDA"   /*!< Identifier name */

/* PORTD8 (coord C9), U8[4]/U19[11]/I2C0_SCL */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_PERIPHERAL                        I2C0   /*!< Device name: I2C0 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_SIGNAL                             SCL   /*!< I2C0 signal: SCL */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_PIN_NAME                      I2C0_SCL   /*!< Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_LABEL         "U8[4]/U19[11]/I2C0_SCL"   /*!< Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_NAME                       "ACCEL_SCL"   /*!< Identifier name */


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

/*******************************************************************************
 * EOF
 ******************************************************************************/
