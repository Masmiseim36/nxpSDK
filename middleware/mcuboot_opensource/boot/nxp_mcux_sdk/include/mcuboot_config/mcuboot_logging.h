/*
 * Copyright (c) 2026 NXP Semiconductor
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __MCUBOOT_LOGGING_H__
#define __MCUBOOT_LOGGING_H__

#include "fsl_debug_console.h"

extern int SerialRecoveryActive;

#ifdef NDEBUG
#undef assert
#define assert(x) ((void)(x))
#endif

#define MCUBOOT_LOG_MODULE_DECLARE(domain)
#define MCUBOOT_LOG_MODULE_REGISTER(domain)

#define MCUBOOT_LOG_COMMON(...)           \
    {                                     \
        if(!SerialRecoveryActive)         \
        {                                 \
          DbgConsole_Printf(__VA_ARGS__); \
          DbgConsole_Printf("\r\n");      \
        }                                 \
    }

#define MCUBOOT_LOG_ERR(...) MCUBOOT_LOG_COMMON(__VA_ARGS__)
#define MCUBOOT_LOG_WRN(...) MCUBOOT_LOG_COMMON(__VA_ARGS__)
#define MCUBOOT_LOG_INF(...) MCUBOOT_LOG_COMMON(__VA_ARGS__)
#define MCUBOOT_LOG_DBG(...) MCUBOOT_LOG_COMMON(__VA_ARGS__)

#endif /* __MCUBOOT_LOGGING_H__ */
