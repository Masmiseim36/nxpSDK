/*
 * Copyright 2018-2022 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*!
 * @file    process_sink.c
 * @brief  This file contains the implementation for the Process Sink
 */

#include <string.h>

#include "process_sink.h"
#include "pipeline.h"
#include "streamer_element_properties.h"

/*!
 * @param pad: pad handle
 * @param active: boolean activate/disactivate pad
 *
 * @brief This function activates the sink pad and allocates
 * memory for the process sink element.
 *
 * @returns true/false
 * @retval   true success
 * @retval   false failed
 */
static uint8_t processsink_sink_pad_activation_handler(StreamPad *pad, uint8_t active)
{
    uint8_t ret = true;
    ElementProcessSink *process_sink_element;
    STREAMER_FUNC_ENTER(DBG_PROCESS_SINK);

    CHK_ARGS(NULL == pad, false);

    /* get the parent element from the pad */
    process_sink_element = (ElementProcessSink *)PAD_PARENT(pad);

    STREAMER_LOG_DEBUG(DBG_PROCESS_SINK, "[Process SINK]Activate pad: %d\n", active);

    if (true == ret)
    {
        /* Activate/Deactivate peer source pads in PUSH mode */
        process_sink_element->current_index = 0;
        ret                             = pad_activate_push(pad->peer, active);
    }

    if (ret != true)
    {
        STREAMER_LOG_DEBUG(DBG_PROCESS_SINK, "[Process SINK]Activetion pad returns: %d\n", ret);
    }
    STREAMER_FUNC_EXIT(DBG_PROCESS_SINK);
    return ret;
}

/*!
 * @param pad: pad handle
 * @param buf: input buffer
 *
 * @brief This pad function takes data from the source element and
 * pushes it onto the PROCESS library.
 * The PROCESS sink requires currently precise data buffer size.
 *
 * @returns FlowReturn
 * @retval FLOW_OK success
 * @retval FLOW_UNEXPECTED bad paramesters
 * @retval FLOW_ERROR write failed
 */

static FlowReturn processsink_sink_pad_chain_handler(StreamPad *pad, StreamBuffer *buf)
{
    ElementProcessSink *process_sink_element = NULL;
    FlowReturn ret                   = FLOW_OK;
    uint32_t pkt_hdr_size            = 0, raw_data_size;
    int8_t *data_ptr;
    int dummy_arg = 0;
    int process_ret;
    AudioPacketHeader *data_packet = NULL;
    uint32_t chunk_size;

    STREAMER_FUNC_ENTER(DBG_PROCESS_SINK);

    CHK_ARGS((NULL == pad) || (NULL == buf), FLOW_UNEXPECTED);

    /* get the parent element from the pad */
    process_sink_element = (ElementProcessSink *)PAD_PARENT(pad);

    CHK_ARGS((((RawPacketHeader *)(buf->buffer))->id != AUDIO_DATA), FLOW_NOT_SUPPORTED);

    /* check what data packet header the buffer has and typecast accordingly to
     * store the appropriate data
     */
    pkt_hdr_size = sizeof(AudioPacketHeader);
    data_packet  = (AudioPacketHeader *)BUFFER_PTR(buf);
    chunk_size   = AUDIO_CHUNK_SIZE(data_packet);

    /* buf->size is the total buffer size including the packet header
     * Get the raw data size considering the offset as offset into raw data
     * from which data has to be written.
     */
    raw_data_size = buf->size - pkt_hdr_size;

    STREAMER_LOG_DEBUG(DBG_PROCESS_SINK, "\traw_data_size = %d\n", raw_data_size);
    STREAMER_LOG_DEBUG(DBG_PROCESS_SINK, "\tbuf->offset = %d\n", buf->offset);
    STREAMER_LOG_DEBUG(DBG_PROCESS_SINK, "\tpkt_hdr_size = %d\n", pkt_hdr_size);

    data_ptr = (buf->buffer + pkt_hdr_size);
    STREAMER_LOG_DEBUG(DBG_PROCESS_SINK, "\tdata_ptr = %X\n", (uint32_t)data_ptr);
    STREAMER_LOG_DEBUG(DBG_PROCESS_SINK, "\tcurrent_index = %d\n", process_sink_element->current_index);

    /* Need to call init here too
     * because of function pointers, cant call it in element init part
     */

    if (process_sink_element->initialized == false)
    {
        if (process_sink_element->init_func == NULL)
        {
            STREAMER_LOG_DEBUG(DBG_PROCESS_SINK, "[Process SINK]External PROCESS init function is not registered");
            return false;
        }
        process_ret = process_sink_element->init_func(&dummy_arg);
        if (process_ret != 0)
        {
            STREAMER_LOG_DEBUG(DBG_PROCESS_SINK, "[Process SINK]PROCESS init error: %d\n", process_ret);
            return false;
        }
        else
        {
            process_sink_element->initialized = true;
        }
    }

    /* Call the process function pointer */
    if (process_sink_element->proc_func == NULL)
    {
        STREAMER_LOG_DEBUG(DBG_PROCESS_SINK, "[Process SINK]External PROCESS process function is not registered");
        return FLOW_ERROR;
    }
    process_ret = process_sink_element->proc_func((void *)&pkt_hdr_size, (void *)buf, chunk_size);
    if (process_ret != 0)
    {
        STREAMER_LOG_DEBUG(DBG_PROCESS_SINK, "[Process SINK]PROCESS Execute: %d\n", process_ret);
        ret = FLOW_ERROR;
    }

    if (FLOW_OK != ret)
    {
        /* Post EOS message to the pipeline message queue here */
        send_msg_element(PAD_PARENT_ELEMENT(pad), MSG_EOS, 0);
        STREAMER_LOG_DEBUG(DBG_PROCESS_SINK, "[Process SINK]PROCESS sink pad flow: %d\n", ret);
    }
    STREAMER_FUNC_EXIT(DBG_PROCESS_SINK);
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

static uint8_t processsink_sink_pad_event_handler(StreamPad *pad, StreamEvent *event)
{
    uint8_t ret = true;

    STREAMER_FUNC_ENTER(DBG_PROCESS_SINK);

    CHK_ARGS(NULL == pad, false);

    /* Device driver type */

    STREAMER_LOG_DEBUG(DBG_PROCESS_SINK, "Event [PROCESS SINK]: %d\n", EVENT_TYPE(event) >> 4);

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
                break;
            default:
                ret = pad_send_event(pad, event);
                break;
        }
    }

    STREAMER_FUNC_EXIT(DBG_PROCESS_SINK);
    return ret;
}

/*!
 * @param element_ptr: element
 * @param new_state: process sink state to be changed
 *
 * @brief This function changes the PROCESS sink state and calls
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
static int32_t processsink_change_state(StreamElement *element_ptr, PipelineState new_state)
{
    PipelineStateChange state_change;
    ElementProcessSink *process_sink_ptr = (ElementProcessSink *)element_ptr;
    int32_t ret                  = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_PROCESS_SINK);

    CHK_ARGS(NULL == process_sink_ptr, STREAM_ERR_INVALID_ARGS);

    /* get the transition status with the new passed state and the existing
     * state of the sink element
     */
    state_change = (PipelineStateChange)STATE_TRANSITION(process_sink_ptr->state, new_state);

    process_sink_ptr->state = new_state;
    switch (state_change)
    {
        case STATE_CHANGE_NULL_TO_READY:
            STREAMER_LOG_DEBUG(DBG_PROCESS_SINK, "[PROCESS SINK]STATE_CHANGE_NULL_TO_READY\n");
            /* Nothing to do here for a PROCESS sink */
            break;

        case STATE_CHANGE_PAUSED_TO_PLAYING:
            STREAMER_LOG_DEBUG(DBG_PROCESS_SINK, "[PROCESS SINK]STATE_CHANGE_PAUSED_TO_PLAYING\n");
            break;

        case STATE_CHANGE_READY_TO_NULL:
            STREAMER_LOG_DEBUG(DBG_PROCESS_SINK, "[PROCESS SINK]STATE_CHANGE_READY_TO_NULL\n");
            if ((process_sink_ptr->initialized == true) && (process_sink_ptr->deinit_func != NULL))
            {
                process_sink_ptr->deinit_func();
            }
            /* Nothing to do here for a PROCESS sink */
            break;

            /* When going to pause state then set the pad to blocked state so
             * that data flow is blocked.
             */
        case STATE_CHANGE_READY_TO_PAUSED:
        case STATE_CHANGE_PLAYING_TO_PAUSED:
            STREAMER_LOG_DEBUG(DBG_PROCESS_SINK, "[PROCESS SINK]STATE_CHANGE_XXX_TO_PAUSE\n");
            break;

            /* deactivate pads */
        case STATE_CHANGE_PAUSED_TO_READY:
            STREAMER_LOG_DEBUG(DBG_PROCESS_SINK, "[PROCESS SINK]STATE_CHANGE_PAUSED_TO_READY\n");
            break;

        default:
            break;
    }

    STREAMER_FUNC_EXIT(DBG_PROCESS_SINK);
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
static uint8_t processsink_sink_pad_query_handler(StreamPad *pad, StreamQuery *query)
{
    ElementProcessSink *element;
    StreamPad *peer;
    uint8_t ret = false;

    STREAMER_FUNC_ENTER(DBG_PROCESS_SINK);

    CHK_ARGS(NULL == pad || NULL == query || NULL == QUERY_DATA(query), false);

    peer = PAD_PEER(pad);
    CHK_ARGS(NULL == peer, false);

    /* get the parent element from the pad */
    element = (ElementProcessSink *)PAD_PARENT(pad);
    CHK_ARGS(NULL == element, false);

    STREAMER_LOG_DEBUG(DBG_PROCESS_SINK, "process_sink_sink_pad_query_handler: query[%d]\n", QUERY_TYPE(query));

    switch (QUERY_TYPE(query))
    {
        default:
            /* Relay all query downstream.
             */
            ret = pad_query(peer, query);
            break;
    }

    STREAMER_FUNC_EXIT(DBG_PROCESS_SINK);
    return ret;
}

/*!
 * @param element_ptr: PROCESS sink element instance
 * @param prop: properties
 * @param val: parameters
 *
 * @brief This function sets PROCESS sink element's properties
 *
 * @returns StreamReturnType
 *
 * @retval STREAM_OK success
 * @retval STREAM_ERR_INFO_ABSENT unknown property
 * @retval STREAM_GENERAL  bad status to set the property
 *
 */
static int32_t processsink_set_property(StreamElement *element_ptr, uint16_t prop, uint32_t val)
{
    int32_t ret = STREAM_OK;

    // SET_BUFFER_DESC_T *buffer_desc_ptr = NULL;

    STREAMER_FUNC_ENTER(DBG_PROCESS_SINK);

    switch (prop)
    {
        case PROP_VITSINK_FUNCPTR:
        {
            EXT_PROCESS_DESC_T *desc_ptr = (EXT_PROCESS_DESC_T *)val;
            ret                          = process_register_ext_processing((ElementHandle)element_ptr, desc_ptr->init_func,
                                                                       (ProcessSinkPostProcFunc)desc_ptr->proc_func, desc_ptr->deinit_func,
                                                                       desc_ptr->arg_ptr);
        }
        break;
            /*
                    case PROP_MEMSINK_RAW_WRITE_MODE:
                        STREAMER_LOG_DEBUG(DBG_MEM_SINK, "[PROP_SET_RAW_WRITE_MODE]%d",val);
                        ret = memsink_set_raw_write_mode((ElementHandle)element_ptr,
                                                         (uint8_t)val);
                    break;
                    */
        default:
            ret = STREAM_ERR_INFO_ABSENT;
            break;
    }

    STREAMER_FUNC_EXIT(DBG_PROCESS_SINK);
    return ret;
}

/*!
 *
 * @param element_ptr: audio sink element instance
 * @param prop: properties
 * @param val_ptr: parameters
 *
 * @brief This function gets PROCESS sink element's properties
 *
 * @returns AudioSinkStreamErrorType
 *
 * @retval STREAM_OK success
 * @retval STREAM_ERR_INVALID_ARGS bad arguments
 * @retval STREAM_ERR_INFO_ABSENT unknown property
 *
 */
static int32_t processsink_get_property(StreamElement *element_ptr, uint16_t prop, uint32_t *val_ptr)
{
    int32_t ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_PROCESS_SINK);

    if (NULL == val_ptr)
    {
        STREAMER_FUNC_EXIT(DBG_PROCESS_SINK);
        return STREAM_ERR_INVALID_ARGS;
    }

    switch (prop)
    {
        default:
            ret = STREAM_ERR_INFO_ABSENT;
            break;
    }

    STREAMER_FUNC_EXIT(DBG_PROCESS_SINK);
    return ret;
}

int32_t processsink_init_element(StreamElement *element_ptr)
{
    int32_t ret                  = STREAM_OK;
    uint32_t i                   = 0;
    ElementProcessSink *process_sink_ptr = (ElementProcessSink *)element_ptr;

    STREAMER_FUNC_ENTER(DBG_PROCESS_SINK);

    if (NULL == process_sink_ptr)
    {
        ret = STREAM_ERR_ELEMENT_NOT_FOUND;
        goto ERR;
    }

    /* set element change state function */
    process_sink_ptr->change_state = processsink_change_state;
    process_sink_ptr->set_property = processsink_set_property;
    process_sink_ptr->get_property = processsink_get_property;

    /* initialize element fields */
    process_sink_ptr->location      = NULL;
    process_sink_ptr->raw_write     = FILE_RAW_WRITE;
    process_sink_ptr->size          = 0;
    process_sink_ptr->current_index = 0;
    process_sink_ptr->initialized   = false;

    /* initialize only sink pads handlers, no source pads for memory sink */
    for (i = 0; i < NUM_SINKS(TYPE_ELEMENT_PROCESS_SINK); i++)
    {
        PadSink *pad            = (PadSink *)ELEMENT_SINK_PAD(process_sink_ptr, i);
        pad->event_handler      = processsink_sink_pad_event_handler;
        pad->activation_handler = processsink_sink_pad_activation_handler;
        pad->chain_handler      = processsink_sink_pad_chain_handler;
        pad->query_handler      = processsink_sink_pad_query_handler;
        pad->process_precheck   = NULL;
        pad->process_handler    = NULL;
        pad->scheduling         = SCHEDULING_PUSH;
    }
    /* Cant really call init here
     * this is called in pipeline construction
     * and function pointers are not available yet*/
    /*process_ret = process_sink_ptr->init_func(&dummy_arg);
    if ( process_ret != 0)
    {
        STREAMER_LOG_DEBUG(DBG_PROCESS_SINK,
        "[Process SINK]PROCESS init error: %d\n", process_ret);
        return false;
    }*/

ERR:
    STREAMER_FUNC_EXIT(DBG_PROCESS_SINK);
    return ret;
}

int32_t processsink_set_buffer(ElementHandle element_hdl, char *location_ptr, uint32_t size)
{
    ElementProcessSink *process_sink_ptr = (ElementProcessSink *)element_hdl;
    StreamReturnType ret         = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_PROCESS_SINK);

    CHK_ARGS(process_sink_ptr == NULL || location_ptr == NULL || size == 0, STREAM_ERR_INVALID_ARGS);

    if (process_sink_ptr->state >= STATE_PAUSED)
    {
        STREAMER_FUNC_EXIT(DBG_PROCESS_SINK);
        return STREAM_ERR_GENERAL;
    }

    process_sink_ptr->location = location_ptr;
    process_sink_ptr->size     = size;

    STREAMER_FUNC_EXIT(DBG_PROCESS_SINK);
    return ret;
}

int32_t processsink_set_raw_write_mode(ElementHandle element_hdl, uint8_t raw_write_mode)
{
    ElementProcessSink *process_sink_ptr = (ElementProcessSink *)element_hdl;
    StreamReturnType ret         = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_PROCESS_SINK);

    CHK_ARGS(process_sink_ptr == NULL, STREAM_ERR_INVALID_ARGS);

    if (process_sink_ptr->state >= STATE_PAUSED)
    {
        STREAMER_FUNC_EXIT(DBG_PROCESS_SINK);
        return STREAM_ERR_GENERAL;
    }

    process_sink_ptr->raw_write = raw_write_mode;

    STREAMER_FUNC_EXIT(DBG_PROCESS_SINK);
    return ret;
}

int32_t process_register_ext_processing(ElementHandle element,
                                    ProcessSinkInitFunc init_func_ptr,
                                    ProcessSinkPostProcFunc proc_func_ptr,
                                    ProcessSinkDeinitFunc deinit_func_ptr,
                                    void *arg_ptr)
{
    int32_t ret = STREAM_OK;

    ElementProcessSink *process_sink_ptr = (ElementProcessSink *)element;

    STREAMER_FUNC_ENTER(DBG_PROCESS_SINK);

    if ((NULL == process_sink_ptr) || (NULL == init_func_ptr) || (NULL == proc_func_ptr) || (NULL == deinit_func_ptr))
    {
        STREAMER_FUNC_EXIT(DBG_PROCESS_SINK);
        return STREAM_ERR_INVALID_ARGS;
    }

    process_sink_ptr->init_func   = init_func_ptr;
    process_sink_ptr->proc_func   = proc_func_ptr;
    process_sink_ptr->deinit_func = deinit_func_ptr;
    process_sink_ptr->arg_ptr     = arg_ptr;

    STREAMER_FUNC_EXIT(DBG_PROCESS_SINK);
    return ret;
}
