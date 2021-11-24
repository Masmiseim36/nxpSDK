/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief performance measurement declaration. Used to capture performance statistics.
 */

#ifndef _FWK_PERF_H_
#define _FWK_PERF_H_


#define FWK_PERF 0

/* The fps device type. The maximum devices of each type has been defined in the framework_common.h */
typedef enum _fwk_fps_type
{
    kFWKFPSType_Camera  = 0,
    kFWKFPSType_Display = 1,
    kFWKFPSType_VAlgo   = 2,
    kFWKFPSType_Count
} fwk_fps_type_t;

#if defined(__cplusplus)
extern "C" {
#endif

#if FWK_PERF
/* calculate the current fps of the device */
float fwk_fps(fwk_fps_type_t type, int id);

/* get the current fps of the device */
float fwk_get_fps(fwk_fps_type_t type, int id);
#else
#define fwk_fps(x, y)
#define fwk_get_fps(x, y)
#endif

#if defined(__cplusplus)
}
#endif

#endif /*_FWK_SLN_PERF_H_*/
