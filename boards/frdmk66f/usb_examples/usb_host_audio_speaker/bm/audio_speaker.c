/*
 * Copyright (c) 2015 -2016, Freescale Semiconductor, Inc.
 * Copyright 2016, 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_host_config.h"
#include "usb_host.h"
#include "audio_speaker.h"
#include "usb_host_audio.h"
#include "fsl_debug_console.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
void Audio_SendData(void);
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#define MAX_ISO_PACKET_SIZE 1024U
#define NUMBER_OF_BUFFER 0x5U
#define NUMBER_OF_COMMAND_RETRY 0x3U
/*******************************************************************************
 * Variables
 ******************************************************************************/
/* USB audio transfer Types string*/
static char *strTransferType[4] = {"Control", "Isochronous", "Bulk", "Interrupt"};
/* USB audio Sync Types string*/
static char *strSyncType[4] = {"No synchronization", "Asynchronous", "Adaptive", "Synchronous"};
/* USB audio Data Types string*/
static char *strDataType[4] = {"Data endpoint", "Feedback endpoint", "Implicit feedback", "Reserved"};
usb_device_handle g_audioDeviceHandle;
usb_host_interface_handle g_audioOutControlifHandle;
usb_host_interface_handle g_audioOutStreamifHandle;
audio_speraker_instance_t g_audio;
USB_RAM_ADDRESS_ALIGNMENT(USB_CACHE_LINESIZE) uint8_t g_wavBuff[MAX_ISO_PACKET_SIZE];
uint32_t g_packetSize;
extern const unsigned char wav_data[];
extern const uint16_t wav_size;
uint8_t g_hostCurVolume = 4U;
uint16_t g_deviceVolumeStep;
static uint8_t g_index = 0U;
usb_audio_stream_format_type_desc_t *g_pFormatTypeDesc = NULL;
usb_audio_stream_spepific_as_intf_desc_t *g_pAsItfDesc = NULL;
usb_audio_stream_specific_iso_endp_desc_t *g_pIsoEndpSpecDesc = NULL;
static uint16_t g_curVolume;
static uint16_t g_minVolume;
static uint16_t g_maxVolume;
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint8_t g_curVol[2];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint8_t g_curMute[2];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint16_t minVol[2];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint16_t maxVol[2];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint16_t resVol[2];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint32_t freq;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief usb host audio control transfer callback.
 *
 * This function is used as callback function for control transfer .
 *
 * @param param    the host audio instance pointer.
 * @param data      data buffer pointer.
 * @param dataLen data length.
 * @param status         transfer result status.
 */
void Audio_ControlCallback(void *param, uint8_t *data, uint32_t dataLen, usb_status_t status)
{
    audio_speraker_instance_t *audio_ptr = (audio_speraker_instance_t *)param;
    static uint32_t retryCount = 0;

    if (status != kStatus_USB_Success)
    {
        if (kUSB_HostAudioRunWaitAudioSetCurSamplingFreq == audio_ptr->runWaitState)
        {
            usb_echo("audio speaker doest not support SamplingFreq request!\r\n");
        }
        retryCount++;
        if (audio_ptr->runWaitState == kUSB_HostAudioRunWaitSetStreamInterface)
        {
            audio_ptr->runState = kUSB_HostAudioRunSetControlInterfaceDone;
        }
        else if (audio_ptr->runWaitState == kUSB_HostAudioRunWaitAudioGetMinVolume)
        {
            audio_ptr->runState = kUSB_HostAudioRunSetInterfaceDone;
        }
        else if (audio_ptr->runWaitState == kUSB_HostAudioRunWaitAudioGetMaxVolume)
        {
            audio_ptr->runState = kUSB_HostAudioRunAudioGetMaxVolume;
        }
        else if (audio_ptr->runWaitState == kUSB_HostAudioRunWaitAudioGetResVolume)
        {
            audio_ptr->runState = kUSB_HostAudioRunAudioGetResVolume;
        }
        else
        {
            retryCount = NUMBER_OF_COMMAND_RETRY;
        }
        if (retryCount < NUMBER_OF_COMMAND_RETRY)
        {
            return;
        }
    }
    if (audio_ptr->runState == kUSB_HostAudioRunIdle)
    {
        retryCount = 0;
        if (audio_ptr->runWaitState == kUSB_HostAudioRunSetControlInterface)
        {
            audio_ptr->runState = kUSB_HostAudioRunSetControlInterfaceDone;
        }
        else if (audio_ptr->runWaitState == kUSB_HostAudioRunWaitSetStreamInterface)
        {
            audio_ptr->runState = kUSB_HostAudioRunSetInterfaceDone;
        }
        else if (audio_ptr->runWaitState == kUSB_HostAudioRunWaitAudioGetMinVolume)
        {
            audio_ptr->runState = kUSB_HostAudioRunAudioGetMaxVolume;
        }
        else if (audio_ptr->runWaitState == kUSB_HostAudioRunWaitAudioGetMaxVolume)
        {
            audio_ptr->runState = kUSB_HostAudioRunAudioGetResVolume;
        }
        else if (audio_ptr->runWaitState == kUSB_HostAudioRunWaitAudioGetResVolume)
        {
            audio_ptr->runState = kUSB_HostAudioRunAudioConfigChannel;
        }
        else if (audio_ptr->runWaitState == kUSB_HostAudioRunWaitAudioConfigChannel)
        {
            audio_ptr->runState = kUSB_HostAudioRunAudioConfigChannel1Vol;
        }
        else if (audio_ptr->runWaitState == kUSB_HostAudioRunWaitAudioConfigChannel1Vol)
        {
            audio_ptr->runState = kUSB_HostAudioRunAudioConfigChannel2Vol;
        }
        else if (audio_ptr->runWaitState == kUSB_HostAudioRunWaitAudioSetCurSamplingFreq)
        {
            audio_ptr->runState = kUSB_HostAudioRunAudioDone;
        }
        else if (audio_ptr->runWaitState == kUSB_HostAudioRunWaitAudioConfigChannel2Vol)
        {
            if (g_audio.deviceIsUsed == 0)
            {
                audio_ptr->runState = kUSB_HostAudioRunAudioSetCurSamplingFreq;
            }
            else if (g_audio.deviceIsUsed == 1)
            {
                audio_ptr->runState = kUSB_HostAudioRunIdle;
            }
            else
            {
            }
        }
        else if (audio_ptr->runWaitState == kUSB_HostAudioRunWaitAudioConfigMute)
        {
            audio_ptr->runState = kUSB_HostAudioRunIdle;
        }
        else
        {
        }
    }
}

/*!
 * @brief host audio  iso  out transfer callback.
 *
 * This function is used as callback function when call usb_host_audio_send .
 *
 * @param param    the host audio instance pointer.
 * @param data      data buffer pointer.
 * @param dataLen data length.
 * @param status    transfer result status.
 */
void Audio_OutCallback(void *param, uint8_t *data, uint32_t dataLen, usb_status_t status)
{
    audio_speraker_instance_t *audio_ptr = (audio_speraker_instance_t *)param;
    if (status == kStatus_USB_TransferCancel)
    {
        return;
    }
    g_audio.bufCount--;

    if (status == kStatus_USB_Success)
    {
        if (audio_ptr->devState == kStatus_DEV_Attached)
        {
            Audio_SendData();
        }
        else
        {
            audio_ptr->runState = kUSB_HostAudioRunIdle;
        }
    }
    else
    {
        if (audio_ptr->devState == kStatus_DEV_Attached)
        {
            Audio_SendData();
        }
    }
}

/*!
 * @brief prepare usb transfer data.
 */
static void USB_PrepareData(void)
{
    uint32_t resolution_size = g_packetSize >> 5U;
    static uint32_t audio_position = 0U;

    uint8_t k, j = 0U;

    for (k = 0U; k < 32U; k++)
    {
        for (j = 0U; j < resolution_size; j++)
        {
            g_wavBuff[g_index * g_packetSize + j * 32 + k] = wav_data[audio_position];
        }
        audio_position++;
    }

    if (audio_position >= 65856U)
    {
        audio_position = 0U;
    }
}

/*!
 * @brief host audio iso out transfer send data function.
 *
 * This function is used to send iso data.
 *
 */
void Audio_SendData(void)
{
    usb_status_t status = kStatus_USB_Success;

#if ((defined USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI))
    while (g_audio.bufCount < 3U)
#endif
    {
        USB_PrepareData();
        status = USB_HostAudioStreamSend(g_audio.classHandle, (unsigned char *)&g_wavBuff[g_packetSize * g_index],
                                         g_audio.maxPacketSize, Audio_OutCallback, &g_audio);
        if (status != kStatus_USB_Success)
        {
            usb_echo("Error in USB_HostAudioStreamSend: %x\r\n", status);
            return;
        }
        g_index++;
        if (g_index == NUMBER_OF_BUFFER)
        {
            g_index = 0U;
        }
        g_audio.bufCount++;
    }
}

/*!
 * @brief host usb audio task function.
 *
 * This function implements the host audio action, it is used to create task.
 *
 * @param param  the host audio instance pointer.
 */
void USB_AudioTask(void *arg)
{
    usb_status_t status = kStatus_USB_Success;
    uint32_t bsamfreqtype_g_index;

    /* device state changes */
    if (g_audio.devState != g_audio.prevState)
    {
        g_audio.prevState = g_audio.devState;
        switch (g_audio.devState)
        {
            case kStatus_DEV_Idle:
                break;

            case kStatus_DEV_Attached:
                g_audio.runState = kUSB_HostAudioRunSetControlInterface;
                g_audio.deviceIsUsed = 0;
                g_audio.bufCount = 0;
                g_hostCurVolume = 4;
                USB_HostAudioInit(g_audio.deviceHandle, &g_audio.classHandle);
                usb_echo("USB audio attached\r\n");
                break;

            case kStatus_DEV_Detached:
                g_audio.devState = kStatus_DEV_Idle;
                g_audio.runState = kUSB_HostAudioRunIdle;
                USB_HostAudioDeinit(g_audio.deviceHandle, g_audio.classHandle);
                g_audio.classHandle = NULL;
                g_audio.bufCount = 0;
                usb_echo("audio speaker detached\r\n");
                break;

            default:
                break;
        }
    }

    /* run state */
    switch (g_audio.runState)
    {
        case kUSB_HostAudioRunIdle:
            break;

        case kUSB_HostAudioRunSetControlInterface:
            g_audio.runWaitState = kUSB_HostAudioRunSetControlInterface;
            g_audio.runState = kUSB_HostAudioRunIdle;
            if (USB_HostAudioControlSetInterface(g_audio.classHandle, g_audio.controlIntfHandle, 0,
                                                 Audio_ControlCallback, &g_audio) != kStatus_USB_Success)
            {
                usb_echo("set interface error\r\n");
            }
            break;
        case kUSB_HostAudioRunSetControlInterfaceDone:
            g_audio.runWaitState = kUSB_HostAudioRunWaitSetStreamInterface;
            g_audio.runState = kUSB_HostAudioRunIdle;
            if (USB_HostAudioStreamSetInterface(g_audio.classHandle, g_audio.streamIntfHandle, 1, Audio_ControlCallback,
                                                &g_audio) != kStatus_USB_Success)
            {
                usb_echo("set interface error\r\n");
            }
            break;

        case kUSB_HostAudioRunSetInterfaceDone:
            if (USB_HostAudioStreamGetCurrentAltsettingDescriptors(
                    g_audio.classHandle, (usb_audio_stream_spepific_as_intf_desc_t **)&g_pAsItfDesc,
                    (usb_audio_stream_format_type_desc_t **)&g_pFormatTypeDesc,
                    (usb_audio_stream_specific_iso_endp_desc_t **)&g_pIsoEndpSpecDesc) != kStatus_USB_Success)
            {
                usb_echo("USB_HostAudioStreamGetCurrentAltsettingDescriptors error\r\n");
            }
            g_audio.maxPacketSize = g_packetSize =
                USB_HostAudioPacketSize(g_audio.classHandle, USB_ENDPOINT_ISOCHRONOUS, USB_OUT);
            g_audio.runWaitState = kUSB_HostAudioRunWaitAudioGetMinVolume;
            g_audio.runState = kUSB_HostAudioRunIdle;
            if (kStatus_USB_Success == USB_HostAudioFeatureUnitRequest(g_audio.classHandle, 1, (void *)minVol,
                                                                       USB_AUDIO_GET_MIN_VOLUME, Audio_ControlCallback,
                                                                       &g_audio))
            {
                g_audio.runWaitState = kUSB_HostAudioRunWaitAudioGetMinVolume;
            }
            else
            {
                g_audio.runState = kUSB_HostAudioRunAudioDone;
            }
            usb_echo("AUDIO_GET_MIN_VOLUME\n\r");
            break;

        case kUSB_HostAudioRunAudioGetMaxVolume:
            g_audio.runWaitState = kUSB_HostAudioRunWaitAudioGetMaxVolume;
            g_audio.runState = kUSB_HostAudioRunIdle;
            if (kStatus_USB_Success == USB_HostAudioFeatureUnitRequest(g_audio.classHandle, 1, (void *)maxVol,
                                                                       USB_AUDIO_GET_MAX_VOLUME, Audio_ControlCallback,
                                                                       &g_audio))
            {
                g_audio.runWaitState = kUSB_HostAudioRunWaitAudioGetMaxVolume;
            }
            else
            {
                g_audio.runState = kUSB_HostAudioRunAudioDone;
            }
            break;

        case kUSB_HostAudioRunAudioGetResVolume:
            g_audio.runState = kUSB_HostAudioRunIdle;
            g_audio.runWaitState = kUSB_HostAudioRunWaitAudioGetResVolume;
            if (kStatus_USB_Success == USB_HostAudioFeatureUnitRequest(g_audio.classHandle, 1, (void *)resVol,
                                                                       USB_AUDIO_GET_RES_VOLUME, Audio_ControlCallback,
                                                                       &g_audio))
            {
                g_audio.runWaitState = kUSB_HostAudioRunWaitAudioGetResVolume;
            }
            else
            {
                g_audio.runState = kUSB_HostAudioRunAudioDone;
            }
            break;

        case kUSB_HostAudioRunAudioConfigChannel:
            g_audio.runState = kUSB_HostAudioRunIdle;
            g_minVolume = (uint16_t)(minVol[1] << 8U) | (minVol[0]);
            g_maxVolume = (uint16_t)(maxVol[1] << 8U) | (maxVol[0]);
            g_deviceVolumeStep =
                (int16_t)(((int16_t)(g_maxVolume) - (int16_t)(g_minVolume)) / (HOST_MAX_VOLUME - HOST_MIN_VOLUME));
            g_curVolume = (int16_t)(g_minVolume + g_deviceVolumeStep * g_hostCurVolume);
            g_curVol[0] = (int8_t)((uint16_t)(g_curVolume)&0x00FF);
            g_curVol[1] = (int8_t)((uint16_t)(g_curVolume) >> 8U);
            g_curMute[0] = 0U;
            g_audio.runWaitState = kUSB_HostAudioRunWaitAudioConfigChannel;
            if (kStatus_USB_Success == USB_HostAudioFeatureUnitRequest(g_audio.classHandle, 0, (void *)g_curMute,
                                                                       USB_AUDIO_SET_CUR_MUTE, Audio_ControlCallback,
                                                                       &g_audio))
            {
                g_audio.runWaitState = kUSB_HostAudioRunWaitAudioConfigChannel;
            }
            else
            {
                g_audio.runState = kUSB_HostAudioRunAudioConfigChannel1Vol;
            }
            break;

        case kUSB_HostAudioRunAudioConfigMute:
            g_audio.runState = kUSB_HostAudioRunIdle;
            g_curMute[0] = !g_curMute[0];
            g_audio.runWaitState = kUSB_HostAudioRunWaitAudioConfigMute;
            USB_HostAudioFeatureUnitRequest(g_audio.classHandle, 0, (void *)g_curMute, USB_AUDIO_SET_CUR_MUTE,
                                            Audio_ControlCallback, &g_audio);
            g_audio.runWaitState = kUSB_HostAudioRunWaitAudioConfigMute;
            break;

        case kUSB_HostAudioRunAudioConfigChannel1Vol:
            g_audio.runState = kUSB_HostAudioRunIdle;
            g_audio.runWaitState = kUSB_HostAudioRunWaitAudioConfigChannel1Vol;
            USB_HostAudioFeatureUnitRequest(g_audio.classHandle, 1, (void *)g_curVol, USB_AUDIO_SET_CUR_VOLUME,
                                            Audio_ControlCallback, &g_audio);
            break;

        case kUSB_HostAudioRunAudioConfigChannel2Vol:
            g_audio.runState = kUSB_HostAudioRunIdle;
            g_audio.runWaitState = kUSB_HostAudioRunWaitAudioConfigChannel2Vol;
            USB_HostAudioFeatureUnitRequest(g_audio.classHandle, 2, (void *)g_curVol, USB_AUDIO_SET_CUR_VOLUME,
                                            Audio_ControlCallback, &g_audio);
            break;

        case kUSB_HostAudioRunAudioSetCurSamplingFreq:
            usb_echo("Audio Speaker device information:\r\n");
            for (bsamfreqtype_g_index = 0U; bsamfreqtype_g_index < g_pFormatTypeDesc->bsamfreqtype;
                 bsamfreqtype_g_index++)
            {
                usb_echo("   - Frequency device support      : %d Hz\n\r",
                         (((uint32_t)g_pFormatTypeDesc->tsamfreq[bsamfreqtype_g_index][2]) << 16U) |
                             (((uint32_t)g_pFormatTypeDesc->tsamfreq[bsamfreqtype_g_index][1]) << 8U) |
                             (((uint32_t)g_pFormatTypeDesc->tsamfreq[bsamfreqtype_g_index][0]) << 0U));
            }
            usb_echo("   - Bit resolution : %d bits\n\r", g_pFormatTypeDesc->bbitresolution);
            usb_echo("   - Number of channels : %d channels\n\r", g_pFormatTypeDesc->bnrchannels);
            usb_echo("   - Transfer type : %s\n\r", strTransferType[(g_pIsoEndpSpecDesc->bmattributes) & EP_TYPE_MASK]);
            usb_echo("   - Sync type : %s\n\r",
                     strSyncType[(uint8_t)(g_pIsoEndpSpecDesc->bmattributes >> 2) & EP_TYPE_MASK]);
            usb_echo("   - Usage type : %s\n\r",
                     strDataType[(uint8_t)(g_pIsoEndpSpecDesc->bmattributes >> 4) & EP_TYPE_MASK]);

            usb_echo("This audio device supports play audio files with these properties:\r\n");
            usb_echo("   - Sample rate    :\r\n");
            for (bsamfreqtype_g_index = 0U; bsamfreqtype_g_index < g_pFormatTypeDesc->bsamfreqtype;
                 bsamfreqtype_g_index++)
            {
                usb_echo("                    : %d Hz\n\r",
                         (((uint32_t)g_pFormatTypeDesc->tsamfreq[bsamfreqtype_g_index][2]) << 16U) |
                             (((uint32_t)g_pFormatTypeDesc->tsamfreq[bsamfreqtype_g_index][1]) << 8U) |
                             (((uint32_t)g_pFormatTypeDesc->tsamfreq[bsamfreqtype_g_index][0]) << 0U));
            }
            usb_echo("   - Sample size    : %d bits\n\r", g_pFormatTypeDesc->bbitresolution);
            usb_echo("   - Number of channels : %d channels\n\r", g_pFormatTypeDesc->bnrchannels);
            usb_echo("USB Speaker example will loop playback %dk_%dbit_%dch format audio.\r\n",
                     ((((uint32_t)g_pFormatTypeDesc->tsamfreq[0][2]) << 16U) |
                      (((uint32_t)g_pFormatTypeDesc->tsamfreq[0][1]) << 8U) |
                      (((uint32_t)g_pFormatTypeDesc->tsamfreq[0][0]) << 0U)) /
                         1000U,
                     g_pFormatTypeDesc->bbitresolution, g_pFormatTypeDesc->bnrchannels);

            freq = (((uint32_t)g_pFormatTypeDesc->tsamfreq[0][2]) << 16U) |
                   (((uint32_t)g_pFormatTypeDesc->tsamfreq[0][1]) << 8U) |
                   (((uint32_t)g_pFormatTypeDesc->tsamfreq[0][0]) << 0U);
            g_audio.runState = kUSB_HostAudioRunIdle;
            g_audio.runWaitState = kUSB_HostAudioRunWaitAudioSetCurSamplingFreq;
            USB_HostAudioEndpointRequest(g_audio.classHandle, &freq, USB_AUDIO_SET_CUR_SAMPLING_FREQ,
                                         Audio_ControlCallback, &g_audio);
            break;

        case kUSB_HostAudioRunAudioDone:
            g_index = 0U;
            while (g_audio.bufCount < 3U)
            {
                USB_PrepareData();
                g_audio.runState = kUSB_HostAudioRunIdle;
                if (USB_HostAudioStreamSend(g_audio.classHandle, (unsigned char *)&g_wavBuff[g_packetSize * g_index],
                                            g_audio.maxPacketSize, Audio_OutCallback, &g_audio) != kStatus_USB_Success)
                {
                    usb_echo("Error in USB_HostAudioStreamSend: %x\r\n", status);
                }
                g_index++;
                if (g_index == NUMBER_OF_BUFFER)
                {
                    g_index = 0U;
                }
                g_audio.bufCount++;
            }
            g_audio.deviceIsUsed = 1;
            break;

        default:
            break;
    }
}

/*!
 * @brief host usb audio mute request.
 *
 * This function implements the host audio mute request action, it is used to mute or unmute audio device.
 *
 */
void Audio_MuteRequest(void)
{
    if (g_audio.devState > kStatus_DEV_Detached)
    {
        usb_echo("  err: Audio Speaker is not connected\n\r");
        return;
    }

    if (g_audio.deviceIsUsed == 0)
    {
        usb_echo("  err: Audio Speaker is not Ready\n\r");
        return;
    }

    g_audio.runState = kUSB_HostAudioRunAudioConfigMute;
}

/*!
 * @brief host usb audio increase volume request.
 *
 * This function implements the host audio increase volume request action, it is used to increase the volume of audio
 * device.
 *
 * @param channel   the audio device channel number.
 */
void Audio_IncreaseVolumeRequest(uint8_t channel)
{
    static uint32_t i;
    uint8_t max_audio_channel;
    max_audio_channel = 2U;
    if (channel > max_audio_channel)
    {
        usb_echo("  err: Channel number larger than max channel\n\r");
        return;
    }
    if (g_audio.devState > kStatus_DEV_Detached)
    {
        usb_echo("  err: Audio Speaker is not connected\n\r");
        return;
    }

    if (g_audio.deviceIsUsed == 0)
    {
        usb_echo("  err: Audio Speaker is not Ready\n\r");
        return;
    }
    if (channel == 1U)
    {
        /* Send set mute request */
        if ((g_hostCurVolume + HOST_VOLUME_STEP) > HOST_MAX_VOLUME)
        {
            g_hostCurVolume = HOST_MAX_VOLUME;
            i = 0U;
        }
        else
        {
            g_hostCurVolume += HOST_VOLUME_STEP;
            i = 1U;
        }
        g_curVolume += (int16_t)(i * HOST_VOLUME_STEP * g_deviceVolumeStep);
        g_curVol[0] = (int8_t)((uint16_t)(g_curVolume)&0x00FF);
        g_curVol[1] = (int8_t)((uint16_t)(g_curVolume) >> 8U);
    }

    g_audio.runState = kUSB_HostAudioRunAudioConfigChannel1Vol;
}

/*!
 * @brief host usb audio decrease volume request.
 *
 * This function implements the host audio decrease volume request action, it is used to decrease the volume of audio
 * device.
 *
 * @param channel   the audio device channel number.
 */
void Audio_DecreaseVolumeRequest(uint8_t channel)
{
    static uint32_t i;
    uint8_t max_audio_channel;
    max_audio_channel = g_pFormatTypeDesc->bnrchannels;
    if (channel > max_audio_channel)
    {
        usb_echo("  err: Channel number larger than max channel\n\r");
        return;
    }
    if (g_audio.devState > kStatus_DEV_Detached)
    {
        usb_echo("  err: Audio Speaker is not connected\n\r");
        return;
    }

    if (g_audio.deviceIsUsed == 0U)
    {
        usb_echo("  err: Audio Speaker is not Ready\n\r");
        return;
    }
    if (channel == 1U)
    {
        if (g_hostCurVolume < (HOST_VOLUME_STEP + HOST_MIN_VOLUME))
        {
            g_hostCurVolume = HOST_MIN_VOLUME;
            i = 0U;
        }
        else
        {
            g_hostCurVolume -= HOST_VOLUME_STEP;
            i = 1U;
        }
        g_curVolume -= (int16_t)(i * HOST_VOLUME_STEP * g_deviceVolumeStep);
        g_curVol[0] = (int8_t)((uint16_t)(g_curVolume)&0x00FF);
        g_curVol[1] = (int8_t)((uint16_t)(g_curVolume) >> 8U);
    }

    g_audio.runState = kUSB_HostAudioRunAudioConfigChannel1Vol;
}

/*!
 * @brief host audio callback function.
 *
 * This function should be called in the host callback function.
 *
 * @param deviceHandle           device handle.
 * @param configurationHandle attached device's configuration descriptor information.
 * @param eventCode              callback event code, please reference to enumeration host_event_t.
 *
 * @retval kStatus_USB_Success              The host is initialized successfully.
 * @retval kStatus_USB_NotSupported         The configuration don't contain hid mouse interface.
 */
usb_status_t USB_HostAudioEvent(usb_device_handle deviceHandle,
                                usb_host_configuration_handle configurationHandle,
                                uint32_t eventCode)
{
    uint8_t id;
    usb_host_configuration_t *configuration_ptr;
    uint8_t interface_g_index;
    usb_host_interface_t *interface_ptr;
    uint32_t info_value;
    usb_status_t status = kStatus_USB_Success;

    switch (eventCode)
    {
        case kUSB_HostEventAttach:
            /* judge whether is configurationHandle supported */
            configuration_ptr = (usb_host_configuration_t *)configurationHandle;
            g_audioDeviceHandle = NULL;
            g_audioOutControlifHandle = NULL;
            g_audioOutStreamifHandle = NULL;
            for (interface_g_index = 0U; interface_g_index < configuration_ptr->interfaceCount; ++interface_g_index)
            {
                interface_ptr = &configuration_ptr->interfaceList[interface_g_index];
                id = interface_ptr->interfaceDesc->bInterfaceClass;
                if (id != USB_AUDIO_CLASS_CODE)
                {
                    continue;
                }
                id = interface_ptr->interfaceDesc->bInterfaceSubClass;
                if (id == USB_AUDIO_SUBCLASS_CODE_CONTROL)
                {
                    g_audioDeviceHandle = deviceHandle;
                    g_audioOutControlifHandle = interface_ptr;
                    continue;
                }
                else if (id == USB_AUDIO_SUBCLASS_CODE_AUDIOSTREAMING)
                {
                    g_audioDeviceHandle = deviceHandle;
                    g_audioOutStreamifHandle = interface_ptr;
                    continue;
                }
                else
                {
                }
            }
            if (g_audioDeviceHandle != NULL)
            {
                return kStatus_USB_Success;
            }
            status = kStatus_USB_NotSupported;
            break;

        case kUSB_HostEventNotSupported:
            break;

        case kUSB_HostEventEnumerationDone:
            if (g_audio.devState == kStatus_DEV_Idle)
            {
                if ((g_audioDeviceHandle != NULL) && (g_audioOutControlifHandle != NULL))
                {
                    g_audio.devState = kStatus_DEV_Attached;
                    g_audio.deviceHandle = g_audioDeviceHandle;
                    g_audio.controlIntfHandle = g_audioOutControlifHandle;
                    g_audio.streamIntfHandle = g_audioOutStreamifHandle;

                    USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDevicePID, &info_value);
                    usb_echo("hid audio attached:pid=0x%x", info_value);
                    USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDeviceVID, &info_value);
                    usb_echo("vid=0x%x ", info_value);
                    USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDeviceAddress, &info_value);
                    usb_echo("address=%d\r\n", info_value);
                }
            }
            else
            {
                usb_echo("not idle audio instance\r\n");
            }
            break;

        case kUSB_HostEventDetach:
            if (g_audio.devState != kStatus_DEV_Idle)
            {
                g_audio.devState = kStatus_DEV_Detached;
            }
            break;

        default:
            break;
    }
    return status;
}
