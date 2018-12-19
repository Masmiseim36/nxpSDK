/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
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


/* PORTB16 (number 62), U7[4]/UART0_RX */
#define BOARD_INITPINS_DEBUG_UART_RX_PERIPHERAL                            UART0   /*!< Device name: UART0 */
#define BOARD_INITPINS_DEBUG_UART_RX_SIGNAL                                   RX   /*!< UART0 signal: RX */
#define BOARD_INITPINS_DEBUG_UART_RX_PIN_NAME                           UART0_RX   /*!< Pin name */
#define BOARD_INITPINS_DEBUG_UART_RX_LABEL                      "U7[4]/UART0_RX"   /*!< Label */
#define BOARD_INITPINS_DEBUG_UART_RX_NAME                        "DEBUG_UART_RX"   /*!< Identifier name */

/* PORTB17 (number 63), U10[1]/UART0_TX */
#define BOARD_INITPINS_DEBUG_UART_TX_PERIPHERAL                            UART0   /*!< Device name: UART0 */
#define BOARD_INITPINS_DEBUG_UART_TX_SIGNAL                                   TX   /*!< UART0 signal: TX */
#define BOARD_INITPINS_DEBUG_UART_TX_PIN_NAME                           UART0_TX   /*!< Pin name */
#define BOARD_INITPINS_DEBUG_UART_TX_LABEL                     "U10[1]/UART0_TX"   /*!< Label */
#define BOARD_INITPINS_DEBUG_UART_TX_NAME                        "DEBUG_UART_TX"   /*!< Identifier name */

/* PORTC16 (number 90), J1[2] */
#define BOARD_INITPINS_TMR_1588_0_PERIPHERAL                                ENET   /*!< Device name: ENET */
#define BOARD_INITPINS_TMR_1588_0_SIGNAL                                TMR_1588   /*!< ENET signal: TMR_1588 */
#define BOARD_INITPINS_TMR_1588_0_CHANNEL                                      0   /*!< ENET TMR_1588 channel: 0 */
#define BOARD_INITPINS_TMR_1588_0_PIN_NAME                       ENET0_1588_TMR0   /*!< Pin name */
#define BOARD_INITPINS_TMR_1588_0_LABEL                                  "J1[2]"   /*!< Label */
#define BOARD_INITPINS_TMR_1588_0_NAME                              "TMR_1588_0"   /*!< Identifier name */

/* PORTC17 (number 91), J1[4] */
#define BOARD_INITPINS_TMR_1588_1_PERIPHERAL                                ENET   /*!< Device name: ENET */
#define BOARD_INITPINS_TMR_1588_1_SIGNAL                                TMR_1588   /*!< ENET signal: TMR_1588 */
#define BOARD_INITPINS_TMR_1588_1_CHANNEL                                      1   /*!< ENET TMR_1588 channel: 1 */
#define BOARD_INITPINS_TMR_1588_1_PIN_NAME                       ENET0_1588_TMR1   /*!< Pin name */
#define BOARD_INITPINS_TMR_1588_1_LABEL                                  "J1[4]"   /*!< Label */
#define BOARD_INITPINS_TMR_1588_1_NAME                              "TMR_1588_1"   /*!< Identifier name */

/* PORTB1 (number 54), U13[11]/RMII0_MDC */
#define BOARD_INITPINS_RMII0_MDC_PERIPHERAL                                 ENET   /*!< Device name: ENET */
#define BOARD_INITPINS_RMII0_MDC_SIGNAL                                 RMII_MDC   /*!< ENET signal: RMII_MDC */
#define BOARD_INITPINS_RMII0_MDC_PIN_NAME                              RMII0_MDC   /*!< Pin name */
#define BOARD_INITPINS_RMII0_MDC_LABEL                       "U13[11]/RMII0_MDC"   /*!< Label */
#define BOARD_INITPINS_RMII0_MDC_NAME                                "RMII0_MDC"   /*!< Identifier name */

/* PORTB0 (number 53), U13[10]/RMII0_MDIO */
#define BOARD_INITPINS_RMII0_MDIO_PERIPHERAL                                ENET   /*!< Device name: ENET */
#define BOARD_INITPINS_RMII0_MDIO_SIGNAL                               RMII_MDIO   /*!< ENET signal: RMII_MDIO */
#define BOARD_INITPINS_RMII0_MDIO_PIN_NAME                            RMII0_MDIO   /*!< Pin name */
#define BOARD_INITPINS_RMII0_MDIO_LABEL                     "U13[10]/RMII0_MDIO"   /*!< Label */
#define BOARD_INITPINS_RMII0_MDIO_NAME                              "RMII0_MDIO"   /*!< Identifier name */

/* PORTA12 (number 42), U13[12]/RMII0_RXD_1 */
#define BOARD_INITPINS_RMII0_RXD1_PERIPHERAL                                ENET   /*!< Device name: ENET */
#define BOARD_INITPINS_RMII0_RXD1_SIGNAL                               RMII_RXD1   /*!< ENET signal: RMII_RXD1 */
#define BOARD_INITPINS_RMII0_RXD1_PIN_NAME                            RMII0_RXD1   /*!< Pin name */
#define BOARD_INITPINS_RMII0_RXD1_LABEL                    "U13[12]/RMII0_RXD_1"   /*!< Label */
#define BOARD_INITPINS_RMII0_RXD1_NAME                              "RMII0_RXD1"   /*!< Identifier name */

/* PORTA13 (number 43), U13[13]/RMII0_RXD_0 */
#define BOARD_INITPINS_RMII0_RXD0_PERIPHERAL                                ENET   /*!< Device name: ENET */
#define BOARD_INITPINS_RMII0_RXD0_SIGNAL                               RMII_RXD0   /*!< ENET signal: RMII_RXD0 */
#define BOARD_INITPINS_RMII0_RXD0_PIN_NAME                            RMII0_RXD0   /*!< Pin name */
#define BOARD_INITPINS_RMII0_RXD0_LABEL                    "U13[13]/RMII0_RXD_0"   /*!< Label */
#define BOARD_INITPINS_RMII0_RXD0_NAME                              "RMII0_RXD0"   /*!< Identifier name */

/* PORTA14 (number 44), U13[15]/RMII0_CRS_DV */
#define BOARD_INITPINS_RMII0_CRS_DV_PERIPHERAL                              ENET   /*!< Device name: ENET */
#define BOARD_INITPINS_RMII0_CRS_DV_SIGNAL                           RMII_CRS_DV   /*!< ENET signal: RMII_CRS_DV */
#define BOARD_INITPINS_RMII0_CRS_DV_PIN_NAME                        RMII0_CRS_DV   /*!< Pin name */
#define BOARD_INITPINS_RMII0_CRS_DV_LABEL                 "U13[15]/RMII0_CRS_DV"   /*!< Label */
#define BOARD_INITPINS_RMII0_CRS_DV_NAME                          "RMII0_CRS_DV"   /*!< Identifier name */

/* PORTA15 (number 45), U13[19]/RMII0_TXEN */
#define BOARD_INITPINS_RMII0_TXEN_PERIPHERAL                                ENET   /*!< Device name: ENET */
#define BOARD_INITPINS_RMII0_TXEN_SIGNAL                               RMII_TXEN   /*!< ENET signal: RMII_TXEN */
#define BOARD_INITPINS_RMII0_TXEN_PIN_NAME                            RMII0_TXEN   /*!< Pin name */
#define BOARD_INITPINS_RMII0_TXEN_LABEL                     "U13[19]/RMII0_TXEN"   /*!< Label */
#define BOARD_INITPINS_RMII0_TXEN_NAME                              "RMII0_TXEN"   /*!< Identifier name */

/* PORTA17 (number 47), U13[21]/RMII0_TXD1 */
#define BOARD_INITPINS_RMII0_TXD1_PERIPHERAL                                ENET   /*!< Device name: ENET */
#define BOARD_INITPINS_RMII0_TXD1_SIGNAL                               RMII_TXD1   /*!< ENET signal: RMII_TXD1 */
#define BOARD_INITPINS_RMII0_TXD1_PIN_NAME                            RMII0_TXD1   /*!< Pin name */
#define BOARD_INITPINS_RMII0_TXD1_LABEL                     "U13[21]/RMII0_TXD1"   /*!< Label */
#define BOARD_INITPINS_RMII0_TXD1_NAME                              "RMII0_TXD1"   /*!< Identifier name */

/* PORTA16 (number 46), U13[20]/RMII0_TXD0 */
#define BOARD_INITPINS_RMII0_TXD0_PERIPHERAL                                ENET   /*!< Device name: ENET */
#define BOARD_INITPINS_RMII0_TXD0_SIGNAL                               RMII_TXD0   /*!< ENET signal: RMII_TXD0 */
#define BOARD_INITPINS_RMII0_TXD0_PIN_NAME                            RMII0_TXD0   /*!< Pin name */
#define BOARD_INITPINS_RMII0_TXD0_LABEL                     "U13[20]/RMII0_TXD0"   /*!< Label */
#define BOARD_INITPINS_RMII0_TXD0_NAME                              "RMII0_TXD0"   /*!< Identifier name */

/* PORTA5 (number 39), U13[17]/RMII0_RXER */
#define BOARD_INITPINS_RMII0_RXER_PERIPHERAL                                ENET   /*!< Device name: ENET */
#define BOARD_INITPINS_RMII0_RXER_SIGNAL                               RMII_RXER   /*!< ENET signal: RMII_RXER */
#define BOARD_INITPINS_RMII0_RXER_PIN_NAME                            RMII0_RXER   /*!< Pin name */
#define BOARD_INITPINS_RMII0_RXER_LABEL                     "U13[17]/RMII0_RXER"   /*!< Label */
#define BOARD_INITPINS_RMII0_RXER_NAME                              "RMII0_RXER"   /*!< Identifier name */

/* PORTB21 (number 67), D12[3]/LEDRGB_BLUE */
#define BOARD_INITPINS_LED_BLUE_GPIO                                       GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITPINS_LED_BLUE_PORT                                       PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITPINS_LED_BLUE_GPIO_PIN                                     21U   /*!< PORTB pin index: 21 */
#define BOARD_INITPINS_LED_BLUE_PIN_NAME                                   PTB21   /*!< Pin name */
#define BOARD_INITPINS_LED_BLUE_LABEL                       "D12[3]/LEDRGB_BLUE"   /*!< Label */
#define BOARD_INITPINS_LED_BLUE_NAME                                  "LED_BLUE"   /*!< Identifier name */

/* PORTB22 (number 68), D12[1]/LEDRGB_RED */
#define BOARD_INITPINS_LED_RED_GPIO                                        GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITPINS_LED_RED_PORT                                        PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITPINS_LED_RED_GPIO_PIN                                      22U   /*!< PORTB pin index: 22 */
#define BOARD_INITPINS_LED_RED_PIN_NAME                                    PTB22   /*!< Pin name */
#define BOARD_INITPINS_LED_RED_LABEL                         "D12[1]/LEDRGB_RED"   /*!< Label */
#define BOARD_INITPINS_LED_RED_NAME                                    "LED_RED"   /*!< Identifier name */

/* PORTE26 (number 33), J2[1]/D12[4]/LEDRGB_GREEN */
#define BOARD_INITPINS_LED_GREEN_GPIO                                      GPIOE   /*!< GPIO device name: GPIOE */
#define BOARD_INITPINS_LED_GREEN_PORT                                      PORTE   /*!< PORT device name: PORTE */
#define BOARD_INITPINS_LED_GREEN_GPIO_PIN                                    26U   /*!< PORTE pin index: 26 */
#define BOARD_INITPINS_LED_GREEN_PIN_NAME                                  PTE26   /*!< Pin name */
#define BOARD_INITPINS_LED_GREEN_LABEL               "J2[1]/D12[4]/LEDRGB_GREEN"   /*!< Label */
#define BOARD_INITPINS_LED_GREEN_NAME                                "LED_GREEN"   /*!< Identifier name */


/*!
 *
 */
void BOARD_InitPins(void);

/* PORTE24 (number 31), J2[20]/U8[4]/I2C0_SCL */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_PERIPHERAL                        I2C0   /*!< Device name: I2C0 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_SIGNAL                             SCL   /*!< I2C0 signal: SCL */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_PIN_NAME                      I2C0_SCL   /*!< Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_LABEL          "J2[20]/U8[4]/I2C0_SCL"   /*!< Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SCL_NAME                       "ACCEL_SCL"   /*!< Identifier name */

/* PORTE25 (number 32), J2[18]/U8[6]/I2C0_SDA */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_PERIPHERAL                        I2C0   /*!< Device name: I2C0 */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_SIGNAL                             SDA   /*!< I2C0 signal: SDA */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_PIN_NAME                      I2C0_SDA   /*!< Pin name */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_LABEL          "J2[18]/U8[6]/I2C0_SDA"   /*!< Label */
#define BOARD_I2C_CONFIGUREPINS_ACCEL_SDA_NAME                       "ACCEL_SDA"   /*!< Identifier name */


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

/*******************************************************************************
 * EOF
 ******************************************************************************/
