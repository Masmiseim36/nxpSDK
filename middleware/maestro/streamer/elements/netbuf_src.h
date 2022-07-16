/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef NETBUFSRC_H
#define NETBUFSRC_H

/*!
 *  @file    netbuf_src.h
 *  @brief   This is the header file for the network buffer source
 */

#include "streamer_element.h"

/*!
 * Memory Source Element data structure
 */
typedef struct _ElementNetbufSrc
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

    uint8_t end_of_stream;                                    /*!< @brief End of stream flag */
    int32_t (*audio_callback)(int8_t *buffer, uint32_t size); /*!< @brief Audio callback function pointer */
} ElementNetbufSrc;

/**
 * @brief Audio callback function pointer for read
 *
 * @param buffer Buffer pointer
 * @param size   Buffer size
 * @return typedef
 */
typedef int32_t audio_callback_t(int8_t *buffer, uint32_t size);

/*!
 *
 * @brief this function initializes the netbuf source element and source pad.
 * all the pad's handlers are intialized.
 *
 * @param element pointer to element
 *
 * @returns StreamReturnType
 * @retval    STREAM_OK if successful
 * @retval    STREAM_ERR_INVALID_ARGS means invalid arguments
 *
 */
int32_t netbufsrc_init(StreamElement *element);

#endif
