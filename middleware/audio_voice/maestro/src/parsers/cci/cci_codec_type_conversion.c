/*
 * Copyright 2018-2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*!
 * @file    cci_codec_type_conversion.c
 * @brief   This file Contains CCI decoder type conversion APIs
 *
 */

/*
 *  Includes
 */
#include <string.h>
#include "audio_cfg.h"
#include "streamer.h"
#include "streamer_api.h"
#include "codec_interface_public_api.h"

/*
 * Prototypes
 */

/*! @brief Convert CCI stream type to a decoder type.
 *
 *
 * @param[in] stream_type             CCI stream type.
 *
 * @retval Decoder Type
 */
int32_t stream_type_conversion(int32_t stream_type)
{
    int32_t decoder_type;

    switch (stream_type)
    {
        case STREAM_TYPE_PCM:
        case STREAM_TYPE_MSADPCM:
        case STREAM_TYPE_IMAADPCM:
            decoder_type = DECODER_TYPE_WAV;
            break;
        case STREAM_TYPE_MP3:
            decoder_type = DECODER_TYPE_MP3;
            break;
        case STREAM_TYPE_AAC:
            decoder_type = DECODER_TYPE_AAC;
            break;
        case STREAM_TYPE_FLAC:
            decoder_type = DECODER_TYPE_FLAC;
            break;
        default:
            decoder_type = DECODER_TYPE_UNKNOWN;
            break;
    }

    return decoder_type;
}
/*! @brief Convert Decoder type to CCI stream type.
 *
 *
 * @param[in] decoder_type Decoder type.
 *
 * @retval CCI Stream Type
 */
uint8_t cci_decoder_type_conversion(DecoderType decoder_type)
{
    audio_stream_type_t cci_type;

    STREAMER_FUNC_ENTER(DBG_CCID);
    switch (decoder_type)
    {
        case DECODER_TYPE_WAV:
            cci_type = STREAM_TYPE_PCM;
            break;
        case DECODER_TYPE_MP3:
            cci_type = STREAM_TYPE_MP3;
            break;
        case DECODER_TYPE_OPUS:
            cci_type = STREAM_TYPE_OPUS;
            break;
        case DECODER_TYPE_OGG_OPUS:
            cci_type = STREAM_TYPE_OGG_OPUS;
            break;
        case DECODER_TYPE_AAC:
            cci_type = STREAM_TYPE_AAC;
            break;
        case DECODER_TYPE_FLAC:
            cci_type = STREAM_TYPE_FLAC;
            break;
        default:
            cci_type = STREAM_TYPE_UNKNOWN;
            break;
    }
    STREAMER_FUNC_EXIT(DBG_CCID);
    return cci_type;
}

/*
 * End of file
 * @}
 */
