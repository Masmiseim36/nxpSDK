/*
 * Copyright 2018-2022 NXP.
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
#include "streamer_pcm_eap_speaker.h"
#include "streamer_element_properties.h"
#include "eap.h"

int streamer_build_pcm_speaker_pipeline(int8_t pipeline_index, StreamPipelineType pipeline_type, STREAMER_T *task_data)
{
    int ret;
    uint32_t level         = 0;
    StreamElement *element = NULL;

    // Resulting pipeline:
    //
    // [filesrc] => [eap] => [audiosink]
    //
    task_data->pipeline_type = pipeline_type;
    // Create pipeline
    ret = create_pipeline(&task_data->pipes[pipeline_index], pipeline_index, pipeline_type, &task_data->mq_out);

    // Create filesrc
    ret = create_element(&task_data->elems[ELEMENT_FILE_SRC_INDEX], TYPE_ELEMENT_FILE_SRC, 0);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed: %d\n", ELEMENT_FILE_SRC_INDEX, ret);
        goto err_catch;
    }

    if (pipeline_type == STREAM_PIPELINE_PCM_EAP_AUDIO)
    {
        /* CREATE EAP ELEMENT */
        ret = create_element(&task_data->elems[ELEMENT_EAP_INDEX], TYPE_ELEMENT_EAP, 0);
        if (STREAM_OK != ret)
        {
            STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed:%d\n", ELEMENT_EAP_INDEX, ret);
            goto err_catch;
        }
    }

    // Create audiosink
    ret = create_element(&task_data->elems[ELEMENT_AUDIO_SINK_INDEX], TYPE_ELEMENT_AUDIO_SINK, 0);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed: %d\n", ELEMENT_AUDIO_SINK_INDEX, ret);
        goto err_catch;
    }
    // Set audiosink scheduling to push
    element                         = (StreamElement *)task_data->elems[ELEMENT_AUDIO_SINK_INDEX];
    element->sink_pad[0].scheduling = SCHEDULING_PUSH;

    // Add elements to pipeline

    // + filesrc
    ret = add_element_pipeline(task_data->pipes[pipeline_index], task_data->elems[ELEMENT_FILE_SRC_INDEX], level++);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "add element(%d) failed: %d\n", ELEMENT_FILE_SRC_INDEX, ret);
        goto err_catch;
    }

    if (pipeline_type == STREAM_PIPELINE_PCM_EAP_AUDIO)
    {
        /* Add eap to pipeline */
        ret = add_element_pipeline(task_data->pipes[pipeline_index], task_data->elems[ELEMENT_EAP_INDEX], level++);

        if (STREAM_OK != ret)
        {
            STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "add element(%d) failed:%d\n", ELEMENT_EAP_INDEX, ret);
            goto err_catch;
        }
    }

    // + audiosink
    ret = add_element_pipeline(task_data->pipes[pipeline_index], task_data->elems[ELEMENT_AUDIO_SINK_INDEX], level++);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "add element(%d) failed: %d\n", ELEMENT_AUDIO_SINK_INDEX, ret);
        goto err_catch;
    }

    // Link elements in pipeline
    if (pipeline_type == STREAM_PIPELINE_PCM_EAP_AUDIO)
    {
        /*Link file src to eap*/
        ret = link_elements(task_data->elems[ELEMENT_FILE_SRC_INDEX], 0, task_data->elems[ELEMENT_EAP_INDEX], 0);
        if (STREAM_OK != ret)
        {
            STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "link element(%d) to element(%d) failed:%d\n",
                             ELEMENT_DECODER_INDEX, ELEMENT_EAP_INDEX, ret);
            goto err_catch;
        }

        /* Link the EAP to the audio sink */
        ret = link_elements(task_data->elems[ELEMENT_EAP_INDEX], 0, task_data->elems[ELEMENT_AUDIO_SINK_INDEX], 0);
        if (ret != STREAM_OK)
        {
            STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "link element %d => %d failed: %d\n", ELEMENT_EAP_INDEX,
                             ELEMENT_AUDIO_SINK_INDEX, ret);
            goto err_catch;
        }
    }
    else
    {
        // [filesrc: sink0] => [src0: audiosink]
        ret = link_elements(task_data->elems[ELEMENT_FILE_SRC_INDEX], 0, task_data->elems[ELEMENT_AUDIO_SINK_INDEX], 0);
        if (ret != STREAM_OK)
        {
            STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "link element %d => %d failed: %d\n", ELEMENT_FILE_SRC_INDEX,
                             ELEMENT_AUDIO_SINK_INDEX, ret);
            goto err_catch;
        }
    }

    // Configure audiosink

    // Device
    ELEMENT_PROPERTY_T prop;
    prop.prop = PROP_AUDIOSINK_DEVICE_DRIVER_TYPE;
    prop.val  = STREAMER_DEFAULT_OUT_DEVICE;

    ret = element_set_property(task_data->elems[ELEMENT_AUDIO_SINK_INDEX], prop.prop, prop.val);

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

    ret = element_set_property(task_data->elems[ELEMENT_AUDIO_SINK_INDEX], prop.prop, prop.val);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set element(%d) property(%d) failed:%d\n",
                         ELEMENT_AUDIO_SINK_INDEX, PROP_AUDIOSINK_TIME_UPDATE_MS, ret);
        goto err_catch;
    }

err_catch:
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "build pcm_speaker pipeline failed: %d", ret);
        streamer_destroy_pcm_speaker_pipeline(pipeline_index, task_data);
    }

    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int streamer_destroy_pcm_speaker_pipeline(int8_t pipeline_index, STREAMER_T *task_data)
{
    int ret;

    // Unlink elements in pipeline
    if (task_data->pipeline_type == STREAM_PIPELINE_PCM_EAP_AUDIO)
    {
        ret = unlink_elements(task_data->elems[ELEMENT_FILE_SRC_INDEX], 0, task_data->elems[ELEMENT_EAP_INDEX], 0);
        if (STREAM_OK != ret)
        {
            STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to unlink decoder and EAP\n");
            return ret;
        }

        ret = unlink_elements(task_data->elems[ELEMENT_EAP_INDEX], 0, task_data->elems[ELEMENT_AUDIO_SINK_INDEX], 0);
        if (ret != STREAM_OK)
        {
            STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Unlink %d =/=> %d failed: %d\n", ELEMENT_EAP_INDEX,
                             ELEMENT_AUDIO_SINK_INDEX, ret);
            return ret;
        }
    }
    else
    {
        // [filesrc: sink0] =/=> [src0: audiosink]
        ret =
            unlink_elements(task_data->elems[ELEMENT_FILE_SRC_INDEX], 0, task_data->elems[ELEMENT_AUDIO_SINK_INDEX], 0);
        if (ret != STREAM_OK)
        {
            STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Unlink %d =/=> %d failed: %d\n", ELEMENT_FILE_SRC_INDEX,
                             ELEMENT_AUDIO_SINK_INDEX, ret);
            return ret;
        }
    }

    // Remove elements in pipeline

    // -audiosink
    ret = remove_element_pipeline(task_data->pipes[pipeline_index], task_data->elems[ELEMENT_AUDIO_SINK_INDEX]);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Remove element %d failed: %d\n", ELEMENT_AUDIO_SINK_INDEX, ret);
        return ret;
    }
    if (task_data->pipeline_type == STREAM_PIPELINE_PCM_EAP_AUDIO)
    {
        ret = remove_element_pipeline(task_data->pipes[pipeline_index], task_data->elems[ELEMENT_EAP_INDEX]);
        if (STREAM_OK != ret)
        {
            STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to remove EAP\n");
            return ret;
        }
    }

    // -filesrc
    ret = remove_element_pipeline(task_data->pipes[pipeline_index], task_data->elems[ELEMENT_FILE_SRC_INDEX]);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Remove element %d failed: %d\n", ELEMENT_FILE_SRC_INDEX, ret);
        return ret;
    }

    // Destroy elements

    if (task_data->pipeline_type == STREAM_PIPELINE_PCM_EAP_AUDIO)
    {
        // EAP also needs to deinit external lib
        ElementEap *eap_ptr = (ElementEap *)task_data->elems[ELEMENT_EAP_INDEX];
        ret                 = eap_ptr->deinit_func();
        if (ret != 0)
        {
            STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to deinit EAP with error: %d\n", ret);
            return STREAM_ERR_GENERAL;
        }

        ret = destroy_element(task_data->elems[ELEMENT_EAP_INDEX]);
        if (STREAM_OK != ret)
        {
            STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to destroy decoder\n");
            return ret;
        }
        task_data->elems[ELEMENT_EAP_INDEX] = 0;
    }

    // audiosink
    ret = destroy_element(task_data->elems[ELEMENT_AUDIO_SINK_INDEX]);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Destroy element %d failed: %d\n", ELEMENT_AUDIO_SINK_INDEX, ret);
        return ret;
    }
    task_data->elems[ELEMENT_AUDIO_SINK_INDEX] = 0;

    // filesrc
    ret = destroy_element(task_data->elems[ELEMENT_FILE_SRC_INDEX]);
    if (ret != STREAM_OK)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Destroy element %d failed: %d\n", ELEMENT_FILE_SRC_INDEX, ret);
        return ret;
    }
    task_data->elems[ELEMENT_FILE_SRC_INDEX] = 0;

    /* destroy pipeline*/
    ret = destroy_pipeline(task_data->pipes[pipeline_index]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to destroy pipeline\n");
        return ret;
    }
    task_data->pipes[pipeline_index] = 0;

    return STREAM_OK;
}
