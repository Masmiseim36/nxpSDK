/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    ccidec.c
 * @brief   This file contains the CCI decoder element pad implementation.
 *
 */

/*
 * INCLUDE FILES
 */
#include <string.h>

#include "audio_cfg.h"
#include "ccidec.h"
#include "decoder.h"
#include "streamer.h"
#include "codec_interface_public_api.h"
#include "general_utils.h"
#include "audio_sink.h"
#include "cci_metadata.h"
#include "pipeline.h"

// #define DUMP_PCM
#ifdef DUMP_PCM
#include "ff.h"
FIL *pcm_file = NULL;
#endif

extern uint8_t cci_decoder_type_conversion(DecoderType audio_type);

/**
 * @brief Get data from a file source
 *
 * @param stream_num     Stream number
 * @param decoder_buffer Buffer to be filled
 * @param size_to_read   Size to be read
 * @param update_tail    Update tail (currently unused)
 * @param context        Pointer to the CCIDec context
 * @return int32_t
 */
int32_t _get_file_data(
    int32_t stream_num, uint8_t *decoder_buffer, int32_t size_to_read, int32_t update_tail, void *context)
{
    int32_t ret_bytes = 0; /* num bytes read and returned*/
    StreamBuffer buffer;
    uint8_t *buffer_ptr;
    CCIDecInfo *cci_dec = (CCIDecInfo *)context;
    StreamPad *sink;
    ElementDecoder *decoder = (ElementDecoder *)cci_dec->element;
    FlowReturn flowret;

    STREAMER_FUNC_ENTER(DBG_CCID);

    STREAMER_LOG_DEBUG(DBG_CCID,
                       "%s: available input size=%d, "
                       "required=%d stream no = %d\n",
                       __FUNCTION__, cci_dec->filesrc_size[stream_num], size_to_read, stream_num);

    sink = ELEMENT_SINK_PAD(decoder, stream_num);

    if (size_to_read > 0)
    {
        if (size_to_read > cci_dec->filesrc_size[stream_num])
        {
            /* not enough data available in filesrc buffer, refill buffer
             * first then return data */
            int32_t RemainingBytes      = size_to_read - cci_dec->filesrc_size[stream_num];
            int32_t DecoderBufferOffset = cci_dec->filesrc_size[stream_num];
            /* Copy the bytes available in src buffer to decoder_buffer */
            memcpy(decoder_buffer, cci_dec->filesrc_buffer[stream_num] + cci_dec->filesrc_buffer_idx[stream_num],
                   cci_dec->filesrc_size[stream_num]);
            /* After copy bytes update filesrc_buffer variables */
            cci_dec->filesrc_offset[stream_num] += cci_dec->filesrc_size[stream_num];
            cci_dec->filesrc_buffer_idx[stream_num] += cci_dec->filesrc_size[stream_num];
            cci_dec->filesrc_size[stream_num] -= cci_dec->filesrc_size[stream_num];
            /* Define new chunk size in bytes to fill src buffer */
            buffer.buffer = (int8_t *)cci_dec->filesrc_buffer[stream_num] - sizeof(RawPacketHeader);
            /* Request filesrc element to read file and refill src buffer
             * based on stream using */
            flowret = pad_pull_range(sink, cci_dec->filesrc_offset[stream_num],
                                     stream_num == 0 ? (uint32_t)cci_dec->dec_frame_size : 2048U, &buffer);
            if (FLOW_OK == flowret)
            {
                if (((PacketHeader *)BUFFER_PTR(&buffer))->id == RAW_DATA)
                {
                    /*A read file from file source element always return a RAW_DATA id,
                     * after verify it we don't need it any more so shift cci_dec->filesrc_buffer.
                     * Update filesrc_buffer variables after a success read file.*/
                    buffer_ptr                          = (uint8_t *)(buffer.buffer + sizeof(RawPacketHeader));
                    cci_dec->filesrc_size[stream_num]   = (buffer.size - sizeof(RawPacketHeader));
                    cci_dec->filesrc_offset[stream_num] = buffer.offset;
                    /*Verify if the size read is bigger than RemainingBytes*/
                    if (RemainingBytes > cci_dec->filesrc_size[stream_num])
                        RemainingBytes = cci_dec->filesrc_size[stream_num];
                    /* Copy the remaining size_to_read bytes to decoder_buffer*/
                    memcpy(decoder_buffer + DecoderBufferOffset, buffer_ptr, RemainingBytes);
                    /* After a copy bytes update filesrc_buffer variables */
                    cci_dec->filesrc_buffer_idx[stream_num] = RemainingBytes;
                    cci_dec->filesrc_size[stream_num] -= RemainingBytes;
                    cci_dec->filesrc_offset[stream_num] += RemainingBytes;
                    ret_bytes = DecoderBufferOffset + RemainingBytes;
                }
                else
                {
                    STREAMER_LOG_CATA(DBG_CCID, ERRCODE_INTERNAL, "[CCID]Not able to read more data\n");
                    ret_bytes = DecoderBufferOffset;
                }
                /* a problem occurred during pull from filesrc */
            }
            else if (FLOW_EOS == flowret)
            {
                /* EOS EVENT only send when Decoder return End Of Decode. */
                /* In case of AAC Decoder, it may possible that we reach at the end of file during parsing.
                 * So if we return from this point than AAC file will not play. */
                STREAMER_LOG_DEBUG(DBG_CCID, "%s: No input data available DecoderBufferOffset %d", __FUNCTION__,
                                   DecoderBufferOffset);
                ret_bytes = DecoderBufferOffset;
            }
            else
            {
                STREAMER_LOG_CATA(DBG_CCID, ERRCODE_INTERNAL, "[CCID]Flow not ok: get_file_data\n");
                ret_bytes = 0;
            }
            /* enough data already in the filesrc buffer, return requested data */
        }
        else
        {
            memcpy(decoder_buffer, cci_dec->filesrc_buffer[stream_num] + cci_dec->filesrc_buffer_idx[stream_num],
                   size_to_read);
            /* After a copy bytes update filesrc_buffer variables */
            cci_dec->filesrc_size[stream_num] -= size_to_read;
            cci_dec->filesrc_offset[stream_num] += size_to_read;
            cci_dec->filesrc_buffer_idx[stream_num] += size_to_read;
            ret_bytes = size_to_read;
        }
    }
    else
    {
        /* caller error, no bytes to read */
        STREAMER_LOG_DEBUG(DBG_CCID, "CCID_ERROR_INVALID_PARAM\n");
    }
    STREAMER_FUNC_EXIT(DBG_CCID);
    return ret_bytes;
}

/**
 * @brief Callback used by CCI to seek within the file
 *
 * @param stream_num Stream number
 * @param offset     Offset
 * @param position   Seek type
 * @param context    CCIDecInfo context
 * @return int32_t
 */
static int32_t _seek_file_data(int32_t stream_num, int32_t offset, int32_t position, void *context)
{
    int32_t ret         = 0;
    CCIDecInfo *cci_dec = (CCIDecInfo *)context;
    int32_t reloffset   = 0;
    int32_t absoffset   = 0;

    STREAMER_FUNC_ENTER(DBG_CCID);
    STREAMER_LOG_DEBUG(DBG_CCID,
                       "%s: offset required=%d, "
                       "position=%d\n",
                       __FUNCTION__, offset, position);

    if (SEEK_SET == position)
    {
        /* Here the offset is absolute */
        absoffset = offset;
        reloffset = offset - cci_dec->filesrc_offset[stream_num];
    }
    else if (SEEK_CUR == position)
    {
        /* Here the offset is relative */
        absoffset = cci_dec->filesrc_offset[stream_num] + offset;
        reloffset = offset;
    }
    else if (SEEK_END == position)
    {
        /* Here offset is relative to the end */
        absoffset = cci_dec->element->file_size + offset;
        reloffset = absoffset - cci_dec->filesrc_offset[stream_num];
    }
    /* Verify If offset fall inside of current filesrc_buffer */
    if ((absoffset >= (int32_t)(cci_dec->filesrc_offset[stream_num] - cci_dec->filesrc_buffer_idx[stream_num])) &&
        (reloffset < cci_dec->filesrc_size[stream_num]))
    {
        /* If offset fall inside of current buffer then Update size and
        idx variables */
        cci_dec->filesrc_size[stream_num] -= reloffset;
        cci_dec->filesrc_buffer_idx[stream_num] += reloffset;
    }
    else
    {
        cci_dec->filesrc_size[stream_num]       = 0;
        cci_dec->filesrc_buffer_idx[stream_num] = 0;
    }
    /* Here the new offset is updated */
    cci_dec->filesrc_offset[stream_num] = absoffset;

    STREAMER_FUNC_EXIT(DBG_CCID);

    return ret;
}

/**
 * @brief Callback to provide current position of the file to the CCI
 *
 * @param stream_num Stream number
 * @param user_data  Pointer to CCIDecInfo structure
 * @return int32_t
 */
int32_t _get_file_position(int32_t stream_num, void *user_data)
{
    CCIDecInfo *cci_dec = (CCIDecInfo *)user_data;

    if (NULL == cci_dec)
        return CODEC_MEDIA_READ_ERROR;
    else
        return cci_dec->filesrc_offset[stream_num];
}

/**
 * @brief Decode a frame using the CCI decoder function
 *
 * @param element Decoder element
 * @return int32_t
 */
int32_t ccidec_decode_frame(ElementDecoder *element)
{
    int32_t decoded_buffer;                           /* buffer returned from CCI decoder
                                                         this is part of ccidec_memory already allocated */
    int32_t bytes_produced;                           /* returned from CCI decoder */
    StreamBuffer out_buffer;                          /* struct passed to audio sink */
    uint8_t *data                    = NULL;          /* used to offset decoded data */
    CCIDecInfo *cci_dec              = NULL;          /* primary structure containing CCI data */
    AudioPacketHeader *output_header = NULL;          /* used to load audio header */
    int32_t ret                      = CODEC_SUCCESS; /* Just use CCI status in this function */
    FlowReturn flow_ret              = FLOW_OK;       /* use for pad push return */

    STREAMER_FUNC_ENTER(DBG_CCID);

    cci_dec = (CCIDecInfo *)element->dec_info;
    if (NULL == cci_dec)
    {
        return (int32_t)CODEC_DECODE_ERROR;
    }

    ret = codec_decode((audio_stream_type_t)cci_dec->stream_type, &cci_dec->ccidec_memory, &bytes_produced,
                       &decoded_buffer);
    if (CODEC_SUCCESS != ret && CODEC_END_OF_DECODE != ret && CODEC_BROKEN_FRAME != ret)
    {
        if (CODEC_MEMORY_EXCEED == ret)
        {
            STREAMER_LOG_CATA(DBG_CCID, ERRCODE_OUT_OF_MEMORY, "[CCID]Codec Memory Exceed\n", ret);
        }
        else if (CODEC_UNSUPPORTED_FILE == ret)
        {
            STREAMER_LOG_CATA(DBG_CCID, ERRCODE_NOT_SUPPORTED, "[CCID]Codec unsupported Track\n", ret);
        }
        else
        {
            STREAMER_LOG_CATA(DBG_CCID, ERRCODE_GENERAL_ERROR, "[CCID]Codec Decode Failed\n", ret);
        }
    }
    else
    {
        if (bytes_produced > 0)
        {
            STREAMER_LOG_DEBUG(DBG_CCID, "[CCID] Decoded\n");

            /* Init StreamBuffer used to send data to AudioSink */
            out_buffer.buffer = NULL;
            out_buffer.size   = 0;
            /* Don't know anything about time */
            out_buffer.time   = (uint32_t)-1;
            out_buffer.offset = 0;

            /* initialize data buffer to copy data after header */
            data = cci_dec->packed_buffer[cci_dec->buffer_index] + sizeof(AudioPacketHeader);

            /*  extract pcm samples accordingly */
            if (element->mute_frames > 0)
            {
                memset((int32_t *)decoded_buffer, 0, bytes_produced);
                element->mute_frames--;
            }
            else
            {
                memcpy(data, (int32_t *)decoded_buffer, bytes_produced);
            }

            /* Use this pointer to load header info */
            output_header = (AudioPacketHeader *)cci_dec->packed_buffer[cci_dec->buffer_index];
            /* clear init header info */
            memset(output_header, 0, sizeof(AudioPacketHeader));
            /* load header info */
            AUDIO_ID(output_header)          = AUDIO_DATA;
            AUDIO_CHUNK_SIZE(output_header)  = bytes_produced;
            AUDIO_SAMPLE_RATE(output_header) = cci_dec->metadata->sample_rate;

            /* 8Bits Per Sample Audio is generating 16 bit output...*/
            if (cci_dec->metadata->bits_per_sample == 8)
                cci_dec->metadata->bits_per_sample = cci_dec->metadata->bits_per_sample * 2;

            AUDIO_BITS_PER_SAMPLE(output_header) = cci_dec->metadata->bits_per_sample;
            AUDIO_NUM_CHANNELS(output_header)    = cci_dec->metadata->num_channels;

            AUDIO_FORMAT(output_header) = AUDIO_SET_FORMAT(DECODER_INTERLEAVED, AF_LITTLE_ENDIAN, DECODER_SIGNED,
                                                           cci_dec->metadata->bits_per_sample);

            /* setup StreamBuffer structure for output */
            BUFFER_PTR(&out_buffer)    = (int8_t *)cci_dec->packed_buffer[cci_dec->buffer_index];
            BUFFER_OFFSET(&out_buffer) = 0;
            BUFFER_SIZE(&out_buffer)   = bytes_produced + sizeof(AudioPacketHeader);
            /* count the total decoded frames */
            element->num_decoded_frames++;
            STREAMER_LOG_DEBUG(DBG_CCID, "[CCID] Decoded %d frames\n", element->num_decoded_frames);
#ifdef DUMP_PCM
            unsigned int written = 0;
            if (pcm_file && (element->num_decoded_frames < 200))
                (void)f_write(pcm_file, cci_dec->packed_buffer[cci_dec->buffer_index] + sizeof(AudioPacketHeader),
                              bytes_produced, &written);
            if (written)
            {
                STREAMER_LOG_DEBUG(DBG_CCID, "[CCID] Written %d bytes\n", written);
            }
#endif

            /* send the output to the audio sink, call src pad push function */
            flow_ret = pad_push((StreamPad *)ELEMENT_SRC_PAD(element, 0), &out_buffer);
            if (FLOW_OK != flow_ret)
            {
                ret = CODEC_DECODE_ERROR;
                goto EXIT;
            }

            cci_dec->buffer_index++;
            if (cci_dec->buffer_index >= NUM_OF_PING_PONG_BUFFER)
            {
                cci_dec->buffer_index = 0;
            }

#ifdef DUMP_PCM
            if (element->num_decoded_frames == 200)
            {
                // fflush(pcm_file);
                f_close(pcm_file);
                pcm_file = NULL;
            }
#endif
            BUFFER_PTR(&out_buffer)    = NULL;
            BUFFER_SIZE(&out_buffer)   = 0;
            BUFFER_OFFSET(&out_buffer) = 0;
        }
    }

EXIT:
    STREAMER_FUNC_EXIT(DBG_CCID);
    return ret;
}

/**
 * @brief Get the cci metadata from the file header
 *
 * @param element Decoder element
 * @return uint8_t
 */
uint8_t get_cci_metadata(ElementDecoder *element)
{
    CCI_Ctx ctx;
    uint8_t ret = true; /* streamer return codes */
    uint32_t smallest;
    char metadata_string[TITLE_SIZE];
    uint8_t cci_ret     = CODEC_SUCCESS; /* return codes used by CCI */
    CCIDecInfo *cci_dec = NULL;
    StreamData data     = {0};

    /* setup StreamElement pointer for msgs */
    StreamElement *decoder = (StreamElement *)element;

    cci_dec = (CCIDecInfo *)element->dec_info;
    if (NULL == cci_dec)
    {
        ret = false;
        goto Error;
    }

    STREAMER_LOG_DEBUG(DBG_CCID, "[CCID] !!! Streamer Interface !!! \n");
    /* CCI Context Structure. Callbacks assignment */
    ctx.user_data    = (void *)cci_dec;
    ctx.cci_dec_read = _get_file_data;
    ctx.cci_dec_seek = _seek_file_data;
    ctx.cci_dec_tell = _get_file_position;

    if (STREAM_OK != query_info_pipeline((PipelineHandle)element->parent, INFO_SIZE, &data))
    {
        STREAMER_LOG_DEBUG(DBG_CCID, "[CCID] Get File Size error ");
    }
    element->file_size = data.value32u;
    /* Extract metadata for the given input file */
    cci_ret = ccidec_extract_meta_data((audio_stream_type_t)cci_dec->stream_type, cci_dec->metadata, &ctx);

    if (CODEC_SUCCESS != cci_ret)
    {
        STREAMER_LOG_CATA(DBG_CCID, ERRCODE_NOT_FOUND, "[CCID]Metadata Error\n");
        ret = false;
        goto Error;
    }

    send_msg_element(decoder, MSG_INFO_AUDIO_TYPE, element->decoder_type);

    element->sample_rate = cci_dec->metadata->sample_rate;
    send_msg_element(decoder, MSG_INFO_AUDIO_SAMPLERATE, element->sample_rate);
    STREAMER_LOG_DEBUG(DBG_CCID, "[CCID]Sample rate : %d\n", element->sample_rate);

    element->num_channels = cci_dec->metadata->num_channels;
    send_msg_element(decoder, MSG_INFO_AUDIO_CHANNELS, element->num_channels);
    STREAMER_LOG_DEBUG(DBG_CCID, "[CCID]Num channels : %d\n", element->num_channels);

    element->average_bit_rate = cci_dec->metadata->avg_bit_rate;
    send_msg_element(decoder, MSG_INFO_AUDIO_BITRATE, element->average_bit_rate);
    STREAMER_LOG_DEBUG(DBG_CCID, "[CCID]Avg bitrate : %d\n", element->average_bit_rate);

    element->duration = cci_dec->metadata->total_time;
    send_msg_element(decoder, MSG_INFO_DURATION, element->duration);
    STREAMER_LOG_DEBUG(DBG_CCID, "[CCID]Duration : %d\n", element->duration);

    smallest = smallest_int(2 * TITLE_SIZE, CASCFG_TRACK_TITLE_LEN);
    memcpy(metadata_string, cci_dec->metadata->song_title, smallest);

    /* sending title */
    send_msg_element(decoder, MSG_INFO_TITLE, (uintptr_t)metadata_string);
    STREAMER_LOG_DEBUG(DBG_CCID, "[CCID]Title : %d\n", cci_dec->metadata->song_title);
    smallest = smallest_int(2 * ARTIST_SIZE, CASCFG_TRACK_ARTIST_LEN);
    memcpy(metadata_string, cci_dec->metadata->song_artist, smallest);

    /* sending artist */
    send_msg_element(decoder, MSG_INFO_ARTIST, (uintptr_t)metadata_string);
    STREAMER_LOG_DEBUG(DBG_CCID, "[CCID]Artist : %d\n", cci_dec->metadata->song_artist);

    smallest = smallest_int(2 * ALBUM_SIZE, CASCFG_TRACK_ALBUM_LEN);
    memcpy(metadata_string, cci_dec->metadata->song_album, smallest);

    /* sending album */
    send_msg_element(decoder, MSG_INFO_ALBUM, (uintptr_t)metadata_string);
    STREAMER_LOG_DEBUG(DBG_CCID, "[CCID]Album : %d\n", cci_dec->metadata->song_album);
    smallest = smallest_int(2 * GENRE_SIZE, CASCFG_TRACK_GENRE_LEN);
    memcpy(metadata_string, cci_dec->metadata->song_genre, smallest);

    /* sending genre */
    send_msg_element(decoder, MSG_INFO_GENRE, (uintptr_t)metadata_string);
    STREAMER_LOG_DEBUG(DBG_CCID, "[CCID]Genre : %d\n", cci_dec->metadata->song_genre);
    /* sending track number */
    send_msg_element(decoder, MSG_INFO_TRACK, (uintptr_t)cci_dec->metadata->track_number);
    STREAMER_LOG_DEBUG(DBG_CCID, "[CCID]Track Number : %d\n", cci_dec->metadata->track_number);

    smallest = smallest_int(2 * YEAR_SIZE, CASCFG_TRACK_YEAR_LEN);
    memcpy(metadata_string, cci_dec->metadata->song_year, smallest);

    /* sending year */
    send_msg_element(decoder, MSG_INFO_YEAR, (uintptr_t)metadata_string);
    STREAMER_LOG_DEBUG(DBG_CCID, "[CCID]Year : %d\n", cci_dec->metadata->song_year);

    send_msg_element(decoder, MSG_STREAM_INFO_UPDATED, 0);

Error:
    return ret;
}

/* called by pipeline to activate & deactivate decoder pads */
uint8_t ccidec_sink_pad_activation_handler(StreamPad *pad, uint8_t active)
{
    ElementDecoder *element = NULL;
    bool ret                = true; /* return codecs used by streamer */
    int32_t dec_in_frame_size;
    int32_t dec_out_frame_size;
    CCIDecInfo *cci_dec = NULL;
    int32_t cci_mem_size;
    int32_t i = 0;

    STREAMER_FUNC_ENTER(DBG_CCID);
    /* get the parent element from the pad */
    element = (ElementDecoder *)PAD_PARENT(pad);

    /* activate the pad */
    if ((uint8_t) true == active)
    {
#ifdef DUMP_PCM
        FRESULT fret;
        pcm_file = OSA_MemoryAllocate(sizeof(FIL));
        fret     = f_open(pcm_file, _T("cci_pcm.pcm"), FA_WRITE | FA_CREATE_ALWAYS);
        if (fret)
        {
            STREAMER_LOG_CATA(DBG_CCID, ERRCODE_OUT_OF_MEMORY, "[CCID]f_open failed\n");
        }
#endif
        element->dec_info = NULL;

        /* allocate the cci decoder structure & init dec_info until filled */
        cci_dec = OSA_MemoryAllocate(sizeof(CCIDecInfo));
        if (NULL == cci_dec)
        {
            STREAMER_LOG_CATA(DBG_CCID, ERRCODE_OUT_OF_MEMORY, "[CCID]Out of memory: cci_dec\n");
            ret = false;
            goto Error;
        }
        /* Keep parent element pointer */
        cci_dec->element = element;
        /* initialize the cci decoder structure */
        cci_dec->init_done = false;
        cci_dec->eos_done  = false;

        /* Convert AUDIO decoder type to CCI type to use in CCI API */
        /* Keep AUDIO decoder type in element, use CCI type in cce_dec variable */
        cci_dec->stream_type = (audio_stream_type_t)cci_decoder_type_conversion((DecoderType)element->decoder_type);

        /* need dec_frame_size for both cases */
        if (codec_get_io_frame_size((audio_stream_type_t)cci_dec->stream_type, &dec_in_frame_size,
                                    &dec_out_frame_size) != 0)
        {
            ret = false;
            goto Error;
        }
        cci_dec->dec_frame_size = dec_in_frame_size;

        /* Query CCI for the memory required for internal structures
         * CCI will not allocate any memory internally, so allocate
         * memory for CCI usage, and save pointer which will be provided
         * to CCI for all APIs.
         */
        cci_mem_size = codec_get_mem_info((audio_stream_type_t)cci_dec->stream_type);
        STREAMER_LOG_DEBUG(DBG_CCID, "[CCID] CCI Decoder Memory %d\n", cci_mem_size);
        // handle negative value of cci_mem_size
        if (cci_mem_size > 0)
        {
            cci_dec->ccidec_memory = OSA_MemoryAllocate(cci_mem_size);
        }
        if (NULL == cci_dec->ccidec_memory)
        {
            STREAMER_LOG_CATA(DBG_CCID, ERRCODE_OUT_OF_MEMORY, "[CCID]Out of memory: ccidec_mem\n");
            ret = false;
            goto Error;
        }

        /* CCI requires a metadata structure, this will also be accessible
         * through CCIDecInfo
         */
        cci_dec->metadata = OSA_MemoryAllocate(sizeof(file_meta_data_t));
        STREAMER_LOG_DEBUG(DBG_CCID, "[CCID] CCI Metadata Memory %d\n", sizeof(file_meta_data_t));
        if (NULL == cci_dec->metadata)
        {
            STREAMER_LOG_CATA(DBG_CCID, ERRCODE_OUT_OF_MEMORY, "[CCID]Out of memory: metadata\n");
            ret = false;
            goto Error;
        }

        /* filesrc_buffer[] stores the data sent from filesrc and it is doubled
         * since the max bytes saved can be ((2 * frame size) - 1) */
        cci_dec->unaligned_filesrc_buffer[0] =
            OSA_MemoryAllocate(cci_dec->dec_frame_size + sizeof(uint32_t) + sizeof(RawPacketHeader));
        STREAMER_LOG_DEBUG(DBG_CCID, "[CCID] CCI unaligned_filesrc_buffer[0]  %d\n",
                           cci_dec->dec_frame_size + sizeof(uint32_t));
        if (NULL == cci_dec->unaligned_filesrc_buffer[0])
        {
            STREAMER_LOG_CATA(DBG_CCID, ERRCODE_OUT_OF_MEMORY, "[CCID]Out of memory: unaligned_filesrc_buffer[0]\n");
            ret = false;
            goto Error;
        }

        /* packed_buffer[] will store the data to be sent to audiosink */
        cci_dec->unaligned_packed_buffer = OSA_MemoryAllocate(
            (sizeof(AudioPacketHeader) + dec_out_frame_size + sizeof(uint32_t)) * NUM_OF_PING_PONG_BUFFER);

        STREAMER_LOG_DEBUG(
            DBG_CCID, "[CCID] CCI unaligned_packed_buffer  %d\n",
            (sizeof(AudioPacketHeader) + dec_out_frame_size + sizeof(uint32_t)) * NUM_OF_PING_PONG_BUFFER);
        if (NULL == cci_dec->unaligned_packed_buffer)
        {
            STREAMER_LOG_CATA(DBG_CCID, ERRCODE_OUT_OF_MEMORY, "[CCID]Out of memory: unaligned_packed_buffer\n");
            ret = false;
            goto Error;
        }
        /* set memtype after all allocations are successful */

        /* align buffers to 4 byte boundary */
        cci_dec->filesrc_buffer[0] =
            (uint8_t *)MEM4_ALIGN((uintptr_t)cci_dec->unaligned_filesrc_buffer[0] + sizeof(RawPacketHeader));

        for (i = 0; i < NUM_OF_PING_PONG_BUFFER; i++)
        {
            cci_dec->packed_buffer[i] =
                (void *)MEM4_ALIGN((uintptr_t)cci_dec->unaligned_packed_buffer +
                                   (i * (sizeof(AudioPacketHeader) + dec_out_frame_size + sizeof(uint32_t))));
        }

        /* Init input buffer usage variables */
        /* size of data available to send to the CCI */
        cci_dec->filesrc_size[0] = 0;
        cci_dec->filesrc_size[1] = 0;
        /* position of the srcfile based on data sent to CCI */
        cci_dec->filesrc_offset[0]     = 0;
        cci_dec->filesrc_buffer_idx[0] = 0;
        cci_dec->filesrc_offset[1]     = 0;
        cci_dec->filesrc_buffer_idx[1] = 0;
        cci_dec->buffer_index          = 0;

        /* store the cci decoder info in the decoder element */
        element->dec_info = cci_dec;

        /* exit activation handler */
        STREAMER_FUNC_EXIT(DBG_CCID);
        return (uint8_t) true;

        /* deactivate the pad */
    }
    else
    {
        /* init cci_dec here, but free memory during the common Error path */
        cci_dec = element->dec_info;
        if (cci_dec)
        {
            cci_dec->init_done = false;
            cci_dec->eos_done  = false;
        }
        if (cci_dec->stream_type == STREAM_TYPE_OGG_OPUS)
        {
            if (codec_reset((audio_stream_type_t)cci_dec->stream_type, &cci_dec->ccidec_memory) != 0)
            {
                ret = false;
                goto Error;
            }
        }
    }

Error:
    if (cci_dec)
    {
        /* deallocate all buffers used in CCI decoder structure */
        if (cci_dec->ccidec_memory)
        {
            OSA_MemoryFree(cci_dec->ccidec_memory);
            cci_dec->ccidec_memory = NULL;
        }
        if (cci_dec->metadata)
        {
            OSA_MemoryFree(cci_dec->metadata);
            cci_dec->metadata = NULL;
        }
        if (cci_dec->unaligned_filesrc_buffer[0])
        {
            OSA_MemoryFree(cci_dec->unaligned_filesrc_buffer[0]);
            cci_dec->unaligned_filesrc_buffer[0] = NULL;
        }
        if (cci_dec->unaligned_packed_buffer)
        {
            OSA_MemoryFree(cci_dec->unaligned_packed_buffer);
            cci_dec->unaligned_packed_buffer = NULL;
        }
        /* deallocate cci decoder structure */
        OSA_MemoryFree(cci_dec);
        cci_dec = NULL;
    }
    element->dec_info = NULL;
    STREAMER_FUNC_EXIT(DBG_CCID);
    return (uint8_t)ret;
}

uint8_t ccidec_sink_pad_event_handler(StreamPad *pad, StreamEvent *event)
{
    uint8_t ret             = true;
    ElementDecoder *element = NULL;
    CCIDecInfo *cci_dec     = NULL;

    STREAMER_FUNC_ENTER(DBG_CCID);
    CHK_ARGS(NULL == pad, false);

    /* get the parent element from the pad */
    element = PAD_PARENT(pad);
    CHK_ARGS(NULL == element, false);

    cci_dec = (CCIDecInfo *)element->dec_info;
    CHK_ARGS(NULL == cci_dec, false);

    /* handle the event */
    switch (EVENT_TYPE(event))
    {
        case EVENT_EOS:
            ret = false;
            break;

        /* ensure that the format is time units before sending it downstream */
        case EVENT_NEWSEGMENT:
            /* If the event is in bytes format then convert it to the time
             * according the bitrate and send push it forward to sink pad.
             */
            if (EVENT_FORMAT(event) == DATA_FORMAT_BYTES)
            {
                uint32_t time   = 0;
                uint32_t offset = EVENT_DATA(event);

                if (element->average_bit_rate)
                {
                    /* Calculate the rate in bytes */
                    uint32_t bit_rate = element->average_bit_rate >> 3;
                    time += ((offset / bit_rate) * 1000) + (((offset % bit_rate) * 1000) / bit_rate);
                    if (time > element->duration)
                    {
                        time = element->duration;
                    }
                    event_create_new_segment(event, DATA_FORMAT_TIME, time);
                }
                else
                {
                    if (cci_dec->metadata->audio_sub_type == MEDIA_SUBTYPE_ADTS ||
                        cci_dec->metadata->audio_sub_type == MEDIA_SUBTYPE_ADIF)
                    {
                        // No Bitrate info is available for ADTS files...
                        // Seek 0 is only possible for these files...
                        event_create_new_segment(event, DATA_FORMAT_TIME, time);
                    }
                }
            }
            ret = pad_push_event(ELEMENT_SRC_PAD(element, 0), event);
            break;

        case EVENT_FLUSH_STOP:
            /* Clear the buffered data */
            cci_dec->filesrc_size[0] = 0;
            cci_dec->filesrc_size[1] = 0;
            ret                      = false;
            break;

        case EVENT_FLUSH_START:
            ret = false;
            break;

        default:
            ret = false;
            break;
    }

    STREAMER_FUNC_EXIT(DBG_CCID);
    return ret;
}

uint8_t ccidec_src_pad_event_handler(StreamPad *pad, StreamEvent *event)
{
    uint8_t ret             = true;
    ElementDecoder *element = NULL;
    CCIDecInfo *cci_dec     = NULL;
    uint32_t offset;
    uint64_t offset_temp;

    STREAMER_FUNC_ENTER(DBG_CCID);
    CHK_ARGS(NULL == pad, false);

    /* get the parent element from the pad */
    element = PAD_PARENT(pad);
    CHK_ARGS(NULL == element, false);

    cci_dec = (CCIDecInfo *)element->dec_info;
    CHK_ARGS(NULL == cci_dec, false);

    /* handle the event */
    switch (EVENT_TYPE(event))
    {
        case EVENT_SEEK:
            /* Prevent SEEK if codec is not initialized */
            if ((uint8_t) false == cci_dec->init_done)
            {
                STREAMER_LOG_WARN(DBG_CCID, ERRCODE_INTERNAL,
                                  "[CCID]Seek is not applied as codec is un-initialized!\n");
                return (uint8_t)ERRCODE_NOT_SEEKABLE;
            }

            /* If the seek event is in time then convert it to bytes using the
             * current bitrate and forward it to upstream
             */
            if (EVENT_FORMAT(event) == DATA_FORMAT_TIME)
            {
                StreamEvent data_event;
                int32_t stream_size   = decoder_get_duration(element, DATA_FORMAT_BYTES);
                uint32_t time_in_msec = EVENT_DATA(event);

                STREAMER_LOG_DEBUG(DBG_CCID, "[CCID]Time: %d\n", time_in_msec);
                STREAMER_LOG_DEBUG(DBG_CCID, "[CCID]Average bit rate: %d\n", cci_dec->metadata->avg_bit_rate);

                if (cci_dec->stream_type != STREAM_TYPE_OGG_OPUS)
                {
                    uint32_t time_in_sec = time_in_msec / 1000;
                    /* get fractional seconds */
                    time_in_msec = time_in_msec - (time_in_sec * 1000);
                    /* use avg bit rate to estimate offset in bytes for whole seconds */
                    offset_temp = (uint64_t)((uint64_t)time_in_sec * cci_dec->metadata->avg_bit_rate);
                    offset      = (uint32_t)((uint64_t)(offset_temp) / 8);
                    /* and account for fractional seconds */
                    offset_temp = ((uint64_t)time_in_msec * cci_dec->metadata->avg_bit_rate);
                    offset += (uint32_t)((uint64_t)(offset_temp) / (8 * 1000));

                    if (offset > stream_size)
                        offset = stream_size;
                }
                else
                {
                    offset = time_in_msec;
                }

                STREAMER_LOG_DEBUG(DBG_CCID, "[CCID]Offset from event: %d\n", offset);

                if (cci_dec->metadata->audio_sub_type == MEDIA_SUBTYPE_ADTS ||
                    cci_dec->metadata->audio_sub_type == MEDIA_SUBTYPE_ADIF)
                {
                    // Seek 0 is possible only...
                    if (0 == offset)
                    {
                        offset = (uint32_t)codec_seek((audio_stream_type_t)cci_dec->stream_type,
                                                      &cci_dec->ccidec_memory, (int32_t)offset);

                        STREAMER_LOG_DEBUG(DBG_CCID, "[CCID]Offset after alignment: %d\n", offset);
                        event_create_seek(&data_event, DATA_FORMAT_BYTES, offset);

                        /* Push the event out from the sink pad */
                        ret = pad_push_event(ELEMENT_SINK_PAD(element, 0), &data_event);
                    }
                    else
                    {
                        STREAMER_LOG_WARN(DBG_CCID, ERRCODE_INTERNAL,
                                          "[CCID]Seek 0 is only supported for ADTS and ADIF sub stream types of AAC\n");

                        ret = ERRCODE_NOT_SEEKABLE;
                    }
                }
                else
                {
                    /* Use CCI to get offset aligned to a frame */
                    offset = (uint32_t)codec_seek((audio_stream_type_t)cci_dec->stream_type, &cci_dec->ccidec_memory,
                                                  (int32_t)offset);

                    if (cci_dec->stream_type == STREAM_TYPE_MP3)
                    {
                        _seek_file_data(0, (int32_t)offset + (int32_t)cci_dec->metadata->start_pos, SEEK_SET,
                                        (void *)cci_dec);
                    }
                    else if (cci_dec->stream_type != STREAM_TYPE_OGG_OPUS)
                    {
                        _seek_file_data(0, (int32_t)offset, SEEK_SET, (void *)cci_dec);
                    }

                    STREAMER_LOG_DEBUG(DBG_CCID, "[CCID]Offset after alignment: %d\n", offset);
                    event_create_seek(&data_event, DATA_FORMAT_BYTES, offset);

                    /* Push the event out from the sink pad */
                    ret = pad_push_event(ELEMENT_SINK_PAD(element, 0), &data_event);
                }
            }
            else
            {
                /* already in bytes */
                /* Use CCI to get offset aligned to a frame */
                if (cci_dec->stream_type == STREAM_TYPE_OGG_OPUS)
                {
                    /* A negative EVENT_DATA(event) means the number of bytes (raw seek) and a positive number the
                     * number of pcm samples (pcm seek) */
                    offset = (uint32_t)codec_seek((audio_stream_type_t)cci_dec->stream_type, &cci_dec->ccidec_memory,
                                                  -(int32_t)EVENT_DATA(event));
                }
                else
                {
                    offset = (uint32_t)codec_seek((audio_stream_type_t)cci_dec->stream_type, &cci_dec->ccidec_memory,
                                                  (int32_t)EVENT_DATA(event));
                }

                if (cci_dec->stream_type == STREAM_TYPE_MP3)
                    _seek_file_data(0, offset + cci_dec->metadata->start_pos, SEEK_SET, cci_dec);
                else
                    _seek_file_data(0, offset, SEEK_SET, cci_dec);
                /* Push the event out from the sink pad */
                ret = pad_push_event(ELEMENT_SINK_PAD(element, 0), event);
            }
            break;

        default:
            ret = false;
            break;
    }

    STREAMER_FUNC_EXIT(DBG_CCID);
    return ret;
}

uint8_t ccidec_src_pad_query_handler(StreamPad *pad, StreamQuery *query)
{
    uint8_t ret             = true;
    ElementDecoder *element = NULL;
    CCIDecInfo *cci_dec     = NULL;
    uint32_t smallest;
    char metadata_string[TITLE_SIZE];
    StreamData *data;

    STREAMER_FUNC_ENTER(DBG_CCID);

    CHK_ARGS(NULL == pad || NULL == query || NULL == QUERY_DATA(query), false);
    data = QUERY_DATA(query);

    /* get the parent element from the pad */
    element = PAD_PARENT(pad);
    CHK_ARGS(NULL == element, false);

    cci_dec = (CCIDecInfo *)element->dec_info;
    CHK_ARGS(NULL == cci_dec, false);

    data->string.unicode = false; /* all strings are in UTF-8 format */

    switch (QUERY_TYPE(query))
    {
        case INFO_ALBUM:
            smallest = smallest_int(2 * ALBUM_SIZE, CASCFG_TRACK_ALBUM_LEN);
            memcpy(metadata_string, cci_dec->metadata->song_album, smallest);
            data->string.string_buffer = metadata_string;
            data->string.num_chars     = strlen(metadata_string);
            break;

        case INFO_ARTIST:
            smallest = smallest_int(2 * ARTIST_SIZE, CASCFG_TRACK_ARTIST_LEN);
            memcpy(metadata_string, cci_dec->metadata->song_artist, smallest);
            data->string.string_buffer = metadata_string;
            data->string.num_chars     = strlen(metadata_string);
            break;

        case INFO_TITLE:
            smallest = smallest_int(2 * TITLE_SIZE, CASCFG_TRACK_TITLE_LEN);
            memcpy(metadata_string, cci_dec->metadata->song_title, smallest);
            data->string.string_buffer = metadata_string;
            data->string.num_chars     = strlen(metadata_string);
            break;

        case INFO_GENRE:
            smallest = smallest_int(2 * GENRE_SIZE, CASCFG_TRACK_GENRE_LEN);
            memcpy(metadata_string, cci_dec->metadata->song_genre, smallest);
            data->string.string_buffer = metadata_string;
            data->string.num_chars     = strlen(metadata_string);
            break;

        case INFO_YEAR:
            smallest = smallest_int(2 * YEAR_SIZE, CASCFG_TRACK_YEAR_LEN);
            memcpy(metadata_string, cci_dec->metadata->song_year, smallest);
            data->string.string_buffer = metadata_string;
            data->string.num_chars     = 4; /* Should not be more than 4 chars */
            break;

        case INFO_TRACK:
            /* convert string to decimal number */
            data->value8u = cci_dec->metadata->track_number;
            break;

        default:
            ret = false;
            break;
    }

    STREAMER_FUNC_EXIT(DBG_CCID);
    return ret;
}

int32_t ccidec_sink_pad_process_handler(StreamPad *pad)
{
    int8_t ret              = FLOW_OK;
    ElementDecoder *element = NULL;
    CCIDecInfo *cci_dec     = NULL;
    int32_t retCode         = 0; /* CCI return code */
    int32_t *callback_funct_array[3];

    STREAMER_FUNC_ENTER(DBG_CCID);
    CHK_ARGS(NULL == pad, false);

    /* get the parent element from the pad */
    element = PAD_PARENT(pad);
    CHK_ARGS(NULL == element, false);

    cci_dec = (CCIDecInfo *)element->dec_info;
    CHK_ARGS(NULL == cci_dec, false);
    if ((uint8_t) false == cci_dec->init_done)
    {
        retCode = (int32_t)get_cci_metadata(element);
        if (retCode == (uint8_t) false)
        {
            /* metadata extraction failed */
            ret = FLOW_ERROR;
            goto Error;
        }
        /* seek to the playback start position of the file */
        if (cci_dec->stream_type == STREAM_TYPE_MP3)
        {
            if (cci_dec->metadata->start_pos > 0)
            {
                STREAMER_LOG_DEBUG(DBG_CCID, "[CCID] File Start Pos = %d\n", cci_dec->metadata->start_pos);
                _seek_file_data(0, cci_dec->metadata->start_pos, SEEK_SET, cci_dec);
            }
        }
        else
        {
            _seek_file_data(0, 0, SEEK_SET, cci_dec);
        }
    }
    while ((uint8_t) false == cci_dec->init_done)
    {
        /* set cci callbacks pointer functions array required by codec init.*/
        callback_funct_array[0] = (int32_t *)&_get_file_data;
        callback_funct_array[1] = (int32_t *)&_seek_file_data;
        callback_funct_array[2] = (int32_t *)&_get_file_position;

        /************** Here start cci init ***************/
        retCode = codec_init((audio_stream_type_t)cci_dec->stream_type, &cci_dec->ccidec_memory, callback_funct_array,
                             (int32_t *)&cci_dec);
        if (retCode == CODEC_INIT_ERROR)
        {
            // Init failed
            STREAMER_LOG_CATA(DBG_CCID, ERRCODE_GENERAL_ERROR, "[CCID]Codec Init Failed\n");
            ret = FLOW_ERROR;
            goto Error;
        }
        if (retCode == CODEC_UNSUPPORTED_FILE)
        {
            // Init failed
            STREAMER_LOG_CATA(DBG_CCID, ERRCODE_GENERAL_ERROR, "[CCID]Codec Unsupported File\n");
            ret = FLOW_ERROR;
            goto Error;
        }
        if (retCode == CODEC_MORE_DATA_REQUIRED)
        {
            /* just continue */
            STREAMER_LOG_DEBUG(DBG_CCID, "[CCID] More Data Processing Required for Init\n");
        }
        if (retCode == CODEC_SUCCESS)
        {
            /* Init done */
            cci_dec->init_done = true;
            STREAMER_LOG_DEBUG(DBG_CCID, "[CCID] Codec Init Done Successfully\n");
        }
    }
    /********** Init complete, start cci decode frame ***************/
    if (cci_dec->eos_done == (uint8_t) false)
    {
        retCode = ccidec_decode_frame(element);
        if (CODEC_SUCCESS != retCode && CODEC_END_OF_DECODE != retCode && CODEC_BROKEN_FRAME != retCode)
        {
            ret = FLOW_ERROR;
            STREAMER_LOG_ERR(DBG_CCID, ERRCODE_INTERNAL, "[CCID] Unexpected flow error\n");
        }
        else if (CODEC_END_OF_DECODE == retCode)
        {
            StreamEvent event;

            STREAMER_LOG_DEBUG(DBG_CCID, "[CCID]EOS EVENT\n");
            /* send eos to peer pad */
            cci_dec->eos_done = true;
            event_create_eos(&event);
            pad_push_event(pad, &event);
        }
    }
    else
    {
        /* After "EOS" is hit, continuously executing streamer task is delayed
        to make sure that the player task gets some time slice to handle its
        pending messages */
        OSA_TimeDelay(10);
    }

Error:
    STREAMER_FUNC_EXIT(DBG_CCID);
    return ret;
}
