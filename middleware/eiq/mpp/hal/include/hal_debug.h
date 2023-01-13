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

/*
 * Implementation of LOG() should be external since
 * this is depending on SDK configuration, especially
 * regarding flag SDK_DEBUGCONSOLE.
 */
extern void LOGE(const char* module, const char* func, int line, const char* format, ...);
extern void LOGI(const char* module, const char* func, int line, const char* format, ...);
extern void LOGD(const char* module, const char* func, int line, const char* format, ...);

#define HAL_LOGE(format, ...) \
    LOGE("HAL", __func__, __LINE__, format, ##__VA_ARGS__)
#define HAL_LOGI(format, ...) \
    LOGI("HAL", __func__, __LINE__, format, ##__VA_ARGS__)
#define HAL_LOGD(format, ...) \
    LOGD("HAL", __func__, __LINE__, format, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* _HAL_DEBUG_H */
