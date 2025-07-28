/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/************************************************************************************
 * Include
 ************************************************************************************/

#include "fsl_common.h"
#include "PWR_Interface.h"
#include "FreeRTOS.h"
#include "task.h"

/************************************************************************************
 * Private type definitions and macros
 ************************************************************************************/

#define TICK_TO_US(tick) (uint64_t)((uint64_t)(tick) * (uint64_t)portTICK_PERIOD_MS * (uint64_t)1000U)
#define US_TO_TICK(us)   (TickType_t)((uint64_t)(us) / ((uint64_t)portTICK_PERIOD_MS * (uint64_t)1000U))

/************************************************************************************
 * Private functions prototypes
 ************************************************************************************/

/************************************************************************************
 * Private memory declarations
 ************************************************************************************/

#if defined(configUSE_TICKLESS_IDLE) && (configUSE_TICKLESS_IDLE > 0)
static uint64_t cumulatedErrorUs = 0U;
#endif

/************************************************************************************
 * Private functions definitions
 ************************************************************************************/

/************************************************************************************
 * Public functions
 ************************************************************************************/

bool PWR_SysticksPreProcess(uint32_t xExpectedIdleTime, uint64_t *expectedIdleTimeUs)
{
#if defined(configUSE_TICKLESS_IDLE) && (configUSE_TICKLESS_IDLE > 0)
    bool             abortIdle = false;
    eSleepModeStatus eSleepStatus;
    TickType_t       cumulatedErrorTick;

    /* Convert cumulated error to ticks */
    cumulatedErrorTick = US_TO_TICK(cumulatedErrorUs);

    if (cumulatedErrorTick > 0U)
    {
        if (cumulatedErrorTick <= xExpectedIdleTime)
        {
            /* In order to recover most of the cumulated error, we increase the
             * FreeRTOS tick count with a number of ticks corresponding to the
             * overall cumulated error */
            vTaskStepTick(cumulatedErrorTick);

            /* Substract the error compensated to the overall error and reduce the
             * expected idle time accordingly */
            cumulatedErrorUs -= TICK_TO_US(cumulatedErrorTick);
            xExpectedIdleTime -= cumulatedErrorTick;
        }
        else
        {
            assert(cumulatedErrorTick > xExpectedIdleTime);

            /* As the expected idle time is less than the error, we cannot
             * recover the whole drift for now, we'll try to do it next time */
            vTaskStepTick(xExpectedIdleTime);
            cumulatedErrorUs -= TICK_TO_US(xExpectedIdleTime);
        }
        /* Whenever the timebase is compensated, abort the current idle loop to let the kernel re-calculate the
         * expected idle time */
        abortIdle = true;
    }

    if (abortIdle == false)
    {
        /* Disable Systicks for tickless mode */
        SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);

        /* Make sure it's still ok to enter low power mode */
        eSleepStatus = eTaskConfirmSleepModeStatus();

        if (eSleepStatus != eAbortSleep)
        {
            if (eSleepStatus == eNoTasksWaitingTimeout)
            {
                /* if no tasks are waiting a timeout, then idle time can be infinite */
                *expectedIdleTimeUs = 0U;
            }
            else
            {
                assert(eSleepStatus == eStandardSleep);

                /* Convert the expected idle time in us for the PWR function, the
                 * sleep mode will not last any longer than this expected idle time */
                *expectedIdleTimeUs = TICK_TO_US(xExpectedIdleTime);
            }
        }
        else
        {
            abortIdle = true;
        }
    }

    /* Abort Idle -> re enable systicks */
    if (abortIdle == true)
    {
        /* Re-enable Systicks before releasing interrupts in case an interrupt fires
         * directly, otherwise we could loose some precision */
        SysTick->CTRL |= (SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
    }

    return abortIdle;
#else
    return true;
#endif
}

void PWR_SysticksPostProcess(uint64_t expectedIdleTimeUs, uint64_t actualIdleTimeUs)
{
#if defined(configUSE_TICKLESS_IDLE) && (configUSE_TICKLESS_IDLE > 0)
    TickType_t idleTimeTick;

    if ((actualIdleTimeUs > expectedIdleTimeUs) && (expectedIdleTimeUs > 0U))
    {
        /* Add the error to be compensated on next low power period */
        cumulatedErrorUs += actualIdleTimeUs - expectedIdleTimeUs;

        /* Fake the idle time to avoid corrupting FreeRTOS timebase
         * On next idle loop, the tick count will be increased by the ticks
         * we missed there, and the next low power period will be reduced */
        actualIdleTimeUs = expectedIdleTimeUs;

        idleTimeTick = US_TO_TICK(actualIdleTimeUs);
    }
    else
    {
        /* Convert us to ticks */
        idleTimeTick = US_TO_TICK(actualIdleTimeUs);

        /* Compute the error due to discarded LSB */
        cumulatedErrorUs += actualIdleTimeUs - TICK_TO_US(idleTimeTick);
    }

    /* Update the OS time ticks. */
    vTaskStepTick(idleTimeTick);

    /* Re-enable Systicks before releasing interrupts in case an interrupt fires
     * directly, otherwise we could loose some precision */
    SysTick->CTRL |= (SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
#endif
}
