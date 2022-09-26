/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef VITSINK_H
#define VITSINK_H

/*!
 * @file    vit_sink.h
 * @brief  This file includes defines, enums and typedefs for vit sink.
 */

#include "streamer_element.h"

/**
 * @brief Get audio packet chunk size
 *
 */
#define AUDIO_CHUNK_SIZE(pstaudio_packet) ((pstaudio_packet)->chunk_size)

/*!
 * @brief audio sink initialize function proto type
 */
typedef int (*VitSinkInitFunc)(void *);

/*!
 * @brief audio sink post process function proto type
 */
typedef int (*VitSinkPostProcFunc)(void *, void *, int);

/*!
 * @brief audio sink deinitialize function proto type
 */
typedef int (*VitSinkDeinitFunc)(void);

/*!
 * @brief VIT sink element's data structure.
 * @details This data structure is created by create_element,
 * initialized by vitsink_init_element
 */
struct _ElementVitSink
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

    char *location;         /*!< @brief pointer to memory buffer start address */
    uint8_t raw_write;      /*!< @brief raw write mode */
    uint32_t size;          /*!< @brief memory buffer size */
    uint32_t current_index; /*!< @brief internal used for memory buffer */

    VitSinkInitFunc init_func;     /*!< @brief Init function pointer */
    VitSinkPostProcFunc proc_func; /*!< @brief Process function pointer */
    VitSinkDeinitFunc deinit_func; /*!< @brief Deinit function pointer */
    void *arg_ptr;                 /*!< @brief Arguments pointer */
    bool initialized;              /*!< @brief Initialized flag */
};
typedef struct _ElementVitSink ElementVitSink;

/*!
 * @param element_hdl element handle
 * @param location_ptr  location path
 * @param size memory buffer size
 *
 * @brief Set the location of the memory buffer to write to as well as the size
 *
 * @returns StreamReturnType
 * @retval STREAM_OK success
 * @retval STREAM_ERR_INVALID_ARGS invalid parameters
 * @retval STREAM_ERR_GENERAL status is bigger than or equal to STATE_PAUSED
 */
int32_t vitsink_set_buffer(ElementHandle element_hdl, char *location_ptr, uint32_t size);

/*!
 * @param element_hdl element handle
 * @param raw_write_mode  write mode raw (or) packet header
 *
 * @brief This function sets the raw write mode or the packet
 * header mode to write to memory.
 * (write_mode == true) means only data will be written (no packet headers)
 * (write_mode == false) means packet header will be written aOSA_LONG with data
 *
 * @returns StreamReturnType
 * @retval   STREAM_OK   success
 * @retval   STREAM_ERR_INVALID_ARGS invalid arguments
 * @retval   STREAM_ERR_GENERAL value is bigger than or equal to STATE_PAUSED
 */
int32_t vitsink_set_raw_write_mode(ElementHandle element_hdl, uint8_t raw_write_mode);

/*!
 * @param element_ptr pointer of memory sink element instance
 *
 * @brief This function initializes the vit sink element
 * and sink pads.
 *
 * @returns StreamReturnType
 * @retval STREAM_OK  success
 * @retval STREAM_ERR_ELEMENT_NOT_FOUND invalid instance pointer of element_ptr
 */
int32_t vitsink_init_element(StreamElement *element_ptr);

/**
 * @brief Register external processing function and argument pointers
 *
 * @param element VIT element handle
 * @param init_func_ptr Init function pointer
 * @param proc_func_ptr Process function pointer
 * @param deinit_func_ptr Deinit function pointer
 * @param arg_ptr Arguments pointer
 * @return STREAM_OK
 * @return STREAM_ERR_INVALID_ARGS
 */
int32_t vit_register_ext_processing(ElementHandle element,
                                 VitSinkInitFunc init_func_ptr,
                                 VitSinkPostProcFunc proc_func_ptr,
                                 VitSinkDeinitFunc deinit_func_ptr,
                                 void *arg_ptr);

#endif
