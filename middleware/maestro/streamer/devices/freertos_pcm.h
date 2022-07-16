/*
 * Copyright 2018-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef FREERTOS_PCM_H
#define FREERTOS_PCM_H

/* Forward declaration for PCM RTOS device-specific structure. */
struct _pcm_rtos_t;
typedef struct _pcm_rtos_t pcm_rtos_t;

pcm_rtos_t *streamer_pcm_open(uint32_t num_buffers);
void streamer_pcm_close(pcm_rtos_t *pcm);
void streamer_pcm_start(pcm_rtos_t *pcm);
int streamer_pcm_write(pcm_rtos_t *pcm, char *data, uint32_t size);
int streamer_pcm_read(char *data, uint32_t size);
int streamer_pcm_setparams(pcm_rtos_t *pcm,
                           uint32_t sample_rate,
                           uint32_t bit_width,
                           uint8_t num_channels,
                           bool transfer,
                           bool dummy_tx,
                           int volume);
void streamer_pcm_getparams(pcm_rtos_t *pcm, uint32_t *sample_rate, uint32_t *bit_width, uint8_t *num_channels);
int streamer_pcm_mute(pcm_rtos_t *pcm, bool mute);
int streamer_pcm_set_volume(pcm_rtos_t *pcm, int volume);

#endif
