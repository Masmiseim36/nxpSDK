/*
 * Copyright (c) 2013 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "milliseconds_delay.h"

#if defined(TOWER)
#define LED1_OFFSET 6
#define LED2_OFFSET 7
#define LED3_OFFSET 8
#define LED4_OFFSET 9

#define DELAY_1MS (1000)
#define LED_COUNT 4

static uint8_t led_offset[LED_COUNT] = { LED1_OFFSET, LED2_OFFSET, LED3_OFFSET, LED4_OFFSET };
static PORT_Type *led_port[LED_COUNT] = { PORTE, PORTE, PORTE, PORTE };
static GPIO_Type *led_gpio[LED_COUNT] = { GPIOE, GPIOE, GPIOE, GPIOE };
#endif // TOWER

#if defined(FREEDOM)
#define LED1_OFFSET 21
#define LED2_OFFSET 22
#define LED3_OFFSET 26

#define DELAY_1MS (1000)
#define LED_COUNT 3

static uint8_t led_offset[LED_COUNT] = { LED1_OFFSET, LED2_OFFSET, LED3_OFFSET };
static PORT_Type *led_port[LED_COUNT] = { PORTB, PORTB, PORTE };
static GPIO_Type *led_gpio[LED_COUNT] = { GPIOB, GPIOB, GPIOE };
#endif // FREEDOM

static void init_hardware(void)
{
    SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK |
                   SIM_SCGC5_PORTE_MASK);

    SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK; // set PLLFLLSEL to select the PLL for this clock source

    uint8_t i;
    for (i = 0; i < LED_COUNT; i++)
    {
        // Enable the LED pins GPIO
        led_port[i]->PCR[led_offset[i]] = PORT_PCR_MUX(1);
        // Set ports to outputs
        led_gpio[i]->PDDR |= (1 << led_offset[i]);
    }
}

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
    init_hardware();
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
