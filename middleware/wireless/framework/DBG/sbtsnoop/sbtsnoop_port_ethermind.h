/* -------------------------------------------------------------------------- */
/*                           Copyright 2021-2022 NXP                          */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

#ifndef _SBTSNOOP_PORT_ETHERMIND_H_
#define _SBTSNOOP_PORT_ETHERMIND_H_

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */
#include "fsl_debug_console.h"
#include "fsl_debug_console_conf.h"
#include "BT_snoop.h"

/* -------------------------------------------------------------------------- */
/*                                   Macros                                   */
/* -------------------------------------------------------------------------- */
#if !defined(SDK_DEBUGCONSOLE) || (SDK_DEBUGCONSOLE == DEBUGCONSOLE_DISABLE)
#warning "WARNING: SDK_DEBUGCONSOLE is disabled"
#endif

#define sbtsnoop_log_printf(...)            PRINTF(__VA_ARGS__)
#define sbtsnoop_log_scanf(...)             (void)SCANF((char *)__VA_ARGS__)
#define sbtsnoop_get_us_timestamp           EM_get_us_timestamp

#endif /* _SBTSNOOP_PORT_ETHERMIND_H_ */
