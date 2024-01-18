/*
 * Copyright 2018-2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AUDIO_SOURCE_PCMRTOS_H
#define AUDIO_SOURCE_PCMRTOS_H

/*!
 *  @file    audio_src_pcmrtos.h
 *  @brief   This is the header file for the audio source element of pcmrtos.
 */

#include "audio_src.h"
#include "streamer_pcm.h"
#include "streamer.h"

/**
 * @brief Audio source buffer number
 *
 */
#if !defined(AUDIO_SRC_BUFFER_NUM)
#if defined(SAI_XFER_QUEUE_SIZE)
#define AUDIO_SRC_BUFFER_NUM (SAI_XFER_QUEUE_SIZE + 1)
#elif defined(I2S_NUM_BUFFERS)
#define AUDIO_SRC_BUFFER_NUM (I2S_NUM_BUFFERS + 1)
#else
#define AUDIO_SRC_BUFFER_NUM (3)
#endif
#endif

/*
 *  @brief Structure used for the synchronization
 *
 */
typedef struct _AudioSrcBufferQueue_T
{
#if AUDIO_SRC_BUFFER_NUM >= 4
    int8_t buffer_idx[AUDIO_SRC_BUFFER_NUM];
#else
    int8_t buffer_idx[5]; // More than SAI_XFER_QUEUE_SIZE or I2S_NUM_BUFFERS
#endif
    int8_t read_idx;
    int8_t write_idx;
    int8_t size;
} AudioSrcBufferQueue_t;

/**
 * @brief Buffer size alignment
 *
 */
#define SIZE_ALIGNMENT 32

/**
 * @brief PCM Source device information
 *
 */
typedef struct _PCMRtosDeviceInfo
{
    char *unaligned_buf[AUDIO_SRC_BUFFER_NUM]; /*!< @brief Pointers to unaligned buffers */
    char *audbuf[AUDIO_SRC_BUFFER_NUM];        /*!< @brief Pointers to aligned audio buffers */
    uint32_t buff_size[AUDIO_SRC_BUFFER_NUM];  /*!< @brief Size of the data in the audbuf */
    uint8_t buff_index;                        /*!< @brief Current buffer index */
    uint8_t device_state;                      /*!< @brief Device state */
    bool init_params_done;                     /*!< @brief Initialization done flag */
    bool continuous_read;                      /*!< @brief Continuous read flag */
    AudioSrcBufferQueue_t buffer_queue;        /*!< @brief Queue of the requested audio buffer index */
} PCMRtosDeviceInfo;

/**
 * @brief Audio source pcmrtos device initialization function
 *
 * @param audio_src_element Audio source pointer
 * @return AudioSrcStreamErrorType
 */
AudioSrcStreamErrorType audio_src_pcmrtos_init_device(ElementAudioSrc *audio_src_element);

/**
 * @brief Audio source pcmrtos device deinitialization function
 *
 * @param audio_src_element Audio source pointer
 * @return AudioSrcStreamErrorType
 */
AudioSrcStreamErrorType audio_src_pcmrtos_deinit_device(ElementAudioSrc *audio_src_element);

/**
 * @brief Audio source pcmrtos device read function
 *
 * @param audio_src_element Audio source pointer
 * @param length Length to be read
 * @param buf buffer pointer
 * @return AudioSrcStreamErrorType
 */
AudioSrcStreamErrorType audio_src_pcmrtos_read_device(ElementAudioSrc *audio_src_element,
                                                      uint32_t length,
                                                      StreamBuffer *buf);

/**
 * @brief Audio source pcmrtos device start device function
 *
 * @param audio_src_element Audio source pointer
 * @return AudioSrcStreamErrorType
 */
AudioSrcStreamErrorType audio_src_pcmrtos_start_device(ElementAudioSrc *audio_src_element);

/**
 * @brief Audio source pcmrtos device stop device function
 *
 * @param audio_src_element Audio source pointer
 * @return AudioSrcStreamErrorType
 */
AudioSrcStreamErrorType audio_src_pcmrtos_stop_device(ElementAudioSrc *audio_src_element);

#endif
