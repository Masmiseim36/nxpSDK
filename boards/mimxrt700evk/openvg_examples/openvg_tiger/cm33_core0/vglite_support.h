/*
 * Copyright 2019, 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _VGLITE_SUPPORT_H_
#define _VGLITE_SUPPORT_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MAX_CONTIGUOUS_SIZE         0x200000

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif                                          /* __cplusplus */

#define VG_LITE_COMMAND_BUFFER_SIZE (128 << 10) /* 128 KB */

/* Default tessellation window width and height, in pixels */
#define DEFAULT_VG_LITE_TW_WIDTH  128 /* pixels */
#define DEFAULT_VG_LITE_TW_HEIGHT DEMO_PANEL_HEIGHT /* pixels */

status_t BOARD_PrepareVGLiteController(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _VGLITE_SUPPORT_H_ */
