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


#define SOPT5_UART1TXSRC_UART_TX 0x00u /*!<@brief UART 1 transmit data source select: UART1_TX pin */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*! @name PORTD4 (number 61), CS
  @{ */
#define BOARD_INITGT202SHIELD_RF_CS_PERIPHERAL SPI1    /*!<@brief Device name: SPI1 */
#define BOARD_INITGT202SHIELD_RF_CS_SIGNAL PCS0_SS     /*!<@brief SPI1 signal: PCS0_SS */
#define BOARD_INITGT202SHIELD_RF_CS_PORT PORTD         /*!<@brief PORT device name: PORTD */
#define BOARD_INITGT202SHIELD_RF_CS_PIN 4U             /*!<@brief PORTD pin index: 4 */
#define BOARD_INITGT202SHIELD_RF_CS_PIN_NAME SPI1_PCS0 /*!<@brief Pin name */
#define BOARD_INITGT202SHIELD_RF_CS_LABEL "CS"         /*!<@brief Label */
#define BOARD_INITGT202SHIELD_RF_CS_NAME "RF_CS"       /*!<@brief Identifier name */
                                                       /* @} */

/*! @name PORTD5 (number 62), SCK
  @{ */
#define BOARD_INITGT202SHIELD_LEDRGB_BLUE_PERIPHERAL SPI1    /*!<@brief Device name: SPI1 */
#define BOARD_INITGT202SHIELD_LEDRGB_BLUE_SIGNAL SCK         /*!<@brief SPI1 signal: SCK */
#define BOARD_INITGT202SHIELD_LEDRGB_BLUE_PORT PORTD         /*!<@brief PORT device name: PORTD */
#define BOARD_INITGT202SHIELD_LEDRGB_BLUE_PIN 5U             /*!<@brief PORTD pin index: 5 */
#define BOARD_INITGT202SHIELD_LEDRGB_BLUE_PIN_NAME SPI1_SCK  /*!<@brief Pin name */
#define BOARD_INITGT202SHIELD_LEDRGB_BLUE_LABEL "SCK"        /*!<@brief Label */
#define BOARD_INITGT202SHIELD_LEDRGB_BLUE_NAME "LEDRGB_BLUE" /*!<@brief Identifier name */
                                                             /* @} */

/*! @name PORTC6 (number 51), IRQ
  @{ */
#define BOARD_INITGT202SHIELD_IRQ_PERIPHERAL GPIOC                  /*!<@brief Device name: GPIOC */
#define BOARD_INITGT202SHIELD_IRQ_SIGNAL GPIO                       /*!<@brief GPIOC signal: GPIO */
#define BOARD_INITGT202SHIELD_IRQ_GPIO GPIOC                        /*!<@brief GPIO device name: GPIOC */
#define BOARD_INITGT202SHIELD_IRQ_GPIO_PIN 6U                       /*!<@brief PORTC pin index: 6 */
#define BOARD_INITGT202SHIELD_IRQ_PORT PORTC                        /*!<@brief PORT device name: PORTC */
#define BOARD_INITGT202SHIELD_IRQ_PIN 6U                            /*!<@brief PORTC pin index: 6 */
#define BOARD_INITGT202SHIELD_IRQ_CHANNEL 6                         /*!<@brief GPIOC GPIO channel: 6 */
#define BOARD_INITGT202SHIELD_IRQ_PIN_NAME PTC6                     /*!<@brief Pin name */
#define BOARD_INITGT202SHIELD_IRQ_LABEL "IRQ"                       /*!<@brief Label */
#define BOARD_INITGT202SHIELD_IRQ_NAME "IRQ"                        /*!<@brief Identifier name */
#define BOARD_INITGT202SHIELD_IRQ_DIRECTION kPIN_MUX_DirectionInput /*!<@brief Direction */
                                                                    /* @} */

/*! @name PORTB19 (number 42), PWRON
  @{ */
#define BOARD_INITGT202SHIELD_PWRON_PERIPHERAL GPIOB                   /*!<@brief Device name: GPIOB */
#define BOARD_INITGT202SHIELD_PWRON_SIGNAL GPIO                        /*!<@brief GPIOB signal: GPIO */
#define BOARD_INITGT202SHIELD_PWRON_GPIO GPIOB                         /*!<@brief GPIO device name: GPIOB */
#define BOARD_INITGT202SHIELD_PWRON_GPIO_PIN 19U                       /*!<@brief PORTB pin index: 19 */
#define BOARD_INITGT202SHIELD_PWRON_PORT PORTB                         /*!<@brief PORT device name: PORTB */
#define BOARD_INITGT202SHIELD_PWRON_PIN 19U                            /*!<@brief PORTB pin index: 19 */
#define BOARD_INITGT202SHIELD_PWRON_CHANNEL 19                         /*!<@brief GPIOB GPIO channel: 19 */
#define BOARD_INITGT202SHIELD_PWRON_PIN_NAME PTB19                     /*!<@brief Pin name */
#define BOARD_INITGT202SHIELD_PWRON_LABEL "PWRON"                      /*!<@brief Label */
#define BOARD_INITGT202SHIELD_PWRON_NAME "PWRON"                       /*!<@brief Identifier name */
#define BOARD_INITGT202SHIELD_PWRON_DIRECTION kPIN_MUX_DirectionOutput /*!<@brief Direction */
                                                                       /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitGT202Shield(void);

/*! @name PORTD4 (number 61), CS
  @{ */
#define BOARD_INITSILEX2401SHIELD_RF_CS_PERIPHERAL SPI1    /*!<@brief Device name: SPI1 */
#define BOARD_INITSILEX2401SHIELD_RF_CS_SIGNAL PCS0_SS     /*!<@brief SPI1 signal: PCS0_SS */
#define BOARD_INITSILEX2401SHIELD_RF_CS_PORT PORTD         /*!<@brief PORT device name: PORTD */
#define BOARD_INITSILEX2401SHIELD_RF_CS_PIN 4U             /*!<@brief PORTD pin index: 4 */
#define BOARD_INITSILEX2401SHIELD_RF_CS_PIN_NAME SPI1_PCS0 /*!<@brief Pin name */
#define BOARD_INITSILEX2401SHIELD_RF_CS_LABEL "CS"         /*!<@brief Label */
#define BOARD_INITSILEX2401SHIELD_RF_CS_NAME "RF_CS"       /*!<@brief Identifier name */
                                                           /* @} */

/*! @name PORTD5 (number 62), SCK
  @{ */
#define BOARD_INITSILEX2401SHIELD_LEDRGB_BLUE_PERIPHERAL SPI1    /*!<@brief Device name: SPI1 */
#define BOARD_INITSILEX2401SHIELD_LEDRGB_BLUE_SIGNAL SCK         /*!<@brief SPI1 signal: SCK */
#define BOARD_INITSILEX2401SHIELD_LEDRGB_BLUE_PORT PORTD         /*!<@brief PORT device name: PORTD */
#define BOARD_INITSILEX2401SHIELD_LEDRGB_BLUE_PIN 5U             /*!<@brief PORTD pin index: 5 */
#define BOARD_INITSILEX2401SHIELD_LEDRGB_BLUE_PIN_NAME SPI1_SCK  /*!<@brief Pin name */
#define BOARD_INITSILEX2401SHIELD_LEDRGB_BLUE_LABEL "SCK"        /*!<@brief Label */
#define BOARD_INITSILEX2401SHIELD_LEDRGB_BLUE_NAME "LEDRGB_BLUE" /*!<@brief Identifier name */
                                                                 /* @} */

/*! @name PORTB16 (number 39), IRQ
  @{ */
#define BOARD_INITSILEX2401SHIELD_IRQ_PERIPHERAL GPIOB                  /*!<@brief Device name: GPIOB */
#define BOARD_INITSILEX2401SHIELD_IRQ_SIGNAL GPIO                       /*!<@brief GPIOB signal: GPIO */
#define BOARD_INITSILEX2401SHIELD_IRQ_GPIO GPIOB                        /*!<@brief GPIO device name: GPIOB */
#define BOARD_INITSILEX2401SHIELD_IRQ_GPIO_PIN 16U                      /*!<@brief PORTB pin index: 16 */
#define BOARD_INITSILEX2401SHIELD_IRQ_PORT PORTB                        /*!<@brief PORT device name: PORTB */
#define BOARD_INITSILEX2401SHIELD_IRQ_PIN 16U                           /*!<@brief PORTB pin index: 16 */
#define BOARD_INITSILEX2401SHIELD_IRQ_CHANNEL 16                        /*!<@brief GPIOB GPIO channel: 16 */
#define BOARD_INITSILEX2401SHIELD_IRQ_PIN_NAME PTB16                    /*!<@brief Pin name */
#define BOARD_INITSILEX2401SHIELD_IRQ_LABEL "IRQ"                       /*!<@brief Label */
#define BOARD_INITSILEX2401SHIELD_IRQ_NAME "IRQ"                        /*!<@brief Identifier name */
#define BOARD_INITSILEX2401SHIELD_IRQ_DIRECTION kPIN_MUX_DirectionInput /*!<@brief Direction */
                                                                        /* @} */

/*! @name PORTA4 (number 26), PWRON
  @{ */
#define BOARD_INITSILEX2401SHIELD_PWRON_PERIPHERAL GPIOA                   /*!<@brief Device name: GPIOA */
#define BOARD_INITSILEX2401SHIELD_PWRON_SIGNAL GPIO                        /*!<@brief GPIOA signal: GPIO */
#define BOARD_INITSILEX2401SHIELD_PWRON_GPIO GPIOA                         /*!<@brief GPIO device name: GPIOA */
#define BOARD_INITSILEX2401SHIELD_PWRON_GPIO_PIN 4U                        /*!<@brief PORTA pin index: 4 */
#define BOARD_INITSILEX2401SHIELD_PWRON_PORT PORTA                         /*!<@brief PORT device name: PORTA */
#define BOARD_INITSILEX2401SHIELD_PWRON_PIN 4U                             /*!<@brief PORTA pin index: 4 */
#define BOARD_INITSILEX2401SHIELD_PWRON_CHANNEL 4                          /*!<@brief GPIOA GPIO channel: 4 */
#define BOARD_INITSILEX2401SHIELD_PWRON_PIN_NAME PTA4                      /*!<@brief Pin name */
#define BOARD_INITSILEX2401SHIELD_PWRON_LABEL "PWRON"                      /*!<@brief Label */
#define BOARD_INITSILEX2401SHIELD_PWRON_NAME "PWRON"                       /*!<@brief Identifier name */
#define BOARD_INITSILEX2401SHIELD_PWRON_DIRECTION kPIN_MUX_DirectionOutput /*!<@brief Direction */
                                                                           /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitSilex2401Shield(void);

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
