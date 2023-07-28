/*
 * Copyright 2023 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef __STREAMER_PCM_AUDIO_PROC_SPEAKER_MEM_H__
#define __STREAMER_PCM_AUDIO_PROC_SPEAKER_MEM_H__

int streamer_build_pcm_speaker_mem_pipeline(int8_t pipeline_index,
                                            StreamPipelineType pipeline_type,
                                            STREAMER_T *task_data);

int streamer_destroy_pcm_speaker_mem_pipeline(int8_t pipeline_index, STREAMER_T *task_data);

#endif
