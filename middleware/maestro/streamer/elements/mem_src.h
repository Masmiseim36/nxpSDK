/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef MEMSRC_H
#define MEMSRC_H

/*!
 *  @file    mem_src.h
 *  @brief   This is the header file for the memory source element.
 */

#include "streamer_element.h"

/*
 * CONSTANTS, DEFINES AND MACROS
 */

/*!
 * Default read size
 */
#define MEMSRC_DEFAULT_CHUNK_SIZE 1024

/*!
 * Memory Source Element data structure
 */
struct _ElementMemSrc
{
    int32_t (*change_state)(struct _StreamElement *, PipelineState); /*!< @brief Change state function pointer */
    int32_t (*set_property)(struct _StreamElement *,
                            uint16_t prop,
                            uintptr_t val); /*!< @brief Set property function pointer */
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

    uint32_t chunk_size;    /*!< @brief Chunk size */
    int8_t *location;       /*!< @brief File location */
    int8_t *buffer;         /*!< @brief Buffer pointer */
    uint8_t end_of_stream;  /*!< @brief End of stream flag */
    uint32_t read_position; /*!< @brief Last read offset */
    uint32_t size;          /*!< @brief Overall size */
    uint8_t mem_type;       /*!< @brief Memory type */
    uint32_t sample_rate;   /*!< @brief Sample rate */
    uint8_t num_channels;   /*!< @brief Number of channels */
    uint8_t bit_width;      /*!< @brief Bit width */
};
typedef struct _ElementMemSrc ElementMemSrc;

/*!
 * @ingroup coreapi
 * @{
 */

/*
 * FUNCTION PROTOTYPES
 */

/*!
 *
 * @brief this function initializes the memory source element and source pad.
 * all the pad's handlers are intialized.
 *
 * @param element pointer to element
 *
 * @returns StreamReturnType
 * @retval    STREAM_OK if successful
 * @retval    STREAM_ERR_INVALID_ARGS means invalid arguments
 *
 */
int32_t memsrc_init(StreamElement *element);

/*!
 *
 * @brief    This function sets a destiniaiton buffer to be read
 *              by memory source element
 *
 * @param element: element handle
 * @param location: memory location
 * @param size: memory size
 *
 * @returns  StreamReturnType
 * @retval    STREAM_OK if successful
 * @retval    STREAM_ERR_INVALID_ARGS means invalid arguments
 *
 */
int32_t memsrc_set_buffer(ElementHandle element, int8_t *location, uint32_t size);

/*!
 *
 * @brief    This function sets the chunk size in which the momery source sends
 * the data in push mode
 *
 * @param  element element handle
 * @param  chunk_size chunk size
 *
 * @returns  StreamReturnType
 * @retval    STREAM_OK if successful
 * @retval    STREAM_ERR_INVALID_ARGS means invalid arguments
 *
 */
int32_t memsrc_set_push_chunk_size(ElementHandle element, uint32_t chunk_size);
/*!
 *
 * @brief    This function gets the chunk size
 *
 * @param  element element handle
 * @param  chunk_size chunk size
 *
 * @returns  StreamReturnType
 * @retval    STREAM_OK if successful
 * @retval    STREAM_ERR_INVALID_ARGS means invalid arguments
 *
 */
int32_t memsrc_get_push_chunk_size(ElementHandle element, uint32_t *chunk_size);

/*! @} */

#endif

/* end of file */
