/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"
#include "tickless_api.h"

#if configUSE_TICKLESS_IDLE == 2

/* Static variables for tickless implementation */
static uint32_t ulTimerCountsForOneTick         = 0;
static uint32_t xMaximumPossibleSuppressedTicks = 0;
static uint32_t ulTimerCountsForOneSec          = 0;
static uint32_t ulLPTimerCountsForOneTick       = 0;
static volatile bool ulTimerInterruptFired      = false;

void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
    const tickless_timer_hal_t *timer_hal = tickless_get_timer_hal();
    void *timer_base                      = timer_hal->get_timer_base();
    uint32_t ulReloadValue, ulCompleteTickPeriods;
    TickType_t xModifiableIdleTime;

    /* Make sure the timer period doesn't overflow the counter */
    if (xExpectedIdleTime > xMaximumPossibleSuppressedTicks)
    {
        xExpectedIdleTime = xMaximumPossibleSuppressedTicks;
    }

    if (xExpectedIdleTime <= 0)
    {
        return;
    }

    /**
     * Calculate the reload value required to wait xExpectedIdleTime
     * tick periods.  -1 is used because this code will execute part way
     * through one of the tick periods.
     */
    if ((xExpectedIdleTime > 1UL) && (ulLPTimerCountsForOneTick > (UINT32_MAX / (xExpectedIdleTime - 1UL))))
    {
        /* Would overflow, cap at maximum */
        ulReloadValue = UINT32_MAX;
    }
    else
    {
        ulReloadValue = ulLPTimerCountsForOneTick * (xExpectedIdleTime - 1UL);
    }

    /**
     * Stop the timer and systick momentarily. The time the time and systick is stopped for
     * is accounted for as best it can be, but using the tickless mode will
     * inevitably result in some tiny drift of the time maintained by the
     * kernel with respect to calendar time.
     */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    timer_hal->stop_timer(timer_base);

    /**
     * Enter a critical section but don't use the taskENTER_CRITICAL()
     * method as that will mask interrupts that should exit sleep mode.
     */
    __disable_irq();
    __DSB();
    __ISB();

    /**
     * If a context switch is pending or a task is waiting for the scheduler
     * to be unsuspended then abandon the low power entry.
     */
    if (eTaskConfirmSleepModeStatus() == eAbortSleep)
    {
        /**
         * Restart from whatever is left in the count register to complete
         * this tick period.
         */
        SysTick->LOAD = SysTick->VAL;

        /* Restart SysTick. */
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

        /**
         * Reset the reload register to the value required for normal tick
         * periods.
         */
        SysTick->LOAD = ulTimerCountsForOneTick - 1UL;

        /**
         * Re-enable interrupts - see comments above __disable_irq()
         * call above. \
         */
        __enable_irq();
    }
    else
    {
        /* Configure and start timer */
        timer_hal->clear_status_flags(timer_base);
        timer_hal->set_timer_period(timer_base, ulReloadValue);
        timer_hal->start_timer(timer_base);

        /**
         * Sleep until something happens.  configPRE_SLEEP_PROCESSING() can
         * set its parameter to 0 to indicate that its implementation contains
         * its own wait for interrupt or wait for event instruction, and so wfi
         * should not be executed again.  However, the original expected idle
         * time variable must remain unmodified, so a copy is taken.
         */
        xModifiableIdleTime = xExpectedIdleTime;
        configPRE_SLEEP_PROCESSING(xModifiableIdleTime);
        if (xModifiableIdleTime > 0)
        {
            __DSB();
            __WFI();
            __ISB();
        }
        configPOST_SLEEP_PROCESSING(xExpectedIdleTime);

        /* Reset interrupt flag */
        ulTimerInterruptFired = false;

        /**
         * Re-enable interrupts - see comments above __disable_irq()
         * call above.
         */
        __enable_irq();
        __DSB();
        __ISB();

        /*
         * CRITICAL TIMING SECTION:
         * When the CPU wakes from WFI, interrupts are still disabled but pending.
         * After enabling interrupts above, the pending interrupt doesn't execute immediately.
         * Instead, there's a latency before the interrupt handler starts executing.
         *
         * The issue is:
         * 1. We enable interrupts with __enable_irq()
         * 2. The code continues execution immediately
         * 3. The pending interrupt handler starts executing in parallel
         * 4. We check ulTimerInterruptFired before the interrupt handler has had time to set it
         *
         * This creates a race condition where the code might check the flag
         * before the interrupt handler has had a chance to set it, especially
         * in highly optimized release builds.
         *
         * The loop below creates a window of time for the interrupt handler to execute
         * and set the flag before we check it, avoiding the race condition.
         */
        for (volatile int i = 0; i < 100; i++)
        {
            if (ulTimerInterruptFired)
            {
                break;
            }
            __NOP();
        }

        /* Determine how many tick periods elapsed */
        if (ulTimerInterruptFired)
        {
            /**
             * The tick interrupt handler will already have pended the tick
             * processing in the kernel.  As the pending tick will be
             * processed as soon as this function exits, the tick value
             * maintained by the tick is stepped forward by one less than the
             * time spent waiting.
             */
            ulCompleteTickPeriods = xExpectedIdleTime - 1UL;

            /* Reset interrupt flag */
            ulTimerInterruptFired = false;
        }
        else
        {
            /**
             * Something other than the tick interrupt ended the sleep.
             * Work out how long the sleep lasted rounded to complete tick
             * periods (not the ulReload value which accounted for part
             * ticks).
             */
            ulCompleteTickPeriods = timer_hal->calculate_elapsed_ticks(timer_base, ulReloadValue, xExpectedIdleTime);
        }

        /**
         * Stop timer when CPU waked up then set SysTick->LOAD back to its standard
         * value.  The critical section is used to ensure the tick interrupt
         * can only execute once in the case that the reload register is near
         * zero.
         */
        timer_hal->stop_timer(timer_base);

        /* Restart SysTick and update tick count */
        portENTER_CRITICAL();
        {
            SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
            vTaskStepTick(ulCompleteTickPeriods);
            SysTick->LOAD = ulTimerCountsForOneTick - 1UL;
        }
        portEXIT_CRITICAL();
    }
}

/**
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
void vPortSetupTimerInterrupt(void)
{
    const tickless_timer_hal_t *timer_hal = tickless_get_timer_hal();

    /* Initialize timer hardware */
    timer_hal->init();

    /* Calculate constants */
    ulTimerCountsForOneTick = (configCPU_CLOCK_HZ / configTICK_RATE_HZ);

    /* Get timer frequency from HAL */
    ulTimerCountsForOneSec = timer_hal->get_timer_frequency();

    /* Calculate timer counts for one tick */
    ulLPTimerCountsForOneTick = ulTimerCountsForOneSec / configTICK_RATE_HZ;

    /* Calculate maximum suppressed ticks */
    if (ulLPTimerCountsForOneTick != 0)
    {
        xMaximumPossibleSuppressedTicks = portMAX_32_BIT_NUMBER / ulLPTimerCountsForOneTick;
    }
    else
    {
        /* ulLPTimerCountsForOneTick is zero, not allowed state */
        while (1)
            ;
    }

    /* Enable timer interrupt */
    NVIC_EnableIRQ(timer_hal->get_timer_irqn());

    /* Configure SysTick */
    SysTick->LOAD = ulTimerCountsForOneTick - 1UL;
    SysTick->VAL  = 0UL;
    SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
}

/* Timer ISR handler - to be called from the platform-specific ISR */
void vPortTimerISRHandler(void)
{
    ulTimerInterruptFired                 = true;
    const tickless_timer_hal_t *timer_hal = tickless_get_timer_hal();
    timer_hal->isr_handler();
}

#endif /* configUSE_TICKLESS_IDLE == 2 */
