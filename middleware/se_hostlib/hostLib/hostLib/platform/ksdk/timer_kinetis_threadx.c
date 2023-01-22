/*
 * Copyright 2022 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#if defined(USE_THREADX_RTOS)

#include "board.h"
#include "sm_timer.h"

#include "tx_api.h"

#define CONVERT_MS_TO_TICKS(msec)       ((msec) * (TX_TIMER_TICKS_PER_SECOND) / 1000UL)

void sm_sleep(uint32_t msec)
{
    uint32_t ticks = CONVERT_MS_TO_TICKS(msec);

    /* sleep at least one tick */
    if (ticks == 0)
    {
        ticks = 1;
    }

    tx_thread_sleep(ticks);
}

#endif
