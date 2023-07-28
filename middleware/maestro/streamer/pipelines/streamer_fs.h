/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _STREAMER_FS_H_
#define _STREAMER_FS_H_

/*!
 * @file    streamer_fs.h
 * @brief   This file contains the definition of the streamer filesystem
 *          functionality.
 */

/*
 * CONSTANTS, DEFINES AND MACROS
 */

/*
 *  ENUMS
 */

/*
 * STRUCTURES AND OTHER TYPEDEFS
 */
/*!
 * Stream builder decoder table
 * This struct is for the decoder file formats table definition
 */
typedef struct
{
    char *extension;  /*!< @brief Extension name */
    int decoder_type; /*!< @brief Relate with DecoderType */
    int parser_type;  /*!< @brief Relate with ParserType */
} StreamerFileSrcConfig;

/*
 * GLOBAL VARIABLE DECLARATIONS
 */

/*
 * FUNCTION PROTOTYPES
 */

/**
 * @brief Build the file source pipeline
 *
 * @param pipeline_index Pipeline index
 * @param out_dev_name   Out device name
 * @param pipeline_type  Pipeline type
 * @param task_data      Pointer to streamer object
 * @return int
 */
int streamer_build_fs_pipeline(int8_t pipeline_index,
                               const char *out_dev_name,
                               StreamPipelineType pipeline_type,
                               STREAMER_T *task_data);

/**
 * @brief Destroy file system pipeline
 *
 * @param pipeline_index Pipeline index
 * @param task_data      Pointer to streamer object
 * @return int
 */
int streamer_destroy_fs_pipeline(int8_t pipeline_index, STREAMER_T *task_data);

/**
 * @brief Get file type from the file name and extension
 * @details Assign corresponding decoder type according to the file extension
 *
 * @param [in] filename Input file name
 * @param [out] config  Output config structure
 * @return int
 */
int streamer_get_fs_pipeline_config(char *filename, StreamerFileSrcConfig *config);
/*
 * Externs
 */

#endif /* _STREAMER_FS_H_ */
