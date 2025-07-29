/*
 * Copyright 2014-2016 Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* Compiler includes. */
#if defined(__ICCARM__)
#include <intrinsics.h>
#endif

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

#if configUSE_TICKLESS_IDLE == 2
#include "fsl_ostimer.h"
#else
#include "fsl_device_registers.h"
#endif

#include "fsl_tickless_ostimer.h"

extern uint32_t SystemCoreClock; /* in Kinetis SDK, this contains the system core clock speed */

static uint32_t ulStoppedTimerCompensation = 0;

/*
 * LPT timer base address and interrupt number
 */
#if configUSE_TICKLESS_IDLE == 2
extern OSTIMER_Type *vPortGetOstimerBase(void);
extern IRQn_Type vPortGetOstimerIrqn(void);
#endif /* configUSE_TICKLESS_IDLE */

/*
 * The number of SysTick increments that make up one tick period.
 */
#if configUSE_TICKLESS_IDLE == 2
static uint32_t ulTimerCountsForOneTick = 0;
#endif /* configUSE_TICKLESS_IDLE */

/*
 * The maximum number of tick periods that can be suppressed is limited by the
 * 24 bit resolution of the SysTick timer.
 */
#if configUSE_TICKLESS_IDLE == 2
static uint32_t xMaximumPossibleSuppressedTicks = 0;
#endif /* configUSE_TICKLESS_IDLE */

/*
 * The number of LPTIMER increments that make up one tick period.
 */
#if configUSE_TICKLESS_IDLE == 2
static uint32_t ulLPTimerCountsForOneTick = 0;
#endif /* configUSE_TICKLESS_IDLE */

/*
 * The flag of LPTIMER is occurs or not.
 */
#if configUSE_TICKLESS_IDLE == 2
static volatile bool ulLPTimerInterruptFired = false;
#endif /* configUSE_TICKLESS_IDLE */

#if configUSE_TICKLESS_IDLE == 2

void OstimerCallback(void)
{
    ulLPTimerInterruptFired = true;
    OSTIMER_ClearStatusFlags(vPortGetOstimerBase(), kOSTIMER_MatchInterruptFlag);
}

void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
    uint32_t ulReloadValue, ulCompleteTickPeriods;
    TickType_t xModifiableIdleTime;
    OSTIMER_Type *pxOstimerBase;
    uint64_t xOstimerStartValue = 0;
    uint64_t xOstimerCurrentValue = 0;
    uint32_t ulTimerCountsElapsed = 0;
    bool timerExpired = false;

    pxOstimerBase = vPortGetOstimerBase();
    if (pxOstimerBase == 0 || xExpectedIdleTime <= 0)
        return;

    /* Make sure the timer period doesn't overflow the counter. */
    if (xExpectedIdleTime > xMaximumPossibleSuppressedTicks)
    {
        xExpectedIdleTime = xMaximumPossibleSuppressedTicks;
    }

    /* Calculate the reload value required to wait xExpectedIdleTime
     * tick periods. -1 is used because this code will execute part way
     * through one of the tick periods. */
    ulReloadValue = ulLPTimerCountsForOneTick * (xExpectedIdleTime - 1UL);

    /* Compensate for timer startup/shutdown latency */
    if (ulReloadValue > ulStoppedTimerCompensation)
    {
        ulReloadValue -= ulStoppedTimerCompensation;
    }

    /* Stop the SysTick momentarily. */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

    /* Enter a critical section but don't use the taskENTER_CRITICAL()
     * method as that will mask interrupts that should exit sleep mode. */
    __disable_irq();
    __DSB();
    __ISB();

    /* If a context switch is pending or a task is waiting for the scheduler
     * to be unsuspended then abandon the low power entry. */
    if (eTaskConfirmSleepModeStatus() == eAbortSleep)
    {
        /* Restart from whatever is left in the count register to complete
         * this tick period. */
        SysTick->LOAD = SysTick->VAL;

        /* Restart SysTick. */
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

        /* Reset the reload register to the value required for normal tick
         * periods. */
        SysTick->LOAD = ulTimerCountsForOneTick - 1UL;

        /* Re-enable interrupts */
        __enable_irq();
    }
    else
    {
        /* Initialize the OSTIMER for this sleep period */
        OSTIMER_Init(pxOstimerBase);
        OSTIMER_ClearStatusFlags(pxOstimerBase, kOSTIMER_MatchInterruptFlag);

        /* Set up the interrupt flag */
        ulLPTimerInterruptFired = false;

        /* Get current timer value and set match for the future */
        xOstimerStartValue = OSTIMER_GetCurrentTimerValue(pxOstimerBase);
        OSTIMER_SetMatchValue(pxOstimerBase, (xOstimerStartValue + ulReloadValue), OstimerCallback);

        /* Sleep until something happens. */
        xModifiableIdleTime = xExpectedIdleTime;
        configPRE_SLEEP_PROCESSING(xModifiableIdleTime);

        if (xModifiableIdleTime > 0)
        {
            __DSB();
            __WFI();
            __ISB();
        }

        configPOST_SLEEP_PROCESSING(xExpectedIdleTime);

        /* Re-enable interrupts to allow the interrupt that brought the MCU
         * out of sleep mode to execute immediately. */
        __enable_irq();
        __DSB();
        __ISB();

        /* Capture the current time after waking */
        xOstimerCurrentValue = OSTIMER_GetCurrentTimerValue(pxOstimerBase);

        /* Check if the timer interrupt fired */
        timerExpired = ulLPTimerInterruptFired;
        ulLPTimerInterruptFired = false;

        /* Disable interrupts again while we update the tick count */
        __disable_irq();

        if (timerExpired)
        {
            /* The timer expired, so we know exactly how many ticks passed.
             * As the pending tick will be processed as soon as this
             * function exits, the tick value maintained by the tick is
             * stepped forward by one less than the time spent waiting. */
            ulCompleteTickPeriods = xExpectedIdleTime - 1UL;
        }
        else
        {
            /* Something other than the timer ended the sleep.
             * Work out how long the sleep lasted in timer counts, then
             * convert to ticks to calculate how many tick periods passed during sleep. */
            ulTimerCountsElapsed = (uint32_t)(xOstimerCurrentValue - xOstimerStartValue);

            /* How many complete tick periods passed while the processor was sleeping? */
            ulCompleteTickPeriods = ulTimerCountsElapsed / ulLPTimerCountsForOneTick;

            /* Limit to the maximum expected value as a safety check.
             * This also handles potential timer wraparound issues. */
            if (ulCompleteTickPeriods > (xExpectedIdleTime - 1))
            {
                ulCompleteTickPeriods = xExpectedIdleTime - 1;
            }
        }

        /* Stop OSTIMER to avoid further interrupts */
        OSTIMER_Deinit(pxOstimerBase);

        /* Restart SysTick for normal operation */
        portENTER_CRITICAL();
        {
            /* Restart SysTick */
            SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

            /* Step the tick forward by the number of tick periods that completed */
            vTaskStepTick(ulCompleteTickPeriods);

            /* Reset SysTick to handle the next tick */
            SysTick->LOAD = ulTimerCountsForOneTick - 1UL;
        }
        portEXIT_CRITICAL();

        /* Re-enable interrupts before exiting */
        __enable_irq();
    }
}

/*
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
void vPortSetupTimerInterrupt(void)
{
    /* Calculate the constants required to configure the tick interrupt. */
    ulTimerCountsForOneTick = (configCPU_CLOCK_HZ / configTICK_RATE_HZ);
    ulLPTimerCountsForOneTick = (configOSTIMER_CLOCK_HZ / configTICK_RATE_HZ);

    /* Calculate the timer compensation factor */
    ulStoppedTimerCompensation = 45UL * (configCPU_CLOCK_HZ / configOSTIMER_CLOCK_HZ);

    if (ulLPTimerCountsForOneTick != 0)
    {
        /* Calculate the maximum number of ticks we can suppress based on timer resolution */
        xMaximumPossibleSuppressedTicks = portMAX_32_BIT_NUMBER / ulLPTimerCountsForOneTick;
    }
    else
    {
        /* ulLPTimerCountsForOneTick is zero, not allowed state */
        while (1)
            ;
    }

    /* Enable the OSTIMER interrupt in NVIC */
    NVIC_EnableIRQ(vPortGetOstimerIrqn());

    /* Configure SysTick to interrupt at the requested rate. */
    SysTick->LOAD = ulTimerCountsForOneTick - 1UL;
    SysTick->VAL = 0UL;
    SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
}
#endif /* configUSE_TICKLESS_IDLE */
