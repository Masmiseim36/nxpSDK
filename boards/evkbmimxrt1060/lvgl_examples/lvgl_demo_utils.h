/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _LVGL_DEMO_UTILS_H_
#define _LVGL_DEMO_UTILS_H_

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * APIs
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/* Initialize the timer to get microsecond. */
void DEMO_InitUsTimer(void);

/* Get current time in microsecond. */
uint32_t DEMO_GetUsTimer(void);

/* Get CPU clock frequency. */
uint32_t DEMO_GetCpuClockFreq(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/*! @} */

#endif /*_LVGL_DEMO_UTILS_H_*/
