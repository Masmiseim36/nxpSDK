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


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*! @name PORTC13 (number 85), IRQ
  @{ */
#define BOARD_INITGT202SHIELD_IRQ_PERIPHERAL GPIOC                  /*!<@brief Device name: GPIOC */
#define BOARD_INITGT202SHIELD_IRQ_SIGNAL GPIO                       /*!<@brief GPIOC signal: GPIO */
#define BOARD_INITGT202SHIELD_IRQ_GPIO GPIOC                        /*!<@brief GPIO device name: GPIOC */
#define BOARD_INITGT202SHIELD_IRQ_GPIO_PIN 13U                      /*!<@brief PORTC pin index: 13 */
#define BOARD_INITGT202SHIELD_IRQ_PORT PORTC                        /*!<@brief PORT device name: PORTC */
#define BOARD_INITGT202SHIELD_IRQ_PIN 13U                           /*!<@brief PORTC pin index: 13 */
#define BOARD_INITGT202SHIELD_IRQ_CHANNEL 13                        /*!<@brief GPIOC GPIO channel: 13 */
#define BOARD_INITGT202SHIELD_IRQ_PIN_NAME PTC13                    /*!<@brief Pin name */
#define BOARD_INITGT202SHIELD_IRQ_LABEL "IRQ"                       /*!<@brief Label */
#define BOARD_INITGT202SHIELD_IRQ_NAME "IRQ"                        /*!<@brief Identifier name */
#define BOARD_INITGT202SHIELD_IRQ_DIRECTION kPIN_MUX_DirectionInput /*!<@brief Direction */
                                                                    /* @} */

/*! @name PORTD6 (number 99), PWRON
  @{ */
#define BOARD_INITGT202SHIELD_PWRON_PERIPHERAL GPIOD                   /*!<@brief Device name: GPIOD */
#define BOARD_INITGT202SHIELD_PWRON_SIGNAL GPIO                        /*!<@brief GPIOD signal: GPIO */
#define BOARD_INITGT202SHIELD_PWRON_GPIO GPIOD                         /*!<@brief GPIO device name: GPIOD */
#define BOARD_INITGT202SHIELD_PWRON_GPIO_PIN 6U                        /*!<@brief PORTD pin index: 6 */
#define BOARD_INITGT202SHIELD_PWRON_PORT PORTD                         /*!<@brief PORT device name: PORTD */
#define BOARD_INITGT202SHIELD_PWRON_PIN 6U                             /*!<@brief PORTD pin index: 6 */
#define BOARD_INITGT202SHIELD_PWRON_CHANNEL 6                          /*!<@brief GPIOD GPIO channel: 6 */
#define BOARD_INITGT202SHIELD_PWRON_PIN_NAME PTD6                      /*!<@brief Pin name */
#define BOARD_INITGT202SHIELD_PWRON_LABEL "PWRON"                      /*!<@brief Label */
#define BOARD_INITGT202SHIELD_PWRON_NAME "PWRON"                       /*!<@brief Identifier name */
#define BOARD_INITGT202SHIELD_PWRON_DIRECTION kPIN_MUX_DirectionOutput /*!<@brief Direction */
                                                                       /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitGT202Shield(void);

/*! @name PORTB2 (number 55), IRQ
  @{ */
#define BOARD_INITSILEX2401SHIELD_IRQ_PERIPHERAL GPIOB                  /*!<@brief Device name: GPIOB */
#define BOARD_INITSILEX2401SHIELD_IRQ_SIGNAL GPIO                       /*!<@brief GPIOB signal: GPIO */
#define BOARD_INITSILEX2401SHIELD_IRQ_GPIO GPIOB                        /*!<@brief GPIO device name: GPIOB */
#define BOARD_INITSILEX2401SHIELD_IRQ_GPIO_PIN 2U                       /*!<@brief PORTB pin index: 2 */
#define BOARD_INITSILEX2401SHIELD_IRQ_PORT PORTB                        /*!<@brief PORT device name: PORTB */
#define BOARD_INITSILEX2401SHIELD_IRQ_PIN 2U                            /*!<@brief PORTB pin index: 2 */
#define BOARD_INITSILEX2401SHIELD_IRQ_CHANNEL 2                         /*!<@brief GPIOB GPIO channel: 2 */
#define BOARD_INITSILEX2401SHIELD_IRQ_PIN_NAME PTB2                     /*!<@brief Pin name */
#define BOARD_INITSILEX2401SHIELD_IRQ_LABEL "IRQ"                       /*!<@brief Label */
#define BOARD_INITSILEX2401SHIELD_IRQ_NAME "IRQ"                        /*!<@brief Identifier name */
#define BOARD_INITSILEX2401SHIELD_IRQ_DIRECTION kPIN_MUX_DirectionInput /*!<@brief Direction */
                                                                        /* @} */

/*! @name PORTB10 (number 60), PWRON
  @{ */
#define BOARD_INITSILEX2401SHIELD_PWRON_PERIPHERAL GPIOB                   /*!<@brief Device name: GPIOB */
#define BOARD_INITSILEX2401SHIELD_PWRON_SIGNAL GPIO                        /*!<@brief GPIOB signal: GPIO */
#define BOARD_INITSILEX2401SHIELD_PWRON_GPIO GPIOB                         /*!<@brief GPIO device name: GPIOB */
#define BOARD_INITSILEX2401SHIELD_PWRON_GPIO_PIN 10U                       /*!<@brief PORTB pin index: 10 */
#define BOARD_INITSILEX2401SHIELD_PWRON_PORT PORTB                         /*!<@brief PORT device name: PORTB */
#define BOARD_INITSILEX2401SHIELD_PWRON_PIN 10U                            /*!<@brief PORTB pin index: 10 */
#define BOARD_INITSILEX2401SHIELD_PWRON_CHANNEL 10                         /*!<@brief GPIOB GPIO channel: 10 */
#define BOARD_INITSILEX2401SHIELD_PWRON_PIN_NAME PTB10                     /*!<@brief Pin name */
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
