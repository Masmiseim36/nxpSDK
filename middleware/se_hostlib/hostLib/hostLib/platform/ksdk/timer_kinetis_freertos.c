/*
 * Copyright 2016-2018 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
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
