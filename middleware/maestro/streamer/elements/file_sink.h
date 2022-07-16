/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef FILESINK_H
#define FILESINK_H

/*!
 * @file    file_sink.h
 * @brief  This file includes defines, enums and typedefs for file sink.
 */

#include "streamer_element.h"
#ifdef FILE_ENABLE_FATFS
#include "ff.h"
#endif // FILE_ENABLE_FATFS

/**
 * @brief Default file sink location (file name)
 *
 */
#define DEFAULT_FILESINK_LOCATION "rec.pcm"

/*!
 * File Sink Element data structure
 */
struct _ElementFileSink
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

    int32_t fd;                              /*!< @brief File descriptor */
    char location[MAX_LOCATION_PATH_LENGTH]; /*!< @brief File location */
    uint32_t chunk_size;                     /*!< @brief Chunk size */
    uint8_t end_of_stream;                   /*!< @brief End of stream flag */
    uint8_t raw_write;                       /*!< @brief Raw write flag */
    void *pullbuf;                           /*!< @brief Pull buffer pointer */
};
typedef struct _ElementFileSink ElementFileSink;

/*!
 * File dump structure
 */
typedef struct _FileDump
{
    ElementFileSink *file_sink_element;     /*!< @brief File sink element pointer */
    int8_t *data_ptr;                       /*!< @brief Data pointer */
    uint32_t data_ptr_size;                 /*!< @brief Data buffer size */
    uint32_t size;                          /*!< @brief File dump size */
    FlowReturn ret;                         /*!< @brief Return value */
    volatile bool run;                      /*!< @brief File dump task run flag */
    volatile bool first_run;                /*!< @brief File dump task first run flag */
    OSA_SEMAPHORE_HANDLE_DEFINE(sem_Read);  /*!< @brief Read semaphore */
    OSA_SEMAPHORE_HANDLE_DEFINE(sem_Write); /*!< @brief Write semaphore */
    OSA_SEMAPHORE_HANDLE_DEFINE(sem_End);   /*!< @brief End semaphore */
    OSA_MUTEX_HANDLE_DEFINE(fileDataMutex); /*!< @brief File data mutex */
} FileDump;

/*!
 *
 * @param param NULL
 *
 * @brief this function is a task that writes data to the SD card
 *
 */
void FILE_Dump_Task(void *param);

/*!
 *
 * @param element an element which is created by parent element
 *
 * @brief this function initializes the audio sink element and sink pads
 *
 * @returns   StreamReturnType
 * @retval    STREAM_OK if successful
 * @retval    STREAM_ERR_INVALID_ARGS means invalid arguments
 *
 */
int32_t filesink_init_element(StreamElement *element);

/*!
 *
 * @param  element element handle
 * @param  path    pointer to a file path
 *
 * @brief    This function sets a destiniaiton file to be writen by file sink
 *
 * @returns  StreamReturnType
 *
 * @retval STREAM_OK if successful
 * @retval STREAM_ERR_INVALID_ARGS means invalid arguments
 *
 */
int32_t filesink_set_location(ElementHandle element, char *path);

/*!
 * @param element element handle
 * @param write_mode  write mode raw (or) packet header
 *
 * @brief Set the raw write mode or the packet header mode to write to file
 *
 * @returns: StreamReturnType
 * @retval STREAM_OK if successful
 * @retval STREAM_ERR_INVALID_ARGS means invalid arguments
 */
int32_t filesink_set_write_mode(ElementHandle element, uint8_t write_mode);

#endif
