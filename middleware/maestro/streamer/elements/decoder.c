/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    decoder.c
 * @brief   This file contains the decoder element functionality.
 *
 */

/*
 * INCLUDE FILES
 */
#include "streamer.h"
#include "decoder.h"
#include "decoder_pads.h"
#include "pipeline.h"
#include "streamer_element_properties.h"
#include "audio_cfg.h"

/* CCI uses the same decoder element for all formats
 * CCI operates is pull mode from the filesrc, so the pad_process_handler
 * is used instead of the chain_handler
 */
extern uint8_t ccidec_src_pad_event_handler(StreamPad *, StreamEvent *);
extern uint8_t ccidec_src_pad_query_handler(StreamPad *, StreamQuery *);
extern uint8_t ccidec_sink_pad_activation_handler(StreamPad *, uint8_t);
extern uint8_t ccidec_sink_pad_event_handler(StreamPad *, StreamEvent *);
extern int32_t ccidec_sink_pad_process_handler(StreamPad *);

/**
 * @brief Decoder handler function pointers
 *
 */
static const DecoderHandlersType handlers_cci = {
    NULL, ccidec_src_pad_event_handler,  ccidec_src_pad_query_handler,   ccidec_sink_pad_activation_handler,
    NULL, ccidec_sink_pad_event_handler, ccidec_sink_pad_process_handler};

int32_t decoder_change_state(StreamElement *element, PipelineState new_dec_state)
{
    PipelineStateChange state_change;
    ElementDecoder *dec_element = (ElementDecoder *)element;
    uint32_t ret                = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_DECODER);

    CHK_ARGS(NULL == dec_element, STREAM_ERR_INVALID_ARGS);

    /* get the transition status with the new passed state and the existing
     * state of the decoder element
     */
    state_change = (PipelineStateChange)STATE_TRANSITION(dec_element->state, new_dec_state);

    switch (state_change)
    {
        case STATE_CHANGE_NULL_TO_READY:
            /* check for and open resources
             * N/A for decoder
             */
            STREAMER_LOG_DEBUG(DBG_DECODER, "[Decoder]STATE_CHANGE_NULL_TO_READY\n");
            if ((dec_element->decoder_type == DECODER_TYPE_UNKNOWN) || (dec_element->handlers == NULL))
            {
                STREAMER_LOG_ERR(DBG_DECODER, ERRCODE_NOT_SUPPORTED, "[Decoder] unknown decoder type\n");
                ret = STREAM_ERR_GENERAL;
            }
            break;
        case STATE_CHANGE_READY_TO_PAUSED:
            /* activate pads
             * These functions are different for different docders
             * Activation is done in the activate pads function
             */
            STREAMER_LOG_DEBUG(DBG_DECODER, "[Decoder]STATE_CHANGE_READY_TO_PAUSED\n");
            break;
        case STATE_CHANGE_PAUSED_TO_PLAYING:
            STREAMER_LOG_DEBUG(DBG_DECODER, "[Decoder]STATE_CHANGE_PAUSED_TO_PLAYING\n");
            break;
            /* do nothing */
        case STATE_CHANGE_PLAYING_TO_PAUSED:
            STREAMER_LOG_DEBUG(DBG_DECODER, "[Decoder]STATE_CHANGE_PLAYING_TO_PAUSED\n");
            break;
            /* do nothing */
        case STATE_CHANGE_PAUSED_TO_READY:
            STREAMER_LOG_DEBUG(DBG_DECODER, "[Decoder]STATE_CHANGE_PAUSED_TO_READY\n");
            break;
            /* Pads will be deactivated here. */
        case STATE_CHANGE_READY_TO_NULL:
            STREAMER_LOG_DEBUG(DBG_DECODER, "[Decoder]STATE_CHANGE_READY_TO_NULL\n");
            break;
            /* close resources if anything was opened
             * N/A for decoder
             */
        default:
            break;
    }

    STREAMER_FUNC_EXIT(DBG_DECODER);
    return ret;
}

uint32_t decoder_get_duration(ElementDecoder *element, StreamDataFormat format)
{
    StreamQuery query;
    StreamData data;
    StreamPad *pad  = ELEMENT_SINK_PAD(element, 0); /* Sink pad */
    StreamPad *peer = PAD_PEER(pad);                /* Connected peer */

    STREAMER_FUNC_ENTER(DBG_DECODER);

    query_create(&query, INFO_DURATION, format, DATA_TYPE_UINT32, &data);

    data.value32u = 0;
    pad_query(peer, &query);

    STREAMER_FUNC_EXIT(DBG_DECODER);
    return data.value32u;
}

bool decoder_is_source_seekable(ElementDecoder *decoder, StreamDataFormat format)
{
    StreamQuery seek_query;
    StreamData is_seekable;
    StreamPad *sink = ELEMENT_SINK_PAD(decoder, 0); /* Sink pad */
    StreamPad *peer = PAD_PEER(sink);               /* Connected peer */
    bool ret        = false;

    STREAMER_FUNC_ENTER(DBG_DECODER);

    query_create(&seek_query, INFO_TIME_SEEKABLE, format, DATA_TYPE_bool, &is_seekable);

    if ((uint8_t) true == pad_query(peer, &seek_query))
        ret = is_seekable.valuebool;

    STREAMER_FUNC_EXIT(DBG_DECODER);
    return ret;
}

void decoder_set_duration(ElementDecoder *element, uint32_t stream_size, uint32_t bit_rate)
{
    STREAMER_FUNC_ENTER(DBG_DECODER);

    /* Calculate the rate in bytes */
    bit_rate = element->average_bit_rate >> 3;

    /* Get the total time */
    element->duration = ((stream_size / bit_rate) * 1000) + (((stream_size % bit_rate) * 1000) / bit_rate);

    STREAMER_FUNC_EXIT(DBG_DECODER);
}

int32_t decoder_init_element(StreamElement *element)
{
    StreamReturnType ret        = STREAM_OK;
    ElementDecoder *dec_element = (ElementDecoder *)element;
    uint32_t i                  = 0;

    STREAMER_FUNC_ENTER(DBG_DECODER);

    CHK_ARGS(NULL == dec_element, STREAM_ERR_INVALID_ARGS);

    /* initialize element fields */
    dec_element->chunk_size         = 0;
    dec_element->sample_rate        = 0;
    dec_element->current_bit_rate   = 0;
    dec_element->average_bit_rate   = 0;
    dec_element->num_channels       = 0;
    dec_element->endian             = AF_LITTLE_ENDIAN;
    dec_element->output_wordlength  = DECODER_OUTPUT_WORD_LENGTH_16;
    dec_element->output_interleaved = DECODER_INTERLEAVED;
    dec_element->dec_info           = NULL;
    dec_element->num_decoded_frames = 0;
    dec_element->mute_frames        = 0;

    /* set element change state function */
    dec_element->change_state = decoder_change_state;
    dec_element->set_property = decoder_set_property;
    dec_element->get_property = decoder_get_property;
    dec_element->handlers     = NULL;

    /* initialize src pads handlers */
    for (i = 0; i < NUM_SRCS(TYPE_ELEMENT_DECODER); i++)
    {
        PadSrc *pad;
        pad = (PadSrc *)ELEMENT_SRC_PAD(dec_element, i);

        pad->query_handler      = decoder_src_pad_query_handler;
        pad->event_handler      = decoder_src_pad_event_handler;
        pad->activate_push      = decoder_src_pad_activation_handler;
        pad->activation_handler = NULL;
        pad->pull_handler       = NULL;
        pad->process_handler    = decoder_sink_pad_process_handler;
        pad->process_precheck   = NULL;
    }

    /* initialize sink pads handlers */
    for (i = 0; i < NUM_SINKS(TYPE_ELEMENT_DECODER); i++)
    {
        PadSink *pad;
        pad = (PadSink *)ELEMENT_SINK_PAD(dec_element, i);

        pad->event_handler      = decoder_sink_pad_event_handler;
        pad->activation_handler = decoder_sink_pad_activation_handler;
        pad->chain_handler      = decoder_sink_pad_chain_handler;
        pad->query_handler      = NULL;
        pad->process_precheck   = NULL;
        pad->process_handler    = NULL;
    }

    STREAMER_FUNC_EXIT(DBG_DECODER);
    return ret;
}

int32_t decoder_set_type(ElementHandle element, DecoderType dec_type)
{
    ElementDecoder *dec  = (ElementDecoder *)element;
    StreamReturnType ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_DECODER);

    CHK_ARGS(dec == NULL || dec_type >= LAST_DECODER_TYPE, STREAM_ERR_INVALID_ARGS);

    /* will always hold AUDIO type, CCI needs type conversion */
    dec->decoder_type = dec_type;
    dec->handlers     = (void *)&handlers_cci;
    STREAMER_FUNC_EXIT(DBG_DECODER);
    return ret;
}

int32_t decoder_activate_parse_tag(ElementHandle element, uint8_t activate)
{
    ElementDecoder *dec  = (ElementDecoder *)element;
    StreamReturnType ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_DECODER);

    CHK_ARGS(dec == NULL, STREAM_ERR_INVALID_ARGS);

    dec->parse_tag = activate;

    STREAMER_FUNC_EXIT(DBG_DECODER);
    return ret;
}

void vUpdateAverageBitrate(ElementDecoder *element)
{
    STREAMER_FUNC_ENTER(DBG_DECODER);

    /* Find moving average of bitrate */
    element->average_bit_rate =
        ((element->average_bit_rate * element->num_decoded_frames) + element->current_bit_rate) /
        (element->num_decoded_frames + 1);

    STREAMER_FUNC_EXIT(DBG_DECODER);
}

int32_t decoder_set_property(StreamElement *element, uint16_t prop_id, uint32_t prop_value)
{
    int32_t ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_DECODER);

    switch (prop_id)
    {
        case PROP_DECODER_DECODER_TYPE:
            STREAMER_LOG_DEBUG(DBG_DECODER, "[Decoder]set decode type :%d\n", prop_value);
            ret = decoder_set_type((ElementHandle)element, (DecoderType)prop_value);
            break;
        case PROP_DECODER_PARSE_TAG:
            ret = decoder_activate_parse_tag((ElementHandle)element, (prop_value != 0));
            break;
        default:
            ret = STREAM_ERR_INVALID_ARGS;
            break;
    }

    STREAMER_FUNC_EXIT(DBG_DECODER);
    return ret;
}

int32_t decoder_get_property(StreamElement *element, uint16_t prop_id, uint32_t *prop_value)
{
    ElementDecoder *decoder = (ElementDecoder *)element;
    StreamReturnType ret    = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_DECODER);

    CHK_ARGS(decoder == NULL || NULL == prop_value, STREAM_ERR_INVALID_ARGS);

    switch (prop_id)
    {
        case PROP_DECODER_DECODER_TYPE:
            *prop_value = decoder->decoder_type;
            break;
        case PROP_DECODER_PARSE_TAG:
            *prop_value = (int32_t)decoder->parse_tag;
            break;
        default:
            ret = STREAM_ERR_INVALID_ARGS;
            break;
    }

    STREAMER_FUNC_EXIT(DBG_DECODER);
    return ret;
}
