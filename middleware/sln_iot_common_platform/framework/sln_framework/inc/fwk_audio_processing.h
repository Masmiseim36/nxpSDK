/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _FWK_AUDIO_PROCESSING_H_
#define _FWK_AUDIO_PROCESSING_H_

#include "hal_audio_processing_dev.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Start Audio Processing manager
 *
 * @return int Return 0 if the starting process was successful
 */
int FWK_AudioProcessing_Start(void);

/**
 * @brief Init Audio Processing manager
 *
 * @return int Return 0 if the init process was successful
 */
int FWK_AudioProcessing_Init(void);

/**
 * @brief Deinit Audio Processing manager
 *
 * @return int Return 0 if the deit process was successful
 */
int FWK_AudioProcessing_Deinit(void);

/**
 * @brief Register an audio processing device
 *
 * @param dev Pointer to an Audio Processing device
 * @return int Return 0 if the register was successful
 */
int FWK_AudioProcessing_DeviceRegister(audio_processing_dev_t *dev);

#if defined(__cplusplus)
}
#endif

#endif /*_FWK_AUDIO_MANAGER_H_*/
