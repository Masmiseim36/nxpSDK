/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include <assert.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*
 * In SCG based example, SCG clock sources are configured as:
 *
 * SCG_SOSC: Disabled.
 * SCG_SIRC: 8MHz.
 * SCG_FIRC: 48MHz.
 * SCG_SPLL: 240/336MHz.
 *
 * VLPR:
 *  - Clock source: SIRC
 * RUN:
 *  - Clock source: SPLL
 * HSRUN:
 *  - Clock source: SPLL
 */

/*
 * SIRC clock setting.
 * SIRC clock           : 8MHz
 * SIRCDIV1_CLK output  : 8MHz
 * SIRCDIV2_CLK output  : 4MHz
 */
const scg_sirc_config_t s_scgSircConfig = {.enableMode = kSCG_SircEnable | kSCG_SircEnableInLowPower,
                                           .div1       = kSCG_AsyncClkDivBy1,
                                           .div2       = kSCG_AsyncClkDivBy2,
                                           .range      = kSCG_SircRangeHigh};

/*
 * FIRC clock setting.
 * FIRC clock           : 48MHz
 * FIRCDIV1_CLK output  : 48MHz
 * FIRCDIV2_CLK output  : 48MHz
 */
const scg_firc_config_t s_scgFircConfig = {.enableMode = kSCG_FircEnable,
                                           .div2       = kSCG_AsyncClkDivBy1,
                                           .div1       = kSCG_AsyncClkDivBy1,
                                           .range      = kSCG_FircRange48M,
                                           .trimConfig = NULL};

/*
 * SYSPLL clock setting in RUN mode.
 * SYSPLL clock       : 120MHz
 * SYSPLLDIV1 output  : 120MHz
 * SYSPLLDIV2 output  : 60MHz
 */
const scg_spll_config_t s_scgSysPllConfigRun = {.enableMode  = kSCG_SysPllEnable,
                                                .monitorMode = kSCG_SysPllMonitorDisable,
                                                .div1        = kSCG_AsyncClkDivBy1,
                                                .div2        = kSCG_AsyncClkDivBy2,
                                                .src         = kSCG_SysPllSrcFirc,
                                                .prediv      = 5U,
                                                .mult        = 14U};

/*
 * SYSPLL clock setting in HSRUN mode.
 * SYSPLL clock       : 168MHz
 * SYSPLLDIV1 output  : 168MHz
 * SYSPLLDIV2 output  : 84MHz
 */
const scg_spll_config_t s_scgSysPllConfigHsRun = {.enableMode  = kSCG_SysPllEnable,
                                                  .monitorMode = kSCG_SysPllMonitorDisable,
                                                  .div1        = kSCG_AsyncClkDivBy1,
                                                  .div2        = kSCG_AsyncClkDivBy2,
                                                  .src         = kSCG_SysPllSrcFirc,
                                                  .prediv      = 5U,
                                                  .mult        = 26U};

/*
 * System clock configuration while using SIRC in RUN mode.
 * Core clock : 8MHz
 * Slow clock : 4MHz
 */
const scg_sys_clk_config_t s_sysClkConfigSircInRun = {
    .divSlow = kSCG_SysClkDivBy2, .divBus = kSCG_SysClkDivBy1, .divCore = kSCG_SysClkDivBy1, .src = kSCG_SysClkSrcSirc};

/*
 * System clock configuration while using SIRC in VLPR mode.
 * Core clock : 4MHz
 * Slow clock : 1MHz
 */
const scg_sys_clk_config_t s_sysClkConfigSircInVlpr = {
    .divSlow = kSCG_SysClkDivBy4, .divBus = kSCG_SysClkDivBy1, .divCore = kSCG_SysClkDivBy2, .src = kSCG_SysClkSrcSirc};

/*
 * System clock configuration while using SYSPLL in RUN mode.
 * Core clock : 120MHz
 * Slow clock : 24MHz
 */
const scg_sys_clk_config_t s_sysClkConfigSysPllInRun = {.divSlow = kSCG_SysClkDivBy5,
                                                        .divBus  = kSCG_SysClkDivBy2,
                                                        .divCore = kSCG_SysClkDivBy1,
                                                        .src     = kSCG_SysClkSrcSysPll};

/*
 * System clock configuration while using SYSPLL in HSRUN mode.
 * Core clock : 168MHz
 * Slow clock : 24MHz
 */
const scg_sys_clk_config_t s_sysClkConfigSysPllInHsrun = {.divSlow = kSCG_SysClkDivBy7,
                                                          .divBus  = kSCG_SysClkDivBy2,
                                                          .divCore = kSCG_SysClkDivBy1,
                                                          .src     = kSCG_SysClkSrcSysPll};

/*
 * Initialize SCG setting after system boot up.
 */
void APP_InitClock(void)
{
    scg_sys_clk_config_t sysClkConfig;
    scg_sys_clk_config_t curSysClkConfig;

    /*
     * Setup SIRC and FIRC:
     * On some platforms, SIRC is used by default after reset, while on some
     * other platforms, FIRC is used after reset. So at the begining, the
     * workflow is different.
     */
    CLOCK_GetCurSysClkConfig(&sysClkConfig);

    CLOCK_InitSirc(&s_scgSircConfig);
    CLOCK_SetRunModeSysClkConfig(&s_sysClkConfigSircInRun);

    /* Wait for clock source change finished. */
    do
    {
        CLOCK_GetCurSysClkConfig(&curSysClkConfig);
    } while (curSysClkConfig.src != s_sysClkConfigSircInRun.src);
    CLOCK_InitFirc(&s_scgFircConfig);

    CLOCK_InitSysPll(&s_scgSysPllConfigRun);
    CLOCK_SetRunModeSysClkConfig(&s_sysClkConfigSysPllInRun);

    /* Wait for clock source switch finished. */
    do
    {
        CLOCK_GetCurSysClkConfig(&curSysClkConfig);
    } while (curSysClkConfig.src != s_sysClkConfigSysPllInRun.src);

    CLOCK_SetVlprModeSysClkConfig(&s_sysClkConfigSircInVlpr);
}

/*
 * Set the clock configuration for HSRUN mode.
 */
void APP_SetClockHsrun(void)
{
    scg_sys_clk_config_t curSysClkConfig;

    CLOCK_SetHsrunModeSysClkConfig(&s_sysClkConfigSircInRun);
    /* Wait for clock source switch finished. */
    do
    {
        CLOCK_GetCurSysClkConfig(&curSysClkConfig);
    } while (curSysClkConfig.src != s_sysClkConfigSircInRun.src);
    CLOCK_InitSysPll(&s_scgSysPllConfigHsRun);
    CLOCK_SetHsrunModeSysClkConfig(&s_sysClkConfigSysPllInHsrun);
    while (!CLOCK_IsSysPllValid())
    {
    }
}

/*
 * Set the clock configuration for RUN mode from HSRUN mode.
 */
void APP_SetClockRunFromHsrun(void)
{
    scg_sys_clk_config_t curSysClkConfig;

    CLOCK_SetHsrunModeSysClkConfig(&s_sysClkConfigSircInRun);
    /* Wait for clock source switch finished. */
    do
    {
        CLOCK_GetCurSysClkConfig(&curSysClkConfig);
    } while (curSysClkConfig.src != s_sysClkConfigSircInRun.src);
    CLOCK_InitSysPll(&s_scgSysPllConfigRun);
    while (!CLOCK_IsSysPllValid())
    {
    }
}

/*
 * Set the clock configuration for RUN mode from VLPR mode.
 */
void APP_SetClockRunFromVlpr(void)
{
    while (!CLOCK_IsFircValid())
    {
    }
}

/*
 * Set the clock configuration for VLPR mode.
 */
void APP_SetClockVlpr(void)
{
}
