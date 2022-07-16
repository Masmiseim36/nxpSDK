/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _STREAMER_OPUSMEM2MEM_H_
#define _STREAMER_OPUSMEM2MEM_H_

/*!
 * @file    streamer_opusmem2mem.h
 * @brief   This file contains headers for the temporary
 *          memory-to-memory pipeline with Opus encoder.
 */

/*
 * FUNCTION PROTOTYPES
 */

/**
 * @brief Build the opus memory to memory pipeline
 *
 * @param pipeline_index Pipeline index
 * @param pipeline_type  Pipeline type
 * @param task_data      Pointer to streamer object
 * @return int
 */
int streamer_build_opusmem2mem_pipeline(int8_t pipeline_index, StreamPipelineType pipeline_type, STREAMER_T *task_data);

/**
 * @brief Destroy the opus memory to memory pipeline
 *
 * @param pipeline_index Pipeline index
 * @param task_data      Pointer to streamer task data
 * @return int
 */
int streamer_destroy_opusmem2mem_pipeline(int8_t pipeline_index, STREAMER_T *task_data);

#endif /* _STREAMER_OPUSMEM2MEM_H_ */
