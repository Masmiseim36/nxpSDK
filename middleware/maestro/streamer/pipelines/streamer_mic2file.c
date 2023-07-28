/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    streamer_audiosrc.c
 * @brief   Implementation of streamer audio source support
 *
 */

/*
 * INCLUDE FILES
 */
#include <string.h>

#include "audio_cfg.h"
#include "pipeline.h"
#include "streamer_audiosrc.h"
#include "streamer_element_properties.h"

#ifdef STREAMER_ENABLE_MEM_SRC
#include "mem_src.h"
#endif

/*
 * CONSTANTS, DEFINES AND MACROS
 */

int streamer_build_mic2file_pipeline(int8_t pipeline_index,
                                     const char *in_dev_name,
                                     const char *out_dev_name,
                                     StreamPipelineType pipeline_type,
                                     STREAMER_T *task_data)
{
    int32_t ret             = STREAM_OK;
    uint32_t level          = 0;
    ELEMENT_PROPERTY_T prop = {0};
    AUDSRC_SET_NAME_T audsrc_name;
    StreamElement *element;

    STREAMER_FUNC_ENTER(DBG_CORE);

    task_data->pipeline_type[pipeline_index] = pipeline_type;

    /* Create the pipeline */
    ret = create_pipeline(&task_data->pipes[pipeline_index], pipeline_index, pipeline_type, &task_data->mq_out);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create pipeline failed:%d\n", ret);
        goto err_catch;
    }

    /* Create each of the pipeline elements: AUDIOSRC, AUDIOSINK */
    ret = create_element(&task_data->elems[pipeline_index][ELEMENT_AUDIO_SRC_INDEX], TYPE_ELEMENT_AUDIO_SRC, 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed:%d\n", ELEMENT_AUDIO_SRC_INDEX, ret);
        goto err_catch;
    }

    ret = create_element(&task_data->elems[pipeline_index][ELEMENT_FILE_SINK_INDEX], TYPE_ELEMENT_FILE_SINK, 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed:%d\n", ELEMENT_FILE_SINK_INDEX, ret);
        goto err_catch;
    }

    element = (StreamElement *)task_data->elems[pipeline_index][ELEMENT_FILE_SINK_INDEX];
    /* Configure as push mode */
    element->sink_pad[0].scheduling = SCHEDULING_PUSH;

    /* Add each of the created elements to the pipeline */
    ret = add_element_pipeline(task_data->pipes[pipeline_index],
                               task_data->elems[pipeline_index][ELEMENT_AUDIO_SRC_INDEX], level++);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "add element(%d) failed:%d\n", ELEMENT_AUDIO_SRC_INDEX, ret);
        goto err_catch;
    }

    ret = add_element_pipeline(task_data->pipes[pipeline_index],
                               task_data->elems[pipeline_index][ELEMENT_FILE_SINK_INDEX], level++);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "add element(%d) failed:%d\n", ELEMENT_FILE_SINK_INDEX, ret);
        goto err_catch;
    }

    /* Pipeline:
     *   Link each of the elements in the pipeline together
     *    Level-0        Level-1(optional)  Level-2
     *   Audiosrc[src]->[sink]Queue[src]->[sink]Audio
     */
    ret = link_elements(task_data->elems[pipeline_index][ELEMENT_AUDIO_SRC_INDEX], 0,
                        task_data->elems[pipeline_index][ELEMENT_FILE_SINK_INDEX], 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "link element(%d) to element(%d) failed:%d\n",
                         ELEMENT_AUDIO_SRC_INDEX, ELEMENT_FILE_SINK_INDEX, ret);
        goto err_catch;
    }

    /* Set the properties of the audio source */

    /* Set the device name */
    audsrc_name.device_name        = in_dev_name;
    audsrc_name.output_device_name = out_dev_name;
    prop.prop                      = PROP_AUDIOSRC_SET_DEVICE_NAME;
    prop.val                       = (uintptr_t)&audsrc_name;

    ret = element_set_property(task_data->elems[pipeline_index][ELEMENT_AUDIO_SRC_INDEX], prop.prop, prop.val);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set element(%d) property(%d) failed:%d\n",
                         ELEMENT_AUDIO_SRC_INDEX, PROP_AUDIOSRC_SET_DEVICE_NAME, ret);
        goto err_catch;
    }

    /* Set the sample rate */
    prop.prop = PROP_AUDIOSRC_SET_SAMPLE_RATE;
    prop.val  = STREAMER_AUDIO_SRC_DEFAULT_SAMPLE_RATE;

    ret = element_set_property(task_data->elems[pipeline_index][ELEMENT_AUDIO_SRC_INDEX], prop.prop, prop.val);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set element(%d) property(%d) failed:%d\n",
                         ELEMENT_AUDIO_SRC_INDEX, PROP_AUDIOSRC_SET_SAMPLE_RATE, ret);
        goto err_catch;
    }

    /* Set the dummy tx for mic2file*/
    prop.prop = PROP_AUDIOSRC_SET_DUMMY_TX_ENABLE;
    prop.val  = true;

    ret = element_set_property(task_data->elems[pipeline_index][ELEMENT_AUDIO_SRC_INDEX], prop.prop, prop.val);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set element(%d) property(%d) failed:%d\n",
                         ELEMENT_AUDIO_SRC_INDEX, PROP_AUDIOSRC_SET_DUMMY_TX_ENABLE, ret);
        goto err_catch;
    }

    /* Set the device type based on the input device name */

    prop.val  = (uint32_t)AUDIOSRC_PCMRTOS;
    prop.prop = PROP_AUDIOSRC_SET_DEVICE_TYPE;

    ret = element_set_property(task_data->elems[pipeline_index][ELEMENT_AUDIO_SRC_INDEX], prop.prop, prop.val);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set element(%d) property(%d) failed:%d\n",
                         ELEMENT_AUDIO_SRC_INDEX, PROP_AUDIOSRC_SET_DEVICE_TYPE, ret);
        goto err_catch;
    }

    /* Set the audio sink output device
     * for filesink use PROP_FILESINK_RAW_WRITE_MODE
     */
    prop.prop = PROP_FILESINK_RAW_WRITE_MODE;
    prop.val  = (uint8_t) true;

    ret = element_set_property(task_data->elems[pipeline_index][ELEMENT_FILE_SINK_INDEX], prop.prop, prop.val);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set element(%d) property(%d) failed:%d in raw mode\n",
                         ELEMENT_FILE_SINK_INDEX, PROP_AUDIOSINK_DEVICE_DRIVER_TYPE, ret);
        goto err_catch;
    }

    /* Set the audio sink device name */
    /* Not for file sink */
    /* prop.prop = PROP_AUDIOSINK_DEVICE_DRIVER_STRING_NAME;
    prop.val  = (uintptr_t) out_dev_name;

    ret = element_set_property(
                 task_data->elems[pipeline_index][ELEMENT_FILE_SINK_INDEX],
                 prop.prop, prop.val);

    if (STREAM_OK != ret) {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL,
             "set element(%d) property(%d) failed:%d\n",
             ELEMENT_FILE_SINK_INDEX,
             PROP_AUDIOSINK_DEVICE_DRIVER_STRING_NAME, ret);
        goto err_catch;
    }*/

err_catch:
    /* If any of the above steps failed, destroy the pipeline and
       return an error.
    */
    if (STREAM_OK != ret)
    {
        streamer_destroy_audiosrc_pipeline(pipeline_index, task_data);
    }
    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int streamer_destroy_mic2file_pipeline(int8_t pipeline_index, STREAMER_T *task_data)
{
    int ret;

    STREAMER_FUNC_ENTER(DBG_CORE);

    /* unlink elements in pipeline */
    ret = unlink_elements(task_data->elems[pipeline_index][ELEMENT_AUDIO_SRC_INDEX], 0,
                          task_data->elems[pipeline_index][ELEMENT_FILE_SINK_INDEX], 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to unlink audio src and audio sink\n");
        return ret;
    }

    /* remove_elements from pipeline*/
    ret = remove_element_pipeline(task_data->pipes[pipeline_index],
                                  task_data->elems[pipeline_index][ELEMENT_AUDIO_SRC_INDEX]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to remove audio src\n");
        return ret;
    }

    ret = remove_element_pipeline(task_data->pipes[pipeline_index],
                                  task_data->elems[pipeline_index][ELEMENT_FILE_SINK_INDEX]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to remove audio sink\n");
        return ret;
    }

    /* destroy elements*/
    ret = destroy_element(task_data->elems[pipeline_index][ELEMENT_AUDIO_SRC_INDEX]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to remove audio src\n");
        return ret;
    }
    task_data->elems[pipeline_index][ELEMENT_AUDIO_SRC_INDEX] = (uintptr_t)NULL;

    ret = destroy_element(task_data->elems[pipeline_index][ELEMENT_FILE_SINK_INDEX]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to remove audio sink\n");
        return ret;
    }
    task_data->elems[pipeline_index][ELEMENT_FILE_SINK_INDEX] = (uintptr_t)NULL;

    /* destroy pipeline*/
    ret = destroy_pipeline(task_data->pipes[pipeline_index]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to destroy pipeline\n");
        return ret;
    }
    task_data->pipes[pipeline_index] = (uintptr_t)NULL;

    return ret;
}
