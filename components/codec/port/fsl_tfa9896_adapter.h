/*
 * Copyright  2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_TFA9896_ADAPTER_H_
#define _FSL_TFA9896_ADAPTER_H_

#include "fsl_tfa9896.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!@brief codec handler size */
#define HAL_CODEC_TFA9896_HANDLER_SIZE (TFA9896_I2C_HANDLER_SIZE + 4)
/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif
/*!
 * @brief Codec initilization.
 *
 * @param handle codec handle.
 * @param config codec configuration.
 * @return kStatus_Success is success, else initial failed.
 */
status_t HAL_CODEC_TFA9896_Init(void *handle, void *config);

/*!
 * @brief Codec de-initilization.
 *
 * @param handle codec handle.
 * @return kStatus_Success is success, else de-initial failed.
 */
status_t HAL_CODEC_TFA9896_Deinit(void *handle);

/*!
 * @brief set audio data format.
 *
 * @param handle codec handle.
 * @param mclk master clock frequency in HZ.
 * @param sampleRate sample rate in HZ.
 * @param bitWidth bit width.
 * @return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_TFA9896_SetFormat(void *handle, uint32_t mclk, uint32_t sampleRate, uint32_t bitWidth);

/*!
 * @brief set audio codec module volume.
 *
 * @param handle codec handle.
 * @param playChannel audio codec play channel, can be a value or combine value of _codec_play_channel.
 * @param volume volume value, support 0 ~ 100, 0 is mute, 100 is the maximum volume value.
 * @return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_TFA9896_SetVolume(void *handle, uint32_t playChannel, uint32_t volume);

/*!
 * @brief set audio codec module mute.
 *
 * @param handle codec handle.
 * @param playChannel audio codec play channel, can be a value or combine value of _codec_play_channel.
 * @param isMute true is mute, false is unmute.
 * @return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_TFA9896_SetMute(void *handle, uint32_t playChannel, bool isMute);

#if defined(__cplusplus)
}
#endif

#endif /* _FSL_TFA9896_ADAPTER_H_ */
