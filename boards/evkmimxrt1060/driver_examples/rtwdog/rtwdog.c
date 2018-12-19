/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_rtwdog.h"
#if defined(FSL_FEATURE_SOC_RCM_COUNT) && (FSL_FEATURE_SOC_RCM_COUNT)
#include "fsl_rcm.h"
#endif
#if defined(FSL_FEATURE_SOC_SMC_COUNT) && (FSL_FEATURE_SOC_SMC_COUNT > 1)
#include "fsl_msmc.h"
#endif
#if defined(FSL_FEATURE_SOC_ASMC_COUNT) && (FSL_FEATURE_SOC_ASMC_COUNT)
#include "fsl_asmc.h"
#endif
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* RESET_CHECK_FLAG is a RAM variable used for wdog32 self test.
 * Make sure this variable's location is proper that it will not be affected by watchdog reset,
 * that is, the variable shall not be intialized in startup code.
 */
#define RESET_CHECK_FLAG (*((uint32_t *)0x20001000))
#define RESET_CHECK_INIT_VALUE 0x0D0D
#define EXAMPLE_WDOG_BASE RTWDOG
#define DELAY_TIME 100000U
#define WDOG_IRQHandler RTWDOG_IRQHandler

#define WDOG_WCT_INSTRUCITON_COUNT (128U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if defined(FSL_FEATURE_SOC_RCM_COUNT) && (FSL_FEATURE_SOC_RCM_COUNT)
static RCM_Type *rcm_base = RCM;
#endif
static rtwdog_config_t config;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Wait until the WCT is closed.
 *
 * This function is used to wait until the WCT window is closed, WCT time is 128 bus cycles
 *
 * @param base RTWDOG peripheral base address
 */
static void WaitWctClose(RTWDOG_Type *base)
{
    /* Accessing register by bus clock */
    for (uint32_t i = 0; i < WDOG_WCT_INSTRUCITON_COUNT; i++)
    {
        (void)base->CNT;
    }
}

/*!
 * @brief Get current test mode.
 *
 * @param base RTWDOG peripheral base address
 */
static inline rtwdog_test_mode_t GetTestMode(RTWDOG_Type *base)
{
    return (rtwdog_test_mode_t)((base->CS & RTWDOG_CS_TST_MASK) >> RTWDOG_CS_TST_SHIFT);
}

#if !(defined(FSL_FEATURE_SOC_ASMC_COUNT) && (FSL_FEATURE_SOC_ASMC_COUNT))
/*!
 * @brief WDOG0 IRQ handler.
 *
 */
void WDOG_IRQHandler(void)
{
    RTWDOG_ClearStatusFlags(EXAMPLE_WDOG_BASE, kRTWDOG_InterruptFlag);

    RESET_CHECK_FLAG++;
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F, Cortex-M7, Cortex-M7F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U || __CORTEX_M == 7U)
    __DSB();
#endif
}
#endif /* FSL_FEATURE_SOC_ASMC_COUNT */

/*!
 * @brief WDOG32 fast testing.
 *
 * Testing each byte of the WDOG32 counter
 */
void RTWdogFastTesting(void)
{
    rtwdog_test_mode_t current_test_mode;
    uint32_t temp;

    /* When system is boot up, WDOG32 is disabled. We must wait for at least 2.5
     * periods of wdog32 clock to reconfigure wodg32. So Delay a while to wait for
     * the previous configuration taking effect. */
    for (temp = 0; temp < DELAY_TIME; temp++)
    {
        __NOP();
    }
    /*
     * config.enableWdog32 = true;
     * config.clockSource = kWDOG32_ClockSource1;
     * config.prescaler = kWDOG32_ClockPrescalerDivide1;
     * config.testMode = kWDOG32_TestModeDisabled;
     * config.enableUpdate = true;
     * config.enableInterrupt = false;
     * config.enableWindowMode = false;
     * config.windowValue = 0U;
     * config.timeoutValue = 0xFFFFU;
     */
    RTWDOG_GetDefaultConfig(&config);

    config.enableInterrupt = true;
    config.timeoutValue = 0xf0f0U;

    current_test_mode = GetTestMode(EXAMPLE_WDOG_BASE);
    if (current_test_mode == kRTWDOG_TestModeDisabled)
    {
/* Set flags in RAM */
#if defined(APP_SKIP_LOW_BYTE_TEST) && (APP_SKIP_LOW_BYTE_TEST)
        RESET_CHECK_FLAG = RESET_CHECK_INIT_VALUE + 1;

        /* High byte test */
        config.testMode = kRTWDOG_HighByteTest;
#else
        RESET_CHECK_FLAG = RESET_CHECK_INIT_VALUE;

        /* Low byte test */
        config.testMode = kRTWDOG_HighByteTest;
#endif

        RTWDOG_Init(EXAMPLE_WDOG_BASE, &config);

        /* Wait for timeout reset */
        while (1)
        {
        }
    }
    else if (current_test_mode == kRTWDOG_LowByteTest)
    {
        if ((RESET_CHECK_FLAG != (RESET_CHECK_INIT_VALUE + 1))
#if defined(FSL_FEATURE_SOC_RCM_COUNT) && (FSL_FEATURE_SOC_RCM_COUNT)
            || ((RCM_GetPreviousResetSources(rcm_base) & kRCM_SourceWdog) == 0)
#elif defined(FSL_FEATURE_SOC_SMC_COUNT) && (FSL_FEATURE_SOC_SMC_COUNT > 1) /* MSMC */
            || ((SMC_GetPreviousResetSources(EXAMPLE_MSMC_BASE) & kSMC_SourceWdog) == 0)
#elif defined(FSL_FEATURE_SOC_ASMC_COUNT) && (FSL_FEATURE_SOC_ASMC_COUNT) /* ASMC */
            || ((ASMC_GetSystemResetStatusFlags(EXAMPLE_ASMC_BASE) & kASMC_WatchdogResetFlag) == 0)
#endif
                )
        {
            PRINTF("Low Byte test fail\r\n");
        }
        else
        {
            PRINTF("Low Byte test success\r\n");
            /* High byte test */
            config.testMode = kRTWDOG_HighByteTest;

            RTWDOG_Init(EXAMPLE_WDOG_BASE, &config);
            /* Wait for timeout reset */
            while (1)
            {
            }
        }
    }
    else if (current_test_mode == kRTWDOG_HighByteTest)
    {
        if ((RESET_CHECK_FLAG != (RESET_CHECK_INIT_VALUE + 1))
#if defined(FSL_FEATURE_SOC_RCM_COUNT) && (FSL_FEATURE_SOC_RCM_COUNT)
            || ((RCM_GetPreviousResetSources(rcm_base) & kRCM_SourceWdog) == 0)
#elif defined(FSL_FEATURE_SOC_SMC_COUNT) && (FSL_FEATURE_SOC_SMC_COUNT > 1) /* MSMC */
            || ((SMC_GetPreviousResetSources(EXAMPLE_MSMC_BASE) & kSMC_SourceWdog) == 0)
#elif defined(FSL_FEATURE_SOC_ASMC_COUNT) && (FSL_FEATURE_SOC_ASMC_COUNT) /* ASMC */
            || ((ASMC_GetSystemResetStatusFlags(EXAMPLE_ASMC_BASE) & kASMC_WatchdogResetFlag) == 0)
#endif
                )
        {
            PRINTF("High Byte test fail\r\n");
        }
        else
        {
            PRINTF("High Byte test success\r\n");

            config.testMode = kRTWDOG_UserModeEnabled;
            config.enableRtwdog = false;

            RTWDOG_Init(EXAMPLE_WDOG_BASE, &config);
            WaitWctClose(EXAMPLE_WDOG_BASE);
        }
    }
    else
    {
    }
}

/*!
 * @brief RTWDOG refresh testing
 *
 * Refresh RTWDOG in window and non-window mode.
 */
void RTWdogRefreshTest(void)
{
    /*
     * config.enableWdog32 = true;
     * config.clockSource = kWDOG32_ClockSource1;
     * config.prescaler = kWDOG32_ClockPrescalerDivide1;
     * config.testMode = kWDOG32_TestModeDisabled;
     * config.enableUpdate = true;
     * config.enableInterrupt = false;
     * config.enableWindowMode = false;
     * config.windowValue = 0U;
     * config.timeoutValue = 0xFFFFU;
     */
    RTWDOG_GetDefaultConfig(&config);

    config.testMode = kRTWDOG_UserModeEnabled;

    config.clockSource = kRTWDOG_ClockSource0;
    config.prescaler = kRTWDOG_ClockPrescalerDivide256;
    config.windowValue = 6000U;
    config.timeoutValue = 60000U;

    PRINTF("\r\n----- Refresh test start -----\r\n");

    /* Refresh test in none-window mode */
    PRINTF("----- None-window mode -----\r\n");
    config.enableWindowMode = false;
    config.enableRtwdog = true;

    RTWDOG_Init(EXAMPLE_WDOG_BASE, &config);

    for (int i = 0; i < 10; i++)
    {
        for (;;)
        {
            if (1000 * i < RTWDOG_GetCounterValue(EXAMPLE_WDOG_BASE))
            {
                PRINTF("Refresh rtwdog %d time\r\n", i);
                RTWDOG_Refresh(EXAMPLE_WDOG_BASE);
                break;
            }
        }
    }
    RTWDOG_Unlock(EXAMPLE_WDOG_BASE);
    RTWDOG_Disable(EXAMPLE_WDOG_BASE);
    WaitWctClose(EXAMPLE_WDOG_BASE);
    /* Refresh test in window mode */
    PRINTF("----- Window mode -----\r\n");

    config.enableWindowMode = true;
    config.enableRtwdog = true;

#if (!defined(BOARD_XTAL0_CLK_HZ))
    /* Use internal clocks when oscilator clock is not available */
    config.clockSource = kRTWDOG_ClockSource1;
#else
    config.clockSource = kRTWDOG_ClockSource2;
#endif

    config.prescaler = kRTWDOG_ClockPrescalerDivide1;

    RTWDOG_Init(EXAMPLE_WDOG_BASE, &config);
    /* When switching clock sources during reconfiguration, the watchdog hardware holds the counter at
       zero for 2.5 periods of the previous clock source and 2.5 periods of the new clock source
       after the configuration time period (128 bus clocks) ends */
    while (RTWDOG_GetCounterValue(EXAMPLE_WDOG_BASE) == 0)
    {
    }
    for (int i = 6; i < 9; i++)
    {
        for (;;)
        {
            /* Refresh wdog32 in the refresh window */
            if (1000 * i < RTWDOG_GetCounterValue(EXAMPLE_WDOG_BASE))
            {
                PRINTF("Refresh rtwdog %d time\r\n", i - 6);
                RTWDOG_Refresh(EXAMPLE_WDOG_BASE);
                break;
            }
        }
    }

    config.enableRtwdog = false;
    config.testMode = kRTWDOG_TestModeDisabled;

    RTWDOG_Init(EXAMPLE_WDOG_BASE, &config);
    WaitWctClose(EXAMPLE_WDOG_BASE);

    PRINTF("----- Refresh test success  -----\r\n\r\n");
}

int main(void)
{
    /* Board pin, clock, debug console init */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /* Enable RTWDOG clock */
    CLOCK_EnableClock(kCLOCK_Wdog3);
    NVIC_EnableIRQ(RTWDOG_IRQn);

#if defined(FSL_FEATURE_SOC_ASMC_COUNT) && (FSL_FEATURE_SOC_ASMC_COUNT)
    if ((ASMC_GetSystemResetStatusFlags(EXAMPLE_ASMC_BASE) & kASMC_WatchdogResetFlag))
    {
        RESET_CHECK_FLAG++;
    }
#endif
    RTWdogFastTesting();
    RTWdogRefreshTest();
    PRINTF("----- End of RTWDOG example  -----\r\n\r\n");

    while (1)
    {
    }
}
