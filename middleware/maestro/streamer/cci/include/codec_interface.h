/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
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
extern int32_t MP3DecoderGetMemorySize();
extern int32_t MP3DecoderInit(int32_t *memory[], int32_t *callback_fn[], int32_t *user_data);
extern int32_t MP3DecoderDecode(int32_t *memory[], int32_t *sample_produced, int32_t *buf_out);
extern int32_t MP3DecoderSeek(int32_t *memory[], int32_t seek_offset);
extern int32_t MP3DecoderGetIOFrameSize(int32_t *in_size, int32_t *out_size);
#endif

/* WAV */
#ifdef WAV_DEC
extern int32_t WAVDecoderGetMemorySize();
extern int32_t WAVDecoderInit(int32_t *memory[], int32_t *callback_fn[], int32_t *user_data);
extern int32_t WAVDecoderDecode(int32_t *memory[], int32_t *sample_produced, int32_t *buf_out);
extern int32_t WAVDecoderReset(int32_t *memory[]);
extern int32_t WAVDecoderSeek(int32_t *memory[], int32_t seek_offset);
extern int32_t WAVDecoderGetIOFrameSize(int32_t *in_size, int32_t *out_size);
#endif

#ifdef OPUS_DEC
/* OPUS */
extern int32_t OPUSDecoderGetMemorySize();
extern int32_t OPUSDecoderInit(int32_t *memory[], int32_t *callback_fn[], int32_t *user_data);
extern int32_t OPUSDecoderDecode(int32_t *memory[], int32_t *sample_produced, int32_t *buf_out);
extern int32_t OPUSDecoderSeek(int32_t *memory[], int32_t seek_offset);
extern int32_t OPUSDecoderGetIOFrameSize(int32_t *in_size, int32_t *out_size);
#endif

#ifdef OGG_OPUS_DEC
/* OGG OPUS */
extern int32_t OggOPUSGetMemorySize();
extern int32_t OggOPUSInit(int32_t *memory[], int32_t *pCallbackFn[], int32_t *pUserData);
extern int32_t OggOPUSDecode(int32_t *memory[], int32_t *sampleProudced, int32_t *bufOut);
extern int32_t OggOPUSReset(int32_t *memory[]);
extern int32_t OggOPUSSeek(int32_t *memory[], int32_t seek_offset);
extern int32_t OggOPUSGetIOFrameSize(int32_t *in_size, int32_t *out_size);
#endif

/* AAC */
#if defined(AAC_DEC) && !defined(__ICCARM__)
extern int32_t AACDecoderGetMemorySize();
extern int32_t AACDecoderInit(int32_t *memory[], int32_t *callback_fn[], int32_t *user_data);
extern int32_t AACDecoderDecode(int32_t *memory[], int32_t *sample_produced, int32_t *buf_out);
extern int32_t AACDecoderSeek(int32_t *memory[], int32_t seek_offset);
extern int32_t AACDecoderGetIOFrameSize(int32_t *in_size, int32_t *out_size);

extern int32_t AACDecodergetMetaData(int32_t *memory[], int32_t *callback_fn[], int32_t *user_data);
#endif

/* FLAC */
#ifdef FLAC_DEC
extern int32_t FLACDecoderGetMemorySize();
extern int32_t FLACDecoderInit(int32_t *memory[], int32_t *callback_fn[], int32_t *user_data);
extern int32_t FLACDecoderDecode(int32_t *memory[], int32_t *sample_produced, int32_t *buf_out);
extern int32_t FLACDecoderSeek(int32_t *memory[], int32_t seek_offset);
extern int32_t FLACDecoderGetIOFrameSize(int32_t *in_size, int32_t *out_size);
extern int32_t FLACDecoderReset(int32_t *memory[]);

extern int32_t FLACDecodergetMetaData(int32_t *memory[], int32_t *callback_fn[], int32_t *user_data);
#endif

#if defined(__cplusplus)
}
#endif

#endif /* __CODEC_INTERFACE_H__ */
