/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "milliseconds_delay.h"

#define LED0_OFFSET 5
#define LED1_OFFSET 4
#define LED2_OFFSET 1
#define LED0_PORT (PORTB)
#define LED0_GPIO (GPIOB)
#define LED1_PORT (PORTB)
#define LED1_GPIO (GPIOB)
#define LED2_PORT (PORTD)
#define LED2_GPIO (GPIOD)

#define LED0 (1) // USER_LED0
#define LED1 (2) // USER_LED1
#define LED2 (4) // USER_LED2
#define MAX_LED_INDEX (LED2)

static void init_hardware(void)
{
    PCC->CLKCFG[PCC_PORTB_INDEX] = PCC_CLKCFG_CGC(1);
    PCC->CLKCFG[PCC_PORTD_INDEX] = PCC_CLKCFG_CGC(1);

    // Enable the LED pins GPIO
    LED0_PORT->PCR[LED0_OFFSET] = PORT_PCR_MUX(1);
    LED1_PORT->PCR[LED1_OFFSET] = PORT_PCR_MUX(1);
    LED2_PORT->PCR[LED2_OFFSET] = PORT_PCR_MUX(1);

    // Set ports to outputs
    LED0_GPIO->PDDR |= (uint32_t)(1 << LED0_OFFSET);
    LED1_GPIO->PDDR |= (uint32_t)(1 << LED1_OFFSET);
    LED2_GPIO->PDDR |= (uint32_t)(1 << LED2_OFFSET);
}

static void led_toggle(uint32_t leds)
{
    if (leds & LED0)
    {
        LED0_GPIO->PTOR |= (1 << LED0_OFFSET);
    }
    if (leds & LED1)
    {
        LED1_GPIO->PTOR |= (1 << LED1_OFFSET);
    }
    if (leds & LED2)
    {
        LED2_GPIO->PTOR |= (1 << LED2_OFFSET);
    }
}

int main(void)
{
    /* Initialize Hardware */
    init_hardware();
    /* Initialize Timer */
    milliseconds_delay_init();

    /* Run LED blinking demo */
    uint32_t leds = LED0;
    while (1)
    {
        led_toggle(leds);
        milliseconds_delay(1000);
        led_toggle(leds);

        leds <<= 1;
        if (leds > MAX_LED_INDEX)
        {
            leds = LED0;
        }
    }
}
