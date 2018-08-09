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

/*! \file driver_systick.c
    \brief Encapsulates the ARM sysTick counter, which is used for benchmarking
*/

#include "sensor_fusion.h"
#include "drivers.h"

// SysTick register definitions based on CMSIS definitions for same
#define SYST_CSR    SysTick->CTRL   // SysTick Control & Status Register
#define SYST_RVR    SysTick->LOAD   // SysTick Reload Value Register
#define SYST_CVR    SysTick->VAL    // SysTick Current Value Register

// ARM-core specific function that enables the ARM systick timer on Kinetis uCs.
// this is purely for algorithm benchmarking and can either be deleted or re-written for use on other uCs.
// the systick clock frequency for each uC are defined as CORE_SYSTICK_HZ in types.h.

// the timer is 24 bit so allows measurement of intervals up to 2^24/CORE_SYSTICK_HZ secs=0.35s for a 48MHz uC.
void ARM_systick_enable(void)
{
    SYST_CSR = 0x5u;        // enable systick from internal clock
    SYST_RVR = 0x00FFFFFFu; // set reload to maximum 24 bit value
    return;
}

// ARM-core specific function to store the current systick timer ticks for benchmarking
void ARM_systick_start_ticks(int32 *pstart)
{
    // store the 24 bit systick timer
    *pstart = SYST_CVR & 0x00FFFFFF;

    return;
}

// ARM-core specific function to compute the elapsed systick timer ticks for benchmarking
int32 ARM_systick_elapsed_ticks(int32 start_ticks)
{
	int32 elapsed_ticks;

	// subtract the stored start ticks and check for wraparound down through zero
	elapsed_ticks = start_ticks - (SYST_CVR & 0x00FFFFFF);
	if (elapsed_ticks < 0) elapsed_ticks += SYST_RVR;

	return elapsed_ticks;
}

void ARM_systick_delay_ms(uint32 iSystemCoreClock, uint32 delay_ms)
{
	int32 istart_ticks;	// start ticks on entry
	int32 ielapsed_ticks;	// elapsed ticks
	int16 i;		// loop counter

	// loop for requested number of ms
	for (i = 0; i < delay_ms; i++)
	{
		// loop until 1ms has elapsed
		ARM_systick_start_ticks(&istart_ticks);
		do
		{
			ielapsed_ticks = ARM_systick_elapsed_ticks(istart_ticks);
		} while (ielapsed_ticks < iSystemCoreClock / 1000);
	}

	return;
}
