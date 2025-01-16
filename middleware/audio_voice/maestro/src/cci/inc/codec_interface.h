/*
 * Copyright 2018-2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! @file codec_interface.h
 *  @brief Contains codec interface definitions.
 */
#ifndef __CODEC_INTERFACE_H__
#define __CODEC_INTERFACE_H__

/*
 *  Includes
 */
#include "codec_interface_public_api.h"
#if defined(__cplusplus)
extern "C" {
#endif
/*
 * Structure
 */

/*!
 * @brief Declares codec_get_memory_sizes_function_t as a ptr to a function that
 *                                                   calls codec get memory size
 */
typedef int32_t (*codec_get_memory_sizes_function_t)(void);
/*!
 * @brief Declares codec_init_function_t as a pointer to a function that calls
 *                                                                    codec init
 */
typedef int32_t (*codec_init_function_t)(int32_t *memory[], int32_t *callback_fn[], int32_t *user_data);
/*!
 * @brief Declares codec_decode_function_t as a pointer to a function that calls
 *                                                                  codec decode
 */
typedef int32_t (*codec_decode_function_t)(int32_t *memory[], int32_t *sample_produced, int32_t *buf_out);
/*!
 * @brief Declares codec_get_pcm_samples_function_t as a ptr to a function that
 *                                                   calls codec get pcm samples
 */
typedef int32_t (*codec_get_pcm_samples_function_t)(int32_t *memory[],
                                                    int32_t *sample_produced,
                                                    int32_t max_sample,
                                                    int32_t *buf_out);
/*!
 * @brief Declares codec_reset_function_t as a pointer to a function that calls
 *                                                                   codec reset
 */
typedef int32_t (*codec_reset_function_t)(int32_t *memory[]);
/*!
 * @brief Declares codec_file_seek_function_t as a pointer to a function that
 *                                                             calls codec seek
 */
typedef int32_t (*codec_file_seek_function_t)(int32_t *memory[], int32_t seek_offset);
/*!
 * @brief Declares codec_get_input_frame_size_function_t as a ptr to a
 *                                   function that calls codec get input frame size
 */
typedef int32_t (*codec_get_io_frame_size_function_t)(int32_t *in_size, int32_t *out_size);

/*!
 * @brief Codec Interface Function Table
 */
typedef struct _codec_interface_function_table
{
    codec_get_memory_sizes_function_t GetMemorySizes;  /*!< @brief get size of
                                                 required memory for codec*/
    codec_init_function_t Init;                        /*!< @brief Initialization of Codec*/
    codec_decode_function_t Decode;                    /*!< @brief Decoding of codec*/
    codec_get_pcm_samples_function_t GetPcmSamples;    /*!< @brief get pcm samples
                                                         for codec*/
    codec_reset_function_t Reset;                      /*!< @brief reset codec*/
    codec_file_seek_function_t FileSeek;               /*!< @brief seek at specific
                                                            offset*/
    codec_get_io_frame_size_function_t GetIOFrameSize; /*!< @brief Get io frame size*/
} codec_interface_function_table_t;

/*
 *  External Definitions
 */
extern const codec_interface_function_table_t g_codec_function_table[STREAM_TYPE_COUNT];

/* MP3 */
#ifdef MP3_DEC
#include "mp3_cci.h"
#endif

/* WAV */
#ifdef WAV_DEC
#include "wav_cci.h"
#endif

/* FLAC */
#ifdef FLAC_DEC
#include "flac_cci.h"
#endif

/* AAC */
#if defined(AAC_DEC) && !defined(__ICCARM__)
#include "aac_cci.h"
#endif

#ifdef OPUS_DEC
#include "opus_cci_raw.h"
#endif

#ifdef OGG_OPUS_DEC
#include "opus_cci.h"
#endif

#if defined(__cplusplus)
}
#endif

#endif /* __CODEC_INTERFACE_H__ */
