/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SLN_RT10XX_LED_H_
#define _SLN_RT10XX_LED_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief LED Brightness */
typedef enum
{
    LED_BRIGHT_OFF    = 0, /*!< LED Off */
    LED_BRIGHT_LOW    = 1, /*!< LED Low Brightness */
    LED_BRIGHT_MEDIUM = 2, /*!< LED Medium Brightness */
    LED_BRIGHT_HIGH   = 3  /*!< LED High Brightness */
} rgb_led_brightness_t;

/*! @brief LED Colog */
typedef enum _rgb_led_color
{
    LED_COLOR_RED,    /*!< LED Red Color */
    LED_COLOR_ORANGE, /*!< LED Orange Color */
    LED_COLOR_YELLOW, /*!< LED Yellow Color */
    LED_COLOR_GREEN,  /*!< LED Green Color */
    LED_COLOR_BLUE,   /*!< LED Blue Color */
    LED_COLOR_PURPLE, /*!< LED Purple Color */
    LED_COLOR_CYAN,   /*!< LED Cyan Color */
    LED_COLOR_WHITE,  /*!< LED White Color */
    LED_COLOR_OFF,    /*!< LED Off */
} rgbLedColor_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Set RGB color from enum
 *
 * @param color Target color to set RGB LED
 */
void RGB_LED_SetColor(rgbLedColor_t color);

/*!
 * @brief Set RGB color and brightness from enum
 *
 * @param brightness Target brightness to set RGB LED
 * @param color Target color to set RGB LED
 */
void RGB_LED_SetBrightnessColor(rgb_led_brightness_t brightness, rgbLedColor_t color);

/*!
 * @brief Blink RGB LED for given duration and frequency
 *
 * @param brightness Target brightness to set RGB LED
 * @param color Target color to set RGB LED
 * @param blinkrate Target rate to blink RGB LED
 * @param blinkcount Pointer to count target
 * @param blinktoggle Pointer to initial toggle state
 */
void RGB_LED_Blink(uint8_t brightness, uint8_t color, uint32_t blinkrate, uint32_t *blinkcount, bool *blinktoggle);

/*!
 * @brief Blink red led function, called when MSD / OTA fails
 *
 */
void ErrorBlinkLED(void);

/*!
 * @brief RGB LED Initialization
 *
 * @returns Status of initialization
 */
status_t RGB_LED_Init(void);

#if defined(__cplusplus)
}
#endif
#endif
