/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_pit.h"
#include "usb_timer.h"

static usb_timer_callback_t timerCallback;

void PIT0_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, PIT_TFLG_TIF_MASK);
    timerCallback();
   /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping 
     exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U || __CORTEX_M == 7U)
    __DSB();
#endif
}

void PIT_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, PIT_TFLG_TIF_MASK);
    timerCallback();
   /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping 
     exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U || __CORTEX_M == 7U)
    __DSB();
#endif
}

void USB_TimerInit(uint8_t instance, uint32_t interval, uint32_t clock, usb_timer_callback_t callback)
{
    PIT_Type *instanceList[] = PIT_BASE_PTRS;
    IRQn_Type instanceIrq[][FSL_FEATURE_PIT_TIMER_COUNT] = PIT_IRQS;
    /* Structure of initialize PIT */
    pit_config_t pitConfig;

    /*
     * pitConfig.enableRunInDebug = false;
     */
    PIT_GetDefaultConfig(&pitConfig);

    /* Init pit module */
    PIT_Init(instanceList[instance], &pitConfig);

    /* Set timer period for channel 0 */
    PIT_SetTimerPeriod(instanceList[instance], kPIT_Chnl_0, USEC_TO_COUNT(interval, clock));

    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(instanceList[instance], kPIT_Chnl_0, kPIT_TimerInterruptEnable);

    timerCallback = callback;

    /* Enable at the NVIC */
    EnableIRQ(instanceIrq[0][instance]);
}

void USB_TimerInt(uint8_t instance, uint8_t enable)
{
    PIT_Type *instanceList[] = PIT_BASE_PTRS;
    if (enable)
    {
        /* Start counting */
        PIT_StartTimer(instanceList[instance], kPIT_Chnl_0);
    }
    else
    {
        PIT_StopTimer(instanceList[instance], kPIT_Chnl_0);
    }
}
