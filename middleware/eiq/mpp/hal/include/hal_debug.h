/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * HAL debug functions public header
 */

#ifndef _HAL_DEBUG_H
#define _HAL_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mpp_config.h"

#ifndef EMULATOR
#include "fsl_debug_console.h"
#else
#include <stdio.h>
#define PRINTF printf
#endif

#define LOG_LVL_ERR       0
#define LOG_LVL_INFO      1
#define LOG_LVL_DEBUG     2

#define LOG_DEFAULT_LEVEL LOG_LVL_ERR

#ifndef HAL_LOG_LEVEL
#define HAL_LOG_LEVEL LOG_DEFAULT_LEVEL
#endif

/*
 * Implementation of LOG() should be external since
 * this is depending on SDK configuration, especially
 * regarding flag SDK_DEBUGCONSOLE.
 */
extern void LOG(int cond, const char* module, const char* lvl_str, const char* format, ...);

#define HAL_LOGE(format, ...) \
    LOG(LOG_LVL_ERR <= HAL_LOG_LEVEL, "HAL", "ERR", format, ##__VA_ARGS__)
#define HAL_LOGI(format, ...) \
    LOG(LOG_LVL_INFO <= HAL_LOG_LEVEL, "HAL", "INFO", format, ##__VA_ARGS__)
#define HAL_LOGD(format, ...) \
    LOG(LOG_LVL_DEBUG <= HAL_LOG_LEVEL, "HAL", "DBG", format, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* _HAL_DEBUG_H */
