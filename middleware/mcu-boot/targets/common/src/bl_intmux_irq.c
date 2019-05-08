/*
 * Copyright (c) 2014-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include <stdbool.h>

/*******************************************************************************
 * Code
 ******************************************************************************/

static INTMUX_Type *g_IntmuxBases[] = INTMUX_BASE_PTRS;

static inline bool INTMUX_IsChannelPending(uint32_t instance, uint32_t channel)
{
    return (g_IntmuxBases[instance]->CHANNEL[channel].CHn_IPR_31_0);
}

static inline uint32_t INTMUX_getVectorNumber(uint32_t instance, uint32_t channel)
{
    return (g_IntmuxBases[instance]->CHANNEL[channel].CHn_VEC);
}

static void INTMUX_irq_handler(uint32_t instance, uint32_t channel)
{
    if (INTMUX_IsChannelPending(instance, channel))
    {
        uint32_t vectorNumber = INTMUX_getVectorNumber(instance, channel);
        if (vectorNumber)
        {
            uint32_t *vectorTable = (uint32_t *)(SCB->VTOR + vectorNumber);
            uint32_t fnAddress = *vectorTable;
            void (*activeVectorIsr)(void) = (void (*)(void))fnAddress;
            activeVectorIsr();
        }
    }
}

void INTMUX0_0_IRQHandler(void)
{
    INTMUX_irq_handler(0, 0);
}

void INTMUX0_1_IRQHandler(void)
{
    INTMUX_irq_handler(0, 1);
}

void INTMUX0_2_IRQHandler(void)
{
    INTMUX_irq_handler(0, 2);
}

void INTMUX0_3_IRQHandler(void)
{
    INTMUX_irq_handler(0, 3);
}

#if (FSL_FEATURE_SOC_INTMUX_COUNT > 1)
void INTMUX1_0_IRQHandler(void)
{
    INTMUX_irq_handler(1, 0);
}

void INTMUX1_1_IRQHandler(void)
{
    INTMUX_irq_handler(1, 1);
}

void INTMUX1_2_IRQHandler(void)
{
    INTMUX_irq_handler(1, 2);
}

void INTMUX1_3_IRQHandler(void)
{
    INTMUX_irq_handler(1, 3);
}
#endif // (FSL_FEATURE_SOC_INTMUX_COUNT > 1)
