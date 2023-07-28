/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _STREAMER_VIT_FILESINK_H_
#define _STREAMER_VIT_FILESINK_H_
/*!
 * @file    streamer_vit_filesink.h
 * @brief   This file contains the definition of the streamer audio source
 *          functionality.
 */

/*
 * CONSTANTS, DEFINES AND MACROS
 */
#define STREAMER_AUDIO_SRC_DEFAULT_SAMPLE_RATE (48000)

/*
 * FUNCTION PROTOTYPES
 */
int streamer_build_vit_filesink_pipeline(int8_t pipeline_index,
                                         const char *in_dev_name,
                                         const char *out_dev_name,
                                         StreamPipelineType pipeline_type,
                                         STREAMER_T *task_data);

int streamer_destroy_vit_filesink_pipeline(int8_t pipeline_index, STREAMER_T *task_data);

#endif /* _STREAMER_VIT_FILESINK_H_ */
