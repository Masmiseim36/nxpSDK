/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "fsl_pit.h"

static int TimeInited = 0;

/* time unit in us */
static unsigned int TimeUnit = 0;

/*! @brief Time_Init.
 *
 * Init the PIT timer 1.
 */
void Time_Init(unsigned int unit_us)
{
    if (!TimeInited)
    {
        pit_config_t pitConfig;
        pit_chnl_t pit_c0, pit_c1;
        uint8_t index = 1;
        assert(index == 0 || index == 1);
        assert(unit_us > 0 && unit_us < 10000UL);
#if ORBBECU1S_RESET_IN_BOOTLOADER
#else
        /* Set PERCLK_CLK source to OSC_CLK*/
        // CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
        /* Set PERCLK_CLK divider to 1 */
        // CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);
        /*
         * pitConfig.enableRunInDebug = false;
         */
        PIT_GetDefaultConfig(&pitConfig);
        /* Init pit module */
        PIT_Init(PIT1, &pitConfig);
#endif

        if (index == 0)
        {
            pit_c0 = kPIT_Chnl_0;
            pit_c1 = kPIT_Chnl_1;
        }
        else
        {
            pit_c0 = kPIT_Chnl_2;
            pit_c1 = kPIT_Chnl_3;
        }

        PIT_SetTimerPeriod(PIT1, pit_c0, USEC_TO_COUNT(unit_us, CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)));
        PIT_SetTimerChainMode(PIT1, pit_c1, 1);
        PIT_SetTimerPeriod(PIT1, pit_c1, 0xFFFFFFFF);
        PIT_StartTimer(PIT1, pit_c1);
        PIT_StartTimer(PIT1, pit_c0);
        TimeInited = 1;
        TimeUnit   = unit_us;
    }
}

/*! @brief TIME_Now.
 *
 * Get the original decremental PIT timer 1.
 */
unsigned int Time_Now()
{
    unsigned int Count = 0xFFFFFFFF;

    if (TimeInited)
    {
        Count = PIT_GetCurrentTimerCount(PIT1, kPIT_Chnl_3);
    }

    return Count;
}

/*! @brief Time_Current.
 *
 * Get the incremental PIT timer 1 value
 */
unsigned int Time_Current()
{
    return (0xFFFFFFFF - Time_Now());
}

/*! @brief Time_Unit.
 *
 * Return the unit in us of the PIT timer 1.
 */
unsigned int Time_Unit()
{
    return TimeUnit;
}
