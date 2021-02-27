/*
 * Copyright  2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_tfa9896_adapter.h"
#include "fsl_codec_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief TFA9896 play capability*/
#ifndef HAL_TFA9896_PLAY_CAPABILITY
#define HAL_TFA9896_PLAY_CAPABILITY \
    kCODEC_SupportModuleSpeaker | kCODEC_SupportPlayChannelLeft0 | kCODEC_SupportPlayChannelRight0

#endif /* HAL_TFA9896_PLAY_CAPABILITY */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * brief Codec initilization.
 *
 * param handle codec handle.
 * param config codec configuration.
 * return kStatus_Success is success, else initial failed.
 */
status_t HAL_CODEC_TFA9896_Init(void *handle, void *config)
{
    assert((config != NULL) && (handle != NULL));

    codec_config_t *codecConfig = (codec_config_t *)config;
    tfa9896_config_t *devConfig = (tfa9896_config_t *)(codecConfig->codecDevConfig);
    tfa9896_handle_t *devHandle = (tfa9896_handle_t *)((uint32_t) & (((codec_handle_t *)handle)->codecDevHandle));
    /* codec device initialization */
    return TFA9896_Init(devHandle, devConfig);
}

/*!
 * brief Codec de-initilization.
 *
 * param handle codec handle.
 * return kStatus_Success is success, else de-initial failed.
 */
status_t HAL_CODEC_TFA9896_Deinit(void *handle)
{
    assert(handle != NULL);

    return TFA9896_Deinit((tfa9896_handle_t *)((uint32_t) & (((codec_handle_t *)handle)->codecDevHandle)));
}

/*!
 * brief set audio data format.
 *
 * param handle codec handle.
 * param mclk master clock frequency in HZ.
 * param sampleRate sample rate in HZ.
 * param bitWidth bit width.
 * return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_TFA9896_SetFormat(void *handle, uint32_t mclk, uint32_t sampleRate, uint32_t bitWidth)
{
    assert(handle != NULL);

    return kStatus_CODEC_NotSupport;
}

/*!
 * brief set audio codec module volume.
 *
 * param handle codec handle.
 * param channel audio codec play channel, can be a value or combine value of _codec_play_channel.
 * param volume volume value, support 0 ~ 100, 0 is mute, 100 is the maximum volume value.
 * return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_TFA9896_SetVolume(void *handle, uint32_t playChannel, uint32_t volume)
{
    assert(handle != NULL);
    return kStatus_Success;
}

/*!
 * brief set audio codec module mute.
 *
 * param handle codec handle.
 * param channel audio codec play channel, can be a value or combine value of _codec_play_channel.
 * param isMute true is mute, false is unmute.
 * return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_TFA9896_SetMute(void *handle, uint32_t playChannel, bool isMute)
{
    assert(handle != NULL);
    status_t ret = kStatus_Success;
    ret          = TFA9896_SetMute((tfa9896_handle_t *)((codec_handle_t *)handle), Mute_Digital);
    if (ret != kStatus_Success)
    {
        return ret;
    }

    return ret;
}
