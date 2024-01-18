/*
 * Copyright 2018-2023 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __STREAMER_ELEMENT_PROPERTIES_H__
#define __STREAMER_ELEMENT_PROPERTIES_H__
#include "fsl_common.h"
/*!
 * @file    streamer_element_properties.h
 * @brief   Header for streamer element properties
 * @details This header file contains all the element properties.
 * To set any element property, streamer_set_property function can be used.
 * To get any property value, streamer_get_property function can be used.
 */

/**
 * @brief External processing funcion prototypes and arguments
 *
 */
typedef struct
{
    int (*init_func)(void *);              /*!< @brief Init func pointer */
    int (*proc_func)(void *, void *, int); /*!< @brief Process func pointer */
    int (*deinit_func)(void);              /*!< @brief Deinit func pointer */
    void *arg_ptr;                         /*!< @brief Arguments pointer */
} EXT_PROCESS_DESC_T;

/**
 * @brief Structure for arguments of post processing
 *
 */
typedef struct
{
    int num_channels; /*!< @brief Number of channels*/
    int sample_rate;  /*!< @brief Sample rate*/
} ext_proc_args;

/**
 * @brief Set buffer description
 *
 */
typedef struct
{
    int8_t *ptr;   /*!< @brief Buffer pointer */
    uint32_t size; /*!< @brief Buffer size */
} SET_BUFFER_DESC_T;

/**
 * @brief String descrtiption
 *
 */
typedef struct
{
    int8_t *str;  /*!< @brief String pointer */
    uint32_t len; /*!< @brief String length */
} STRING_DESC_T;

#define PROP_ELEMENT_MASK (0xFF00)

/* FILE SOURCE */
#define PROP_FILESRC_MASK             0x100
#define PROP_FILESRC_SET_LOCATION     0x100
#define PROP_FILESRC_SET_CHUNK_SIZE   0x101
#define PROP_FILESRC_GET_CHUNK_SIZE   0x102
#define PROP_FILESRC_SET_FILE_TYPE    0x103
#define PROP_FILESRC_SET_SAMPLE_RATE  0x104
#define PROP_FILESRC_SET_NUM_CHANNELS 0x105
#define PROP_FILESRC_SET_BIT_WIDTH    0x106

/* MICROPHONE */
#define PROP_MICROPHONE_MASK                0x200
#define PROP_MICROPHONE_SET_DEVICE_TYPE     0x200
#define PROP_MICROPHONE_SET_CHUNK_SIZE      0x201
#define PROP_MICROPHONE_SET_SAMPLE_RATE     0x202
#define PROP_MICROPHONE_GET_CHUNK_SIZE      0x203
#define PROP_MICROPHONE_SET_DEVICE_NAME     0x204
#define PROP_MICROPHONE_SET_CONTINUOUS_READ 0x205
#define PROP_MICROPHONE_SET_DUMMY_TX_ENABLE 0x206
#define PROP_MICROPHONE_SET_NUM_CHANNELS    0x207
#define PROP_MICROPHONE_SET_BITS_PER_SAMPLE 0x208
#define PROP_MICROPHONE_SET_FRAME_MS        0x209
#define PROP_MICROPHONE_SET_APP_FUNCTIONS   0x20A

/* MEMORY SOURCE */
#define PROP_MEMSRC_MASK             0x300
#define PROP_MEMSRC_SET_BUFF         0x300
#define PROP_MEMSRC_SET_CHUNK_SIZE   0x301
#define PROP_MEMSRC_GET_CHUNK_SIZE   0x302
#define PROP_MEMSRC_SET_MEM_TYPE     0x303
#define PROP_MEMSRC_SET_SAMPLE_RATE  0x304
#define PROP_MEMSRC_SET_NUM_CHANNELS 0x305
#define PROP_MEMSRC_SET_BIT_WIDTH    0x306

/* DECODER */
#define PROP_DECODER_MASK         0x400
#define PROP_DECODER_DECODER_TYPE 0x400
#define PROP_DECODER_PARSE_TAG    0x401

/* SPEAKER */
#define PROP_SPEAKER_MASK                      0x700
#define PROP_SPEAKER_DEVICE_DRIVER_TYPE        0x700
#define PROP_SPEAKER_TIME_UPDATE_MS            0x701
#define PROP_SPEAKER_DEVICE_DRIVER_STRING_NAME 0x702
#define PROP_SPEAKER_BUFFER_USE_CHUNK_SIZE     0x703
#define PROP_SPEAKER_SET_VOLUME                0x704
#define PROP_SPEAKER_SET_REFDATA_ELEMENT       0x705
#define PROP_SPEAKER_SET_APP_FUNCTIONS         0x706

/* FILE SINK */
#define PROP_FILESINK_MASK           0x800
#define PROP_FILESINK_LOCATION       0x800
#define PROP_FILESINK_RAW_WRITE_MODE 0x801

/* MEMORY SINK */
#define PROP_MEMSINK_MASK           0x900
#define PROP_MEMSINK_BUFFER_DESC    0x900
#define PROP_MEMSINK_RAW_WRITE_MODE 0x901

/* NETBUF SOURCE */
#define PROP_NETBUFSRC_MASK         0xa00
#define PROP_NETBUFSRC_SET_CALLBACK 0xa00

/* VIT SINK */
#define PROP_VITSINK_MASK    0xb00
#define PROP_VITSINK_FUNCPTR 0xb00

/* ENCODER */
#define PROP_ENCODER_MASK          0xc00
#define PROP_ENCODER_CHUNK_SIZE    0xc00
#define PROP_ENCODER_TYPE          0xc01
#define PROP_ENCODER_CONFIG        0xc02
#define PROP_ENCODER_BITSTREAMINFO 0xc03

/* VOICESEEKER */
#define PROP_VOICESEEKER_MASK                0xd00
#define PROP_VOICESEEKER_FUNCPTR             0xd00
#define PROP_VOICESEEKER_REFDATA_FUNCPTR     0xd01
#define PROP_VOICESEEKER_REFDATA_NUM_BUFFERS 0xd02
#define PROP_VOICESEEKER_REFDATA_PUSH        0xd03
#define PROP_VOICESEEKER_SET_DEBUGGING       0xd04

/* SRC */
#define PROP_SRC_MASK    0xe00
#define PROP_SRC_FUNCPTR 0xe00

/* USB_SRC */
#define PROP_USB_SRC_MASK                0xf00
#define PROP_USB_SRC_SET_DEVICE_TYPE     0xf00
#define PROP_USB_SRC_SET_CHUNK_SIZE      0xf01
#define PROP_USB_SRC_SET_SAMPLE_RATE     0xf02
#define PROP_USB_SRC_GET_CHUNK_SIZE      0xf03
#define PROP_USB_SRC_SET_DEVICE_NAME     0xf04
#define PROP_USB_SRC_SET_CONTINUOUS_READ 0xf05
#define PROP_USB_SRC_SET_DUMMY_TX_ENABLE 0xf06
#define PROP_USB_SRC_SET_NUM_CHANNELS    0xf07
#define PROP_USB_SRC_SET_BITS_PER_SAMPLE 0xf08
#define PROP_USB_SRC_SET_FRAME_MS        0xf09
#define PROP_USB_SRC_SET_APP_FUNCTIONS   0xf0A

/* USB_SINK */
#define PROP_USB_SINK_MASK                      0x1000
#define PROP_USB_SINK_DEVICE_DRIVER_TYPE        0x1000
#define PROP_USB_SINK_TIME_UPDATE_MS            0x1001
#define PROP_USB_SINK_DEVICE_DRIVER_STRING_NAME 0x1002
#define PROP_USB_SINK_BUFFER_USE_CHUNK_SIZE     0x1003
#define PROP_USB_SINK_SET_VOLUME                0x1004
#define PROP_USB_SINK_SET_REFDATA_ELEMENT       0x1005
#define PROP_USB_SINK_SET_APP_FUNCTIONS         0x1006

/* ASRC */
#define PROP_ASRC_MASK    0x1100
#define PROP_ASRC_FUNCPTR 0x1100

/* VIT_PROC */
#define PROP_VIT_PROC_MASK    0x1200
#define PROP_VIT_PROC_FUNCPTR 0x1200

/*! Structure for setting memory source buffer*/
typedef struct
{
    int8_t *location; /*!< @brief Buffer pointer */
    uint32_t size;    /*!< @brief Buffer size */
} MEMSRC_SET_BUFFER_T;

/*! Structure for setting audio source name*/
typedef struct
{
    const char *device_name;        /*!< @brief Device name */
    const char *output_device_name; /*!< @brief Output device name */
} AUDSRC_SET_NAME_T;

/*! Structure for setting queue buffer*/
typedef struct
{
    int8_t *qbuffer;      /*!< @brief Buffer pointer */
    uint32_t buffer_size; /*!< @brief Buffer size */
    uint32_t sector_size; /*!< @brief Sector size */
} QUEUE_SET_BUFFER_T;

/**
 * @brief External processing funcion prototypes for process reference data (VoiceSeeker)
 *
 */
typedef struct
{
    int (*set_num_buff_func)(int); /*!< @brief Set num buffer func pointer */
    int (*push_func)(void *);      /*!< @brief Push func pointer */
    int (*set_debugging)(bool);    /*!< @brief Set debugging func pointer */
} EXT_PROCESS_REFDAT_DESC_T;

/**
 * @brief Reference audio data structure (due to VoiceSeeker AEC)
 *
 */
typedef struct
{
    uint8_t *buffer; /*!< @brief pointers to reference data */
    uint16_t size;   /*!< @brief size of reference data */
} AudioRefData_t;

/*!
 * @brief Audio sink application functions
 * @details Those functionalities are defined in the application and are used to control audio sink devices.
 */
typedef struct
{
    int (*open_func)(uint32_t num_buffers);            /*!< @brief open */
    void (*close_func)(void);                          /*!< @brief close */
    void (*start_func)(void);                          /*!< @brief start */
    int (*process_func)(uint8_t *data, uint32_t size); /*!< @brief write / read function */
    int (*set_param_func)(uint32_t sample_rate,
                          uint32_t bit_width,
                          uint8_t num_channels,
                          bool transfer,
                          bool dummy_tx,
                          int volume); /*!< @brief set parammeters */
    void (*get_param_func)(uint32_t *sample_rate,
                           uint32_t *bit_width,
                           uint8_t *num_channels); /*!< @brief get parammeters */
    int (*mute_func)(bool mute);                   /*!< @brief mute / unmute */
    int (*volume_func)(int volume);                /*!< @brief set volume */
} EXT_AUDIOELEMENT_DESC_T;

#endif
