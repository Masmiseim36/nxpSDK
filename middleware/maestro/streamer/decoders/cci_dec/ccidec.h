/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef CCIDEC_H
#define CCIDEC_H

/*!
 * @file    ccidec.h
 * @brief   This file contains the CCI header information.
 *
 */

#include "streamer.h"
#include "decoder.h"
#include "codec_interface_public_api.h"
#include "cci_metadata.h"

/*
 * STRUCTURES AND OTHER TYPEDEFS
 */

#define NUM_OF_PING_PONG_BUFFER 2

/**
 * @brief Common codec interface information
 *
 */
typedef struct
{
    ElementDecoder *element;       /*!< @brief used to access element structure */
    bool init_done;                /*!< @brief flag for multi-pass initialization */
    uint8_t *filesrc_buffer[3];    /*!< @brief internal buffer to hold data from
                                     StreamBuffer buffer until callback
                                     provides it to decoder */
    int32_t filesrc_buffer_idx[3]; /*!< @brief index used to track the current buffer position*/
    int32_t filesrc_size[3];       /*!< @brief size of unused buffer filled from
                                      StreamBuffer buffer -- read callback will
                                      decrement this as read from filesrc_buffer */
    uint32_t filesrc_offset[3];    /*!< @brief represents offset into the file
                                    (from StreamBuffer buffer.offset) */
    uint8_t *packed_buffer[NUM_OF_PING_PONG_BUFFER]; /*!< @brief buffer used to create audio packet -
                                 used by StreamBuffer */
    int32_t dec_frame_size;                                 /*!< @brief Used to keep the maximum buffer size
                                                                  and depending of the decoder type>*/
    /* Exact heap allocated unaligned addresses to free later
     * these buffers will be aligned before use via the uint8_t *ptr_buffer
     */
    void *unaligned_filesrc_buffer[3]; /*!< @brief File source buffer pointers */
    void *unaligned_packed_buffer;     /*!< @brief Output buffer pointer */

    int32_t *ccidec_memory;          /*!< @brief pointer to memory allocated for cci */
    audio_stream_type_t stream_type; /*!< @brief holds converted decoder_type, passed to cci */
    file_meta_data_t *metadata;      /*!< @brief pointer to metadata structure */

    /* any new member in this structure should be added here */
    bool eos_done;        /*!< @brief End of stream flag */
    int32_t buffer_index; /*!< @brief Buffer index for ping pong buffer */
} CCIDecInfo;

//! \brief Declares pfnReadCallback as a ptr to a function that Read data from the input stream
typedef int(pfnReadCallback)(int StreamNo, unsigned char *pAlgorithmBuffer, int Size, int UpdateTail, void *UserData);
//! \brief Declares pfnSeekCallback as a pointer to a function that seek the input stream
typedef int(pfnSeekCallback)(int StreamNo, int offset, int position, void *UserData);
//! \brief Declares pfnFtellCallback as a pointer to a function that gives current position of the input stream
typedef int(pfnFtellCallback)(int StreamNo, void *UserData);

//! \brief This structure is used by the decoder to register the callback APIs.
typedef struct _decoder_callback_FunctionTable
{
    //! \brief Pointer to a function that Reads data from the input stream
    pfnReadCallback *pfnRead;
    //! \brief Pointer to a function that seeks the input stream
    pfnSeekCallback *pfnSeek;
    //! \brief Pointer to a function that gives current position of the input stream
    pfnFtellCallback *pfnFtell;

} decoder_callback_FunctionTable_t;

/*
 * FUNCTION PROTOTYPES
 */

/*!
 * @brief   ccidec_sink_pad_activation_handler
 * @details This function activates the sink pad and allocates memory for
 *          the cci decoder structure and initializes this structure.
 * Activates or deactivates the sink pad for incoming data.
 *
 * @param   pad pad handle
 * @param   active boolean o activate/disactivate pad
 *
 * @returns pad error return
 */
uint8_t ccidec_sink_pad_activation_handler(StreamPad *pad, uint8_t active);

/*!
 * @brief   ccidec_sink_pad_event_handler
 * @details This function handles the pipeline events for the cci decoder.
 *          These events are downstream events and all events will be pushed to
 *          the peer pad.
 *
 * @param   pad pad handle
 * @param   event event enumeration
 *
 * @returns pad error return
 */
uint8_t ccidec_sink_pad_event_handler(StreamPad *pad, StreamEvent *event);

/*!
 * @brief   ccidec_src_pad_event_handler
 * @details This function handles the src events for the cci decoder.
 *          These events are upstream events and all events will be pushed to
 *          the peer pad.
 *
 * @param   pad pad handle
 * @param   event event enumeration
 *
 * @returns pad error return
 */
uint8_t ccidec_src_pad_event_handler(StreamPad *pad, StreamEvent *event);

/*!
 * @brief   ccidec_src_pad_query_handler
 * @details This function handles the src queries for the cci decoder.
 *
 * @param   pad pad handle
 * @param   query event enumeration
 *
 * @returns pad error return
 */
uint8_t ccidec_src_pad_query_handler(StreamPad *pad, StreamQuery *query);
/*!
 * @brief   Process handler for the CCI decoder.
 * @details Used in pull mode, the filesrc process handler should be NULL
 *          so this function will be called instead.  This function will call
 *          the CCI functions to initialize and decode the file.  The CCI
 *          callback will be used to pull data from the filesrc when needed.
 * @param   pad pad handle
 *
 * @returns pad error return
 */
int32_t ccidec_sink_pad_process_handler(StreamPad *pad);
#endif
