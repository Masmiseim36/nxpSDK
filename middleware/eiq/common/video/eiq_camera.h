/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EIQ_CAMERA_H_
#define _EIQ_CAMERA_H_

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

#include "eiq_iui.h"
#include "eiq_camera_conf.h"
#include "eiq_display_conf.h"
#include "stdbool.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Image structure */
typedef struct
{
  EIQ_IUi_t base;
  void (*setReadyCallback)(EIQ_IBufferAddrUpdater_t updater);
} EIQ_Camera_t;

EIQ_Camera_t* EIQ_CameraInit();

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _EIQ_CAMERA_H_ */
