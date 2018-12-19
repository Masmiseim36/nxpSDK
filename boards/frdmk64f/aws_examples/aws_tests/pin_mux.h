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



/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/* PORTC3 (number 73), IRQ */
#define BOARD_INITGT202SHIELD_IRQ_GPIO                                     GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_INITGT202SHIELD_IRQ_PORT                                     PORTC   /*!< PORT device name: PORTC */
#define BOARD_INITGT202SHIELD_IRQ_GPIO_PIN                                    3U   /*!< PORTC pin index: 3 */
#define BOARD_INITGT202SHIELD_IRQ_PIN_NAME                                  PTC3   /*!< Pin name */
#define BOARD_INITGT202SHIELD_IRQ_LABEL                                    "IRQ"   /*!< Label */
#define BOARD_INITGT202SHIELD_IRQ_NAME                                     "IRQ"   /*!< Identifier name */
#define BOARD_INITGT202SHIELD_IRQ_DIRECTION              kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTC12 (number 84), PWRON */
#define BOARD_INITGT202SHIELD_PWRON_GPIO                                   GPIOC   /*!< GPIO device name: GPIOC */
#define BOARD_INITGT202SHIELD_PWRON_PORT                                   PORTC   /*!< PORT device name: PORTC */
#define BOARD_INITGT202SHIELD_PWRON_GPIO_PIN                                 12U   /*!< PORTC pin index: 12 */
#define BOARD_INITGT202SHIELD_PWRON_PIN_NAME                               PTC12   /*!< Pin name */
#define BOARD_INITGT202SHIELD_PWRON_LABEL                                "PWRON"   /*!< Label */
#define BOARD_INITGT202SHIELD_PWRON_NAME                                 "PWRON"   /*!< Identifier name */
#define BOARD_INITGT202SHIELD_PWRON_DIRECTION           kPIN_MUX_DirectionOutput   /*!< Direction */


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitGT202Shield(void);

/* PORTB9 (number 57), IRQ */
#define BOARD_INITSILEX2401SHIELD_IRQ_GPIO                                 GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITSILEX2401SHIELD_IRQ_PORT                                 PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITSILEX2401SHIELD_IRQ_GPIO_PIN                                9U   /*!< PORTB pin index: 9 */
#define BOARD_INITSILEX2401SHIELD_IRQ_PIN_NAME                              PTB9   /*!< Pin name */
#define BOARD_INITSILEX2401SHIELD_IRQ_LABEL                                "IRQ"   /*!< Label */
#define BOARD_INITSILEX2401SHIELD_IRQ_NAME                                 "IRQ"   /*!< Identifier name */
#define BOARD_INITSILEX2401SHIELD_IRQ_DIRECTION          kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTB23 (number 69), PWRON */
#define BOARD_INITSILEX2401SHIELD_PWRON_GPIO                               GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITSILEX2401SHIELD_PWRON_PORT                               PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITSILEX2401SHIELD_PWRON_GPIO_PIN                             23U   /*!< PORTB pin index: 23 */
#define BOARD_INITSILEX2401SHIELD_PWRON_PIN_NAME                           PTB23   /*!< Pin name */
#define BOARD_INITSILEX2401SHIELD_PWRON_LABEL                            "PWRON"   /*!< Label */
#define BOARD_INITSILEX2401SHIELD_PWRON_NAME                             "PWRON"   /*!< Identifier name */
#define BOARD_INITSILEX2401SHIELD_PWRON_DIRECTION       kPIN_MUX_DirectionOutput   /*!< Direction */


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

/*******************************************************************************
 * EOF
 ******************************************************************************/
