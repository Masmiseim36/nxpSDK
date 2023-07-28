/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef AUDIO_SRC_H
#define AUDIO_SRC_H

/*!
 *  @file    audio_src.h
 *  @brief   This is the header file for the audio source element.
 */

/*----------------------------- Header Files --------------------------*/
#include "streamer_element.h"
/*-------------------------------------------------------------------*/

/*
 * CONSTANTS, DEFINES AND MACROS
 */

/** @brief Audio source default chunk size */
#define AUDIOSRC_DEFAULT_CHUNK_SIZE (1024)
/** @brief Audio source default number of channels */
#define AUDIOSRC_DEFAULT_NUM_CHANNELS (1)
/** @brief Audio source default  bits per sample*/
#define AUDIOSRC_DEFAULT_BITS_PER_SAMPLE (16)
/** @brief Audio source default sample rate */
#define AUDIOSRC_DEFAULT_SAMPLE_RATE (48000)
/** @brief Audio source default sign */
#define AUDIOSRC_DEFAULT_SIGN (1)
/** @brief Audio source default frame size in ms*/
#define AUDIOSRC_DEFAULT_FRAME_MS (10)

/*!
 * @brief AudioSourceStreamErrorType
 * @details Potential Return Values (Error Codes) for Audio Source Operations
 *
 */
typedef enum
{
    AUDIOSRC_SUCCESS                              = 0,
    AUDIOSRC_FAILED                               = -1,
    AUDIOSRC_ERROR_INVALID_ARGS                   = -2,
    AUDIOSRC_ERROR_NEED_DATA                      = -3,
    AUDIOSRC_ERROR_OSA_ERROR                      = -4,
    AUDIOSRC_ERROR_OUT_OF_HEAP_MEMORY             = -5,
    AUDIOSRC_ERROR_OUT_OF_STACK_MEMORY            = -6,
    AUDIOSRC_ERROR_UNDEFINED_STREAM_PROPERTY_TYPE = -7,
    AUDIOSRC_ERROR_READONLY_STREAM_PROPERTY_TYPE  = -8,
    AUDIOSRC_ERROR_UNHANDLED_EVENT                = -9,
    AUDIOSRC_ERROR_PAD                            = -10,
    AUDIOSRC_ERROR_DEVICE_OPEN_FAILED             = -11,
    AUDIOSRC_ERROR_DEVICE_CLOSE_FAILED            = -12,
    AUDIOSRC_ERROR_READ_TIME_OUT                  = -13
} AudioSrcStreamErrorType;

/*!
 * Audio Source Element data structure
 */
struct _ElementAudioSrc
{
    int32_t (*change_state)(struct _StreamElement *, PipelineState); /*!< @brief Change state function pointer */
    int32_t (*set_property)(struct _StreamElement *,
                            uint16_t prop,
                            uint32_t val); /*!< @brief Set property function pointer */
    int32_t (*get_property)(struct _StreamElement *,
                            uint16_t prop,
                            uint64_t *val_ptr);      /*!< @brief Get property function pointer */
    void *parent;                                    /*!< @brief Parent pointer */
    int8_t key;                                      /*!< @brief Key */
    int8_t depth;                                    /*!< @brief Depth */
    PipelineState state;                             /*!< @brief Pipeline state */
    StreamElementType type;                          /*!< @brief Element type */
    PadSrc src_pad[MAX_NUM_PADS];                    /*!< @brief Source pads */
    PadSink sink_pad[MAX_NUM_PADS];                  /*!< @brief Sink pads */
    struct _StreamElement *sibling;                  /*!< @brief Sibling pointer */
    int8_t *buffer;                                  /*!< @brief Buffer pointer */
    uint8_t end_of_stream;                           /*!< @brief End of stream flag */
    AudioPacketHeader pkt_hdr;                       /*!< @brief Audio packet header */
    char device_name[MAX_DEVICE_NAME_LENGTH];        /*!< @brief Device name */
    char output_device_name[MAX_DEVICE_NAME_LENGTH]; /*!< @brief Output device name */
    uint32_t chunk_size;                             /*!< @brief Chunk size */
    uint32_t sample_rate;                            /*!< @brief Sample rate */
    uint32_t retries;                                /*!< @brief Number of retries */
    uint32_t device_driver_type;                     /*!< @brief Device driver type */
    void *device_info;                               /*!< @brief Device info pointer */
    bool continuous_read;                            /*!< @brief Continuous read */
    volatile bool dummy_tx;                          /*!< @brief Dummy tx enable */
    uint8_t frame_ms;                                /*!< @brief Frame size in ms */
    bool first_run;                                  /*!< @brief First run flag */
};
typedef struct _ElementAudioSrc ElementAudioSrc;

/*!
 * @brief audio sink device state
 */
typedef enum
{
    AUDIO_SRC_DEVICE_STATE_CLOSED = 0,
    /*!< @brief device is closed */
    AUDIO_SRC_DEVICE_STATE_OPENED,
    /*!< @brief device is opened */
    LAST_AUDIO_SRC_DEVICE_STATE_TYPE
} AudioSrcDeviceStateType;

/*!
 * @ingroup coreapi
 * @{
 */

/*
 * FUNCTION PROTOTYPES
 */

/*!
 *
 * @brief This function initializes audio source element and its source pads
 *
 * @param element audio source element
 *
 * @returns AudioSourceStreamErrorType
 * @retval AUDIOSRC_SUCCESS success
 * @retval AUDIOSRC_ERROR_INVALID_ARGS invalid argument
 *
 */
int32_t audiosrc_init(StreamElement *element);

/*! @} */

#endif

/* end of file */
