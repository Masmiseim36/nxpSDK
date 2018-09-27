/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_gpio.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Array of GPIO peripheral base address. */
static GPIO_Type *const s_gpioBases[] = GPIO_BASE_PTRS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/* Array of GPIO clock name. */
static const clock_ip_name_t s_gpioClock[] = GPIO_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
* Prototypes
******************************************************************************/

/*!
* @brief Gets the GPIO instance according to the GPIO base
*
* @param base    GPIO peripheral base pointer(PTA, PTB, PTC, etc.)
* @retval GPIO instance
*/
static uint32_t GPIO_GetInstance(GPIO_Type *base);

/*******************************************************************************
 * Code
 ******************************************************************************/

static uint32_t GPIO_GetInstance(GPIO_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_gpioBases); instance++)
    {
        if (s_gpioBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < ARRAY_SIZE(s_gpioBases));

    return instance;
}

void GPIO_PinInit(GPIO_Type *base, uint32_t pin, const gpio_pin_config_t *Config)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable GPIO clock. */
    CLOCK_EnableClock(s_gpioClock[GPIO_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    /* Register reset to default value */
    base->IMR &= ~(1U << pin);

    /* Configure GPIO pin direction */
    if (Config->direction == kGPIO_DigitalInput)
    {
        base->GDIR &= ~(1U << pin);
    }
    else
    {
        GPIO_WritePinOutput(base, pin, Config->outputLogic);
        base->GDIR |= (1U << pin);
    }

    /* Configure GPIO pin interrupt mode */
    GPIO_SetPinInterruptConfig(base, pin, Config->interruptMode);
}

void GPIO_WritePinOutput(GPIO_Type *base, uint32_t pin, uint8_t output)
{
    assert(pin < 32);
    if (output == 0U)
    {
        base->DR &= ~(1U << pin); /* Set pin output to low level.*/
    }
    else
    {
        base->DR |= (1U << pin); /* Set pin output to high level.*/
    }
}

void GPIO_SetPinInterruptConfig(GPIO_Type *base, uint32_t pin, gpio_interrupt_mode_t pinInterruptMode)
{
    volatile uint32_t *icr;
    uint32_t icrShift;

    icrShift = pin;

    /* Register reset to default value */
    base->EDGE_SEL &= ~(1U << pin);

    if (pin < 16)
    {
        icr = &(base->ICR1);
    }
    else
    {
        icr = &(base->ICR2);
        icrShift -= 16;
    }
    switch (pinInterruptMode)
    {
        case (kGPIO_IntLowLevel):
            *icr &= ~(3U << (2 * icrShift));
            break;
        case (kGPIO_IntHighLevel):
            *icr = (*icr & (~(3U << (2 * icrShift)))) | (1U << (2 * icrShift));
            break;
        case (kGPIO_IntRisingEdge):
            *icr = (*icr & (~(3U << (2 * icrShift)))) | (2U << (2 * icrShift));
            break;
        case (kGPIO_IntFallingEdge):
            *icr |= (3U << (2 * icrShift));
            break;
        case (kGPIO_IntRisingOrFallingEdge):
            base->EDGE_SEL |= (1U << pin);
            break;
        default:
            break;
    }
}
