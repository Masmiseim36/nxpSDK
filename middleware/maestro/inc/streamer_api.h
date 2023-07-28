/*
 * Copyright 2018-2023 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef STREAMER_API_H
#define STREAMER_API_H

/*!
 * @file    streamer_api.h
 * @brief   Streamer API header. Public APIs for using the maestro streamer framework
 *
 */

/*
 * INCLUDE FILES
 */
#include <stddef.h>

#include "fsl_os_abstraction.h"
#include "streamer_message.h"

/**
 * @brief Message ID prefix
 *
 */
#define AF_MESSAGE_SECTION 0x10000
/**
 * @brief First message ID
 *
 */
#define STREAMER_MSG_START (0x0000 | AF_MESSAGE_SECTION)
/**
 * @brief Last message ID
 *
 */
#define STREAMER_MSG_END (0x00FF | AF_MESSAGE_SECTION)
/**
 * @brief Message queue name length
 *
 */
#define STREAMER_MQ_NAME_LENGTH (32)
/**
 * @brief Codec information length
 *
 */
#define CODEC_INFO_LEN 64
/**
 * @brief Get bit on specified position
 *
 */
#define GET_BIT(Bit) (1 << (Bit))
/**
 * @brief Set bit on specified position
 *
 */
#define SET_BIT(Flag, Bit) ((Flag) |= GET_BIT(Bit))
/**
 * @brief Clear bit on specified position
 *
 */
#define CLEAR_BIT(Flag, Bit) ((Flag) &= ~GET_BIT(Bit))
/**
 * @brief Build element ID based on number of sources, sinks and type element ID
 *
 */
#define BUILD_ELEMENT(n_src, n_sink, id) (_SRC(n_src) | _SINK(n_sink) | id)
/**
 * @brief Helper macro for building element ID with sink pad number
 *
 */
#define _SINK(num) ((num & 0x3) << 14)
/**
 * @brief Helper macro for building element ID with source pad number
 *
 */
#define _SRC(num) ((num & 0x3) << 12)
/**
 * @brief Get element ID based on its type
 *
 */
#define ELEMENT_ID(type) (type & 0x0FFF)
/**
 * @brief Maximal path name size
 *
 */
#define PATHNAME_SIZE 256
/**
 * @brief Bit for waiting for streamer message response
 *
 */
#define EVENT_WAIT_BIT 1
/**
 * @brief Maximum number of messages in streamer message queue
 *
 */
#define MSG_POOL_NUM_MSGS 10
/**
 * @brief The maximum number of Pipelines that may be created within a single streamer task.
 */
#define MAX_PIPELINES (2)
/**
 * @brief  The maximum number of Elements that may be created within a single streamer task.
 */
#define MAX_ELEMENTS (ELEMENT_LAST_INDEX)
/**
 * @brief The wait time out for streamer response of a message processing completion
 * After streamer processed a message, it will signal the condition variable
 * in the message to make the function call synchronized.
 */
#define WAIT_STREAMER_MSG_RESPONSE_TIME_OUT 8000 /* 8s */

#define TRACK_TITLE_LEN  64 /*!< @brief Maximum track title length */
#define TRACK_ARTIST_LEN 64 /*!< @brief Maximum track artist length */
#define TRACK_ALBUM_LEN  64 /*!< @brief Maximum track album length */
#define TRACK_GENRE_LEN  34 /*!< @brief Maximum track genre length */
#define TRACK_YEAR_LEN   5  /*!< @brief Maximum track year length */

/*!
 * StreamReturnType
 * Handles return status from pipeline operations
 */
typedef enum
{
    STREAM_OK = 0,
    /*!< @brief General success. */
    STREAM_ERR_GENERAL = -1,
    /*!< @brief General failure. */
    STREAM_ERR_INVALID_ARGS = -2,
    /*!< @brief Invalid arguments to the function. */
    STREAM_ERR_NO_MEM = -3,
    /*!< @brief Not enough memory to continue. Fatal. */
    STREAM_ERR_INVALID_SEEK_TIME = -4,
    /*!< @brief Invalid seek time specified. */
    STREAM_ERR_NOT_SEEKABLE = -5,
    /*!< @brief Pipeline playback not seekable. */
    STREAM_ERR_INFO_ABSENT = -6,
    /*!< @brief Required info not available. */
    STREAM_ERR_ELEMENT_ALREADY_IN_PIPELINE = -7,
    /*!< @brief Element already a part of pipeline.*/
    STREAM_ERR_ELEMENT_LINKED = -8,
    /*!< @brief Element linked to another. */
    STREAM_ERR_ELEMENT_NOT_FREE = -9,
    /*!< @brief Element still linked an part of pipeline. */
    STREAM_ERR_ELEMENT_NOT_FOUND = -10,
    /*!< @brief Specified element not found in pipeline. */
    STREAM_ERR_ELEMENT_INVALID_PAD = -11,
    /*!< @brief Invalid pad index specified in element.*/
    STREAM_ERR_ELEMENT_PADS_NOT_LINKED = -12,
    /*!< @brief Specified pads not linked.*/
    STREAM_ERR_ELEMENT_BAD_STATUS = -13,
    /*!< @brief try to operate in a bad status.*/
} StreamReturnType;

/*!
 * Streamer Message Identifiers
 * Enumeration of all streamer request messages
 */
typedef enum
{
    STREAM_MSG_CLOSE_TASK = STREAMER_MSG_START, /*!< @brief 0 Close Streamer Task */
    STREAM_MSG_CREATE_PIPELINE,                 /*!< @brief 1 Create Pipeline */
    STREAM_MSG_DESTROY_PIPELINE,                /*!< @brief 2 Destroy Pipeline */
    STREAM_MSG_CREATE_ELEMENT,                  /*!< @brief 3 Create Element */
    STREAM_MSG_DESTROY_ELEMENT,                 /*!< @brief 4 Destroy Element */
    STREAM_MSG_ADD_ELEMENT,                     /*!< @brief 5 Add Element to Pipeline */
    STREAM_MSG_REMOVE_ELEMENT,                  /*!< @brief 6 Remove Element from Pipeline */
    STREAM_MSG_LINK_ELEMENTS,                   /*!< @brief 7 Link two elements together */
    STREAM_MSG_UNLINK_ELEMENTS,                 /*!< @brief 8 Unlink two elements together */
    STREAM_MSG_SET_STATE,                       /*!< @brief 9 Set Pipeline State */
    STREAM_MSG_GET_STATE,                       /*!< @brief a Get Pipeline State */
    STREAM_MSG_SET_FILE,                        /*!< @brief b Set the file path and reconfigure the
                                                            pipeline */
    STREAM_MSG_SET_PROPERTY,                    /*!< @brief c Set an Element specific property
                                                            value */
    STREAM_MSG_GET_PROPERTY,                    /*!< @brief d Get an Element specific property
                                                            value */
    STREAM_MSG_GET_TRACK_INFO,                  /*!< @brief e Get track info */
    STREAM_MSG_SEEK_PIPELINE,                   /*!< @brief f Seek the pipeline to a time */
    STREAM_MSG_QUERY_PIPELINE,                  /*!< @brief 10 Query the pipeline */
    STREAM_MSG_SET_REPEAT,                      /*!< @brief 11 Set repeat mode of pipeline */
    STREAM_MSG_UPDATE_DURATION,                 /*!< @brief 12 Track duration info updated */
    STREAM_MSG_UPDATE_POSITION,                 /*!< @brief 13 Track playing position updated */
    STREAM_MSG_UPDATE_TRACK_INFO,               /*!< @brief 14 Track info updated */
    STREAM_MSG_EOS,                             /*!< @brief 15 End of track reached */
    STREAM_MSG_ERROR,                           /*!< @brief 16 Error in streamer */
    STREAM_MSG_LAST = STREAMER_MSG_END          /*!< @brief 17 */
} STREAMER_MSG_ID;

/*!
 *
 * @brief PipelineState
 * @details States for pipeline and individual elements of the pipeline. In
 *          ideal condition all the elements will have the same state as the
 *          parent pipeline state.
 */
typedef enum
{

    STATE_NULL,
    /*!< @brief A pipeline is created, but all the elements are in initial state
    (decoder type, parser type are not determined - using default value).
    Source file is not assigned or open.*/
    STATE_READY,
    /*!< @brief All the streamer elements’ properties are initialized
    (source file opened, decoder type, parser type are determined).
    The pipeline is ready for playing or pause. */
    STATE_PAUSED, /*!< @brief The element should be ready to accept and
                              process data. */
    STATE_PLAYING /*!< @brief The same as PAUSED except for live sources and
                              sinks. */
} PipelineState;

/*!
 * @brief StreamPipelineType
 * @details The type of pipeline in streamer. These may not all be supported.
 */
typedef enum
{
    STREAM_PIPELINE_FILESYSTEM = 0, /*!< @brief Pipeline with file source, decoder and audio sink */
    STREAM_PIPELINE_MEM,            /*!< @brief Pipeline with memory buffer source, decoder and audio sink */
    STREAM_PIPELINE_NETBUF,         /*!< @brief Pipeline with network buffer source, decoder and audio sink */
    STREAM_PIPELINE_PCM,            /*!< @brief Pipeline with audio (PCM) source and audio sink */
    STREAM_PIPELINE_OPUS_MEM2MEM,
    STREAM_PIPELINE_MIC2FILE,
    STREAM_PIPELINE_VIT,
    STREAM_PIPELINE_PCM_AUDIO,
    STREAM_PIPELINE_PCM_AUDIO_MEM,
    STREAM_PIPELINE_PCM_AUDIO_PROC_AUDIO,
    STREAM_PIPELINE_PCM_EAP_PROC_AUDIO,
    STREAM_PIPELINE_TEST_AUDIO_PROCFILE2FILE,
    STREAM_PIPELINE_VIT_FILESINK,
    STREAM_PIPELINE_MAX
} StreamPipelineType;

/**
 * @brief Element index for the lookup table
 *
 */
typedef enum
{
    ELEMENT_FILE_SRC_INDEX,
    ELEMENT_MEM_SRC_INDEX,
    ELEMENT_NETBUF_SRC_INDEX,
    ELEMENT_AUDIO_SRC_INDEX,
    ELEMENT_FILE_SINK_INDEX,
    ELEMENT_MEM_SINK_INDEX,
    ELEMENT_AUDIO_SINK_INDEX,
    ELEMENT_DECODER_INDEX,
    ELEMENT_ENCODER_INDEX,
    ELEMENT_VIT_INDEX,
    ELEMENT_EAP_PROC_INDEX,
    ELEMENT_VOICESEEKER_PROC_INDEX,
    ELEMENT_SRC_PROC_INDEX,
    ELEMENT_LAST_INDEX
} ElementIndex;

/*!
 * @brief   StreamElementType
 * @details Holds an element type for pipeline operations. Type is built based
 *          on number of source pads, number of sink pads, and a unique index
 */
typedef enum
{
    /* Source elements (with one or more source pads and no sink pads) */
    TYPE_ELEMENT_FILE_SRC = BUILD_ELEMENT(1, 0, 0),
    /*!< @brief File source element */
    TYPE_ELEMENT_MEM_SRC = BUILD_ELEMENT(1, 0, 1),
    /*!< @brief Memory source element */
    TYPE_ELEMENT_NETBUF_SRC = BUILD_ELEMENT(1, 0, 2),
    /*!< @brief Network buffer source element. Requires CCI usage. */
    TYPE_ELEMENT_AUDIO_SRC = BUILD_ELEMENT(1, 0, 3),
    /*!< @brief Audio source element */
    /* Sink elements (with no source pads and one sink pad) */
    TYPE_ELEMENT_FILE_SINK = BUILD_ELEMENT(0, 1, 4),
    /*!< @brief File sink element */
    TYPE_ELEMENT_MEM_SINK = BUILD_ELEMENT(0, 1, 5),
    /*!< @brief Memory sink element */
    TYPE_ELEMENT_AUDIO_SINK = BUILD_ELEMENT(0, 1, 6),
    /*!< @brief Audio sink element */
    /* Decoder element (with one source pad and one sink pad) */
    TYPE_ELEMENT_DECODER = BUILD_ELEMENT(1, 1, 7),
    /*!< @brief Decoder element */
    TYPE_ELEMENT_ENCODER = BUILD_ELEMENT(1, 1, 8),

    TYPE_ELEMENT_VIT_SINK = BUILD_ELEMENT(0, 1, 9),

    TYPE_ELEMENT_AUDIO_PROC = BUILD_ELEMENT(1, 1, 10),

    TYPE_ELEMENT_LAST = 11
} StreamElementType;

/*!
 * @brief DecoderType
 * @details Enumeration of potential media decoders. These may not all be supported.
 */
typedef enum
{
    DECODER_TYPE_UNKNOWN, /*!< @brief unknown Decoder*/
    DECODER_TYPE_WAV,     /*!< @brief Wave Decoder */
    DECODER_TYPE_MP3,     /*!< @brief MP3 Decoder */
    DECODER_TYPE_MP2,
    DECODER_TYPE_AAC,
    DECODER_TYPE_OGG,
    DECODER_TYPE_WMA,
    DECODER_TYPE_WMV,
    DECODER_TYPE_MPEG2,
    DECODER_TYPE_MPEG4,
    DECODER_TYPE_JPEG,
    DECODER_TYPE_CD_DA,
    DECODER_TYPE_MFI,
    DECODER_TYPE_FLAC,
    DECODER_TYPE_ALAC,
    DECODER_TYPE_AC3,
    DECODER_TYPE_OPUS,     /*!< @brief raw OPUS decoder */
    DECODER_TYPE_OGG_OPUS, /*!< @brief OGG OPUS decoder */
    LAST_DECODER_TYPE      /*!< @brief Placeholder */

} DecoderType;

/*!
 * @brief ParserType
 * @details Enumeration of potential media parsers.
 * @        These may not all be supported.
 *
 */
typedef enum
{
    PARSER_TYPE_ASF,     /*!< @brief ASF Parser */
    PARSER_TYPE_MP4,     /*!< @brief MP4 Parser */
    PARSER_TYPE_3GP,     /*!< @brief 3GP parser */
    PARSER_TYPE_OGG,     /*!< @brief OGG parser */
    PARSER_TYPE_BY_PASS, /*!< @brief parser will work in by pass mode by default*/
    LAST_PARSER_TYPE     /*!< @brief Placeholder */

} ParserType;

/*!
 * @brief   StreamInfoType
 * @details Types of Information the pipeline may provide
 */
typedef enum
{
    INFO_AUDIO_BITRATE,      /*!< @brief Audio Bitrate */
    INFO_AUDIO_CHANNELS,     /*!< @brief Number of audio channels */
    INFO_AUDIO_SAMPLERATE,   /*!< @brief Audio Sample Rate (Hz) */
    INFO_AUDIO_TYPE,         /*!< @brief Audio File */
    INFO_CODEC_INFO,         /*!< @brief Codec information */
    INFO_DURATION,           /*!< @brief Duration of the Track (ms) */
    INFO_POSITION,           /*!< @brief Current Position of the Track (ms) */
    INFO_TIME_SEEKABLE,      /*!< @brief Input Supports Seek by Time */
    INFO_BYTE_SEEKABLE,      /*!< @brief Input Supports Seek by Byte */
    INFO_STREAM_INFO_UPDATE, /*!< @brief stream info updated */
    INFO_SIZE,               /*!< @brief Size of audio size*/
    INFO_BUFF_OCC,           /*!< @brief Buffer level/audio latency */
    INFO_ALBUM,              /*!< @brief Album Name */
    INFO_ARTIST,             /*!< @brief Artist Name */
    INFO_TITLE,              /*!< @brief Track Title */
    INFO_GENRE,              /*!< @brief Genre */
    INFO_YEAR,               /*!< @brief Year */
    INFO_TRACK,              /*!< @brief Track Number */
    INFO_ALBUMART_IMAGE,     /*!< @brief Album Artwork */
    INFO_META_INFO_UPDATE,   /*!< @brief metadata info updated */
    INFO_ERROR,              /*!< @brief error */

    INFO_LAST /*!< @brief Placeholder always at the end */
} StreamInfoType;

/*!
 * @brief AudioSrcDeviceDriverType
 * @details Enumeration of potential audio src devices
 *
 */
typedef enum
{
    AUDIOSRC_PCMRTOS,                /*!< @brief Audio source device using pcm interface and rtos */
    LAST_AUDIOSRC_DEVICE_DRIVER_TYPE /*!< @brief Placeholder */
} AudioSrcDeviceDriverType;

/*!
 * @brief AudioSinkDeviceDriverType
 * @details Enumeration of potential audio sink devices
 *
 */
typedef enum
{
    AUDIO_SINK_DEVICE_TYPE_PCMRTOS,    /*!< @brief Audio sink device using pcm interface and rtos */
    LAST_AUDIO_SINK_DEVICE_DRIVER_TYPE /*!< @brief Placeholder */
} AudioSinkDeviceDriverType;

/*!
 * @brief StreamMsgType
 * Different message types that are available.
 */
typedef enum
{
    MESSAGE_UNKNOWN = 0,               /*!< @brief Unknown */
    MESSAGE_EOS     = GET_BIT(0),      /*!< @brief End-of-stream reached in a pipeline. The
                                        *         application will only receive this message in
                                        *         the PLAYING state and every time it sets a
                                        *         pipeline to PLAYING that is in the EOS state.
                                        *         The application can perform a flushing seek in
                                        *         the pipeline, which will undo the EOS state
                                        *         again. */
    MESSAGE_ERROR = GET_BIT(1),        /*!< @brief An error occurred. When the application receives
                                        *         an error message it should stop playback of the
                                        *         pipeline and not assume that more data will be played. */
    MESSAGE_WARNING   = GET_BIT(2),    /*!< @brief A warning occurred */
    MESSAGE_INFO      = GET_BIT(3),    /*!< @brief An info message occurred */
    MESSAGE_BUFFERING = GET_BIT(4),    /*!< @brief The pipeline is buffering. When the application
                                        *         receives a buffering msg in the PLAYING state
                                        *         for a non-live pipeline it must PAUSE the
                                        *         pipeline until the buffering completes, when the
                                        *         percent field in the message is 100%. For live
                                        *         pipelines, no action must be performed and the
                                        *         buffering percentage can be used to inform the
                                        *         user about the progress. */
    MESSAGE_STATE_CHANGED = GET_BIT(5) /*!< @brief A state change happened */
} StreamMsgType;

/*!
 * @brief Album Art Source
 * @details Identify the source of an album art image
 */
typedef enum
{
    ART_DEFAULT = 0, /*!< @brief No album art found, using default */
    ART_EMBEDDED,    /*!< @brief Art found embedded in source file */
    ART_FOLDER       /*!< @brief Art found in containing folder */
} AlbumArtSource;

/*!
 * @brief Image Format
 * @details Enumeration of output formats supported by the Image Decoder
 */
typedef enum
{
    IMG_RGB_555 = 0, /*!< @brief RGB 555 (16-bit) (PPM) */
    IMG_RGB_565,     /*!< @brief RGB 565 (16-bit) (PPM) */
    IMG_RGB_666,     /*!< @brief RGB 666 (18-bit) (PPM) */
    IMG_RGB_888,     /*!< @brief RGB 888 (24-bit) (PPM) */
    IMG_GRAYSCALE    /*!< @brief Grayscale (PGM) */
} ImageFormat;

/*!
 * @brief Album Art Object
 * @details Album Art associated with an album or track
 */
typedef struct _AlbumArt
{
    char src_path[PATHNAME_SIZE]; /*!< @brief Path to compressed src image */
    char *data;                   /*!< @brief Pointer to uncompressed image */
    ImageFormat format;           /*!< @brief Image Output Format */
    uint32_t height;              /*!< @brief Height of Image in Pixels */
    uint32_t width;               /*!< @brief Width of Image in Pixels */
    size_t image_size;            /*!< @brief image size*/
    AlbumArtSource source;        /*!< @brief Source Type of Album Art */
} AlbumArt;

/*!
 * @brief Track Info Object
 * @details Holds information regarding an Audio Track
 */
typedef struct _TrackInfo
{
    char title[TRACK_TITLE_LEN];
    /*!< @brief Song Title */
    char path[PATHNAME_SIZE];
    /*!< @brief Song Path */
    char artist[TRACK_ARTIST_LEN];
    /*!< @brief Artist Name */
    char album[TRACK_ALBUM_LEN];
    /*!< @brief Album Title */
    char genre[TRACK_GENRE_LEN];
    /*!< @brief Genre */
    char year[TRACK_YEAR_LEN];
    /*!< @brief Year of Recording */
    uint32_t track_number;
    /*!< @brief Track Number within album */
    bool art_embedded;
    /*!< @brief Album Art in file */
    uint32_t duration;
    /*!< @brief Track duration in milliseconds */
    int32_t channels;
    /*!< @brief channels */
    int32_t bit_rate;
    /*!< @brief bit rate */
    int32_t sample_rate;
    /*!< @brief sample rate */
    int32_t audio_type;
    /*!< @brief decoder type, relate to DecoderType in streamer_api.h */
    char codec_info[CODEC_INFO_LEN];
    /*!< @brief codec info */
    AlbumArt album_art;
    /*!< @brief Artwork for current track */
} TrackInfo;

/*!
 * @brief StreamImageType
 * @details Used to hold image data and specify format
 */
typedef struct
{
    int8_t *image_buffer;  /*!< @brief buffer pointer */
    int8_t bits_per_pixel; /*!< @brief bits per pixel */
    uint32_t height;       /*!< @brief image height */
    uint32_t width;        /*!< @brief image width */
    size_t image_size;     /*!< @brief image size */
} StreamImageType;

/*!
 * @brief StreamStringType
 * @details Used to hold string data
 */
typedef struct
{
    char *string_buffer; /*!< @brief buffer pointer */
    uint32_t num_chars;  /*!< @brief number of characters */
    uint8_t unicode;     /*!< @brief unicode flag */
} StreamStringType;

/*!
 * @brief StreamData
 * @details Holds the data related to a query or event
 */
typedef union
{
    uint32_t value32u;       /*!< @brief 32 bit unsigned value */
    uint16_t value16u;       /*!< @brief 16 bit unsigned value */
    uint8_t value8u;         /*!< @brief 8 bit unsigned value */
    int32_t value32s;        /*!< @brief 32 bit signed value */
    int16_t value16s;        /*!< @brief 16 bit signed value */
    int8_t value8s;          /*!< @brief 8 bit signed value */
    uint8_t valuebool;       /*!< @brief bool value */
    StreamStringType string; /*!< @brief string value */
    StreamImageType image;   /*!< @brief image value */
} StreamData;

/*!
 * @brief Element Property structure
 * @details Used to set/get element properties
 */
typedef struct
{
    uint16_t prop; /*!< @brief property type */
    uintptr_t val; /*!< @brief property value */
} ELEMENT_PROPERTY_T;

/*!
 * @brief Streamer Message Type
 * @details Structure for messages transmitted and received by the streamer task.
 */
typedef struct
{
    uint32_t id;                         /*!< @brief Message Identifier,
                                       relative with STREAMER_MSG_ID */
    int errorcode;                       /*!< @brief Error Code */
    int8_t pipeline_index;               /*!< @brief Pipeline Index */
    StreamPipelineType pipeline_type;    /*!< @brief Pipeline Type */
    ELEMENT_PROPERTY_T element_property; /*!< @brief Property Structure */
    PipelineState state;                 /*!< @brief Pipeline State */
    StreamInfoType query_type;           /*!< @brief Query Type */
    StreamData query_data;               /*!< @brief Query Return Data */
    uint32_t event_id;                   /*!< @brief Event Identifier */
    int event_data;                      /*!< @brief Event Data */
    uint32_t time_ms;                    /*!< @brief Time */
    const char *in_dev_name;             /*!< @brief Input Device Name*/
    const char *out_dev_name;            /*!< @brief Output Device Name*/
    OSA_MUTEX_HANDLE_DEFINE(mutex);      /*!< @brief Mutex to protect signal */
    OSA_EVENT_HANDLE_DEFINE(cond);       /*!< @brief Signal to unlock wait for streamer response */
    bool *processed;                     /*!< @brief Flag to indicate streamer msg processed */
    int32_t *ret;                        /*!< @brief streamer message processing result */
    void *get_value;                     /*!< @brief pointer to save the getting result */
} STREAMER_MSG_T;

/**
 * @brief Size of streamer message structure
 *
 */
#define STREAMER_MSG_SIZE (sizeof(STREAMER_MSG_T))

/*!
 * @brief Streamer Arguments
 * @details Structure for passing input arguments to the streamer task.
 * Note: A full message queue name will consist of a generic base name
 * concatenated with a unique identifier which is taken from the Thread ID for
 * the task that owns (receives messages on) the message queue.
 */
typedef struct
{
    char out_mq_name[STREAMER_MQ_NAME_LENGTH]; /*!< @brief msg queue to which streamer sends messages */
    uint32_t stack_size;                       /*!< @brief Stack size of streamer task */
    uint32_t interval;                         /*!< @brief task interval of streamer task */
    StreamPipelineType pipeline_type;          /*!< @brief streamer pipeline type */
    const char *in_dev_name;                   /*!< @brief input device name */
    const char *out_dev_name;                  /*!< @brief output device name */
    char *task_name;                           /*!< @brief task name */
} STREAMER_CREATE_PARAM;

/**
 * @brief Element handle
 *
 */
typedef uintptr_t ElementHandle;

/**
 * @brief Pipeline handle
 *
 */
typedef uintptr_t PipelineHandle;

/**
 * @brief Application handle
 *
 */
typedef uintptr_t APPHandle;

/*!
 * @brief Streamer Data Structure
 * @details Holds data used throughout the streamer task.
 * This includes:
 * - Array of Pipelines
 * - Array of Elements
 * - Exit Thread Flag
 * - Application Callback Function
 */
typedef struct
{
    PipelineHandle pipes[MAX_PIPELINES];
    /*!< @brief Array of pointers to pipeline objects */
    ElementHandle elems[MAX_PIPELINES][MAX_ELEMENTS];
    /*!< @brief Array of pointers to element objects */
    int8_t exit_thread;
    /*!< @brief Flag used to signify thread should be terminated. */
    osa_msgq_handle_t mq_out;
    /*!< @brief Pointer to Message Queue for sending message out */
    osa_msgq_handle_t streamer_mq;
    /*!< @brief Pointer to message queue for receive message */
    bool is_active;                                  /*!< @brief is tasking running or terminated */
    StreamPipelineType pipeline_type[MAX_PIPELINES]; /*!< @brief streamer pipeline type */
} STREAMER_T;

/*
 * GLOBAL VARIABLE DECLARATIONS
 */

/*
 * FUNCTION PROTOTYPES
 */

/**
 * @brief Create a streamer
 * @details Function for creating of a streamer object serving as the core of the framework.
 *          A streamer task is created that is responsible for handling the messages between the framework and the
 * application. Then a pipeline for audio processing is created using streamer_create_pipeline.
 *
 * @param task_param paremeters for creating the core streamer task
 * @return STREAMER_T*
 */
STREAMER_T *streamer_create(STREAMER_CREATE_PARAM *task_param);

/**
 * @brief Destroy a streamer object
 *
 * @param streamer The streamer object to be destroyed
 */
int32_t streamer_destroy(STREAMER_T *streamer);

/**
 * @brief Create a pipeline that will handle the different connected elements
 *
 * @param streamer streamer object
 * @param pipeline_id unique pipeline ID
 * @param pipeline_type type of the pipeline
 * @param in_dev_name input device name
 * @param out_dev_name output device name
 * @param block blocking / non-blocking creation of pipeline
 * called directly
 * @return int32_t error code
 */
int32_t streamer_create_pipeline(STREAMER_T *streamer,
                                 int32_t pipeline_id,
                                 StreamPipelineType pipeline_type,
                                 const char *in_dev_name,
                                 const char *out_dev_name,
                                 bool block);

/**
 * @brief Destroy a pipeline
 *
 * @param streamer  streamer object
 * @param pipeline_id unique pipeline ID
 * @param block blocking / non-blocking destruction of pipeline
 * @return int32_t error code
 */
int32_t streamer_destroy_pipeline(STREAMER_T *streamer, int32_t pipeline_id, bool block);

/**
 * @brief Get track information
 *
 * @param streamer streamer object
 * @param pipeline_id unique pipeline ID
 * @param info track information
 * @param block blocking / non-blocking mode
 * @return int32_t error code
 */
int32_t streamer_get_track_info(STREAMER_T *streamer, int32_t pipeline_id, TrackInfo *info, bool block);

/**
 * @brief Set streamer state
 *
 * @param streamer streamer object
 * @param pipeline_id unique pipeline ID
 * @param state streamer state
 * @param block blocking / non-blocking mode
 * @return int32_t error code
 */
int32_t streamer_set_state(STREAMER_T *streamer, int32_t pipeline_id, PipelineState state, bool block);

/**
 * @brief Get streamer state
 *
 * @param streamer streamer object
 * @param pipeline_id unique pipeline ID
 * @param state streamer state
 * @param block blocking / non-blocking mode
 * @return int32_t error code
 */
int32_t streamer_get_state(STREAMER_T *streamer, int32_t pipeline_id, PipelineState *state, bool block);

/**
 * @brief Query streamer information
 *
 * @param streamer streamer object
 * @param pipeline_id unique pipeline ID
 * @param infoType streamer information type
 * @param query_data query data
 * @param block blocking / non-blocking mode
 * @return int32_t error code
 */
int32_t streamer_query_info(
    STREAMER_T *streamer, int32_t pipeline_id, StreamInfoType infoType, StreamData *query_data, bool block);

/**
 * @brief Seek pipeline
 *
 * @param streamer streamer object
 * @param pipeline_id unique pipeline ID
 * @param seek_time_ms desired seek time in [ms]
 * @param block blocking / non-blocking mode
 * @return int32_t error code
 */
int32_t streamer_seek_pipeline(STREAMER_T *streamer, int32_t pipeline_id, int32_t seek_time_ms, bool block);

/**
 * @brief Set element property
 *
 * @param streamer streamer object
 * @param pipeline_id pipeline index
 * @param prop element property
 * @param block blocking / non-blocking mode
 * @return int32_t error code
 */
int32_t streamer_set_property(STREAMER_T *streamer, int8_t pipeline_id, ELEMENT_PROPERTY_T prop, bool block);

/**
 * @brief Get element property
 *
 * @param streamer streamer object
 * @param pipeline_id pipeline index
 * @param prop element property
 * @param val_ptr property value
 * @param block blocking / non-blocking mode
 * @return int32_t error code
 */
int32_t streamer_get_property(STREAMER_T *streamer, int8_t pipeline_id, uint16_t prop, uint32_t *val_ptr, bool block);

/**
 * @brief Set file source
 *
 * @param streamer streamer object
 * @param pipeline_id unique pipeline ID
 * @param path path to the file
 * @param state streamer state
 * @param block blocking / non-blocking mode
 * @return int32_t error code
 */
int32_t streamer_set_file(STREAMER_T *streamer, int32_t pipeline_id, char *path, PipelineState state, bool block);

/**
 * @brief Set pipeline to repeat
 *
 * @param streamer streamer object
 * @param pipeline_id unique pipeline ID
 * @param repeat_on repeat flag
 * @param block blocking / non-blocking mode
 * @return int32_t error code
 */
int32_t streamer_set_repeat(STREAMER_T *streamer, int32_t pipeline_id, bool repeat_on, bool block);

#endif /* STREAMER_API_H */
/* end of file */
