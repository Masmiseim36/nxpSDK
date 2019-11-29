/*
 * Copyright  2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_wm8524.h"
#include "fsl_wm8524_adapter.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

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
status_t HAL_CODEC_WM8524_Init(codec_handle_t *handle, void *config)
{
    assert((config != NULL) && (handle != NULL));
    assert(CODEC_HANDLE_SIZE >= (sizeof(codec_handle_t) + sizeof(wm8524_handle_t)));

    codec_config_t *codecConfig = (codec_config_t *)config;

    wm8524_config_t *wm8524Config = (wm8524_config_t *)(codecConfig->codecDevConfig);
    wm8524_handle_t *wm8524Handle = (wm8524_handle_t *)((uint32_t) & (handle->codecDevHandle));

    /* codec device initialization */
    return WM8524_Init(wm8524Handle, wm8524Config);
}

/*!
 * brief Codec de-initilization.
 *
 * param handle codec handle.
 * return kStatus_Success is success, else de-initial failed.
 */
status_t HAL_CODEC_WM8524_Deinit(codec_handle_t *handle)
{
    return kStatus_CODEC_NotSupport;
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
status_t HAL_CODEC_WM8524_SetFormat(codec_handle_t *handle, uint32_t mclk, uint32_t sampleRate, uint32_t bitWidth)
{
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
status_t HAL_CODEC_WM8524_SetVolume(codec_handle_t *handle, uint32_t playChannel, uint32_t volume)
{
    return kStatus_CODEC_NotSupport;
}

/*!
 * brief set audio codec module mute.
 *
 * param handle codec handle.
 * param channel audio codec play channel, can be a value or combine value of _codec_play_channel.
 * param isMute true is mute, false is unmute.
 * return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_WM8524_SetMute(codec_handle_t *handle, uint32_t playChannel, bool isMute)
{
    assert(handle != NULL);

    WM8524_SetMute((wm8524_handle_t *)((uint32_t) & (handle->codecDevHandle)), isMute);

    return kStatus_Success;
}

/*!
 * brief set audio codec module power.
 *
 * param handle codec handle.
 * param module audio codec module.
 * param powerOn true is power on, false is power down.
 * return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_WM8524_SetPower(codec_handle_t *handle, codec_module_t module, bool powerOn)
{
    return kStatus_CODEC_NotSupport;
}

/*!
 * brief codec set record channel.
 *
 * param handle codec handle.
 * param leftRecordChannel audio codec record channel, reference _codec_record_channel, can be a value or combine value
 of member in _codec_record_channel.
 * param rightRecordChannel audio codec record channel, reference _codec_record_channel, can be a value combine of
 member in _codec_record_channel.

 * return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_WM8524_SetRecordChannel(codec_handle_t *handle,
                                           uint32_t leftRecordChannel,
                                           uint32_t rightRecordChannel)
{
    return kStatus_CODEC_NotSupport;
}

/*!
 * brief codec set record source.
 *
 * param handle codec handle.
 * param source audio codec record source, can be a value or combine value of _codec_record_source.
 *
 * return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_WM8524_SetRecord(codec_handle_t *handle, uint32_t recordSource)
{
    return kStatus_CODEC_NotSupport;
}

/*!
 * brief codec set play source.
 *
 * param handle codec handle.
 * param playSource audio codec play source, can be a value or combine value of _codec_play_source.
 *
 * return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_WM8524_SetPlay(codec_handle_t *handle, uint32_t playSource)
{
    return kStatus_CODEC_NotSupport;
}

/*!
 * brief codec module control.
 *
 * param handle codec handle.
 * param cmd module control cmd, reference _codec_module_ctrl_cmd.
 * param data value to write, when cmd is kCODEC_ModuleRecordSourceChannel, the data should be a value combine
 *  of channel and source, please reference macro CODEC_MODULE_RECORD_SOURCE_CHANNEL(source, LP, LN, RP, RN), reference
 *  codec specific driver for detail configurations.
 * return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_WM8524_ModuleControl(codec_handle_t *handle, codec_module_ctrl_cmd_t cmd, uint32_t data)
{
    return kStatus_CODEC_NotSupport;
}
