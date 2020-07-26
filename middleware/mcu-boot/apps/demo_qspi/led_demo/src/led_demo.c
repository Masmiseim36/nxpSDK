/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "milliseconds_delay.h"
#include "led_bsp.h"
#include "pin_mux.h"

//! @brief Toggle LEDs according to specified parameter leds.
void led_toggle(uint32_t leds);

int main(void)
{
    BOARD_InitBootPins();
    milliseconds_delay_init();

    uint32_t led_idx = LED0;
    while (1)
    {
        led_toggle(led_idx);
        milliseconds_delay(500);
        led_toggle(led_idx);

        led_idx <<= 1;
        if (led_idx > MAX_LED_IDX)
        {
            led_idx = LED0;
        }
    }
}

void led_toggle(uint32_t leds)
{
    if (leds & LED0)
    {
        LED0_GPIO->PTOR |= (1 << LED0_OFFSET);
    }
    if (leds & LED1)
    {
        LED1_GPIO->PTOR |= (1 << LED1_OFFSET);
    }
#if MAX_LED_IDX == LED2
    if (leds & LED2)
    {
        LED2_GPIO->PTOR |= (1 << LED2_OFFSET);
    }
#endif // #if MAX_LED_IDX == LED2
}
