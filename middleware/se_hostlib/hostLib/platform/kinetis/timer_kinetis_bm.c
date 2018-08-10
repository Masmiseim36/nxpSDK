/*
 * Copyright(C) NXP Semiconductors, 2016
 * All rights reserved.
 *
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * A7-series security ICs.  This software is supplied "AS IS" without any
 * warranties of any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * Permission to use, copy and modify this software is hereby granted,
 * under NXP Semiconductors' and its licensor's relevant copyrights in
 * the software, without fee, provided that it is used in conjunction with
 * NXP Semiconductors products. This copyright, permission, and disclaimer notice
 * must appear in all copies of this code.
 */

#include <sm_timer.h>
#include <stdint.h>

#include "board.h"

#if !defined(SDK_OS_FREE_RTOS) && !defined(FSL_RTOS_FREE_RTOS)

extern volatile uint32_t gtimer_kinetis_msticks; // counter for 1ms SysTicks

//__INLINE
static void systick_delay(const uint32_t delayTicks) {
    uint32_t currentTicks;
    assert(delayTicks < 0x7FFFFFFFu);

    __disable_irq();

    if ((gtimer_kinetis_msticks) & 0x80000000u)
    {
        /* gtimer_kinetis_msticks has increased drastically (MSB is set),
         * So, reset gtimer_kinetis_msticks before it's too late to detect an
         * overflow. */
        gtimer_kinetis_msticks = 0;
    }

    currentTicks = gtimer_kinetis_msticks;   // read current tick counter

    __DSB();
    __enable_irq();

    // Now loop until required number of ticks passes
    while ((gtimer_kinetis_msticks - currentTicks) <= delayTicks) {
#ifdef __WFI
        __WFI();
#endif
    }
}

/* interrupt handler for system ticks */
void SysTick_Handler(void) {
    gtimer_kinetis_msticks++;
}


/* initializes the system tick counter
 * return 0 on succes, 1 on failure */
uint32_t sm_initSleep() {
    gtimer_kinetis_msticks = 0;
    SysTick_Config(SystemCoreClock / 1000);
    __enable_irq();
    return 0;
}

/**
 * Implement a blocking (for the calling thread) wait for a number of milliseconds.
 */
void sm_sleep(uint32_t msec) {
    systick_delay(msec);
}

#endif /* !SDK_OS_FREE_RTOS && ! FSL_RTOS_FREE_RTOS */
