////////////////////////////////////////////////////////////////////////////////
//! \addtogroup opusdec OPUS Decoder
//! @{
//
// Copyright (c) 2020 NXP
//
//! \file opus_cci.c
//! \brief Contains OPUS Decoder CCI functions.
////////////////////////////////////////////////////////////////////////////////

#ifdef OGG_OPUS_DEC

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////
#include "opus_cci.h"
#include <string.h>

/////////////////////////////////////////////////////////////////////////////////
//! \brief This function populates the minimum input/output frame size for the decoder.
//!
//!
//! \param[out] inSize  Input Frame Size
//! \param[out] outSize Output Frame Size
//!
//! \retval it returns the minimum input/output frame size of the decoder in bytes.
/////////////////////////////////////////////////////////////////////////////////
int OggOPUSGetIOFrameSize(int *inSize, int *outSize)
{
    *inSize  = INPUT_BUFFER_SIZE;
    *outSize = OUTPUT_BUFFER_SIZE;

    return 0;
}
/////////////////////////////////////////////////////////////////////////////////
//! \brief This function populates the required memory for the decoder and returns the required memory size in bytes.
//!
//!
//! \param[in] None
//!
//! \returns it returns the size of the decoder in bytes.
/////////////////////////////////////////////////////////////////////////////////
int OggOPUSGetMemorySize(void)
{
    return (sizeof(OpusDec_CCI_t));
}

/////////////////////////////////////////////////////////////////////////////////
//! \brief Initializes the memory and required data structures for the decoder and assigns the required memory to the
//! decoder internal structures.
//!
//!
//! \param[in] memory         This is pointer to the decoder memory.
//! \param[in] callbackFn     This is pointer to the decoder callback functions.
//! \param[in] userData        This is pointer to the user specific data.
//!
//! \retval kCodecSuccess    if codec initializes successfully.
//! \retval kCodecInitError     if codec fails to initialize.
/////////////////////////////////////////////////////////////////////////////////

int OggOPUSInit(int *pMemory[], int *pCallbackFn[], int *pUserData)
{
    OpusDec_CCI_t *opus_cci = (OpusDec_CCI_t *)*pMemory;
    OggOpusFile *of         = &opus_cci->opus_file;
    CCIDecInfo *cci_dec     = (CCIDecInfo *)*pUserData;
    OpusHead *header        = &opus_cci->header;
    OpusTags *tags          = NULL;
    OpusDecoder *decoder;
    int nb_read, serialno, ret;
    int header_parsed = 0, tags_parsed = 0;

    decoder_callback_FunctionTable_t *pCall = (decoder_callback_FunctionTable_t *)pCallbackFn;
    opus_cci->callbacks                     = *pCall;

    memset(of, 0, sizeof(OggOpusFile));

    if (ogg_sync_init(&of->oy) != 0)
    {
        return CODEC_INIT_ERROR;
    }

    while (!header_parsed || !tags_parsed)
    {
        while (ogg_sync_pageout(&of->oy, &opus_cci->og) != 1)
        {
            opus_cci->input_buffer = ogg_sync_buffer(&of->oy, HEADER_SIZE);
            if (opus_cci->input_buffer == NULL)
            {
                ogg_sync_clear(&of->oy);
                return CODEC_INIT_ERROR;
            }
            nb_read = pCall->pfnRead(0, (unsigned char *)opus_cci->input_buffer, HEADER_SIZE, 0, cci_dec);
            if (ogg_sync_wrote(&of->oy, nb_read) != 0)
            {
                ogg_sync_clear(&of->oy);
                return CODEC_INIT_ERROR;
            }
        }

        serialno = ogg_page_serialno(&opus_cci->og);

        if (ogg_page_bos(&opus_cci->og))
        {
            ogg_stream_init(&of->os, serialno);
            /*Add page to the bitstream*/
            if (!ogg_stream_pagein(&of->os, &opus_cci->og))
            {
                if (ogg_stream_packetout(&of->os, &of->op[0]) > 0)
                {
                    if (opus_head_parse(header, of->op[0].packet, of->op[0].bytes))
                    {
                        goto cleanup;
                    }
                    else
                    {
                        header_parsed = 1;
                        /* Seek the next page with tags */
                        ogg_sync_pageseek(&of->oy, &opus_cci->og);
                    }
                }
            }
        }
        if (header_parsed)
        {
            if (!ogg_stream_pagein(&of->os, &opus_cci->og))
            {
                if (ogg_stream_packetout(&of->os, &of->op[0]) > 0)
                {
                    if (opus_tags_parse(tags, of->op[0].packet, of->op[0].bytes) < 0)
                    {
                        goto cleanup;
                    }
                    else
                    {
                        ret = ogg_stream_packetout(&of->os, &of->op[0]) > 0;
                        if (ret != 0 || opus_cci->og.header[opus_cci->og.header_len - 1] == 255)
                        {
                            goto cleanup;
                        }
                        else
                        {
                            decoder = opus_decoder_create((opus_int32)cci_dec->metadata->sample_rate, header->channel_count, NULL);
                            if (decoder == NULL)
                            {
                                goto cleanup;
                            }
                            else
                            {
                                opus_cci->decoder = decoder;
                            }
                            cci_dec->init_done = true;
                            opus_cci->cci_dec  = *cci_dec;
                            return CODEC_SUCCESS;
                        }
                    }
                }
            }
        }
    }
cleanup:
    if (&of->oy)
    {
        (void)ogg_sync_clear(&of->oy);
    }
    if (&of->os)
    {
        (void)ogg_stream_clear(&of->os);
    }
    if (opus_cci->decoder != NULL)
    {
        opus_decoder_destroy(opus_cci->decoder);
        opus_cci->decoder = NULL;
    }
    return CODEC_INIT_ERROR;
}
/////////////////////////////////////////////////////////////////////////////////
//! \brief Decodes the input bit stream and generates the PCM data in output pcm buffer.
//!
//!
//! \param[in] memory              This is pointer to the decoder memory.
//! \param[in] sampleProduced      This is pointer to the samples produced by the codec.
//! \param[in] bufOuts             This is pointer to the output pcm buffer.
//!
//! \retval kCodecSuccess           if codec decodes frame successfully.
//! \retval kCodecDecodeError       if codec fails to decode frame.
//! \retval kCodeEndOfDecode        if decoding of input file is done.
/////////////////////////////////////////////////////////////////////////////////
int OggOPUSDecode(int *pMemory[], int *pSampleToProduced, int *pBufOut)
{
    OpusDec_CCI_t *opus_cci = (OpusDec_CCI_t *)*pMemory;
    OggOpusFile *of         = &opus_cci->opus_file;
    int nb_read, out_len, ret;

    /* If a packet hasn't been decoded yet, we have to read a page first */
    if (opus_cci->packet_decode == 0)
    {
        /* Read a new page */
        while (ogg_sync_pageout(&of->oy, &opus_cci->og) != 1)
        {
            /* Get the ogg buffer for writing*/
            opus_cci->input_buffer = ogg_sync_buffer(&of->oy, INPUT_BUFFER_SIZE);
            /* If we get the input buffer allocated, read bitstream from input file*/
            if (opus_cci->input_buffer != NULL)
            {
                nb_read = opus_cci->callbacks.pfnRead(0, (unsigned char *)opus_cci->input_buffer, INPUT_BUFFER_SIZE, 0,
                                                      &opus_cci->cci_dec);
            }
            /* Otherwise report an error - not enough memory */
            else
            {
                ret = CODEC_DECODE_ERROR;
                goto cleanup;
            }
            if (nb_read <= 0)
            {
                /* We didn't read anything - possible EOF*/
                break;
            }
            /* If we have some problem with storage, cleanup */
            if (ogg_sync_wrote(&of->oy, nb_read) != 0)
            {
                ret = CODEC_DECODE_ERROR;
                goto cleanup;
            }
        }
        if (ogg_page_serialno(&opus_cci->og) != of->os.serialno)
        {
            /* so all streams are read. */
            ogg_stream_reset_serialno(&of->os, ogg_page_serialno(&opus_cci->og));
        }
        /*Add page to the bitstream*/
        if (ogg_stream_pagein(&of->os, &opus_cci->og) != 0)
        {
            ret = CODEC_DECODE_ERROR;
            goto cleanup;
        }
        opus_cci->packet_decode = 1;
    }
    /*Extract available packet*/
    if (ogg_stream_packetout(&of->os, &of->op[0]))
    {
        /*Decode Opus packet*/
        out_len = opus_decode(opus_cci->decoder, of->op[0].packet, of->op[0].bytes, opus_cci->output_buffer,
                              OUTPUT_BUFFER_SIZE, 0);
        /*If the decoder returned less than zero, we have an error.*/
        if (out_len < 0)
        {
            ret = CODEC_DECODE_ERROR;
            goto cleanup;
        }
        *pSampleToProduced += out_len * BYTE_WIDTH * opus_cci->header.channel_count;
        *pBufOut = (int)&opus_cci->output_buffer;
        if (of->op[0].e_o_s)
        {
            ret = CODEC_END_OF_DECODE;
            goto cleanup;
        }
        return CODEC_SUCCESS;
    }
    opus_cci->packet_decode = 0;
    return CODEC_SUCCESS;
cleanup:
    if (opus_cci->decoder != NULL)
    {
        (void)ogg_sync_clear(&of->oy);
        (void)ogg_stream_clear(&of->os);
        opus_decoder_destroy(opus_cci->decoder);
        opus_cci->decoder = NULL;
    }
    return ret;
}

int OggOPUSReset(int *pMemory[])
{
    OpusDec_CCI_t *opus_cci = (OpusDec_CCI_t *)*pMemory;
    OggOpusFile *of         = &opus_cci->opus_file;
    if (opus_cci->decoder != NULL)
    {
        (void)ogg_sync_clear(&of->oy);
        (void)ogg_stream_clear(&of->os);
        opus_decoder_destroy(opus_cci->decoder);
        opus_cci->decoder = NULL;
    }
    return (int)CODEC_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////
//! \brief Decoder seek function to seek at specific offset.
//!
//!
//! \param[in] memory              This is pointer to the decoder memory.
//! \param[in] seekOffset           It is unalign seek offset.
//!
//! \retval It returns frame boundary align offset.
/////////////////////////////////////////////////////////////////////////////////
int OggOPUSSeek(int *pMemory[], int seekOffset)
{
    int actualOffset = seekOffset;

    return actualOffset;
}

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
//! @}

#endif /* OGG_OPUS_DEC */
