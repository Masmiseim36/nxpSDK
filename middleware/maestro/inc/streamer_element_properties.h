/*
 * Copyright 2018-2023 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
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
    int preset_num;     /*!< @brief Preset number */
    int num_channels;   /*!< @brief Number of channels*/
    int sample_rate;    /*!< @brief Sample rate*/
    uint8_t xo_enabled; /*!< @brief Crossover enabled*/
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

/* AUDIO SOURCE */
#define PROP_AUDIOSRC_MASK                0x200
#define PROP_AUDIOSRC_SET_DEVICE_TYPE     0x200
#define PROP_AUDIOSRC_SET_CHUNK_SIZE      0x201
#define PROP_AUDIOSRC_SET_SAMPLE_RATE     0x202
#define PROP_AUDIOSRC_GET_CHUNK_SIZE      0x203
#define PROP_AUDIOSRC_SET_DEVICE_NAME     0x204
#define PROP_AUDIOSRC_SET_CONTINUOUS_READ 0x205
#define PROP_AUDIOSRC_SET_DUMMY_TX_ENABLE 0x206
#define PROP_AUDIOSRC_SET_NUM_CHANNELS    0x207
#define PROP_AUDIOSRC_SET_BITS_PER_SAMPLE 0x208
#define PROP_AUDIOSRC_SET_FRAME_MS        0x209

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

/* AUDIO SINK */
#define PROP_AUDIOSINK_MASK                      0x700
#define PROP_AUDIOSINK_DEVICE_DRIVER_TYPE        0x700
#define PROP_AUDIOSINK_TIME_UPDATE_MS            0x701
#define PROP_AUDIOSINK_DEVICE_DRIVER_STRING_NAME 0x702
#define PROP_AUDIOSINK_BUFFER_USE_CHUNK_SIZE     0x703
#define PROP_AUDIOSINK_SET_VOLUME                0x704
#define PROP_AUDIOSINK_SET_REFDATA_ELEMENT       0x705

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
#define PROP_VITSINK_PROC_MASK    0xb00
#define PROP_VITSINK_PROC_FUNCPTR 0xb00

/* ENCODER */
#define PROP_ENCODER_MASK          0xc00
#define PROP_ENCODER_CHUNK_SIZE    0xc00
#define PROP_ENCODER_TYPE          0xc01
#define PROP_ENCODER_CONFIG        0xc02
#define PROP_ENCODER_BITSTREAMINFO 0xc03

/* EAP_PROC */
#define PROP_EAP_PROC_MASK    0xd00
#define PROP_EAP_PROC_FUNCPTR 0xd00

/* VOICESEEKER_PROC */
#define PROP_VOICESEEKER_PROC_MASK                0xe00
#define PROP_VOICESEEKER_PROC_FUNCPTR             0xe00
#define PROP_VOICESEEKER_PROC_REFDATA_FUNCPTR     0xe01
#define PROP_VOICESEEKER_PROC_REFDATA_NUM_BUFFERS 0xe02
#define PROP_VOICESEEKER_PROC_REFDATA_PUSH        0xe03
#define PROP_VOICESEEKER_PROC_SET_DEBUGGING       0xe04

/* SRC_PROC */
#define PROP_SRC_PROC_MASK    0xf00
#define PROP_SRC_PROC_FUNCPTR 0xf00

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

#endif
