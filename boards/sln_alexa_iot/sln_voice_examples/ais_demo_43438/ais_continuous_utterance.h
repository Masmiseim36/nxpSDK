/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/** @file ais_continuous_utterance.h
 *  @brief This file handles the ring buffer
 *
 *  The ring buffer saves 500ms of pre wake word + wake word which are sent to the service
 *  so it can confirm that it wasn't a false-positive
 */

#ifndef AIS_CONTINUOUS_UTTERANCE_H_
#define AIS_CONTINUOUS_UTTERANCE_H_

#include "stdint.h"
#include "string.h"

#include "pdm_pcm_definitions.h"

/* define the utterance length in ms */
#define CONTINUOUS_UTTERANCE_BUFFER_MS_LENGTH 2000
#define PRE_UTTERANCE_MS_LENGTH               500

#define SHIFT_LEFT  0
#define SHIFT_RIGHT 1

#if USE_16BIT_PCM
#define RING_BUFFER_SIZE   PCM_SAMPLE_RATE_HZ / 1000 * CONTINUOUS_UTTERANCE_BUFFER_MS_LENGTH *PCM_SAMPLE_SIZE_BYTES
#define PRE_UTTERANCE_SIZE PCM_SAMPLE_RATE_HZ / 1000 * PRE_UTTERANCE_MS_LENGTH *PCM_SAMPLE_SIZE_BYTES
#elif USE_32BIT_PCM
/* TODO PCM_SAMPLE_SIZE_BYTES should be added for 32BIT_PCM */
#define RING_BUFFER_SIZE   PCM_SAMPLE_RATE_HZ / 1000 * CONTINUOUS_UTTERANCE_BUFFER_MS_LENGTH *PCM_SAMPLE_SIZE_BYTES
#define PRE_UTTERANCE_SIZE PCM_SAMPLE_RATE_HZ / 1000 * PRE_UTTERANCE_MS_LENGTH *PCM_SAMPLE_SIZE_BYTES
#else
/* shouldn't happen */
#endif

/** @brief Adds samples in the samples ring buffer
 *
 *  @param samples_buffer[in]        Pointer to the samples buffer which should be added
 *  @param samples_buffer_len[in]    Length of the samples buffer
 *  @return                          0 on success, 1 otherwise
 */
uint8_t continuous_utterance_samples_add(uint8_t *samples_buffer, uint32_t samples_buffer_len);

/** @brief Called after a wake word detection is triggered. Sets the ring_buffer
 *         in the expected format: 500ms of pre wake word + wake word
 *
 *  @param buffer[out]              Address of a pointer which will be set towards the arranged buffer
 *  @param buffer_len[out]          Size of the arranged buffer
 *  @param wake_word_started[in]    How many frames ago the wake word started. A frame
 *                                  has 10 ms samples, so for 16 bits PCM, 16000Hz,
 *                                  it occupies 16 * 10 * 2 bytes
 *  @return                         0 on success, 1 otherwise
 */
uint8_t continuous_utterance_buffer_set(uint8_t **buffer, uint32_t *buffer_len, uint16_t wake_word_started);

/** @brief Get a pointer of the ring buffer
 *  @return Pointer of the ring buffer
 */
uint8_t *get_ring_buffer(void);

/** @brief Get the ring buffer index
 *  @return Ring buffer index
 */
uint32_t get_ring_buffer_index(void);

/** @brief Sets index to 0 and memsets ring_buffer to 0
 *  @return Void
 */
void reset_ring_buffer(void);

#endif /* AIS_CONTINUOUS_UTTERANCE_H_ */
