/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    mem_sink.c
 * @brief  This file contains the implementation for the Memory Sink
 */

#include <string.h>

#include "mem_sink.h"
#include "pipeline.h"
#include "streamer_element_properties.h"

/*!
 * @param pad: pad handle
 * @param active: boolean activate/disactivate pad
 *
 * @brief This function activates the sink pad and allocates
 * memory for the memory sink element.
 *
 * @returns true/false
 * @retval   true success
 * @retval   false failed
 */
static uint8_t memsink_sink_pad_activation_handler(StreamPad *pad, uint8_t active)
{
    uint8_t ret = true;
    ElementMemSink *mem_sink_element;

    STREAMER_FUNC_ENTER(DBG_MEM_SINK);

    CHK_ARGS(NULL == pad, false);

    /* get the parent element from the pad */
    mem_sink_element = (ElementMemSink *)PAD_PARENT(pad);

    STREAMER_LOG_DEBUG(DBG_MEM_SINK, "[Memory SINK]Activate pad: %d\n", active);

    if (true == ret)
    {
        /* Activate/Deactivate peer source pads in PUSH mode */
        mem_sink_element->current_index = 0;
        ret                             = pad_activate_push(pad->peer, active);
    }

    STREAMER_FUNC_EXIT(DBG_MEM_SINK);
    return ret;
}

/*!
 * @param pad: pad handle
 * @param buf: input buffer
 *
 * @brief This pad function accumulates data from the decoder and
 * pushes it onto the selected audio driver.
 * The audio sink maintains a double buffer and starts sending data once both
 * the buffers are full. It blocks if the previous data is not sent out by the
 * driver for some reason.
 *
 * @returns FlowReturn
 * @retval FLOW_OK success
 * @retval FLOW_UNEXPECTED bad paramesters
 * @retval FLOW_ERROR write failed
 */

static FlowReturn memsink_sink_pad_chain_handler(StreamPad *pad, StreamBuffer *buf)
{
    ElementMemSink *mem_sink_element = NULL;
    FlowReturn ret                   = FLOW_OK;
    uint32_t pkt_hdr_size            = 0, raw_data_size;
    int8_t *data_ptr;

    STREAMER_FUNC_ENTER(DBG_MEM_SINK);

    CHK_ARGS((NULL == pad) || (NULL == buf), FLOW_UNEXPECTED);

    /* get the parent element from the pad */
    mem_sink_element = (ElementMemSink *)PAD_PARENT(pad);

    /* the buffer should be written to memory here */
    /* check what data packet header the buffer has and typecast accordingly to
     * store the appropriate data
     */
    switch (((PacketHeader *)BUFFER_PTR(buf))->id)
    {
        case RAW_DATA:
            pkt_hdr_size = sizeof(RawPacketHeader);
            break;
        case AUDIO_DATA:
            pkt_hdr_size = sizeof(AudioPacketHeader);
            break;
        default:
            break;
    }

    /* buf->size is the total buffer size including the packet header
     * Get the raw data size considering the offset as offset into raw data
     * from which data has to be written.
     */
    raw_data_size = buf->size - pkt_hdr_size;

    STREAMER_LOG_DEBUG(DBG_MEM_SINK, "\traw_data_size = %d\n", raw_data_size);
    STREAMER_LOG_DEBUG(DBG_MEM_SINK, "\tbuf->offset = %d\n", buf->offset);
    STREAMER_LOG_DEBUG(DBG_MEM_SINK, "\tpkt_hdr_size = %d\n", pkt_hdr_size);

    /* In raw write mode we do not write the packet headers - only data
     * Check if we are not in raw_write mode (basically debug mode)
     */
    if (!mem_sink_element->raw_write)
    {
        /**********************************************
         * THIS IS A DEBUG MODE
         **********************************************/
        /* write header to file */
        data_ptr = buf->buffer;
        if (mem_sink_element->location != NULL)
        {
            if (pkt_hdr_size)
            {
                // Make sure we won't overflow our memory buffer
                if (mem_sink_element->current_index + pkt_hdr_size > mem_sink_element->size)
                {
                    STREAMER_LOG_ERR(DBG_MEM_SINK, ERRCODE_INTERNAL, "memory sink no more source1\n");
                    ret = FLOW_ERROR;
                    goto Error;
                }
                else
                {
                    /* Copy the header into sink buffer and update the
                     * current index */
                    STREAMER_LOG_DEBUG(DBG_MEM_SINK, "memory sink write success 1\n");
                    memcpy(&(mem_sink_element->location[mem_sink_element->current_index]), data_ptr, pkt_hdr_size);
                    mem_sink_element->current_index += pkt_hdr_size;
                }
            }
        }
        else
        {
            STREAMER_LOG_ERR(DBG_MEM_SINK, ERRCODE_INTERNAL, "memory sink location not set. FAIL\n");
            ret = FLOW_ERROR;
            goto Error;
        }
    }

    data_ptr = (buf->buffer + pkt_hdr_size);
    STREAMER_LOG_DEBUG(DBG_MEM_SINK, "\tdata_ptr = %X\n", (uint32_t)data_ptr);
    STREAMER_LOG_DEBUG(DBG_MEM_SINK, "\tcurrent_index = %d\n", mem_sink_element->current_index);

    if (mem_sink_element->location != NULL)
    {
        if (raw_data_size)
        {
            // Make sure we won't overflow our memory buffer
            if (mem_sink_element->current_index + raw_data_size > mem_sink_element->size)
            {
                STREAMER_LOG_ERR(DBG_MEM_SINK, ERRCODE_INTERNAL, "memory sink no more resource!\n");
                ret = FLOW_ERROR;
                goto Error;
            }
            else
            {
                // Copy the data into sink buffer and update the current index
                memcpy(&(mem_sink_element->location[mem_sink_element->current_index]), data_ptr, raw_data_size);
                mem_sink_element->current_index += raw_data_size;
            }
        }
    }
    else
    {
        ret = FLOW_ERROR;
        goto Error;
    }

Error:

    if ((mem_sink_element->current_index == mem_sink_element->size) || (FLOW_OK != ret))
    {
        /* Post EOS message to the pipeline message queue here */
        send_msg_element(PAD_PARENT_ELEMENT(pad), MSG_EOS, 0);
    }
    STREAMER_FUNC_EXIT(DBG_MEM_SINK);
    return ret;
}

/*!
 * @param pad: pad handle
 * @param event: event enumeration
 *
 * @brief This function is the calls the specified audio device
 * driver implementation of the sink pad event handler.
 *
 * @returns true/false
 * @retval true success
 * @retval false failed
 */

static uint8_t memsink_sink_pad_event_handler(StreamPad *pad, StreamEvent *event)
{
    uint8_t ret = true;

    STREAMER_FUNC_ENTER(DBG_MEM_SINK);

    CHK_ARGS(NULL == pad, false);

    /* Device driver type */

    STREAMER_LOG_DEBUG(DBG_MEM_SINK, "Event [Memory SINK]: %d\n", EVENT_TYPE(event) >> 4);

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

    STREAMER_FUNC_EXIT(DBG_MEM_SINK);
    return ret;
}

/*!
 * @param element_ptr: element
 * @param new_state:   memory sink state to be changed
 *
 * @brief This function changes the memory sink state and calls
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
static int32_t memsink_change_state(StreamElement *element_ptr, PipelineState new_state)
{
    PipelineStateChange state_change;
    ElementMemSink *mem_sink_ptr = (ElementMemSink *)element_ptr;
    int32_t ret                  = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_MEM_SINK);

    CHK_ARGS(NULL == mem_sink_ptr, STREAM_ERR_INVALID_ARGS);

    /* get the transition status with the new passed state and the existing
     * state of the sink element
     */
    state_change = (PipelineStateChange)STATE_TRANSITION(mem_sink_ptr->state, new_state);

    mem_sink_ptr->state = new_state;
    switch (state_change)
    {
        case STATE_CHANGE_NULL_TO_READY:
            STREAMER_LOG_DEBUG(DBG_MEM_SINK, "[Memory SINK]STATE_CHANGE_NULL_TO_READY\n");
            /* Nothing to do here for a memory sink */
            break;

        case STATE_CHANGE_PAUSED_TO_PLAYING:
            STREAMER_LOG_DEBUG(DBG_MEM_SINK, "[Memory SINK]STATE_CHANGE_PAUSED_TO_PLAYING\n");
            break;

        case STATE_CHANGE_READY_TO_NULL:
            STREAMER_LOG_DEBUG(DBG_MEM_SINK, "[Memory SINK]STATE_CHANGE_READY_TO_NULL\n");
            /* Nothing to do here for a memory sink */
            break;

            /* When going to pause state then set the pad to blocked state so
             * that data flow is blocked.
             */
        case STATE_CHANGE_READY_TO_PAUSED:
        case STATE_CHANGE_PLAYING_TO_PAUSED:
            STREAMER_LOG_DEBUG(DBG_MEM_SINK, "[Memory SINK]STATE_CHANGE_XXX_TO_PAUSE\n");
            break;

            /* deactivate pads */
        case STATE_CHANGE_PAUSED_TO_READY:
            STREAMER_LOG_DEBUG(DBG_MEM_SINK, "[Memory SINK]STATE_CHANGE_PAUSED_TO_READY\n");
            break;

        default:
            break;
    }

    STREAMER_FUNC_EXIT(DBG_MEM_SINK);
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
static uint8_t memsink_sink_pad_query_handler(StreamPad *pad, StreamQuery *query)
{
    ElementMemSink *element;
    StreamPad *peer;
    uint8_t ret = false;

    STREAMER_FUNC_ENTER(DBG_MEM_SINK);

    CHK_ARGS(NULL == pad || NULL == query || NULL == QUERY_DATA(query), false);

    peer = PAD_PEER(pad);
    CHK_ARGS(NULL == peer, false);

    /* get the parent element from the pad */
    element = (ElementMemSink *)PAD_PARENT(pad);
    CHK_ARGS(NULL == element, false);

    STREAMER_LOG_DEBUG(DBG_MEM_SINK, "mem_sink_sink_pad_query_handler: query[%d]\n", QUERY_TYPE(query));

    switch (QUERY_TYPE(query))
    {
        default:
            /* Relay all query downstream. Memory SINK will only handle current
             * elapsed time.
             */
            ret = pad_query(peer, query);
            break;
    }

    STREAMER_FUNC_EXIT(DBG_MEM_SINK);
    return ret;
}

/*!
 * @param element_ptr: audio sink element instance
 * @param prop: element property
 * @param val:  value to be set
 *
 * @brief This function sets mem sink element's properties
 *
 * @returns StreamReturnType
 *
 * @retval STREAM_OK success
 * @retval STREAM_ERR_INFO_ABSENT unknown property
 * @retval STREAM_GENERAL  bad status to set the property
 *
 */
static int32_t memsink_set_property(StreamElement *element_ptr, uint16_t prop, uint32_t val)
{
    int32_t ret                        = STREAM_OK;
    SET_BUFFER_DESC_T *buffer_desc_ptr = NULL;

    STREAMER_FUNC_ENTER(DBG_MEM_SINK);

    switch (prop)
    {
        case PROP_MEMSINK_BUFFER_DESC:
            buffer_desc_ptr = (SET_BUFFER_DESC_T *)val;
            STREAMER_LOG_DEBUG(DBG_MEM_SINK, "[PROP_SET_BUFFER]%x,%d", (uint32_t)buffer_desc_ptr->ptr,
                               buffer_desc_ptr->size);
            ret = memsink_set_buffer((ElementHandle)element_ptr, (char *)buffer_desc_ptr->ptr, buffer_desc_ptr->size);
            break;

        case PROP_MEMSINK_RAW_WRITE_MODE:
            STREAMER_LOG_DEBUG(DBG_MEM_SINK, "[PROP_SET_RAW_WRITE_MODE]%d", val);
            ret = memsink_set_raw_write_mode((ElementHandle)element_ptr, (uint8_t)val);
            break;

        default:
            ret = STREAM_ERR_INFO_ABSENT;
            break;
    }

    STREAMER_FUNC_EXIT(DBG_MEM_SINK);
    return ret;
}

/*!
 *
 * @param element_ptr: audio sink element instance
 * @param prop:        element property
 * @param val_ptr:     value to be acquired
 *
 * @brief This function gets mem sink element's properties
 *
 * @returns AudioSinkStreamErrorType
 *
 * @retval STREAM_OK success
 * @retval STREAM_ERR_INVALID_ARGS bad arguments
 * @retval STREAM_ERR_INFO_ABSENT unknown property
 *
 */
static int32_t memsink_get_property(StreamElement *element_ptr, uint16_t prop, uint32_t *val_ptr)
{
    ElementMemSink *mem_sink_ptr       = NULL;
    int32_t ret                        = STREAM_OK;
    SET_BUFFER_DESC_T *buffer_desc_ptr = NULL;

    STREAMER_FUNC_ENTER(DBG_MEM_SINK);

    if (NULL == val_ptr)
    {
        STREAMER_FUNC_EXIT(DBG_MEM_SINK);
        return STREAM_ERR_INVALID_ARGS;
    }
    mem_sink_ptr = (ElementMemSink *)element_ptr;
    switch (prop)
    {
        case PROP_MEMSINK_BUFFER_DESC:
            buffer_desc_ptr       = (SET_BUFFER_DESC_T *)val_ptr;
            buffer_desc_ptr->ptr  = (int8_t *)mem_sink_ptr->location;
            buffer_desc_ptr->size = mem_sink_ptr->size;
            break;

        case PROP_MEMSINK_RAW_WRITE_MODE:
            *(uint8_t *)val_ptr = mem_sink_ptr->raw_write;
            break;

        default:
            ret = STREAM_ERR_INFO_ABSENT;
            break;
    }

    STREAMER_FUNC_EXIT(DBG_MEM_SINK);
    return ret;
}

int32_t memsink_init_element(StreamElement *element_ptr)
{
    int32_t ret                  = STREAM_OK;
    uint32_t i                   = 0;
    ElementMemSink *mem_sink_ptr = (ElementMemSink *)element_ptr;

    STREAMER_FUNC_ENTER(DBG_MEM_SINK);

    if (NULL == mem_sink_ptr)
    {
        ret = STREAM_ERR_ELEMENT_NOT_FOUND;
        goto ERR;
    }

    /* set element change state function */
    mem_sink_ptr->change_state = memsink_change_state;
    mem_sink_ptr->set_property = memsink_set_property;
    mem_sink_ptr->get_property = memsink_get_property;

    /* initialize element fields */
    mem_sink_ptr->location      = NULL;
    mem_sink_ptr->raw_write     = FILE_RAW_WRITE;
    mem_sink_ptr->size          = 0;
    mem_sink_ptr->current_index = 0;

    /* initialize only sink pads handlers, no source pads for memory sink */
    for (i = 0; i < NUM_SINKS(TYPE_ELEMENT_MEM_SINK); i++)
    {
        PadSink *pad            = (PadSink *)ELEMENT_SINK_PAD(mem_sink_ptr, i);
        pad->event_handler      = memsink_sink_pad_event_handler;
        pad->activation_handler = memsink_sink_pad_activation_handler;
        pad->chain_handler      = memsink_sink_pad_chain_handler;
        pad->query_handler      = memsink_sink_pad_query_handler;
        pad->process_precheck   = NULL;
        pad->process_handler    = NULL;
    }

ERR:
    STREAMER_FUNC_EXIT(DBG_MEM_SINK);
    return ret;
}

int32_t memsink_set_buffer(ElementHandle element_hdl, char *location_ptr, uint32_t size)
{
    ElementMemSink *mem_sink_ptr = (ElementMemSink *)element_hdl;
    StreamReturnType ret         = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_MEM_SINK);

    CHK_ARGS(mem_sink_ptr == NULL || location_ptr == NULL || size == 0, STREAM_ERR_INVALID_ARGS);

    if (mem_sink_ptr->state >= STATE_PAUSED)
    {
        STREAMER_FUNC_EXIT(DBG_MEM_SINK);
        return STREAM_ERR_GENERAL;
    }

    mem_sink_ptr->location = location_ptr;
    mem_sink_ptr->size     = size;

    STREAMER_FUNC_EXIT(DBG_MEM_SINK);
    return ret;
}

int32_t memsink_set_raw_write_mode(ElementHandle element_hdl, uint8_t raw_write_mode)
{
    ElementMemSink *mem_sink_ptr = (ElementMemSink *)element_hdl;
    StreamReturnType ret         = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_MEM_SINK);

    CHK_ARGS(mem_sink_ptr == NULL, STREAM_ERR_INVALID_ARGS);

    if (mem_sink_ptr->state >= STATE_PAUSED)
    {
        STREAMER_FUNC_EXIT(DBG_MEM_SINK);
        return STREAM_ERR_GENERAL;
    }

    mem_sink_ptr->raw_write = raw_write_mode;

    STREAMER_FUNC_EXIT(DBG_MEM_SINK);
    return ret;
}
