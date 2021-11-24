/*
 * Copyright 2019-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief log wrapper.
 */

#ifndef _FWK_LOG_H_
#define _FWK_LOG_H_

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>

#ifdef LOG_ENABLE

#ifndef LOG_TAG
#define LOG_TAG ""
#endif

#ifdef RT_PLATFORM
#include "fsl_debug_console.h"
#include "fwk_common.h"
#include "fwk_sln_config.h"
#else
#define PRINTF printf
#endif

#include "fwk_sln_platform.h"

#define printlog(tag, fmt, args...)                                                 \
    {                                                                               \
        do                                                                          \
        {                                                                           \
            uint32_t t = sln_current_time_us();                                     \
            PRINTF("[%7d.%3d][%s] - " fmt "\r\n", t / 1000, t % 1000, tag, ##args); \
        } while (0);                                                                \
    }

#ifndef LOG
#define LOG(tag, fmt, args...) ({ printlog(tag, fmt, ##args); })
#endif

#ifndef LOGV
#define LOGV(fmt, args...)                                \
    {                                                     \
        if (FWK_ConfigGetLogLevel() >= kLOGLevel_Verbose) \
        {                                                 \
            LOG("Verbose", fmt, ##args);                  \
        }                                                 \
    }
#endif

#ifndef LOGD
#define LOGD(fmt, args...)                              \
    {                                                   \
        if (FWK_ConfigGetLogLevel() >= kLOGLevel_Debug) \
        {                                               \
            LOG("Debug", fmt, ##args);                  \
        }                                               \
    }
#endif

#ifndef LOGI
#define LOGI(fmt, args...)                             \
    {                                                  \
        if (FWK_ConfigGetLogLevel() >= kLOGLevel_Info) \
        {                                              \
            LOG("Info", fmt, ##args);                  \
        }                                              \
    }
#endif

#ifndef LOGE
#define LOGE(fmt, args...)                                                              \
    {                                                                                   \
        if (FWK_ConfigGetLogLevel() >= kLOGLevel_Error)                                 \
        {                                                                               \
            PRINTF("\r\n%s:%d: In function \"%s\":\r\n", __FILE__, __LINE__, __func__); \
            LOG("Error", fmt, ##args);                                                  \
        }                                                                               \
    }
#endif

#else

#define LOGV(...)
#define LOGD(...)
#define LOGI(...)
#define LOGE(...)

#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /*_FWK_LOG_H_*/
