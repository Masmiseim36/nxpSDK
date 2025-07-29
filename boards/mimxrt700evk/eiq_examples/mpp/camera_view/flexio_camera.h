/*
 * Copyright 2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _FSL_FLEXIO_CAMERA_H_
#define _FSL_FLEXIO_CAMERA_H_

#include "camera_config.h"

/*!
 * @addtogroup flexio_camera
 * @{
 */

/*******************************************************************************
 * Definitions
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

void CAMERA_Init(void);

/*! @} */

#if defined(__cplusplus)
}
#endif /*_cplusplus*/

/*@}*/

#endif /*_FSL_FLEXIO_CAMERA_H_*/
