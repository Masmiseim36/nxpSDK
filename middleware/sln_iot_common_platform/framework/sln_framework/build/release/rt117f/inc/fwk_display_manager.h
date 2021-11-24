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

#ifndef _FWK_DISPLAY_MANAGER_H_
#define _FWK_DISPLAY_MANAGER_H_

#include "hal_display_dev.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Register a display device. All display devices need to be registered before FWK_DisplayManager_Start is
 * called.
 * @param dev Pointer to a display device structure
 * @return int Return 0 if registration was successful
 */
int FWK_DisplayManager_DeviceRegister(display_dev_t *dev);

/**
 * @brief Init internal structures for display manager.
 * @return int Return 0 if the init process was successful
 */
int FWK_DisplayManager_Init();

/**
 * @brief Init internal structures for display manager.
 * @return int Return 0 if the init process was successful
 */
int FWK_DisplayManager_Deinit();

/**
 * @brief Spawn Display manager task which will call init/start for all registered display devices. Will start the flow
 * to recive frames from the camera.
 * @return int Return 0 if starting was successful
 */
int FWK_DisplayManager_Start();

#if defined(__cplusplus)
}
#endif

#endif /*_FWK_DISPLAY_MANAGER_H_*/
