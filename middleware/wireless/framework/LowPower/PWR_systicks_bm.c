/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/************************************************************************************
 * Include
 ************************************************************************************/

#include "fsl_common.h"
#include "fsl_pm_core.h"
#include "PWR_Interface.h"
#include "fsl_os_abstraction.h"
#include "fwk_platform_lowpower_timer.h"
#if (FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE)
/************************************************************************************
 * Private type definitions and macros
 ************************************************************************************/

/************************************************************************************
 * Private functions prototypes
 ************************************************************************************/
static status_t PWR_SysTicksLowPowerCallback(pm_event_type_t eventType, uint8_t powerState, void *data);
/************************************************************************************
 * Private memory declarations
 ************************************************************************************/
static uint64_t accumulatedError;
static uint64_t enterLowPowerTimestampMicroseconds;

static pm_notify_element_t pwrSysTicksLpNotifyGroup = {
    .notifyCallback = PWR_SysTicksLowPowerCallback,
    .data           = NULL,
};
/************************************************************************************
 * Private functions definitions
 ************************************************************************************/

/************************************************************************************
 * Public functions
 ************************************************************************************/
bool PWR_SysticksPreProcess(uint32_t xExpectedIdleTime, uint64_t *expectedIdleTimeUs)
{
    (void)xExpectedIdleTime;
    (void)expectedIdleTimeUs;

    OSA_DisableIRQGlobal();

    /* Disable Systicks */
    SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);

    if (SCB->ICSR & SCB_ICSR_PENDSTSET_Msk)
    {
        /*
         * If SYSTick interrupt is pending, then increment the OSA tick here
         * by 1. We clear the interrupt below, thus missing 1 tick, which could
         * be a potential source for drift.
         */
        OSA_UpdateSysTickCounter(1);
    }

    /* Clear SysTick Pending Interrupt in ICSR (write 1b to clear, reads 0) */
    SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk;

    enterLowPowerTimestampMicroseconds = PLATFORM_GetLowPowerTimestampUs();

    OSA_EnableIRQGlobal();

    return true;
}

void PWR_SysticksPostProcess(uint64_t expectedIdleTimeUs, uint64_t actualIdleTimeUs)
{
    uint64_t        sleepDurationMicroseconds, sleepCount, sysTickVirtualInterrupts;
    const uint32_t  sysTickReloadValue = SysTick->LOAD;
    extern uint32_t SystemCoreClock;

    (void)expectedIdleTimeUs;
    (void)actualIdleTimeUs;

    OSA_DisableIRQGlobal();

    sleepDurationMicroseconds =
        PLATFORM_GetLowPowerDurationUs(enterLowPowerTimestampMicroseconds, PLATFORM_GetLowPowerTimestampUs());

    /* Convert the sleep duration to CPU ticks (src for SysTick) */
    sleepCount = USEC_TO_COUNT(sleepDurationMicroseconds, SystemCoreClock);

    /*
     * How many times would the SysTick interrupt tick when running for
     * u64SleepDurationUsec?
     */
    sysTickVirtualInterrupts = (uint64_t)(accumulatedError + sleepCount) / (uint64_t)sysTickReloadValue;

    if (sysTickVirtualInterrupts != 0)
    {
        /*
         * Update the OSA ticks as if u64SysTickVirtInts interrupts would have
         * occurred.
         */
        OSA_UpdateSysTickCounter(sysTickVirtualInterrupts);

        /*
         * Recalculate the accumulated error; remember that this needs to be
         * passed over for the next sleep cycle.
         */
        accumulatedError =
            (uint64_t)(accumulatedError + sleepCount) - sysTickVirtualInterrupts * (uint64_t)sysTickReloadValue;
    }
    else
    {
        /*
         * Just accumulate the error here, the OSA ticks can't be updated
         * yet.
         */
        accumulatedError += sleepCount;
    }

    /*
     * Re-enable ints here: if the SysTick INT is about to fire, it will
     * fire right after re-enabling the SysTick count.
     */
    OSA_EnableIRQGlobal();

    SysTick->CTRL |= (SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
}

void PWR_SysTicksLowPowerInit(void)
{
    status_t status;
    status = PM_RegisterNotify(kPM_NotifyGroup2, &pwrSysTicksLpNotifyGroup);
    assert(status == kStatus_Success);
    (void)status;
}

static status_t PWR_SysTicksLowPowerCallback(pm_event_type_t eventType, uint8_t powerState, void *data)
{
    status_t ret = kStatus_Success;
    if (eventType == kPM_EventEnteringSleep)
    {
        PWR_SysticksPreProcess(0, 0);
    }
    else
    {
        PWR_SysticksPostProcess(0, 0);
    }
    return ret;
}
#endif /* FSL_OSA_BM_TIMER_CONFIG != FSL_OSA_BM_TIMER_NONE */
