/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    pipeline.c
 * @brief   The implementation for the Af Streamer pipeline.
 */

/*
 * INCLUDE FILES
 */
#include <string.h>

#include "audio_cfg.h"
#include "streamer_api.h"
#include "streamer.h"
#include "pipeline.h"
#include "streamer_element.h"

/*
 * GLOBAL CONSTANTS
 */

/*
 * GLOBAL VARIABLES
 */

/*
 * LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 */

/*
 * LOCAL MACROS
 */

/*
 * LOCAL CONSTANTS
 */

/*
 * LOCAL VARIABLES
 */

/*
 * LOCAL FUNCTION PROTOTYPES
 */

/**
 * @brief find_element_sibling_list
 *
 * @details Finds an element within a specified level of the pipeline.  This
 *          functions iterates through the siblings at a specified level pointed
 *          to by 'list_head' and tries to find the element matching the values
 *          provided for Type and Key.
 *
 * @param list_head Pointer to element at head of a pipeline level.
 * @param type      Type of element
 * @param key       Unique Key value to identify the element
 * @return StreamElement* Pointer to element
 */
static StreamElement *find_element_sibling_list(StreamElement *list_head, StreamElementType type, int8_t key);
/**
 * @brief   Pipeline Path Blocked?
 *
 * @details Check to see if the pipeline path is blocked starting with the
 *          element passed in.
 *
 * @param   element Pointer to an element in the pipeline marking the start of
 *          the current data flow path.
 *
 * @returns Pipeline Path is Blocked?
 *
 * @retval  0 Path is NOT BLOCKED
 * @retval  1 Path is blocked
 *
 */
static int pipeline_path_blocked(StreamElement *element);

/*
 * GLOBAL FUNCTIONS
 */

int32_t create_pipeline(PipelineHandle *handle, unsigned int index, StreamPipelineType type, osa_msgq_handle_t *app_mq)
{
    Pipeline *pipeline = NULL;
    int32_t ret        = STREAM_OK;
    int8_t i;

    STREAMER_FUNC_ENTER(DBG_CORE);

    if (NULL == handle)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return STREAM_ERR_INVALID_ARGS;
    }

    /* Allocate memory for the type of element from the element list table */
    pipeline = OSA_MemoryAllocate(sizeof(Pipeline));
    if (NULL == pipeline)
    {
        *handle = (PipelineHandle)NULL;
        STREAMER_FUNC_EXIT(DBG_CORE);
        return STREAM_ERR_NO_MEM;
    }

    pipeline->index  = index;
    pipeline->app_mq = app_mq;

    PIPELINE_STATE(pipeline) = STATE_NULL;

    for (i = 0; i < MAX_ELEMENT_LEVEL; i++)
    {
        StreamElement *head = PIPELINE_LEVEL_HEAD(pipeline, i);

        ELEMENT_KEY(head)   = 0;
        ELEMENT_STATE(head) = STATE_NULL;
    }

    pipeline->type = type;
    /* return pipeline handle */
    *handle = (PipelineHandle)pipeline;

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int32_t destroy_pipeline(PipelineHandle handle)
{
    Pipeline *pipeline = (Pipeline *)handle;

    STREAMER_FUNC_ENTER(DBG_CORE);

    CHK_ARGS(pipeline == NULL, STREAM_ERR_INVALID_ARGS);

    /* NOTE: Assuming pipeline does not contain any elements */

    /* De-allocate memory for the pipeline */
    OSA_MemoryFree(pipeline);
    pipeline = NULL;

    STREAMER_FUNC_EXIT(DBG_CORE);
    return STREAM_OK;
}

PipelineState get_state_pipeline(PipelineHandle handle)
{
    PipelineState state = STATE_NULL;
    Pipeline *pipeline  = (Pipeline *)handle;

    STREAMER_FUNC_ENTER(DBG_CORE);

    /* If any of the arguments are invalid */
    if (pipeline)
    {
        state = PIPELINE_STATE(pipeline);
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return state;
}

int32_t set_state_pipeline(PipelineHandle handle, PipelineState state)
{
    Pipeline *pipeline = (Pipeline *)handle;
    int32_t ret        = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_CORE);

    /* If any of the arguments are invalid */
    CHK_ARGS(pipeline == NULL, STREAM_ERR_INVALID_ARGS);

    STREAMER_LOG_DEBUG(DBG_CORE, "[Pipeline] set_state_pipeline %d\n", state);

    if (PIPELINE_STATE(pipeline) != state)
    {
        int16_t level;

        /* Change state from the last level to the first. This means that
         * elements will change state from the sink level to the source level.
         */
        for (level = MAX_ELEMENT_LEVEL - 1; level >= 0; level--)
        {
            StreamElement *element = PIPELINE_LEVEL_HEAD(pipeline, level);
            element                = ELEMENT_SIBLING(element);

            while (element != NULL)
            {
                PipelineState current_state;
                PipelineState next_state;
                int8_t increment = 1;

                current_state = ELEMENT_STATE(element);

                /* If the new state is downhill state then decrement to reach
                 * the state.
                 */
                if (current_state > state)
                    increment = -1;

                while (current_state != state)
                {
                    next_state = (PipelineState)(((uint32_t)current_state + increment) & PIPELINE_STATE_MASK);

                    /* Activate or deactivate the pad if we are making a
                     * transitions from READY to PAUSE or vice versa
                     * respectively.
                     */
                    switch (STATE_TRANSITION(current_state, next_state))
                    {
                        case STATE_CHANGE_READY_TO_PAUSED:
                            if (activate_pads_element(element, true) != (uint8_t) true)
                            {
                                ret                      = STREAM_ERR_GENERAL;
                                PIPELINE_STATE(pipeline) = state;
                                ELEMENT_STATE(element)   = next_state;
                                set_state_pipeline((PipelineHandle)pipeline, STATE_NULL);
                                goto Error;
                            }
                            break;

                        case STATE_CHANGE_PAUSED_TO_READY:
                            if (activate_pads_element(element, false) != (uint8_t) true)
                            {
                                ret                      = STREAM_ERR_GENERAL;
                                PIPELINE_STATE(pipeline) = state;
                                ELEMENT_STATE(element)   = next_state;
                                set_state_pipeline((PipelineHandle)pipeline, STATE_NULL);
                                goto Error;
                            }
                            break;

                        case STATE_CHANGE_PAUSED_TO_PLAYING:
                        case STATE_CHANGE_PLAYING_TO_PAUSED:
                        case STATE_CHANGE_NULL_TO_READY:
                        case STATE_CHANGE_READY_TO_NULL:
                        default:
                            break;
                    }

                    /* Reset the return type so that the state is changed
                     * even if the change_state function pointer is NULL.
                     **/
                    ret = STREAM_OK;

                    if (element->change_state != NULL)
                    {
                        ret = element->change_state(element, next_state);

                        if (STREAM_OK != ret)
                        {
                            ret                      = STREAM_ERR_GENERAL;
                            PIPELINE_STATE(pipeline) = state;
                            ELEMENT_STATE(element)   = next_state;
                            set_state_pipeline((PipelineHandle)pipeline, STATE_NULL);
                            goto Error;
                        }
                    }

                    /* Set the element state to the next one */
                    ELEMENT_STATE(element) = next_state;

                    current_state = next_state;
                }

                /* Go to the next element in the same level. */
                element = ELEMENT_SIBLING(element);
            }
        }

        /* All elements in the required state. Set the pipeline state to the
         * required value.
         */
        PIPELINE_STATE(pipeline) = state;
    }

Error:
    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int32_t seek_pipeline(PipelineHandle handle, uint32_t time_msec)
{
    StreamEvent seek;
    StreamReturnType ret   = STREAM_OK;
    Pipeline *pipeline     = (Pipeline *)handle;
    StreamElement *element = NULL;
    int8_t level;
    uint8_t seeked = true;

    STREAMER_FUNC_ENTER(DBG_CORE);

    /* If any of the arguments are invalid */
    CHK_ARGS(pipeline == NULL, STREAM_ERR_INVALID_ARGS);

    /* PipelineState should either be paused or playing for seek */
    if (PIPELINE_STATE(pipeline) < STATE_PAUSED)
    {
        ret = STREAM_ERR_NOT_SEEKABLE;
        goto Error;
    }

    /* Find the sink element level head */
    for (level = 0; level < MAX_ELEMENT_LEVEL; level++)
    {
        StreamElement *head = PIPELINE_LEVEL_HEAD(pipeline, level);
        head                = ELEMENT_SIBLING(head);

        /* If there a elements in this level and the head is a sink element then
         * then it is assumed that all the siblings of this list will also be
         * sink elements.
         */
        if (head != NULL && IS_SINK_ELEMENT(ELEMENT_TYPE(head)))
        {
            element = head;
            break;
        }
    }

    /* Create seek event with time offset in msec */
    event_create_seek(&seek, DATA_FORMAT_TIME, time_msec);

    /* Push seek events in all sink elements */
    while (element != NULL && (uint8_t) true == seeked)
    {
        StreamPad *pad = ELEMENT_SINK_PAD(element, 0);

        /* Send the query to sink pad of the sink element */
        seeked = pad_push_event(pad, &seek);

        /* Go to the next element in the same level. */
        element = ELEMENT_SIBLING(element);
    }

    /* If query not succesful */
    if ((uint8_t) false == seeked)
        ret = STREAM_ERR_NOT_SEEKABLE;
    else if ((uint8_t)ERRCODE_NOT_SEEKABLE == seeked)
        ret = (StreamReturnType)ERRCODE_NOT_SEEKABLE;

Error:
    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int32_t seek_relative(PipelineHandle handle, int32_t time_msec)
{
    StreamData data;
    int32_t new_time;
    int32_t ret;
    int32_t duration = 0;

    STREAMER_FUNC_ENTER(DBG_CORE);

    ret = query_info_pipeline(handle, INFO_DURATION, &data);
    if (STREAM_OK != ret)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return ret;
    }
    duration = data.value32s;

    ret = query_info_pipeline(handle, INFO_POSITION, &data);

    if (STREAM_OK == ret)
    {
        new_time = ((int32_t)data.value32u + time_msec);
        if (duration < new_time)
        {
            new_time = duration - 1000;
        }
        if (new_time < 0)
        {
            new_time = 0;
        }
        ret = seek_pipeline(handle, new_time);
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int32_t query_info_pipeline(PipelineHandle handle, StreamInfoType info_type, StreamData *data)
{
    StreamQuery query;
    StreamDataFormat format = DATA_FORMAT_BYTES;
    StreamDataType type     = DATA_TYPE_UINT32;
    StreamReturnType ret    = STREAM_OK;
    Pipeline *pipeline      = (Pipeline *)handle;
    StreamElement *element  = NULL;
    int8_t level;
    uint8_t done = false;

    STREAMER_FUNC_ENTER(DBG_CORE);

    /* If any of the arguments are invalid */
    CHK_ARGS(pipeline == NULL, STREAM_ERR_INVALID_ARGS);

    switch (info_type)
    {
        case INFO_AUDIO_BITRATE:
        case INFO_AUDIO_CHANNELS:
        case INFO_AUDIO_SAMPLERATE:
        case INFO_AUDIO_TYPE:
            break;

        case INFO_TRACK:
            type = DATA_TYPE_UINT8;
            break;

        case INFO_TIME_SEEKABLE:
            format = DATA_FORMAT_TIME;
            type   = DATA_TYPE_bool;
            break;

        case INFO_BYTE_SEEKABLE:
            format = DATA_FORMAT_BYTES;
            type   = DATA_TYPE_bool;
            break;

        case INFO_DURATION:
        case INFO_POSITION:
            format = DATA_FORMAT_TIME;
            break;

        case INFO_ALBUM:
        case INFO_ARTIST:
        case INFO_TITLE:
        case INFO_GENRE:
        case INFO_YEAR:
            type = DATA_TYPE_STRING;
            break;

        case INFO_ALBUMART_IMAGE:
            type = DATA_TYPE_IMAGE;
            break;

        case INFO_META_INFO_UPDATE:
            /* don't need query this dummy info, just return back the query type */
            STREAMER_FUNC_EXIT(DBG_CORE);
            return ret;

        case INFO_SIZE:
            type = DATA_TYPE_UINT32;
            break;

        case INFO_BUFF_OCC:
            type = DATA_TYPE_UINT32;
            break;

        default:
            ret = STREAM_ERR_INVALID_ARGS;
            goto Error;
    }

    query_create(&query, info_type, format, type, data);

    /* PipelineState should either be paused or playing for query */
    if (PIPELINE_STATE(pipeline) < STATE_PAUSED)
    {
        ret = STREAM_ERR_INFO_ABSENT;
        goto Error;
    }

    /* Find the sink element level head */
    for (level = 0; level < MAX_ELEMENT_LEVEL; level++)
    {
        StreamElement *head = PIPELINE_LEVEL_HEAD(pipeline, level);
        head                = ELEMENT_SIBLING(head);

        /* If there a elements in this level and the head is a sink element then
         * then it is assumed that all the siblings of this list will also be
         * sink elements.
         */
        if (head != NULL && IS_SINK_ELEMENT(ELEMENT_TYPE(head)))
        {
            element = head;
            break;
        }
    }

    while (element != NULL && done == (uint8_t) false)
    {
        StreamPad *pad = ELEMENT_SINK_PAD(element, 0);

        /* Send the query to sink pad of the sink element */
        done = pad_query(pad, &query);

        /* Go to the next element in the same level. */
        element = ELEMENT_SIBLING(element);
    }

    /* If query not succesful */
    if ((uint8_t) false == done)
        ret = STREAM_ERR_INFO_ABSENT;

Error:
    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int32_t add_element_pipeline(PipelineHandle pipeline_handle, ElementHandle element_handle, int8_t level)
{
    Pipeline *pipeline     = (Pipeline *)pipeline_handle;
    StreamElement *element = (StreamElement *)element_handle;
    StreamElement *list_head;
    int32_t ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_CORE);

    /* If any of the arguments are invalid */
    CHK_ARGS(pipeline == NULL || element == NULL || level >= MAX_ELEMENT_LEVEL, STREAM_ERR_INVALID_ARGS);

    if (ELEMENT_PARENT(element) == NULL)
    {
        /* Init the element with default values */
        ELEMENT_SIBLING(element) = NULL;
        ELEMENT_DEPTH(element)   = level;
        ELEMENT_STATE(element)   = STATE_NULL;

        /* First link it to the main link list of all elements */
        list_head = PIPELINE_LEVEL_HEAD(pipeline, level);

        /* Reach the end of the list */
        while (ELEMENT_SIBLING(list_head) != NULL)
            list_head = ELEMENT_SIBLING(list_head);

        /* Connect to the last element */
        ELEMENT_SIBLING(list_head) = element;

        /* Set the parent pipeline */
        ELEMENT_PARENT(element) = pipeline;
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int32_t remove_element_pipeline(PipelineHandle pipeline_handle, ElementHandle element_handle)
{
    Pipeline *pipeline     = (Pipeline *)pipeline_handle;
    StreamElement *element = (StreamElement *)element_handle;
    int32_t ret            = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_CORE);

    /* If any of the arguments are invalid */
    CHK_ARGS(pipeline == NULL || element == NULL, STREAM_ERR_INVALID_ARGS);

    /* If element has been added into the pipeline */
    if (ELEMENT_PARENT_PIPELINE(element) == pipeline)
    {
        StreamElement *list_head = NULL;
        int8_t level             = ELEMENT_DEPTH(element);

        /* Note: should check if the element is not linked with any other
         * element.
         */

        /* Get the level link list head */
        list_head = PIPELINE_LEVEL_HEAD(pipeline, level);

        /* Reach the end of the list */
        while ((ELEMENT_SIBLING(list_head) != element) && (ELEMENT_SIBLING(list_head) != NULL))
            list_head = ELEMENT_SIBLING(list_head);

        /* If we have reached the end of list and the tail link is not
         * the element we are searching for then, return with an error.
         */
        if (ELEMENT_SIBLING(list_head) == NULL)
            ret = STREAM_ERR_ELEMENT_NOT_FOUND;
        else
        {
            /* Connect the previous element to the next element */
            ELEMENT_SIBLING(list_head) = ELEMENT_SIBLING(element);

            /* Reset the element links */
            ELEMENT_PARENT(element)  = NULL;
            ELEMENT_SIBLING(element) = NULL;
            ELEMENT_DEPTH(element)   = 0;
        }
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int32_t get_element_pipeline(PipelineHandle pipeline_handle, StreamElementType type, int8_t key, ElementHandle *handle)
{
    Pipeline *pipeline     = (Pipeline *)pipeline_handle;
    StreamElement *element = NULL;
    int32_t ret            = STREAM_ERR_INVALID_ARGS;
    int8_t level;

    STREAMER_FUNC_ENTER(DBG_CORE);

    /* If any of the arguments are invalid */
    CHK_ARGS(pipeline == NULL, STREAM_ERR_INVALID_ARGS);

    /* Start from the first level and search the sibling list. */
    for (level = 0; level < MAX_ELEMENT_LEVEL && element == NULL; level++)
    {
        StreamElement *list_head = PIPELINE_LEVEL_HEAD(pipeline, level);

        /* Get the first element from the list */
        list_head = ELEMENT_SIBLING(list_head);

        /* If elements at this level then search through the list */
        if (list_head)
            element = find_element_sibling_list(list_head, type, key);
    }

    /* If found then return handle as the pointer to the element. */
    if (element)
    {
        *handle = (ElementHandle)element;
        ret     = STREAM_OK;
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int32_t send_msg_pipeline(Pipeline *pipeline, StreamMessage *msg)
{
    STREAMER_MSG_T streamer_msg = {0};
    int32_t ret                 = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_CORE);

    /* If any of the arguments are invalid */
    CHK_ARGS(pipeline == NULL || msg == NULL, STREAM_ERR_INVALID_ARGS);

    streamer_msg.pipeline_index = pipeline->index;
    streamer_msg.state          = pipeline->state;
    streamer_msg.pipeline_type  = pipeline->type;
    switch (msg->message)
    {
        case MSG_EOS:
            if (pipeline->repeat)
            {
                seek_pipeline((PipelineHandle)pipeline, 0);
            }
            else
            {
                if (NULL != pipeline->app_mq)
                {
                    streamer_msg.id = STREAM_MSG_EOS;
                    ret             = OSA_MsgQPut(pipeline->app_mq, (char *)&streamer_msg);
                }
            }
            break;
        case MSG_INFO_AUDIO_BITRATE:
            pipeline->track_info.bit_rate = msg->data;
            break;
        case MSG_INFO_AUDIO_CHANNELS:
            pipeline->track_info.channels = msg->data;
            break;
        case MSG_INFO_AUDIO_SAMPLERATE:
            pipeline->track_info.sample_rate = msg->data;
            break;
        case MSG_INFO_AUDIO_TYPE:
            pipeline->track_info.audio_type = msg->data;
            break;
        case MSG_INFO_CODEC_INFO:
            strncpy(pipeline->track_info.codec_info, (char *)msg->data, CODEC_INFO_LEN - 1);
            break;
        case MSG_INFO_DURATION:
            if (msg->data <= 0)
            {
                STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "[Pipeline] Duration is wrong %d\n", msg->data);
            }
            else
            {
                pipeline->track_info.duration = msg->data;
                if (NULL != pipeline->app_mq)
                {
                    streamer_msg.id         = STREAM_MSG_UPDATE_DURATION;
                    streamer_msg.event_data = msg->data;
                    ret                     = OSA_MsgQPut(pipeline->app_mq, (char *)&streamer_msg);
                }
            }
            break;
        case MSG_INFO_ALBUM:
            strncpy(pipeline->track_info.album, (char *)msg->data, CASCFG_TRACK_ALBUM_LEN - 1);
            break;
        case MSG_INFO_ARTIST:
            strncpy(pipeline->track_info.artist, (char *)msg->data, CASCFG_TRACK_ARTIST_LEN - 1);
            break;
        case MSG_INFO_TITLE:
            strncpy(pipeline->track_info.title, (char *)msg->data, CASCFG_TRACK_TITLE_LEN - 1);
            break;
        case MSG_INFO_GENRE:
            strncpy(pipeline->track_info.genre, (char *)msg->data, CASCFG_TRACK_GENRE_LEN - 1);
            break;
        case MSG_INFO_YEAR:
            strncpy(pipeline->track_info.year, (char *)msg->data, CASCFG_TRACK_YEAR_LEN - 1);
            break;
        case MSG_INFO_TRACK:
            pipeline->track_info.track_number = msg->data;
            break;
        case MSG_INFO_ALBUMART:
        {
            StreamImageType *image                    = (StreamImageType *)msg->data;
            pipeline->track_info.album_art.data       = (char *)image->image_buffer;
            pipeline->track_info.album_art.height     = image->height;
            pipeline->track_info.album_art.width      = image->width;
            pipeline->track_info.album_art.image_size = image->image_size;
            switch (image->bits_per_pixel)
            {
                case 1:
                    pipeline->track_info.album_art.format = IMG_GRAYSCALE;
                    break;
                case 15:
                    pipeline->track_info.album_art.format = IMG_RGB_555;
                    break;
                case 16:
                    pipeline->track_info.album_art.format = IMG_RGB_565;
                    break;
                case 18:
                    pipeline->track_info.album_art.format = IMG_RGB_666;
                    break;
                case 24:
                    pipeline->track_info.album_art.format = IMG_RGB_888;
                    break;
                default:
                    break;
            }
            break;
        }
        case MSG_INFO_POSITION:
            if (NULL != pipeline->app_mq)
            {
                streamer_msg.id         = STREAM_MSG_UPDATE_POSITION;
                streamer_msg.event_data = msg->data;
                ret                     = OSA_MsgQPut(pipeline->app_mq, (char *)&streamer_msg);
            }
            break;
        case MSG_STREAM_INFO_UPDATED:
        case MSG_META_INFO_UPDATED:
            if (NULL != pipeline->app_mq)
            {
                streamer_msg.id       = STREAM_MSG_UPDATE_TRACK_INFO;
                streamer_msg.event_id = msg->message;
                ret                   = OSA_MsgQPut(pipeline->app_mq, (char *)&streamer_msg);
            }
            break;
        case MSG_STREAM_ERROR:
            if (NULL != pipeline->app_mq)
            {
                streamer_msg.id         = STREAM_MSG_ERROR;
                streamer_msg.event_data = msg->data;
                ret                     = OSA_MsgQPut(pipeline->app_mq, (char *)&streamer_msg);
            }
            break;
        default:
            STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INVALID_ARGUMENT, "[Pipeline] Unknown pipeline message %d\n",
                             msg->message);
            break;
    }

    if (ERRCODE_NO_ERROR != ret)
        STREAMER_LOG_CATA(DBG_CORE, ERRCODE_MSG_FAILURE, "[Pipeline] Send MSG failed msg=%x err=%d\n", streamer_msg.id,
                          ret);

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int32_t process_pipeline(PipelineHandle handle)
{
    Pipeline *pipeline = (Pipeline *)handle;
    int32_t ret        = STREAM_OK;
    PadSrc *src_pad;
    PadSink *sink_pad;

    STREAMER_FUNC_ENTER(DBG_CORE);

    /* If any of the arguments are invalid */
    CHK_ARGS(pipeline == NULL, STREAM_ERR_INVALID_ARGS);

    if (PIPELINE_STATE(pipeline) != STATE_NULL)
    {
        int16_t level;

        for (level = 0; level < MAX_ELEMENT_LEVEL; level++)
        {
            StreamElement *element = PIPELINE_LEVEL_HEAD(pipeline, level);
            element                = ELEMENT_SIBLING(element);

            if (NULL != element)
            {
                if (NUM_SRCS(ELEMENT_TYPE(element)))
                {
                    src_pad = (PadSrc *)ELEMENT_SRC_PAD(element, 0);
                    if ((NULL != src_pad->process_handler) && (src_pad->peer) &&
                        (SCHEDULING_PUSH == src_pad->scheduling) && ((uint8_t) false == pipeline_path_blocked(element)))
                    {
                        ret = src_pad->process_handler((StreamPad *)src_pad);
                        if (ret != STREAM_OK)
                        {
                            goto Error;
                        }
                    }
                }

                if (NUM_SINKS(ELEMENT_TYPE(element)))
                {
                    sink_pad = (PadSink *)ELEMENT_SINK_PAD(element, 0);
                    if ((NULL != sink_pad->process_handler) && (sink_pad->peer) &&
                        (SCHEDULING_PULL == sink_pad->peer->scheduling) &&
                        ((uint8_t) false == pipeline_path_blocked(element)))
                    {
                        ret = sink_pad->process_handler((StreamPad *)sink_pad);
                        if (ret != STREAM_OK)
                        {
                            goto Error;
                        }
                    }
                }
            }
        }
    }
Error:
    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

/*
 * LOCAL FUNCTIONS
 */

static int pipeline_path_blocked(StreamElement *element)
{
    StreamPad *peer_sink_pad;
    StreamPad *src_pad;
    StreamPad *sink_pad;

    STREAMER_FUNC_ENTER(DBG_CORE);

    if (NUM_SRCS(ELEMENT_TYPE(element)))
    {
        src_pad = ELEMENT_SRC_PAD(element, 0);
    }
    else
    {
        /* If there are no sources, there must be a sink pad */
        sink_pad = ELEMENT_SINK_PAD(element, 0);
        src_pad  = sink_pad->peer;

        if (!src_pad)
        {
            STREAMER_FUNC_EXIT(DBG_CORE);
            return true; /* No source pad on sink peer! */
        }
        else
        {
            element = PAD_PARENT_ELEMENT(src_pad);
        }
    }

    if (src_pad->process_precheck)
    {
        if ((uint8_t) false == src_pad->process_precheck(src_pad))
        {
            STREAMER_FUNC_EXIT(DBG_CORE);
            return true;
        }
    }

    /*
     * Enter a loop where we check each sink pad downstream to see if a pad
     * is currently blocking.  The loop ends when we encounter a blocking pad
     * or we reach a queue or sink element.
     */
    do
    {
        peer_sink_pad = PAD_PEER(ELEMENT_SRC_PAD(element, 0));

        /* Check if Pad is blocked on next downstream element */
        if (peer_sink_pad->process_precheck)
        {
            if ((uint8_t) false == peer_sink_pad->process_precheck(peer_sink_pad))
            {
                STREAMER_FUNC_EXIT(DBG_CORE);
                return true;
            }
        }

        /* Update element to point to next downstream element */
        element = PAD_PARENT_ELEMENT(peer_sink_pad);

        /*
         * Check if element is a sink or queue. If it is we have reached the
         * end of the path so the path is not blocked.  If element is of another
         * type then check to see if the Source Pad is currently blocked.
         */
        if (IS_SINK_ELEMENT(ELEMENT_TYPE(element)))
        {
            STREAMER_FUNC_EXIT(DBG_CORE);
            return false;
        }
        else
        {
            if (NUM_SRCS(ELEMENT_TYPE(element)))
            {
                src_pad = (ELEMENT_SRC_PAD(element, 0));
                if (src_pad->process_precheck)
                {
                    if ((uint8_t) false == src_pad->process_precheck(src_pad))
                    {
                        STREAMER_FUNC_EXIT(DBG_CORE);
                        return true;
                    }
                }
            }
            else
            {
                STREAMER_FUNC_EXIT(DBG_CORE);
                return true; /* No source pad on initial element! */
            }
        }
    } while (1);
}

static StreamElement *find_element_sibling_list(StreamElement *list_head, StreamElementType type, int8_t key)
{
    STREAMER_FUNC_ENTER(DBG_CORE);

    while (list_head != NULL)
    {
        if (ELEMENT_KEY(list_head) == key && ELEMENT_TYPE(list_head) == type)
        {
            break;
        }
        else
        {
            list_head = ELEMENT_SIBLING(list_head);
        }
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return list_head;
}

int32_t set_repeat_pipeline(PipelineHandle handle, bool repeat)
{
    Pipeline *pipeline = (Pipeline *)handle;

    STREAMER_FUNC_ENTER(DBG_CORE);

    /* If any of the arguments are invalid */
    if (pipeline == NULL)
        return STREAM_ERR_INVALID_ARGS;

    pipeline->repeat = repeat;

    STREAMER_FUNC_EXIT(DBG_CORE);
    return STREAM_OK;
}

bool get_repeat_pipeline(PipelineHandle handle)
{
    Pipeline *pipeline = (Pipeline *)handle;

    STREAMER_FUNC_ENTER(DBG_CORE);

    /* If any of the arguments are invalid */
    if (pipeline == NULL)
        return STREAM_ERR_INVALID_ARGS;

    STREAMER_FUNC_EXIT(DBG_CORE);
    return pipeline->repeat;
}

void clear_pipeline_trackinfo(PipelineHandle handle)
{
    Pipeline *pipeline = (Pipeline *)handle;

    STREAMER_FUNC_ENTER(DBG_CORE);

    memset((void *)&pipeline->track_info, 0, sizeof(TrackInfo));

    STREAMER_FUNC_EXIT(DBG_CORE);
}
/*----------------------------------------------------------------------------*/
