/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.d
 */

#ifndef _AUDIO_PROCESSING_TASK_H_
#define _AUDIO_PROCESSING_TASK_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

/*!
 * @addtogroup
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef enum __audio_processing_states
{
    kWaiting          = (1U << 0U),
    kWakeWordDetected = (1U << 1U),
    kMicRecording     = (1U << 2U),
    kMicStopRecording = (1U << 3U),
    kMicQueueReady    = (1U << 4U),
    kMicQueueFull     = (1U << 5U),
    kMicKeepOpen      = (1U << 6U),
    kMicStop          = (1U << 8U)
} audio_processing_states_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief
 */
void audio_processing_task(void *pvParameters);

/*!
 * @brief
 */
void audio_processing_set_mic_input_buffer(int16_t **buf);

/*!
 * @brief
 */
void audio_processing_set_amp_input_buffer(int16_t **buf);

/*!
 * @brief
 */
uint32_t audio_processing_get_output_buffer(uint8_t **outBuf, uint32_t *outLen);

/*!
 * @brief
 */
void audio_processing_set_task_handle(TaskHandle_t *handle);

/*!
 * @brief
 */
TaskHandle_t audio_processing_get_task_handle(void);

/*!
 * @brief
 */
void audio_processing_set_app_task_handle(TaskHandle_t *handle);

/*!
 * @brief
 */
void audio_processing_set_state(audio_processing_states_t state);

/*!
 * @brief
 */
audio_processing_states_t audio_processing_get_state(void);

/*!
 * @brief
 */
void audio_processing_start_capture(void);

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* _AUDIO_PROCESSING_TASK_H_ */

/*******************************************************************************
 * API
 ******************************************************************************/
