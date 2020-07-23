/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _POWER_MODE_SWITCH_H_
#define _POWER_MODE_SWITCH_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

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
