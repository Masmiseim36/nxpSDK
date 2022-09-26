/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef EAP_H
#define EAP_H

/*!
 * @file    eap.h
 * @brief  This file includes defines, enums and typedefs for eap.
 */

#include "streamer_element.h"

/** @brief Get audio chunk size */
#define AUDIO_CHUNK_SIZE(pstaudio_packet) ((pstaudio_packet)->chunk_size)
/** @brief Get audio sample rate */
#define AUDIO_SAMPLE_RATE(pstaudio_packet) ((pstaudio_packet)->sample_rate)
/** @brief Get audio number of channels */
#define AUDIO_NUM_CHANNELS(pstaudio_packet) ((pstaudio_packet)->num_channels)

/*!
 * @brief audio sink initialize function proto type
 */
typedef int (*EapInitFunc)(void *);

/*!
 * @brief audio sink post process function proto type
 */
typedef int (*EapPostProcFunc)(void *, void *, int);

/*!
 * @brief audio sink deinitialize function proto type
 */
typedef int (*EapDeinitFunc)(void);

/*!
 * @brief VIT sink element's data structure.
 * @details This data structure is created by create_element,
 * initialized by vitsink_init_element
 */
struct _ElementEap
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

    EapInitFunc init_func;     /*!< @brief EAP init function pointer */
    EapPostProcFunc proc_func; /*!< @brief EAP process function pointer */
    EapDeinitFunc deinit_func; /*!< @brief EAP deinit function pointer */
    void *arg_ptr;             /*!< @brief EAP arguments pointer */
    bool initialized;          /*!< @brief EAP initialized flag */
};
typedef struct _ElementEap ElementEap;

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
int32_t eap_set_buffer(ElementHandle element_hdl, char *location_ptr, uint32_t size);

/*!
 * @param element_ptr pointer of eap element instance
 *
 * @brief This function initializes the eap element
 * and pads.
 *
 * @returns StreamReturnType
 * @retval STREAM_OK  success
 * @retval STREAM_ERR_ELEMENT_NOT_FOUND invalid instance pointer of element_ptr
 */
int32_t eap_init_element(StreamElement *element_ptr);

/**
 * @brief Register function pointers and arguments
 *
 * @param element eap element handle
 * @param init_func_ptr initialization function pointer
 * @param proc_func_ptr processing function pointer
 * @param deinit_func_ptr deinit function pointer
 * @param arg_ptr arguments pointer
 * @return STREAM_OK
 * @return STREAM_ERR_INVALID_ARGS
 */
int32_t eap_register_ext_processing(ElementHandle element,
                                 EapInitFunc init_func_ptr,
                                 EapPostProcFunc proc_func_ptr,
                                 EapDeinitFunc deinit_func_ptr,
                                 void *arg_ptr);

#endif
