/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _CAMERA_TASK_H_
#define _CAMERA_TASK_H_

#include "camera_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern volatile CameraDvpTransfer g_dvpTransfer;
extern volatile uint32_t g_newVideoFrame;
extern volatile CameraBuffer_t *g_stCamBuf;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/

void camera_task(void *pvParam);
/*! @} */

#if defined(__cplusplus)
}
#endif /*_cplusplus*/

/*@}*/

#endif /*_FSL_FLEXIO_CAMERA_H_*/
