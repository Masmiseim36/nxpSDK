/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    netbuf_src.c
 * @brief   This file implement the network buffer source fucntions
 */

#include <string.h>

#include "streamer_element.h"
#include "netbuf_src.h"
#include "pipeline.h"
#include "streamer_element_properties.h"

static int32_t netbufsrc_change_state(StreamElement *element, PipelineState state);
static uint8_t netbufsrc_handle_src_event(StreamPad *pad, StreamEvent *event);
static uint8_t netbufsrc_handle_src_query(StreamPad *pad, StreamQuery *event);
static uint8_t netbufsrc_src_activate_pull(StreamPad *pad, uint8_t active);
static uint8_t netbufsrc_src_activation_handler(StreamPad *pad, uint8_t active);
static FlowReturn netbufsrc_pull(StreamPad *pad, StreamBuffer *buffer, uint32_t size, uint32_t offset);
static int32_t netbufsrc_get_property(StreamElement *element_ptr, uint16_t prop, uint32_t *val_ptr);
static int32_t netbufsrc_set_property(StreamElement *element_ptr, uint16_t prop, uintptr_t val);

int32_t netbufsrc_init(StreamElement *element)
{
    PadSrc *pad                 = NULL;
    ElementNetbufSrc *netbufsrc = (ElementNetbufSrc *)element;

    STREAMER_FUNC_ENTER(DBG_NETBUFSRC);

    /* element specific init */
    netbufsrc->end_of_stream  = false;
    netbufsrc->audio_callback = NULL;

    /* set element change state function */
    netbufsrc->change_state = netbufsrc_change_state;
    netbufsrc->set_property = netbufsrc_set_property;
    netbufsrc->get_property = netbufsrc_get_property;

    /* init elements pads */
    for (int32_t i = 0; i < NUM_SRCS(TYPE_ELEMENT_NETBUF_SRC); i++)
    {
        pad = (PadSrc *)ELEMENT_SRC_PAD(netbufsrc, i);

        /* Set handlers */
        pad->event_handler      = netbufsrc_handle_src_event;
        pad->query_handler      = netbufsrc_handle_src_query;
        pad->activation_handler = netbufsrc_src_activation_handler;

        pad->activate_pull = netbufsrc_src_activate_pull;
        pad->activate_push = NULL;

        /* Set source pad pull function */
        pad->pull_handler     = netbufsrc_pull;
        pad->process_handler  = NULL;
        pad->process_precheck = NULL;
    }

    STREAMER_FUNC_EXIT(DBG_NETBUFSRC);
    return 0;
}

/*!
 *
 * @param element: Pointer to element
 * @param state: changed state.
 *
 * @brief Function handles the state change of the element in a pipeline.
 */
static int32_t netbufsrc_change_state(StreamElement *element, PipelineState state)
{
    PipelineStateChange state_change;
    ElementNetbufSrc *netbufsrc = (ElementNetbufSrc *)element;
    int32_t ret                 = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_NETBUFSRC);

    CHK_ARGS(NULL == netbufsrc, STREAM_ERR_INVALID_ARGS);

    /* get the transition status with the new passed state and the existing
     * state of the decoder element
     */
    state_change = (PipelineStateChange)STATE_TRANSITION(netbufsrc->state, state);

    switch (state_change)
    {
        case STATE_CHANGE_NULL_TO_READY:
            STREAMER_LOG_DEBUG(DBG_NETBUFSRC, "[NetbufSRC]STATE_CHANGE_NULL_TO_READY\n");
            break;
        case STATE_CHANGE_READY_TO_PAUSED:
            STREAMER_LOG_DEBUG(DBG_NETBUFSRC, "[NetbufSRC]STATE_CHANGE_READY_TO_PAUSED\n");
            break;
        case STATE_CHANGE_PAUSED_TO_PLAYING:
            STREAMER_LOG_DEBUG(DBG_NETBUFSRC, "[NetbufSRC]STATE_CHANGE_PAUSED_TO_PLAYING\n");
            break;
        case STATE_CHANGE_PLAYING_TO_PAUSED:
            STREAMER_LOG_DEBUG(DBG_NETBUFSRC, "[NetbufSRC]STATE_CHANGE_PLAYING_TO_PAUSED\n");
            break;
            /* do nothing */
        case STATE_CHANGE_PAUSED_TO_READY:
            STREAMER_LOG_DEBUG(DBG_NETBUFSRC, "[NetbufSRC]STATE_CHANGE_PAUSED_TO_READY\n");
            break;
            /* Pads will be deactivated here. */
        case STATE_CHANGE_READY_TO_NULL:
            STREAMER_LOG_DEBUG(DBG_NETBUFSRC, "[NetbufSRC]STATE_CHANGE_READY_TO_NULL\n");
            break;
            /* close resources if anything was opened
             * N/A for decoder
             */
        default:
            break;
    }

    STREAMER_FUNC_EXIT(DBG_NETBUFSRC);
    return ret;
}

/*!
 *
 * @param pad: Queue element src pad
 * @param active: To activate or deactivate pad.
 *
 * @brief queue src pad activatation handler. Activation is handled
 * by the peer sink pad, this should just handle the deactivation.
 */
static uint8_t netbufsrc_src_activation_handler(StreamPad *pad, uint8_t active)
{
    uint8_t ret = true;

    STREAMER_FUNC_ENTER(DBG_NETBUFSRC);

    STREAMER_LOG_DEBUG(DBG_NETBUFSRC, "netbufsrc_src_activation_handler(%d)\n", (uint32_t)active);

    if (!active)
    {
        PadScheduling mode = PAD_SCHEDULING(pad);

        if (SCHEDULING_PUSH == mode)
        {
            /* Fall through to close source (ret == true).  Not supported. */
            STREAMER_LOG_DEBUG(DBG_NETBUFSRC, "[NetbufSRC]Sched mode PUSH\n");
        }
        else if (SCHEDULING_PULL == mode)
        {
            STREAMER_LOG_DEBUG(DBG_NETBUFSRC, "[NetbufSRC]Sched mode PULL\n");
            ret = pad_activate_pull(pad, active);
        }

        if (ret)
        {
            STREAMER_LOG_DEBUG(DBG_NETBUFSRC, "[NetbufSRC]Close NetbufSRC\n");
        }
    }

    STREAMER_FUNC_EXIT(DBG_NETBUFSRC);
    return ret;
}

/*!
 *
 * @param netbufsrc: Pointer to netbufsrc element
 * @param offset: Offset of the file to read data from
 * @param length: Number of bytes to read
 * @param buf: Pointer to buffer to read data
 *
 * @brief Reads the data from the memory array specified by the element.
 * NOTE: Function may or may not be able to read the required
 * length of data.
 * @retval FLOW_EOS when EOF file is encountered.
 * @retval FLOW_ERROR when read fails.
 *
 */
static FlowReturn netbufsrc_read(ElementNetbufSrc *netbufsrc, int32_t offset, int32_t length, StreamBuffer *buf)
{
    int ret;

    STREAMER_FUNC_ENTER(DBG_NETBUFSRC);

    STREAMER_LOG_DEBUG(DBG_NETBUFSRC, "[NetbufSRC]read: size %d, offset %d\n", length, offset);

    ret = netbufsrc->audio_callback(buf->buffer + sizeof(RawPacketHeader), length);
    /* The decoder will detect EOS, so don't check here for return value. */

    buf->offset = offset;
    /* Only read up to max available. */
    buf->size = sizeof(RawPacketHeader) + ret;

    STREAMER_FUNC_EXIT(DBG_NETBUFSRC);

    return (ret != length) ? FLOW_EOS : FLOW_OK;
}

/*!
 *
 * @param pad: Element pad
 * @param buffer: Pointer of the buffer to read data
 * @param size: Number of bytes to read
 * @param offset: Offset to the file
 *
 * @brief Function is the pull function of the netbufsrc source pad in
 * SCHEDULING_PULL mode. It reads the size of data from
 * the offset to buffer.
 *
 */
static FlowReturn netbufsrc_pull(StreamPad *pad, StreamBuffer *buffer, uint32_t size, uint32_t offset)
{
    ElementNetbufSrc *netbufsrc = (ElementNetbufSrc *)pad->parent;
    FlowReturn ret              = FLOW_OK;

    STREAMER_FUNC_ENTER(DBG_NETBUFSRC);

    /* Put Raw header in the buffer */
    ((PacketHeader *)BUFFER_PTR(buffer))->id = RAW_DATA;

    /* read chunk_size of data */
    ret = netbufsrc_read(netbufsrc, offset, size, buffer);
    if (ret == FLOW_EOS)
    {
        /* Its an end of stream */
        if (netbufsrc->end_of_stream != true)
        {
            STREAMER_LOG_DEBUG(DBG_NETBUFSRC, "[NetbufSRC]Pull EOS");

            /* Need not send the EOS event here. The element calling this
             * function should check the return value and send EOS event
             * downstream.
             */
            netbufsrc->end_of_stream = true;
        }
    }

    STREAMER_FUNC_EXIT(DBG_NETBUFSRC);
    return ret;
}

/*!
 *
 * @param pad: Element pad
 * @param event: type of event
 *
 * @brief Function handles the events of the element source pad.
 *
 */
static uint8_t netbufsrc_handle_src_event(StreamPad *pad, StreamEvent *event)
{
    uint8_t ret = true;

    STREAMER_FUNC_ENTER(DBG_NETBUFSRC);

    /* handle the event */
    switch (EVENT_TYPE(event))
    {
            /* case EVENT_EOS: */
            /* case EVENT_NEWSEGMENT: */

        case EVENT_SEEK:
        case EVENT_FLUSH_START:
        case EVENT_FLUSH_STOP:
        default:
            break;
    }

    STREAMER_FUNC_EXIT(DBG_NETBUFSRC);
    return ret;
}

/*!
 *
 * @param pad: pad handle
 * @param query: query
 *
 * @brief Function Description: This function is the empty impl of the src pad
 * query handler.
 *
 * @returns uint8_t
 */

static uint8_t netbufsrc_handle_src_query(StreamPad *pad, StreamQuery *query)
{
    uint8_t ret      = false;
    StreamData *data = NULL;

    STREAMER_FUNC_ENTER(DBG_NETBUFSRC);

    CHK_ARGS(NULL == pad || NULL == query || NULL == QUERY_DATA(query), false);
    data = QUERY_DATA(query);

    switch (QUERY_TYPE(query))
    {
        case INFO_TIME_SEEKABLE:
            data->valuebool = false;
            ret             = true;
            break;

        case INFO_BYTE_SEEKABLE:
            data->valuebool = false;
            ret             = true;
            break;

        case INFO_SIZE:
            data->value32u = 1; /* This is a size number due to decoder needs*/
            ret            = true;
            break;
        default:
            break;
    }

    STREAMER_FUNC_EXIT(DBG_NETBUFSRC);

    return ret;
}

/*!
 *
 * @param pad: Element pad
 * @param active: true if pad is to be activated else false to deactivate
 *
 * @brief Function activates or deactivates the netbufsrc source pad in PULL mode.
 *
 */
static uint8_t netbufsrc_src_activate_pull(StreamPad *pad, uint8_t active)
{
    ElementNetbufSrc *netbufsrc = (ElementNetbufSrc *)pad->parent;

    STREAMER_FUNC_ENTER(DBG_NETBUFSRC);

    /* Activate pad in PULL mode */
    if (active)
    {
        /* start of stream */
        STREAMER_LOG_DEBUG(DBG_NETBUFSRC, "[NetbufSRC]Activate pull mode: %s\n", "netbufsrc");
    }
    else
    {
        /* Deactivate pad in PULL mode */
        STREAMER_LOG_DEBUG(DBG_NETBUFSRC, "[NetbufSRC]Deactivate pull mode: %s\n", "netbufsrc");
    }

    netbufsrc->end_of_stream = false;

    STREAMER_FUNC_EXIT(DBG_NETBUFSRC);
    return true;
}

/*!
 *
 * @param element_ptr: element pointer
 * @param prop: properties
 * @param val_ptr: parameters
 *
 * @brief This function get memory source element properties
 *
 * @returns StreamErrorType
 *
 */
static int32_t netbufsrc_get_property(StreamElement *element_ptr, uint16_t prop, uint32_t *val_ptr)
{
    STREAMER_FUNC_ENTER(DBG_NETBUFSRC);

    if (!val_ptr)
    {
        STREAMER_FUNC_EXIT(DBG_NETBUFSRC);
        return STREAM_ERR_INVALID_ARGS;
    }

    /* No 'get' properties for this source. */
    switch (prop)
    {
        default:
            STREAMER_FUNC_EXIT(DBG_NETBUFSRC);
            return STREAM_ERR_INVALID_ARGS;
    }
}

/*!
 *
 * @param element_ptr: element pointer
 * @param prop: properties
 * @param val: parameters
 *
 * @brief This function set memory source element properties
 *
 * @returns StreamErrorType
 *
 */
static int32_t netbufsrc_set_property(StreamElement *element_ptr, uint16_t prop, uintptr_t val)
{
    ElementNetbufSrc *netbufsrc_ptr = NULL;

    STREAMER_FUNC_ENTER(DBG_NETBUFSRC);

    netbufsrc_ptr = (ElementNetbufSrc *)element_ptr;

    switch (prop)
    {
        case PROP_NETBUFSRC_SET_CALLBACK:
            netbufsrc_ptr->audio_callback = (audio_callback_t *)val;
            break;

        default:
            STREAMER_FUNC_EXIT(DBG_NETBUFSRC);
            return STREAM_ERR_INVALID_ARGS;
    }

    STREAMER_FUNC_EXIT(DBG_NETBUFSRC);
    return STREAM_OK;
}
