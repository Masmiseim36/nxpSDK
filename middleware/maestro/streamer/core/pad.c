/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    pad.c
 * @brief   This file contains the streamer element pad implementation.
 */

/*
 * INCLUDE FILES
 */
#include "streamer.h"
#include "streamer_api.h"
#include "pad.h"

/*
 * LOCAL FUNCTION PROTOTYPES
 */

/**
 * @brief Function for processing the pad chain handler
 *
 * @param pad sink pad that needs to process the chain handler
 * @param buffer buffer to be processed
 * @return FlowReturn
 */
static FlowReturn pad_chain(StreamPad *pad, StreamBuffer *buffer);
/**
 * @brief Function for processing the pad pull handler
 *
 * @param pad source pad that needs to process the pull handler
 * @param offset position in the source buffer
 * @param size size to be read
 * @param buffer buffer to be filled
 * @return FlowReturn
 */
static FlowReturn pad_pull(StreamPad *pad, uint32_t offset, uint32_t size, StreamBuffer *buffer);
/**
 * @brief Pre activation function
 *
 * @param pad pad to be activated
 * @param new_mode scheduling mode to be applied
 */
static void pre_activate(StreamPad *pad, PadScheduling new_mode);
/**
 * @brief Post activation function
 *
 * @param pad pad to be activated
 * @param new_mode scheduling mode to be applied
 */
static void post_activate(StreamPad *pad, PadScheduling new_mode);
/**
 * @brief Create event to be sent to another pad
 *
 * @param event event object
 * @param type type of event
 * @param format event format
 * @param data event data
 * @return FlowReturn
 */
static FlowReturn event_create(StreamEvent *event, EventType type, StreamDataFormat format, uint32_t data);

/*
 * GLOBAL FUNCTIONS
 */

FlowReturn init_pad(StreamPad *pad, void *parent, PadType type)
{
    STREAMER_FUNC_ENTER(DBG_CORE);

    PAD_SET_FLUSHING(pad);
    PAD_CLEAR_ACTIVATED(pad);

    PAD_TYPE(pad)       = type;
    PAD_PARENT(pad)     = parent;
    PAD_SCHEDULING(pad) = SCHEDULING_NONE;
    PAD_PEER(pad)       = NULL;

    STREAMER_FUNC_EXIT(DBG_CORE);
    return FLOW_OK;
}

FlowReturn deinit_pad(StreamPad *pad)
{
    STREAMER_FUNC_ENTER(DBG_CORE);

    PAD_SET_FLUSHING(pad);
    PAD_CLEAR_ACTIVATED(pad);

    PAD_TYPE(pad)       = TYPE_PAD_LAST;
    PAD_PARENT(pad)     = NULL;
    PAD_SCHEDULING(pad) = SCHEDULING_NONE;

    STREAMER_FUNC_EXIT(DBG_CORE);
    return FLOW_OK;
}

FlowReturn pad_push(StreamPad *pad, StreamBuffer *buffer)
{
    StreamPad *peer;
    FlowReturn ret;

    STREAMER_FUNC_ENTER(DBG_CORE);

    CHK_ARGS(pad == NULL || buffer == NULL || PAD_TYPE(pad) != TYPE_PAD_SRC, FLOW_ERROR);

    peer = PAD_PEER(pad);

    CHK_ARGS(peer == NULL, FLOW_NOT_LINKED);

    ret = pad_chain(peer, buffer);

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

FlowReturn pad_pull_range(StreamPad *pad, uint32_t offset, uint32_t size, StreamBuffer *buffer)
{
    StreamPad *peer;
    FlowReturn ret;

    STREAMER_FUNC_ENTER(DBG_CORE);

    CHK_ARGS(pad == NULL || size == 0 || PAD_TYPE(pad) != TYPE_PAD_SINK, FLOW_ERROR);

    peer = PAD_PEER(pad);

    CHK_ARGS(peer == NULL, FLOW_NOT_LINKED);
    ret = pad_pull(peer, offset, size, buffer);

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

uint8_t pad_push_event(StreamPad *pad, StreamEvent *event)
{
    StreamPad *peerpad;

    STREAMER_FUNC_ENTER(DBG_CORE);

    CHK_ARGS(pad == NULL || event == NULL || EVENT_TYPE(event) == EVENT_UNKNOWN, (uint8_t)FLOW_BAD_PARAMETER);

    /* (un)set the FLUSHING flag for flushing events */
    switch (EVENT_TYPE(event))
    {
        case EVENT_FLUSH_START:
            PAD_SET_FLUSHING(pad);
            break;

        case EVENT_FLUSH_STOP:
            PAD_CLEAR_FLUSHING(pad);
            break;

        default:
            break;
    }

    peerpad = PAD_PEER(pad);

    if (peerpad == NULL)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return false;
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return pad_send_event(peerpad, event);
}

uint8_t pad_send_event(StreamPad *pad, StreamEvent *event)
{
    uint8_t ret = false;
    PadEventFunc eventfunc;
    uint8_t serialized;

    STREAMER_FUNC_ENTER(DBG_CORE);

    CHK_ARGS(pad == NULL || event == NULL || EVENT_TYPE(event) == EVENT_UNKNOWN, false);

    if (PAD_TYPE(pad) == TYPE_PAD_SINK)
    {
        if (EVENT_IS_DOWNSTREAM(event) == (uint8_t) false)
            goto wrong_direction;

        serialized = EVENT_IS_SERIALIZED(event);
    }
    else if (PAD_TYPE(pad) == TYPE_PAD_SRC)
    {
        if (EVENT_IS_UPSTREAM(event) == (uint8_t) false)
            goto wrong_direction;

        /* events on srcpad never are serialized */
        serialized = false;
    }
    else
        goto wrong_direction;

    switch (EVENT_TYPE(event))
    {
        case EVENT_FLUSH_START:
            /* can't even accept a flush begin event when flushing */
            if (PAD_IS_FLUSHING(pad))
                goto flushing;

            PAD_SET_FLUSHING(pad);
            break;

        case EVENT_FLUSH_STOP:
            PAD_CLEAR_FLUSHING(pad);
            break;

        default:
            /* can't accept event when flushing */
            if (PAD_IS_FLUSHING(pad))
                goto flushing;

            if (serialized)
            {
                if ((PAD_IS_FLUSHING(pad)))
                    goto flushing;
            }
            break;
    }

    if ((eventfunc = PAD_EVENTFUNC(pad)) == NULL)
        goto no_function;

    ret = eventfunc(pad, event);

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;

    /* ERROR handling */
wrong_direction:
    STREAMER_FUNC_EXIT(DBG_CORE);
    return false;

flushing:
no_function:
    STREAMER_FUNC_EXIT(DBG_CORE);
    return false;
}

uint8_t activate_pad(StreamPad *pad, uint8_t activate)
{
    uint8_t ret;
    PadActivateFunc activatefunc;
    activatefunc = PAD_ACTIVATEFUNC(pad);

    STREAMER_FUNC_ENTER(DBG_CORE);

    if (activate)
    {
        PAD_CLEAR_FLUSHING(pad);
        PAD_SET_ACTIVATED(pad);
    }
    else
    {
        /* Before acquiring stream lock, deactivate function needs to be
         * called in case the pad is stuck, deactivate will unstuck the pad
         * and then wait for the stream flow to be over.
         */
        PAD_SET_FLUSHING(pad);
        PAD_CLEAR_ACTIVATED(pad);
    }

    if (activatefunc && PAD_PEER(pad))
    {
        ret = activatefunc(pad, activate);
        if (ret != (uint8_t) true)
        {
            STREAMER_FUNC_EXIT(DBG_CORE);
            return false;
        }
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return true;
}

uint8_t pad_activate_pull(StreamPad *pad, uint8_t active)
{
    PadScheduling old;
    PadScheduling new;
    PadSrc *src = (PadSrc *)pad;
    uint8_t ret = true;

    STREAMER_FUNC_ENTER(DBG_CORE);

    old = PAD_SCHEDULING(pad);
    if (active)
    {
        switch (old)
        {
            case SCHEDULING_PULL:
                goto Exit;

            case SCHEDULING_PUSH:
                /* pad was activate in the wrong direction, deactivate it
                 * and reactivate it in pull mode */

                if (pad_activate_push(pad, false) == (uint8_t) false)
                {
                    ret = false;
                    goto Exit;
                }
                /* fallthrough, pad is deactivated now. */

            case SCHEDULING_NONE:
                break;
        }
    }
    else
    {
        switch (old)
        {
            case SCHEDULING_NONE:
                goto Exit;

            case SCHEDULING_PUSH:
                /* pad was activated in the other direction, deactivate it
                 * in push mode, this should not happen... */
                if (pad_activate_push(pad, false) == (uint8_t) false)
                    ret = false;

                goto Exit;

            case SCHEDULING_PULL:
                break;
        }
    }

    new = active ? SCHEDULING_PULL : SCHEDULING_NONE;

    pre_activate(pad, new);

    /* Can't activate pull on a src without a pull function */
    if (PAD_PULLFUNC(src) == NULL)
        ret = false;
    else if (PAD_ACTIVATEPULLFUNC(pad))
    {
        if (PAD_ACTIVATEPULLFUNC(pad)(pad, active) == (uint8_t) false)
            ret = false;
    }

    if ((uint8_t) true == ret)
        post_activate(pad, new);

Exit:
    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

uint8_t pad_activate_push(StreamPad *pad, uint8_t active)
{
    PadScheduling old;
    PadScheduling new;
    uint8_t ret = true;

    STREAMER_FUNC_ENTER(DBG_CORE);

    old = PAD_SCHEDULING(pad);

    if (active)
    {
        switch (old)
        {
            case SCHEDULING_PUSH:
                goto Exit;

            case SCHEDULING_PULL:
                /* pad was activate in the wrong direction, deactivate it
                 * an reactivate it in push mode */
                if (pad_activate_pull(pad, false) == (uint8_t) false)
                {
                    ret = false;
                    goto Exit;
                }

                /* fallthrough, pad is deactivated now. */
            case SCHEDULING_NONE:
                break;
        }
    }
    else
    {
        switch (old)
        {
            case SCHEDULING_NONE:
                goto Exit;
            case SCHEDULING_PULL:
                /* pad was activated in the other direction, deactivate it
                 * in pull mode, this should not happen... */
                if (pad_activate_pull(pad, false) == (uint8_t) false)
                    ret = false;

                /* everything is fine now */
                goto Exit;

            case SCHEDULING_PUSH:
                break;
        }
    }

    new = active ? SCHEDULING_PUSH : SCHEDULING_NONE;

    pre_activate(pad, new);

    if (PAD_ACTIVATEPUSHFUNC(pad))
    {
        if (PAD_ACTIVATEPUSHFUNC(pad)(pad, active) == (uint8_t) false)
            ret = false;
    }

    if ((uint8_t) true == ret)
        post_activate(pad, new);

Exit:
    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

FlowReturn event_create_flush_start(StreamEvent *event)
{
    return event_create(event, EVENT_FLUSH_START, DATA_FORMAT_UNDEFINED, 0);
}

FlowReturn event_create_flush_stop(StreamEvent *event)
{
    return event_create(event, EVENT_FLUSH_STOP, DATA_FORMAT_UNDEFINED, 0);
}

FlowReturn event_create_eos(StreamEvent *event)
{
    return event_create(event, EVENT_EOS, DATA_FORMAT_UNDEFINED, 0);
}

FlowReturn event_create_new_segment(StreamEvent *event, StreamDataFormat format, uint32_t data)
{
    return event_create(event, EVENT_NEWSEGMENT, format, data);
}

FlowReturn event_create_seek(StreamEvent *event, StreamDataFormat format, uint32_t data)
{
    return event_create(event, EVENT_SEEK, format, data);
}

uint8_t pad_query(StreamPad *pad, StreamQuery *query)
{
    PadQueryFunc func;

    STREAMER_FUNC_ENTER(DBG_CORE);

    CHK_ARGS(pad == NULL || query == NULL, false);

    CHK_ARGS((func = PAD_QUERYFUNC(pad)) == NULL, false);

    STREAMER_FUNC_EXIT(DBG_CORE);
    return func(pad, query);
}

FlowReturn query_create(
    StreamQuery *query, StreamInfoType type, StreamDataFormat format, StreamDataType data_type, StreamData *data)
{
    FlowReturn ret = FLOW_BAD_PARAMETER;

    STREAMER_FUNC_ENTER(DBG_CORE);

    /* If allocated */
    if (query != NULL)
    {
        QUERY_TYPE(query)     = type;
        QUERY_FORMAT(query)   = format;
        QUERY_DATATYPE(query) = data_type;
        QUERY_DATA(query)     = data;

        ret = FLOW_OK;
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

/*
 * LOCAL FUNCTIONS
 */

static FlowReturn pad_chain(StreamPad *pad, StreamBuffer *buffer)
{
    PadSink *sink = (PadSink *)pad;
    PadChainFunc chainfunc;
    FlowReturn ret;

    STREAMER_FUNC_ENTER(DBG_CORE);

    CHK_ARGS(PAD_TYPE(pad) != TYPE_PAD_SINK, FLOW_ERROR);

    /* Check if the pad is sink and chain function is in place */
    CHK_ARGS((TYPE_PAD_SINK != PAD_TYPE(pad)), FLOW_NOT_SUPPORTED);
    CHK_ARGS((chainfunc = PAD_CHAINFUNC(sink)) == NULL, FLOW_NOT_SUPPORTED);

    /* Return with error if the pads are being flushed right now. */
    CHK_ARGS(PAD_IS_FLUSHING(pad), FLOW_WRONG_STATE);

    /* Return the result from the chain function call */
    ret = chainfunc(pad, buffer);

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

static FlowReturn pad_pull(StreamPad *pad, uint32_t offset, uint32_t size, StreamBuffer *buffer)
{
    FlowReturn ret;
    PadPullFunc pullfunc;
    PadSrc *src = (PadSrc *)pad;

    STREAMER_FUNC_ENTER(DBG_CORE);

    CHK_ARGS(pad == NULL || size == 0 || PAD_TYPE(pad) != TYPE_PAD_SRC, FLOW_ERROR);

    CHK_ARGS(PAD_IS_FLUSHING(pad), FLOW_WRONG_STATE);
    CHK_ARGS((pullfunc = PAD_PULLFUNC(src)) == NULL, FLOW_NOT_SUPPORTED);

    ret = (FlowReturn)pullfunc(pad, buffer, size, offset);

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

static void pre_activate(StreamPad *pad, PadScheduling new_mode)
{
    STREAMER_FUNC_ENTER(DBG_CORE);

    switch (new_mode)
    {
        case SCHEDULING_PUSH:
        case SCHEDULING_PULL:
            PAD_CLEAR_FLUSHING(pad);
            PAD_SCHEDULING(pad) = new_mode;
            PAD_SET_ACTIVATED(pad);
            break;

        case SCHEDULING_NONE:
            PAD_SET_FLUSHING(pad);
            PAD_SCHEDULING(pad) = new_mode;
            break;
    }
    STREAMER_FUNC_EXIT(DBG_CORE);
}

static void post_activate(StreamPad *pad, PadScheduling new_mode)
{
    STREAMER_FUNC_ENTER(DBG_CORE);

    switch (new_mode)
    {
        case SCHEDULING_PUSH:
        case SCHEDULING_PULL:
        case SCHEDULING_NONE:
            /* ensures that streaming stops */
            break;
    }
    STREAMER_FUNC_EXIT(DBG_CORE);
}

static FlowReturn event_create(StreamEvent *event, EventType type, StreamDataFormat format, uint32_t data)
{
    FlowReturn ret = FLOW_BAD_PARAMETER;

    STREAMER_FUNC_ENTER(DBG_CORE);

    /* If allocated */
    if (event != NULL)
    {
        EVENT_TYPE(event)   = type;
        EVENT_FORMAT(event) = format;
        EVENT_DATA(event)   = data;

        ret = FLOW_OK;
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

/* End of File */
