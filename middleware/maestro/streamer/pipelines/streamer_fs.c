/*
 * Copyright 2018-2023 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*!
 * @file    streamer_fs.c
 * @brief   Implementation of streamer file system support
 *
 */

/*
 * INCLUDE FILES
 */
#include <string.h>

#include "audio_cfg.h"
#include "pipeline.h"
#include "streamer_fs.h"
#include "streamer_element_properties.h"

/*!
 * This table is used by Filesystem pipeline to select the appropriate decoder
 * and parser based on the extension of the file.  This is needed to properly
 * configure the pipeline when switching between different audio file types.
 */
static StreamerFileSrcConfig file_src_cfg_lookup_table[] = {
#ifdef MP3_DEC
    {"mp3", DECODER_TYPE_MP3, PARSER_TYPE_BY_PASS},
#endif
#ifdef WAV_DEC
    {"wav", DECODER_TYPE_WAV, PARSER_TYPE_BY_PASS},
#endif
#ifdef OGG_OPUS_DEC
    {"opus", DECODER_TYPE_OGG_OPUS, PARSER_TYPE_BY_PASS},
    {"ogg", DECODER_TYPE_OGG_OPUS, PARSER_TYPE_BY_PASS},
#endif
#if defined(AAC_DEC) && !defined(__ICCARM__)
    {"aac", DECODER_TYPE_AAC, PARSER_TYPE_BY_PASS},
#endif
#ifdef FLAC_DEC
    {"flac", DECODER_TYPE_FLAC, PARSER_TYPE_BY_PASS},
#endif
    {"", LAST_DECODER_TYPE, LAST_PARSER_TYPE}};

/*
 * STATIC FUNCTION DECLARATIONS
 */

int streamer_build_fs_pipeline(int8_t pipeline_index,
                               const char *out_dev_name,
                               StreamPipelineType pipeline_type,
                               STREAMER_T *task_data)
{
    int ret;
    uint32_t level = 0;
    StreamElement *element;
    ELEMENT_PROPERTY_T prop = {0};
    int file_src_idx;
    StreamElementType file_src_type;

    STREAMER_FUNC_ENTER(DBG_CORE);

    switch (pipeline_type)
    {
        case STREAM_PIPELINE_FILESYSTEM:
            file_src_idx  = ELEMENT_FILE_SRC_INDEX;
            file_src_type = TYPE_ELEMENT_FILE_SRC;
            break;

        case STREAM_PIPELINE_NETBUF:
            file_src_idx  = ELEMENT_NETBUF_SRC_INDEX;
            file_src_type = TYPE_ELEMENT_NETBUF_SRC;
            break;

        case STREAM_PIPELINE_MEM:
        default:
            file_src_idx  = ELEMENT_MEM_SRC_INDEX;
            file_src_type = TYPE_ELEMENT_MEM_SRC;
            break;
    }

    task_data->pipeline_type[pipeline_index] = pipeline_type;

    /* CREATE PIPELINE */
    ret = create_pipeline(&task_data->pipes[pipeline_index], pipeline_index, pipeline_type, &task_data->mq_out);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create pipeline(%d) failed:%d\n", ret);
        goto err_catch;
    }

    /* CREATE FILE/NET/NETBUF SOURCE */
    ret = create_element(&task_data->elems[pipeline_index][file_src_idx], file_src_type, 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed:%d\n", file_src_idx, ret);
        goto err_catch;
    }

    /* CREATE DECODER */
    ret = create_element(&task_data->elems[pipeline_index][ELEMENT_DECODER_INDEX], TYPE_ELEMENT_DECODER, 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed:%d\n", ELEMENT_DECODER_INDEX, ret);
        goto err_catch;
    }

#ifdef EAP_PROC
    /* CREATE EAP_PROC ELEMENT */
    ret = create_element(&task_data->elems[pipeline_index][ELEMENT_EAP_PROC_INDEX], TYPE_ELEMENT_AUDIO_PROC, 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed:%d\n", ELEMENT_EAP_PROC_INDEX, ret);
        goto err_catch;
    }
#endif
#if (SSRC_PROC == 1)
    /* CREATE SSRC_PROC ELEMENT */
    ret = create_element(&task_data->elems[pipeline_index][ELEMENT_SRC_PROC_INDEX], TYPE_ELEMENT_AUDIO_PROC, 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed:%d\n", ELEMENT_SRC_PROC_INDEX, ret);
        goto err_catch;
    }
#endif
    /* CREATE AUDIO SINK */
    ret = create_element(&task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX], TYPE_ELEMENT_AUDIO_SINK, 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "create element(%d) failed:%d\n", ELEMENT_AUDIO_SINK_INDEX, ret);
        goto err_catch;
    }

    element                         = (StreamElement *)task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX];
    element->sink_pad[0].scheduling = SCHEDULING_PUSH;

    /* ASSOCIATE ELEMENTS WITH PIPELINE */
    /* Add source to pipeline */
    ret =
        add_element_pipeline(task_data->pipes[pipeline_index], task_data->elems[pipeline_index][file_src_idx], level++);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "add element(%d) failed:%d\n", file_src_idx, ret);
        goto err_catch;
    }

    /* Add decoder to pipeline */
    ret = add_element_pipeline(task_data->pipes[pipeline_index],
                               task_data->elems[pipeline_index][ELEMENT_DECODER_INDEX], level++);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "add element(%d) failed:%d\n", ELEMENT_DECODER_INDEX, ret);
        goto err_catch;
    }

#ifdef EAP_PROC
    /* Add eap_proc to pipeline */
    ret = add_element_pipeline(task_data->pipes[pipeline_index],
                               task_data->elems[pipeline_index][ELEMENT_EAP_PROC_INDEX], level++);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "add element(%d) failed:%d\n", ELEMENT_EAP_PROC_INDEX, ret);
        goto err_catch;
    }
#endif

#if (SSRC_PROC == 1)
    /* Add ssrc_proc to pipeline */
    ret = add_element_pipeline(task_data->pipes[pipeline_index],
                               task_data->elems[pipeline_index][ELEMENT_SRC_PROC_INDEX], level++);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "add element(%d) failed:%d\n", ELEMENT_SRC_PROC_INDEX, ret);
        goto err_catch;
    }
#endif
    /* Add sink to pipeline */
    ret = add_element_pipeline(task_data->pipes[pipeline_index],
                               task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX], level++);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "add element(%d) failed:%d\n", ELEMENT_AUDIO_SINK_INDEX, ret);
        goto err_catch;
    }

    /* CCI Pipeline:
     *    Level-0         Level-1           Level-2             Level-3
     *   Filesrc[src]->[sink]Decoder[src]->[sink]ThreadElement[src]->[sink]Audio
     */
    /* Link the source to the decoder PAD 0 */
    ret = link_elements(task_data->elems[pipeline_index][file_src_idx], 0,
                        task_data->elems[pipeline_index][ELEMENT_DECODER_INDEX], 0);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "link element(%d) to element(%d) PAD 0 failed:%d\n", file_src_idx,
                         ELEMENT_DECODER_INDEX, ret);
        goto err_catch;
    }

#ifdef EAP_PROC
    /*Link decoder to eap_proc*/
    ret = link_elements(task_data->elems[pipeline_index][ELEMENT_DECODER_INDEX], 0,
                        task_data->elems[pipeline_index][ELEMENT_EAP_PROC_INDEX], 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "link element(%d) to element(%d) failed:%d\n",
                         ELEMENT_DECODER_INDEX, ELEMENT_EAP_PROC_INDEX, ret);
        goto err_catch;
    }
#if (SSRC_PROC == 1)
    /* Link eap_proc to ssrc_proc */
    ret = link_elements(task_data->elems[pipeline_index][ELEMENT_EAP_PROC_INDEX], 0,
                        task_data->elems[pipeline_index][ELEMENT_SRC_PROC_INDEX], 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "link element(%d) to element(%d) failed:%d\n",
                         ELEMENT_EAP_PROC_INDEX, ELEMENT_SRC_PROC_INDEX, ret);
        goto err_catch;
    }

    /* Link the SSRC_PROC to the audio sink */
    ret = link_elements(task_data->elems[pipeline_index][ELEMENT_SRC_PROC_INDEX], 0,
                        task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX], 0);
#else
    /* Link the EAP_PROC to the audio sink */
    ret = link_elements(task_data->elems[pipeline_index][ELEMENT_EAP_PROC_INDEX], 0,
                        task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX], 0);
#endif
#else
#if (SSRC_PROC == 1)
    /* Link decoder to ssrc_proc */
    ret = link_elements(task_data->elems[pipeline_index][ELEMENT_DECODER_INDEX], 0,
                        task_data->elems[pipeline_index][ELEMENT_SRC_PROC_INDEX], 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "link element(%d) to element(%d) failed:%d\n",
                         ELEMENT_DECODER_INDEX, ELEMENT_SRC_PROC_INDEX, ret);
        goto err_catch;
    }

    /* Link the SSRC_PROC to the audio sink */
    ret = link_elements(task_data->elems[pipeline_index][ELEMENT_SRC_PROC_INDEX], 0,
                        task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX], 0);
#else
    /* Link the decoder to the audio sink */
    ret = link_elements(task_data->elems[pipeline_index][ELEMENT_DECODER_INDEX], 0,
                        task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX], 0);
#endif
#endif

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "link element(%d) to element(%d) failed:%d\n",
                         ELEMENT_DECODER_INDEX, ELEMENT_AUDIO_SINK_INDEX, ret);
        goto err_catch;
    }

    /* CONFIGURE AUDIOSINK */
    /* Set the audio sink output driver type */
    prop.prop = PROP_AUDIOSINK_DEVICE_DRIVER_TYPE;
    prop.val  = STREAMER_DEFAULT_OUT_DEVICE;

    ret = element_set_property(task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX], prop.prop, prop.val);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set element(%d) property(%d) failed:%d\n",
                         ELEMENT_AUDIO_SINK_INDEX, PROP_AUDIOSINK_DEVICE_DRIVER_TYPE, ret);
        goto err_catch;
    }

    /* Set audio sink output device name */
    prop.prop = PROP_AUDIOSINK_DEVICE_DRIVER_STRING_NAME;
    prop.val  = (uintptr_t)out_dev_name;

    ret = element_set_property(task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX], prop.prop, prop.val);

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "set element(%d) property(%d) failed:%d\n",
                         ELEMENT_AUDIO_SINK_INDEX, PROP_AUDIOSINK_DEVICE_DRIVER_STRING_NAME, ret);
        goto err_catch;
    }

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

err_catch:
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "build filesystem pipeline failed:%d\n", ret);
        streamer_destroy_fs_pipeline(pipeline_index, task_data);
    }
    STREAMER_FUNC_EXIT(DBG_CORE);
    return ret;
}

int streamer_destroy_fs_pipeline(int8_t pipeline_index, STREAMER_T *task_data)
{
    int ret;
    int file_src_idx;

    switch (task_data->pipeline_type[pipeline_index])
    {
#ifdef STREAMER_ENABLE_FILESRC
        case STREAM_PIPELINE_FILESYSTEM:
            file_src_idx = ELEMENT_FILE_SRC_INDEX;
            break;
#endif

        case STREAM_PIPELINE_NETBUF:
            file_src_idx = ELEMENT_NETBUF_SRC_INDEX;
            break;

        case STREAM_PIPELINE_MEM:
        default:
            file_src_idx = ELEMENT_MEM_SRC_INDEX;
            break;
    }

    STREAMER_FUNC_ENTER(DBG_CORE);

    /* Unlink Elements depending upon which pipeline is used */
    ret = unlink_elements(task_data->elems[pipeline_index][file_src_idx], 0,
                          task_data->elems[pipeline_index][ELEMENT_DECODER_INDEX], 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to unlink file src and decoder PAD 0\n");
        return ret;
    }

#ifdef EAP_PROC
    ret = unlink_elements(task_data->elems[pipeline_index][ELEMENT_DECODER_INDEX], 0,
                          task_data->elems[pipeline_index][ELEMENT_EAP_PROC_INDEX], 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to unlink decoder and EAP_PROC\n");
        return ret;
    }

#if (SSRC_PROC == 1)
    ret = unlink_elements(task_data->elems[pipeline_index][ELEMENT_EAP_PROC_INDEX], 0,
                          task_data->elems[pipeline_index][ELEMENT_SRC_PROC_INDEX], 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to unlink EAP_PROC and SRC_PROC\n");
        return ret;
    }

    ret = unlink_elements(task_data->elems[pipeline_index][ELEMENT_SRC_PROC_INDEX], 0,
                          task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX], 0);
#else
    ret = unlink_elements(task_data->elems[pipeline_index][ELEMENT_EAP_PROC_INDEX], 0,
                          task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX], 0);
#endif
#else
#if (SSRC_PROC == 1)
    ret = unlink_elements(task_data->elems[pipeline_index][ELEMENT_DECODER_INDEX], 0,
                          task_data->elems[pipeline_index][ELEMENT_SRC_PROC_INDEX], 0);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to unlink decoder and SRC_PROC\n");
        return ret;
    }

    ret = unlink_elements(task_data->elems[pipeline_index][ELEMENT_SRC_PROC_INDEX], 0,
                          task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX], 0);
#else
    ret = unlink_elements(task_data->elems[pipeline_index][ELEMENT_DECODER_INDEX], 0,
                          task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX], 0);
#endif
#endif

    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to unlink SRC_PROC or Decoder and audio sink\n");
        return ret;
    }

    /* Remove Elements from pipeline */
    ret = remove_element_pipeline(task_data->pipes[pipeline_index], task_data->elems[pipeline_index][file_src_idx]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to remove file src\n");
        return ret;
    }

    ret = remove_element_pipeline(task_data->pipes[pipeline_index],
                                  task_data->elems[pipeline_index][ELEMENT_DECODER_INDEX]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to remove decoder\n");
        return ret;
    }

#ifdef EAP_PROC
    ret = remove_element_pipeline(task_data->pipes[pipeline_index],
                                  task_data->elems[pipeline_index][ELEMENT_EAP_PROC_INDEX]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to remove EAP_PROC\n");
        return ret;
    }
#endif
#if (SSRC_PROC == 1)
    ret = remove_element_pipeline(task_data->pipes[pipeline_index],
                                  task_data->elems[pipeline_index][ELEMENT_SRC_PROC_INDEX]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to remove SSRC_PROC\n");
        return ret;
    }
#endif
    ret = remove_element_pipeline(task_data->pipes[pipeline_index],
                                  task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to remove audio sink\n");
        return ret;
    }

    /* destroy elements*/
    ret = destroy_element(task_data->elems[pipeline_index][file_src_idx]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to destroy file src\n");
        return ret;
    }
    task_data->elems[pipeline_index][file_src_idx] = (uintptr_t)NULL;

    ret = destroy_element(task_data->elems[pipeline_index][ELEMENT_DECODER_INDEX]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to destroy decoder\n");
        return ret;
    }
    task_data->elems[pipeline_index][ELEMENT_DECODER_INDEX] = (uintptr_t)NULL;

#ifdef EAP_PROC
    ret = destroy_element(task_data->elems[pipeline_index][ELEMENT_EAP_PROC_INDEX]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to destroy EAP_PROC\n");
        return ret;
    }

    task_data->elems[pipeline_index][ELEMENT_EAP_PROC_INDEX] = (uintptr_t)NULL;
#endif
#if (SSRC_PROC == 1)
    ret = destroy_element(task_data->elems[pipeline_index][ELEMENT_SRC_PROC_INDEX]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to destroy ssrc_proc\n");
        return ret;
    }

    task_data->elems[pipeline_index][ELEMENT_SRC_PROC_INDEX] = (uintptr_t)NULL;
#endif

    ret = destroy_element(task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX]);
    if (STREAM_OK != ret)
    {
        STREAMER_LOG_ERR(DBG_CORE, ERRCODE_INTERNAL, "Failed to destroy audio sink\n");
        return ret;
    }
    task_data->elems[pipeline_index][ELEMENT_AUDIO_SINK_INDEX] = (uintptr_t)NULL;

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

int streamer_get_fs_pipeline_config(char *filename, StreamerFileSrcConfig *config)
{
    char *ext = NULL;
    int i     = 0;
    int size  = sizeof(file_src_cfg_lookup_table) / sizeof(StreamerFileSrcConfig);

    STREAMER_FUNC_ENTER(DBG_CORE);

    if ((config == NULL) || (filename == NULL))
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return STREAM_ERR_GENERAL;
    }

    ext = strrchr(filename, '.');
    if (!ext)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return STREAM_ERR_GENERAL; /* Make sure file contains an extension */
    }
    ext++;

    for (i = 0; i < size; i++)
    {
        if (strcasecmp(ext, file_src_cfg_lookup_table[i].extension) == 0)
        {
            break;
        }
    }

    /* Extension unknown - failed to find corresponding config type */
    if (i >= size)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return STREAM_ERR_GENERAL;
    }

    if (DECODER_TYPE_UNKNOWN == file_src_cfg_lookup_table[i].decoder_type)
    {
        STREAMER_FUNC_EXIT(DBG_CORE);
        return STREAM_ERR_GENERAL;
    }

    /* set the decoder and parser type in return structure */
    config->decoder_type = file_src_cfg_lookup_table[i].decoder_type;
    config->parser_type  = file_src_cfg_lookup_table[i].parser_type;

    STREAMER_FUNC_EXIT(DBG_CORE);
    return STREAM_OK;
}
