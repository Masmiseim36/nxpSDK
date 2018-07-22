/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_common.h"
#include "clock_config.h"
#include <assert.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*
 * README:
 *
 * The clock configuration in differet power modes.
 *
 * VLPR:  MCG work in BLPI mode, MCGOUTCLK = 4MHz.
 * RUN:   MCG work mode depends on g_defaultClockConfigRun defined in clock_config.c
 * HSRUN: MCG work mode depends on g_defaultClockConfigHsrun defined in clock_config.c
 *
 * RESTIRCTION:
 *
 * MCG supports many modes, in this example, there are such restrictions to make
 * the MCG mode switch easy and clear:
 * 1. MCG works in FEE or PEE mode if power mode is RUN/HSRUN.
 * 2. In RUN mode and HSRUN mode, the MCG works in same mode but with different
 *    settings. For example, in RUN mode, MCG works in FEE mode with 80MHz, in
 *    HSRUN mode, MCG works in FEE mode with 100MHz.
 * 3. If use external reference clock, the external reference clock source does not change,
 *    so don't need to call CLOCK_SetExternalRefClkConfig(oscsel); while mode switch.
 *
 * In other words, this example only supports two cases:
 * Case 1:
 *     Power mode    |   VLPR   |   RUN   |  HSRUN
 * --------------------------------------------------
 *      MCG mode     |   BLPI   |   PEE   |  PEE
 *
 * Case 2:
 *     Power mode    |   VLPR   |   RUN   |  HSRUN
 * --------------------------------------------------
 *      MCG mode     |   BLPI   |   FEE   |  FEE
 */

/*! @brief Clock configuration structure. */
typedef struct _clock_config
{
    mcg_config_t mcgConfig;       /*!< MCG configuration.      */
    sim_clock_config_t simConfig; /*!< SIM configuration.      */
    osc_config_t oscConfig;       /*!< OSC configuration.      */
} clock_config_t;

extern const clock_config_t g_defaultClockConfigHsrun;
extern const clock_config_t g_defaultClockConfigRun;
extern const clock_config_t g_defaultClockConfigVlpr;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * Set the clock configuration for HSRUN mode.
 */
void APP_SetClockHsrun(void)
{
    CLOCK_SetSimSafeDivs();
    /*
     * Current power mode is RUN, in RUN mode and HSRUN mode, MCG works in the
     * same mode, see RESTIRCTION 2 at the beginning of this file.
     */
    /* MCG works in PEE mode. */
    CLOCK_SetPbeMode(kMCG_PllClkSelPll0, &g_defaultClockConfigHsrun.mcgConfig.pll0Config);
    CLOCK_SetPeeMode();

    CLOCK_SetSimConfig(&g_defaultClockConfigHsrun.simConfig);
}

/*
 * Set the clock configuration for RUN mode from HSRUN mode.
 */
void APP_SetClockRunFromHsrun(void)
{
    CLOCK_SetSimSafeDivs();
    /*
     * Current power mode is RUN, in RUN mode and HSRUN mode, MCG works in the
     * same mode, see RESTIRCTION 2 at the beginning of this file.
     */

    /* MCG works in PEE mode. */
    CLOCK_SetPbeMode(kMCG_PllClkSelPll0, &g_defaultClockConfigRun.mcgConfig.pll0Config);
    CLOCK_SetPeeMode();

    CLOCK_SetSimConfig(&g_defaultClockConfigRun.simConfig);
}

/*
 * Set the clock configuration for RUN mode from VLPR mode.
 */
void APP_SetClockRunFromVlpr(void)
{
    CLOCK_SetSimSafeDivs();
    /*
     * See RESTIRCTION at the beginning of this file.
     * The target MCG mode must be PEE.
     * If the SOC does not support HSRUN mode, then must
     */
    /* Enter FBI. */
    CLOCK_SetLowPowerEnable(false);
    /* Enter FBE. */
    CLOCK_SetFbeMode(g_defaultClockConfigRun.mcgConfig.frdiv, g_defaultClockConfigRun.mcgConfig.dmx32,
                     g_defaultClockConfigRun.mcgConfig.drs, NULL);
    /* Enter PBE. */
    CLOCK_SetPbeMode(kMCG_PllClkSelPll0, &g_defaultClockConfigRun.mcgConfig.pll0Config);
    /* Enter PEE. */
    CLOCK_SetPeeMode();

    CLOCK_SetSimConfig(&g_defaultClockConfigRun.simConfig);
}

/*
 * Set the clock configuration for VLPR mode.
 */
void APP_SetClockVlpr(void)
{
    CLOCK_SetSimSafeDivs();
    CLOCK_SetInternalRefClkConfig(g_defaultClockConfigVlpr.mcgConfig.irclkEnableMode,
                                  g_defaultClockConfigVlpr.mcgConfig.ircs, g_defaultClockConfigVlpr.mcgConfig.fcrdiv);

    /*
     * Current power mode should be RUN mode and MCG mode should be PEE,
     * see RESTIRCTION 1 at the beginning of this file.
     */

    /* MCG works in PEE mode. */
    CLOCK_ExternalModeToFbeModeQuick();  /* Enter FBE. */
    CLOCK_SetFbiMode(kMCG_DrsLow, NULL); /* Enter FBI. */
    CLOCK_SetLowPowerEnable(true);       /* Enter BLPI. */

    CLOCK_SetSimConfig(&g_defaultClockConfigVlpr.simConfig);
}
