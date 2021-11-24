/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief vision algorithm manager declaration.
 */

#ifndef _FWK_VISION_ALGO_MANAGER_H_
#define _FWK_VISION_ALGO_MANAGER_H_

#include "hal_valgo_dev.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Register a vision algorithm device. All algorithm devices need to be registered before
 * FWK_VisionAlgoManager_Start is called
 * @param dev Pointer to a vision algo device structure
 * @return int Return 0 if registration was successful
 */
int FWK_VisionAlgoManager_DeviceRegister(vision_algo_dev_t *dev);

/**
 * @brief Init internal structures for VisionAlgo manager.
 * @return int Return 0 if the init process was successful
 */
int FWK_VisionAlgoManager_Init();

/**
 * @brief Deinit VisionAlgoManager
 * @return int Return 0 if the deinit process was successful
 */
int FWK_VisionAlgoManager_Deinit();

/**
 * @brief Spawn VisionAlgo manager task which will call init/start for all registered VisionAlgo devices
 * @return int Return 0 if the starting process was successul
 */
int FWK_VisionAlgoManager_Start();

#if defined(__cplusplus)
}
#endif

#endif /*_FWK_VISION_ALGO_MANAGER_H_*/
