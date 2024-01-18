/*
 * Copyright 2018-2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*!
 * @file    streamer_msg.c
 * @brief   Implementation of streamer message handlers
 *
 */

/*
 * INCLUDE FILES
 */

#include <string.h>
#include "pipeline.h"
#include "file_src_freertos.h"
#include "streamer_element_properties.h"

#ifdef STREAMER_ENABLE_MEM_SRC
#include "mem_src.h"
#endif

/*!
 * @ingroup streamer_msg
 * @{
 */

/**
 * @brief Element not used index
 *
 */
#define ELEMENT_NOT_USED (-1)

/**
 * @brief Element index and property
 *
 */
typedef struct
{
    /** @brief Element property */
    int16_t property;
    /** @brief Element index */
    uint16_t element_index;
} ElementPropertyLookup;

/*!
 * This table is used to indentify which element in the pipeline a property
 * is associated with. Properties can be used to configure the individual
 * pipeline elements.
 */
static ElementPropertyLookup property_lookup_table[] = {{PROP_MICROPHONE_MASK, ELEMENT_MICROPHONE_INDEX},
                                                        {PROP_FILESRC_MASK, ELEMENT_FILE_SRC_INDEX},
                                                        {PROP_MEMSRC_MASK, ELEMENT_MEM_SRC_INDEX},
                                                        {PROP_NETBUFSRC_MASK, ELEMENT_NETBUF_SRC_INDEX},
                                                        {PROP_DECODER_MASK, ELEMENT_DECODER_INDEX},
                                                        {PROP_SPEAKER_MASK, ELEMENT_SPEAKER_INDEX},
                                                        {PROP_FILESINK_MASK, ELEMENT_FILE_SINK_INDEX},
                                                        {PROP_MEMSINK_MASK, ELEMENT_MEM_SINK_INDEX},
                                                        {PROP_VITSINK_MASK, ELEMENT_VIT_INDEX},
                                                        {PROP_VIT_PROC_MASK, ELEMENT_VIT_PROC_INDEX},
                                                        {PROP_VOICESEEKER_MASK, ELEMENT_VOICESEEKER_INDEX},
                                                        {PROP_SRC_MASK, ELEMENT_SRC_INDEX},
                                                        {PROP_ENCODER_MASK, ELEMENT_ENCODER_INDEX},
                                                        {PROP_USB_SRC_MASK, ELEMENT_USB_SRC_INDEX},
                                                        {PROP_USB_SINK_MASK, ELEMENT_USB_SINK_INDEX},
                                                        {PROP_ASRC_MASK, ELEMENT_ASRC_INDEX}};

/*
 * GLOBAL FUNCTIONS
 */

/*!
 * @brief   Streamer Get Track Inforamtion from a pipeline
 * @details Handles request to get track information from a pipeline object.
 *
 * @param   [in] streamer Pointer to streamer structure
 * @param   [in] msg_data Pointer to Message Data
 * @returns Error Status
 *
 * @retval STREAM_OK                    State retrieved successfully.
 * @retval STREAM_ERR_INVALID_ARGS      Invalid pipeline object
 */
int streamer_msg_get_track_info(STREAMER_T *streamer, void *msg_data)
{
    STREAMER_MSG_T *streamer_msg;
    int ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_CORE);
    streamer_msg = (STREAMER_MSG_T *)msg_data;

    if (NULL == streamer_msg->get_value)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return ERRCODE_GENERAL_ERROR;
    }

    if (streamer_msg->pipeline_index >= MAX_PIPELINES)
    {
        ret = STREAM_ERR_INVALID_ARGS;
    }
    else if (0 == streamer->pipes[streamer_msg->pipeline_index])
    {
        ret = STREAM_ERR_INVALID_ARGS;
    }
    else
    {
        Pipeline *pipeline = (Pipeline *)streamer->pipes[streamer_msg->pipeline_index];
        memcpy(streamer_msg->get_value, &pipeline->track_info, sizeof(TrackInfo));
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

/*!
 * @brief   Streamer Set Pipeline State
 * @details Handles request to set the state of a pipeline object.
 *
 * @param   [in] task_data Pointer to task data structure
 * @param   [in] msg_data  Pointer to Message Data
 * @returns Error Status
 *
 * @retval STREAM_OK                    State retrieved successfully.
 * @retval STREAM_ERR_INVALID_ARGS      Invalid pipeline object
 */
int streamer_msg_set_state(STREAMER_T *task_data, void *msg_data)
{
    STREAMER_MSG_T *streamer_msg;
    int ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_CORE);

    streamer_msg = (STREAMER_MSG_T *)msg_data;

    if (streamer_msg->pipeline_index >= MAX_PIPELINES)
    {
        ret = STREAM_ERR_INVALID_ARGS;
    }
    else if (0 == task_data->pipes[streamer_msg->pipeline_index])
    {
        ret = STREAM_ERR_INVALID_ARGS;
    }
    else
    {
        ret = set_state_pipeline(task_data->pipes[streamer_msg->pipeline_index], streamer_msg->state);
    }

    streamer_msg->errorcode = ret;

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

/*!
 * @brief   Streamer Get State
 * @details Handles request to get the state of a pipeline object.  State will
 *          be sent back to the application through the callback function.
 *
 * @param   [in] task_data Pointer to task data structure
 * @param   [in] msg_data  Pointer to Message Data
 * @returns Error Status
 *
 * @retval STREAM_OK                    Success
 * @retval STREAM_ERR_INVALID_ARGS      Invalid argument
 */
int streamer_msg_get_state(STREAMER_T *task_data, void *msg_data)
{
    STREAMER_MSG_T *streamer_msg;
    int ret             = STREAM_OK;
    PipelineState state = STATE_NULL;

    STREAMER_FUNC_ENTER(DBG_CORE);

    streamer_msg = (STREAMER_MSG_T *)msg_data;

    if (streamer_msg->pipeline_index >= MAX_PIPELINES)
    {
        ret = STREAM_ERR_INVALID_ARGS;
    }
    else if (0 == task_data->pipes[streamer_msg->pipeline_index])
    {
        ret = STREAM_ERR_INVALID_ARGS;
    }
    else
    {
        state = get_state_pipeline(task_data->pipes[streamer_msg->pipeline_index]);
        ret   = STREAM_OK;
    }

    streamer_msg->errorcode = ret;
    streamer_msg->state     = state;
    if (streamer_msg->get_value)
        *(PipelineState *)streamer_msg->get_value = state;

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

/*!
 * @brief   Streamer Send Query
 * @details Handles request to send a query to a pipeline object. Query data is
 *          returned through the callback function.
 *
 * @param   [in] task_data Pointer to task data structure
 * @param   [in] msg_data  Pointer to Message Data
 * @returns Error Status
 * @retval STREAM_OK                    Query successful
 * @retval STREAM_ERR_INFO_ABSENT       Query unsuccessful. Info not present
 * @retval STREAM_ERR_INVALID_ARGS      Invalid pipeline object
 */
int streamer_msg_send_query(STREAMER_T *task_data, void *msg_data)
{
    STREAMER_MSG_T *streamer_msg;
    int ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_CORE);

    streamer_msg = (STREAMER_MSG_T *)msg_data;

    if (streamer_msg->pipeline_index >= MAX_PIPELINES)
    {
        ret = STREAM_ERR_INVALID_ARGS;
    }
    else if (0 == task_data->pipes[streamer_msg->pipeline_index])
    {
        ret = STREAM_ERR_INVALID_ARGS;
    }
    else
    {
        ret = query_info_pipeline(task_data->pipes[streamer_msg->pipeline_index], streamer_msg->query_type,
                                  &streamer_msg->query_data);
    }

    streamer_msg->errorcode = ret;
    if (streamer_msg->get_value)
        memcpy(streamer_msg->get_value, &streamer_msg->query_data, sizeof(StreamData));

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

/*!
 * @brief   Streamer Seek Pipeline
 * @details Handles request to seek to a specified time in an audio stream
 *
 * @param   [in] task_data Pointer to task data structure
 * @param   [in] msg_data  Pointer to Message Data
 * @returns Error Status
 *
 * @retval STREAM_OK                    Seek successful
 * @retval STREAM_ERR_INVALID_ARGS      Invalid Pipeline object
 * @retval STREAM_ERR_NOT_SEEKABLE      Pipeline does not support seeking
 */
int streamer_msg_seek_pipeline(STREAMER_T *task_data, void *msg_data)
{
    STREAMER_MSG_T *streamer_msg;
    int ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_CORE);

    streamer_msg = (STREAMER_MSG_T *)msg_data;

    if (streamer_msg->pipeline_index >= MAX_PIPELINES)
    {
        ret = STREAM_ERR_INVALID_ARGS;
    }
    else
    {
        ret = seek_pipeline(task_data->pipes[streamer_msg->pipeline_index], streamer_msg->time_ms);
    }

    streamer_msg->errorcode = ret;

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}
/**
 * @brief Streamer get element index
 *
 * @param task_data streamer object
 * @param property element property
 * @return int
 */
static int streamer_get_element_index(STREAMER_T *task_data, ELEMENT_PROPERTY_T property)
{
    int index;
    int element_idx = ELEMENT_NOT_USED;
    int16_t mask;

    STREAMER_FUNC_ENTER(DBG_CORE);

    mask = property.prop & PROP_ELEMENT_MASK;

    /* Find the index of the element associated with this property */
    for (index = 0; index < sizeof(property_lookup_table) / sizeof(ElementPropertyLookup); index++)
    {
        if (property_lookup_table[index].property == mask)
        {
            element_idx = property_lookup_table[index].element_index;
            break;
        }
    }

    if (ELEMENT_NOT_USED == element_idx)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Unable to find element");
        STREAMER_FUNC_EXIT(DBG_CORE);
        return element_idx;
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return element_idx;
}

/*!
 * @brief   Streamer Set Property
 * @details Set an element specific property value
 *
 * @param   [in] task_data Pointer to task data structure
 * @param   [in] msg_data  Pointer to Message Data
 * @returns Error Status
 *
 * @retval STREAM_OK                    Property successfully set.
 * @retval STREAM_ERR_INVALID_ARGS      Invalid arguments
 */
int streamer_msg_set_property(STREAMER_T *task_data, void *msg_data)
{
    STREAMER_MSG_T *streamer_msg;
    ELEMENT_PROPERTY_T property;
    int ret           = STREAM_OK;
    int element_index = ELEMENT_NOT_USED;

    streamer_msg  = (STREAMER_MSG_T *)msg_data;
    property      = streamer_msg->element_property;
    element_index = streamer_get_element_index(task_data, property);

    if (ELEMENT_NOT_USED == element_index)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return STREAM_ERR_INVALID_ARGS;
    }

    if (!task_data->elems[streamer_msg->pipeline_index][element_index])
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return STREAM_ERR_INVALID_ARGS;
    }

    ret = element_set_property(task_data->elems[streamer_msg->pipeline_index][element_index], property.prop,
                               property.val);

    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to set element property. ret=%d\n", ret);
    }
    streamer_msg->errorcode = ret;

    STREAMER_FUNC_EXIT(DBG_CORE);
    return (int)ret;
}

/*!
 * @brief   Streamer Get Property
 * @details Get an element specific property value
 *
 * @param   [in] task_data Pointer to task data structure
 * @param   [in] msg_data  Pointer to Message Data
 * @returns Error Status
 *
 * @retval STREAM_OK                    Property successfully retrieved.
 * @retval STREAM_ERR_INVALID_ARGS      Invalid arguments
 */
int streamer_msg_get_property(STREAMER_T *task_data, void *msg_data)
{
    STREAMER_MSG_T *streamer_msg;
    int ret           = STREAM_OK;
    int element_index = ELEMENT_NOT_USED;

    STREAMER_FUNC_ENTER(DBG_CORE);

    streamer_msg  = (STREAMER_MSG_T *)msg_data;
    element_index = streamer_get_element_index(task_data, streamer_msg->element_property);

    if (ELEMENT_NOT_USED == element_index)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return STREAM_ERR_INVALID_ARGS;
    }

    if (!task_data->elems[streamer_msg->pipeline_index][element_index])
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return STREAM_ERR_INVALID_ARGS;
    }

    ret = element_get_property(task_data->elems[streamer_msg->pipeline_index][element_index],
                               streamer_msg->element_property.prop, streamer_msg->get_value);

    streamer_msg->errorcode = ret;

    STREAMER_FUNC_EXIT(DBG_CORE);
    return (int)ret;
}

/**
 * @brief Streamer message set file
 *
 * @param   [in] task_data Pointer to task data structure
 * @param   [in] msg_data  Pointer to Message Data
 * @returns Error Status
 *
 * @retval STREAM_OK                    Property successfully retrieved.
 * @retval STREAM_ERR_INVALID_ARGS      Invalid arguments
 */
int streamer_msg_set_file(STREAMER_T *task_data, void *msg_data)
{
    STREAMER_MSG_T *streamer_msg;
    ELEMENT_PROPERTY_T property;
    int ret = STREAM_OK;
    char *filename;
    StreamerFileSrcConfig config;

    STREAMER_FUNC_ENTER(DBG_CORE);

    streamer_msg = (STREAMER_MSG_T *)msg_data;
    property     = streamer_msg->element_property;
    filename     = (char *)property.val;

    /* STOP THE PIPELINE TO BE SAFE */
    if (!task_data->pipes[streamer_msg->pipeline_index])
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Unable to find pipeline object:%d\n",
                         streamer_msg->pipeline_index);
        STREAMER_FUNC_EXIT(DBG_CORE);
        return STREAM_ERR_GENERAL;
    }

    ret = set_state_pipeline(task_data->pipes[streamer_msg->pipeline_index], STATE_NULL);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set state failed:%d\n", ret);
        STREAMER_FUNC_EXIT(DBG_CORE);
        return ret;
    }

    /* DETERMINE THE DECODER AND PARSER TYPE BASED ON FILENAME */
    ret = file_src_get_decoder_type(filename, &config);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "streamer_get_fs_pipeline_config failed:%d\n", ret);
        STREAMER_FUNC_EXIT(DBG_CORE);
        return STREAM_ERR_INVALID_ARGS;
    }

    /* SET THE FILE PATH FOR FILESOURCE ELEMENT */
    if (task_data->elems[streamer_msg->pipeline_index][ELEMENT_FILE_SRC_INDEX])
    {
        property.prop = PROP_FILESRC_SET_LOCATION;
        ret           = element_set_property(task_data->elems[streamer_msg->pipeline_index][ELEMENT_FILE_SRC_INDEX],
                                             property.prop, property.val);
        if (STREAM_OK != ret)
        {
            STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set element(%d) property(%d) failed:%d\n",
                             ELEMENT_FILE_SRC_INDEX, PROP_FILESRC_SET_LOCATION, ret);
            STREAMER_FUNC_EXIT(DBG_CORE);
            return ret;
        }
    }
    else
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "File Source Element does not exist.\n");
        STREAMER_FUNC_EXIT(DBG_CORE);
        return STREAM_ERR_ELEMENT_NOT_FOUND;
    }

    if (task_data->elems[streamer_msg->pipeline_index][ELEMENT_DECODER_INDEX])
    {
        property.prop = PROP_DECODER_DECODER_TYPE;
        property.val  = config.decoder_type;
        ret = element_set_property(task_data->elems[streamer_msg->pipeline_index][ELEMENT_DECODER_INDEX], property.prop,
                                   property.val);
        if (STREAM_OK != ret)
        {
            STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set element(%d) property(%d) failed:%d\n",
                             ELEMENT_DECODER_INDEX, PROP_DECODER_DECODER_TYPE, ret);
            STREAMER_FUNC_EXIT(DBG_CORE);
            return ret;
        }

        if (DECODER_TYPE_WAV == config.decoder_type)
        {
            property.prop = PROP_FILESRC_SET_CHUNK_SIZE;
            property.val  = STREAMER_WAVE_FILE_CHUNK_SIZE;
            ret           = element_set_property(task_data->elems[streamer_msg->pipeline_index][ELEMENT_FILE_SRC_INDEX],
                                                 property.prop, property.val);

            if (STREAM_OK != ret)
            {
                STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set element(%d) property(%d) failed:%d\n",
                                 ELEMENT_FILE_SRC_INDEX, PROP_FILESRC_SET_CHUNK_SIZE, ret);
            }
        }
    }
    else
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Decoder object not found.\n");
        STREAMER_FUNC_EXIT(DBG_CORE);
        return STREAM_ERR_ELEMENT_NOT_FOUND;
    }

    /* CCI contains the parsers, omit the streamer parser element if using CCI */
    if (task_data->elems[streamer_msg->pipeline_index][ELEMENT_DECODER_INDEX])
    {
        /* CONFIGURE DECODER ACCORDING TO PARSER TYPE*/
        property.prop = PROP_DECODER_PARSE_TAG;
        property.val  = PARSER_TYPE_BY_PASS == config.parser_type ? true : false;

        ret = element_set_property(task_data->elems[streamer_msg->pipeline_index][ELEMENT_DECODER_INDEX], property.prop,
                                   property.val);

        if (STREAM_OK != ret)
        {
            STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set element(%d) property(%d) failed:%d\n",
                             ELEMENT_DECODER_INDEX, PROP_DECODER_PARSE_TAG, ret);
            STREAMER_FUNC_EXIT(DBG_CORE);
            return ret;
        }
    }
    else
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Parser object not found.\n");
        STREAMER_FUNC_EXIT(DBG_CORE);
        return STREAM_ERR_ELEMENT_NOT_FOUND;
    }

    /* Update the pipeline state */
    ret = set_state_pipeline(task_data->pipes[streamer_msg->pipeline_index], streamer_msg->state);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "streamer_set_file: set state failed:%d\n", ret);
        STREAMER_FUNC_EXIT(DBG_CORE);
        return ret;
    }

    streamer_msg->errorcode = ret;

    STREAMER_FUNC_EXIT(DBG_CORE);
    return (int)ret;
}

/*!
 * @brief   Streamer Set Pipeline State
 * @details Handles request to set the state of a pipeline object.
 *
 * @param   [in] task_data Pointer to task data structure
 * @param   [in] msg_data  Pointer to Message Data
 * @returns Error Status
 *
 * @retval STREAM_OK                    State retrieved successfully.
 * @retval STREAM_ERR_INVALID_ARGS      Invalid pipeline object
 */
int streamer_msg_set_repeat(STREAMER_T *task_data, void *msg_data)
{
    STREAMER_MSG_T *streamer_msg;
    int ret = STREAM_OK;

    STREAMER_FUNC_ENTER(DBG_CORE);

    streamer_msg   = (STREAMER_MSG_T *)msg_data;
    bool repeat_on = streamer_msg->element_property.val;

    if (streamer_msg->pipeline_index >= MAX_PIPELINES)
    {
        ret = STREAM_ERR_INVALID_ARGS;
    }
    else if (0 == task_data->pipes[streamer_msg->pipeline_index])
    {
        ret = STREAM_ERR_INVALID_ARGS;
    }
    else
    {
        ret = set_repeat_pipeline(task_data->pipes[streamer_msg->pipeline_index], repeat_on);
    }

    streamer_msg->errorcode = ret;

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

/*! @} */
