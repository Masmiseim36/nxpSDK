/*
############################################################################
#             Copyright (C), NXP Semiconductors
#                       (C), NXP B.V. of Eindhoven
#
# All rights are reserved. Reproduction in whole or in part is prohibited
# without the written consent of the copyright owner.
# NXP reserves the right to make changes without notice at any time.
# NXP makes no warranty, expressed, implied or statutory, including but
# not limited to any implied warranty of merchantability or fitness for
# any particular purpose, or that the use will not infringe any third
# party patent, copyright or trademark. NXP must not be liable for any
# loss or damage arising from its use.
############################################################################
*/

/*
 * @brief output led device implementation.
 * @Author lin.tao@nxp.com
 */

#include "board_define.h"

#include "FreeRTOS.h"
#include "board.h"
#include "sln_rgb_led_driver.h"

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

void RGB_LED_SetColor(rgbLedColor_t color)
{
    switch (color)
    {
        case LED_COLOR_RED:
            GPIO_PinWrite(BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, 1);
            GPIO_PinWrite(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, 0);
            GPIO_PinWrite(BOARD_LED_BLUE_PORT, BOARD_LED_BLUE_PIN, 0);
            break;
        case LED_COLOR_ORANGE:
            GPIO_PinWrite(BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, 1);
            GPIO_PinWrite(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, 1);
            GPIO_PinWrite(BOARD_LED_BLUE_PORT, BOARD_LED_BLUE_PIN, 0);
            break;
        case LED_COLOR_YELLOW:
            GPIO_PinWrite(BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, 1);
            GPIO_PinWrite(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, 1);
            GPIO_PinWrite(BOARD_LED_BLUE_PORT, BOARD_LED_BLUE_PIN, 0);
            break;
        case LED_COLOR_GREEN:
            GPIO_PinWrite(BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, 0);
            GPIO_PinWrite(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, 1);
            GPIO_PinWrite(BOARD_LED_BLUE_PORT, BOARD_LED_BLUE_PIN, 0);
            break;
        case LED_COLOR_BLUE:
            GPIO_PinWrite(BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, 0);
            GPIO_PinWrite(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, 0);
            GPIO_PinWrite(BOARD_LED_BLUE_PORT, BOARD_LED_BLUE_PIN, 1);
            break;
        case LED_COLOR_PURPLE:
            GPIO_PinWrite(BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, 1);
            GPIO_PinWrite(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, 0);
            GPIO_PinWrite(BOARD_LED_BLUE_PORT, BOARD_LED_BLUE_PIN, 1);
            break;
        case LED_COLOR_CYAN:
            GPIO_PinWrite(BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, 0);
            GPIO_PinWrite(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, 1);
            GPIO_PinWrite(BOARD_LED_BLUE_PORT, BOARD_LED_BLUE_PIN, 1);
            break;
        case LED_COLOR_WHITE:
            GPIO_PinWrite(BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, 1);
            GPIO_PinWrite(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, 1);
            GPIO_PinWrite(BOARD_LED_BLUE_PORT, BOARD_LED_BLUE_PIN, 1);
            break;
        case LED_COLOR_OFF:
            GPIO_PinWrite(BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, 0);
            GPIO_PinWrite(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, 0);
            GPIO_PinWrite(BOARD_LED_BLUE_PORT, BOARD_LED_BLUE_PIN, 0);
            break;
        default:
            GPIO_PinWrite(BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, 0);
            GPIO_PinWrite(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, 0);
            GPIO_PinWrite(BOARD_LED_BLUE_PORT, BOARD_LED_BLUE_PIN, 0);
            break;
    }
}

void RGB_LED_SetBrightnessColor(rgb_led_brightness_t brightness, rgbLedColor_t color)
{
    // Unable to set brightness for the LED on the 117F
    RGB_LED_SetColor(color);
}

status_t RGB_LED_Init(void)
{
    gpio_pin_config_t config = {
        .direction     = kGPIO_DigitalOutput, /*!< Specifies the pin direction. */
        .outputLogic   = 0,                   /*!< Set a default output logic, which has no use in input */
        .interruptMode = kGPIO_NoIntmode,     /*!< Specifies the pin interrupt mode, a value of
                                                 @ref gpio_interrupt_mode_t. */
    };
    int error = 0;

    GPIO_PinInit(BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, &config);
    GPIO_PinInit(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, &config);
    GPIO_PinInit(BOARD_LED_BLUE_PORT, BOARD_LED_BLUE_PIN, &config);
    return error;
}
