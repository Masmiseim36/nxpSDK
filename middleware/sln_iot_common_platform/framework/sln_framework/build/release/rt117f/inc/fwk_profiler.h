/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief profiler declaration. Used to capture event-related performance measurements.
 */

#ifndef _FWK_PROFILER_H_
#define _FWK_PROFILER_H_

#if defined(__cplusplus)
extern "C" {
#endif

//#define FWK_PROFILER
#ifdef FWK_PROFILER

unsigned int FWK_Profiler_StartEvent(unsigned int eventHandle);
void FWK_Profiler_EndEvent(unsigned int eventHandle);
void FWK_Profiler_ClearEvents();
void FWK_Profiler_Log(void);

#else

#define FWK_Profiler_StartEvent(x)
#define FWK_Profiler_EndEvent(x)
#define FWK_Profiler_ClearEvents()
#define FWK_Profiler_Log()

#endif

#if defined(__cplusplus)
}
#endif

#endif /* _FWK_PROFILER_H_ */
