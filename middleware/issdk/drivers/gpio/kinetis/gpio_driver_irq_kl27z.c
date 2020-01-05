/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file gpio_driver_irq_kl27z.c
 * @brief The gpio_driver_irq_kl27z.c file containes the Generic IRQ implmentation for the FRDM-KL27Z GPIO.
*/

#include "fsl_gpio.h"
#include "frdm_kl27z.h"

extern void ksdk_gpio_handle_interrupt(GPIO_Type *base, port_number_t portNumber);

/*******************************************************************************
 * Functions - GPIOIRQ implementation
 ******************************************************************/
void PORTA_IRQHandler(void)
{
    ksdk_gpio_handle_interrupt(GPIOA, PORTA_NUM);
}
void PORTB_PORTC_PORTD_PORTE_IRQHandler(void)
{
    ksdk_gpio_handle_interrupt(GPIO_TYPE, GPIO_PORT_NUM);
}
