/**
 * @file ledHander.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 *
 * Copyright 2017,2018 NXP
 * SPDX-License-Identifier: Apache-2.0
 *
 * @par Description
 * LED Handler
 */

#ifndef SOURCES_LED_HANDLER_H_
#define SOURCES_LED_HANDLER_H_

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */
#include <sm_types.h>
#if AX_EMBEDDED
#   include <board.h>
#endif
#include <stdio.h>

/*******************************************************************************
 * types
 ******************************************************************************/
#if !AX_EMBEDDED || defined(IMX_RT)
#define LED_RED_ON() LOG_I("* RED LED ON\n");

#define LED_RED_OFF() LOG_I("* RED LED OFF\n");

#define LED_RED_TOGGLE() LOG_I("* RED LED TOGGLE\n");

#define LED_GREEN_ON() LOG_I("* GREEN LED ON\n");

#define LED_GREEN_OFF() LOG_I("* GREEN LED OFF\n");

#define LED_GREEN_TOGGLE() LOG_I("* GREEN LED TOGGLE\n");

#define LED_BLUE_ON() LOG_I("* BLUE LED ON\n");

#define LED_BLUE_OFF() LOG_I("* BLUE LED OFF\n");

#define LED_BLUE_TOGGLE() LOG_I("* BLUE LED TOGGLE\n\n");
#endif

typedef enum LED_COLOR
{
    LED_INVALID,
    RED,
    GREEN,
    BLUE
} ledColor_t;
typedef enum LED_STATE
{
    LED_ON,
    LED_OFF,
    LED_TOGGLE
} ledState_t;
/*******************************************************************************
* Functions
******************************************************************************/
void led_handler(ledColor_t led, ledState_t state);

#endif /* SOURCES_LED_HANDLER_H_ */
