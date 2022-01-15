/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_AUDIO_CONFIG_H__
#define __USB_AUDIO_CONFIG_H__ 1U

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

/*! @brief Whether USB Audio use syn mode or not, note that some socs may not support sync mode */
#define USB_DEVICE_AUDIO_USE_SYNC_MODE (1U)

/* Packet size and interval.
 * note: if ISO out endpoint interval is changed, please change AUDIO_UPDATE_FEEDBACK_DATA accordingly.
 */
#define HS_ISO_OUT_ENDP_INTERVAL (0x04)

/* the threshold transfer count that can tolerance by frame */
#define AUDIO_SYNC_DATA_BASED_ADJUST_THRESHOLD (4U)

/* For ip3511hs in high speed mode, microframe can not be obtained and only for frame, the used feedback solution
 * requires us to have to use larger latency and buffer size to avoid buffer overflow or underflow. Sync mode can use
 * low latency (<1ms) even if on ip3511hs */
#define AUDIO_CLASS_2_0_HS_LOW_LATENCY_TRANSFER_COUNT (0x08) /* 0x08 means 8 frames (8ms) */
#define AUDIO_SPEAKER_DATA_WHOLE_BUFFER_COUNT \
    (600UL) /* 16 units size buffer (1 unit means the size to play during 1ms) */

#define AUDIO_SPEAKER_DATA_WHOLE_BUFFER_COUNT_NORMAL  (16 * 2)
#define AUDIO_RECORDER_DATA_WHOLE_BUFFER_COUNT_NORMAL (16 * 2)

#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
#else
/* feedback calculate interval */
#define AUDIO_CALCULATE_Ff_INTERVAL                        (16U)
/* the threshold transfer count that can tolerance by frame */
#define USB_AUDIO_PLAY_BUFFER_FEEDBACK_TOLERANCE_THRESHOLD (4U)
/* feedback value discard times, the first feedback vaules are discarded */
#define AUDIO_SPEAKER_FEEDBACK_DISCARD_COUNT               (4U)
#endif

/* pll adjustment and fro trim calculate interval */
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
#define AUDIO_PLL_ADJUST_INTERVAL (8U)
#else
#define AUDIO_PLL_ADJUST_INTERVAL (2U)
#endif
#define AUDIO_FRO_ADJUST_INTERVAL (2U)

#if defined(USB_DEVICE_AUDIO_USE_SYNC_MODE) && (USB_DEVICE_AUDIO_USE_SYNC_MODE > 0U)
/**********************************************************************
Audio PLL contants
      AUDIO_PLL_USB_SOF_INTERVAL_TICK_COUNT
      The Audio PLL clock is 24.576Mhz, and the USB SOF TOGGLE frequency is 1kHz when the full speed device is attached,
      so AUDIO_PLL_USB_SOF_INTERVAL_TICK_COUNT = (24576000 * AUDIO_PLL_ADJUST_INTERVAL (stands for counter
interval) / 1000 = 49152
**********************************************************************/
#if (defined USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS)
/* use FRO clock source */
#define AUDIO_PLL_FRACTIONAL_DIVIDER (3298534883U)
#elif (defined USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS)
/* use external OSC clock source */
#define AUDIO_PLL_FRACTIONAL_DIVIDER (5040U)
#endif

#if (defined USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS)
/* The USB_SOF_TOGGLE's frequency is 1kHz. */
#define AUDIO_PLL_USB_SOF_INTERVAL_TICK_COUNT (24576U * AUDIO_PLL_ADJUST_INTERVAL)
#define AUDIO_FRO_USB_SOF_INTERVAL_COUNT      (96000U * AUDIO_FRO_ADJUST_INTERVAL)
#elif (defined USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS)
/* The USB_SOF_TOGGLE's frequency is 4kHz. */
#define AUDIO_PLL_USB_SOF_INTERVAL_TICK_COUNT (24576U * AUDIO_PLL_ADJUST_INTERVAL / 4U)
#endif

#define AUDIO_PLL_FRACTION_TICK_BASED_PRECISION (1U)
#define AUDIO_USB_FRO_TRIM_TICK_BASED_PRECISION (3U)
#define AUDIO_FRO_TRIM_DATA_BASED_INTERVAL      (16U)
#define AUDIO_PLL_ADJUST_DATA_BASED_INTERVAL    (16U)
#if (defined USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS)
#define AUDIO_PLL_ADJUST_DATA_BASED_STEP (1U)
#elif (defined USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS)
#define AUDIO_PLL_ADJUST_DATA_BASED_STEP (1U)
#endif
#endif /* USB_DEVICE_AUDIO_USE_SYNC_MODE */

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
/*${prototype:end}*/

#endif /* _USB_AUDIO_CONFIG_H_ */
