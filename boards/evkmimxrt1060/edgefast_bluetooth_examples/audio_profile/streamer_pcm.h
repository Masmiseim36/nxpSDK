/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _FSL_STREAMER_PCM_H_
#define _FSL_STREAMER_PCM_H_

#include "fsl_dmamux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief PCM interface structure */
typedef struct _pcm_rtos_t
{
#if 0 /* don't need codec in this demo */
    sai_transfer_t saiTx;
    sai_edma_handle_t saiTxHandle;
    edma_handle_t dmaTxHandle;

    sai_transfer_t saiRx;
    sai_edma_handle_t saiRxHandle;
    edma_handle_t dmaRxHandle;
#endif

    uint32_t sample_rate;
    uint32_t bit_width;
    uint8_t num_channels;

    volatile uint32_t emptyBlock;
    volatile uint8_t streamerStop;
    volatile uint8_t streamerGettingData;
} pcm_rtos_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize the transfer PCM interface
 *
 * This function initializes the PCM interface for audio data output to a
 * speaker on a specific target hardware platform.  This function should be
 * executed directly by the application or other middleware library, and should
 * be called before any other PCM interface functions.  This function is
 * responsible for initializing and configuring audio output peripherals, DMA,
 * and codecs so that they are ready the PCM interface library to utilitize.
 */
void streamer_pcm_init(void);

/*!
 * @brief Open the transfer PCM interface for writing and configuring
 *
 * This function returns a handle to the PCM interface that can be used for
 * configuration and writing data.
 *
 * This function is called from the streamer when it is placed into a playing
 * state, before output audio.
 *
 * @param num_buffers Number of SAI EDMA buffers to use
 * @return Pointer to handle for PCM interface
 */
pcm_rtos_t *streamer_pcm_open(uint32_t num_buffers);

/*!
 * @brief Open the receive PCM interface for writing and configuring
 *
 * This function returns a handle to the PCM interface that can be used for
 * configuration and writing data.
 *
 * This function is called from the streamer when it is placed into a playing
 * state, before output audio.
 *
 * @param num_buffers Number of SAI EDMA buffers to use
 * @return Pointer to handle for PCM interface
 */
pcm_rtos_t *streamer_pcm_rx_open(uint32_t num_buffers);

/*!
 * @brief Close the transfer PCM interface
 *
 * This function closes the handle to the PCM interface
 *
 * This function is called from the streamer when it is placed into a stopped
 * state.
 *
 * @param pcm Pointer to PCM interface handle
 */
void streamer_pcm_close(pcm_rtos_t *pcm);

/*!
 * @brief Close the receive PCM interface
 *
 * This function closes the handle to the PCM interface
 *
 * This function is called from the streamer when it is placed into a stopped
 * state.
 *
 * @param pcm Pointer to PCM interface handle
 */
void streamer_pcm_rx_close(pcm_rtos_t *pcm);

/*!
 * @brief Start playback on the transfer PCM interface
 *
 * This function places the PCM interface into a started state
 *
 * This function is called from the streamer when it is placed into a playing
 * state at the start of audio playback.
 *
 * @param pcm Pointer to PCM interface handle
 */
void streamer_pcm_start(pcm_rtos_t *pcm);

/*!
 * @brief Start playback on the receive PCM interface
 *
 * This function places the PCM interface into a started state
 *
 * This function is called from the streamer when it is placed into a playing
 * state at the start of audio playback.
 *
 * @param pcm Pointer to PCM interface handle
 */
void streamer_pcm_rx_start(pcm_rtos_t *pcm);

/*!
 * @brief Write audio data to the transef PCM interface
 *
 * This function writes raw PCM data to the PCM output interface
 *
 * This function is called from the streamer to output decoded audio data.
 *
 * @param pcm Pointer to PCM interface handle
 * @param data Pointer to data buffer with PCM data
 * @param size Size in bytes of the data buffer
 * @return 0 on succes, non-zero on failure
 */
int streamer_pcm_write(pcm_rtos_t *pcm, uint8_t *data, uint32_t size);

/*!
 * @brief Read audio data from the receive PCM interface
 *
 * This function writes raw PCM data to the PCM output interface
 *
 * This function is called from the streamer to output decoded audio data.
 *
 * @param pcm Pointer to PCM interface handle
 * @param data Pointer to data buffer with PCM data
 * @param size Size in bytes of the data buffer
 * @return 0 on succes, non-zero on failure
 */
int streamer_pcm_read(pcm_rtos_t *pcm, uint8_t *data, uint8_t *next_buffer, uint32_t size);

/*!
 * @brief Set transfer PCM interface parameters
 *
 * This function is called from the streamer after the start of audio data
 * decode, once the parameters of the data format are known.  The PCM interface
 * should be configured to match these parameters.  The pcm handle should be
 * reflect the values configured.
 *
 * If the PCM interface cannot support these parameters, the pcm handle should
 * be set to the nearest values the interface can support and success should be
 * returned.  These will be queried by the streamer with streamer_pcm_getparams,
 * and the streamer will resample or reconfigure the audio format to fit the
 * desired output.
 *
 * @param pcm Pointer to PCM interface handle
 * @param sample_rate Sample rate in Hz of the PCM data
 * @param bit_width Size of each PCM data sample, in bits
 * @param num_channels Number of channels of audio data for each PCM frame
 * @param transfer setting for transfer or receive
 * @return 0 on succes, non-zero on failure
 */
int streamer_pcm_setparams(
    pcm_rtos_t *pcm, uint32_t sample_rate, uint32_t bit_width, uint8_t num_channels, bool transfer);

/*!
 * @brief Get PCM interface parameters
 *
 * This function is called by the streamer after streamer_pcm_setparams, and
 * should reflect what parameters can be supported by the PCM interface.  If
 * these do not match what was requested in streamer_pcm_setparams, the streamer
 * will attempt to configure SRC or other behavior to adjust the data format.
 * If the streamer cannot support this, it will return a failure status to the
 * application via the message task.
 *
 * @param pcm Pointer to PCM interface handle
 * @param sample_rate Pointer to pcm handle sample rate
 * @param bit_width Pointer to pcm handle bit width
 * @param num_channels Pointer to pcm handle number of channels
 */
void streamer_pcm_getparams(pcm_rtos_t *pcm, uint32_t *sample_rate, uint32_t *bit_width, uint8_t *num_channels);

/*!
 * @brief Set PCM interface mute status
 *
 * This function is called by the streamer while entering the stopped state to
 * mute the PCM output codec.  This function is optional to implement as not all
 * codecs will support this functionality.
 *
 * @param pcm Pointer to PCM interface handle
 * @param mute Boolean indicated true/false requested codec mute state
 * @return 0 on success, non-zero on failure
 */
int streamer_pcm_mute(pcm_rtos_t *pcm, bool mute);

/*!
 * @brief Set PCM interface output volume
 *
 * This function is called by the application interface to adjust the output
 * volume for the PCM interface.
 *
 * @param volume Volume on a scale from 0-100
 * @return 0 on success, non-zero on failure
 */
int streamer_pcm_set_volume(uint32_t volume);

void streamer_pcm_get_buffer(uint8_t evenOdd, uint8_t **buffer, uint32_t *length);

void streamer_pcm_get_mp3_file_info(uint32_t *sample_rate, uint32_t *bit_width, uint8_t *num_channels);

void streamer_pcm_stop(void);

#if defined(__cplusplus)
}
#endif

#endif
