/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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

#include "fsl_device_registers.h"
#include "fsl_epit.h"
#include "usb_timer.h"
#include "fsl_gpc.h"

static usb_timer_callback_t timerCallback;

void EPIT1_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    EPIT_ClearStatusFlags(EPIT1, kEPIT_OutputCompareFlag);
    timerCallback();
}

void USB_TimerInit(uint8_t instance, uint32_t interval, uint32_t clock, usb_timer_callback_t callback)
{
    EPIT_Type *instanceList[] = EPIT_BASE_PTRS;
    IRQn_Type instanceIrq[] = EPIT_IRQS;
    /* Structure of initialize PIT */
    epit_config_t epitConfig;

    /*
     * lpitConfig.enableRunInDebug = false;
     * lpitConfig.enableRunInDoze = false;
     */
    EPIT_GetDefaultConfig(&epitConfig);
    epitConfig.clockSource = kEPIT_ClockSource_LowFreq; /* 32K RTC OSC */

    /* Init lpit module */
    EPIT_Init(instanceList[instance], &epitConfig);

    EPIT_SetOutputCompareValue(EPIT1, 0);

    /* Set timer period for channel 0 */
    EPIT_SetTimerPeriod(instanceList[instance], 31U); // Set the interrupt frequency to 1ms

    /* Enable timer interrupts for channel 0 */
    EPIT_EnableInterrupts(instanceList[instance], kEPIT_OutputCompareInterruptEnable);

    timerCallback = callback;

    /* Enable at the NVIC */
    EnableIRQ(instanceIrq[instance]);
}

void USB_TimerInt(uint8_t instance, uint8_t enable)
{
    EPIT_Type *instanceList[] = EPIT_BASE_PTRS;
    if (enable)
    {
        /* Start counting */
        EPIT_StartTimer(instanceList[instance]);
    }
    else
    {
        EPIT_StopTimer(instanceList[instance]);
    }
}
