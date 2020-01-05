/*
 * Copyright (c) 2017, Freescale Semiconductor, Inc.
 * Copyright 2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file gpio_driver_irq.c
 * @brief The gpio_driver_irq.c file containes the Generic IRQ implmentation for GPIO.
*/

#include "fsl_gint.h"

extern void issdk_gpio_handle_interrupt(GINT_Type *base, gint_port_t portNumber);

/*******************************************************************************
 * Functions - GPIOIRQ implementation
 ******************************************************************/
#if defined(GINT0)
void PORT0_IRQHandler(void)
{
    issdk_gpio_handle_interrupt(GINT0, kGINT_Port0);
}
#endif
#if defined(GINT1)
void PORT1_IRQHandler(void)
{
    issdk_gpio_handle_interrupt(GINT1, kGINT_Port1);
}
#endif
#if defined(GINT2)
void PORT2_IRQHandler(void)
{
    issdk_gpio_handle_interrupt(GINT2, kGINT_Port2);
}
#endif
#if defined(GINT3)
void PORT3_IRQHandler(void)
{
    issdk_gpio_handle_interrupt(GINT3, kGINT_Port3);
}
#endif
#if defined(GINT4)
void PORT4_IRQHandler(void)
{
    issdk_gpio_handle_interrupt(GINT4, kGINT_Port4);
}
#endif
#if defined(GINT5)
void PORT5_IRQHandler(void)
{
    issdk_gpio_handle_interrupt(GINT5, kGINT_Port5);
}
#endif
#if defined(GINT6)
void PORT6_IRQHandler(void)
{
    issdk_gpio_handle_interrupt(GINT6, kGINT_Port6);
}
#endif
#if defined(GINT7)
void PORT7_IRQHandler(void)
{
    issdk_gpio_handle_interrupt(GINT7, kGINT_Port7);
}
#endif
