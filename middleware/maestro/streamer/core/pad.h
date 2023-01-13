/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef PAD_H
#define PAD_H

/*!
 * @file    pad.h
 * @brief   This file contains the definition of an element pad object.
 */

#include "streamer.h"
#include "streamer_api.h"

/*
 * CONSTANTS, DEFINES AND MACROS
 */

/**
 * @brief Get buffer pointer
 *
 */
#define BUFFER_PTR(pstbuffer) ((pstbuffer)->buffer)
/**
 * @brief Get buffer size
 *
 */
#define BUFFER_SIZE(pstbuffer) ((pstbuffer)->size)
/**
 * @brief Get buffer offset
 *
 */
#define BUFFER_OFFSET(pstbuffer) ((pstbuffer)->offset)
/**
 * @brief Get time
 *
 */
#define BUFFER_TIME(pstbuffer) ((pstbuffer)->time)
/**
 * @brief The same thing as EVENT_TYPE_UPSTREAM | EVENT_TYPE_DOWNSTREAM.
 *
 */
#define EVENT_TYPE_BOTH (EVENT_TYPE_UPSTREAM | EVENT_TYPE_DOWNSTREAM)
/**
 * @brief Shift for custom event type creation
 *
 */
#define EVENT_TYPE_SHIFT 4

/*!
 * EVENT_MAKE_TYPE:
 * @param num: the event number to create
 * @param flags: the event flags
 *
 * when making custom event types, use this macro with the num and
 * the given flags
 */
#define EVENT_MAKE_TYPE(num, flags) (((num) << EVENT_TYPE_SHIFT) | (flags))
/**
 * @brief Event type flag creation
 *
 */
#define FLAG(name) EVENT_TYPE_##name

/*!
 * EVENT_IS_UPSTREAM
 * @param ev: the event to query
 *
 * Check if an event can travel upstream.
 */
#define EVENT_IS_UPSTREAM(ev) ((uint32_t)EVENT_TYPE(ev) & (uint32_t)EVENT_TYPE_UPSTREAM)
/*!
 * EVENT_IS_DOWNSTREAM
 * @param ev: the event to query
 *
 * Check if an event can travel downstream.
 */
#define EVENT_IS_DOWNSTREAM(ev) ((uint32_t)EVENT_TYPE(ev) & (uint32_t)EVENT_TYPE_DOWNSTREAM)
/*!
 * EVENT_IS_SERIALIZED
 * @param ev: the event to query
 *
 * Check if an event is serialized with the data stream.
 */
#define EVENT_IS_SERIALIZED(ev) ((uint32_t)EVENT_TYPE(ev) & (uint32_t)EVENT_TYPE_SERIALIZED)

/**
 * @brief Get event type
 *
 */
#define EVENT_TYPE(pstevent) ((pstevent)->type)
/**
 * @brief Get event format
 *
 */
#define EVENT_FORMAT(pstevent) ((pstevent)->format)
/**
 * @brief Get event data
 *
 */
#define EVENT_DATA(pstevent) ((pstevent)->data)
/**
 * @brief Get query type
 *
 */
#define QUERY_TYPE(pStreamQuery) ((pStreamQuery)->type)
/**
 * @brief Get query format
 *
 */
#define QUERY_FORMAT(pStreamQuery) ((pStreamQuery)->format)
/**
 * @brief Get query data type
 *
 */
#define QUERY_DATATYPE(pStreamQuery) ((pStreamQuery)->data_type)
/**
 * @brief Get query data
 *
 */
#define QUERY_DATA(pStreamQuery) ((pStreamQuery)->data)
/**
 * @brief Generic pad
 *
 */
#define MEMBERS_PAD                     \
    void *parent;                       \
    int8_t flushing;                    \
    int8_t blocked;                     \
    int8_t activated;                   \
    PadType type;                       \
    PadScheduling scheduling;           \
    struct _StreamPad *peer;            \
    PadEventFunc event_handler;         \
    PadQueryFunc query_handler;         \
    PadActivateFunc activation_handler; \
    PadActivateFunc activate_pull;      \
    PadActivateFunc activate_push;      \
    PadProcessFunc process_handler;     \
    PadPrecheckFunc process_precheck

/**
 * @brief Get pad parent element
 *
 */
#define PAD_PARENT_ELEMENT(pstpad) ((StreamElement *)((pstpad)->parent))
/**
 * @brief Get pad parent pipeline
 *
 */
#define PAD_PARENT_PIPELINE(pstpad) ((Pipeline *)((PAD_PARENT_ELEMENT(pstpad))->parent))
/**
 * @brief Get pad parent
 *
 */
#define PAD_PARENT(pstpad) ((pstpad)->parent)
/**
 * @brief Get pad type
 *
 */
#define PAD_TYPE(pstpad) ((pstpad)->type)
/**
 * @brief Get pad scheduling
 *
 */
#define PAD_SCHEDULING(pstpad) ((pstpad)->scheduling)
/**
 * @brief Get pad peer
 *
 */
#define PAD_PEER(pstpad) ((pstpad)->peer)
/**
 * @brief Get pad flushing
 *
 */
#define PAD_IS_FLUSHING(pstpad) ((pstpad)->flushing)
/**
 * @brief Set pad flushing
 *
 */
#define PAD_SET_FLUSHING(pstpad) ((pstpad)->flushing = true)
/**
 * @brief Clear pad flushing
 *
 */
#define PAD_CLEAR_FLUSHING(pstpad) ((pstpad)->flushing = false)
/**
 * @brief Get pad activated
 *
 */
#define PAD_IS_ACTIVATED(pstpad) ((pstpad)->activated)
/**
 * @brief Set pad activated
 *
 */
#define PAD_SET_ACTIVATED(pstpad) ((pstpad)->activated = true)
/**
 * @brief Clear pad activated
 *
 */
#define PAD_CLEAR_ACTIVATED(pstpad) ((pstpad)->activated = false)
/**
 * @brief Get pad activation handler
 *
 */
#define PAD_ACTIVATEFUNC(pstpad) ((pstpad)->activation_handler)
/**
 * @brief Get pad activate pull function
 *
 */
#define PAD_ACTIVATEPULLFUNC(pstpad) ((pstpad)->activate_pull)
/**
 * @brief Get pad activate push function
 *
 */
#define PAD_ACTIVATEPUSHFUNC(pstpad) ((pstpad)->activate_push)
/**
 * @brief Get pad event handler
 *
 */
#define PAD_EVENTFUNC(pstpad) ((pstpad)->event_handler)
/**
 * @brief Get pad query handler
 *
 */
#define PAD_QUERYFUNC(pstpad) ((pstpad)->query_handler)
/**
 * @brief Get pad pull handler
 *
 */
#define PAD_PULLFUNC(pstpad) ((pstpad)->pull_handler)
/**
 * @brief Get pad chain handler
 *
 */
#define PAD_CHAINFUNC(pstpad) ((pstpad)->chain_handler)

/*
 *  ENUMS
 */

/*!
 * EventTypeFlags:
 * EventTypeFlags indicate the aspects of the different EventType values.
 */
typedef enum
{
    EVENT_TYPE_UPSTREAM   = 1 << 0, /*!< @brief Travels towards source */
    EVENT_TYPE_DOWNSTREAM = 1 << 1, /*!< @brief Travels towards sink */
    EVENT_TYPE_SERIALIZED = 1 << 2  /*!< @brief Should be serialized with data
                                                flow. */

} EventTypeFlags;

/*!
 * EventType
 * EventType lists the standard event types that can be sent in a pipeline.
 */
typedef enum
{
    EVENT_UNKNOWN = EVENT_MAKE_TYPE(0, 0), /*!< @brief Unknown */

    /* bidirectional events */
    EVENT_FLUSH_START = EVENT_MAKE_TYPE(1, FLAG(BOTH)),
    /*!< @brief Start a flush operation */
    EVENT_FLUSH_STOP = EVENT_MAKE_TYPE(2, FLAG(BOTH) | FLAG(SERIALIZED)),
    /*!< @brief Stop a flush operation */

    /* downstream serialized events */
    EVENT_EOS = EVENT_MAKE_TYPE(3, FLAG(DOWNSTREAM) | FLAG(SERIALIZED)),
    /*!< @brief End-Of-Stream. No more data is to be expected to follow
                without a NEWSEGMENT event.*/
    EVENT_NEWSEGMENT = EVENT_MAKE_TYPE(4, FLAG(DOWNSTREAM) | FLAG(SERIALIZED)),
    /*!< @brief A new media segment follows in the dataflow.*/

    /* upstream events */
    EVENT_SEEK = EVENT_MAKE_TYPE(5, FLAG(UPSTREAM))
    /*!< @brief A request for a new playback position and rate. */

} EventType;

/*!
 * PadScheduling
 * The status of a Pad. After activating a pad, which usually happens when the
 * parent element goes from READY to PAUSED, the ActivateMode defines if the
 * pad operates in push or pull mode.
 */
typedef enum
{
    SCHEDULING_NONE, /*!< @brief Pad will not handle dataflow */
    SCHEDULING_PULL, /*!< @brief Pad handles dataflow in upstream pull mode */
    SCHEDULING_PUSH  /*!< @brief Pad handles dataflow in downstream push mode */
} PadScheduling;

/*!
 * PadType
 * The type of pad, based on the direction of data.
 */
typedef enum
{
    TYPE_PAD_SRC,  /*!< @brief Source Pad */
    TYPE_PAD_SINK, /*!< @brief Sink Pad */
    TYPE_PAD_LAST
} PadType;

/*!
 * FlowReturn
 * The result of passing data to a pad.
 * Note that the custom return values should not be exposed outside of the
 * element scope and are available since 0.10.7.
 */
typedef enum
{
    /* core predefined */
    FLOW_OK = 0, /*!< @brief Data passing was ok. */

    /* expected failures */
    FLOW_NOT_LINKED  = -1, /*!< @brief Pad is not linked. */
    FLOW_WRONG_STATE = -2, /*!< @brief Pad is in wrong state. */

    /* error cases */
    FLOW_UNEXPECTED = -3,    /*!< @brief Did not expect anything, like after
                                         EOS. */
    FLOW_ERROR = -4,         /*!< @brief Some (fatal) error occured. Element
                                         generating this error should post an
                                         error message with more details. */
    FLOW_NOT_SUPPORTED = -5, /*!< @brief This operation is not supported. */
    FLOW_BAD_PARAMETER = -6, /*!< @brief Bad parameter detected. */
    FLOW_TIME_OUT      = -7, /*!< @brief No more resources. */
    FLOW_EOS           = -8  /*!< @brief Operation is in a terminated state. */
} FlowReturn;

/*
 * STRUCTURES AND OTHER TYPEDEFS
 */

/**
 * StreamEvent:
 * @param type: the EventType of the event
 * @param data: data associated with the event
 * @param format: format of the data
 *
 * StreamEvent.
 */
typedef struct _StreamEvent
{
    EventType type;          /*!< @brief Event type */
    StreamDataFormat format; /*!< @brief Event format */
    uint32_t data;           /*!< @brief Event data */
} StreamEvent;

struct _StreamPad;

/*!
 * Pad Activation Function Handler
 */
typedef uint8_t (*PadActivateFunc)(struct _StreamPad *pad, uint8_t active);

/*!
 * Pad Pull Function Handler
 */
typedef FlowReturn (*PadPullFunc)(struct _StreamPad *pad, StreamBuffer *buffer, uint32_t size, uint32_t offset);

/*!
 * Pad Chain Function Handler
 */
typedef FlowReturn (*PadChainFunc)(struct _StreamPad *pad, StreamBuffer *buffer);

/*!
 * Pad Event Function Handler
 */
typedef uint8_t (*PadEventFunc)(struct _StreamPad *pad, StreamEvent *event);

/*!
 * Pad Query Function Handler
 */
typedef uint8_t (*PadQueryFunc)(struct _StreamPad *pad, StreamQuery *query);

/*!
 * Pad Process Function Handler
 */
typedef int32_t (*PadProcessFunc)(struct _StreamPad *pad);

/*!
 * Pad Process Pre-check Function Handler
 */
typedef uint8_t (*PadPrecheckFunc)(struct _StreamPad *pad);

/*!
 *  Generic Pad Structure
 */
struct _StreamPad
{
    MEMBERS_PAD; /*!< @brief Pad members */
};
typedef struct _StreamPad StreamPad;

/*----------------------------------------------------------------------------*/

/*!
 * Source Pad Structure.
 */
struct _PadSrc
{
    MEMBERS_PAD;              /*!< @brief Pad members */
    PadPullFunc pull_handler; /*!< @brief Pad pull handler function */
};
typedef struct _PadSrc PadSrc;

/*!
 * Sink Pad Structure.
 */
struct _PadSink
{
    MEMBERS_PAD;                /*!< @brief Pad members */
    PadChainFunc chain_handler; /*!< @brief Pad chain handler function */
};
typedef struct _PadSink PadSink;

/*
 * GLOBAL VARIABLE DECLARATIONS
 */

/*
 * FUNCTION PROTOTYPES
 */

/**
 * @brief Pad initialization function
 *
 * @param pad pad object
 * @param parent parent element object
 * @param type pad type
 * @return FlowReturn
 */
FlowReturn init_pad(StreamPad *pad, void *parent, PadType type);
/**
 * @brief Pad deinitialization function
 *
 * @param pad pad object
 * @return FlowReturn
 */
FlowReturn deinit_pad(StreamPad *pad);
/**
 * @brief Push the data buffer to the peer pad
 *
 * @param pad pad object
 * @param buffer data buffer
 * @return FlowReturn
 */
FlowReturn pad_push(StreamPad *pad, StreamBuffer *buffer);
/**
 * @brief Pull the data buffer from the peer pad
 *
 * @param pad pad object
 * @param offset offset position to be read from
 * @param size size to be read
 * @param buffer data buffer to be filled
 * @return FlowReturn
 */
FlowReturn pad_pull_range(StreamPad *pad, uint32_t offset, uint32_t size, StreamBuffer *buffer);
/**
 * @brief Push event to the peer pad
 *
 * @param pad pad object
 * @param event event object
 * @return uint8_t
 */
uint8_t pad_push_event(StreamPad *pad, StreamEvent *event);
/**
 * @brief Send the event to the element where it will be handled by a handler function
 *
 * @param pad pad object
 * @param event event object
 * @return uint8_t
 */
uint8_t pad_send_event(StreamPad *pad, StreamEvent *event);
/**
 * @brief Call the appropriate pad activation function
 *
 * @param pad pad object
 * @param activate activation/deactivation flag
 * @return uint8_t
 */
uint8_t activate_pad(StreamPad *pad, uint8_t activate);
/**
 * @brief Pad activation/deactivation in pull mode
 *
 * @param pad pad object
 * @param active activation/deactivation flag
 * @return uint8_t
 */
uint8_t pad_activate_pull(StreamPad *pad, uint8_t active);
/**
 * @brief Pad activation/deactivation in push mode
 *
 * @param pad pad object
 * @param active activation/deactivation flag
 * @return uint8_t
 */
uint8_t pad_activate_push(StreamPad *pad, uint8_t active);

/* Create and free events */

/**
 * @brief Create flush start event
 *
 * @param event event object
 * @return FlowReturn
 */
FlowReturn event_create_flush_start(StreamEvent *event);
/**
 * @brief Create flush stop event
 *
 * @param event event object
 * @return FlowReturn
 */
FlowReturn event_create_flush_stop(StreamEvent *event);
/**
 * @brief Create end of stream event
 *
 * @param event event object
 * @return FlowReturn
 */
FlowReturn event_create_eos(StreamEvent *event);
/**
 * @brief Create new segment event
 *
 * @param event event object
 * @param format data format
 * @param data data object
 * @return FlowReturn
 */
FlowReturn event_create_new_segment(StreamEvent *event, StreamDataFormat format, uint32_t data);
/**
 * @brief Create seek event
 *
 * @param event event object
 * @param format data format
 * @param data data object
 * @return FlowReturn
 */
FlowReturn event_create_seek(StreamEvent *event, StreamDataFormat format, uint32_t data);
/**
 * @brief Send a query to a pad
 *
 * @param pad pad object
 * @param query query object
 * @return uint8_t
 */
uint8_t pad_query(StreamPad *pad, StreamQuery *query);
/**
 * @brief Create a query for element or pipeline
 *
 * @param query query object to be created
 * @param type type of information to be queried
 * @param format data format
 * @param data_type data type
 * @param data data object
 * @return FlowReturn
 */
FlowReturn query_create(
    StreamQuery *query, StreamInfoType type, StreamDataFormat format, StreamDataType data_type, StreamData *data);

#endif /* PAD_H */
/* End of file */
