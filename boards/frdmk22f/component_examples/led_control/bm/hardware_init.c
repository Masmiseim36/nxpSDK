/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#if (defined(BUTTON_COUNT) && (BUTTON_COUNT > 0U))
#include "fsl_component_button.h"
#endif
#include "fsl_component_led.h"

/*${header:end}*/

/*${variable:start}*/
button_config_t g_buttonConfig[BUTTON_COUNT] = {
    {
        .gpio =
            {
                .direction       = kHAL_GpioDirectionIn,
                .pinStateDefault = 1,
                .port            = 1,
                .pin             = BOARD_SW3_GPIO_PIN,
            },
    },
};

led_config_t g_ledMonochrome[LED_TYPE_MONOCHROME_COUNT] = {
    {
        .type = kLED_TypeMonochrome,
        .ledMonochrome =
            {
                .monochromePin =
                    {
                        .dimmingEnable = 0,
                        .gpio =
                            {
#if (defined(LED_USE_CONFIGURE_STRUCTURE) && (LED_USE_CONFIGURE_STRUCTURE > 0U))
                                .direction = kHAL_GpioDirectionOut,
#endif
                                .level = 1,
                                .port  = 0,
                                .pin   = BOARD_LED_RED_GPIO_PIN,
                            },
                    },
            },
    },
};

led_config_t g_ledMonochromeDim[LED_TYPE_MONOCHROME_COUNT] = {
    {
        .type = kLED_TypeMonochrome,
        .ledMonochrome =
            {
                .monochromePin =
                    {
                        .dimmingEnable = 1,
                        .dimming =
                            {
                                BOARD_SOURCE_CLOCK,
                                RED_INSTACE,
                                RED_CHANNEL,
                                1,
                            },
                    },
            },
    },
};
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
#if (defined(LED_DIMMING_ENABLEMENT) && (LED_DIMMING_ENABLEMENT > 0U))
    BOARD_InitPWM();
#else
    BOARD_InitLEDs();
#endif
}
/*${function:end}*/
