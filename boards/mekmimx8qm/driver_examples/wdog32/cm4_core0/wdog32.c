/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_wdog32.h"
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
/* RESET_CHECK_FLAG is a RAM variable
 * make sure this variable's location is proper that it will not be affected by watchdog reset
 */
#define RESET_CHECK_FLAG (*((uint32_t *)0x20002000))
#define RESET_CHECK_INIT_VALUE 0x0D0D
#define EXAMPLE_WDOG_BASE CM4_0__WDOG
#define EXAMPLE_ASMC_BASE BBS_SIM
#define DELAY_TIME 10000U

#define WDOG_WCT_INSTRUCITON_COUNT (128U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static WDOG_Type *wdog32_base = EXAMPLE_WDOG_BASE;
#if defined(FSL_FEATURE_SOC_RCM_COUNT) && (FSL_FEATURE_SOC_RCM_COUNT)
static RCM_Type *rcm_base = RCM;
#endif
AT_QUICKACCESS_SECTION_DATA(static wdog32_config_t config);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Wait until the WCT is closed.
 *
 * This function is used to wait until the WCT window is closed, WCT time is 128 bus cycles
 *
 * @param base WDOG32 peripheral base address
 */
static void WaitWctClose(WDOG_Type *base)
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
 * @param base WDOG32 peripheral base address
 */
static inline wdog32_test_mode_t GetTestMode(WDOG_Type *base)
{
    return (wdog32_test_mode_t)((base->CS & WDOG_CS_TST_MASK) >> WDOG_CS_TST_SHIFT);
}

#if !(defined(FSL_FEATURE_SOC_ASMC_COUNT) && (FSL_FEATURE_SOC_ASMC_COUNT))
/*!
 * @brief WDOG0 IRQ handler.
 *
 */
void WDOG_IRQHandler(void)
{
    WDOG32_ClearStatusFlags(wdog32_base, kWDOG32_InterruptFlag);

    RESET_CHECK_FLAG++;
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}
#endif /* FSL_FEATURE_SOC_ASMC_COUNT */

/*!
 * @brief WDOG32 fast testing.
 *
 * Testing each byte of the WDOG32 counter
 */
void Wdog32FastTesting(void)
{
    wdog32_test_mode_t current_test_mode;
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
    WDOG32_GetDefaultConfig(&config);

    config.enableInterrupt = true;
    config.timeoutValue    = 0xf0f0U;
    current_test_mode = GetTestMode(wdog32_base);

    if (current_test_mode == kWDOG32_TestModeDisabled)
    {
#if defined(APP_SKIP_LOW_BYTE_TEST) && (APP_SKIP_LOW_BYTE_TEST)
        /* Set flags in RAM */
        RESET_CHECK_FLAG = RESET_CHECK_INIT_VALUE + 1;

        /* High byte test */
        config.testMode = kWDOG32_HighByteTest;
#else
        RESET_CHECK_FLAG = RESET_CHECK_INIT_VALUE;

        /* Low byte test */
        config.testMode = kWDOG32_LowByteTest;
#endif
        WDOG32_Init(wdog32_base, &config);
        /* Wait for timeout reset */
        while (1)
        {
        }
    }
    else if (current_test_mode == kWDOG32_LowByteTest)
    {
        if ((RESET_CHECK_FLAG != (RESET_CHECK_INIT_VALUE + 1))
#if defined(FSL_FEATURE_SOC_RCM_COUNT) && (FSL_FEATURE_SOC_RCM_COUNT)
            || ((RCM_GetPreviousResetSources(rcm_base) & kRCM_SourceWdog) == 0)
#elif defined(FSL_FEATURE_SOC_SMC_COUNT) && (FSL_FEATURE_SOC_SMC_COUNT > 1) /* MSMC */
            || ((SMC_GetPreviousResetSources(EXAMPLE_MSMC_BASE) & kSMC_SourceWdog) == 0)
#elif defined(FSL_FEATURE_SOC_ASMC_COUNT) && (FSL_FEATURE_SOC_ASMC_COUNT)   /* ASMC */
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
            config.testMode = kWDOG32_HighByteTest;

            WDOG32_Init(wdog32_base, &config);
            /* Wait for timeout reset */
            while (1)
            {
            }
        }
    }
    else if (current_test_mode == kWDOG32_HighByteTest)
    {
        if ((RESET_CHECK_FLAG != (RESET_CHECK_INIT_VALUE + 2))
#if defined(FSL_FEATURE_SOC_RCM_COUNT) && (FSL_FEATURE_SOC_RCM_COUNT)
            || ((RCM_GetPreviousResetSources(rcm_base) & kRCM_SourceWdog) == 0)
#elif defined(FSL_FEATURE_SOC_SMC_COUNT) && (FSL_FEATURE_SOC_SMC_COUNT > 1) /* MSMC */
            || ((SMC_GetPreviousResetSources(EXAMPLE_MSMC_BASE) & kSMC_SourceWdog) == 0)
#elif defined(FSL_FEATURE_SOC_ASMC_COUNT) && (FSL_FEATURE_SOC_ASMC_COUNT)   /* ASMC */
            || ((ASMC_GetSystemResetStatusFlags(EXAMPLE_ASMC_BASE) & kASMC_WatchdogResetFlag) == 0)
#endif
        )
        {
            PRINTF("High Byte test fail\r\n");
        }
        else
        {
            PRINTF("High Byte test success\r\n");

            config.testMode     = kWDOG32_UserModeEnabled;
            config.enableWdog32 = false;

            WDOG32_Init(wdog32_base, &config);
            WaitWctClose(wdog32_base);
        }
    }
    else
    {
    }
}

/*!
 * @brief WDOG32 refresh testing
 *
 * Refresh WDOG32 in window and non-window mode.
 */
void Wdog32RefreshTest(void)
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
    WDOG32_GetDefaultConfig(&config);

    config.testMode = kWDOG32_UserModeEnabled;

    config.clockSource  = kWDOG32_ClockSource0;
    config.prescaler    = kWDOG32_ClockPrescalerDivide256;
    config.windowValue  = 6000U;
    config.timeoutValue = 60000U;

    PRINTF("\r\n----- Refresh test start -----\r\n");

    /* Refresh test in none-window mode */
    PRINTF("----- None-window mode -----\r\n");
    config.enableWindowMode = false;
    config.enableWdog32     = true;

    WDOG32_Init(wdog32_base, &config);

    for (int i = 0; i < 10; i++)
    {
        for (;;)
        {
            if (1000 * i < WDOG32_GetCounterValue(wdog32_base))
            {
                PRINTF("Refresh wdog32 %d time\r\n", i);
                WDOG32_Refresh(wdog32_base);
                break;
            }
        }
    }
    WDOG32_Unlock(wdog32_base);
    WDOG32_Disable(wdog32_base);
    WaitWctClose(wdog32_base);
    /* Refresh test in window mode */
    PRINTF("----- Window mode -----\r\n");

    config.enableWindowMode = true;
    config.enableWdog32     = true;

#if (!defined(BOARD_XTAL0_CLK_HZ))
    /* Use internal clocks when oscilator clock is not available */
    config.clockSource = kWDOG32_ClockSource1;
#else
    config.clockSource = kWDOG32_ClockSource2;
#endif

    config.prescaler = kWDOG32_ClockPrescalerDivide1;

    WDOG32_Init(wdog32_base, &config);
    /* When switching clock sources during reconfiguration, the watchdog hardware holds the counter at
       zero for 2.5 periods of the previous clock source and 2.5 periods of the new clock source
       after the configuration time period (128 bus clocks) ends */
    while (WDOG32_GetCounterValue(wdog32_base) != 0)
    {
    }
    while (WDOG32_GetCounterValue(wdog32_base) == 0)
    {
    }
    for (int i = 6; i < 9; i++)
    {
        for (;;)
        {
            /* Refresh wdog32 in the refresh window */
            if (1000 * i < WDOG32_GetCounterValue(wdog32_base))
            {
                PRINTF("Refresh wdog32 %d time\r\n", i - 6);
                WDOG32_Refresh(wdog32_base);
                break;
            }
        }
    }

    config.enableWdog32 = false;
    config.testMode     = kWDOG32_TestModeDisabled;

    WDOG32_Init(wdog32_base, &config);
    WaitWctClose(wdog32_base);

    PRINTF("----- Refresh test success  -----\r\n\r\n");
}

int main(void)
{
    /* Board pin, clock, debug console init */
    sc_ipc_t ipc = BOARD_InitRpc();

    BOARD_InitPins(ipc);
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

#if defined(FSL_FEATURE_SOC_ASMC_COUNT) && (FSL_FEATURE_SOC_ASMC_COUNT)
    if ((ASMC_GetSystemResetStatusFlags(EXAMPLE_ASMC_BASE) & kASMC_WatchdogResetFlag))
    {
        RESET_CHECK_FLAG++;
    }
#endif

    Wdog32FastTesting();
    Wdog32RefreshTest();
    PRINTF("----- End of WDOG32 example  -----\r\n\r\n");

    while (1)
    {
    }
}
