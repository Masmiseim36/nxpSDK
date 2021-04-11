/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "milliseconds_delay.h"
#include "pin_mux.h"

#if defined(TOWER)
#define LED1_OFFSET 17
#define LED2_OFFSET 18

#define DELAY_1MS (1000)
#define LED_COUNT 2

static uint8_t led_offset[LED_COUNT] = { LED1_OFFSET, LED2_OFFSET };
static PORT_Type *led_port[LED_COUNT] = { PORTC, PORTC };
static GPIO_Type *led_gpio[LED_COUNT] = { GPIOC, GPIOC };
#endif // TOWER

#if defined(FREEDOM)
#define LED1_OFFSET 29
#define LED2_OFFSET 4
#define LED3_OFFSET 31

#define DELAY_1MS (1000)
#define LED_COUNT 3

static uint8_t led_offset[LED_COUNT] = { LED1_OFFSET, LED2_OFFSET, LED3_OFFSET };
static PORT_Type *led_port[LED_COUNT] = { PORTE, PORTC, PORTE };
static GPIO_Type *led_gpio[LED_COUNT] = { GPIOE, GPIOC, GPIOE };
#endif // FREEDOM

static void led_toggle(uint32_t leds)
{
    uint8_t i = 0;
    // led OFF
    for (i = 0; i < LED_COUNT; i++)
    {
        led_gpio[i]->PDOR |= (uint32_t)(1 << led_offset[i]);
    }
    // led ON
    led_gpio[leds]->PDOR &= (uint32_t) ~(1 << led_offset[leds]);
}

void delay(void)
{
    volatile uint32_t delayTicks = 2000000;

    while (delayTicks--)
    {
        __ASM("nop");
    }
}

int main(void)
{
    BOARD_InitBootPins();
    // Note: for ROM development, use this version of delay function,
    // Which is in order to test if the VTCOR is correct.
    milliseconds_delay_init();
    uint32_t leds = 0;
    while (1)
    {
        led_toggle(leds);
        milliseconds_delay(DELAY_1MS);

        ++leds;
        if (leds == LED_COUNT)
        {
            leds = 0;
        }
    }
}
