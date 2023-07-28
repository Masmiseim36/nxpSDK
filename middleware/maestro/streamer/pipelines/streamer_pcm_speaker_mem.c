/*
 * Copyright 2023 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include <string.h>
#include <stdint.h>

#include "audio_cfg.h"
#include "pipeline.h"
#include "streamer_api.h"
#include "streamer_pcm_speaker_mem.h"
#include "streamer_element_properties.h"
#include "audio_proc.h"

int streamer_build_pcm_speaker_mem_pipeline(int8_t pipeline_index,
                                            StreamPipelineType pipeline_type,
                                            STREAMER_T *task_data)
{
    int ret;
    uint32_t level         = 0;
    StreamElement *element = NULL;

    // Resulting pipeline:
    //
    // [memsrc] => [audio_proc] => [audiosink]
    //
    task_data->pipeline_type[pipeline_index] = pipeline_type;

    // Create pipeline
    ret = create_pipeline(&task_data->pipes[pipeline_index], pipeline_index, pipeline_type, &task_data->mq_out);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create_pipeline(%d) failed: %d\n", 0, ret);
        goto err_catch;
    }

    // Create memsrc
    ret = create_element(&task_data->elems[pipeline_index][ELEMENT_MEM_SRC_INDEX], TYPE_ELEMENT_MEM_SRC, 0);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed: %d\n", ELEMENT_MEM_SRC_INDEX, ret);
        goto err_catch;
    }

    // Create audiosink
    ret = create_element(&task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX], TYPE_ELEMENT_AUDIO_SINK, 0);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed: %d\n", ELEMENT_AUDIO_SINK_INDEX, ret);
        goto err_catch;
    }
    // Set audiosink scheduling to push
    element                         = (StreamElement *)task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX];
    element->sink_pad[0].scheduling = SCHEDULING_PUSH;

    // Add elements to pipeline

    // + memsrc
    ret = add_element_pipeline(task_data->pipes[pipeline_index],
                               task_data->elems[pipeline_index][ELEMENT_MEM_SRC_INDEX], level++);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "add element(%d) failed: %d\n", ELEMENT_MEM_SRC_INDEX, ret);
        goto err_catch;
    }

    // + audiosink
    ret = add_element_pipeline(task_data->pipes[pipeline_index],
                               task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX], level++);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "add element(%d) failed: %d\n", ELEMENT_AUDIO_SINK_INDEX, ret);
        goto err_catch;
    }

    // Link elements in pipeline
    // [memsrc: sink0] => [src0: audiosink]
    ret = link_elements(task_data->elems[pipeline_index][ELEMENT_MEM_SRC_INDEX], 0,
                        task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX], 0);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "link element %d => %d failed: %d\n", ELEMENT_MEM_SRC_INDEX,
                         ELEMENT_AUDIO_SINK_INDEX, ret);
        goto err_catch;
    }

    // Configure audiosink

    // Device
    ELEMENT_PROPERTY_T prop;
    prop.prop = PROP_AUDIOSINK_DEVICE_DRIVER_TYPE;
    prop.val  = STREAMER_DEFAULT_OUT_DEVICE;

    ret = element_set_property(task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX], prop.prop, prop.val);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set element(%d) property(%d) failed:%d\n",
                         ELEMENT_AUDIO_SINK_INDEX, PROP_AUDIOSINK_DEVICE_DRIVER_TYPE, ret);
        goto err_catch;
    }

    // Update duration
    /* Set default streamer update duration */
    prop.prop = PROP_AUDIOSINK_TIME_UPDATE_MS;
    prop.val  = STREAMER_DEFAULT_UPDATE_DURATION;

    ret = element_set_property(task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX], prop.prop, prop.val);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set element(%d) property(%d) failed:%d\n",
                         ELEMENT_AUDIO_SINK_INDEX, PROP_AUDIOSINK_TIME_UPDATE_MS, ret);
        goto err_catch;
    }

    // Configure filesrc
    /* Set file type */
    prop.prop = PROP_MEMSRC_SET_MEM_TYPE;
    prop.val  = AUDIO_DATA;

    ret = element_set_property(task_data->elems[pipeline_index][ELEMENT_MEM_SRC_INDEX], prop.prop, prop.val);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set element(%d) property(%d) failed:%d\n", ELEMENT_MEM_SRC_INDEX,
                         PROP_FILESRC_SET_FILE_TYPE, ret);
        goto err_catch;
    }

err_catch:
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "build pcm_speaker pipeline failed: %d", ret);
        streamer_destroy_pcm_speaker_mem_pipeline(pipeline_index, task_data);
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int streamer_destroy_pcm_speaker_mem_pipeline(int8_t pipeline_index, STREAMER_T *task_data)
{
    int ret;

    // Unlink elements in pipeline
    // [memsrc: sink0] =/=> [src0: audiosink]
    ret = unlink_elements(task_data->elems[pipeline_index][ELEMENT_MEM_SRC_INDEX], 0,
                          task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX], 0);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Unlink %d =/=> %d failed: %d\n", ELEMENT_MEM_SRC_INDEX,
                         ELEMENT_AUDIO_SINK_INDEX, ret);
        return ret;
    }

    // Remove elements in pipeline

    // -audiosink
    ret = remove_element_pipeline(task_data->pipes[pipeline_index],
                                  task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX]);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Remove element %d failed: %d\n", ELEMENT_AUDIO_SINK_INDEX, ret);
        return ret;
    }

    // -memsrc
    ret = remove_element_pipeline(task_data->pipes[pipeline_index],
                                  task_data->elems[pipeline_index][ELEMENT_MEM_SRC_INDEX]);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Remove element %d failed: %d\n", ELEMENT_MEM_SRC_INDEX, ret);
        return ret;
    }

    // Destroy elements

    // audiosink
    ret = destroy_element(task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX]);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Destroy element %d failed: %d\n", ELEMENT_AUDIO_SINK_INDEX, ret);
        return ret;
    }
    task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX] = (uintptr_t)NULL;

    // memsrc
    ret = destroy_element(task_data->elems[pipeline_index][ELEMENT_MEM_SRC_INDEX]);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Destroy element %d failed: %d\n", ELEMENT_MEM_SRC_INDEX, ret);
        return ret;
    }
    task_data->elems[pipeline_index][ELEMENT_MEM_SRC_INDEX] = (uintptr_t)NULL;

    /* destroy pipeline*/
    ret = destroy_pipeline(task_data->pipes[pipeline_index]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to destroy pipeline\n");
        return ret;
    }
    task_data->pipes[pipeline_index] = (uintptr_t)NULL;

    return STREAM_OK;
}
