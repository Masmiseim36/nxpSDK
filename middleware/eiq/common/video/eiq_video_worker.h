/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EIQ_VIDEO_WORKER_H_
#define _EIQ_VIDEO_WORKER_H_

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

#include "eiq_iworker.h"
#include "eiq_camera.h"
#include "eiq_display.h"
#include "eiq_pxp.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef EIQ_DEFAULT_CAPTURE_RATE
#define EIQ_DEFAULT_CAPTURE_RATE 50
#endif

typedef struct{
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
} Rect_t;

/*! @brief Image structure */
typedef struct
{
  EIQ_IWorker_t base;
#ifdef EIQ_CAPTURE_RECT
  status_t (*setCaptureWindowRectRate)(int heightRate, int widthRate);
#else
  status_t (*setCaptureWindowHeightRate)(int heightRate);
#endif
  EIQ_Camera_t *receiver;
  EIQ_Display_t *sender;
} EIQ_VideoWorker_t;

EIQ_VideoWorker_t* EIQ_VideoWorkerInit();

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _IMAGE_H_ */
