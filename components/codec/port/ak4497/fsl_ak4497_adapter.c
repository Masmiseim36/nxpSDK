/*
 * Copyright  2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_ak4497.h"
#include "fsl_ak4497_adapter.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static const codec_capability_t s_ak4497_capability = {
    .codecModuleCapability = kCODEC_SupportModuleI2SInSwitchInterface,
};
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
status_t HAL_CODEC_AK4497_Init(codec_handle_t *handle, void *config)
{
    assert((config != NULL) && (handle != NULL));

    codec_config_t *codecConfig = (codec_config_t *)config;

    ak4497_config_t *ak4497Config = (ak4497_config_t *)(codecConfig->codecDevConfig);
    ak4497_handle_t *ak4497Handle = (ak4497_handle_t *)((uint32_t)(handle->codecDevHandle));

    handle->codecCapability = &s_ak4497_capability;
    /* codec device initialization */
    return AK4497_Init(ak4497Handle, ak4497Config);
}

/*!
 * brief Codec de-initilization.
 *
 * param handle codec handle.
 * return kStatus_Success is success, else de-initial failed.
 */
status_t HAL_CODEC_AK4497_Deinit(codec_handle_t *handle)
{
    assert(handle != NULL);

    return AK4497_Deinit((ak4497_handle_t *)((uint32_t)(handle->codecDevHandle)));
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
status_t HAL_CODEC_AK4497_SetFormat(codec_handle_t *handle, uint32_t mclk, uint32_t sampleRate, uint32_t bitWidth)
{
    assert(handle != NULL);

    return AK4497_ConfigDataFormat((ak4497_handle_t *)((uint32_t)(handle->codecDevHandle)), mclk, sampleRate, bitWidth);
}

/*!
 * brief set audio codec module volume.
 *
 * param handle codec handle.
 * param channel audio codec play channel, can be a value or combine value of _codec_play_channel.
 * param volume volume value, support 0 ~ 100, 0 is mute, 100 is the maximum volume value.
 * return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_AK4497_SetVolume(codec_handle_t *handle, uint32_t playChannel, uint32_t volume)
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
status_t HAL_CODEC_AK4497_SetMute(codec_handle_t *handle, uint32_t playChannel, bool isMute)
{
    return kStatus_CODEC_NotSupport;
}

/*!
 * brief set audio codec module power.
 *
 * param handle codec handle.
 * param module audio codec module.
 * param powerOn true is power on, false is power down.
 * return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_AK4497_SetPower(codec_handle_t *handle, codec_module_t module, bool powerOn)
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
status_t HAL_CODEC_AK4497_SetRecord(codec_handle_t *handle, uint32_t recordSource)
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
status_t HAL_CODEC_AK4497_SetRecordChannel(codec_handle_t *handle,
                                           uint32_t leftRecordChannel,
                                           uint32_t rightRecordChannel)
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
status_t HAL_CODEC_AK4497_SetPlay(codec_handle_t *handle, uint32_t playSource)
{
    return kStatus_CODEC_NotSupport;
}

/*!
 * brief codec module control.
 *
 * param handle codec handle.
 * param cmd module control cmd, reference _codec_module_ctrl_cmd.
 * param data value to write.
 * return kStatus_Success is success, else configure failed.
 */
status_t HAL_CODEC_AK4497_ModuleControl(codec_handle_t *handle, codec_module_ctrl_cmd_t cmd, uint32_t data)
{
    assert(cmd == kCODEC_ModuleSwitchI2SInInterface);

    return AK4497_ModuleControl((ak4497_handle_t *)((uint32_t)(handle->codecDevHandle)), (ak4497_module_ctrl_cmd_t)cmd,
                                data);
}
