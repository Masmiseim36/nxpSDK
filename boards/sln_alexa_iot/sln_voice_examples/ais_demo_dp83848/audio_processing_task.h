/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _AUDIO_PROCESSING_TASK_H_
#define _AUDIO_PROCESSING_TASK_H_

#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"

/*!
 * @addtogroup
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief The states of the audio processing task
 */
typedef enum __audio_processing_states
{
    kIdle                 = (1U << 0U), /*!< Audio processing in idle state. */
    kWakeWordDetected     = (1U << 1U), /*!< Audio processing has detected a wake word. */
    kMicRecording         = (1U << 2U), /*!< Audio processing is recording microphone capture. */
    kMicStopRecording     = (1U << 3U), /*!< Audio processing has been instructed to stop recording. */
    kMicKeepOpen          = (1U << 4U), /*!< Audio processing should start recording without wake word detection. */
    kMicStop              = (1U << 5U), /*!< Audio processing has stopped recording microphone stream. */
    kMicCloudWakeVerifier = (1U << 6U), /*!< Audio processing is streaming cloud wake word verification data. */
    kReconnect            = (1U << 7U) /*!< Audio processing ends current state and returns to idle for reconnection. */
} audio_processing_states_t;

/*!
 * @brief The mute states of the microphone
 */
typedef enum __mic_mute_mode
{
    kMicMuteModeOff = 0, /*!< Microphone mute mode disbabled, device is listening */
    kMicMuteModeOn,      /*!< Microphone mute mode enable, device is not listening */
} mic_mute_mode_t;

#define AUDIO_PROCESSING_NOT_LISTENING_MASK (kIdle | kReconnect | kMicStopRecording | kMicStop)
#define AUDIO_PROCESSING_IS_LISTENING_MASK  (kWakeWordDetected | kMicRecording | kMicKeepOpen | kMicCloudWakeVerifier)

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief The main code for the audio processing task
 * @param pvParameters  Pointer to the task parameters, in accordance with TaskFunction_t
 */
void audio_processing_task(void *pvParameters);

/*!
 * @brief Sets the microphone input buffer
 * @param buff  Pointer to the buffer
 */
void audio_processing_set_mic_input_buffer(int16_t **buf);

/*!
 * @brief Sets the amplifier input buffer
 * @param buff Pointer to the buffer (int16_t **)
 */
void audio_processing_set_amp_input_buffer(int16_t **buf);

/*!
 * @brief Gets a chunk of the wake word verifying audio buffer
 * @param outBuf Reference created for tracking wake word verifier buffer (uint8_t **)
 * @param outLen Reference to length of wake word verifier buffer to get (uint32_t *)
 * @param index Refence to track index of wake word verifier buffer (uint32_t *)
 * @param size Reference to size of wake word verifier buffer being copied (uint32_t *)
 * @param data Reference to buffer to be published to cloud (uint8_t **)
 * @returns Status of this operation
 */
uint32_t audio_processing_get_continuous_utterance(
    uint8_t **outBuf, uint32_t *outLen, uint32_t *index, uint32_t *size, uint8_t **data);

/*!
 * @brief Retrieves microphone output buffer from queue
 * @param outBuf Reference to buffer to store output buffer from queue (uint8_t **)
 * @param outLen Reference to length of buffer retrieved, should match AUDIO_QUEUE_WTRMRK_BYTES (uint8_t *)
 * @returns Status of buffer retrieval
 */
uint32_t audio_processing_get_output_buffer(uint8_t **outBuf, uint32_t *outLen);

/*!
 * @brief Provides the audio processing functiosn the externally created task handle
 * @param handle Reference to externally created FreeRTOS task (TaskHandle_t *)
 */
void audio_processing_set_task_handle(TaskHandle_t *handle);

/*!
 * @brief Method for passing audio processing task handle to other tasks/modules
 * @returns Current audio processing task handle (TaskHandle_t)
 */
TaskHandle_t audio_processing_get_task_handle(void);

/*!
 * @brief Pass the main app task handle to the audio processing task
 * @param *handle Reference to the main processing task handle
 */
void audio_processing_set_app_task_handle(TaskHandle_t *handle);

/*!
 * @brief Explicitly set the audio processing state
 * @param state Desired state type to set audio processing task (audio_processing_states_t)
 */
void audio_processing_set_state(audio_processing_states_t state);

/*!
 * @brief Get current state of the audio processing task
 * @returns Current state of the audio processing tast (audio_processing_states_t)
 */
audio_processing_states_t audio_processing_get_state(void);

/*!
 * @brief
 */
uint32_t audio_processing_get_wake_word_end(void);

/*!
 * @brief Gets the mute mode
 * @returns Current mute mode of audio processing task (mic_mute_mode_t)
 */
mic_mute_mode_t audio_processing_get_mic_mute(void);

/*!
 * @brief Sets the microphone mute mode in audio processing task
 * @param mute_mode  can be kMicMuteModeOff or kMicMuteModeOn
 * @param persistent if true, mute mode will be saved in flash
 * @returns 0 on success, error code otherwise)
 */
uint32_t audio_processing_set_mic_mute(mic_mute_mode_t mute_mode, bool persistent);

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* _AUDIO_PROCESSING_TASK_H_ */

/*******************************************************************************
 * API
 ******************************************************************************/
