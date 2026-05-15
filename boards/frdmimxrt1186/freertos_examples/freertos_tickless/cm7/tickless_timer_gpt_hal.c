/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "tickless_hal.h"

#if configUSE_TICKLESS_IDLE == 2
#include "fsl_gpt.h"
#else
#include "fsl_device_registers.h"
#endif

/**
 * @file tickless_timer_gpt_hal.c
 * @brief GPT HAL implementation for FreeRTOS tickless mode
 *
 * This file implements the tickless_timer_hal_t interface for the General Purpose Timer (GPT) peripheral.
 *
 * @note This implementation requires the following board-specific macros to be defined
 * in the examples's app.h file:
 *
 * - BOARD_GPT_TIMER_INSTANCE: The GPT peripheral instance to use (e.g., GPT1)
 * - BOARD_GPT_TIMER_IRQ: The IRQ number for the GPT peripheral (e.g., GPT1_IRQn)
 * - BOARD_GPT_CLOCK_SOURCE: The clock source for the GPT (e.g., kGPT_ClockSource_Periph)
 *
 * Example definitions in app.h:
 * @code
 * #define BOARD_GPT_TIMER_INSTANCE GPT1
 * #define BOARD_GPT_TIMER_IRQ GPT1_IRQn
 * #define BOARD_GPT_CLOCK_SOURCE kGPT_ClockSource_Periph
 * @endcode
 *
 */

#if configUSE_TICKLESS_IDLE == 2

/* Initialize the timer hardware */
static void gpt_init(void)
{
    gpt_config_t gptConfig;

    /* Initialize GPT module */
    GPT_GetDefaultConfig(&gptConfig);
    gptConfig.enableRunInDoze = true;
    gptConfig.enableRunInStop = true;
    gptConfig.enableRunInWait = true;
    gptConfig.clockSource     = kGPT_ClockSource_Periph;
    gptConfig.divider         = 1;
    GPT_Init(BOARD_GPT_TIMER_INSTANCE, &gptConfig);

    /* Divide GPT clock source frequency by 1 inside GPT module */
    GPT_SetClockDivider(GPT1, 1);

    /* Enable GPT Output Compare1 interrupt */
    GPT_EnableInterrupts(GPT1, kGPT_OutputCompare1InterruptEnable);
}

/* Get timer base address */
static void *gpt_get_timer_base(void)
{
    return BOARD_GPT_TIMER_INSTANCE;
}

/* Get timer IRQ number */
static IRQn_Type gpt_get_timer_irqn(void)
{
    return BOARD_GPT_TIMER_IRQ;
}

/* Start the timer with given value */
static void gpt_start_timer(void *base)
{
    GPT_Type *gptBase = (GPT_Type *)base;

    /* Enable output compare interrupt */
    GPT_EnableInterrupts(gptBase, kGPT_OutputCompare1InterruptEnable);

    /* Start timer */
    GPT_StartTimer(gptBase);
}

/* Stop the timer */
static void gpt_stop_timer(void *base)
{
    GPT_Type *gptBase = (GPT_Type *)base;

    /* Disable interrupts */
    GPT_DisableInterrupts(gptBase, kGPT_OutputCompare1InterruptEnable);

    /* Stop timer */
    GPT_StopTimer(gptBase);
}

/* Get current timer count */
static uint32_t gpt_get_current_count(void *base)
{
    GPT_Type *gptBase = (GPT_Type *)base;
    return GPT_GetCurrentTimerCount(gptBase);
}

/* Set timer period/match value */
static void gpt_set_timer_period(void *base, uint32_t value)
{
    GPT_Type *gptBase = (GPT_Type *)base;
    GPT_SetOutputCompareValue(gptBase, kGPT_OutputCompare_Channel1, value);
}

/* Clear timer status flags */
static void gpt_clear_status_flags(void *base)
{
    GPT_Type *gptBase = (GPT_Type *)base;
    GPT_ClearStatusFlags(gptBase, kGPT_OutputCompare1Flag);
}

/* Timer-specific ISR handler */
static void gpt_isr_handler(void)
{
    /* Clear interrupt flag */
    GPT_ClearStatusFlags(BOARD_GPT_TIMER_INSTANCE, kGPT_OutputCompare1Flag);
}

/* Get timer clock frequency */
static uint32_t gpt_get_timer_frequency(void)
{
#if defined(MIMXRT1176_cm7_SERIES) || defined(MIMXRT1176_cm4_SERIES) || defined(MIMXRT1166_cm7_SERIES) || \
    defined(MIMXRT1166_cm4_SERIES)
    return CLOCK_GetFreq(kCLOCK_OscRc48MDiv2);
#elif defined(IMX8MSCALE_SERIES)
    return (CLOCK_GetPllFreq(kCLOCK_SystemPll1Ctrl) / (CLOCK_GetRootPreDivider(kCLOCK_RootGpt1)) /
            (CLOCK_GetRootPostDivider(kCLOCK_RootGpt1)) / 20);
#elif defined(MIMXRT1181_cm33_SERIES) || defined(MIMXRT1182_cm33_SERIES) || defined(MIMXRT1186_cm33_SERIES) || \
    defined(MIMXRT1186_cm7_SERIES) || defined(MIMXRT1187_cm33_SERIES) || defined(MIMXRT1187_cm7_SERIES) ||     \
    defined(MIMXRT1189_cm33_SERIES) || defined(MIMXRT1189_cm7_SERIES)
    return CLOCK_GetRootClockFreq(kCLOCK_Root_Gpt1);
#else
    return (CLOCK_GetFreq(kCLOCK_IpgClk) / 2);
#endif
}

/* Calculate elapsed ticks based on current timer state */
static uint32_t gpt_calculate_elapsed_ticks(void *base, uint32_t reload_value, uint32_t expected_idle_time)
{
    GPT_Type *gptBase      = (GPT_Type *)base;
    uint32_t current_count = GPT_GetCurrentTimerCount(gptBase);

    /* Calculate timer counts for one tick */
    uint32_t timer_counts_for_one_tick = gpt_get_timer_frequency() / configTICK_RATE_HZ;

    uint32_t complete_tick_periods = current_count / timer_counts_for_one_tick;

    /* Safety check with explicit bounds checking */
    uint32_t max_allowed_ticks = 0U;

    if (expected_idle_time > 1U)
    {
        /* Safe subtraction since expected_idle_time > 1 */
        max_allowed_ticks = expected_idle_time - 1U;
    }

    /* Safety check */
    if (complete_tick_periods > max_allowed_ticks)
    {
        complete_tick_periods = max_allowed_ticks;
    }

    return complete_tick_periods;
}

/* GPT HAL implementation */
static const tickless_timer_hal_t gpt_hal = {.init                    = gpt_init,
                                             .get_timer_base          = gpt_get_timer_base,
                                             .get_timer_irqn          = gpt_get_timer_irqn,
                                             .start_timer             = gpt_start_timer,
                                             .stop_timer              = gpt_stop_timer,
                                             .get_current_count       = gpt_get_current_count,
                                             .set_timer_period        = gpt_set_timer_period,
                                             .clear_status_flags      = gpt_clear_status_flags,
                                             .isr_handler             = gpt_isr_handler,
                                             .get_timer_frequency     = gpt_get_timer_frequency,
                                             .calculate_elapsed_ticks = gpt_calculate_elapsed_ticks};

#if defined(CONFIG_TICKLESS_TIMER_GPT)
const tickless_timer_hal_t *tickless_get_timer_hal(void)
{
    return &gpt_hal;
}
#endif

#endif /* configUSE_TICKLESS_IDLE == 2 */
