/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef DECODER_H
#define DECODER_H

/*!
 * @file    decoder.h
 * @brief   This file includes defines, enums and typedefs for decoder elements.
 *
 */

#include "streamer_element.h"

/*
 * CONSTANTS, DEFINES AND MACROS
 */

/*!
 * Buffer sizes for supported decoders
 */
/** @brief Decoder interleaved */
#define DECODER_INTERLEAVED false
/** @brief Decoder noninterleaved */
#define DECODER_NONINTERLEAVED true
/** @brief Decoder signed */
#define DECODER_SIGNED true
/** @brief Decoder unsigned */
#define DECODER_UNSIGNED false

/**
 * The decoder output word length
 */
typedef enum
{
    DECODER_OUTPUT_WORD_LENGTH_8  = 8,
    DECODER_OUTPUT_WORD_LENGTH_16 = 16,
    DECODER_OUTPUT_WORD_LENGTH_24 = 24,
    DECODER_OUTPUT_WORD_LENGTH_32 = 32,
    LAST_DECODER_OUTPUT_WORD_LENGTH
} DecoderOutputWordLength;

/*!
 * Decoder element pad handlers:
 * Those functionalities are decoder pads handlers, and can be provided
 * by sub decoders like mp3, wav, wma, aac etc.
 * They are installed when a property PROP_DECODER_DECODER_TYPE is set.
 */
typedef struct
{
    PadActivateFunc src_activate;  /*!< @brief src pad activate handler */
    PadEventFunc src_event;        /*!< @brief src pad event handler */
    PadQueryFunc src_query;        /*!< @brief src pad query handler */
    PadActivateFunc sink_activate; /*!< @brief sink pad activate handler */
    PadChainFunc sink_chain;       /*!< @brief sink chain handler */
    PadEventFunc sink_event;       /*!< @brief sink pad event handler */
    PadProcessFunc sink_process;   /*!< @brief sink pad event handler */
} DecoderHandlersType;

/*!
 * Decoder Element data structure
 */
struct _ElementDecoder
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

    uint8_t decoder_type;       /*!< @brief Decoder type */
    uint32_t chunk_size;        /*!< @brief Chunk size */
    uint32_t sample_rate;       /*!< @brief Sample rate */
    uint32_t current_bit_rate;  /*!< @brief Current bit rate */
    uint32_t average_bit_rate;  /*!< @brief Average bit rate */
    uint32_t num_channels;      /*!< @brief Number of channels */
    uint32_t duration;          /*!< @brief Duration */
    uint8_t endian;             /*!< @brief Endiannes */
    uint8_t output_signed;      /*!< @brief Output signed */
    uint32_t output_wordlength; /*!< @brief Output word length */
    uint8_t mute_frames;        /*!< @brief Mute frames */
    uint8_t output_interleaved; /*!< @brief Output interleaved */
    uint8_t parse_tag;          /*!< @brief Parse tag */
    uint32_t file_size;         /*!< @brief File size */

    /* decoder type specific */
    /* change this to decoder specific events */
    uint32_t num_decoded_frames;         /*!< @brief Number of decoded frames */
    void *dec_info;                      /*!< @brief Pointer to decoder info */
    const DecoderHandlersType *handlers; /*!< @brief Pointer to decoder handlers */
};

typedef struct _ElementDecoder ElementDecoder;

/*
 * Decoder APIs
 */

/*!
 * @brief This function initializes the decoder element, and loads the
 *        pad source and sink functions.
 * @details
 *  The following Pad Functions are used in this implementation:
 *  SRC FUNCTIONS
 *   pad->event_handler = decoder_src_pad_event_handler()
 *   pad->query_handler = decoder_src_pad_query_handler()
 *   pad->activation_handler = NULL
 *   pad->activate_pull =
 *   pad->activate_push = decoder_src_pad_activation_handler()
 *   pad->process_handler = NULL
 *   pad->process_precheck = NULL
 *   pad->pull_handler = NULL
 *  SINK FUNCTIONS
 *   pad->event_handler = decoder_sink_pad_event_handler()
 *   pad->query_handler = NULL
 *   pad->activation_handler = decoder_sink_pad_activation_handler()
 *   pad->activate_pull = ???
 *   pad->activate_push = ???
 *   pad->process_handler = decoder_sink_pad_process_handler()
 *   pad->process_precheck = NULL
 *   pad->chain_handler = decoder_sink_pad_chain_handler()
 *  This function is loaded in the  ElementInit element_list[] table and called
 *  during create_element()
 *
 * @param   element StreamElement pointer will be used to create ElementDecoder
 *
 * @retval  STREAM_OK if successful
 * @retval  STREAM_ERR_INVALID_ARGS means invalid arguments
 */
int32_t decoder_init_element(StreamElement *element);

/*!
 * @brief   This function changes the decoder state by calling
 *          STATE_TRANSITION pipeline macro.
 *          If the new state is NULL_TO_READY, it verifies the
 *          decoder type is known and handler is setup.
 *
 * @param   element StreamElement will be used to create ElementDecoder
 * @param   new_dec_state new decoder state to change to
 *
 * @retval  STREAM_ERR_INVALID_ARGS  invalid parameters
 * @retval  STREAM_ERR_GENERAL if moved to ready state and decoders not setup
 * @retval  STREAM_OK successful
 */
int32_t decoder_change_state(StreamElement *element, PipelineState new_dec_state);

/*!
 * @brief Returns the duration of a source
 * PadQueryFunc is called to return the element duration in
 * the format defined by the StreamDataFormat parameter.
 *
 * @param element ElementDecoder pointer will be used to create StreamPad sink
 * @param data StreamDataFormat defines the format of the data to return.
 *
 * @returns duration or 0 if failed
 */
uint32_t decoder_get_duration(ElementDecoder *element, StreamDataFormat data);

/*!
 * @brief   Calculate and set the duration of a source
 * This function calculates the duration in bytes using the stream size
 * and stream bitrate, and sets it in bytes in the ElementDecoder variable.
 *
 * @param element ElementDecoder pointer will hold the calculated duration value
 * @param stream_size Size of the stream
 * @param bit_rate Bit rate of the stream
 *
 * @returns None
 */
void decoder_set_duration(ElementDecoder *element, uint32_t stream_size, uint32_t bit_rate);

/*!
 * @brief   Set a property of a decoder element
 * @details This is a new interface to replace the original decoder_set_type().
 *
 * @param element StreamElement pointer
 * @param prop_id id number of a element's property
 * @param prop_value the property value to be set
 *
 * @returns StreamReturnType
 *
 * @retval    STREAM_OK if successful
 * @retval    STREAM_ERR_INVALID_ARGS means invalid arguments
 */
int32_t decoder_set_property(StreamElement *element, uint16_t prop_id, uint32_t prop_value);

/*!
 * @brief   Get a property of a decoder element
 *
 * @param element StreamElement pointer that will be used as ElementHandle
 * @param prop_id  id number of a element's property
 * @param prop_value the property value to be saved in
 *
 * @retval    STREAM_OK if successful
 * @retval    STREAM_ERR_INVALID_ARGS means invalid arguments
 */
int32_t decoder_get_property(StreamElement *element, uint16_t prop_id, uint32_t *prop_value);

/*!
 * @brief Set the decoder type for the input element.
 * This function maps the generic decoder element pad handlers to the
 * specific decoder implementation using DecoderHandlersType.
 *
 * @param element ElementHandle that will be used as ElementDecoder to
 *                set the handlers to the input decoder type
 * @param dec_type DecoderType value that will be used to map the appropriate
 *                 handler functions.
 *
 * @retval    STREAM_OK if successful
 * @retval    STREAM_ERR_INVALID_ARGS means invalid arguments
 * @retval    STREAM_ERR_GENERAL is returned if dec_type is not handled
 */
int32_t decoder_set_type(ElementHandle element, DecoderType dec_type);

/*!
 * @brief   Enable/Disable the tag parsing
 * Sets the parse_tag variable to the value passed in via activate variable.
 *
 * @param   element ElementHandle that will be used as ElementDecoder
 * @param   activate value used to set parse_tag in decoder element.
 *
 * @retval    STREAM_OK if successful
 * @retval    STREAM_ERR_INVALID_ARGS means invalid arguments
 */
int32_t decoder_activate_parse_tag(ElementHandle element, uint8_t activate);

/*!
 * @brief    Is current source seekable
 * @details  This function sends a query to sink pad of the decoder
 *           element to check if check if seek is possible in the
 *           specified format.
 *
 * @param decoder ElementDecoder pointer will be use to create a sink StreamPad
 * @param format format type used to create the query.
 *
 * @returns bool
 * @retval true source is seekable
 * @retval false souce is not seekable
 */
bool decoder_is_source_seekable(ElementDecoder *decoder, StreamDataFormat format);

/*!
 * @brief   Update a running average of bit rate.
 *
 * @param element  ElementDecoder pointer that will contain the
 *                 calculated average bit rate.
 *
 * @returns None
 */
void vUpdateAverageBitrate(ElementDecoder *element);

#endif
