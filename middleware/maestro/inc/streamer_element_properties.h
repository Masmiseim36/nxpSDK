/*
 * Copyright 2018-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef __STREAMER_ELEMENT_PROPERTIES_H__
#define __STREAMER_ELEMENT_PROPERTIES_H__

/*!
 * @file    streamer_element_properties.h
 * @brief   Header for streamer element properties
 */

typedef struct {
    int (*init_func) (void *);
    int (*proc_func) (void *, void *, int);
    int (*deinit_func) (void);
    void *arg_ptr;
    uint8_t  bl_enabled;
} EXT_PROCESS_DESC_T;

/*! Structure for arguments of post processing */
typedef struct {
	int preset_num;
	int num_channels;
	int sample_rate;
    uint8_t xo_enabled;
} ext_proc_args;

/*! Structure for Set Buffer */
typedef struct {
    int8_t    *ptr;
    uint32_t   size;
} SET_BUFFER_DESC_T;

/*! Structure for string */
typedef struct {
    int8_t    *str;
    uint32_t   len;
} STRING_DESC_T;


#define PROP_ELEMENT_MASK                           (0xFF00)

/* FILE SOURCE */
#define PROP_FILESRC_MASK                           0x100
#define PROP_FILESRC_SET_LOCATION                   0x100
#define PROP_FILESRC_SET_CHUNK_SIZE                 0x101
#define PROP_FILESRC_GET_CHUNK_SIZE                 0x102
#define PROP_FILESRC_SET_FILE_TYPE                  0x103
#define PROP_FILESRC_SET_SAMPLE_RATE                0x104
#define PROP_FILESRC_SET_NUM_CHANNELS               0x105
#define PROP_FILESRC_SET_BIT_WIDTH                  0x106

/* AUDIO SOURCE */
#define PROP_AUDIOSRC_MASK                          0x200
#define PROP_AUDIOSRC_SET_DEVICE_TYPE               0x200
#define PROP_AUDIOSRC_SET_CHUNK_SIZE                0x201
#define PROP_AUDIOSRC_SET_SAMPLE_RATE               0x202
#define PROP_AUDIOSRC_GET_CHUNK_SIZE                0x203
#define PROP_AUDIOSRC_SET_DEVICE_NAME               0x204
#define PROP_AUDIOSRC_SET_CONTINUOUS_READ           0x205
#define PROP_AUDIOSRC_SET_DUMMY_TX_ENABLE           0x206
#define PROP_AUDIOSRC_SET_NUM_CHANNELS              0x207
#define PROP_AUDIOSRC_SET_BITS_PER_SAMPLE           0x208
#define PROP_AUDIOSRC_SET_FRAME_MS                  0x209

/* MEMORY SOURCE */
#define PROP_MEMSRC_MASK                            0x300
#define PROP_MEMSRC_SET_BUFF                        0x300
#define PROP_MEMSRC_SET_CHUNK_SIZE                  0x301
#define PROP_MEMSRC_GET_CHUNK_SIZE                  0x302

/* DECODER */
#define PROP_DECODER_MASK                           0x400
#define PROP_DECODER_DECODER_TYPE                   0x400
#define PROP_DECODER_PARSE_TAG                      0x401

/* PARSER */
#define PROP_PARSER_MASK                            0x500
#define PROP_PARSER_PARSER_TYPE                     0x500

/* QUEUE */
#define PROP_QUEUE_MASK                             0x600
#define PROP_QUEUE_SET_BUFF                         0x600
#define PROP_QUEUE_SET_READ_THRED                   0x601
#define PROP_QUEUE_SET_WRITE_THRED                  0x602

/* AUDIO SINK */
#define PROP_AUDIOSINK_MASK                         0x700
#define PROP_AUDIOSINK_DEVICE_DRIVER_TYPE           0x700
#define PROP_AUDIOSINK_ADD_POST_PROCESS             0x701
#define PROP_AUDIOSINK_CANCEL_POST_PROCESS          0x702
#define PROP_AUDIOSINK_TIME_UPDATE_MS               0x703
#define PROP_AUDIOSINK_DEVICE_DRIVER_STRING_NAME    0x704
#define PROP_AUDIOSINK_BUFFER_USE_CHUNK_SIZE        0x705
#define PROP_AUDIOSINK_SET_VOLUME                   0x706

/* FILE SINK */
#define PROP_FILESINK_MASK                          0x800
#define PROP_FILESINK_LOCATION                      0x800
#define PROP_FILESINK_RAW_WRITE_MODE                0x801

/* MEMORY SINK */
#define PROP_MEMSINK_MASK                           0x900
#define PROP_MEMSINK_BUFFER_DESC                    0x900
#define PROP_MEMSINK_RAW_WRITE_MODE                 0x901

/* NETBUF SOURCE */
#define PROP_NETBUFSRC_MASK                         0xa00
#define PROP_NETBUFSRC_SET_BUFF                     0xa00
#define PROP_NETBUFSRC_SET_CHUNK_SIZE               0xa01
#define PROP_NETBUFSRC_GET_CHUNK_SIZE               0xa02
#define PROP_NETBUFSRC_SET_CALLBACK                 0xa03

/* VIT SINK */
#define PROP_VITSINK_MASK                           0xb00
#define PROP_VITSINK_FPOINT                         0xb00
#define PROP_VITSINK_BUFFER_DESC                    0xb01
#define PROP_VITSINK_TODO                           0xb02

/* EAP */
#define PROP_EAP_MASK                               0xc00
#define PROP_EAP_FPOINT                             0xc00

/* ENCODER */
#define PROP_ENCODER_MASK                           0xd00
#define PROP_ENCODER_CHUNK_SIZE                     0xd00
#define PROP_ENCODER_TYPE                           0xd01
#define PROP_ENCODER_CONFIG                         0xd02
#define PROP_ENCODER_BITSTREAMINFO                  0xd03

typedef struct {
    int8_t    *location;
    uint32_t   size;
} MEMSRC_SET_BUFFER_T;

typedef struct {
    char *device_name;
    const char *output_device_name;
} AUDSRC_SET_NAME_T;

typedef struct {
    int8_t *qbuffer;
    uint32_t buffer_size;
    uint32_t sector_size;
} QUEUE_SET_BUFFER_T;

#endif

