/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef AUDIO_SINK_H
#define AUDIO_SINK_H

/*!
 * @file    audio_sink.h
 * @brief   This file includes defines, enums and typedefs for audio sink.
 */

#include "streamer_element.h"

/*! Maximum number of APP allowed */
#define MAX_NO_OF_APP 1
/** @brief Get audio ID */
#define AUDIO_ID(pstaudio_packet) ((pstaudio_packet)->id)
/** @brief Get audio buffer */
#define AUDIO_BUFFER(pstaudio_packet) ((pstaudio_packet)->buffer)
/** @brief Get audio chunk size */
#define AUDIO_CHUNK_SIZE(pstaudio_packet) ((pstaudio_packet)->chunk_size)
/** @brief Get audio sample rate */
#define AUDIO_SAMPLE_RATE(pstaudio_packet) ((pstaudio_packet)->sample_rate)
/** @brief Get audio bits per sample */
#define AUDIO_BITS_PER_SAMPLE(pstaudio_packet) ((pstaudio_packet)->bits_per_sample)
/** @brief Get audio number of channels */
#define AUDIO_NUM_CHANNELS(pstaudio_packet) ((pstaudio_packet)->num_channels)
/** @brief Get audio format */
#define AUDIO_FORMAT(pstaudio_packet) ((pstaudio_packet)->format)
/** @brief Get audio volume */
#define AUDIO_VOLUME(pstaudio_packet) ((pstaudio_packet)->volume)
/*!
 * Extract Output format:
 * AUDIO_ENDIANNESS
 * AUDIO_INTERLEAVED
 * AUDIO_SIGNED
 * AUDIO_WORD_LENGTH
 *
 * bit (15-8)    bit (3)    bit (2)    bit (1)          bit (0)
 * output word   unused     signed/    little endian/   interleaved/
 * length                   unsigned   big endian       non-interleaved
 */

/** @brief Get interleaved flag */
#define AUDIO_INTERLEAVED(pstaudio_packet) ((pstaudio_packet->format >> 0) & 0x1)
/** @brief Get endiannes flag */
#define AUDIO_ENDIANNESS(pstaudio_packet) ((pstaudio_packet->format >> 1) & 0x1)
/** @brief Get signed flag */
#define AUDIO_SIGNED(pstaudio_packet) ((pstaudio_packet->format >> 2) & 0x1)
/** @brief Get word length flag */
#define AUDIO_WORD_LENGTH(pstaudio_packet) ((pstaudio_packet->format >> 4) & 0xFF)

/*!
 *@brief audio sink device select, reserved for extension devices
 */
typedef enum
{
    AUDIO_SINK_DEVICE_DEFAULT = 0,
    /*!< @brief default sink device */
    AUDIO_SINK_DEVICE_2,
    /*!< @brief second sink device */
    AUDIO_SINK_DEVICE_MAX
} AudioSinkDeviceName;

/*!
 * @brief audio sink device state
 */
typedef enum
{
    AUDIO_SINK_DEVICE_STATE_CLOSED = 0,
    /*!< @brief device is closed */
    AUDIO_SINK_DEVICE_STATE_OPENED,
    /*!< @brief device is opened */
    LAST_AUDIO_SINK_DEVICE_STATE_TYPE
} AudioSinkDeviceStateType;

/*!
 * @brief AudioSinkStreamErrorType
 * @details Potential Return Values (Error Codes) for Audio Sink Operations
 *
 */
typedef enum
{
    AUDIO_SINK_SUCCESS = 0,
    /*!< @brief Success */
    AUDIO_SINK_FAILED = -1,
    /*!< @brief Failed */
    AUDIO_SINK_ERROR_INVALID_ARGS = -2,
    /*!< @brief Invalid Arguments */
    AUDIO_SINK_ERROR_NEED_DATA = -3,
    /*!< @brief Error Need More Data */
    AUDIO_SINK_ERROR_OSA_ERROR = -4,
    /*!< @brief Operating System Abstraction Layer Error */
    AUDIO_SINK_ERROR_OUT_OF_HEAP_MEMORY = -5,
    /*!< @brief Error: Out of heap memory */
    AUDIO_SINK_ERROR_OUT_OF_STACK_MEMORY = -6,
    /*!< @brief Error: Out of stack memory */
    AUDIO_SINK_ERROR_UNDEFINED_STREAM_PROPERTY_TYPE = -7,
    /*!< @brief Undefined Stream Property Type */
    AUDIO_SINK_ERROR_READONLY_STREAM_PROPERTY_TYPE = -8,
    /*!< @brief Read Only Stream Property Type */
    AUDIO_SINK_ERROR_UNHANDLED_EVENT = -9,
    /*!< @brief Unhandled Event */
    AUDIO_SINK_ERROR_PAD = -10,
    /*!< @brief Pad Error */
    AUDIO_SINK_ERROR_DEVICE_OPEN_FAILED = -11,
    /*!< @brief Failed to Open Device */
    AUDIO_SINK_ERROR_DEVICE_CLOSE_FAILED = -12,
    /*!< @brief Failed to Close Device */
    AUDIO_SINK_ERROR_BAD_STATE = -13,
    /*!< @brief try to operate in a bad state */
    AUDIO_SINK_ERROR_NO_RESOURCE = -14,
    /*!< @brief can not find valid resource */
    AUDIO_SINK_ERROR_POST_PROC_INIT_FAILED = -15,
    /*!< @brief could not initialize post processing*/
    AUDIO_SINK_ERROR_POST_PROC_FAILED = -16,
    /*!< @brief Post processing failure*/
    AUDIO_SINK_ERROR_MAX
} AudioSinkStreamErrorType;

typedef struct _ElementAudioSink ElementAudioSink;

/*!
 * @brief open device function proto type
 */
typedef AudioSinkStreamErrorType (*AudioSinkOpenDeviceFunc)(ElementAudioSink *audio_sink_element);

/**
 * @brief close device function proto type
 *
 */
typedef AudioSinkStreamErrorType (*AudioSinkCloseDeviceFunc)(ElementAudioSink *audio_sink_element);
/**
 * @brief Start device function proto type
 *
 */
typedef AudioSinkStreamErrorType (*AudioSinkStartDeviceFunc)(ElementAudioSink *audio_sink_element);
/**
 * @brief Stop device function proto type
 *
 */
typedef AudioSinkStreamErrorType (*AudioSinkStopDeviceFunc)(ElementAudioSink *audio_sink_element);
/**
 * @brief Set volume function proto type
 *
 */
typedef AudioSinkStreamErrorType (*AudioSinkSetVolumeFunc)(ElementAudioSink *audio_sink_element, int volume);

/*!
 * @brief sink pad device IO functions
 * @details Those functionalities are used to control sink devices,
 * and can be provided by sub devices like SSI, ALSA etc,
 * they are installed when a property PROP_AUDIOSINK_DEVICE_DRIVER_TYPE is set.
 */
typedef struct
{
    AudioSinkOpenDeviceFunc open_device_func;   /*!< @brief open device */
    AudioSinkCloseDeviceFunc close_device_func; /*!< @brief close device */
    PadActivateFunc activate_func;              /*!< @brief activate device */
    PadChainFunc chain_func;                    /*!< @brief receive stream data */
    PadEventFunc event_func;                    /*!< @brief event function */
    PadQueryFunc query_func;                    /*!< @brief info query function */
    PadProcessFunc proc_func;                   /*!< @brief process function */
    AudioSinkStartDeviceFunc start_device_func; /*!< @brief start device function */
    AudioSinkStopDeviceFunc stop_device_func;   /*!< @brief stop device function */
    AudioSinkStopDeviceFunc play_device_func;   /*!< @brief play device function */
    AudioSinkStopDeviceFunc pause_device_func;  /*!< @brief pause device function */
    AudioSinkSetVolumeFunc set_volume_func;     /*!< @brief set volume function */
} AudioSinkPadDeviceIOFuncType;

/*!
 * @brief audio sink element's data structures
 * @details This data structure is created by create_element,
 * initialized by audiosink_init_element
 */
struct _ElementAudioSink
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
    uint32_t chunk_size;                        /*!< @brief chunk size */
    uint32_t bits_per_sample;                   /*!< @brief bits per sample */
    uint32_t num_channels;                      /*!< @brief number of audio channels */
    uint32_t sample_rate;                       /*!< @brief sample rate per second */
    uint32_t format;                            /*!< @brief PCM, endian, word length */
    int volume;                                 /*!< @brief Output volume */
    int32_t error_element;                      /*!< @brief saved error type */
    uint32_t time_reported_msec;                /*!< @brief last elapsed time reported to application */
    uint32_t time_report_diff;   /*!< @brief Amount of time that should have passed before sending another message*/
    uint32_t elapsed_time_msec;  /*!< @brief elapsed time in million seconds */
    uint64_t elapsed_time_usec;  /*!< @brief elapsed time In micro seconds */
    uint32_t device_driver_type; /*!< @brief device driver type specific */
    uint32_t device_name; /*!< @brief This is provided just in case an enumeration is done by ALSA or OSS probably
                             before opening the device.*/
    char device_string_name[MAX_DEVICE_NAME_LENGTH]; /*!< @brief Device string name */
    void *device_info;                               /*!< @brief sink device infomations about
                                                         sink device like SSI, ALSA, etc*/
    const AudioSinkPadDeviceIOFuncType *device_ptr;  /*!< @brief device operations
                                              provided by sink device */
    uint32_t last_time_sample_rate;                  /*!< @brief Last time sample rate */
    bool buffer_use_chunk_size;                      /*!< @brief Use the configured chunk
                                                          size for buffer sizing */
    uint32_t last_num_channels;                      /*!< @brief last time number of audio channels */
    uint32_t last_chunk_size;                        /*!< @brief last time chunk size */

    bool device_started;           /*!< Indicates whether the device was started */
    uint8_t codec_num_channels;    /*!< @brief Number of channels for HW codec*/
    uint8_t codec_bits_per_sample; /*!< @brief Sample width for HW codec*/

    ElementHandle refData_element; /*!< @brief Element handle where reference audio data will be sent (VoiceSeeker) */
};

/*!
 *
 * @param element audio sink element
 *
 * @brief This function initializes audio sink element and its sink pads
 *
 * @returns AudioSinkStreamErrorType
 *
 * @retval AUDIO_SINK_SUCCESS success
 * @retval AUDIO_SINK_ERROR_INVALID_ARGS invalid argument
 *
 */
int32_t audiosink_init_element(StreamElement *element);

/*!
 *
 * @param element audio sink element
 * @param device_driver_type set audio device driver type
 * relate with AudioSinkDeviceDriverType
 *
 * @brief This function sets the audio driver type as SSI or ALSA.
 * Now, only a SSI interface is implemented.
 * @note This function should be called only at STATE_NULL state
 *
 * @returns AudioSinkStreamErrorType
 *
 * @retval AUDIO_SINK_SUCCESS success
 * @retval AUDIO_SINK_ERROR_INVALID_ARGS invalid argument
 * @retval AUDIO_SINK_ERROR_BAD_STATE if element state is not STATE_NULL
 */
AudioSinkStreamErrorType audiosink_set_device_driver_type(ElementHandle element,
                                                          AudioSinkDeviceDriverType device_driver_type);

/*!
 *
 * @param element audio sink element
 * @param device_name set audio device name
 *
 * @brief This function sets the audio device name.
 * This API is provided just in case an
 * enumeration is done by ALSA or OSS probably before opening the device.
 *
 * @returns AudioSinkStreamErrorType
 * @retval AUDIO_SINK_SUCCESS success
 * @retval AUDIO_SINK_ERROR_INVALID_ARGS invalid arguments
 */
AudioSinkStreamErrorType audiosink_set_device_name(ElementHandle element, AudioSinkDeviceName device_name);

/*!
 *
 * @param element: audio sink element
 * @param device_string_name: set audio device string name
 *
 * @brief This function sets the audio device string name.
 *
 * @returns AudioSinkStreamErrorType
 * @retval AUDIO_SINK_SUCCESS
 * @retval AUDIO_SINK_ERROR_INVALID_ARGS
 */
AudioSinkStreamErrorType audiosink_set_device_string_name(ElementHandle element, char *device_string_name);

#endif
