/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _LVGL_FREERTOS_H_
#define _LVGL_FREERTOS_H_

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Define to 1, to enable idle time measurement. */
#ifndef DEMO_MEASURE_IDLE_TIME
#define DEMO_MEASURE_IDLE_TIME 0
#endif

/*******************************************************************************
 * APIs
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void traceTaskSwitchedOut(void);
void traceTaskSwitchedIn(void);

/* This feature depends on micro-second timer, DEMO_InitUsTimer need be called first. */
uint64_t getIdleTaskTime(void);
void resetIdleTaskTime(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/*! @} */

#endif /*_LVGL_FREERTOS_H_*/
