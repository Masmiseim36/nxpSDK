/*
 * Copyright 2018-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    audio_sink.c
 * @brief  This file contains the audio sink element implementation.
 */
#include <string.h>
#include "audio_cfg.h"
#include "streamer.h"
#include "decoder.h"
#include "audio_sink.h"
#include "pipeline.h"
#include "streamer_element_properties.h"

#ifdef CASCFG_PLATFORM_FREERTOS
#include "audio_sink_pcmrtos.h"
extern const AudioSinkPadDeviceIOFuncType audio_sink_device_pcmrtos;
#endif

/*!
 * @param pad: pad handle
 * @param active: boolean o activate/disactivate pad
 *
 * @returns: pad error return
 *
 * @brief This function activates the sink pad and allocates
 * memory for the audio sink element.
 *
 */
static uint8_t audiosink_sink_pad_activation_handler(StreamPad *pad, uint8_t active)
{
    uint8_t ret = true;
    ElementAudioSink *audio_sink_ptr;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SINK);

    CHK_ARGS(NULL == pad, false);

    /* get the parent element from the pad */
    audio_sink_ptr = (ElementAudioSink *)PAD_PARENT(pad);

    STREAMER_LOG_DEBUG(DBG_AUDIO_SINK, "[Audio sink]Activate pad: %d\n", active);

    if (NULL == audio_sink_ptr->device_ptr)
    {
        STREAMER_FUNC_EXIT(DBG_AUDIO_SINK);
        return false;
    }

    /* For sink pad, get the stream lock before calling the deactivate
     * function. Sink pads normally are driven by the connected source pad
     * and the stream lock is obtained so that the pad is servicing any data
     * from the source pad.
     */

    /* call the device specific activation function */
    ret = audio_sink_ptr->device_ptr->activate_func(pad, active);

    if ((uint8_t) true == ret)
    {
        audio_sink_ptr->elapsed_time_msec  = 0;
        audio_sink_ptr->elapsed_time_usec  = 0;
        audio_sink_ptr->time_reported_msec = 0;

        /* Activate/Deactivate peer source pads according to scheduling mode */
        if (SCHEDULING_PUSH == pad->scheduling)
        {
            ret = pad_activate_push(pad->peer, active);
        }
        else if (SCHEDULING_PULL == pad->scheduling)
        {
            ret = pad_activate_pull(pad->peer, active);
        }
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_SINK);
    return ret;
}

/*!
 * @param pad: pad instance
 * @param buffer: input buffer
 *
 * @brief This pad function accumulates data from the decoder and
 * pushes it onto the selected audio driver.
 * The audio sink maintains a double buffer and starts sending data once both
 * the buffers are full. It blocks if the previous data is not sent out by the
 * driver for some reason.
 *
 * @returns: FlowReturn error types
 */

static FlowReturn audiosink_sink_pad_chain_handler(StreamPad *pad, StreamBuffer *buffer)
{
    ElementAudioSink *audio_sink_ptr;
    AudioPacketHeader *data_packet = NULL;
    uint32_t chunk_size;
    uint32_t sample_rate;
    uint32_t channels;
    uint32_t time_report_diff;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SINK);

    CHK_ARGS((NULL == pad) || (NULL == buffer), FLOW_UNEXPECTED);
    CHK_ARGS((((RawPacketHeader *)(buffer->buffer))->id != AUDIO_DATA), FLOW_NOT_SUPPORTED);

    /* get the parent element from the pad */
    audio_sink_ptr = (ElementAudioSink *)PAD_PARENT(pad);

    if (NULL == audio_sink_ptr->device_ptr)
    {
        STREAMER_FUNC_EXIT(DBG_AUDIO_SINK);
        return FLOW_NOT_SUPPORTED;
    }

    time_report_diff = audio_sink_ptr->time_report_diff;

    /* Get chunk size and sample_rate from packet header */
    data_packet = (AudioPacketHeader *)BUFFER_PTR(buffer);
    chunk_size  = AUDIO_CHUNK_SIZE(data_packet);
    sample_rate = AUDIO_SAMPLE_RATE(data_packet);
    channels    = AUDIO_NUM_CHANNELS(data_packet);

    if (sample_rate)
    {
        audio_sink_ptr->sample_rate = sample_rate;
    }
    if (channels)
    {
        audio_sink_ptr->num_channels = channels;
    }

    if (chunk_size && sample_rate)
    {
        uint32_t bytes_per_sample;
        bytes_per_sample = AUDIO_BITS_PER_SAMPLE(data_packet) / 8;
        audio_sink_ptr->elapsed_time_usec +=
            (((uint64_t)chunk_size * (uint64_t)1000000) / (bytes_per_sample * channels * sample_rate));

        audio_sink_ptr->elapsed_time_msec += (audio_sink_ptr->elapsed_time_usec / 1000);
        audio_sink_ptr->elapsed_time_usec = (audio_sink_ptr->elapsed_time_usec % 1000);

        if (time_report_diff)
        {
            uint32_t old_time, new_time;

            old_time = audio_sink_ptr->time_reported_msec / time_report_diff;
            new_time = audio_sink_ptr->elapsed_time_msec / time_report_diff;

            if (old_time != new_time)
            {
                audio_sink_ptr->time_reported_msec = audio_sink_ptr->elapsed_time_msec;
                send_msg_element((StreamElement *)audio_sink_ptr, MSG_INFO_POSITION,
                                 audio_sink_ptr->time_reported_msec);
            }
        }
    }

    STREAMER_LOG_DEBUG(DBG_AUDIO_SINK, "[AudioSink]audio_sink_sink_pad_chain_handler [%d] [%d]\n", chunk_size,
                       sample_rate);

    /* call the device specific Chain Handler function */
    STREAMER_FUNC_EXIT(DBG_AUDIO_SINK);
    return audio_sink_ptr->device_ptr->chain_func(pad, buffer);
}

/*!
 *
 * @param pad: Element pad
 *
 * @brief Function is the loop function of the audio sink pad which
 * runs in the task thread. It reads the chunk_size of data
 * and pushes it to peer element sink pad.
 *
 */

static int32_t audiosink_sink_pad_process_handler(StreamPad *pad)
{
    FlowReturn flowret;
    ElementAudioSink *element = (ElementAudioSink *)PAD_PARENT(pad);
    StreamBuffer buffer;

    flowret = (FlowReturn)element->device_ptr->proc_func(pad);

    if (FLOW_OK == flowret)
    {
        /* TODO: This call has a dummy size just to pass the check against
         * 0, but in the general case, the size should be dictated by the
         * sink device.
         */
        flowret = pad_pull_range(pad, 0, 1000, &buffer);

        if (FLOW_OK == flowret)
        {
            (void)audiosink_sink_pad_chain_handler(pad, &buffer);
        }
    }

    return (int32_t)flowret;
}

/**
 * @param pad: pad instance
 * @param event: event enumeration
 *
 * @brief  This function is the calls the specified audio device
 * driver implementation of the sink pad event handler.
 *
 * @returns: true or false
 *
 */

static uint8_t audiosink_sink_pad_event_handler(StreamPad *pad, StreamEvent *event)
{
    ElementAudioSink *audio_sink_ptr;
    Pipeline *pipeline = PAD_PARENT_PIPELINE(pad);
    uint8_t ret        = true;
#ifdef LOG_ENABLED
    uint32_t milliseconds, seconds, minutes;
#endif
    STREAMER_FUNC_ENTER(DBG_AUDIO_SINK);

    CHK_ARGS(NULL == pad, false);

    /* get the parent element from the pad */
    audio_sink_ptr = (ElementAudioSink *)PAD_PARENT(pad);

    /* call the device specific StreamEvent Handler function */
    if (audio_sink_ptr->device_ptr != NULL)
    {
        ret = audio_sink_ptr->device_ptr->event_func(pad, event);
    }

    if ((uint8_t) false == ret)
    {
        /* handle the event */
        STREAMER_LOG_DEBUG(DBG_AUDIO_SINK, "[Audio sink] handle event %d\n", EVENT_TYPE(event));
        switch (EVENT_TYPE(event))
        {
            case EVENT_EOS:
                send_msg_element((StreamElement *)audio_sink_ptr, MSG_EOS, 0);
#ifdef LOG_ENABLED
                milliseconds = audio_sink_ptr->elapsed_time_msec % 1000;
                seconds      = (audio_sink_ptr->elapsed_time_msec / 1000) % 60;
                minutes      = audio_sink_ptr->elapsed_time_msec / 60000;
                STREAMER_LOG_DEBUG(DBG_AUDIO_SINK, "[Audio sink]EOS on %d:%d.%d\n", minutes, seconds, milliseconds);
#endif
                break;

            case EVENT_FLUSH_START:
                /* In case the current state is paused then unblock the pad
                 * signal. Chain function should also check the flushing
                 * flag once it comes out from a wait on the signal to
                 * handle this scenario.
                 */
                break;

            case EVENT_FLUSH_STOP:
                if (PIPELINE_STATE(pipeline) == STATE_PAUSED)
                {
                    /* If the pipeline/element is in paused state, then block
                     * the pad again.
                     */
                }
                break;

            case EVENT_NEWSEGMENT:
                if (EVENT_FORMAT(event) == DATA_FORMAT_TIME)
                {
                    audio_sink_ptr->elapsed_time_usec  = 0;
                    audio_sink_ptr->elapsed_time_msec  = EVENT_DATA(event);
                    audio_sink_ptr->time_reported_msec = EVENT_DATA(event);
                    send_msg_element((StreamElement *)audio_sink_ptr, MSG_INFO_POSITION,
                                     audio_sink_ptr->time_reported_msec);
                    STREAMER_LOG_DEBUG(DBG_AUDIO_SINK, "[Audio sink]new segment to %d\n",
                                       audio_sink_ptr->elapsed_time_msec);
                }
                ret = true;
                break;

            default:
                break;
        }
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_SINK);
    return ret;
}

/**
 * @param element: audio sink element
 * @param new_state: audio sink state to be changed
 *
 * @brief This function changes the audio sink state and calls
 * the appropriate apis for that state and takes the actions and then returns.
 * PipelineState Transition:
 *      NULL to READY: Initialize audio device (now, playback only)
 *      READY to PAUSE: Activate sink pads and their peer pads
 *      PAUSE to PLAY: Start Playing
 *
 * @returns: int32_t
 *
 */
static int32_t audiosink_change_state(StreamElement *element, PipelineState new_state)
{
    PipelineStateChange state_change;
    ElementAudioSink *audio_sink_ptr = NULL;
    int32_t ret                      = STREAM_OK;
    AudioSinkStreamErrorType _ret    = AUDIO_SINK_SUCCESS;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SINK);

    audio_sink_ptr = (ElementAudioSink *)element;
    CHK_ARGS(NULL == audio_sink_ptr, STREAM_ERR_INVALID_ARGS);

    if (NULL == audio_sink_ptr)
    {
        STREAMER_FUNC_EXIT(DBG_AUDIO_SINK);
        return STREAM_ERR_ELEMENT_NOT_FOUND;
    }

    /* get the transition status with the new passed state and the existing
     * state of the sink element
     */
    state_change = (PipelineStateChange)STATE_TRANSITION(audio_sink_ptr->state, new_state);
    /* the pad state are NULL<->READY<->PAUSE<->PLAYING
     * From NULL->READY, the device data are allocated but the device file handler is null.
     * From READY->PAUSE, peer pad is active
     * From PAUSE->PLAYING, the device file handler is opened
     * From PLAYING->PAUSE, the device file handler is released. so that another streamer
     * task may open the device file.
     * From PAUSE->READY, peer pad is deactive
     * From READY->NULL, the device data are de-allocated.
     */

    switch (state_change)
    {
        case STATE_CHANGE_NULL_TO_READY:
            STREAMER_LOG_DEBUG(DBG_AUDIO_SINK, "[Audio sink]STATE_CHANGE_NULL_TO_READY\n");

            _ret = audio_sink_ptr->device_ptr->open_device_func(audio_sink_ptr);
            if (_ret != AUDIO_SINK_SUCCESS)
                ret = STREAM_ERR_GENERAL;
            break;

        case STATE_CHANGE_PAUSED_TO_PLAYING:
            STREAMER_LOG_DEBUG(DBG_AUDIO_SINK, "[Audio sink]STATE_CHANGE_PAUSED_TO_PLAYING\n");
            _ret = audio_sink_ptr->device_ptr->play_device_func(audio_sink_ptr);
            if (_ret != AUDIO_SINK_SUCCESS)
                ret = STREAM_ERR_GENERAL;
            break;

        case STATE_CHANGE_READY_TO_NULL:
            _ret = audio_sink_ptr->device_ptr->close_device_func(audio_sink_ptr);
            if (_ret != AUDIO_SINK_SUCCESS)
                ret = STREAM_ERR_GENERAL;
            STREAMER_LOG_DEBUG(DBG_AUDIO_SINK, "[Audio sink]STATE_CHANGE_READY_TO_NULL\n");
            break;

        case STATE_CHANGE_READY_TO_PAUSED:
            STREAMER_LOG_DEBUG(DBG_AUDIO_SINK, "[Audio sink]STATE_CHANGE_READY_TO_PAUSE\n");
            _ret                           = audio_sink_ptr->device_ptr->start_device_func(audio_sink_ptr);
            audio_sink_ptr->device_started = true;
            if (_ret != AUDIO_SINK_SUCCESS)
                ret = STREAM_ERR_GENERAL;
            break;

        case STATE_CHANGE_PLAYING_TO_PAUSED:
            STREAMER_LOG_DEBUG(DBG_AUDIO_SINK, "[Audio sink]STATE_CHANGE_PLAYING_TO_PAUSE\n");
            _ret = audio_sink_ptr->device_ptr->pause_device_func(audio_sink_ptr);
            if (_ret != AUDIO_SINK_SUCCESS)
                ret = STREAM_ERR_GENERAL;
            break;

        case STATE_CHANGE_PAUSED_TO_READY:
            STREAMER_LOG_DEBUG(DBG_AUDIO_SINK, "[Audio sink]STATE_CHANGE_PAUSED_TO_READY\n");
            if (audio_sink_ptr->device_started)
            {
                _ret                           = audio_sink_ptr->device_ptr->stop_device_func(audio_sink_ptr);
                audio_sink_ptr->device_started = false;
            }
            if (_ret != AUDIO_SINK_SUCCESS)
                ret = STREAM_ERR_GENERAL;
            break;

        default:
            break;
    }

    if (STREAM_OK == ret)
    {
        audio_sink_ptr->state = new_state;
    }
    else
    {
        audio_sink_ptr->state = STATE_NULL;
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_SINK);
    return ret;
}

/*!
 * @param pad: pad instance
 * @param query: input buffer
 *
 * @brief This pad function handles query from the pipeline. If
 * not able to handle the query then it pushes the query to its peer.
 *
 * @returns: true/false
 */

static uint8_t audiosink_sink_pad_query_handler(StreamPad *pad, StreamQuery *query)
{
    ElementAudioSink *element = (ElementAudioSink *)PAD_PARENT(pad);
    StreamData *data;
    StreamPad *peer;
    uint8_t ret = false;

    CHK_ARGS(NULL == pad || NULL == query || NULL == QUERY_DATA(query), false);
    data = QUERY_DATA(query);

    peer = PAD_PEER(pad);
    CHK_ARGS(NULL == peer, false);

    /* get the parent element from the pad */
    element = (ElementAudioSink *)PAD_PARENT(pad);
    CHK_ARGS(NULL == element, false);

    STREAMER_LOG_DEBUG(DBG_AUDIO_SINK, "audio_sink_sink_pad_query_handler: query[%d]\n", QUERY_TYPE(query));

    switch (QUERY_TYPE(query))
    {
        case INFO_POSITION:
            if (QUERY_FORMAT(query) == DATA_FORMAT_TIME)
            {
                data->value32u = element->elapsed_time_msec;
                STREAMER_LOG_DEBUG(DBG_AUDIO_SINK, "audio sink query position:%d\n", data->value32u);
                ret = true;
                break;
            }
        case INFO_BUFF_OCC:
            if (element->device_ptr != NULL)
            {
                ret = element->device_ptr->query_func(pad, query);
            }

            break;
        default:
            /* Relay all query downstream. Audio sink will only handle current
             * elapsed time.
             */
            ret = pad_query(peer, query);
            break;
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_SINK);
    return ret;
}

/*!
 *
 *
 * @brief This function gets audio sink element's properties
 *
 * @param element_ptr: audio sink element instance
 * @param prop:        audio sink element property
 * @param val_ptr:     value pointer
 * @returns AudioSinkStreamErrorType
 *
 * @retval AUDIO_SINK_SUCCESS
 * @retval AUDIO_SINK_ERROR_INVALID_ARGS
 * @retval AUDIO_SINK_ERROR_NO_RESOURCE
 *
 */
static int32_t audiosink_get_property(StreamElement *element_ptr, uint16_t prop, uint32_t *val_ptr)
{
    ElementAudioSink *audio_sink_ptr = NULL;
    int32_t ret                      = AUDIO_SINK_SUCCESS;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SINK);

    audio_sink_ptr = (ElementAudioSink *)element_ptr;
    if (NULL == val_ptr)
    {
        STREAMER_FUNC_EXIT(DBG_AUDIO_SINK);
        return AUDIO_SINK_ERROR_INVALID_ARGS;
    }

    switch (prop)
    {
        case PROP_AUDIOSINK_DEVICE_DRIVER_TYPE:
            *val_ptr = audio_sink_ptr->device_driver_type;
            break;

        case PROP_AUDIOSINK_TIME_UPDATE_MS:
            *val_ptr = audio_sink_ptr->time_report_diff;
            break;

        default:
            ret = AUDIO_SINK_ERROR_NO_RESOURCE;
            break;
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_SINK);
    return ret;
}

/*!
 *
 * @param element_ptr: audio sink element instance
 * @param prop:        audio sink element property
 * @param val:         value pointer
 *
 * @brief This function sets audio sink element's properties
 *
 * @returns AudioSinkStreamErrorType
 *
 * @retval AUDIO_SINK_SUCCESS
 * @retval AUDIO_SINK_ERROR_INVALID_ARGS
 * @retval AUDIO_SINK_ERROR_NO_RESOURCE
 *
 */
static int32_t audiosink_set_property(StreamElement *element_ptr, uint16_t prop, uint32_t val)
{
    ElementAudioSink *audio_sink_ptr = NULL;
    AudioSinkStreamErrorType _ret    = AUDIO_SINK_SUCCESS;
    int32_t ret                      = AUDIO_SINK_SUCCESS;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SINK);

    audio_sink_ptr = (ElementAudioSink *)element_ptr;
    STREAMER_LOG_DEBUG(DBG_AUDIO_SINK, "[Audio sink] audiosink_set_property: %d\n", prop);
    switch (prop)
    {
        case PROP_AUDIOSINK_DEVICE_DRIVER_TYPE:
            STREAMER_LOG_DEBUG(DBG_AUDIO_SINK, "[Audio sink] driver type: %d\n", val);
            ret =
                audiosink_set_device_driver_type((ElementHandle)(uintptr_t)element_ptr, (AudioSinkDeviceDriverType)val);
            if (AUDIO_SINK_SUCCESS == ret)
            {
                audio_sink_ptr->device_driver_type = val;
            }
            break;

        case PROP_AUDIOSINK_TIME_UPDATE_MS:
            audio_sink_ptr->time_report_diff = val;
            break;

        case PROP_AUDIOSINK_DEVICE_DRIVER_STRING_NAME:
            ret = audiosink_set_device_string_name((ElementHandle)(uintptr_t)element_ptr, (char *)(uintptr_t)val);
            break;

        case PROP_AUDIOSINK_BUFFER_USE_CHUNK_SIZE:
            audio_sink_ptr->buffer_use_chunk_size = (bool)val;
            break;

        case PROP_AUDIOSINK_SET_VOLUME:
            _ret = audio_sink_ptr->device_ptr->set_volume_func(audio_sink_ptr, (int)val);
            if (_ret != AUDIO_SINK_SUCCESS)
                ret = STREAM_ERR_GENERAL;
            break;
        case PROP_AUDIOSINK_SET_REFDATA_ELEMENT:
            audio_sink_ptr->refData_element = (ElementHandle)val;
            break;
        default:
            ret = AUDIO_SINK_ERROR_NO_RESOURCE;
            break;
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_SINK);
    return ret;
}

int32_t audiosink_init_element(StreamElement *element)
{
    AudioSinkStreamErrorType ret     = AUDIO_SINK_SUCCESS;
    uint32_t i                       = 0;
    ElementAudioSink *audio_sink_ptr = NULL;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SINK);

    audio_sink_ptr = (ElementAudioSink *)element;
    if (NULL == audio_sink_ptr)
    {
        ret = AUDIO_SINK_ERROR_INVALID_ARGS;
        goto ERR;
    }

    /* set element change state function */
    audio_sink_ptr->change_state = audiosink_change_state;
    audio_sink_ptr->set_property = audiosink_set_property;
    audio_sink_ptr->get_property = audiosink_get_property;

    /* initialize element fields */
    audio_sink_ptr->elapsed_time_msec  = 0;
    audio_sink_ptr->elapsed_time_usec  = 0;
    audio_sink_ptr->time_reported_msec = 0;
    audio_sink_ptr->time_report_diff   = 0;
    audio_sink_ptr->chunk_size         = 0;
    audio_sink_ptr->bits_per_sample    = 0;
    audio_sink_ptr->num_channels       = 0;
    audio_sink_ptr->sample_rate        = 0;
    audio_sink_ptr->format             = 0;
    audio_sink_ptr->device_ptr         = NULL;
    audio_sink_ptr->error_element      = AUDIO_SINK_SUCCESS;
    audio_sink_ptr->device_name        = AUDIO_SINK_DEVICE_DEFAULT;
    audio_sink_ptr->device_info        = NULL;
    audio_sink_ptr->device_started     = false;
    audio_sink_ptr->refData_element    = (ElementHandle)NULL;

    /* initialize only sink pads handlers, no src pads for audio sink */
    for (i = 0; i < NUM_SINKS(TYPE_ELEMENT_AUDIO_SINK); i++)
    {
        PadSink *pad            = (PadSink *)ELEMENT_SINK_PAD(audio_sink_ptr, i);
        pad->event_handler      = audiosink_sink_pad_event_handler;
        pad->activation_handler = audiosink_sink_pad_activation_handler;
        pad->chain_handler      = audiosink_sink_pad_chain_handler;
        pad->query_handler      = audiosink_sink_pad_query_handler;
        pad->process_precheck   = NULL;
        pad->process_handler    = audiosink_sink_pad_process_handler;
    }

ERR:
    STREAMER_FUNC_EXIT(DBG_AUDIO_SINK);
    return ret;
}

AudioSinkStreamErrorType audiosink_set_device_driver_type(ElementHandle element,
                                                          AudioSinkDeviceDriverType device_driver_type)
{
    ElementAudioSink *audio_sink_ptr = (ElementAudioSink *)(uintptr_t)element;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SINK);

    if ((NULL == audio_sink_ptr) || (device_driver_type >= LAST_AUDIO_SINK_DEVICE_DRIVER_TYPE))
    {
        STREAMER_FUNC_EXIT(DBG_AUDIO_SINK);
        return AUDIO_SINK_ERROR_INVALID_ARGS;
    }

    if (audio_sink_ptr->state != STATE_NULL)
    {
        STREAMER_FUNC_EXIT(DBG_AUDIO_SINK);
        return AUDIO_SINK_ERROR_BAD_STATE;
    }

    switch (device_driver_type)
    {
        case AUDIO_SINK_DEVICE_TYPE_PCMRTOS:
            audio_sink_ptr->device_ptr = &audio_sink_device_pcmrtos;
            break;

        default:
            audio_sink_ptr->device_ptr = NULL;
            STREAMER_FUNC_EXIT(DBG_AUDIO_SINK);
            return AUDIO_SINK_ERROR_NO_RESOURCE;
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_SINK);
    return AUDIO_SINK_SUCCESS;
}

AudioSinkStreamErrorType audiosink_set_device_name(ElementHandle element, AudioSinkDeviceName device_name)
{
    ElementAudioSink *audio_sink_ptr = NULL;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SINK);

    audio_sink_ptr = (ElementAudioSink *)(uintptr_t)element;
    if ((NULL == audio_sink_ptr) || (device_name >= AUDIO_SINK_DEVICE_MAX))
    {
        STREAMER_FUNC_EXIT(DBG_AUDIO_SINK);
        return AUDIO_SINK_ERROR_INVALID_ARGS;
    }

    audio_sink_ptr->device_name = device_name;

    STREAMER_FUNC_EXIT(DBG_AUDIO_SINK);
    return AUDIO_SINK_SUCCESS;
}

AudioSinkStreamErrorType audiosink_set_device_string_name(ElementHandle element, char *device_string_name)
{
    ElementAudioSink *audio_sink_ptr = NULL;

    STREAMER_FUNC_ENTER(DBG_AUDIO_SINK);

    audio_sink_ptr = (ElementAudioSink *)(uintptr_t)element;
    if ((NULL == audio_sink_ptr) || (NULL == device_string_name))
    {
        STREAMER_FUNC_EXIT(DBG_AUDIO_SINK);
        return AUDIO_SINK_ERROR_INVALID_ARGS;
    }

    strncpy(audio_sink_ptr->device_string_name, device_string_name, MAX_DEVICE_NAME_LENGTH - 1);
    audio_sink_ptr->device_string_name[MAX_DEVICE_NAME_LENGTH - 1] = '\0';

    STREAMER_FUNC_EXIT(DBG_AUDIO_SINK);
    return AUDIO_SINK_SUCCESS;
}
