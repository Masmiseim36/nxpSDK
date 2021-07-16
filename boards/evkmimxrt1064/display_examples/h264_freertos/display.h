/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

/*
 * Return value:
 *  - 0: Initialize success.
 *  - Not 0: Failed.
 */
int DEMO_InitDisplay(void);

void DEMO_DisplayFrame(uint16_t width,
                       uint16_t height,
                       const uint8_t *Y,
                       const uint8_t *U,
                       const uint8_t *V,
                       uint32_t Y_Stride,
                       uint32_t UV_Stride);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif
