/*
 * Copyright 2019-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LVGL_SUPPORT_H
#define LVGL_SUPPORT_H

#include <stdint.h>
#include "display_support.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void lv_port_disp_init(void);
void lv_port_indev_init(void);
void DEMO_CleanInvalidateCacheByAddr(void * addr, int32_t dsize);

#if defined(__cplusplus)
}
#endif

#endif /*LVGL_SUPPORT_H */
