/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _FSL_STREAMER_H_
#define _FSL_STREAMER_H_

/* streamer library includes. */
#include "streamer_api.h"
#include "streamer_element_properties.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief AIS network audio buffer size */
#define AWS_AUDIO_BUFFER_SIZE (60 * 1024)
#define AWS_AUDIO_START_THRESHOLD (5 * 1024)
#define AWS_AUDIO_BUFFER_UNDERRUN_THRESHOLD (3 * 1024)
#define AWS_AUDIO_BUFFER_OVERRUN_THRESHOLD (30 * 1024)

typedef void (*tvStreamerErrorCallback)();

/*! @brief AIS Streamer decoder algorithm values */
typedef enum
{
    AIS_DECODER_OPUS,
    AIS_DECODER_MP3
} streamer_decoder_t;

/*! @brief AIS Streamer interface structure */
typedef struct _streamer_handle_t
{
    STREAMER_T *streamer;
    bool audioPlaying;
    bool eos;
    tvStreamerErrorCallback pvExceptionCallback;
} streamer_handle_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize the AIS streamer interface
 *
 * This function initializes the AIS streamer library, and initializes the PCM
 * output interface.  This function should be called before creating a streamer
 * handle.
 */
void STREAMER_Init(void);

/*!
 * @brief Create an AIS streamer interface handle
 *
 * This function creates an AIS streamer interface and starts a task for
 * handling media playback, and a task for sending status and error messages
 * back to the application.
 *
 * @param handle Pointer to input handle
 * @param decoder Decoder algorithm to use for audio playback
 * @return kStatus_Success on success, otherwise an error.
 */
status_t STREAMER_Create(streamer_handle_t *handle, streamer_decoder_t decoder);

/*!
 * @brief Destroy an AIS streamer interface handle
 *
 * This function destroys an AIS streamer interface and frees associated memory.
 *
 * @param handle Pointer to input handle
 */
void STREAMER_Destroy(streamer_handle_t *handle);

/*!
 * @brief Start audio playback for the streamer interface
 *
 * This function puts the streamer in a playing state, and begins pulling data
 * from the internal ring buffer, filled with calls to STREAMER_Start.
 *
 * @param handle Pointer to input handle
 */
void STREAMER_Start(streamer_handle_t *handle);

/*!
 * @brief Stop audio playback for the streamer interface
 *
 * This function puts the streamer in a stopped state, and ends playback from
 * the audio buffer.  The internal audio buffer is cleared of any data.
 *
 * @param handle Pointer to input handle
 * @returns Number of bytes that were still queued and got flushed
 */
uint32_t STREAMER_Stop(streamer_handle_t *handle);

/*!
 * @brief Pause audio playback for the streamer interface
 *
 * This function puts the streamer in a paused state, and ends playback from
 * the audio buffer.  It does not empty the unplayed audio buffer.
 *
 * @param handle Pointer to input handle
 */
void STREAMER_Pause(streamer_handle_t *handle);

/*!
 * @brief Get streamer audio input buffer size of queued data
 *
 * This function returns how much audio data, in bytes, occupies the internal
 * source ringbuffer.
 *
 * @param handle Pointer to input handle
 * @returns Number of bytes queued for decode and playback
 */
uint32_t STREAMER_GetQueued(streamer_handle_t *handle);

/*!
 * @brief Get streamer audio input buffer size of queued data, without locking audio streamer mutex
 *        Useful for when this info does not need to be very accurate, like some logs printing
 *        Not locking the mutex for each speaker log will be faster
 *
 * This function returns how much audio data, in bytes, occupies the internal
 * source ringbuffer.
 *
 * @param handle Pointer to input handle
 * @returns Number of bytes queued for decode and playback
 */
uint32_t STREAMER_GetQueuedNotBlocking(streamer_handle_t *handle);

/*!
 * @brief Check if streamer interface is playing
 *
 * This function returns true/false of the playing state for the interface
 *
 * @param handle Pointer to input handle
 * @return true if playing, false if not
 */
bool STREAMER_IsPlaying(streamer_handle_t *handle);

/*!
 * @brief Set volume for streamer playback interface
 *
 * @param volume Volume with range from 0-100
 */
void STREAMER_SetVolume(uint32_t volume);

/*!
 * @brief Write audio data to the streamer interface
 *
 * This function pushes data onto the internal audio ring buffer for processing
 * by the audio streamer.
 *
 * @param data Pointer to audio data
 * @param size Size in bytes of the audio data
 * @return Number of bytes successfully written.  If this is less than the
 *         'size' parameter, an overflow has occured.
 */
int STREAMER_Write(uint8_t *data, uint32_t size);

/*!
 * @brief Read audio data from the internal audio ring buffer
 *
 * This function is called internally by the streamer (passed as a callback
 * function) to consume and process data from the ring buffer.
 *
 * @param data Pointer to buffer to copy audio data into
 * @param size Size in bytes of the buffer to fill
 * @return Number of bytes successfully read.  If this is less than the
 *         'size' parameter, an underflow has occured.
 */
int STREAMER_Read(uint8_t *data, uint32_t size);

/*!
 * @brief Locks the internal streamer recursive mutex
 * @return Void
 */
void STREAMER_MutexLock(void);

/*!
 * @brief Unlocks the internal streamer recursive mutex
 * @return Void
 */
void STREAMER_MutexUnlock(void);

#if defined(__cplusplus)
}
#endif

#endif
