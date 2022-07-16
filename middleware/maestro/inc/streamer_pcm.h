/*
 * Copyright 2018-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _FSL_STREAMER_PCM_H_
#define _FSL_STREAMER_PCM_H_

/*!
 * @file    streamer_pcm.h
 * @brief   Streamer PCM API header. Public APIs for configuring the input/output PCM interfaces
 *
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Opaque type for PCM interface
 * Application should extend this for PCM interface implementation. */
typedef struct _pcm_rtos_t pcm_rtos_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize the PCM interface
 *
 * This function initializes the PCM interface for audio data output to a
 * speaker or input from a microphone on a specific target hardware platform.
 * This function should be executed directly by the application or other
 * middleware library, and should be called before any other PCM interface functions.
 * This function is responsible for initializing and configuring audio output
 * peripherals, DMA and codecs so that they are ready to utilitize the PCM
 * interface library.
 */
void streamer_pcm_init(void);

/*!
 * @brief Open the PCM interface for writing and configuring
 *
 * This function returns a handle to the PCM interface that can be used for
 * configuration and writing the data.
 *
 * This function is called from the streamer when it is placed into a playing
 * state, before audio output starts.
 *
 * @param num_buffers Number of buffers to be used
 * @return Pointer to handle for PCM interface
 */
pcm_rtos_t *streamer_pcm_open(uint32_t num_buffers);

/*!
 * @brief Open the receive PCM interface for reading and configuring
 *
 * This function returns a handle to the PCM interface that can be used for
 * configuration and reading the data.
 *
 * This function is called from the streamer when it is placed into a playing
 * state, before audio input starts.
 *
 * @param num_buffers Number of buffers to be used
 * @return Pointer to handle for PCM interface
 */
pcm_rtos_t *streamer_pcm_rx_open(uint32_t num_buffers);

/*!
 * @brief Close the PCM interface
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
 * @brief Start playback on the PCM interface
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
 * @brief Start recording with the PCM interface
 *
 * This function places the recording PCM interface into a started state
 *
 * This function is called from the streamer when it is placed into a playing
 * state at the start of audio recording.
 *
 * @param pcm Pointer to PCM interface handle
 */
void streamer_pcm_rx_start(pcm_rtos_t *pcm);

/*!
 * @brief Write audio data to the PCM interface
 *
 * This function writes raw PCM data to the PCM output interface
 *
 * This function is called from the streamer to output decoded audio data.
 *
 * @param pcm Pointer to PCM interface handle
 * @param data Pointer to data buffer with PCM data
 * @param size Size in bytes of the data buffer
 * @return 0 on success, non-zero on failure
 */
int streamer_pcm_write(pcm_rtos_t *pcm, uint8_t *data, uint32_t size);

/*!
 * @brief Read audio data from the input PCM interface
 *
 * This function reads raw PCM data from the PCM input interface
 *
 * This function is called from the streamer to input raw audio data.
 *
 * @param pcm Pointer to PCM interface handle
 * @param data Pointer to next data buffer with PCM data
 * @param size Size in bytes of the data buffer
 * @return 0 on success, non-zero on failure
 */
int streamer_pcm_read(pcm_rtos_t *pcm, uint8_t *data, uint32_t size);

/*!
 * @brief Set PCM interface parameters
 *
 * This function is called from the streamer after the start of audio data
 * decode, once the parameters of the data format are known. The PCM interface
 * should be configured to match these parameters. The pcm handle should
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
 * @param transfer Flag for setting params of input/output pcm interface
 * @param dummy_tx Dummy tx setting for clock enablement
 * @param volume Output volume
 * @return 0 on success, non-zero on failure
 */
int streamer_pcm_setparams(pcm_rtos_t *pcm,
                           uint32_t sample_rate,
                           uint32_t bit_width,
                           uint8_t num_channels,
                           bool transfer,
                           bool dummy_tx,
                           int volume);

/*!
 * @brief Get PCM interface parameters
 *
 * This function is called by the streamer after streamer_pcm_setparams, and
 * should reflect what parameters can be supported by the PCM interface. If
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
 * mute the PCM output codec. This function is optional to implement as not all
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
 * @param pcm Pointer to PCM interface handle
 * @param volume Volume on a scale from 0-100
 * @return 0 on success, non-zero on failure
 */
int streamer_pcm_set_volume(pcm_rtos_t *pcm, int volume);

#if defined(__cplusplus)
}
#endif

#endif
