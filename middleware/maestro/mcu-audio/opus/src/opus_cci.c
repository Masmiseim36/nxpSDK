////////////////////////////////////////////////////////////////////////////////
//! \addtogroup opusdec OPUS Decoder
//! @{
//
// Copyright (c) 2020,2022 NXP
//
//! \file opus_cci.c
//! \brief Contains OPUS Decoder CCI functions.
////////////////////////////////////////////////////////////////////////////////

#ifdef OGG_OPUS_DEC

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////
#include "opus_cci.h"
#include "streamer_element.h"
#include <string.h>

/* Callback wrappers */
static int op_read_callback(void *_stream, unsigned char *_ptr, int _buf_size)
{
    OpusDec_CCI_t *opus_cci = (OpusDec_CCI_t *)_stream;
    /* Get data from input file */
    int nb_read = opus_cci->callbacks.pfnRead(0, _ptr, _buf_size, 0, opus_cci->cci_dec);
    return nb_read;
}

static int op_seek_callback(void *_stream, opus_int64 _offset, int _whence)
{
    OpusDec_CCI_t *opus_cci = (OpusDec_CCI_t *)_stream;
    /* Seek in input file */
    int ret = opus_cci->callbacks.pfnSeek(0, _offset, _whence, opus_cci->cci_dec);
    return ret;
}

static opus_int64 op_tell_callback(void *_stream)
{
    OpusDec_CCI_t *opus_cci = (OpusDec_CCI_t *)_stream;
    /* Tell input file */
    opus_int64 ret = opus_cci->callbacks.pfnFtell(0, opus_cci->cci_dec);
    return ret;
}

static const OpusFileCallbacks op_callbacks = {op_read_callback, op_seek_callback, op_tell_callback, NULL};

int32_t OggOPUSGetIOFrameSize(int32_t *in_size, int32_t *out_size)
{
    *in_size  = INPUT_BUFFER_SIZE;
    *out_size = OUTPUT_BUFFER_SIZE;

    return 0;
}

int32_t OggOPUSGetMemorySize(void)
{
    return (sizeof(OpusDec_CCI_t));
}

int32_t OggOPUSInit(int32_t *memory[], int32_t *pCallbackFn[], int32_t *pUserData)
{
    OpusDec_CCI_t *opus_cci                 = (OpusDec_CCI_t *)memory[0];
    decoder_callback_FunctionTable_t *pCall = (decoder_callback_FunctionTable_t *)pCallbackFn;
    CCIDecInfo *cci_dec                     = (CCIDecInfo *)*pUserData;
    int32_t bitrate                         = 0;
    int32_t ret                             = 0;

    opus_cci->callbacks = *pCall;
    opus_cci->opus_file = NULL;
    opus_cci->cci_dec   = cci_dec;

    pCall->pfnSeek(0, 0, SEEK_SET, (CCIDecInfo *)cci_dec);

    /* Find the bitrate and calculate the total time of the opus file */
    opus_cci->opus_file = op_open_callbacks((void *)opus_cci, &op_callbacks, (unsigned char *)NULL, 0, (int *)&ret);
    if ((opus_cci->opus_file == NULL) || (ret != 0))
    {
        goto cleanup;
    }

    bitrate = op_bitrate(opus_cci->opus_file, -1);
    if (bitrate <= 0)
    {
        goto cleanup;
    }

    cci_dec->metadata->bit_rate     = (uint32_t)bitrate;
    cci_dec->metadata->avg_bit_rate = (uint32_t)bitrate;
    cci_dec->metadata->total_time   = ((uint32_t)cci_dec->element->file_size * 8U) / ((uint32_t)bitrate / 1000U);

    cci_dec->element->average_bit_rate = (uint32_t)bitrate;
    cci_dec->element->duration         = cci_dec->metadata->total_time;

    ret = send_msg_element((StreamElement *)(cci_dec->element), MSG_INFO_AUDIO_BITRATE, (uint32_t)bitrate);
    if (ret != 0)
    {
        return ret;
    }
    ret = send_msg_element((StreamElement *)(cci_dec->element), MSG_INFO_DURATION, cci_dec->element->duration);
    if (ret != 0)
    {
        return ret;
    }

    cci_dec->init_done = true;

    return (int32_t)CODEC_SUCCESS;

cleanup:
    if (opus_cci->opus_file != NULL)
    {
        op_free(opus_cci->opus_file);
        opus_cci->opus_file = NULL;
    }
    return (int32_t)CODEC_INIT_ERROR;
}

int32_t OggOPUSDecode(int32_t *memory[], int32_t *sampleProudced, int32_t *bufOut)
{
    OpusDec_CCI_t *opus_cci = (OpusDec_CCI_t *)*memory;
    OggOpusFile *of         = opus_cci->opus_file;
    int out_len             = 0;
    int32_t ret             = (int32_t)CODEC_SUCCESS;

    /* Read and decode Opus packet */
    out_len = op_read(of, opus_cci->output_buffer, OUTPUT_BUFFER_SIZE, NULL);
    if (out_len < 0)
    {
        ret = (int32_t)CODEC_DECODE_ERROR;
        goto cleanup;
    }

    *sampleProudced +=
        out_len * opus_cci->cci_dec->metadata->bits_per_sample / 8 * opus_cci->cci_dec->metadata->num_channels;
    *bufOut = (int32_t)opus_cci->output_buffer;

    if ((of->op[0].e_o_s != 0) || (out_len == 0))
    {
        ret = (int32_t)CODEC_END_OF_DECODE;
        goto cleanup;
    }

    return ret;

cleanup:
    if (opus_cci->opus_file != NULL)
    {
        op_free(opus_cci->opus_file);
        opus_cci->opus_file = NULL;
    }

    return ret;
}

int32_t OggOPUSReset(int32_t *pMemory[])
{
    OpusDec_CCI_t *opus_cci = (OpusDec_CCI_t *)*pMemory;

    if (opus_cci->opus_file != NULL)
    {
        op_free(opus_cci->opus_file);
        opus_cci->opus_file = NULL;
    }

    return (int32_t)CODEC_SUCCESS;
}

int32_t OggOPUSSeek(int32_t *pMemory[], int32_t seekOffset)
{
    OpusDec_CCI_t *opus_cci = (OpusDec_CCI_t *)pMemory[0];
    OggOpusFile *of         = opus_cci->opus_file;
    int32_t ret             = 0;
    /* A negative number means the number of bytes (raw seek) and a positive number the number of pcm samples (pcm seek)
     */
    if (seekOffset < 0)
    {
        ret = (int32_t)op_raw_seek(of, -(ogg_int64_t)seekOffset);
    }
    else
    {
        ret = (int32_t)op_pcm_seek(of, (ogg_int64_t)seekOffset * 48);
    }

    ret = (int32_t)op_raw_tell(of);
    return ret;
}

#endif /* OGG_OPUS_DEC */
