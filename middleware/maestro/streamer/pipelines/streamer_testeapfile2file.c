/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include <string.h>

#include "pipeline.h"
#include "streamer_fs.h"
#include "streamer_element_properties.h"
#include "streamer_testeapfile2file.h"

#include "file_sink.h"

int streamer_build_audio_procfile2file_pipeline(int8_t pipeline_index,
                                                StreamPipelineType pipeline_type,
                                                STREAMER_T *task_data)
{
    int ret               = 0;
    uint32_t level        = 0;
    ElementFileSink *sink = NULL;

    if (pipeline_type != STREAM_PIPELINE_TEST_AUDIO_PROCFILE2FILE)
        return STREAM_ERR_GENERAL;

    // Resulting pipeline:
    //
    // [filesrc] => [audio_proc] => [filesink]
    //
    task_data->pipeline_type[pipeline_index] = pipeline_type;

    // Create pipeline
    ret = create_pipeline(&task_data->pipes[pipeline_index], pipeline_index, pipeline_type, &task_data->mq_out);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create_pipeline(%d) failed: %d\n", 0, ret);
        goto err_catch;
    }

    // Create filesrc
    ret = create_element(&task_data->elems[pipeline_index][ELEMENT_FILE_SRC_INDEX], TYPE_ELEMENT_FILE_SRC, 0);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed: %d\n", ELEMENT_FILE_SRC_INDEX, ret);
        goto err_catch;
    }

    // Create EAP_PROC
    ret = create_element(&task_data->elems[pipeline_index][ELEMENT_EAP_PROC_INDEX], TYPE_ELEMENT_AUDIO_PROC, 0);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed: %d\n", ELEMENT_EAP_PROC_INDEX, ret);
        goto err_catch;
    }

    // Create filesink
    ret = create_element(&task_data->elems[pipeline_index][ELEMENT_FILE_SINK_INDEX], TYPE_ELEMENT_FILE_SINK, 0);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed: %d\n", ELEMENT_FILE_SINK_INDEX, ret);
        goto err_catch;
    }

    // Set push scheduling
    sink                         = (ElementFileSink *)task_data->elems[pipeline_index][ELEMENT_FILE_SINK_INDEX];
    sink->sink_pad[0].scheduling = SCHEDULING_PUSH;

    // ADD ELEMENTS TO PIPELINE

    // Filesrc
    ret = add_element_pipeline(task_data->pipes[pipeline_index],
                               task_data->elems[pipeline_index][ELEMENT_FILE_SRC_INDEX], level++);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "add element(%d) failed: %d\n", ELEMENT_FILE_SRC_INDEX, ret);
        goto err_catch;
    }

    // EAP_PROC
    ret = add_element_pipeline(task_data->pipes[pipeline_index],
                               task_data->elems[pipeline_index][ELEMENT_EAP_PROC_INDEX], level++);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "add element(%d) failed: %d\n", ELEMENT_EAP_PROC_INDEX, ret);
        goto err_catch;
    }

    // Filesink
    ret = add_element_pipeline(task_data->pipes[pipeline_index],
                               task_data->elems[pipeline_index][ELEMENT_FILE_SINK_INDEX], level++);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "add element(%d) failed: %d\n", ELEMENT_FILE_SINK_INDEX, ret);
        goto err_catch;
    }

    // LINK ELEMENTS

    // [filesrc |sink] => [src| EAP_PROC]
    ret = link_elements(task_data->elems[pipeline_index][ELEMENT_FILE_SRC_INDEX], 0,
                        task_data->elems[pipeline_index][ELEMENT_EAP_PROC_INDEX], 0);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "link element(%d -> %d) failed: %d\n", ELEMENT_FILE_SRC_INDEX,
                         ELEMENT_EAP_PROC_INDEX, ret);
        goto err_catch;
    }

    // [EAP_PROC: sink] => [src: filesink]
    ret = link_elements(task_data->elems[pipeline_index][ELEMENT_EAP_PROC_INDEX], 0,
                        task_data->elems[pipeline_index][ELEMENT_FILE_SINK_INDEX], 0);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "link element(%d -> %d) failed: %d\n", ELEMENT_EAP_PROC_INDEX,
                         ELEMENT_FILE_SINK_INDEX, ret);
        goto err_catch;
    }

    // Configure filesrc
    ELEMENT_PROPERTY_T prop;

    /* Set file type */
    prop.prop = PROP_FILESRC_SET_FILE_TYPE;
    prop.val  = AUDIO_DATA;

    ret = element_set_property(task_data->elems[pipeline_index][ELEMENT_FILE_SRC_INDEX], prop.prop, prop.val);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set element(%d) property(%d) failed:%d\n", ELEMENT_FILE_SRC_INDEX,
                         PROP_FILESRC_SET_FILE_TYPE, ret);
        goto err_catch;
    }

err_catch:
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "build EAP_PROC pipeline failed: %d\n", ret);
        streamer_destroy_audio_procfile2file_pipeline(pipeline_index, task_data);
    }
    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int streamer_destroy_audio_procfile2file_pipeline(int8_t pipeline_index, STREAMER_T *task_data)
{
    int ret;

    // UNLINK ELEMENTS

    // [EAP_PROC: sink] =/> [src: filesink]
    ret = unlink_elements(task_data->elems[pipeline_index][ELEMENT_EAP_PROC_INDEX], 0,
                          task_data->elems[pipeline_index][ELEMENT_FILE_SINK_INDEX], 0);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "unlink element(%d -/> %d) failed: %d\n", ELEMENT_EAP_PROC_INDEX,
                         ELEMENT_FILE_SINK_INDEX, ret);
        return ret;
    }

    // [filesrc |sink] =/> [src| EAP_PROC]
    ret = unlink_elements(task_data->elems[pipeline_index][ELEMENT_FILE_SRC_INDEX], 0,
                          task_data->elems[pipeline_index][ELEMENT_EAP_PROC_INDEX], 0);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "unlink element(%d -/> %d) failed: %d\n", ELEMENT_FILE_SRC_INDEX,
                         ELEMENT_EAP_PROC_INDEX, ret);
        return ret;
    }

    // REMOVE ELEMENTS FROM PIPELINE

    // Filesink
    ret = remove_element_pipeline(task_data->pipes[pipeline_index],
                                  task_data->elems[pipeline_index][ELEMENT_FILE_SINK_INDEX]);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "remove element(%d) failed: %d\n", ELEMENT_FILE_SINK_INDEX, ret);
        return ret;
    }

    // EAP_PROC
    ret = remove_element_pipeline(task_data->pipes[pipeline_index],
                                  task_data->elems[pipeline_index][ELEMENT_EAP_PROC_INDEX]);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "remove element(%d) failed: %d\n", ELEMENT_EAP_PROC_INDEX, ret);
        return ret;
    }

    // Filesrc
    ret = remove_element_pipeline(task_data->pipes[pipeline_index],
                                  task_data->elems[pipeline_index][ELEMENT_FILE_SRC_INDEX]);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "remove element(%d) failed: %d\n", ELEMENT_FILE_SRC_INDEX, ret);
        return ret;
    }

    // DESTROY ELEMENTS

    // Filesink
    ret = destroy_element(task_data->elems[pipeline_index][ELEMENT_FILE_SINK_INDEX]);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "destroy element(%d) failed: %d\n", ELEMENT_FILE_SINK_INDEX, ret);
        return ret;
    }
    task_data->elems[pipeline_index][ELEMENT_FILE_SINK_INDEX] = (uintptr_t)NULL;

    ret = destroy_element(task_data->elems[pipeline_index][ELEMENT_EAP_PROC_INDEX]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to destroy EAP_PROC\n");
        return ret;
    }
    task_data->elems[pipeline_index][ELEMENT_EAP_PROC_INDEX] = (uintptr_t)NULL;

    // Filesrc
    ret = destroy_element(task_data->elems[pipeline_index][ELEMENT_FILE_SRC_INDEX]);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "destroy element(%d) failed: %d\n", ELEMENT_FILE_SRC_INDEX, ret);
        return ret;
    }
    task_data->elems[pipeline_index][ELEMENT_FILE_SRC_INDEX] = (uintptr_t)NULL;

    // DESTROY PIPELINE
    ret = destroy_pipeline(task_data->pipes[pipeline_index]);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "destroy pipeline failed: %d\n", ret);
        return ret;
    }

    task_data->pipes[pipeline_index] = (uintptr_t)NULL;

    return ret;
}
