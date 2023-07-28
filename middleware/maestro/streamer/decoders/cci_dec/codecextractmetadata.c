/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
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
