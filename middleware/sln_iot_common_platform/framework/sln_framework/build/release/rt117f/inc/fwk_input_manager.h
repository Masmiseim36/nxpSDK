/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief input device manager framework implementation.
 */

#ifndef _FWK_INPUT_MANAGER_H_
#define _FWK_INPUT_MANAGER_H_

#include "hal_input_dev.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Register a input device. All input devices need to be registered before FWK_InputManager_Start is called.
 * @param dev Pointer to a display device structure
 * @return int Return 0 if registration was successful
 */
int FWK_InputManager_DeviceRegister(input_dev_t *dev);

/**
 * @brief Init internal structures for input manager.
 * @return int Return 0 if the init process was successful
 */
int FWK_InputManager_Init();

/**
 * @brief Spawn Input manager task which will call init/start for all registered input devices
 * @return int Return 0 if the starting proccess was successul
 */
int FWK_InputManager_Start();

/**
 * @brief Denit internal structures for input manager.
 * @return int Return 0 if the deinit process was successful
 */
int FWK_InputManager_Deinit();

#if defined(__cplusplus)
}
#endif

#endif /*_FWK_INPUT_MANAGER_H_*/
