/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_audio.h"

#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "composite.h"

#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include <stdio.h>
#include <stdlib.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define AUDIO_SAMPLING_RATE_TO_10_14 (AUDIO_SAMPLING_RATE_KHZ << 14)
#define AUDIO_SAMPLING_RATE_TO_16_16 (AUDIO_SAMPLING_RATE_KHZ << 20)
#define AUDIO_UPDATE_FEEDBACK_DATA(m, n) \
    {                                    \
        m[0] = (n & 0xFFU);              \
        m[1] = ((n >> 8U) & 0xFFU);      \
        m[2] = ((n >> 16U) & 0xFFU);     \
    }

#define USB_AUDIO_ENTER_CRITICAL() \
                                   \
    USB_OSA_SR_ALLOC();            \
                                   \
    USB_OSA_ENTER_CRITICAL()

#define USB_AUDIO_EXIT_CRITICAL() USB_OSA_EXIT_CRITICAL()
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
usb_status_t USB_DeviceAudioCallback(class_handle_t handle, uint32_t event, void *param);
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);

extern void BOARD_USB_AUDIO_KEYBOARD_Init(void);
extern void BOARD_USB_Audio_TxRxInit(uint32_t samplingRate);
extern void BOARD_Codec_Init(void);
extern void BOARD_DMA_EDMA_Config(void);
extern void BOARD_Create_Audio_DMA_EDMA_Handle(void);
extern void BOARD_DMA_EDMA_Set_AudioFormat(void);
extern void BOARD_DMA_EDMA_Enable_Audio_Interrupts(void);
extern void BOARD_DMA_EDMA_Start(void);
extern void BOARD_SetCodecMuteUnmute(bool);
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
extern void SCTIMER_CaptureInit(void);
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t audioPlayDataBuff[AUDIO_SPEAKER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_OUT_ENDP_PACKET_SIZE];
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t audioPlayPacket[FS_ISO_OUT_ENDP_PACKET_SIZE];
#else
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t audioPlayPacket[(FS_ISO_OUT_ENDP_PACKET_SIZE + AUDIO_FORMAT_CHANNELS * AUDIO_FORMAT_SIZE)];
#endif

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t audioRecDataBuff[AUDIO_RECORDER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_IN_ENDP_PACKET_SIZE];
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t audioRecPacket[(FS_ISO_IN_ENDP_PACKET_SIZE)];
#else
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t audioRecPacket[(FS_ISO_IN_ENDP_PACKET_SIZE + AUDIO_FORMAT_CHANNELS * AUDIO_FORMAT_SIZE)];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
uint8_t audioFeedBackBuffer[3];
#endif

usb_device_composite_struct_t *g_deviceAudioComposite;
volatile bool g_CodecMuteUnmute = false;
extern usb_device_composite_struct_t g_composite;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Initialize the structure information for sai. */
void Init_Board_Sai_Codec(void)
{
    usb_echo("Init Audio SAI and CODEC\r\n");

    BOARD_USB_AUDIO_KEYBOARD_Init();

    BOARD_USB_Audio_TxRxInit(AUDIO_SAMPLING_RATE);
    BOARD_Codec_I2C_Init();
    BOARD_Codec_Init();

    BOARD_DMA_EDMA_Config();
    BOARD_DMA_EDMA_Set_AudioFormat();
    BOARD_DMA_EDMA_Enable_Audio_Interrupts();
    BOARD_DMA_EDMA_Start();
}

/*!
 * @brief Audio class specific request function.
 *
 * This function handles the Audio class specific requests.
 *
 * @param handle           The USB device handle.
 * @param event            The USB device event type.
 * @param param            The parameter of the device specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceAudioRequest(class_handle_t handle, uint32_t event, void *param)
{
    usb_device_control_request_struct_t *request = (usb_device_control_request_struct_t *)param;
    usb_status_t error                           = kStatus_USB_Success;
    uint16_t volume;

    switch (event)
    {
        case USB_DEVICE_AUDIO_GET_CUR_MUTE_CONTROL:
            request->buffer = &g_deviceAudioComposite->audioUnified.curMute;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.curMute);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_VOLUME_CONTROL:
            request->buffer = g_deviceAudioComposite->audioUnified.curVolume;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.curVolume);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_BASS_CONTROL:
            request->buffer = &g_deviceAudioComposite->audioUnified.curBass;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.curBass);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_MID_CONTROL:
            request->buffer = &g_deviceAudioComposite->audioUnified.curMid;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.curMid);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_TREBLE_CONTROL:
            request->buffer = &g_deviceAudioComposite->audioUnified.curTreble;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.curTreble);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_AUTOMATIC_GAIN_CONTROL:
            request->buffer = &g_deviceAudioComposite->audioUnified.curAutomaticGain;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.curAutomaticGain);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_DELAY_CONTROL:
            request->buffer = g_deviceAudioComposite->audioUnified.curDelay;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.curDelay);
            break;
        case USB_DEVICE_AUDIO_GET_CUR_SAMPLING_FREQ_CONTROL:
            request->buffer = g_deviceAudioComposite->audioUnified.curSamplingFrequency;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.curSamplingFrequency);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_VOLUME_CONTROL:
            request->buffer = g_deviceAudioComposite->audioUnified.minVolume;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.minVolume);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_BASS_CONTROL:
            request->buffer = &g_deviceAudioComposite->audioUnified.minBass;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.minBass);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_MID_CONTROL:
            request->buffer = &g_deviceAudioComposite->audioUnified.minMid;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.minMid);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_TREBLE_CONTROL:
            request->buffer = &g_deviceAudioComposite->audioUnified.minTreble;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.minTreble);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_DELAY_CONTROL:
            request->buffer = g_deviceAudioComposite->audioUnified.minDelay;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.minDelay);
            break;
        case USB_DEVICE_AUDIO_GET_MIN_SAMPLING_FREQ_CONTROL:
            request->buffer = g_deviceAudioComposite->audioUnified.minSamplingFrequency;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.minSamplingFrequency);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_VOLUME_CONTROL:
            request->buffer = g_deviceAudioComposite->audioUnified.maxVolume;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.maxVolume);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_BASS_CONTROL:
            request->buffer = &g_deviceAudioComposite->audioUnified.maxBass;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.maxBass);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_MID_CONTROL:
            request->buffer = &g_deviceAudioComposite->audioUnified.maxMid;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.maxMid);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_TREBLE_CONTROL:
            request->buffer = &g_deviceAudioComposite->audioUnified.maxTreble;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.maxTreble);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_DELAY_CONTROL:
            request->buffer = g_deviceAudioComposite->audioUnified.maxDelay;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.maxDelay);
            break;
        case USB_DEVICE_AUDIO_GET_MAX_SAMPLING_FREQ_CONTROL:
            request->buffer = g_deviceAudioComposite->audioUnified.maxSamplingFrequency;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.maxSamplingFrequency);
            break;
        case USB_DEVICE_AUDIO_GET_RES_VOLUME_CONTROL:
            request->buffer = g_deviceAudioComposite->audioUnified.resVolume;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.resVolume);
            break;
        case USB_DEVICE_AUDIO_GET_RES_BASS_CONTROL:
            request->buffer = &g_deviceAudioComposite->audioUnified.resBass;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.resBass);
            break;
        case USB_DEVICE_AUDIO_GET_RES_MID_CONTROL:
            request->buffer = &g_deviceAudioComposite->audioUnified.resMid;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.resMid);
            break;
        case USB_DEVICE_AUDIO_GET_RES_TREBLE_CONTROL:
            request->buffer = &g_deviceAudioComposite->audioUnified.resTreble;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.resTreble);
            break;
        case USB_DEVICE_AUDIO_GET_RES_DELAY_CONTROL:
            request->buffer = g_deviceAudioComposite->audioUnified.resDelay;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.resDelay);
            break;
        case USB_DEVICE_AUDIO_GET_RES_SAMPLING_FREQ_CONTROL:
            request->buffer = g_deviceAudioComposite->audioUnified.resSamplingFrequency;
            request->length = sizeof(g_deviceAudioComposite->audioUnified.resSamplingFrequency);
            break;

        case USB_DEVICE_AUDIO_SET_CUR_VOLUME_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = g_deviceAudioComposite->audioUnified.curVolume;
            }
            else
            {
                volume = (uint16_t)((uint16_t)g_deviceAudioComposite->audioUnified.curVolume[1] << 8U);
                volume |= (uint8_t)(g_deviceAudioComposite->audioUnified.curVolume[0]);
                g_deviceAudioComposite->audioUnified.codecTask |= VOLUME_CHANGE_TASK;
            }
            break;
        case USB_DEVICE_AUDIO_SET_CUR_MUTE_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &g_deviceAudioComposite->audioUnified.curMute;
            }
            else
            {
                if (g_deviceAudioComposite->audioUnified.curMute)
                {
                    g_deviceAudioComposite->audioUnified.codecTask |= MUTE_CODEC_TASK;
                }
                else
                {
                    g_deviceAudioComposite->audioUnified.codecTask |= UNMUTE_CODEC_TASK;
                }
            }
            break;
        case USB_DEVICE_AUDIO_SET_CUR_BASS_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &g_deviceAudioComposite->audioUnified.curBass;
            }
            break;
        case USB_DEVICE_AUDIO_SET_CUR_MID_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &g_deviceAudioComposite->audioUnified.curMid;
            }
            break;
        case USB_DEVICE_AUDIO_SET_CUR_TREBLE_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &g_deviceAudioComposite->audioUnified.curTreble;
            }
            break;
        case USB_DEVICE_AUDIO_SET_CUR_AUTOMATIC_GAIN_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &g_deviceAudioComposite->audioUnified.curAutomaticGain;
            }
            break;
        case USB_DEVICE_AUDIO_SET_CUR_DELAY_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = g_deviceAudioComposite->audioUnified.curDelay;
            }
            break;
        case USB_DEVICE_AUDIO_SET_CUR_SAMPLING_FREQ_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = g_deviceAudioComposite->audioUnified.curSamplingFrequency;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MIN_VOLUME_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = g_deviceAudioComposite->audioUnified.minVolume;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MIN_BASS_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &g_deviceAudioComposite->audioUnified.minBass;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MIN_MID_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &g_deviceAudioComposite->audioUnified.minMid;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MIN_TREBLE_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &g_deviceAudioComposite->audioUnified.minTreble;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MIN_DELAY_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = g_deviceAudioComposite->audioUnified.minDelay;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MIN_SAMPLING_FREQ_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = g_deviceAudioComposite->audioUnified.minSamplingFrequency;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MAX_VOLUME_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = g_deviceAudioComposite->audioUnified.maxVolume;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MAX_BASS_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &g_deviceAudioComposite->audioUnified.maxBass;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MAX_MID_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &g_deviceAudioComposite->audioUnified.maxMid;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MAX_TREBLE_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &g_deviceAudioComposite->audioUnified.maxTreble;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MAX_DELAY_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = g_deviceAudioComposite->audioUnified.maxDelay;
            }
            break;
        case USB_DEVICE_AUDIO_SET_MAX_SAMPLING_FREQ_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = g_deviceAudioComposite->audioUnified.maxSamplingFrequency;
            }
            break;
        case USB_DEVICE_AUDIO_SET_RES_VOLUME_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = g_deviceAudioComposite->audioUnified.resVolume;
            }
            break;
        case USB_DEVICE_AUDIO_SET_RES_BASS_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &g_deviceAudioComposite->audioUnified.resBass;
            }
            break;
        case USB_DEVICE_AUDIO_SET_RES_MID_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &g_deviceAudioComposite->audioUnified.resMid;
            }
            break;
        case USB_DEVICE_AUDIO_SET_RES_TREBLE_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = &g_deviceAudioComposite->audioUnified.resTreble;
            }
            break;
        case USB_DEVICE_AUDIO_SET_RES_DELAY_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = g_deviceAudioComposite->audioUnified.resDelay;
            }
            break;
        case USB_DEVICE_AUDIO_SET_RES_SAMPLING_FREQ_CONTROL:
            if (request->isSetup == 1U)
            {
                request->buffer = g_deviceAudioComposite->audioUnified.resSamplingFrequency;
            }
            break;
        default:
            error = kStatus_USB_InvalidRequest;
            break;
    }
    return error;
}

/* The USB_AudioSpeakerBufferSpaceUsed() function gets the used speaker ringbuffer size */
uint32_t USB_AudioSpeakerBufferSpaceUsed(void)
{
    if (g_deviceAudioComposite->audioUnified.tdReadNumberPlay > g_deviceAudioComposite->audioUnified.tdWriteNumberPlay)
    {
        g_deviceAudioComposite->audioUnified.speakerReservedSpace =
            g_deviceAudioComposite->audioUnified.tdReadNumberPlay -
            g_deviceAudioComposite->audioUnified.tdWriteNumberPlay;
    }
    else
    {
        g_deviceAudioComposite->audioUnified.speakerReservedSpace =
            g_deviceAudioComposite->audioUnified.tdReadNumberPlay +
            AUDIO_SPEAKER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_OUT_ENDP_PACKET_SIZE -
            g_deviceAudioComposite->audioUnified.tdWriteNumberPlay;
    }
    return g_deviceAudioComposite->audioUnified.speakerReservedSpace;
}

/* The USB_AudioRecorderBufferSpaceUsed() function gets the reserved recorder ringbuffer size */
uint32_t USB_AudioRecorderBufferSpaceUsed(void)
{
    if (g_deviceAudioComposite->audioUnified.tdReadNumberRec > g_deviceAudioComposite->audioUnified.tdWriteNumberRec)
    {
        g_deviceAudioComposite->audioUnified.recorderReservedSpace =
            g_deviceAudioComposite->audioUnified.tdReadNumberRec -
            g_deviceAudioComposite->audioUnified.tdWriteNumberRec;
    }
    else
    {
        g_deviceAudioComposite->audioUnified.recorderReservedSpace =
            g_deviceAudioComposite->audioUnified.tdReadNumberRec +
            AUDIO_RECORDER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_IN_ENDP_PACKET_SIZE -
            g_deviceAudioComposite->audioUnified.tdWriteNumberRec;
    }
    return g_deviceAudioComposite->audioUnified.recorderReservedSpace;
}

/* The USB_AudioRecorderGetBuffer() function gets audioRecPacket from the audioRecDataBuff in every callback*/
void USB_AudioRecorderGetBuffer(uint8_t *buffer, uint32_t size)
{
    while (size)
    {
        *buffer = audioRecDataBuff[g_deviceAudioComposite->audioUnified.tdWriteNumberRec];
        g_deviceAudioComposite->audioUnified.tdWriteNumberRec++;
        buffer++;
        size--;

        if (g_deviceAudioComposite->audioUnified.tdWriteNumberRec >=
            AUDIO_RECORDER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_IN_ENDP_PACKET_SIZE)
        {
            g_deviceAudioComposite->audioUnified.tdWriteNumberRec = 0;
        }
    }
}

/* The USB_AudioSpeakerPutBuffer() function fills the audioRecDataBuff with audioPlayPacket in every callback*/
void USB_AudioSpeakerPutBuffer(uint8_t *buffer, uint32_t size)
{
    while (size)
    {
        audioPlayDataBuff[g_deviceAudioComposite->audioUnified.tdReadNumberPlay] = *buffer;
        g_deviceAudioComposite->audioUnified.tdReadNumberPlay++;
        buffer++;
        size--;

        if (g_deviceAudioComposite->audioUnified.tdReadNumberPlay >=
            AUDIO_SPEAKER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_OUT_ENDP_PACKET_SIZE)
        {
            g_deviceAudioComposite->audioUnified.tdReadNumberPlay = 0;
        }
    }
}

#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
#else
/* The USB_AudioFeedbackDataUpdate() function calculates the feedback data */
void USB_AudioFeedbackDataUpdate()
{
    static int32_t audioSpeakerUsedDiff = 0x0, audioSpeakerDiffThres = 0x0;
    static uint32_t feedbackValue = 0x0, originFeedbackValue = 0x0, audioSpeakerUsedSpace = 0x0,
                    audioSpeakerLastUsedSpace = 0x0;

    if (g_deviceAudioComposite->audioUnified.speakerIntervalCount != AUDIO_CALCULATE_Ff_INTERVAL)
    {
        g_deviceAudioComposite->audioUnified.speakerIntervalCount++;
        return;
    }
    g_deviceAudioComposite->audioUnified.speakerIntervalCount = 1;

    g_deviceAudioComposite->audioUnified.timesFeedbackCalculate++;
    if (g_deviceAudioComposite->audioUnified.timesFeedbackCalculate == 2)
    {
        originFeedbackValue = ((g_deviceAudioComposite->audioUnified.audioSendCount -
                                g_deviceAudioComposite->audioUnified.lastAudioSendCount)
                               << 4) /
                              (AUDIO_FORMAT_CHANNELS * AUDIO_FORMAT_SIZE);
        feedbackValue = originFeedbackValue;
        AUDIO_UPDATE_FEEDBACK_DATA(audioFeedBackBuffer, originFeedbackValue);
        audioSpeakerUsedSpace     = USB_AudioSpeakerBufferSpaceUsed();
        audioSpeakerLastUsedSpace = audioSpeakerUsedSpace;
    }
    else if (g_deviceAudioComposite->audioUnified.timesFeedbackCalculate > 2)
    {
        audioSpeakerUsedSpace = USB_AudioSpeakerBufferSpaceUsed();
        audioSpeakerUsedDiff += (audioSpeakerUsedSpace - audioSpeakerLastUsedSpace);
        audioSpeakerLastUsedSpace = audioSpeakerUsedSpace;

        if ((audioSpeakerUsedDiff > -AUDIO_SAMPLING_RATE_KHZ) && (audioSpeakerUsedDiff < AUDIO_SAMPLING_RATE_KHZ))
        {
            audioSpeakerDiffThres = 4 * AUDIO_SAMPLING_RATE_KHZ;
        }
        if (audioSpeakerUsedDiff <= -audioSpeakerDiffThres)
        {
            audioSpeakerDiffThres += 4 * AUDIO_SAMPLING_RATE_KHZ;
            feedbackValue += (AUDIO_SAMPLING_RATE_KHZ / AUDIO_SAMPLING_RATE_16KHZ) * (AUDIO_ADJUST_MIN_STEP);
        }
        if (audioSpeakerUsedDiff >= audioSpeakerDiffThres)
        {
            audioSpeakerDiffThres += 4 * AUDIO_SAMPLING_RATE_KHZ;
            feedbackValue -= (AUDIO_SAMPLING_RATE_KHZ / AUDIO_SAMPLING_RATE_16KHZ) * (AUDIO_ADJUST_MIN_STEP);
        }
        AUDIO_UPDATE_FEEDBACK_DATA(audioFeedBackBuffer, feedbackValue);
    }
    else
    {
    }
    g_deviceAudioComposite->audioUnified.lastAudioSendCount = g_deviceAudioComposite->audioUnified.audioSendCount;
}

/* The USB_RecorderDataMatch() function increase/decrease the adjusted packet interval according to the reserved
 * ringbuffer size */
uint32_t USB_RecorderDataMatch(uint32_t reservedspace)
{
    uint32_t epPacketSize = 0;
    if (reservedspace >= AUDIO_BUFFER_UPPER_LIMIT(AUDIO_RECORDER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_IN_ENDP_PACKET_SIZE))
    {
        epPacketSize = FS_ISO_IN_ENDP_PACKET_SIZE + AUDIO_FORMAT_SIZE * AUDIO_FORMAT_CHANNELS;
    }
    else if ((reservedspace >=
              AUDIO_BUFFER_LOWER_LIMIT(AUDIO_RECORDER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_IN_ENDP_PACKET_SIZE)) &&
             (reservedspace <
              AUDIO_BUFFER_UPPER_LIMIT(AUDIO_RECORDER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_IN_ENDP_PACKET_SIZE)))
    {
        epPacketSize = FS_ISO_IN_ENDP_PACKET_SIZE;
    }
    else if (reservedspace <
             AUDIO_BUFFER_LOWER_LIMIT(AUDIO_RECORDER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_IN_ENDP_PACKET_SIZE))
    {
        epPacketSize = FS_ISO_IN_ENDP_PACKET_SIZE - AUDIO_FORMAT_SIZE * AUDIO_FORMAT_CHANNELS;
    }
    else
    {
    }
    return epPacketSize;
}
#endif
/*!
 * @brief device Audio callback function.
 *
 * This function handle the Audio class specified event.
 * @param handle          The USB class  handle.
 * @param event           The USB device event type.
 * @param param           The parameter of the class specific event.
 * @return kStatus_USB_Success or error.
 */
usb_status_t USB_DeviceAudioCompositeCallback(class_handle_t handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Error;
    usb_device_endpoint_callback_message_struct_t *ep_cb_param;
    ep_cb_param           = (usb_device_endpoint_callback_message_struct_t *)param;
    uint32_t epPacketSize = FS_ISO_IN_ENDP_PACKET_SIZE;

    switch (event)
    {
        case kUSB_DeviceAudioEventStreamSendResponse:
            if ((g_deviceAudioComposite->audioUnified.attach) && (ep_cb_param->length != (USB_UNINITIALIZED_VAL_32)))
            {
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
                if (0)
                {
                }
#else
                if (ep_cb_param->length == ((USB_SPEED_HIGH == g_composite.speed) ? HS_ISO_FEEDBACK_ENDP_PACKET_SIZE :
                                                                                    FS_ISO_FEEDBACK_ENDP_PACKET_SIZE))
                {
                    error =
                        USB_DeviceAudioSend(g_deviceAudioComposite->audioUnified.audioSpeakerHandle,
                                            USB_AUDIO_SPEAKER_FEEDBACK_ENDPOINT, audioFeedBackBuffer,
                                            (USB_SPEED_HIGH == g_composite.speed) ? HS_ISO_FEEDBACK_ENDP_PACKET_SIZE :
                                                                                    FS_ISO_FEEDBACK_ENDP_PACKET_SIZE);
                }
#endif
                else
                {
                    if (g_deviceAudioComposite->audioUnified.startRec == 0)
                    {
                        g_deviceAudioComposite->audioUnified.startRec = 1;
                    }
                    if ((g_deviceAudioComposite->audioUnified.tdReadNumberRec >=
                         AUDIO_RECORDER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_IN_ENDP_PACKET_SIZE / 2) &&
                        (g_deviceAudioComposite->audioUnified.startRecHalfFull == 0))
                    {
                        g_deviceAudioComposite->audioUnified.startRecHalfFull = 1;
                    }
                    if (g_deviceAudioComposite->audioUnified.startRecHalfFull)
                    {
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
                        USB_AudioRecorderGetBuffer(audioRecPacket, epPacketSize);
                        error = USB_DeviceAudioSend(g_deviceAudioComposite->audioUnified.audioRecorderHandle,
                                                    USB_AUDIO_RECORDER_STREAM_ENDPOINT, &audioRecPacket[0],
                                                    FS_ISO_IN_ENDP_PACKET_SIZE);
#else
                        USB_AUDIO_ENTER_CRITICAL();
                        epPacketSize = USB_RecorderDataMatch(USB_AudioRecorderBufferSpaceUsed());
                        USB_AUDIO_EXIT_CRITICAL();

                        USB_AudioRecorderGetBuffer(audioRecPacket, epPacketSize);

                        error =
                            USB_DeviceAudioSend(g_deviceAudioComposite->audioUnified.audioRecorderHandle,
                                                USB_AUDIO_RECORDER_STREAM_ENDPOINT, &audioRecPacket[0], epPacketSize);
#endif

                        g_deviceAudioComposite->audioUnified.usbSendTimes++;
                    }
                    else
                    {
                        error = USB_DeviceAudioSend(g_deviceAudioComposite->audioUnified.audioRecorderHandle,
                                                    USB_AUDIO_RECORDER_STREAM_ENDPOINT, &audioRecDataBuff[0],
                                                    FS_ISO_IN_ENDP_PACKET_SIZE);
                    }
                }
            }
            break;
        case kUSB_DeviceAudioEventStreamRecvResponse:
            if ((g_deviceAudioComposite->audioUnified.attach) && (ep_cb_param->length != (USB_UNINITIALIZED_VAL_32)))
            {
                if (g_deviceAudioComposite->audioUnified.startPlay == 0)
                {
                    g_deviceAudioComposite->audioUnified.startPlay = 1;
                }
                if ((g_deviceAudioComposite->audioUnified.tdReadNumberPlay >=
                     AUDIO_SPEAKER_DATA_WHOLE_BUFFER_LENGTH * FS_ISO_OUT_ENDP_PACKET_SIZE / 2) &&
                    (g_deviceAudioComposite->audioUnified.startPlayHalfFull == 0))
                {
                    g_deviceAudioComposite->audioUnified.startPlayHalfFull = 1;
                }
                USB_AudioSpeakerPutBuffer(audioPlayPacket, ep_cb_param->length);
                g_deviceAudioComposite->audioUnified.usbRecvCount += ep_cb_param->length;
                g_deviceAudioComposite->audioUnified.usbRecvTimes++;
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
                error = USB_DeviceAudioRecv(handle, USB_AUDIO_SPEAKER_STREAM_ENDPOINT, &audioPlayPacket[0],
                                            (FS_ISO_OUT_ENDP_PACKET_SIZE));
#else
                USB_AUDIO_ENTER_CRITICAL();
                USB_AudioFeedbackDataUpdate();
                USB_AUDIO_EXIT_CRITICAL();
                error = USB_DeviceAudioRecv(handle, USB_AUDIO_SPEAKER_STREAM_ENDPOINT, &audioPlayPacket[0],
                                            (FS_ISO_OUT_ENDP_PACKET_SIZE + AUDIO_FORMAT_CHANNELS * AUDIO_FORMAT_SIZE));
#endif
            }
            break;

        default:
            if (param && (event > 0xFF))
            {
                error = USB_DeviceAudioRequest(handle, event, param);
            }
            break;
    }

    return error;
}

/* The USB_DeviceAudioRecorderStatusReset() function resets the audio recorder status to the initialized status */
void USB_DeviceAudioRecorderStatusReset(void)
{
    g_deviceAudioComposite->audioUnified.startRec              = 0;
    g_deviceAudioComposite->audioUnified.startRecHalfFull      = 0;
    g_deviceAudioComposite->audioUnified.audioRecvCount        = 0;
    g_deviceAudioComposite->audioUnified.usbSendTimes          = 0;
    g_deviceAudioComposite->audioUnified.tdReadNumberRec       = 0;
    g_deviceAudioComposite->audioUnified.tdWriteNumberRec      = 0;
    g_deviceAudioComposite->audioUnified.recorderReservedSpace = 0;
}

/* The USB_DeviceAudioSpeakerStatusReset() function resets the audio speaker status to the initialized status */
void USB_DeviceAudioSpeakerStatusReset(void)
{
    g_deviceAudioComposite->audioUnified.startPlay              = 0;
    g_deviceAudioComposite->audioUnified.startPlayHalfFull      = 0;
    g_deviceAudioComposite->audioUnified.tdReadNumberPlay       = 0;
    g_deviceAudioComposite->audioUnified.tdWriteNumberPlay      = 0;
    g_deviceAudioComposite->audioUnified.audioSendCount         = 0;
    g_deviceAudioComposite->audioUnified.usbRecvCount           = 0;
    g_deviceAudioComposite->audioUnified.lastAudioSendCount     = 0;
    g_deviceAudioComposite->audioUnified.audioSendTimes         = 0;
    g_deviceAudioComposite->audioUnified.usbRecvTimes           = 0;
    g_deviceAudioComposite->audioUnified.speakerIntervalCount   = 0;
    g_deviceAudioComposite->audioUnified.speakerReservedSpace   = 0;
    g_deviceAudioComposite->audioUnified.timesFeedbackCalculate = 0;
    g_deviceAudioComposite->audioUnified.speakerDetachOrNoInput = 0;
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
    g_deviceAudioComposite->audioUnified.audioPllTicksPrev   = 0;
    g_deviceAudioComposite->audioUnified.audioPllTicksDiff   = 0;
    g_deviceAudioComposite->audioUnified.audioPllTicksEma    = AUDIO_PLL_USB1_SOF_INTERVAL_COUNT;
    g_deviceAudioComposite->audioUnified.audioPllTickEmaFrac = 0;
    g_deviceAudioComposite->audioUnified.audioPllStep        = AUDIO_PLL_FRACTIONAL_CHANGE_STEP;
#endif
}

/*!
 * @brief Audio set configuration function.
 *
 * This function sets configuration for msc class.
 *
 * @param handle The Audio class handle.
 * @param configure The Audio class configure index.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceAudioCompositeSetConfigure(class_handle_t handle, uint8_t configure)
{
    if (USB_COMPOSITE_CONFIGURE_INDEX == configure)
    {
        g_deviceAudioComposite->audioUnified.attach = 1U;
    }
    return kStatus_USB_Success;
}

usb_status_t USB_DeviceAudioRecorderSetInterface(class_handle_t handle, uint8_t interface, uint8_t alternateSetting)
{
    usb_status_t error = kStatus_USB_Success;

    if (alternateSetting == 1U)
    {
        USB_DeviceAudioRecorderStatusReset();
        error =
            USB_DeviceAudioSend(g_deviceAudioComposite->audioUnified.audioRecorderHandle,
                                USB_AUDIO_RECORDER_STREAM_ENDPOINT, &audioRecDataBuff[0], FS_ISO_IN_ENDP_PACKET_SIZE);
    }
    return error;
}

usb_status_t USB_DeviceAudioSpeakerSetInterface(class_handle_t handle, uint8_t interface, uint8_t alternateSetting)
{
    usb_status_t error = kStatus_USB_Success;

    if (alternateSetting == 1U)
    {
        USB_DeviceAudioSpeakerStatusReset();
        error =
            USB_DeviceAudioRecv(g_deviceAudioComposite->audioUnified.audioSpeakerHandle,
                                USB_AUDIO_SPEAKER_STREAM_ENDPOINT, &audioPlayDataBuff[0], FS_ISO_OUT_ENDP_PACKET_SIZE);
        if (error != kStatus_USB_Success)
        {
            return error;
        }
        else
        {
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
#else
            error = USB_DeviceAudioSend(g_deviceAudioComposite->audioUnified.audioSpeakerHandle,
                                        USB_AUDIO_SPEAKER_FEEDBACK_ENDPOINT, audioFeedBackBuffer,
                                        (USB_SPEED_HIGH == g_composite.speed) ? HS_ISO_FEEDBACK_ENDP_PACKET_SIZE :
                                                                                FS_ISO_FEEDBACK_ENDP_PACKET_SIZE);
#endif
        }
    }
    return error;
}
/*!
 * @brief Audio init function.
 *
 * This function initializes the device with the composite device class information.
 *
 * @param device_composite          The pointer to the composite device structure.
 * @return kStatus_USB_Success .
 */
usb_status_t USB_DeviceAudioCompositeInit(usb_device_composite_struct_t *device_composite)
{
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
    SCTIMER_CaptureInit();
#else
    AUDIO_UPDATE_FEEDBACK_DATA(audioFeedBackBuffer, AUDIO_SAMPLING_RATE_TO_10_14);
#endif
    g_deviceAudioComposite                                       = device_composite;
    g_deviceAudioComposite->audioUnified.copyProtect             = 0x01U;
    g_deviceAudioComposite->audioUnified.curMute                 = 0x00U;
    g_deviceAudioComposite->audioUnified.curVolume[0]            = 0x00U;
    g_deviceAudioComposite->audioUnified.curVolume[1]            = 0x1fU;
    g_deviceAudioComposite->audioUnified.minVolume[0]            = 0x00U;
    g_deviceAudioComposite->audioUnified.minVolume[1]            = 0x00U;
    g_deviceAudioComposite->audioUnified.maxVolume[0]            = 0x00U;
    g_deviceAudioComposite->audioUnified.maxVolume[1]            = 0X43U;
    g_deviceAudioComposite->audioUnified.resVolume[0]            = 0x01U;
    g_deviceAudioComposite->audioUnified.resVolume[1]            = 0x00U;
    g_deviceAudioComposite->audioUnified.curBass                 = 0x00U;
    g_deviceAudioComposite->audioUnified.curBass                 = 0x00U;
    g_deviceAudioComposite->audioUnified.minBass                 = 0x80U;
    g_deviceAudioComposite->audioUnified.maxBass                 = 0x7FU;
    g_deviceAudioComposite->audioUnified.resBass                 = 0x01U;
    g_deviceAudioComposite->audioUnified.curMid                  = 0x00U;
    g_deviceAudioComposite->audioUnified.minMid                  = 0x80U;
    g_deviceAudioComposite->audioUnified.maxMid                  = 0x7FU;
    g_deviceAudioComposite->audioUnified.resMid                  = 0x01U;
    g_deviceAudioComposite->audioUnified.curTreble               = 0x01U;
    g_deviceAudioComposite->audioUnified.minTreble               = 0x80U;
    g_deviceAudioComposite->audioUnified.maxTreble               = 0x7FU;
    g_deviceAudioComposite->audioUnified.resTreble               = 0x01U;
    g_deviceAudioComposite->audioUnified.curAutomaticGain        = 0x01U;
    g_deviceAudioComposite->audioUnified.curDelay[0]             = 0x00U;
    g_deviceAudioComposite->audioUnified.curDelay[1]             = 0x40U;
    g_deviceAudioComposite->audioUnified.minDelay[0]             = 0x00U;
    g_deviceAudioComposite->audioUnified.minDelay[1]             = 0x00U;
    g_deviceAudioComposite->audioUnified.maxDelay[0]             = 0xFFU;
    g_deviceAudioComposite->audioUnified.maxDelay[1]             = 0xFFU;
    g_deviceAudioComposite->audioUnified.resDelay[0]             = 0x00U;
    g_deviceAudioComposite->audioUnified.resDelay[1]             = 0x01U;
    g_deviceAudioComposite->audioUnified.curLoudness             = 0x01U;
    g_deviceAudioComposite->audioUnified.curSamplingFrequency[0] = 0x00U;
    g_deviceAudioComposite->audioUnified.curSamplingFrequency[1] = 0x00U;
    g_deviceAudioComposite->audioUnified.curSamplingFrequency[2] = 0x01U;
    g_deviceAudioComposite->audioUnified.minSamplingFrequency[0] = 0x00U;
    g_deviceAudioComposite->audioUnified.minSamplingFrequency[1] = 0x00U;
    g_deviceAudioComposite->audioUnified.minSamplingFrequency[2] = 0x01U;
    g_deviceAudioComposite->audioUnified.maxSamplingFrequency[0] = 0x00U;
    g_deviceAudioComposite->audioUnified.maxSamplingFrequency[1] = 0x00U;
    g_deviceAudioComposite->audioUnified.maxSamplingFrequency[2] = 0x01U;
    g_deviceAudioComposite->audioUnified.resSamplingFrequency[0] = 0x00U;
    g_deviceAudioComposite->audioUnified.resSamplingFrequency[1] = 0x00U;
    g_deviceAudioComposite->audioUnified.resSamplingFrequency[2] = 0x01U;
    g_deviceAudioComposite->audioUnified.tdReadNumberPlay        = 0;
    g_deviceAudioComposite->audioUnified.tdWriteNumberPlay       = 0;
    g_deviceAudioComposite->audioUnified.tdReadNumberRec         = 0;
    g_deviceAudioComposite->audioUnified.tdWriteNumberRec        = 0;
    g_deviceAudioComposite->audioUnified.audioSendCount          = 0;
    g_deviceAudioComposite->audioUnified.lastAudioSendCount      = 0;
    g_deviceAudioComposite->audioUnified.usbRecvCount            = 0;
    g_deviceAudioComposite->audioUnified.audioSendTimes          = 0;
    g_deviceAudioComposite->audioUnified.usbRecvTimes            = 0;
    g_deviceAudioComposite->audioUnified.audioRecvCount          = 0;
    g_deviceAudioComposite->audioUnified.usbSendTimes            = 0;
    g_deviceAudioComposite->audioUnified.startPlay               = 0;
    g_deviceAudioComposite->audioUnified.startPlayHalfFull       = 0;
    g_deviceAudioComposite->audioUnified.startRec                = 0;
    g_deviceAudioComposite->audioUnified.startRecHalfFull        = 0;
    g_deviceAudioComposite->audioUnified.speakerIntervalCount    = 0;
    g_deviceAudioComposite->audioUnified.speakerReservedSpace    = 0;
    g_deviceAudioComposite->audioUnified.recorderReservedSpace   = 0;
    g_deviceAudioComposite->audioUnified.timesFeedbackCalculate  = 0;
    g_deviceAudioComposite->audioUnified.speakerDetachOrNoInput  = 0;
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
    g_deviceAudioComposite->audioUnified.audioPllTicksPrev   = 0;
    g_deviceAudioComposite->audioUnified.audioPllTicksDiff   = 0;
    g_deviceAudioComposite->audioUnified.audioPllTicksEma    = AUDIO_PLL_USB1_SOF_INTERVAL_COUNT;
    g_deviceAudioComposite->audioUnified.audioPllTickEmaFrac = 0;
    g_deviceAudioComposite->audioUnified.audioPllStep        = AUDIO_PLL_FRACTIONAL_CHANGE_STEP;
#endif
    return kStatus_USB_Success;
}

void USB_AudioCodecTask(void)
{
    if (g_deviceAudioComposite->audioUnified.codecTask & MUTE_CODEC_TASK)
    {
        usb_echo("Set Cur Mute : %x\r\n", g_deviceAudioComposite->audioUnified.curMute);
        BOARD_SetCodecMuteUnmute(true);
        g_deviceAudioComposite->audioUnified.codecTask &= ~MUTE_CODEC_TASK;
        g_CodecMuteUnmute = true;
    }
    if (g_deviceAudioComposite->audioUnified.codecTask & UNMUTE_CODEC_TASK)
    {
        usb_echo("Set Cur Mute : %x\r\n", g_deviceAudioComposite->audioUnified.curMute);
        BOARD_SetCodecMuteUnmute(false);
        g_deviceAudioComposite->audioUnified.codecTask &= ~UNMUTE_CODEC_TASK;
        g_CodecMuteUnmute = true;
    }
    if (g_deviceAudioComposite->audioUnified.codecTask & VOLUME_CHANGE_TASK)
    {
        usb_echo("Set Cur Volume : %x\r\n", (uint16_t)(g_deviceAudioComposite->audioUnified.curVolume[1] << 8U) |
                                                g_deviceAudioComposite->audioUnified.curVolume[0]);
        g_deviceAudioComposite->audioUnified.codecTask &= ~VOLUME_CHANGE_TASK;
    }
}

void USB_AudioSpeakerResetTask(void)
{
    if (g_deviceAudioComposite->audioUnified.speakerDetachOrNoInput)
    {
        USB_DeviceAudioSpeakerStatusReset();
    }
}
