/*
 * Copyright 2018-2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AUDIO_SINK_DEFAULT_H
#define AUDIO_SINK_DEFAULT_H

/*!
 * @file    audio_sink_default.h
 * @brief   This file contains the definitions for the
 *          PCM manager Audio Sink Interface
 */

#include "audio_sink.h"

#include "streamer_pcm.h"

/**
 * @brief Audio sink buffer number
 *
 */
#if !defined(AUDIO_SINK_BUFFER_NUM)
#if defined(SAI_XFER_QUEUE_SIZE)
#define AUDIO_SINK_BUFFER_NUM (SAI_XFER_QUEUE_SIZE + 1)
#elif defined(I2S_NUM_BUFFERS)
#define AUDIO_SINK_BUFFER_NUM (I2S_NUM_BUFFERS + 1)
#else
#define AUDIO_SINK_BUFFER_NUM (3)
#endif
#endif

/**
 * @brief Minimum audio buffer size
 *
 */
#define MIN_AUDIO_BUFFER_SIZE (4 * 1024U)
/**
 * @brief Buffer size alignment
 *
 */
#define SIZE_ALIGNMENT 32U

/**
 * @brief PCM Sink device information
 *
 */
typedef struct _DefaultSinkDeviceInfo
{
    char *unaligned_buf[AUDIO_SINK_BUFFER_NUM]; /*!< @brief Pointers to unaligned audio buffers */
    char *audbuf[AUDIO_SINK_BUFFER_NUM];        /*!< @brief Pointers to aligned audio buffers */
    uint32_t alloc_size;                        /*!< @brief Unaligned_buf allocated size */
    uint32_t input_size;                        /*!< @brief Input size */
    uint8_t input_index;                        /*!< @brief Input index */
    uint8_t device_state;                       /*!< @brief Device state */
    bool init_params_done;                      /*!< @brief Initialization done flag */
    bool resample;                              /*!< @brief Resampling flag */
} DefaultSinkDeviceInfo;

/**
 * @brief Audio sink default init parameters
 *
 * @param audio_sink_ptr Audio sink pointer
 * @return AudioSinkStreamErrorType
 */
AudioSinkStreamErrorType audiosink_default_init_params(ElementAudioSink *audio_sink_ptr);

/**
 * @brief Audio sink default init device
 *
 * @param audio_sink_ptr Audio sink pointer
 * @return AudioSinkStreamErrorType
 */
AudioSinkStreamErrorType audiosink_default_init_device(ElementAudioSink *audio_sink_ptr);

/**
 * @brief Audio sink default deinit device
 *
 * @param audio_sink_ptr Audio sink pointer
 * @return AudioSinkStreamErrorType
 */
AudioSinkStreamErrorType audiosink_default_deinit_device(ElementAudioSink *audio_sink_ptr);

/**
 * @brief Audio sink default sink pad activation handler
 *
 * @param pad    Pad to be activated/deactivated
 * @param active Activate/deactivate
 * @return uint8_t
 */
uint8_t audiosink_default_sink_pad_activation_handler(StreamPad *pad, uint8_t active);

/**
 * @brief Audio sink default sink pad chain handler
 * @details Handle the data buffer and send it to the appropriate audio device
 *
 * @param pad    Pad pointer
 * @param buffer Buffer pointer
 * @return FlowReturn
 */
FlowReturn audiosink_default_sink_pad_chain_handler(StreamPad *pad, StreamBuffer *buffer);

/**
 * @brief Audio sink default pad event handler
 *
 * @param pad   Pad pointer
 * @param event Event to be handled
 * @return uint8_t
 */
uint8_t audiosink_default_sink_pad_event_handler(StreamPad *pad, StreamEvent *event);

/**
 * @brief Audio sink default pad query handler
 *
 * @param pad   Pad pointer
 * @param query Query pointer
 * @return uint8_t
 */
uint8_t audiosink_default_sink_pad_query_handler(StreamPad *pad, StreamQuery *query);

/**
 * @brief Audio sink default start device
 *
 * @param audio_sink_ptr Audio sink pointer
 * @return AudioSinkStreamErrorType
 */
AudioSinkStreamErrorType audiosink_default_start_device(ElementAudioSink *audio_sink_ptr);

/**
 * @brief Audio sink default stop device
 *
 * @param audio_sink_ptr Audio sink pointer
 * @return AudioSinkStreamErrorType
 */
AudioSinkStreamErrorType audiosink_default_stop_device(ElementAudioSink *audio_sink_ptr);

/**
 * @brief Audio sink default play device
 *
 * @param audio_sink_ptr Audio sink pointer
 * @return AudioSinkStreamErrorType
 */
AudioSinkStreamErrorType audiosink_default_play_device(ElementAudioSink *audio_sink_ptr);

/**
 * @brief Audio sink default pause device
 *
 * @param audio_sink_ptr Audio sink pointer
 * @return AudioSinkStreamErrorType
 */
AudioSinkStreamErrorType audiosink_default_pause_device(ElementAudioSink *audio_sink_ptr);

/**
 * @brief Audio sink default set volume
 *
 * @param audio_sink_ptr Audio sink pointer
 * @param volume Volume to be set
 * @return AudioSinkStreamErrorType
 */
AudioSinkStreamErrorType audiosink_default_set_volume(ElementAudioSink *audio_sink_ptr, int volume);

#endif
