/*
 * Copyright 2018-2023 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef STREAMER_H
#define STREAMER_H
/*!
 * @file    streamer.h
 * @brief   Contains common types and functions used throughout the streamer
 */

/*
 * INCLUDE FILES
 */

#include <stdint.h>
#include "error.h"
#include "maestro_logging.h"

#include "streamer_api.h"

/*
 * CONSTANTS, DEFINES AND MACROS
 */

/** @brief Flag enable debug messages */
#define DBG_ON (0x80)
/** @brief Flag disable debug messages */
#define DBG_OFF (0x00)
/** @brief Flag enable core debug messages */
#define DBG_CORE DBG_ON
/** @brief Flag enable audio sink debug messages */
#define DBG_AUDIO_SINK DBG_ON
/** @brief Flag enable audio source debug messages */
#define DBG_AUDIO_SRC DBG_ON
/** @brief Flag enable queue debug messages */
#define DBG_QUEUE DBG_OFF
/** @brief Flag enable file source debug messages */
#define DBG_FILESRC DBG_ON
/** @brief Flag enable file sink debug messages */
#define DBG_FILE_SINK DBG_ON
/** @brief Flag enable decoder debug messages */
#define DBG_DECODER DBG_ON
/** @brief Flag enable encoder debug messages */
#define DBG_ENCODER DBG_ON
/** @brief Flag enable mp3 decoder debug messages */
#define DBG_MP3D DBG_ON
/** @brief Flag enable memory source debug messages */
#define DBG_MEMSRC DBG_ON
/** @brief Flag enable netbuf source debug messages */
#define DBG_NETBUFSRC DBG_ON
/** @brief Flag enable memory sink debug messages */
#define DBG_MEM_SINK DBG_ON
/** @brief Flag enable vit sink debug messages */
#define DBG_VIT_SINK DBG_ON
/** @brief Flag enable audio_proc debug messages */
#define DBG_AUDIO_PROC DBG_ON
/** @brief Flag enable cci decoder debug messages */
#define DBG_CCID DBG_ON
/** @brief Flag enable thread debug messages */
#define DBG_THREAD DBG_ON

/** @brief Number of sink pads*/
#define NUM_SINKS(type) ((type >> 14) & 0x3)
/** @brief Number of source pads*/
#define NUM_SRCS(type) ((type >> 12) & 0x3)
/** @brief Is source element */
#define IS_SRC_ELEMENT(type) (NUM_SRCS(type) > 0 && NUM_SINKS(type) == 0)
/** @brief Is sink element */
#define IS_SINK_ELEMENT(type) (NUM_SINKS(type) > 0 && NUM_SRCS(type) == 0)

/*! Debug printf macros.
 *  @param on is one of the DBG_ categories listed above.  If it has been defined as
 *     DBG_ON, then the message will print.
 *  @param error Error to be print
 */
#define STREAMER_LOG_CATA(on, error, ...) \
    if (on & DBG_ON)                      \
    lprintf(LOGMDL_STREAMER, LOGLVL_CATASTROPHIC, error, __VA_ARGS__)
/** @brief Log error */
#define STREAMER_LOG_ERR(on, error, ...) \
    if (on & DBG_ON)                     \
    lprintf(LOGMDL_STREAMER, LOGLVL_ERROR, error, __VA_ARGS__)
/** @brief Log warning */
#define STREAMER_LOG_WARN(on, error, ...) \
    if (on & DBG_ON)                      \
    lprintf(LOGMDL_STREAMER, LOGLVL_WARNING, error, __VA_ARGS__)
/** @brief Log debug message */
#define STREAMER_LOG_DEBUG(on, ...) \
    if (on & DBG_ON)                \
    dlprintf(LOGMDL_STREAMER, __VA_ARGS__)
/** @brief Log information */
#define STREAMER_LOG_INFO(...) ilprintf(LOGMDL_STREAMER, __VA_ARGS__)
/** @brief Debug log function enter */
#define STREAMER_FUNC_ENTER(on) \
    if (on > 0)                 \
    LOG_ENTER(LOGMDL_STREAMER)
/** @brief Debug log function exit */
#define STREAMER_FUNC_EXIT(on) \
    if (on > 0)                \
    LOG_EXIT(LOGMDL_STREAMER)

/** @brief Macro for aligning memory */
#define MEM4_ALIGN(n) ((uintptr_t)(n) + (-(uintptr_t)(n)&3))
/** @brief Macro to change a value to a given size aligned value */
#define MEM_ALIGN(var, alignbytes) \
    ((unsigned int)((var) + ((alignbytes)-1U)) & (unsigned int)(~(unsigned int)((alignbytes)-1U)))

/** @brief Check arguments */
#define CHK_ARGS(check, error) \
    do                         \
    {                          \
        if ((check))           \
        {                      \
            return error;      \
        }                      \
    } while (0)

/** @brief Little endiannes */
#define AF_LITTLE_ENDIAN false
/** @brief Big endiannes */
#define AF_BIG_ENDIAN true
/** @brief File raw write */
#define FILE_RAW_WRITE 1

/**
 * Data Packets
 */

/** @brief Raw data */
#define RAW_DATA 0
/** @brief Audio data */
#define AUDIO_DATA 1

/*! Default output device */
#define STREAMER_DEFAULT_OUT_DEVICE AUDIO_SINK_DEVICE_TYPE_PCMRTOS

/*! Default source read chunk size for push mode */
#define STREAMER_DEFAULT_CHUNK_SIZE (2048)

/*! Lossless formats need a bit large chunk size to play smoothly */
/* XXX: WAVE chunk default should be 4k for optimal performance.
 *      Reduced to 512 bytes for SD card reads out of DRAM. */
#define STREAMER_WAVE_FILE_CHUNK_SIZE (4096)

/*! Automatically report duration */
#define STREAMER_DEFAULT_UPDATE_DURATION (1000)

/*
 * STRUCTURES AND OTHER TYPEDEFS
 */

/**
 * @brief PacketHeader
 *
 */
typedef struct
{
    uint32_t id; /*!< @brief id should always be the first field */
} PacketHeader;

/*! Structure for Audio Packet Header properties */
typedef struct
{
    uint32_t id;              /*!< @brief id should always be the first field */
    uint32_t sample_rate;     /*!< @brief sample rate */
    uint32_t bits_per_sample; /*!< @brief bits per sample */
    uint32_t num_channels;    /*!< @brief number of channels */
    uint32_t format;          /*!< @brief format */
    uint32_t chunk_size;      /*!< @brief chunk size */
    uint8_t endian;           /*!< @brief endiannes */
    uint8_t sign;             /*!< @brief sign */
    int8_t padding[6];        /*!< @brief To keep the header 32 byte aligned */
} AudioPacketHeader;

/**
 * @brief Raw packet header
 *
 */
typedef struct
{
    uint32_t id; /*!< @brief id should always be the first field */
    /* fill in */
} RawPacketHeader;

/**
 * @brief Stream buffer information
 *
 */
typedef struct
{
    int8_t *buffer;  /*!< @brief Buffer pointer */
    uint32_t size;   /*!< @brief Buffer size */
    uint32_t offset; /*!< @brief Buffer offset */
    uint32_t time;   /*!< @brief Buffer time */
} StreamBuffer;

/*!
 * StreamDataType
 * Specifies the datatype of query information
 */
typedef enum
{
    DATA_TYPE_UINT32 = 1,
    DATA_TYPE_UINT16,
    DATA_TYPE_UINT8,
    DATA_TYPE_INT32,
    DATA_TYPE_INT16,
    DATA_TYPE_INT8,
    DATA_TYPE_bool,
    DATA_TYPE_STRING,
    DATA_TYPE_IMAGE,

    DATA_TYPE_LAST /* Should always be at the end */
} StreamDataType;

/*
 * STRUCTURES AND OTHER TYPEDEFS
 */

/*!
 * Streamer Message Handler Entry
 * Specifies a handler function to be associated with a message ID
 */
typedef struct
{
    STREAMER_MSG_ID id;
    /*!< @brief Message Identifier */
    int (*handler)(STREAMER_T *, void *);
    /*!< @brief Function to handle the associated message */
} STREAMER_MSG_HANDLER_T;

/*!
 * @struct StreamMessage
 * @details StreamMessage structure. Pipeline elements will communicate with the
 * application using this message format. All msgs are defined in streamer_message.h
 */
typedef struct
{
    ElementHandle src_element; /*!< @brief Element handle of msg source */
    uint32_t message;          /*!< @brief Message type */
    uintptr_t data;            /*!< @brief Message data */
} StreamMessage;

/*!
 * @enum StreamDataFormat
 * @details Standard predefined event data formats
 */
typedef enum
{
    DATA_FORMAT_UNDEFINED,
    DATA_FORMAT_BYTES,
    DATA_FORMAT_TIME,
    DATA_FORMAT_PERCENT,
    DATA_FORMAT_UNKNOWN

} StreamDataFormat;

/*!
 * StreamQuery
 */
typedef struct
{
    StreamInfoType type;      /*!< @brief Query type */
    StreamDataFormat format;  /*!< @brief Query format */
    StreamDataType data_type; /*!< @brief Query data type */
    StreamData *data;         /*!< @brief Query data */
} StreamQuery;

/*
 * FUNCTION PROTOTYPES
 */

/*!
 * @brief   Streamer Task
 * @details This is the streamer task function that processes the main streamer
 *          loop.  The loop will look to receive messages from the application
 *          and to initiate the processing of data through active pipelines.
 *
 * @param   [in] arg   Pointer to input argument structure (STREAMER_ARGS_T)
 * @returns N/A
 *
 */
void streamer_task(void *arg);
/*!
 * @brief    Streamer Initialize
 * @details  Initialize data structure associated with Streamer Task
 *
 * @param    [in] streamer  Pointer to streamer task data structure
 */
void streamer_init(STREAMER_T *streamer);
/*!
 * @brief    Streamer Message Handler
 * @details  Processes a single message that was received in the streamer task
 *           message queue. Messages should follow the format defined in
 *           STREAMER_MSG_T structure.
 *
 * @param    [in] streamer  Pointer to streamer task data structure
 * @param    [in] msg Pointer to message data.
 *
 * @retval  ERRCODE_NO_ERROR    Message was successfully processed
 * @retval  ERRCODE_INVALID_ARGUMENT     Invalid Message
 *
 * @return int
 */
int streamer_msg_handler(STREAMER_T *streamer, STREAMER_MSG_T *msg);
/*!
 * @brief    Streamer Process Pipelines
 * @details  Loops through the pipelines present in the streamer task and if
 *           the pipeline is in Playing state calls the process pipeline
 *           function to start the flow of data.
 *
 * @param    [in] streamer Pointer to streamer task data structure
 *
 * @retval  ERRCODE_NO_ERROR    Message was successfully processed
 * @retval  ERRCODE_INVALID_ARGUMENT     Invalid Message
 *
 * @return int
 */
int streamer_process_pipelines(STREAMER_T *streamer);

/* Message Handlers */
int streamer_msg_create_pipeline(STREAMER_T *streamer, void *data);
int streamer_msg_destroy_pipeline(STREAMER_T *streamer, void *data);
int streamer_msg_get_track_info(STREAMER_T *streamer, void *msg_data);
int streamer_msg_set_state(STREAMER_T *streamer, void *data);
int streamer_msg_get_state(STREAMER_T *streamer, void *data);
int streamer_msg_send_query(STREAMER_T *streamer, void *data);
int streamer_msg_seek_pipeline(STREAMER_T *streamer, void *data);
int streamer_msg_set_property(STREAMER_T *streamer, void *data);
int streamer_msg_get_property(STREAMER_T *streamer, void *data);
int streamer_msg_set_file(STREAMER_T *streamer, void *data);
int streamer_msg_set_repeat(STREAMER_T *streamer, void *msg_data);

#endif /* STREAMER_H */

/* End of File */
