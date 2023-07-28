/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    audio_proc.c
 * @brief  This file contains the implementation for the AudioProc integration
 */

#include <string.h>

#include "audio_proc.h"
#include "pipeline.h"

/*!
 * @param pad: pad handle
 * @param active: boolean activate/disactivate pad
 *
 * @brief This function activates the pad and allocates
 * memory for the AudioProc element.
 *
 * @returns true/false
 * @retval   true success
 * @retval   false failed
 */
static uint8_t audio_proc_sink_pad_activation_handler(StreamPad *pad, uint8_t active)
{
    uint8_t ret = true;
    ElementAudioProc *audio_proc_element;
    STREAMER_FUNC_ENTER(DBG_AUDIO_PROC);

    CHK_ARGS(NULL == pad, false);

    /* get the parent element from the pad */
    audio_proc_element = (ElementAudioProc *)PAD_PARENT(pad);

    STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "[AudioProc]Activate pad: %d\n", active);

    if (true == ret)
    {
        /* Activate/Deactivate peer source pads in PUSH mode */
        audio_proc_element->current_index = 0;
        ret                               = pad_activate_push(pad->peer, active);
    }

    if (ret != true)
    {
        STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "[AudioProc SINK]Activetion pad returns: %d\n", ret);
    }
    STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
    return ret;
}

/*!
 * @param pad: pad handle
 * @param buf: input buffer
 *
 * @brief This pad function takes data from the source element and
 * pushes it onto the AUDIO_PROC library.
 *
 * @returns FlowReturn
 * @retval FLOW_OK success
 * @retval FLOW_UNEXPECTED bad paramesters
 * @retval FLOW_ERROR write failed
 */

static FlowReturn audio_proc_sink_pad_chain_handler(StreamPad *pad, StreamBuffer *buf)
{
    ElementAudioProc *audio_proc_element = NULL;
    FlowReturn ret                       = FLOW_OK;
    uint32_t pkt_hdr_size                = 0, raw_data_size;
    int8_t *data_ptr;
    // char dummy_arg;
    int audio_proc_ret;
    AudioPacketHeader *data_packet = NULL;
    uint32_t sample_rate;
    uint32_t channels;
    uint32_t chunk_size;
    ext_proc_args *args;
    StreamPad *src_pad = NULL;

    STREAMER_FUNC_ENTER(DBG_AUDIO_PROC);

    CHK_ARGS((NULL == pad) || (NULL == buf), FLOW_UNEXPECTED);

    /* get the parent element from the pad */
    audio_proc_element = (ElementAudioProc *)PAD_PARENT(pad);
    src_pad            = ELEMENT_SRC_PAD(audio_proc_element, 0);

    CHK_ARGS((((RawPacketHeader *)(buf->buffer))->id != AUDIO_DATA), FLOW_NOT_SUPPORTED);

    /* check what data packet header the buffer has and typecast accordingly to
     * store the appropriate data
     */
    pkt_hdr_size = sizeof(AudioPacketHeader);
    data_packet  = (AudioPacketHeader *)BUFFER_PTR(buf);
    chunk_size   = AUDIO_CHUNK_SIZE(data_packet);
    sample_rate  = AUDIO_SAMPLE_RATE(data_packet);
    channels     = AUDIO_NUM_CHANNELS(data_packet);

    /* buf->size is the total buffer size including the packet header
     * Get the raw data size considering the offset as offset into raw data
     * from which data has to be written.
     */
    raw_data_size = buf->size - pkt_hdr_size;

    STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "\traw_data_size = %d\n", raw_data_size);
    STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "\tbuf->offset = %d\n", buf->offset);
    STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "\tpkt_hdr_size = %d\n", pkt_hdr_size);

    data_ptr = (buf->buffer + pkt_hdr_size);
    STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "\tdata_ptr = %X\n", (uint32_t)data_ptr);
    STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "\tcurrent_index = %d\n", audio_proc_element->current_index);

    /* Need to call init here too
     * because of function pointers, cant call it in element init part
     */

    if (audio_proc_element->initialized == false)
    {
        // check we have non NULL init function pointer
        if (audio_proc_element->init_func == NULL)
        {
            STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "[AudioProc]init function pointer not initialized!");
            return FLOW_BAD_PARAMETER;
        }
        // check this is correctly writen
        args = audio_proc_element->arg_ptr;
        if (args != NULL)
        {
            if (sample_rate)
            {
                args->sample_rate = sample_rate;
            }
            if (channels)
            {
                args->num_channels = channels;
            }
        }

        audio_proc_ret = audio_proc_element->init_func(args);
        if (audio_proc_ret != 0)
        {
            STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "[AudioProc]AUDIO_PROC init error: %d\n", audio_proc_ret);
            return FLOW_BAD_PARAMETER;
        }
        else
        {
            audio_proc_element->initialized = true;
        }
    }

    /* Call the process function pointer */
    if (audio_proc_element->proc_func == NULL)
    {
        STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "[AudioProc]Process function pointer not initialized!");
        return FLOW_ERROR;
    }

    audio_proc_ret = audio_proc_element->proc_func((void *)&pkt_hdr_size, (void *)buf, chunk_size);
    if (audio_proc_ret != 0)
    {
        STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "[AudioProc]AUDIO_PROC Execute: %d\n", audio_proc_ret);
        ret = FLOW_ERROR;
        goto ERR;
    }

    // try to push it to next pad
    // put buffer in proper packet type
    ret = pad_push(src_pad, buf);

ERR:
    if (FLOW_OK != ret)
    {
        /* Post EOS message to the pipeline message queue here */
        send_msg_element(PAD_PARENT_ELEMENT(pad), MSG_EOS, 0);
        STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "[AudioProc]AUDIO_PROC sink pad flow: %d\n", ret);
    }
    STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
    return ret;
}

/*!
 * @param pad: pad handle
 * @param event: event enumeration
 *
 * @brief This function processes the pad events
 *
 * @returns true/false
 * @retval true success
 * @retval false failed
 */

static uint8_t audio_proc_sink_pad_event_handler(StreamPad *pad, StreamEvent *event)
{
    uint8_t ret               = true;
    ElementAudioProc *element = NULL;
    StreamPad *peer           = NULL;

    STREAMER_FUNC_ENTER(DBG_AUDIO_PROC);

    CHK_ARGS(NULL == pad, false);

    /* get the parent element from the pad */
    element = PAD_PARENT(pad);
    CHK_ARGS(NULL == element, false);

    peer = ELEMENT_SRC_PAD(element, 0);
    CHK_ARGS(NULL == peer, false);

    peer = PAD_PEER(peer);
    CHK_ARGS(NULL == peer, false);

    /* Device driver type */

    STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "Event [AUDIO_PROC]: %d\n", EVENT_TYPE(event) >> 4);

    if (true == ret)
    {
        /* handle the event */
        switch (EVENT_TYPE(event))
        {
            case EVENT_EOS:
                /* Post EOS message to the pipeline message queue here */
                send_msg_element(PAD_PARENT_ELEMENT(pad), MSG_EOS, 0);
                break;

            case EVENT_FLUSH_START:
                /* In case the current state is paused then unblock the pad
                 * signal. Chain function should also check the flushing
                 * flag once it comes out from a wait on the signal to
                 * handle this scenario.
                 */
                break;
            case EVENT_FLUSH_STOP:
                break;
            case EVENT_NEWSEGMENT:
                ret = pad_send_event(peer, event);
                break;
            default:
                ret = pad_send_event(pad, event);
                break;
        }
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
    return ret;
}

/*!
 * @param element_ptr: element
 * @param new_state: audio_proc state to be changed
 *
 * @brief This function changes the AUDIO_PROC state and calls
 * the appropriate apis for that state and takes the actions and then returns.
 * PipelineState Transition:
 *      NULL to READY: Nothing to do
 *      READY to PAUSE: Activate sink pads and their peer pads
 *      PAUSE to PLAY: Start Writing
 *      PAUSE to READY: Deactivate sink pads and their peer pads
 *      READY to NULL: Nothing to do
 *
 * @returns StreamReturnType
 * @retval STREAM_OK success
 * @retval STREAM_ERR_INVALID_ARGS invalid arguments
 *
 */
static int32_t audio_proc_change_state(StreamElement *element_ptr, PipelineState new_state)
{
    PipelineStateChange state_change;
    ElementAudioProc *audio_proc_ptr = (ElementAudioProc *)element_ptr;
    int32_t ret                      = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_AUDIO_PROC);

    CHK_ARGS(NULL == audio_proc_ptr, STREAM_ERR_INVALID_ARGS);

    /* get the transition status with the new passed state and the existing
     * state of the element
     */
    state_change = (PipelineStateChange)STATE_TRANSITION(audio_proc_ptr->state, new_state);

    audio_proc_ptr->state = new_state;
    switch (state_change)
    {
        case STATE_CHANGE_NULL_TO_READY:
            STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "[AUDIO_PROC]STATE_CHANGE_NULL_TO_READY\n");
            /* Nothing to do here for a AUDIO_PROC */
            break;

        case STATE_CHANGE_PAUSED_TO_PLAYING:
            STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "[AUDIO_PROC]STATE_CHANGE_PAUSED_TO_PLAYING\n");
            break;

        case STATE_CHANGE_READY_TO_NULL:
            STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "[AUDIO_PROC]STATE_CHANGE_READY_TO_NULL\n");
            if((audio_proc_ptr->initialized == true) && (audio_proc_ptr->deinit_func != NULL))
            {
                audio_proc_ptr->deinit_func();
            }

            /* Nothing to do here for a AUDIO_PROC */
            break;

            /* When going to pause state then set the pad to blocked state so
             * that data flow is blocked.
             */
        case STATE_CHANGE_READY_TO_PAUSED:
        case STATE_CHANGE_PLAYING_TO_PAUSED:
            STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "[AUDIO_PROC]STATE_CHANGE_XXX_TO_PAUSE\n");
            break;

            /* deactivate pads */
        case STATE_CHANGE_PAUSED_TO_READY:
            STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "[AUDIO_PROC]STATE_CHANGE_PAUSED_TO_READY\n");
            break;

        default:
            break;
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
    return ret;
}

/*!
 * @param pad: pad handle
 * @param query: input buffer
 *
 * @brief This pad function handles query from the pipeline. If
 * not able to handle the query then it pushes the query to its peer.
 *
 * @returns: true/false
 */
static uint8_t audio_proc_sink_pad_query_handler(StreamPad *pad, StreamQuery *query)
{
    ElementAudioProc *element;
    StreamPad *peer;
    uint8_t ret = false;

    STREAMER_FUNC_ENTER(DBG_AUDIO_PROC);

    CHK_ARGS(NULL == pad || NULL == query || NULL == QUERY_DATA(query), false);

    peer = PAD_PEER(pad);
    CHK_ARGS(NULL == peer, false);

    /* get the parent element from the pad */
    element = (ElementAudioProc *)PAD_PARENT(pad);
    CHK_ARGS(NULL == element, false);

    STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "audio_proc_sink_pad_query_handler: query[%d]\n", QUERY_TYPE(query));

    switch (QUERY_TYPE(query))
    {
        default:
            /* Relay all query downstream.
             */
            ret = pad_query(peer, query);
            break;
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
    return ret;
}

/*!
 * @brief        audio_proc_src_pad_query_handler
 *
 * @details      This pad function handles query from the pipeline. If
 *               not able to handle the query then it pushes the query to its peer.
 *
 * @param pad:   pad handle
 * @param query: input buffer
 *
 * @returns:     true/false
 */
static uint8_t audio_proc_src_pad_query_handler(StreamPad *pad, StreamQuery *query)
{
    uint8_t ret = false;
    ElementAudioProc *element;

    STREAMER_FUNC_ENTER(DBG_AUDIO_PROC);

    CHK_ARGS(NULL == pad || NULL == query || NULL == QUERY_DATA(query), false);

    /* get the parent element from the pad */
    element = (ElementAudioProc *)PAD_PARENT(pad);
    CHK_ARGS(NULL == element, false);

    STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "[AUDIO_PROC]audio_proc_src_pad_query_handler: query[%d]\n", QUERY_TYPE(query));

    switch (QUERY_TYPE(query))
    {
        default:
        {
            /* Relay all query downstream. */
            StreamPad *sink = ELEMENT_SINK_PAD(element, 0);
            StreamPad *peer = PAD_PEER(sink);

            if (peer)
                ret = pad_query(peer, query);
        }
        break;
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
    return ret;
}

/*!
 * @brief        audio_proc_src_pad_event_handler
 *
 * @details      This pad function handles events from the pipeline. If
 *               not able to handle the event then it pushes the event to its peer.
 *
 * @param pad:   pad handle
 * @param event: input buffer
 *
 * @returns:     true/false
 */
static uint8_t audio_proc_src_pad_event_handler(StreamPad *pad, StreamEvent *event)
{
    uint8_t ret = false;
    ElementAudioProc *element;

    STREAMER_FUNC_ENTER(DBG_AUDIO_PROC);

    CHK_ARGS(NULL == pad || NULL == event, false);

    /* get the parent element from the pad */
    element = PAD_PARENT(pad);
    CHK_ARGS(NULL == element, false);

    STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "[AUDIO_PROC]audio_proc_src_pad_event_handler: event[%d]\n", EVENT_TYPE(event));

    /* handle the event */
    switch (EVENT_TYPE(event))
    {
        case EVENT_SEEK:
            /* Push the event out from the source pad */
            ret = pad_push_event(ELEMENT_SINK_PAD(element, 0), event);
            break;
        default:
            break;
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
    return ret;
}

/**
 * @brief Empty AUDIO_PROC src pull handler function
 *
 * @param pad    Pad pointer
 * @param buffer Buffer pointer
 * @param size   Size
 * @param offset Offset
 * @return FlowReturn
 */
static FlowReturn audio_proc_src_pull_handler(StreamPad *pad, StreamBuffer *buffer, uint32_t size, uint32_t offset)
{
    STREAMER_LOG_DEBUG(DBG_AUDIO_PROC, "[AUDIO_PROC] audio_proc_src_pull_handler request\n");
    return FLOW_OK;
}

/*!
 * @param element_ptr: AUDIO_PROC element instance
 * @param prop:        audio_proc element property
 * @param val:         value pointer
 *
 * @brief This function sets AUDIO_PROC element's properties
 *
 * @returns StreamReturnType
 *
 * @retval STREAM_OK success
 * @retval STREAM_ERR_INFO_ABSENT unknown property
 * @retval STREAM_GENERAL  bad status to set the property
 *
 */
static int32_t audio_proc_set_property(StreamElement *element_ptr, uint16_t prop, uint32_t val)
{
    int32_t ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_AUDIO_PROC);

    switch (prop)
    {
        case PROP_VOICESEEKER_PROC_FUNCPTR:
        case PROP_EAP_PROC_FUNCPTR:
        case PROP_SRC_PROC_FUNCPTR:
        {
            EXT_PROCESS_DESC_T *desc_ptr = (EXT_PROCESS_DESC_T *)val;
            ret = audio_proc_register_ext_processing((ElementHandle)element_ptr, desc_ptr->init_func,
                                                     desc_ptr->proc_func, desc_ptr->deinit_func, desc_ptr->arg_ptr);
        }
        break;
        case PROP_VITSINK_PROC_FUNCPTR:
        {
            EXT_PROCESS_DESC_T *desc_ptr = (EXT_PROCESS_DESC_T *)val;
            ret = audio_proc_register_ext_processing((ElementHandle)element_ptr, desc_ptr->init_func,
                                                     desc_ptr->proc_func, desc_ptr->deinit_func, desc_ptr->arg_ptr);
        }
        break;
        case PROP_VOICESEEKER_PROC_REFDATA_FUNCPTR:
        {
            EXT_PROCESS_REFDAT_DESC_T *desc_ptr = (EXT_PROCESS_REFDAT_DESC_T *)val;
            ret = audio_proc_register_refdata_processing((ElementHandle)element_ptr, desc_ptr->set_num_buff_func,
                                                         desc_ptr->push_func, desc_ptr->set_debugging);
        }
        break;
        case PROP_VOICESEEKER_PROC_REFDATA_NUM_BUFFERS:
            ret = audio_proc_refdata_set_num_buff((ElementHandle)element_ptr, val);
            break;
        case PROP_VOICESEEKER_PROC_REFDATA_PUSH:
            ret = audio_proc_refdata_push((ElementHandle)element_ptr, (AudioRefData_t *)val);
            break;
        case PROP_VOICESEEKER_PROC_SET_DEBUGGING:
            ret = audio_proc_set_debugging((ElementHandle)element_ptr, (bool)val);
            break;
        default:
            ret = STREAM_ERR_INFO_ABSENT;
            break;
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
    return ret;
}

/*!
 *
 * @param element_ptr: AUDIO_PROC element instance
 * @param prop:        audio_proc element property
 * @param val_ptr:     value pointer
 *
 * @brief This function gets AUDIO_PROC element's properties
 *
 * @returns AudioSinkStreamErrorType
 *
 * @retval STREAM_OK success
 * @retval STREAM_ERR_INVALID_ARGS bad arguments
 * @retval STREAM_ERR_INFO_ABSENT unknown property
 *
 */
static int32_t audio_proc_get_property(StreamElement *element_ptr, uint16_t prop, uint32_t *val_ptr)
{
    int32_t ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_AUDIO_PROC);

    if (NULL == val_ptr)
    {
        STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
        return STREAM_ERR_INVALID_ARGS;
    }

    switch (prop)
    {
        default:
            ret = STREAM_ERR_INFO_ABSENT;
            break;
    }

    STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
    return ret;
}

int32_t audio_proc_init_element(StreamElement *element_ptr)
{
    int32_t ret                      = STREAM_OK;
    uint32_t i                       = 0;
    ElementAudioProc *audio_proc_ptr = (ElementAudioProc *)element_ptr;

    STREAMER_FUNC_ENTER(DBG_AUDIO_PROC);

    if (NULL == audio_proc_ptr)
    {
        ret = STREAM_ERR_ELEMENT_NOT_FOUND;
        goto ERR;
    }

    /* set element change state function */
    audio_proc_ptr->change_state = audio_proc_change_state;
    audio_proc_ptr->set_property = audio_proc_set_property;
    audio_proc_ptr->get_property = audio_proc_get_property;

    /* initialize element fields */
    audio_proc_ptr->location      = NULL;
    audio_proc_ptr->size          = 0;
    audio_proc_ptr->current_index = 0;
    audio_proc_ptr->initialized   = false;

    /* initialize sink pads handlers */
    for (i = 0; i < NUM_SINKS(TYPE_ELEMENT_AUDIO_PROC); i++)
    {
        PadSink *pad            = (PadSink *)ELEMENT_SINK_PAD(audio_proc_ptr, i);
        pad->event_handler      = audio_proc_sink_pad_event_handler;
        pad->activation_handler = audio_proc_sink_pad_activation_handler;
        pad->chain_handler      = audio_proc_sink_pad_chain_handler;
        pad->query_handler      = audio_proc_sink_pad_query_handler;
        pad->process_precheck   = NULL;
        pad->process_handler    = NULL;
    }

    /* initialize src pads handlers */
    for (i = 0; i < NUM_SRCS(TYPE_ELEMENT_AUDIO_PROC); i++)
    {
        PadSrc *pad;
        pad = (PadSrc *)ELEMENT_SRC_PAD(audio_proc_ptr, i);

        pad->query_handler      = audio_proc_src_pad_query_handler;
        pad->event_handler      = audio_proc_src_pad_event_handler;
        pad->activate_push      = NULL;
        pad->activation_handler = NULL;
        pad->pull_handler       = audio_proc_src_pull_handler;
        pad->process_handler    = NULL;
        pad->process_precheck   = NULL;
    }

    // set function pointers to NULL
    audio_proc_ptr->init_func   = NULL;
    audio_proc_ptr->proc_func   = NULL;
    audio_proc_ptr->deinit_func = NULL;

ERR:
    STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
    return ret;
}

int32_t audio_proc_set_buffer(ElementHandle element_hdl, char *location_ptr, uint32_t size)
{
    ElementAudioProc *audio_proc_ptr = (ElementAudioProc *)element_hdl;
    StreamReturnType ret             = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_AUDIO_PROC);

    CHK_ARGS(audio_proc_ptr == NULL || location_ptr == NULL || size == 0, STREAM_ERR_INVALID_ARGS);

    if (audio_proc_ptr->state >= STATE_PAUSED)
    {
        STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
        return STREAM_ERR_GENERAL;
    }

    audio_proc_ptr->location = location_ptr;
    audio_proc_ptr->size     = size;

    STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
    return ret;
}

int32_t audio_proc_register_ext_processing(ElementHandle element,
                                           AudioProcInitFunc init_func_ptr,
                                           AudioProcPostProcFunc proc_func_ptr,
                                           AudioProcDeinitFunc deinit_func_ptr,
                                           void *arg_ptr)
{
    int32_t ret = STREAM_OK;

    ElementAudioProc *audio_proc_ptr = (ElementAudioProc *)element;

    STREAMER_FUNC_ENTER(DBG_AUDIO_PROC);

    if ((NULL == audio_proc_ptr) || (NULL == init_func_ptr) || (NULL == proc_func_ptr) || (NULL == deinit_func_ptr))
    {
        STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
        return STREAM_ERR_INVALID_ARGS;
    }

    audio_proc_ptr->init_func   = init_func_ptr;
    audio_proc_ptr->proc_func   = proc_func_ptr;
    audio_proc_ptr->deinit_func = deinit_func_ptr;
    audio_proc_ptr->arg_ptr     = arg_ptr;

    STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
    return ret;
}

int32_t audio_proc_register_refdata_processing(ElementHandle element,
                                               AudioProcRefDataSetNumBuffFunc set_num_buff,
                                               AudioProcRefDataPushFunc refdata_push,
                                               AudioProcSetDebuggingFunc set_debugging)
{
    ElementAudioProc *audio_proc_ptr = (ElementAudioProc *)element;

    STREAMER_FUNC_ENTER(DBG_AUDIO_PROC);

    if ((NULL == audio_proc_ptr) || (NULL == set_num_buff) || (NULL == refdata_push))
    {
        STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
        return STREAM_ERR_INVALID_ARGS;
    }

    audio_proc_ptr->refdata_set_num_buff_func = set_num_buff;
    audio_proc_ptr->refdata_push_func         = refdata_push;
    audio_proc_ptr->set_debugging_func        = set_debugging;

    STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
    return STREAM_OK;
}

int32_t audio_proc_refdata_push(ElementHandle element, AudioRefData_t *ref_data)
{
    int32_t ret = STREAM_OK;

    ElementAudioProc *audio_proc_ptr = (ElementAudioProc *)element;
    STREAMER_FUNC_ENTER(DBG_AUDIO_PROC);

    if ((NULL == audio_proc_ptr) || (NULL == ref_data))
    {
        STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
        return STREAM_ERR_INVALID_ARGS;
    }

    ret = audio_proc_ptr->refdata_push_func(ref_data);

    STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
    return ret;
}

int32_t audio_proc_refdata_set_num_buff(ElementHandle element, uint32_t num_buff)
{
    int32_t ret = STREAM_OK;

    ElementAudioProc *audio_proc_ptr = (ElementAudioProc *)element;
    STREAMER_FUNC_ENTER(DBG_AUDIO_PROC);

    if (NULL == audio_proc_ptr)
    {
        STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
        return STREAM_ERR_INVALID_ARGS;
    }

    ret = audio_proc_ptr->refdata_set_num_buff_func((int)num_buff);

    STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
    return ret;
}

int32_t audio_proc_set_debugging(ElementHandle element, bool set_debugging)
{
    int32_t ret = STREAM_OK;

    ElementAudioProc *audio_proc_ptr = (ElementAudioProc *)element;
    STREAMER_FUNC_ENTER(DBG_AUDIO_PROC);

    if (NULL == audio_proc_ptr)
    {
        STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
        return STREAM_ERR_INVALID_ARGS;
    }

    if (audio_proc_ptr->set_debugging_func == NULL)
    {
        STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
        return -1;
    }

    ret = audio_proc_ptr->set_debugging_func(set_debugging);

    STREAMER_FUNC_EXIT(DBG_AUDIO_PROC);
    return ret;
}
