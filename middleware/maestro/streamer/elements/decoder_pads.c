/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    decoder_pads.c
 * @brief   This file contains the pad functions used in the
 *          decoder element.
 * These functions are wrappers that call the decoder specific
 * implementation defined in DecoderHandlerType
 *
 */

/*
 * INCLUDE FILES
 */
#include "audio_cfg.h"
#include "streamer.h"
#include "decoder.h"
#include "decoder_pads.h"

/*
 * Pad Functions:
 * These are wrapper functions that call the specific decoder
 * implementation loaded in DecoderHandlersType.
 * decoder_init_element() will load these functions into
 * the appropriate Pad*Func
 */

uint8_t decoder_src_pad_activation_handler(StreamPad *pad, uint8_t active)
{
    uint8_t ret = true;
    ElementDecoder *dec_element;

    STREAMER_FUNC_ENTER(DBG_DECODER);

    CHK_ARGS(NULL == pad, false);

    /* get the parent element from the pad */
    dec_element = (ElementDecoder *)PAD_PARENT(pad);

    CHK_ARGS(dec_element == NULL, false);

    /* call decoder sink activation_handler */
    if (dec_element->handlers != NULL && dec_element->handlers->src_activate != NULL)
    {
        ret = dec_element->handlers->src_activate(pad, active);
    }

    STREAMER_FUNC_EXIT(DBG_DECODER);
    return ret;
}

uint8_t decoder_src_pad_event_handler(StreamPad *pad, StreamEvent *event)
{
    uint8_t ret = false;
    ElementDecoder *dec_element;
    PadEventFunc event_func = NULL;
    StreamPad *sink;

    STREAMER_FUNC_ENTER(DBG_DECODER);

    CHK_ARGS(NULL == pad, false);

    /* get the parent element from the pad */
    dec_element = (ElementDecoder *)PAD_PARENT(pad);

    CHK_ARGS(dec_element == NULL, false);

    /* Decoder specific event handler function */
    if (dec_element->handlers != NULL)
        event_func = dec_element->handlers->src_event;

    /* Sink pad */
    sink = ELEMENT_SINK_PAD(dec_element, 0);

    STREAMER_LOG_DEBUG(DBG_DECODER, "[Decoder]Event: %d\n", EVENT_TYPE(event) >> 4);

    /* For a seek event, calls the decoder specific event handler function.*/
    if ((event_func != NULL) && ((uint8_t) false == ret))
    {
        /* call the decoder specific event handler function */
        ret = event_func(pad, event);
    }

    /* If event not handled till now then push the event forward. */
    if ((uint8_t) false == ret)
    {
        /* handle the event */
        switch (EVENT_TYPE(event))
        {
            case EVENT_SEEK:
                /* Push the event out from the source pad */
                ret = pad_push_event(sink, event);
                break;
            default:
                /* Push the event out from the source pad */
                ret = pad_push_event(sink, event);
                break;
        }
    }

    STREAMER_FUNC_EXIT(DBG_DECODER);
    return ret;
}

uint8_t decoder_src_pad_query_handler(StreamPad *pad, StreamQuery *query)
{
    uint8_t ret = false;
    ElementDecoder *dec_element;
    StreamData *data;

    STREAMER_FUNC_ENTER(DBG_DECODER);

    CHK_ARGS(NULL == pad || NULL == query || NULL == QUERY_DATA(query), false);
    data = QUERY_DATA(query);

    /* get the parent element from the pad */
    dec_element = (ElementDecoder *)PAD_PARENT(pad);

    CHK_ARGS(dec_element == NULL, false);

    if (dec_element->handlers != NULL && dec_element->handlers->src_query != NULL)
    {
        ret = dec_element->handlers->src_query(pad, query);
    }

    STREAMER_LOG_DEBUG(DBG_DECODER, "[Decoder]decoder_src_pad_query_handler: query[%d]\n", QUERY_TYPE(query));

    if ((uint8_t) false == ret)
    {
        ret = true;

        switch (QUERY_TYPE(query))
        {
            case INFO_AUDIO_BITRATE:
                data->value32u = dec_element->average_bit_rate;
                break;

            case INFO_AUDIO_CHANNELS:
                data->value32u = dec_element->num_channels;
                break;

            case INFO_AUDIO_SAMPLERATE:
                data->value32u = dec_element->sample_rate;
                break;

            case INFO_AUDIO_TYPE:
                data->value32u = dec_element->decoder_type;
                break;

            case INFO_DURATION:
                data->value32u = dec_element->duration;
                break;

            default:
            {
                StreamPad *sink = ELEMENT_SINK_PAD(dec_element, 0);
                StreamPad *peer = PAD_PEER(sink);

                if (peer)
                    ret = pad_query(peer, query);
            }
            break;
        }
    }

    STREAMER_FUNC_EXIT(DBG_DECODER);
    return ret;
}

uint8_t decoder_sink_pad_activation_handler(StreamPad *pad, uint8_t active)
{
    uint8_t ret = true;
    ElementDecoder *dec_element;

    STREAMER_FUNC_ENTER(DBG_DECODER);

    CHK_ARGS(NULL == pad, false);

    /* get the parent element from the pad */
    dec_element = (ElementDecoder *)PAD_PARENT(pad);

    CHK_ARGS(dec_element == NULL, false);

    if (dec_element->handlers != NULL)
    {
        PadActivateFunc handler = dec_element->handlers->sink_activate;

        /* call decoder sink activation_handler */
        if (handler != NULL)
        {
            /* For sink pad, get the stream lock before calling the deactivate
             * function. Sink pads normally are driven by the connected source
             * pad and the stream lock is obtained so that the pad is servicing
             * any data from the source pad.
             */
            if ((StreamPad *)&dec_element->sink_pad[0] == pad)
            {
                /* Reset all decoder variables when it is activated */
                if ((uint8_t) true == active)
                {
                    dec_element->chunk_size         = 0;
                    dec_element->sample_rate        = 0;
                    dec_element->current_bit_rate   = 0;
                    dec_element->average_bit_rate   = 0;
                    dec_element->duration           = 0;
                    dec_element->num_channels       = 0;
                    dec_element->endian             = AF_LITTLE_ENDIAN;
                    dec_element->output_wordlength  = DECODER_OUTPUT_WORD_LENGTH_16;
                    dec_element->output_interleaved = DECODER_INTERLEAVED;
                    dec_element->dec_info           = NULL;
                    dec_element->num_decoded_frames = 0;
                    dec_element->mute_frames        = 0;
                }

                ret = handler(pad, active);
            }
            /* For activation, activate the peer source pad in push mode. */
            if ((uint8_t) true == ret)
            {
                /* Activate/Deactivate peer source pads in PULL mode */
                ret = pad_activate_pull(pad->peer, active);
            }
        }
    }

    STREAMER_FUNC_EXIT(DBG_DECODER);
    return ret;
}

uint8_t decoder_sink_pad_event_handler(StreamPad *pad, StreamEvent *event)
{
    ElementDecoder *dec_element = NULL;
    StreamPad *src;
    uint8_t ret = true;

    STREAMER_FUNC_ENTER(DBG_DECODER);

    STREAMER_LOG_DEBUG(DBG_DECODER, "decoder_sink_pad_event_handler %d\n", EVENT_TYPE(event));

    CHK_ARGS(NULL == pad, false);

    /* get the parent element from the pad */
    dec_element = (ElementDecoder *)PAD_PARENT(pad);
    src         = ELEMENT_SRC_PAD(dec_element, 0);

    /* call the decoder specific event handler function */
    if (dec_element->handlers != NULL && dec_element->handlers->sink_event != NULL)
    {
        ret = dec_element->handlers->sink_event(pad, event);
    }

    if ((uint8_t) false == ret)
    {
        /* handle the event */
        switch (EVENT_TYPE(event))
        {
            case EVENT_NEWSEGMENT:
                /* Reset the number of frames decoded */
                dec_element->num_decoded_frames = 0;

                /* If the event is in bytes format then convert it to the time
                 * according the bitrate and send push it forward to sink pad.
                 */
                if (EVENT_FORMAT(event) == DATA_FORMAT_BYTES)
                {
                    uint32_t time   = 0;
                    uint32_t offset = EVENT_DATA(event);

                    if (dec_element->average_bit_rate)
                    {
                        /* Calculate the rate in bytes */
                        uint32_t bit_rate = dec_element->average_bit_rate >> 3;

                        time += ((offset / bit_rate) * 1000) + (((offset % bit_rate) * 1000) / bit_rate);

                        event_create_new_segment(event, DATA_FORMAT_TIME, time);
                    }
                }
                ret = pad_push_event(src, event);
                break;

            case EVENT_EOS:
                /* Push the event out from the source pad */
                ret = pad_push_event(src, event);
                break;

            case EVENT_FLUSH_START:
                /* Push the event out from the source pad */
                ret = pad_push_event(src, event);
                break;

            case EVENT_FLUSH_STOP:
                /* Push the event out from the source pad */
                ret = pad_push_event(src, event);
                break;

            default:
                break;
        }
    }

    STREAMER_FUNC_EXIT(DBG_DECODER);
    return ret;
}

FlowReturn decoder_sink_pad_chain_handler(StreamPad *pad, StreamBuffer *buffer)
{
    ElementDecoder *dec_element = NULL;
    FlowReturn ret              = FLOW_OK;

    STREAMER_FUNC_ENTER(DBG_DECODER);

    if (NULL == pad)
    {
        STREAMER_FUNC_EXIT(DBG_DECODER);
        return FLOW_BAD_PARAMETER;
    }

    /* get the parent element from the pad */
    dec_element = (ElementDecoder *)PAD_PARENT(pad);

    /* call the decoder specific chain data function */
    if (dec_element->handlers != NULL && dec_element->handlers->sink_chain != NULL)
    {
        ret = dec_element->handlers->sink_chain(pad, buffer);
    }

    STREAMER_FUNC_EXIT(DBG_DECODER);
    return ret;
}

int32_t decoder_sink_pad_process_handler(StreamPad *pad)
{
    uint8_t ret = true;
    ElementDecoder *dec_element;

    STREAMER_FUNC_ENTER(DBG_DECODER);

    CHK_ARGS(NULL == pad, false);

    /* get the parent element from the pad */
    dec_element = (ElementDecoder *)PAD_PARENT(pad);

    CHK_ARGS(dec_element == NULL, false);

    /* call decoder sink activation_handler */
    if (dec_element->handlers != NULL && dec_element->handlers->sink_process != NULL)
    {
        ret = dec_element->handlers->sink_process(pad);
    }

    STREAMER_FUNC_EXIT(DBG_DECODER);
    return ret;
}
