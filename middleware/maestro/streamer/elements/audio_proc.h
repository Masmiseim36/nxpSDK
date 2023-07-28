/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef AUDIO_PROC_H
#define AUDIO_PROC_H

/*!
 * @file    audio_proc.h
 * @brief  This file includes defines, enums and typedefs for audio_proc.
 */

#include "streamer_element.h"
#include "streamer_element_properties.h"

/** @brief Get audio chunk size */
#define AUDIO_CHUNK_SIZE(pstaudio_packet) ((pstaudio_packet)->chunk_size)
/** @brief Get audio sample rate */
#define AUDIO_SAMPLE_RATE(pstaudio_packet) ((pstaudio_packet)->sample_rate)
/** @brief Get audio number of channels */
#define AUDIO_NUM_CHANNELS(pstaudio_packet) ((pstaudio_packet)->num_channels)

/*!
 * @brief audio sink initialize function proto type
 */
typedef int (*AudioProcInitFunc)(void *);

/*!
 * @brief audio sink post process function proto type
 */
typedef int (*AudioProcPostProcFunc)(void *, void *, int);

/*!
 * @brief audio sink deinitialize function proto type
 */
typedef int (*AudioProcDeinitFunc)(void);

/*!
 * @brief Function prototype for set num of reference data buffers
 */
typedef int (*AudioProcRefDataSetNumBuffFunc)(int);

/*!
 * @brief Function prototype for processing reference data (VoiceSeeker)
 */
typedef int (*AudioProcRefDataPushFunc)(void *);

/*!
 * @brief Function to set debugging (VoiceSeeker)
 */
typedef int (*AudioProcSetDebuggingFunc)(bool);

/*!
 * @brief VIT sink element's data structure.
 * @details This data structure is created by create_element,
 * initialized by vitsink_init_element
 */
struct _ElementAudioProc
{
    int32_t (*change_state)(struct _StreamElement *, PipelineState); /*!< @brief Change state function pointer */
    int32_t (*set_property)(struct _StreamElement *,
                            uint16_t prop,
                            uint32_t val); /*!< @brief Set property function pointer */
    int32_t (*get_property)(struct _StreamElement *,
                            uint16_t prop,
                            uint32_t *val_ptr); /*!< @brief Get property function pointer */
    void *parent;                               /*!< @brief Parent pointer */
    int8_t key;                                 /*!< @brief Key */
    int8_t depth;                               /*!< @brief Depth */
    PipelineState state;                        /*!< @brief Pipeline state */
    StreamElementType type;                     /*!< @brief Element type */
    PadSrc src_pad[MAX_NUM_PADS];               /*!< @brief Source pads */
    PadSink sink_pad[MAX_NUM_PADS];             /*!< @brief Sink pads */
    struct _StreamElement *sibling;             /*!< @brief Sibling pointer */

    char *location;         /*!< @brief location path */
    uint32_t size;          /*!< @brief memory buffer size */
    uint32_t current_index; /*!< @brief internal used for memory buffer */

    AudioProcInitFunc init_func;     /*!< @brief AUDIO_PROC init function pointer */
    AudioProcPostProcFunc proc_func; /*!< @brief AUDIO_PROC process function pointer */
    AudioProcDeinitFunc deinit_func; /*!< @brief AUDIO_PROC deinit function pointer */
    void *arg_ptr;                   /*!< @brief AUDIO_PROC arguments pointer */
    bool initialized;                /*!< @brief AUDIO_PROC initialized flag */

    /* VoiceSeeker functions */
    AudioProcRefDataSetNumBuffFunc
        refdata_set_num_buff_func; /*!< @brief Function to set number of reference data buffers(VoiceSeeker) */
    AudioProcRefDataPushFunc refdata_push_func;   /*!< @brief Function to process reference data (VoiceSeeker) */
    AudioProcSetDebuggingFunc set_debugging_func; /*!< @brief Function to set debugging (VoiceSeeker) */
};
typedef struct _ElementAudioProc ElementAudioProc;

/*!
 * @param element_hdl: element handle
 * @param location_ptr: location path
 * @param size: buffer size
 *
 * @brief Set the location of the memory buffer to write to as well as the size
 *
 * @returns StreamReturnType
 * @retval STREAM_OK success
 * @retval STREAM_ERR_INVALID_ARGS invalid parameters
 * @retval STREAM_ERR_GENERAL status is bigger than or eque to STATE_PAUSED
 */
int32_t audio_proc_set_buffer(ElementHandle element_hdl, char *location_ptr, uint32_t size);

/*!
 * @param element_ptr pointer of audio_proc element instance
 *
 * @brief This function initializes the audio_proc element
 * and pads.
 *
 * @returns StreamReturnType
 * @retval STREAM_OK  success
 * @retval STREAM_ERR_ELEMENT_NOT_FOUND invalid instance pointer of element_ptr
 */
int32_t audio_proc_init_element(StreamElement *element_ptr);

/**
 * @brief Register function pointers and arguments
 *
 * @param element audio_proc element handle
 * @param init_func_ptr initialization function pointer
 * @param proc_func_ptr processing function pointer
 * @param deinit_func_ptr deinit function pointer
 * @param arg_ptr arguments pointer
 * @return STREAM_OK
 * @return STREAM_ERR_INVALID_ARGS
 */
int32_t audio_proc_register_ext_processing(ElementHandle element,
                                           AudioProcInitFunc init_func_ptr,
                                           AudioProcPostProcFunc proc_func_ptr,
                                           AudioProcDeinitFunc deinit_func_ptr,
                                           void *arg_ptr);

int32_t audio_proc_register_refdata_processing(ElementHandle element,
                                               AudioProcRefDataSetNumBuffFunc set_num_buff,
                                               AudioProcRefDataPushFunc refdata_push,
                                               AudioProcSetDebuggingFunc set_debugging);

int32_t audio_proc_refdata_set_num_buff(ElementHandle element, uint32_t num_buff);
int32_t audio_proc_refdata_push(ElementHandle element, AudioRefData_t *ref_data);
int32_t audio_proc_set_debugging(ElementHandle element, bool set_debugging);

#endif
