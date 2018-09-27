/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "milliseconds_delay.h"

enum
{
    kFreq_24MHz = 24000000UL,
    kFreq_396MHz = 396000000UL,
};

#define LED1_OFFSET 9 // D18

#define DELAY_1S (1000)
#define LED_COUNT 1

static uint8_t led_offset[LED_COUNT] = { LED1_OFFSET };
static GPIO_Type *led_gpio[LED_COUNT] = { GPIO1 };

static void init_hardware(void)
{
    IOMUXC->SW_MUX_CTL_PAD[kIOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B0_09] = IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(5);

    uint8_t i;
    for (i = 0; i < LED_COUNT; i++)
    {
        // Set ports to outputs
        led_gpio[i]->GDIR |= (1 << led_offset[i]);
        led_gpio[i]->DR &= (uint32_t) ~(1 << led_offset[i]);
    }
}

static void led_toggle(uint32_t leds)
{
    // led toggle
    if (led_gpio[leds]->DR & (1 << led_offset[leds]))
    {
        led_gpio[leds]->DR &= (uint32_t) ~(1 << led_offset[leds]);
    }
    else
    {
        led_gpio[leds]->DR |= (1 << led_offset[leds]);
    }
}

int main(void)
{
    SystemCoreClock = kFreq_396MHz;

    init_hardware();
    // Note: for ROM development, use this version of delay function,
    // Which is in order to test if the VTCOR is correct.
    milliseconds_delay_init();
    uint32_t leds = 0;

    while (1)
    {
        led_toggle(leds);
        milliseconds_delay(DELAY_1S);
        led_toggle(leds);
        milliseconds_delay(DELAY_1S);

        ++leds;
        if (leds == LED_COUNT)
        {
            leds = 0;
        }
    }
}
