/*
 * Copyright (c) 2019-2021, Arm Limited. All rights reserved.
 * Copyright 2019-2020 NXP. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_plat_defs.h"
#include "tfm_plat_test.h"
#include "device_definition.h"
#include "tfm_peripherals_def.h"

#if defined(FSL_FEATURE_SOC_CTIMER_COUNT) && (FSL_FEATURE_SOC_CTIMER_COUNT > 0)
#define TIMER_RELOAD_VALUE          (CTIMER_CLK_FREQ * 1) /* 1 sec */
#elif defined(FSL_FEATURE_SOC_LPTMR_COUNT) && (FSL_FEATURE_SOC_LPTMR_COUNT > 0)
#include "fsl_lptmr.h"
#define TIMER_RELOAD_VALUE          (TIMER_CLK_FREQ * 1) /* 1 sec */
#endif

#if (__ARM_FEATURE_CMSE & 0x2) /* Secure */

#if defined(TEST_NS_SLIH_IRQ) || defined(TEST_NS_FLIH_IRQ)

extern void TFM_TIMER0_IRQ_Handler(void); /* Implemented in secure_fw\core\tfm_secure_irq_handlers.inc */

#if defined(FSL_FEATURE_SOC_CTIMER_COUNT) && (FSL_FEATURE_SOC_CTIMER_COUNT > 0)
void CTIMER_IRQ_HANDLER(void)
{
    uint32_t int_stat = CTIMER_GetStatusFlags(CTIMER); /* Get Interrupt status flags */

    /* Clear the status flags that were set */
    CTIMER_ClearStatusFlags(CTIMER, int_stat);

    TFM_TIMER0_IRQ_Handler(); /* Call the TFM handler. */
}
#elif defined(FSL_FEATURE_SOC_LPTMR_COUNT) && (FSL_FEATURE_SOC_LPTMR_COUNT > 0)

void TIMER_IRQ_HANDLER(void)
{
    /* Clear the status flags that were set */
    LPTMR_ClearStatusFlags(TIMER_BASE, kLPTMR_TimerCompareFlag);

    TFM_TIMER0_IRQ_Handler(); /* Call the TFM handler. */
}
#endif

void tfm_plat_test_secure_timer_clear_intr(void)
{
#if defined(FSL_FEATURE_SOC_CTIMER_COUNT) && (FSL_FEATURE_SOC_CTIMER_COUNT > 0)
    uint32_t int_stat = CTIMER_GetStatusFlags(CTIMER); /* Get Interrupt status flags */

    /* Clear the status flags that were set */
    CTIMER_ClearStatusFlags(CTIMER, int_stat);
#elif defined(FSL_FEATURE_SOC_LPTMR_COUNT) && (FSL_FEATURE_SOC_LPTMR_COUNT > 0)
    LPTMR_ClearStatusFlags(TIMER_BASE, kLPTMR_TimerCompareFlag);
#endif
}

void tfm_plat_test_secure_timer_start(void)
{
#if defined(FSL_FEATURE_SOC_CTIMER_COUNT) && (FSL_FEATURE_SOC_CTIMER_COUNT > 0)
    /* Match Configuration */
    ctimer_match_config_t matchConfig;
    ctimer_config_t config;

#if defined(CPU_LPC55S36JBD100)
    /* Use 12 MHz clock for some of the Ctimer */
    CLOCK_SetClkDiv(kCLOCK_DivCtimer2Clk, 0u, false);
    CLOCK_SetClkDiv(kCLOCK_DivCtimer2Clk, 1u, true);
#endif
    
#if defined(CPU_MCXN947VDF_cm33_core0)
    CLOCK_SetClkDiv(CTIMER_CLK_DIVIDE, 1u);
#endif

    CLOCK_AttachClk(CTIMER_CLK_ATTACH);

    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(CTIMER, &config);

    /* Initializes the configure structure. */
    memset(&matchConfig, 0, sizeof(matchConfig));
    matchConfig.enableCounterReset = true;
    matchConfig.enableCounterStop  = false;
    matchConfig.matchValue        = TIMER_RELOAD_VALUE;
    matchConfig.enableInterrupt    = true;
    CTIMER_SetupMatch(CTIMER, kCTIMER_Match_0, &matchConfig);

    CTIMER_StartTimer(CTIMER);
#elif defined(FSL_FEATURE_SOC_LPTMR_COUNT) && (FSL_FEATURE_SOC_LPTMR_COUNT > 0)
    LPTMR_Type *base = TIMER_BASE;
    lptmr_config_t lptmrConfig;

    /* Configure LPTMR */
    /*
     * lptmrConfig.timerMode = kLPTMR_TimerModeTimeCounter;
     * lptmrConfig.pinSelect = kLPTMR_PinSelectInput_0;
     * lptmrConfig.pinPolarity = kLPTMR_PinPolarityActiveHigh;
     * lptmrConfig.enableFreeRunning = false;
     * lptmrConfig.bypassPrescaler = true;
     * lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;
     * lptmrConfig.value = kLPTMR_Prescale_Glitch_0;
     */
    LPTMR_GetDefaultConfig(&lptmrConfig);

    /* Initialize the LPTMR */
    LPTMR_Init(base, &lptmrConfig);

    /*
     * Set timer period.
     * Note : the parameter "ticks" of LPTMR_SetTimerPeriod should be equal or greater than 1.
     */
    LPTMR_SetTimerPeriod(base, TIMER_RELOAD_VALUE);

    /* Enable timer interrupt */
    LPTMR_EnableInterrupts(base, kLPTMR_TimerInterruptEnable);

    /* Start counting */
    LPTMR_StartTimer(base);
#endif
}

void tfm_plat_test_secure_timer_stop(void)
{
#if defined(FSL_FEATURE_SOC_CTIMER_COUNT) && (FSL_FEATURE_SOC_CTIMER_COUNT > 0)
    CTIMER_Deinit(CTIMER);
#elif defined(FSL_FEATURE_SOC_LPTMR_COUNT) && (FSL_FEATURE_SOC_LPTMR_COUNT > 0)
    LPTMR_StopTimer(TIMER_BASE);
#endif
}

#endif /* (TEST_NS_SLIH_IRQ || TEST_NS_FLIH_IRQ) */

#else

#if defined(FSL_FEATURE_SOC_CTIMER_COUNT) && (FSL_FEATURE_SOC_CTIMER_COUNT > 0)
void CTIMER_NS_IRQ_HANDLER(void)
{
    uint32_t int_stat = CTIMER_GetStatusFlags(CTIMER_NS); /* Get Interrupt status flags */

    /* Clear the status flags that were set */
    CTIMER_ClearStatusFlags(CTIMER_NS, int_stat);
}
#elif defined(FSL_FEATURE_SOC_LPTMR_COUNT) && (FSL_FEATURE_SOC_LPTMR_COUNT > 0)
void TIMER_NS_IRQ_HANDLER(void)
{
    /* Clear the status flags that were set */
    LPTMR_ClearStatusFlags(TIMER_BASE, kLPTMR_TimerCompareFlag);
}
#endif

void tfm_plat_test_non_secure_timer_start(void)
{
#if defined(FSL_FEATURE_SOC_CTIMER_COUNT) && (FSL_FEATURE_SOC_CTIMER_COUNT > 0)
    /* Match Configuration */
    ctimer_match_config_t matchConfig;
    ctimer_config_t config;

#if defined(CPU_MCXN947VDF_cm33_core0)    
    CLOCK_SetClkDiv(CTIMER_NS_CLK_DIVIDE, 1u);
#endif
    
    /* Use 12 MHz clock for some of the Ctimer */
    CLOCK_AttachClk(CTIMER_NS_CLK_ATTACH);

    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(CTIMER_NS, &config);

    /* Initializes the configure structure. */
    memset(&matchConfig, 0, sizeof(matchConfig));
    matchConfig.enableCounterReset = true;
    matchConfig.enableCounterStop  = true;
    matchConfig.matchValue         = TIMER_RELOAD_VALUE;
    matchConfig.enableInterrupt    = true;
    CTIMER_SetupMatch(CTIMER_NS, kCTIMER_Match_0, &matchConfig);

    CTIMER_StartTimer(CTIMER_NS);
#elif defined(FSL_FEATURE_SOC_LPTMR_COUNT) && (FSL_FEATURE_SOC_LPTMR_COUNT > 0)
    LPTMR_Type *base = TIMER_BASE;
    lptmr_config_t lptmrConfig;

    /* Configure LPTMR */
    /*
     * lptmrConfig.timerMode = kLPTMR_TimerModeTimeCounter;
     * lptmrConfig.pinSelect = kLPTMR_PinSelectInput_0;
     * lptmrConfig.pinPolarity = kLPTMR_PinPolarityActiveHigh;
     * lptmrConfig.enableFreeRunning = false;
     * lptmrConfig.bypassPrescaler = true;
     * lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;
     * lptmrConfig.value = kLPTMR_Prescale_Glitch_0;
     */
    LPTMR_GetDefaultConfig(&lptmrConfig);

    /* Initialize the LPTMR */
    LPTMR_Init(base, &lptmrConfig);

    /*
     * Set timer period.
     * Note : the parameter "ticks" of LPTMR_SetTimerPeriod should be equal or greater than 1.
     */
    LPTMR_SetTimerPeriod(base, TIMER_RELOAD_VALUE);

    /* Enable timer interrupt */
    LPTMR_EnableInterrupts(base, kLPTMR_TimerInterruptEnable);

    /* Start counting */
    LPTMR_StartTimer(base);
#endif
}

void tfm_plat_test_non_secure_timer_stop(void)
{
#if defined(FSL_FEATURE_SOC_CTIMER_COUNT) && (FSL_FEATURE_SOC_CTIMER_COUNT > 0)
    CTIMER_Deinit(CTIMER_NS);
#elif defined(FSL_FEATURE_SOC_LPTMR_COUNT) && (FSL_FEATURE_SOC_LPTMR_COUNT > 0)
    LPTMR_StopTimer(TIMER_BASE);
#endif
}

#endif /* (__ARM_FEATURE_CMSE & 0x2) */
