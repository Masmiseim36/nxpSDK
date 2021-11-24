/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief low power mode manager framework declaration.
 */

#ifndef _FWK_LPM_MANAGER_H_
#define _FWK_LPM_MANAGER_H_

#include "hal_lpm_dev.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Register a low power mode device. Currently, only one low power mode device can be registered at a time.
 * @param dev Pointer to a low power mode device structure
 * @return int Return 0 if registration was successful
 */
int FWK_LpmManager_DeviceRegister(lpm_dev_t *dev);

int FWK_LpmManager_RegisterRequestHandler(hal_lpm_request_t *req);
int FWK_LpmManager_UnregisterRequestHandler(hal_lpm_request_t *req);

int FWK_LpmManager_RuntimeGet(hal_lpm_request_t *req);
int FWK_LpmManager_RuntimePut(hal_lpm_request_t *req);
int FWK_LpmManager_RuntimeSet(hal_lpm_request_t *req, int8_t count);

int FWK_LpmManager_RequestStatus(unsigned int *totalUsageCount);

/**
 * @brief Configure the sleep mode to use when entering sleep
 * @param sleepMode sleep mode to use when entering sleep. Examples include SNVS and other "lighter" sleep modes
 * @return int Return 0 if successful
 */
int FWK_LpmManager_SetSleepMode(hal_lpm_mode_t sleepMode);

/**
 * @brief Configure sleep mode on/off status
 * @param enable used to set sleep mode on/off; true is enable, false is disable
 * @return int Return 0 if successful
 */
int FWK_LpmManager_EnableSleepMode(hal_lpm_manager_status_t enable);

#if defined(__cplusplus)
}
#endif

#endif /* _FWK_LPM_MANAGER_H_ */
