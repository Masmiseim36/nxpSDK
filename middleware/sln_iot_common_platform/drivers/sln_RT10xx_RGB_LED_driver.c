/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes */
#include "FreeRTOS.h"
#include "task.h"
#include "sln_RT10xx_RGB_LED_driver.h"

__attribute__((weak)) status_t RGB_LED_Init(void)
{
    /*  RGB LED Initialization */
    return kStatus_Fail;
}

__attribute__((weak)) void RGB_LED_SetColor(rgbLedColor_t color)
{
    /* Set RGB color from enum */
}

__attribute__((weak)) void RGB_LED_SetBrightnessColor(rgb_led_brightness_t brightness, rgbLedColor_t color)
{
    /* Set RGB color and brightness from enum */
}

void RGB_LED_Blink(uint8_t brightness, uint8_t color, uint32_t blinkrate, uint32_t *blinkcount, bool *blinktoggle)
{
    if (*blinkcount > blinkrate)
    {
        *blinkcount = 0;

        // blink an led so user knows its working
        if (*blinktoggle)
        {
            RGB_LED_SetBrightnessColor(brightness, color);
            *blinktoggle = false;
        }
        else
        {
            RGB_LED_SetBrightnessColor(LED_BRIGHT_OFF, LED_COLOR_OFF); /* off is black */
            *blinktoggle = true;
        }
    }
    *blinkcount += 1;
}

void ErrorBlinkLED(void)
{
    for (int i = 0; i < 6; i++)
    {
        RGB_LED_SetBrightnessColor(LED_BRIGHT_MEDIUM, LED_COLOR_RED);
        vTaskDelay(250);
        RGB_LED_SetBrightnessColor(LED_BRIGHT_OFF, LED_COLOR_OFF);
        vTaskDelay(250);
    }
}
