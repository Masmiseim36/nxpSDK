/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "milliseconds_delay.h"
#include "led_bsp.h"

//! @brief Intialize necessary IPs.
void hardware_init(void);
//! @brief Toggle LEDs according to specified parameter leds.
void led_toggle(uint32_t leds);

int main(void)
{
    hardware_init();
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

void hardware_init(void)
{
#if defined(SIM_SCGC5_PORTA_MASK)
    SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK |
                   SIM_SCGC5_PORTE_MASK);
#else
    SIM->SCGC5 |=
        (SIM_SCGC5_PTA_MASK | SIM_SCGC5_PTB_MASK | SIM_SCGC5_PTC_MASK | SIM_SCGC5_PTD_MASK | SIM_SCGC5_PTE_MASK);
#endif

    // Enable the LED pins GPIO
    LED0_PORT->PCR[LED0_OFFSET] = PORT_PCR_MUX(1);
    LED1_PORT->PCR[LED1_OFFSET] = PORT_PCR_MUX(1);
#if MAX_LED_IDX == LED2
    LED2_PORT->PCR[LED2_OFFSET] = PORT_PCR_MUX(1);
#endif // #if MAX_LED_IDX == LED2

    // Configure port to output mode
    LED0_GPIO->PDDR |= (1 << LED0_OFFSET);
    LED1_GPIO->PDDR |= (1 << LED1_OFFSET);
#if MAX_LED_IDX == LED2
    LED2_GPIO->PDDR |= (1 << LED2_OFFSET);
#endif // #if MAX_LED_IDX == LED2

// If led will be turned on with low level, need to turn all of them off at startup.
#if LED_POLARITY == LED_ON_LOW_LEVEL
    LED0_GPIO->PTOR |= (1 << LED0_OFFSET);
    LED1_GPIO->PTOR |= (1 << LED1_OFFSET);
#if MAX_LED_IDX == LED2
    LED2_GPIO->PTOR |= (1 << LED2_OFFSET);
#endif // #if MAX_LED_IDX == LED2
#endif // #if LED_POLARITY == LED_ON_LOW_LEVEL
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
