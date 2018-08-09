/*
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "main.h"
#include "timer.h"

volatile uint16_t cycle_counter;

#if defined(CPU_MKW24D512VHA5) || defined(MCU_MK21DX256)
    #define full_range 356 /* (0xFFFFFF / 48000000 * 1000) */
#elif defined(MCU_MKW40Z160) || defined(CPU_MKW41Z512VHT4)
    #define full_range 524 /* (0xFFFFFF / 32000000 * 1000) */
#else
    #define full_range 800 /* (0xFFFFFF / 20971520 * 1000) */
#endif

void StartTimer()
{
    /* make sure we dont count while before resetting the values */
    StopTimer();
    /* set maximum range */
    SysTick->LOAD = 0xFFFFFF;
    /* reset cycle counter */
    cycle_counter = 0;
    /* reset counter */
    SysTick->VAL = 0;
    /* enable the module and start counting */
    SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
}
void StopTimer()
{
    /* disable the module */
    SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);
}
/* returns elapsed miliseconds */
uint32_t GetElapsedTime()
{
    return cycle_counter * full_range + ((10000 - ((unsigned long long)SysTick->VAL * 10000 / 0xFFFFFF)) / (10000 / full_range));
}
void SysTickHandler(void)
{
    cycle_counter++;
}
