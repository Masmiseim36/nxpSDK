/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2017-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _POWER_MODE_SWITCH_H_
#define _POWER_MODE_SWITCH_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Power mode definition used in application. */
typedef enum _app_power_mode
{
    kAPP_PowerModeMin = 'A' - 1,
    kAPP_PowerModeRun,  /* Normal RUN mode */
    kAPP_PowerModeWait, /* WAIT mode. */
    kAPP_PowerModeStop, /* STOP mode. */
    kAPP_PowerModeVlpr, /* VLPR mode. */
    kAPP_PowerModeVlpw, /* VLPW mode. */
    kAPP_PowerModeVlps, /* VLPS mode. */
#if (defined(FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE)
    kAPP_PowerModeLls, /* LLS mode. */
#endif
#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
#if (defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE0) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE0)
    kAPP_PowerModeVlls0, /* VLLS0 mode. */
#endif
    kAPP_PowerModeVlls1, /* VLLS1 mode. */
#if (defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE2) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE2)
    kAPP_PowerModeVlls2, /* VLLS2 mode. */
#endif
    kAPP_PowerModeVlls3, /* VLLS3 mode. */
#endif                   /* FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE */
#if (defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE)
    kAPP_PowerModeHsrun, /* HighSpeed RUN mode */
#endif
    kAPP_PowerModeMax
} app_power_mode_t;

typedef enum _app_wakeup_source
{
    kAPP_WakeupSourceLptmr, /*!< Wakeup by LPTMR.        */
    kAPP_WakeupSourcePin    /*!< Wakeup by external pin. */
} app_wakeup_source_t;

#ifdef SUPPORT_WARMBOOT
typedef struct _warmboot_config
{
    uint32_t doWarmbootSequence;
    uint32_t useWarmbootSp;
} warmboot_config_t;

#if (defined(__ICCARM__))
#define AT_WARMBOOTCONFIG_SECTION(var) var @"WarmbootConfig"
#elif (defined(__CC_ARM))
#define AT_WARMBOOTCONFIG_SECTION(var) __attribute__((section("WarmbootConfig"), zero_init)) var
#elif (defined(__GNUC__))
#define AT_WARMBOOTCONFIG_SECTION(var) __attribute__((section("WarmbootConfig,\"aw\",%nobits @"))) var
#else
#error Toolchain not supported.
#endif
#endif

#endif /* _POWER_MODE_SWITCH_H_ */
