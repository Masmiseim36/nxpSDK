/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __USB_AUDIO_SPEAKER_H__
#define __USB_AUDIO_SPEAKER_H__ 1U

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
#define CONTROLLER_ID kUSB_ControllerEhci0
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
#define CONTROLLER_ID kUSB_ControllerKhci0
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Fs0
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Hs0
#endif

/* the threshold transfer count that can tolerance by frame */
#define USB_AUDIO_PLAY_BUFFER_TOLERANCE_THRESHOLD (4U)

#define AUDIO_SAMPLING_RATE_KHZ                 (48)
#define AUDIO_SAMPLING_RATE_16KHZ               (16)
#define AUDIO_SAMPLING_RATE                     (AUDIO_SAMPLING_RATE_KHZ * 1000)
#define AUDIO_RECORDER_DATA_WHOLE_BUFFER_LENGTH (16 * 2)
#define AUDIO_SPEAKER_DATA_WHOLE_BUFFER_LENGTH  (16 * 2)

/* feedback calculate interval */
#define AUDIO_CALCULATE_Ff_INTERVAL (16U)

/* For ip3511hs in high speed mode, microframe can not be obtained and only for frame, the used feedback solution
 * requires us to have to use larger latency and buffer size to avoid buffer overflow or underflow. Sync mode can use
 * low latency (<1ms) even if on ip3511hs */
#if (USB_DEVICE_CONFIG_AUDIO_CLASS_2_0)
#if (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
#define AUDIO_CLASS_2_0_HS_LOW_LATENCY_TRANSFER_COUNT \
    (0x06U) /* 6 means 16 mico frames (6*125us), make sure the latency is smaller than 1ms for sync mode */
#define AUDIO_CLASS_2_0_HS_LOW_LATENCY_BUFFER_COUNT \
    (2U) /* 2 units size buffer (1 unit means the size to play during 1ms) */
#else
#define AUDIO_CLASS_2_0_HS_LOW_LATENCY_TRANSFER_COUNT (0x10U) /* 0x10 means 16 mico frames (16*125us, 2ms) */
#define AUDIO_CLASS_2_0_HS_LOW_LATENCY_BUFFER_COUNT \
    (6U) /* 6 units size buffer (1 unit means the size to play during 1ms) */
#endif
#elif (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U))
#define AUDIO_CLASS_2_0_HS_LOW_LATENCY_TRANSFER_COUNT \
    (0x06U) /* 6 means 16 mico frames (6*125us), make sure the latency is smaller than 1ms for ehci high speed */
#define AUDIO_CLASS_2_0_HS_LOW_LATENCY_BUFFER_COUNT \
    (2U) /* 2 units size buffer (1 unit means the size to play during 1ms) */
#endif
#endif

#define TSAMFREQ2BYTES(f)     (f & 0xFFU), ((f >> 8U) & 0xFFU), ((f >> 16U) & 0xFFU)
#define TSAMFREQ2BYTESHS(f)   (f & 0xFFU), ((f >> 8U) & 0xFFU), ((f >> 16U) & 0xFFU), ((f >> 24U) & 0xFFU)
#define AUDIO_ADJUST_MIN_STEP (0x01)

#if defined(USB_AUDIO_CHANNEL5_1) && (USB_AUDIO_CHANNEL5_1 > 0U)
#define AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME (AUDIO_OUT_TRANSFER_LENGTH_ONE_FRAME / 3)
#else
#define AUDIO_PLAY_BUFFER_SIZE_ONE_FRAME AUDIO_OUT_TRANSFER_LENGTH_ONE_FRAME
#endif

#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
/**********************************************************************
Audio PLL contants
      AUDIO_PLL_USB1_SOF_INTERVAL_COUNT
      The Audio PLL clock is 24.576Mhz, and the USB1_SOF_TOGGLE frequency is 4kHz when the device is attached,
      so AUDIO_PLL_USB1_SOF_INTERVAL_COUNT = (24576000 * AUDIO_CALCULATE_Ff_INTERVAL (stands for counter interval, 16))
/4000 = 98304 /4000 = 98304 AUDIO_PLL_FRACTIONAL_CHANGE_STEP The Audio input clock is 24Mhz, and denominator is 4500,
divider is 15 and PFD is 26. so AUDIO_PLL_FRACTIONAL_CHANGE_STEP = (24000000 * AUDIO_CALCULATE_Ff_INTERVAL (stands for
counter interval, 16) * 18) / (27000 * 26 * 15 *4000) + 1
**********************************************************************/
#define AUDIO_PLL_USB1_SOF_INTERVAL_COUNT  (98304)  /* The USB1_SOF_TOGGLE's frequency is 4kHz. */
#define AUDIO_PLL_USB1_SOF_INTERVAL_COUNT1 (491520) /* The USB1_SOF_TOGGLE's frequency is 4kHz. */
#define AUDIO_PLL_FRACTIONAL_CHANGE_STEP   (1)
#endif

#define MUTE_CODEC_TASK    (1UL << 0U)
#define UNMUTE_CODEC_TASK  (1UL << 1U)
#define VOLUME_CHANGE_TASK (1UL << 2U)

#define USB_DEVICE_INTERRUPT_PRIORITY (3U)
typedef struct _usb_audio_speaker_struct
{
    usb_device_handle deviceHandle;
    uint32_t currentStreamOutMaxPacketSize;
    uint32_t currentFeedbackMaxPacketSize;
    uint8_t currentStreamInterfaceAlternateSetting;
    uint8_t copyProtect;
    uint8_t curMute;
    uint8_t curVolume[2]; /* need to consider the endians */
    uint8_t minVolume[2]; /* need to consider the endians */
    uint8_t maxVolume[2]; /* need to consider the endians */
    uint8_t resVolume[2]; /* need to consider the endians */
    uint8_t curBass;
    uint8_t minBass;
    uint8_t maxBass;
    uint8_t resBass;
    uint8_t curMid;
    uint8_t minMid;
    uint8_t maxMid;
    uint8_t resMid;
    uint8_t curTreble;
    uint8_t minTreble;
    uint8_t maxTreble;
    uint8_t resTreble;
    uint8_t curAutomaticGain;
    uint8_t curDelay[2]; /* need to consider the endians */
    uint8_t minDelay[2]; /* need to consider the endians */
    uint8_t maxDelay[2]; /* need to consider the endians */
    uint8_t resDelay[2]; /* need to consider the endians */
    uint8_t curLoudness;
    uint8_t curSamplingFrequency[3]; /* need to consider the endians */
    uint8_t minSamplingFrequency[3]; /* need to consider the endians */
    uint8_t maxSamplingFrequency[3]; /* need to consider the endians */
    uint8_t resSamplingFrequency[3]; /* need to consider the endians */
#if (USB_DEVICE_CONFIG_AUDIO_CLASS_2_0)
    uint8_t curMute20;
    uint8_t curClockValid;
    uint8_t curVolume20[2];
    uint32_t curSampleFrequency;
    usb_device_control_range_layout3_struct_t freqControlRange;
    usb_device_control_range_layout2_struct_t volumeControlRange;
#endif
    uint8_t currentConfiguration;
    uint8_t currentInterfaceAlternateSetting[USB_AUDIO_SPEAKER_INTERFACE_COUNT];
    uint8_t speed;
    uint8_t attach;
    volatile uint8_t startPlayFlag;
    volatile uint32_t tdWriteNumberPlay;
    volatile uint32_t tdReadNumberPlay;
    volatile uint32_t audioSendCount;
    volatile uint32_t lastAudioSendCount;
    volatile uint32_t usbRecvCount;
    volatile uint32_t audioSendTimes;
    volatile uint32_t usbRecvTimes;
    volatile uint32_t speakerIntervalCount;
    volatile uint32_t speakerReservedSpace;
    volatile uint32_t speakerDetachOrNoInput;
    volatile uint32_t codecTask;
    uint32_t audioPlayTransferSize;
    volatile uint16_t audioPlayBufferSize;
#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
    volatile uint32_t curAudioPllFrac;
    volatile uint32_t audioPllTicksPrev;
    volatile int32_t audioPllTicksDiff;
    volatile int32_t audioPllTicksEma;
    volatile int32_t audioPllTickEmaFrac;
    volatile int32_t audioPllStep;
#else
    volatile uint32_t maxFrameCount;
    volatile uint32_t lastFrameCount;
    volatile uint32_t currentFrameCount;
    volatile uint8_t firstCalculateFeedback;
#endif

} usb_audio_speaker_struct_t;

#endif /* __USB_AUDIO_SPEAKER_H__ */
