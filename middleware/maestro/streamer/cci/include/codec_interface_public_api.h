/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*! @file codec_interface_public_api.h
 *  @brief Contains common codec interface APIs definitions.
 */
#ifndef __CODEC_INTERFACE_PUBLIC_API_H_
#define __CODEC_INTERFACE_PUBLIC_API_H_

/*
 *  Includes
 */
#include "error.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#if defined(__cplusplus)
extern "C" {
#endif
/*
 * Enums
 */
/*!
 * @brief Enum Stream type structure
 * This enum is used as an input to all CCI functions
 * and is used as the index into the function table.
 * @warning do not change the order of this enum or wrong functions will be called
 */
typedef enum _stream_type
{
    STREAM_TYPE_UNKNOWN = 0, /*!< @brief 0 = stream type is unknown*/
    STREAM_TYPE_MP3,         /*!< @brief 1 = stream type is MP3 */
    STREAM_TYPE_PCM,         /*!< @brief 2 = stream type is PCM */
    STREAM_TYPE_MSADPCM,     /*!< @brief 3 = stream type is MSADPCM (WAV)*/
    STREAM_TYPE_IMAADPCM,    /*!< @brief 4 = stream type is IMAADPCM (WAV) */
    STREAM_TYPE_OPUS,        /*!< @brief 5 = stream type is OPUS */
    STREAM_TYPE_OGG_OPUS,    /*!< @brief 6 = stream type is OGG OPUS */
    STREAM_TYPE_AAC,         /*!< @brief 7 = stream type is AAC */
    STREAM_TYPE_FLAC,        /*!< @brief 8 = stream type is FLAC */
    STREAM_TYPE_COUNT,       /*!< @brief count of stream type */
} audio_stream_type_t;

/*!
 * @brief Error codes used by CCI
 */
typedef enum _codec_error_code
{
    CODEC_UNSUPPORTED_FUNCTION = -1, /*!< @brief -1 = codec unsupported function */
    CODEC_SUCCESS              = 0,  /*!< @brief 0 = codec function execute succesfully */
    CODEC_INIT_ERROR,                /*!< @brief 1 = Init fails */
    CODEC_DECODE_ERROR,              /*!< @brief 2 = Decode fails */
    CODEC_END_OF_DECODE,             /*!< @brief 3 = End of decode */
    CODEC_RESET_ERROR,               /*!< @brief 4 = Reset fails */
    CODEC_SEEK_ERROR,                /*!< @brief 5 = Seek fails */
    CODEC_MORE_DATA_REQUIRED,        /*!< @brief 6 = More data requires */
    CODEC_METADATA_FILETYPE_FAILED,  /*!< @brief 7 = codec metadata extraction fails */
    CODEC_METADATA_NOT_FOUND,        /*!< @brief 8 = codec metadata not found */
    CODEC_METADATA_UNSUPPORTED_FILE, /*!< @brief 9 = unsupported metadata */
    CODEC_FILE_OPEN_ERROR,           /*!< @brief 10 = file open fails */
    CODEC_FSEEK_FAILED,              /*!< @brief 11 = file seek fails */
    CODEC_MEDIA_READ_ERROR,          /*!< @brief 12 = codec media read fails */
    CODEC_BROKEN_FRAME,              /*!< @brief 13 = codec broken frame */
    CODEC_MEMORY_EXCEED,             /*!< @brief 14 = codec memory exceed */
    CODEC_UNSUPPORTED_FILE,          /*!< @brief 15 = codec unsupported file */
} codec_error_code_t;

/*!
 * @brief codec media sub type structure
 */
typedef enum _codec_media_sub_type
{
    MEDIA_SUBTYPE_UNKNOWN = 0, /*!< @brief 0 = unknown media sub type */
    MEDIA_SUBTYPE_NONE,        /*!< @brief 1 = there is no sub type */
    MEDIA_SUBTYPE_ADTS,        /*!< @brief 2 = ADTS sub type of aac */
    MEDIA_SUBTYPE_M4A,         /*!< @brief 3 = M4A media sub type */
    MEDIA_SUBTYPE_ADIF,        /*!< @brief 4 = ADIF media sub type */
    MEDIA_SUBTYPE_COUNT,       /*!< @brief count of sub types */
} codec_media_sub_type_t;

/*
 *  Prototypes
 */
/*! @brief codec get input/output frame size in bytes.
 *
 *
 * @param[in] stream_type    Audio Stream Type.
 * @param[out] in_size       Input Frame Size.
 * @param[out] out_size      Output Frame Size.
 *
 * @retval 0 when actual input/output frame size in bytes of the codec is get through in_size, out_size pointers
 * @retval CODEC_UNSUPPORTED_FUNCTION  If this function is not supported for
 *                                    streamtype.
 */
int32_t codec_get_io_frame_size(audio_stream_type_t stream_type, int32_t *in_size, int32_t *out_size);
/*! @brief codec get memory information function.
 *
 *
 * @param[in] stream_type       Audio Stream Type.
 *
 * @retval int Size of the codec.
 * @retval CODEC_UNSUPPORTED_FUNCTION  If this function is not supported for
 *                                    streamtype.
 */
int32_t codec_get_mem_info(audio_stream_type_t stream_type);
/*! @brief codec initialization function.
 *
 *
 * @param[in] stream_type    Audio Stream Type.
 * @param[in] memory         This is pointer to the decoder memory.
 * @param[in] callback_fn    This is pointer to the decoder callback functions.
 * @param[in] user_data      This is pointer to the user specific data.
 *
 * @retval CODEC_SUCCESS     if codec initializes successfully.
 * @retval CODEC_INIT_ERROR  if codec fails to initialize.
 * @retval CODEC_UNSUPPORTED_FUNCTION  If this function is not supported for
 *                                     streamtype
 */
int32_t codec_init(audio_stream_type_t stream_type, int32_t *memory[], int32_t *callback_fn[], int32_t *user_data);

/*! @brief codec decoding function.
 *
 *
 * @param[in] stream_type           Audio Stream Type.
 * @param[in] memory              This is pointer to the decoder memory.
 * @param[in] sample_produced      This is pointer to the samples produced by
 *                                 the codec.
 * @param[in] buf_out             This is pointer to the decoder output pcm
 *                                 buffer.
 *
 * @retval CODEC_SUCCESS           if codec decodes frame successfully.
 * @retval CODEC_DECODE_ERROR       if codec fails to decode frame.
 * @retval CODEC_END_OF_DECODE        if decoding of input file is done.
 * @retval CODEC_UNSUPPORTED_FUNCTION  If this function is not supported for
 *                                    streamtype
 */
int32_t codec_decode(audio_stream_type_t stream_type, int32_t *memory[], int32_t *sample_produced, int32_t *buf_out);

/*! @brief codec get pcm samples function.
 *
 *
 * @param[in] stream_type          Audio Stream Type.
 * @param[in] memory             This is pointer to the decoder memory.
 * @param[in] sample_produced     This is pointer to the samples produced by
 *                                the codec.
 * @param[in] buf_out             This is pointer to the user specific data.
 *
 * @retval int32_t It returns number of samples got from the codec.
 */
int32_t codec_get_pcm_samples(audio_stream_type_t stream_type,
                              int32_t *memory[],
                              int32_t *sample_produced,
                              int32_t *buf_out);

/*! @brief codec reset function.
 *
 *
 * @param[in] stream_type          Audio Stream Type.
 * @param[in] memory             This is pointer to the decoder memory.
 *
 * @retval CODEC_SUCCESS          if codec resets successfully.
 * @retval CODEC_RESET_ERROR       if codec fails to reset.
 * @retval CODEC_UNSUPPORTED_FUNCTION  If this function is not supported for
 *                                    streamtype
 */
int32_t codec_reset(audio_stream_type_t stream_type, int32_t *memory[]);
/*! @brief codec seek function.
 *
 * @param[in] stream_type          Audio Stream Type.
 * @param[in] memory             Pointer to decoder memory allocated based on
 *                                codec_get_mem_info() and passed in by caller.
 * @param[in] byte_to_seek          Number of bytes to seek.
 *
 * @retval int32_t frame boundary align offset.
 * @retval CODEC_UNSUPPORTED_FUNCTION  If this function is not supported for
 *                                    streamtype
 */
int32_t codec_seek(audio_stream_type_t stream_type, int32_t *memory[], int32_t byte_to_seek);

#if defined(__cplusplus)
}
#endif

#endif /* __CODEC_INTERFACE_PUBLIC_API_H_ */
