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


#if defined(SDK_OS_FREE_RTOS) || defined(FSL_RTOS_FREE_RTOS)

#include "FreeRTOS.h"
#include "task.h"

extern volatile uint32_t gtimer_kinetis_msticks; // counter for 1ms SysTicks


/* initializes the system tick counter
 * return 0 on succes, 1 on failure */
uint32_t sm_initSleep() {
    return 0;
}

/**
 * Implement a blocking (for the calling thread) wait for a number of milliseconds.
 */
void sm_sleep(uint32_t msec) {
    vTaskDelay(msec);
}

void vApplicationTickHook() {
    gtimer_kinetis_msticks++;
}

#endif /* SDK_OS_FREE_RTOS || FSL_RTOS_FREE_RTOS */
