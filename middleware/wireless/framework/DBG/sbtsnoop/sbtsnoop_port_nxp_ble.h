/* -------------------------------------------------------------------------- */
/*                           Copyright 2021-2022 NXP                          */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

#ifndef _SBTSNOOP_PORT_NXP_BLE_H_
#define _SBTSNOOP_PORT_NXP_BLE_H_

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */
#include "fsl_debug_console.h"
#include "fsl_debug_console_conf.h"
#include "fsl_component_timer_manager.h"

/* -------------------------------------------------------------------------- */
/*                                   Macros                                   */
/* -------------------------------------------------------------------------- */
#if !defined(SDK_DEBUGCONSOLE) || (SDK_DEBUGCONSOLE == DEBUGCONSOLE_DISABLE)
#warning "WARNING: SDK_DEBUGCONSOLE is disabled"
#endif

#if !defined(TM_ENABLE_TIME_STAMP) || (TM_ENABLE_TIME_STAMP == 0)
#warning "WARNING: TM_ENABLE_TIME_STAMP is disabled, no timestamp will be available"
#endif

#define sbtsnoop_log_printf(...)            (void)PRINTF(__VA_ARGS__)
#define sbtsnoop_log_scanf(...)             (void)SCANF((char *)__VA_ARGS__)
#define sbtsnoop_get_us_timestamp           TM_GetTimestamp
#define UINT32                              uint32_t

#endif /* _SBTSNOOP_PORT_NXP_BLE_H_ */
