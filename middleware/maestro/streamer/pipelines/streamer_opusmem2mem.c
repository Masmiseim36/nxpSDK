/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    streamer_opusmem2mem.c
 * @brief   Implementation of tempoprary Opus memory-to-memory pipeline
 *
 */

/*
 * INCLUDE FILES
 */
#include <string.h>

#include "audio_cfg.h"
#include "pipeline.h"
#include "streamer_opusmem2mem.h"
#include "streamer_element_properties.h"

/*
 * STATIC FUNCTION DECLARATIONS
 */

int streamer_build_opusmem2mem_pipeline(int8_t pipeline_index, StreamPipelineType pipeline_type, STREAMER_T *task_data)
{
    /*
        Resulting pipeline:
        Mem[sink]->[src]OpusEnc[sink]->[src]Mem
    */
    uint8_t level = 0;
    int ret;
    STREAMER_FUNC_ENTER(DBG_CORE);

    task_data->pipeline_type[pipeline_index] = pipeline_type;

    /* Create pipeline */
    ret = create_pipeline(&task_data->pipes[pipeline_index], pipeline_index, pipeline_type, &task_data->mq_out);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create pipeline(%d) failed: %d\n", STREAM_PIPELINE_OPUS_MEM2MEM,
                         ret);
        goto err_catch;
    }

    /* Create the input memory element */
    ret = create_element(&task_data->elems[pipeline_index][ELEMENT_MEM_SRC_INDEX], TYPE_ELEMENT_MEM_SRC, 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed: %d\n", TYPE_ELEMENT_MEM_SRC, ret);
        goto err_catch;
    }

    /* Create the encoder element */
    ret = create_element(&task_data->elems[pipeline_index][ELEMENT_ENCODER_INDEX], TYPE_ELEMENT_ENCODER, 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed: %d\n", TYPE_ELEMENT_ENCODER, ret);
        goto err_catch;
    }

    /* Create the output memory element */
    ret = create_element(&task_data->elems[pipeline_index][ELEMENT_MEM_SINK_INDEX], TYPE_ELEMENT_MEM_SINK, 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed: %d\n", TYPE_ELEMENT_MEM_SINK, ret);
        goto err_catch;
    }

    /* POPULATE THE PIPELINE */

    /* Source */
    ret = add_element_pipeline(task_data->pipes[pipeline_index],
                               task_data->elems[pipeline_index][ELEMENT_MEM_SRC_INDEX], level++);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed: %d\n", ELEMENT_MEM_SRC_INDEX, ret);
        goto err_catch;
    }

    /* Encoder */
    ret = add_element_pipeline(task_data->pipes[pipeline_index],
                               task_data->elems[pipeline_index][ELEMENT_ENCODER_INDEX], level++);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed: %d\n", ELEMENT_ENCODER_INDEX, ret);
        goto err_catch;
    }

    /* Sink */
    ret = add_element_pipeline(task_data->pipes[pipeline_index],
                               task_data->elems[pipeline_index][ELEMENT_MEM_SINK_INDEX], level++);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed: %d\n", ELEMENT_MEM_SINK_INDEX, ret);
        goto err_catch;
    }

    /* LINK ELEMENTS */

    /* MemSrc[sink: 0] => Encoder[src: 0] */
    ret = link_elements(task_data->elems[pipeline_index][ELEMENT_MEM_SRC_INDEX], 0,
                        task_data->elems[pipeline_index][ELEMENT_ENCODER_INDEX], 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "link element(%d) to element(%d) PAD 0 failed: %d\n",
                         ELEMENT_MEM_SRC_INDEX, ELEMENT_ENCODER_INDEX, ret);
        goto err_catch;
    }

    /* Encoder[sink: 0] => MemSink[src: 0] */
    ret = link_elements(task_data->elems[pipeline_index][ELEMENT_ENCODER_INDEX], 0,
                        task_data->elems[pipeline_index][ELEMENT_MEM_SINK_INDEX], 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "link element(%d) to element(%d) PAD 0 failed: %d\n",
                         ELEMENT_ENCODER_INDEX, ELEMENT_MEM_SINK_INDEX, ret);
        goto err_catch;
    }

err_catch:
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "build memory-to-memory w/Opusenc failed: %d\n", ret);
        streamer_destroy_opusmem2mem_pipeline(pipeline_index, task_data);
    }
    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int streamer_destroy_opusmem2mem_pipeline(int8_t pipeline_index, STREAMER_T *task_data)
{
    int ret;

    /* UNLINK ELEMENTS */

    /* MemSrc[sink: 0] =/=> Opus[src: 0] */
    ret = unlink_elements(task_data->elems[pipeline_index][ELEMENT_MEM_SRC_INDEX], 0,
                          task_data->elems[pipeline_index][ELEMENT_ENCODER_INDEX], 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "unlink element(%d) from element(%d) PAD 0 failed:%d\n",
                         ELEMENT_MEM_SRC_INDEX, ELEMENT_ENCODER_INDEX, ret);
        return ret;
    }

    /* Opus[sink: 0] =/=> MemSink[src: 0] */
    ret = unlink_elements(task_data->elems[pipeline_index][ELEMENT_ENCODER_INDEX], 0,
                          task_data->elems[pipeline_index][ELEMENT_MEM_SINK_INDEX], 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "unlink element(%d) from element(%d) PAD 0 failed:%d\n",
                         ELEMENT_ENCODER_INDEX, ELEMENT_MEM_SINK_INDEX, ret);
        return ret;
    }

    /* REMOVE ELEMENTS */

    /* MemSrc */
    ret = remove_element_pipeline(task_data->pipes[pipeline_index],
                                  task_data->elems[pipeline_index][ELEMENT_MEM_SRC_INDEX]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "remove element (%d) failed: %d", ELEMENT_MEM_SRC_INDEX, ret);
        return ret;
    }

    /* Encoder */
    ret = remove_element_pipeline(task_data->pipes[pipeline_index],
                                  task_data->elems[pipeline_index][ELEMENT_ENCODER_INDEX]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "remove element (%d) failed: %d", ELEMENT_ENCODER_INDEX, ret);
        return ret;
    }

    /* MemSink */
    ret = remove_element_pipeline(task_data->pipes[pipeline_index],
                                  task_data->elems[pipeline_index][ELEMENT_MEM_SINK_INDEX]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "remove element (%d) failed: %d", ELEMENT_MEM_SINK_INDEX, ret);
        return ret;
    }

    /* DESTROY ELEMENTS */

    /* MemSrc */
    ret = destroy_element(task_data->elems[pipeline_index][ELEMENT_MEM_SRC_INDEX]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "element destroy (%d) failed: %d", ELEMENT_MEM_SRC_INDEX, ret);
        return ret;
    }
    task_data->elems[pipeline_index][ELEMENT_MEM_SRC_INDEX] = (uintptr_t)NULL;

    /* Encoder */
    ret = destroy_element(task_data->elems[pipeline_index][ELEMENT_ENCODER_INDEX]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "element destroy (%d) failed: %d", ELEMENT_ENCODER_INDEX, ret);
        return ret;
    }
    task_data->elems[pipeline_index][ELEMENT_ENCODER_INDEX] = (uintptr_t)NULL;

    /* MemSink */
    ret = destroy_element(task_data->elems[pipeline_index][ELEMENT_MEM_SINK_INDEX]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "element destroy (%d) failed: %d", ELEMENT_MEM_SINK_INDEX, ret);
        return ret;
    }
    task_data->elems[pipeline_index][ELEMENT_MEM_SINK_INDEX] = (uintptr_t)NULL;

    /* DESTROY PIPELINE */
    ret = destroy_pipeline(task_data->pipes[pipeline_index]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to destroy pipeline: %d", ret);
        return ret;
    }
    task_data->pipes[pipeline_index] = (uintptr_t)NULL;

    return ret;
}
