/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
 * @file  systick_utils.c
 * @brief Encapsulates the ARM sysTick counter, which is used for computing delays.
 *         ARM-core specific function that enables the ARM systick timer on Kinetis uCs.
 *         the timer is 24 bit so allows measurement of intervals up to 2^24/CORE_SYSTICK_HZ secs=0.35s for a 48MHz uC.
*/

#include "issdk_hal.h"

// SysTick register definitions based on CMSIS definitions.
#define SYST_CSR SysTick->CTRL // SysTick Control & Status Register
#define SYST_RVR SysTick->LOAD // SysTick Reload Value Register
#define SYST_CVR SysTick->VAL  // SysTick Current Value Register

uint32_t g_ovf_stamp;
volatile uint32_t g_ovf_counter = 0;

#if !defined(FSL_RTOS_FREE_RTOS) && !defined(ISSDK_USE_BLE_STACK)
// SDK specific SysTick Interrupt Handler
void SysTick_Handler(void)
{
    g_ovf_counter += 1;
}
#endif

uint32_t SysTick_GetOVFCount(void)
{
#ifdef ISSDK_USE_BLE_STACK
    return OSA_TimeGetMsec();
#else
    return g_ovf_counter;
#endif
}

// ARM-core specific function to enable systicks.
void BOARD_SystickEnable(void)
{
    SYST_CSR = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk |
               SysTick_CTRL_ENABLE_Msk; // Enable systick from internal clock with Interrupts.
    SYST_RVR = 0x00FFFFFFu;             // Set reload to maximum 24 bit value.
    return;
}

// ARM-core specific function to store the current systick timer ticks.
void BOARD_SystickStart(int32_t *pStart)
{
    // Store the 24 bit systick timer.
    g_ovf_stamp = SysTick_GetOVFCount();
    *pStart = SYST_CVR & 0x00FFFFFF;
}

// ARM-core specific function to compute the elapsed systick timer ticks.
int32_t BOARD_SystickElapsedTicks(int32_t *pStart)
{
    int32_t elapsed;

    // Subtract the stored start ticks and check for wraparound down through zero.
    elapsed = *pStart - (SYST_CVR & 0x00FFFFFF);
    elapsed += SYST_RVR * (SysTick_GetOVFCount() - g_ovf_stamp);

    return elapsed;
}

// ARM-core specific function to compute the elapsed time in micro seconds.
uint32_t BOARD_SystickElapsedTime_us(int32_t *pStart)
{
    uint32_t time_us, elapsed;
    uint32_t systemCoreClock;

    elapsed = BOARD_SystickElapsedTicks(pStart);
    systemCoreClock = CLOCK_GetFreq(kCLOCK_CoreSysClk);

    time_us = COUNT_TO_USEC(elapsed, systemCoreClock);

    // Update the 24 bit systick timer.
    BOARD_SystickStart(pStart);

    return time_us;
}

// ARM-core specific function to insert delays in milli seconds.
void BOARD_DELAY_ms(uint32_t delay_ms)
{
    int32_t start, elapsed;
    uint32_t systemCoreClock;

    BOARD_SystickStart(&start);
    do // Loop for requested number of ms.
    {
        elapsed = BOARD_SystickElapsedTicks(&start);
        systemCoreClock = CLOCK_GetFreq(kCLOCK_CoreSysClk);
    } while(COUNT_TO_MSEC(elapsed, systemCoreClock) < delay_ms);
}
