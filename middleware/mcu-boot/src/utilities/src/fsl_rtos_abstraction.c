/*
 * Copyright (c) 2013 Freescale Semiconductor, Inc.
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "utilities/fsl_assert.h"
#include "fsl_rtos_abstraction.h"
#include "fsl_os_abstraction.h"
#include "fsl_device_registers.h"
#include "microseconds.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

enum _sync_constants
{
    kSyncUnlocked = 0,
    kSyncLocked = 1
};

OSA_SR_ALLOC();

static uint64_t s_ticksPerMs = 0;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

/*FUNCTION**********************************************************************
 *
 * Function Name : time_diff
 * Description   : This function gets the difference between two time stamp,
 * time overflow is considered.
 *
 *END**************************************************************************/
static uint32_t ms_diff(uint64_t tickStart, uint64_t tickStop)
{
    return ((tickStop - tickStart) / s_ticksPerMs);
}


void sync_init(sync_object_t *obj, bool state)
{
    *obj = state ? kSyncLocked : kSyncUnlocked;
}

bool sync_wait(sync_object_t *obj, uint32_t timeout)
{
    // Increment the object so we can tell if it changes. Because the increment is not
    // atomic (load, add, store), we must disabled interrupts during it.
    OSA_ENTER_CRITICAL();
    ++(*obj);
    OSA_EXIT_CRITICAL();

    // Wait for the object to be unlocked.
    while (*obj != 0)
    {
        // Spin.
    }

    return true;
}

void sync_signal(sync_object_t *obj)
{
    // Atomically unlock the object.
    OSA_ENTER_CRITICAL();
    --(*obj);
    OSA_EXIT_CRITICAL();
}

void sync_reset(sync_object_t *obj)
{
    OSA_ENTER_CRITICAL();
    (*obj) = 0;
    OSA_EXIT_CRITICAL();
}

void lock_init(void)
{
}

void lock_acquire(void)
{
    OSA_ENTER_CRITICAL();
}

void lock_release(void)
{
    OSA_EXIT_CRITICAL();
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
