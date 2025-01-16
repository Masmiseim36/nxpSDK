/*
 * Copyright 2018-2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*!
 * @file    codecextractmetadata.c
 * @brief   This file Contains functionality that determines the file type and
 *        then calls the proper extract meta_data function
 *
 */

/*
 *  Includes
 */
#include "codec_interface_public_api.h"
#include "audio_cfg.h"
#include "cci_metadata.h"

/*
 *  Functions
 */

int32_t ccidec_extract_meta_data(audio_stream_type_t stream_type, file_meta_data_t *meta_data, CCI_Ctx *ctx)
{
    uint8_t ret = CODEC_METADATA_FILETYPE_FAILED;

    switch (stream_type)
    {
#ifdef WAV_DEC
        case STREAM_TYPE_PCM:
        case STREAM_TYPE_MSADPCM:
        case STREAM_TYPE_IMAADPCM:
            ret = (uint8_t)codec_extract_metadata_wave(meta_data, ctx);
            break;
#endif
#ifdef MP3_DEC
        case STREAM_TYPE_MP3:
            ret = (uint8_t)codec_extract_metadata_mp3(meta_data, ctx);
            break;
#endif
#ifdef OPUS_DEC
        case STREAM_TYPE_OPUS:
            ret = codec_extract_metadata_opus(meta_data, ctx);
            break;
#endif /* OPUS_DEC */

#ifdef OGG_OPUS_DEC
        case STREAM_TYPE_OGG_OPUS:
            ret = codec_extract_metadata_ogg_opus(meta_data, ctx);
            break;
#endif /* OGG_OPUS_DEC */
#if defined(AAC_DEC) && !defined(__ICCARM__)
        case STREAM_TYPE_AAC:
            ret = (uint8_t)codec_extract_metadata_aac(meta_data, ctx);
            break;
#endif
#ifdef FLAC_DEC
        case STREAM_TYPE_FLAC:
            ret = (uint8_t)codec_extract_metadata_flac(meta_data, ctx);
            break;
#endif
        default:
            /* Unknown Audio Stream Type (kCodecStreamTypeUnknown) */
            break;
    }

    return (ret);
}

/*
 * End of file
 * @}
 */
