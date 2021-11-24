/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief camera manager framework declaration.
 */

#ifndef _FWK_CAMERA_MANAGER_H_
#define _FWK_CAMERA_MANAGER_H_

#include "hal_camera_dev.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Register a camera device. All camera devices need to be registered before FWK_CameraManager_Start is called
 * @param dev Pointer to a camera device structure
 * @return int Return 0 if registration was successful
 */
int FWK_CameraManager_DeviceRegister(camera_dev_t *dev);

/**
 * @brief Init internal structures for Camera manager.
 * @return int Return 0 if the init process was successful
 */
int FWK_CameraManager_Init();

/**
 * @brief Deinit CameraManager
 * @return int Return 0 if the deinit process was successful
 */
int FWK_CameraManager_Deinit();

/**
 * @brief Spawn Camera manager task which will call init/start for all registered camera devices
 * @return int Return 0 if the starting process was successul
 */
int FWK_CameraManager_Start();

#if defined(__cplusplus)
}
#endif

#endif /*_FWK_CAMERA_MANAGER_H_*/
