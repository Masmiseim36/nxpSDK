/*
 * The Clear BSD License
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

/*! \file driver_pit.c
    \brief Provides a simple abstraction for a periodic interval timer.

   Bare metal implementations of the sensor fusion library require at least
   one periodic interrupt for use as a timebase for sensor fusion functions.
   The Periodic Interval Timer (PIT) is one such module that is commonly
   found on NXP Kinetis MCUs.  The PIT functions are only referenced at the
   main() level.  There is no interaction within the fusion routines themselves.
*/
#include "issdk_hal.h"
#include "board.h"
#include "fsl_pit.h"
#include "pin_mux.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef PIT_LED_HANDLER
#define PIT_LED_HANDLER     PIT0_IRQHandler
#endif
#ifndef PIT_IRQ_ID
#define PIT_IRQ_ID          PIT0_IRQn
#endif
#define PIT_SOURCE_CLOCK    CLOCK_GetFreq(kCLOCK_BusClk)

volatile bool   pitIsrFlag = false;

void PIT_LED_HANDLER(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, PIT_TFLG_TIF_MASK);
    pitIsrFlag = true;
}

void pit_init(uint32_t microseconds)
{
    /* initialize PIT */
    pit_config_t    pitConfig;                  // Declare KSDK PIT configuration structure
    PIT_GetDefaultConfig(&pitConfig);           // Fill out that structure with defaults
    PIT_Init(PIT, &pitConfig);                  // PIT is declared by the KSDK
    // We choose to use Channel 0 of the PIT.  That can obviously be changed
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(microseconds, PIT_SOURCE_CLOCK));
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
    EnableIRQ(PIT_IRQ_ID);
    PIT_StartTimer(PIT, kPIT_Chnl_0);
}
