/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief SLN profiler implementation. Used to capture event-related performance measurements.
 */

#include "fwk_profiler.h"

#ifdef FWK_PROFILER

#include "fwk_sln_platform.h"
#include "fwk_log.h"

#define PROFILER_MAX_EVENTS 20

typedef struct _fwk_profiler_data
{
    unsigned int start_us[PROFILER_MAX_EVENTS];
    unsigned int end_us[PROFILER_MAX_EVENTS];
} fwk_profiler_data_t;

static fwk_profiler_data_t s_FWKProfiler;

unsigned int FWK_Profiler_StartEvent(unsigned int eventHandle)
{
    if (eventHandle < PROFILER_MAX_EVENTS)
    {
        s_FWKProfiler.start_us[eventHandle] = sln_current_time_us();
        s_FWKProfiler.end_us[eventHandle]   = s_FWKProfiler.start_us[eventHandle];
    }
    return eventHandle;
}

void FWK_Profiler_EndEvent(unsigned int eventHandle)
{
    if (eventHandle < PROFILER_MAX_EVENTS)
    {
        s_FWKProfiler.end_us[eventHandle] = sln_current_time_us();
    }
}

void FWK_Profiler_ClearEvents()
{
    for (int i = 0; i < PROFILER_MAX_EVENTS; ++i)
    {
        s_FWKProfiler.start_us[i] = 0;
        s_FWKProfiler.end_us[i]   = 0;
    }
}

void FWK_Profiler_Log(void)
{
    int total_us = 0;
    for (int i = 0; i < PROFILER_MAX_EVENTS; ++i)
    {
        int event_us = s_FWKProfiler.end_us[i] - s_FWKProfiler.start_us[i];
        if (event_us > 0)
        {
            LOGD("Event %d took %d ms", i, event_us / 1000);
            total_us += event_us;
        }
    }

    LOGD("Total time taken: %d ms", total_us / 1000);
}

#endif
