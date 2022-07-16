/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _OPUS_ENC_H_
#define _OPUS_ENC_H_

#include "streamer_element.h"
#include "cei.h"
#include "cei_table.h"

/*!
 * Opus encoder element data structure
 */
typedef struct
{
    int32_t (*change_state)(struct _StreamElement *, PipelineState); /*!< @brief Change state function pointer */
    int32_t (*set_property)(struct _StreamElement *,
                            uint16_t prop,
                            uintptr_t val); /*!< @brief Set property function pointer */
    int32_t (*get_property)(struct _StreamElement *,
                            uint16_t prop,
                            uint64_t *val_ptr); /*!< @brief Get property function pointer */
    void *parent;                               /*!< @brief Parent pointer */
    int8_t key;                                 /*!< @brief Key */
    int8_t depth;                               /*!< @brief Depth */
    PipelineState state;                        /*!< @brief Pipeline state */
    StreamElementType type;                     /*!< @brief Element type */
    PadSrc src_pad[MAX_NUM_PADS];               /*!< @brief Source pads */
    PadSink sink_pad[MAX_NUM_PADS];             /*!< @brief Sink pads */
    struct _StreamElement *sibling;             /*!< @brief Sibling pointer */

    uint32_t chunk_size;              /*!< @brief Chunk size */
    uint8_t bitstream_info_specified; /*!< @brief Bitstream info flag */
    CeiBitstreamInfo bitstream_info;  /*!< @brief Bitstream info */
    uint8_t initialized;              /*!< @brief Initialized flag */
    uint8_t eos_done;                 /*!< @brief End of stream flag */
    uint32_t offset;                  /*!< @brief Input buffer offset */
    uint32_t outOffset;               /*!< @brief Output buffer offset */
    CeiEncoderType enc_type;          /*!< @brief Encoder type */
    void *enc_info;                   /*!< @brief Pointer to encoder info structure */
    void *initial_config_ptr;         /*!< @brief Initial configuration pointer */
    BufferInfo inBuf;                 /*!< @brief Input buffer info */
    BufferInfo outBuf;                /*!< @brief Output buffer info */
} ElementEncoder;

int32_t encoder_init_element(StreamElement *element);

/* Source pad handler functions */
uint8_t encoder_src_pad_activation_handler(StreamPad *pad, uint8_t active);
int32_t encoder_src_pad_process_handler(StreamPad *pad);

// Property functions
int32_t encoder_get_property(StreamElement *element, uint16_t prop, uint64_t *val_ptr);
int32_t encoder_set_property(StreamElement *element, uint16_t prop, uintptr_t val);

#endif /* _OPUS_ENC_H_ */
