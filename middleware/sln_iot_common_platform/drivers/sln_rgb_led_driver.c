/*
 * Copyright 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/* FreeRTOS kernel includes */
#include "FreeRTOS.h"
#include "task.h"
#include "sln_rgb_led_driver.h"

#define ERROR_BLINK_MS  250
#define ERROR_BLINK_CNT 6

__attribute__((weak)) status_t RGB_LED_Init(void)
{
    /*  RGB LED Initialization */
    return kStatus_Success;
}

__attribute__((weak)) void RGB_LED_SetColor(rgbLedColor_t color)
{
    /* Set RGB color from enum */
}

__attribute__((weak)) void RGB_LED_SetBrightnessColor(rgb_led_brightness_t brightness, rgbLedColor_t color)
{
    /* Set RGB color and brightness from enum */
}

__attribute__((weak)) void RGB_LED_Blink(
    uint8_t brightness, uint8_t color, uint32_t blinkrate, uint32_t *blinkcount, bool *blinktoggle)
{
    if (blinkcount == NULL)
    {
        configPRINTF(("[ERROR] RGB_LED_Blink - Invalid blinkcount pointer supplied\r\n"));
        return;
    }
    if (blinktoggle == NULL)
    {
        configPRINTF(("[ERROR] RGB_LED_Blink - Invalid blinktoggle pointer supplied\r\n"));
        return;
    }
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

__attribute__((weak)) void ErrorBlinkLED(void)
{
    for (int i = 0; i < ERROR_BLINK_CNT; i++)
    {
        RGB_LED_SetBrightnessColor(LED_BRIGHT_MEDIUM, LED_COLOR_RED);
        vTaskDelay(ERROR_BLINK_MS);
        RGB_LED_SetBrightnessColor(LED_BRIGHT_OFF, LED_COLOR_OFF);
        vTaskDelay(ERROR_BLINK_MS);
    }
}
