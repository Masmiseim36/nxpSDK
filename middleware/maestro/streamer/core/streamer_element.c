/*
 * Copyright 2018-2023 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    streamer_element.c
 * @brief   Implementation of streamer element
 *
 */

/*
 * INCLUDE FILES
 */
#include "streamer.h"
#include "streamer_api.h"
#include "pipeline.h"
#include "streamer_element.h"

/* Elements */
#include "audio_src.h"
#include "audio_sink.h"

#ifdef STREAMER_ENABLE_FILESRC
#ifdef CASCFG_PLATFORM_FREERTOS
#include "file_src_freertos.h"
#else
#include "file_src.h"
#endif
#endif

#ifdef STREAMER_ENABLE_NETBUFSRC
#include "netbuf_src.h"
#endif

#ifdef STREAMER_ENABLE_DECODER
#include "decoder.h"
#endif

#ifdef STREAMER_ENABLE_FILE_SINK
#include "file_sink.h"
#endif
#ifdef STREAMER_ENABLE_MEM_SRC
#include "mem_src.h"
#endif
#ifdef STREAMER_ENABLE_MEM_SINK
#include "mem_sink.h"
#endif
#ifdef STREAMER_ENABLE_ENCODER
#include "encoder.h"
#endif
#if STREAMER_ENABLE_NETSRC
#include "enet_utils.h"
#include "net_src.h"
#endif
#ifdef STREAMER_ENABLE_VIT_SINK
#include "vit_sink.h"
#endif
#ifdef STREAMER_ENABLE_AUDIO_PROC
#include "audio_proc.h"
#endif

/*!
 * Element List table
 * This contains a list of all elements and their initialization functions.
 * There should be an entry for each element of the 'StreamElementType' enum.
 */
static const ElementInit element_list[] = {
/* Source elements (with one or more source and no sink pads) */
#ifdef STREAMER_ENABLE_FILESRC
    /*! TYPE_ELEMENT_FILE_SRC */
    {sizeof(ElementFileSrc), filesrc_init},
#else
    {0, NULL},
#endif

#ifdef STREAMER_ENABLE_MEM_SRC
    /*! TYPE_ELEMENT_MEM_SRC */
    {sizeof(ElementMemSrc), memsrc_init},
#else
    {0, NULL},
#endif

#ifdef STREAMER_ENABLE_NETBUFSRC
    /*! TYPE_ELEMENT_NETBUF_SRC */
    {sizeof(ElementNetbufSrc), netbufsrc_init},
#else
	{0, NULL},
#endif

    /*! TYPE_ELEMENT_AUDIO_SRC */
    {sizeof(ElementAudioSrc), audiosrc_init},

/* Sink elements (with one more sink and no source pads) */
#ifdef STREAMER_ENABLE_FILE_SINK
    /*! TYPE_ELEMENT_FILE_SINK */
    {sizeof(ElementFileSink), filesink_init_element},
#else
    {0, NULL},
#endif

#ifdef STREAMER_ENABLE_MEM_SINK
    /*! TYPE_ELEMENT_MEM_SINK */
    {sizeof(ElementMemSink), memsink_init_element},
#else
    {0, NULL},
#endif

    /*! TYPE_ELEMENT_AUDIO_SINK */
    {sizeof(ElementAudioSink), audiosink_init_element},

#ifdef STREAMER_ENABLE_DECODER
    /*! TYPE_ELEMENT_DECODER */
    {sizeof(ElementDecoder), decoder_init_element},
#else
    {0, NULL},
#endif


#ifdef STREAMER_ENABLE_ENCODER
    /*! TYPE_ELEMENT_ENCODER */
    {sizeof(ElementEncoder), encoder_init_element},
#else
    {0, NULL},
#endif

#ifdef STREAMER_ENABLE_VIT_SINK
    /*! TYPE_ELEMENT_VIT_SINK */
    {sizeof(ElementVitSink), vitsink_init_element},
#else
    {0, NULL},
#endif

#ifdef STREAMER_ENABLE_AUDIO_PROC
    /*! TYPE_ELEMENT_AUDIO_PROC */
    {sizeof(ElementAudioProc), audio_proc_init_element},
#else
    {0, NULL},
#endif
};

uint8_t activate_pads_element(StreamElement *element, uint8_t activate)
{
    int8_t i;
    uint8_t ret = true;

    STREAMER_FUNC_ENTER(DBG_CORE);

    /* Pads are activated from source pads to sinkpads */

    /* Activate the source pads first */
    for (i = 0; i < NUM_SRCS(ELEMENT_TYPE(element)); i++)
    {
        if (activate_pad(ELEMENT_SRC_PAD(element, i), activate) != (uint8_t) true)
            ret = false;
    }

    /* Activate the sink pads */
    for (i = 0; i < NUM_SINKS(ELEMENT_TYPE(element)); i++)
    {
        if (activate_pad(ELEMENT_SINK_PAD(element, i), activate) != (uint8_t) true)
            ret = false;
    }

    if (ret != (uint8_t) true)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return false;
    }

    STREAMER_FUNC_EXIT(DBG_CORE);

    return true;
}

int32_t send_msg_element(StreamElement *element, uint32_t message, uintptr_t data)
{
    int32_t ret = STREAM_OK;
    StreamMessage msg;

    STREAMER_FUNC_ENTER(DBG_CORE);

    CHK_ARGS(element == NULL, STREAM_ERR_INVALID_ARGS);

    msg.src_element = (ElementHandle)element;
    msg.message     = message;
    msg.data        = data;

    ret = send_msg_pipeline(ELEMENT_PARENT_PIPELINE(element), &msg);

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int32_t create_element(ElementHandle *handle, StreamElementType type, int8_t key)
{
    int32_t ret         = STREAM_OK;
    uint16_t element_id = ELEMENT_ID(type);

    STREAMER_FUNC_ENTER(DBG_CORE);

    /* Verify that this is a valid  element type */
    /* Check if the init_element is NULL, if NULL,
     * this kind of element is not supportted yet */
    if (element_id < TYPE_ELEMENT_LAST && element_list[element_id].init_element != NULL)
    {
        StreamElement *element;

        /* Allocate memory for an element type in element list table */
        element = OSA_MemoryAllocate(element_list[element_id].size);

        if (element)
        {
            int8_t i;

            /* Calloc initializes with most the members as NULL/0. Explicitly
             * init rest of the members.
             */
            ELEMENT_KEY(element)   = key;
            ELEMENT_STATE(element) = STATE_NULL;
            ELEMENT_TYPE(element)  = type;

            for (i = 0; i < NUM_SRCS(type); i++)
                init_pad(ELEMENT_SRC_PAD(element, i), element, TYPE_PAD_SRC);

            for (i = 0; i < NUM_SINKS(type); i++)
                init_pad(ELEMENT_SINK_PAD(element, i), element, TYPE_PAD_SINK);

            /* Return the allocated element type memory */
            *handle = (ElementHandle)element;

            /* Call the init element function and return the return value */
            ret = element_list[element_id].init_element(element);
        }
        /* Return no heap memory if not able to allocate. */
        else
            ret = STREAM_ERR_NO_MEM;
    }
    else
    {
        ret = STREAM_ERR_ELEMENT_NOT_FOUND;
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int32_t destroy_element(ElementHandle handle)
{
    StreamElement *element = (StreamElement *)handle;
    int32_t ret            = STREAM_ERR_INVALID_ARGS;

    STREAMER_FUNC_ENTER(DBG_CORE);

    if (element)
    {
        StreamElementType type = ELEMENT_TYPE(element);

        ret = STREAM_ERR_ELEMENT_NOT_FREE;

        /* Check if the element is still a part of pipeline
         * or it is linked to any other element then return
         * error.
         */
        if (ELEMENT_PARENT_PIPELINE(element) == NULL)
        {
            int8_t i;

            for (i = 0; i < NUM_SRCS(type); i++)
                deinit_pad(ELEMENT_SRC_PAD(element, i));

            for (i = 0; i < NUM_SINKS(type); i++)
                deinit_pad(ELEMENT_SINK_PAD(element, i));

            OSA_MemoryFree(element);
            element = NULL;
            ret     = STREAM_OK;
        }
        else
        {
            /* If succesfull then unlock the element */
        }
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int32_t get_element_info(ElementHandle handle, StreamElementType *ptype, int8_t *pkey)
{
    StreamElement *element = (StreamElement *)handle;
    int32_t ret            = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_CORE);

    CHK_ARGS(element == NULL || ptype == NULL || pkey == NULL, STREAM_ERR_INVALID_ARGS);

    *ptype = ELEMENT_TYPE(element);
    *pkey  = ELEMENT_KEY(element);

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int32_t link_elements(ElementHandle src_handle, int8_t src_pad_index, ElementHandle sink_handle, int8_t sink_pad_index)
{
    int32_t ret                 = STREAM_OK;
    StreamElement *src_element  = (StreamElement *)src_handle;
    StreamElement *sink_element = (StreamElement *)sink_handle;
    StreamPad *src_pad;
    StreamPad *sink_pad;

    STREAMER_FUNC_ENTER(DBG_CORE);

    /* Check:
     * Valid handles.
     * StreamPad indexes valid.
     * Both elements are part of same pipeline.
     */
    CHK_ARGS(src_element == NULL || sink_element == NULL ||
                 /* Valid pad index */
                 src_pad_index >= NUM_SRCS(ELEMENT_TYPE(src_element)) ||
                 sink_pad_index >= NUM_SINKS(ELEMENT_TYPE(sink_element)) ||

                 /* Should pe part of a have valid pipeline */
                 ELEMENT_PARENT_PIPELINE(src_element) == NULL || ELEMENT_PARENT_PIPELINE(sink_element) == NULL ||

                 /* Pipeline parent should be same */
                 (ELEMENT_PARENT_PIPELINE(src_element) != ELEMENT_PARENT_PIPELINE(sink_element)) ||

                 /* Level of sink element should be 1 more than the src */
                 ((ELEMENT_DEPTH(src_element)) >= ELEMENT_DEPTH(sink_element)),

             STREAM_ERR_INVALID_ARGS);

    /* Get the source and sink pads of the elements */
    src_pad  = ELEMENT_SRC_PAD(src_element, src_pad_index);
    sink_pad = ELEMENT_SINK_PAD(sink_element, sink_pad_index);

    /* Check:
     * Pads not already linked.
     */
    CHK_ARGS(PAD_PEER(src_pad) != NULL || PAD_PEER(sink_pad) != NULL, STREAM_ERR_ELEMENT_LINKED);

    /* Connect the pads */
    PAD_PEER(src_pad)  = sink_pad;
    PAD_PEER(sink_pad) = src_pad;

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int32_t unlink_elements(ElementHandle src_handle,
                        int8_t src_pad_index,
                        ElementHandle sink_handle,
                        int8_t sink_pad_index)
{
    int32_t ret                 = STREAM_OK;
    StreamElement *src_element  = (StreamElement *)src_handle;
    StreamElement *sink_element = (StreamElement *)sink_handle;
    StreamPad *src_pad;
    StreamPad *sink_pad;

    STREAMER_FUNC_ENTER(DBG_CORE);

    /* Check:
     * Valid handles.
     * StreamPad indexes valid.
     * Both elements are part of same pipeline.
     */
    CHK_ARGS(src_element == NULL || sink_element == NULL ||

                 /* Valid pad index */
                 src_pad_index >= NUM_SRCS(ELEMENT_TYPE(src_element)) ||
                 sink_pad_index >= NUM_SINKS(ELEMENT_TYPE(sink_element)) ||

                 /* Should pe part of a have valid pipeline */
                 ELEMENT_PARENT_PIPELINE(src_element) == NULL || ELEMENT_PARENT_PIPELINE(sink_element) == NULL ||

                 /* Pipeline parent should be same */
                 ELEMENT_PARENT_PIPELINE(src_element) != ELEMENT_PARENT_PIPELINE(sink_element) ||

                 /* Level of sink element should be 1 more than the src */
                 ((ELEMENT_DEPTH(src_element)) >= ELEMENT_DEPTH(sink_element)),

             STREAM_ERR_INVALID_ARGS);

    /* Get the source and sink pads of the elements */
    src_pad  = ELEMENT_SRC_PAD(src_element, src_pad_index);
    sink_pad = ELEMENT_SINK_PAD(sink_element, sink_pad_index);

    /* Check:
     * Pads linked.
     */
    CHK_ARGS(PAD_PEER(src_pad) != sink_pad || PAD_PEER(sink_pad) != src_pad,

             STREAM_ERR_ELEMENT_PADS_NOT_LINKED);

    /* Disconnect the pads */
    PAD_PEER(src_pad)  = NULL;
    PAD_PEER(sink_pad) = NULL;

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int32_t element_get_property(ElementHandle element_hdl, uint16_t prop, uint64_t *val_ptr)
{
    return ((StreamElement *)element_hdl)->get_property((StreamElement *)element_hdl, prop, val_ptr);
}

int32_t element_set_property(ElementHandle element_hdl, uint16_t prop, uintptr_t val)
{
    return ((StreamElement *)element_hdl)->set_property((StreamElement *)element_hdl, prop, val);
}

/*----------------------------------------------------------------------------*/
