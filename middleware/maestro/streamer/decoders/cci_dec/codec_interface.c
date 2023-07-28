/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    codec_interface.c
 * @brief   This file contains the CCI decoder interface APIs definitions.
 *
 */

/*
 *  Includes
 */
#include "audio_cfg.h"
#include "codec_interface.h"
#include <stdio.h>

/*
 * Codec Interface APIs Table
 */

/**
 * @brief Global table with function pointers for different codecs
 *
 */
const codec_interface_function_table_t g_codec_function_table[STREAM_TYPE_COUNT] = {
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    },
#ifdef MP3_DEC
    {
        &MP3DecoderGetMemorySize,
        &MP3DecoderInit,
        &MP3DecoderDecode,
        NULL,
        NULL,
        &MP3DecoderSeek,
        &MP3DecoderGetIOFrameSize,
    },
#else
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    },
#endif

#ifdef WAV_DEC // PCM
    {
        &WAVDecoderGetMemorySize,
        &WAVDecoderInit,
        &WAVDecoderDecode,
        NULL,
        &WAVDecoderReset,
        &WAVDecoderSeek,
        &WAVDecoderGetIOFrameSize,
    },
#else
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    },
#endif

#ifdef WAV_DEC // MSADPCM
    {
        &WAVDecoderGetMemorySize,
        &WAVDecoderInit,
        &WAVDecoderDecode,
        NULL,
        &WAVDecoderReset,
        &WAVDecoderSeek,
        &WAVDecoderGetIOFrameSize,
    },
#else
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    },
#endif

#ifdef WAV_DEC // IMAADPCM
    {
        &WAVDecoderGetMemorySize,
        &WAVDecoderInit,
        &WAVDecoderDecode,
        NULL,
        &WAVDecoderReset,
        &WAVDecoderSeek,
        &WAVDecoderGetIOFrameSize,
    },
#else
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    },
#endif

#ifdef OPUS_DEC
    {
        &OPUSDecoderGetMemorySize,
        &OPUSDecoderInit,
        &OPUSDecoderDecode,
        NULL,
        NULL,
        &OPUSDecoderSeek,
        &OPUSDecoderGetIOFrameSize,
    },
#else
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    },

#endif /* OPUS_DEC */

#ifdef OGG_OPUS_DEC
    {
        &OggOPUSGetMemorySize,
        &OggOPUSInit,
        &OggOPUSDecode,
        NULL,
        &OggOPUSReset,
        &OggOPUSSeek,
        &OggOPUSGetIOFrameSize,
    },
#else
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    },

#endif /* OGG_OPUS_DEC */

#if defined(AAC_DEC) && !defined(__ICCARM__)
    {
        &AACDecoderGetMemorySize,
        &AACDecoderInit,
        &AACDecoderDecode,
        NULL,
        NULL,
        &AACDecoderSeek,
        &AACDecoderGetIOFrameSize,
    },
#else
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    },
#endif

#ifdef FLAC_DEC
    {
        &FLACDecoderGetMemorySize,
        &FLACDecoderInit,
        &FLACDecoderDecode,
        NULL,
        &FLACDecoderReset,
        &FLACDecoderSeek,
        &FLACDecoderGetIOFrameSize,
    },
#else
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    },
#endif
};
/*
 * Code
 */

int32_t codec_get_io_frame_size(audio_stream_type_t stream_type, int32_t *in_size, int32_t *out_size)
{
    int32_t ret         = 0;
    int32_t input_size  = 0;
    int32_t output_size = 0;

    if (NULL != g_codec_function_table[stream_type].GetIOFrameSize)
    {
        ret       = g_codec_function_table[stream_type].GetIOFrameSize(&input_size, &output_size);
        *in_size  = input_size;
        *out_size = output_size;
    }
    else
    {
        *in_size  = 0;
        *out_size = 0;
        ret       = CODEC_UNSUPPORTED_FUNCTION;
    }

    return ret;
}

int32_t codec_get_mem_info(audio_stream_type_t stream_type)
{
    int32_t ret = 0;

    if (NULL != g_codec_function_table[stream_type].GetMemorySizes)
        ret = g_codec_function_table[stream_type].GetMemorySizes();
    else
        ret = CODEC_UNSUPPORTED_FUNCTION;

    return ret;
}

int32_t codec_init(audio_stream_type_t stream_type, int32_t *memory[], int32_t *callback_fn[], int32_t *user_data)

{
    int32_t ret = 0;

    if (NULL != g_codec_function_table[stream_type].Init)
        ret = g_codec_function_table[stream_type].Init(memory, callback_fn, user_data);
    else
        ret = CODEC_UNSUPPORTED_FUNCTION;

    return ret;
}

int32_t codec_decode(audio_stream_type_t stream_type, int32_t *memory[], int32_t *sample_produced, int32_t *buf_out)

{
    int32_t ret              = 0;
    int32_t produced_samples = 0;
    int32_t buf              = 0;

    if (NULL != g_codec_function_table[stream_type].Decode)
    {
        ret              = g_codec_function_table[stream_type].Decode(memory, &produced_samples, &buf);
        *sample_produced = produced_samples;
        *buf_out         = buf;
    }
    else
    {
        ret              = CODEC_UNSUPPORTED_FUNCTION;
        *sample_produced = produced_samples;
        *buf_out         = buf;
    }

    return ret;
}

int32_t codec_get_pcm_samples(audio_stream_type_t stream_type,
                              int32_t *memory[],
                              int32_t *sample_produced,
                              int32_t *buf_out)

{
    int32_t ret              = 0;
    int32_t produced_samples = 0;
    int32_t buf;

    if (NULL != g_codec_function_table[stream_type].GetPcmSamples)
    {
        ret              = g_codec_function_table[stream_type].GetPcmSamples(memory, &produced_samples, 64, &buf);
        *sample_produced = produced_samples;
        *buf_out         = buf;
        return ret;
    }
    else
    {
        return *sample_produced;
    }
}

int32_t codec_reset(audio_stream_type_t stream_type, int32_t *memory[])
{
    int32_t ret = 0;

    if (NULL != g_codec_function_table[stream_type].Reset)
        ret = g_codec_function_table[stream_type].Reset(memory);
    else
        ret = CODEC_UNSUPPORTED_FUNCTION;

    return ret;
}

int32_t codec_seek(audio_stream_type_t stream_type, int32_t *memory[], int32_t byte_to_seek)
{
    int32_t ret = 0;

    if (NULL != g_codec_function_table[stream_type].FileSeek)
    {
        ret = g_codec_function_table[stream_type].FileSeek(memory, byte_to_seek);
    }
    else
    {
        ret = CODEC_UNSUPPORTED_FUNCTION;
    }

    return ret;
}
/*
 * End of file
 * @}
 */
