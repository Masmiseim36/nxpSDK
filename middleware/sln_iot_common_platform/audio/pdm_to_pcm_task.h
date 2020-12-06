/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _PDM_TO_PCM_TASK_H_
#define _PDM_TO_PCM_TASK_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "event_groups.h"
#include "task.h"
#include "fsl_common.h"
/*!
 * @addtogroup pdm_to_pcm_task
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef enum __pdm_pcm_input_event
{
    MIC1_PING_EVENT      = (1 << 0U),
    MIC2_PING_EVENT      = (1 << 1U),
    MIC1_PONG_EVENT      = (1 << 2U),
    MIC2_PONG_EVENT      = (1 << 3U),
    MIC3_PONG_EVENT      = (1 << 4U),
    MIC3_PING_EVENT      = (1 << 5U),
    AMP_REFERENCE_SIGNAL = (1 << 6U),
    PDM_ERROR_FLAG       = (1 << 7U),
} pdm_pcm_input_event_t;

typedef struct __pdm_pcm_task_config
{
    TaskHandle_t *thisTask;
    TaskHandle_t *processingTask;
    int32_t (*feedbackInit)(void);
    int16_t *feedbackBuffer;
    void (*feedbackEnable)(void);
    void (*feedbackDisable)(void);
} pcm_pcm_task_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Task to handle PDM to PCM conversion; and AFE barge-in capture
 *
 * @param pvParameters Optional parameters to pass into task
 */
void pdm_to_pcm_task(void *pvParameters);

/*!
 * @brief Sets pdm to pcm task configuration
 *
 * @param *config Reference to application specific configuration
 */
status_t pcm_to_pcm_set_config(pcm_pcm_task_config_t *config);

/*!
 * @brief Gets pdm to pcm task configuration
 *
 * @return Reference to application specific configuration
 */
pcm_pcm_task_config_t *pcm_to_pcm_get_config(void);

/*!
 * @brief Sets task handle for PDM to PCM task
 *
 * @param *handle Reference to task handle
 */
void pdm_to_pcm_set_task_handle(TaskHandle_t *handle);

/*!
 * @brief Set task handle for passing notifications to audio processing task
 *
 * @param *handle Reference to task handle
 */
void pdm_to_pcm_set_audio_proces_task_handle(TaskHandle_t *handle);

/*!
 * @brief Get task handle for PDM to PCM task
 *
 * @returns Task handle stored for PDM to PCM task
 */
TaskHandle_t pdm_to_pcm_get_task_handle(void);

/*!
 * @brief Get event group for PDM to PCM task
 *
 * @returns Event group handle for PDM to PCM task
 */
EventGroupHandle_t pdm_to_pcm_get_event_group(void);

/*!
 * @brief Set buffer for amp loopback
 *
 * @param **buf Reference to buffer for amp loopback
 */
void pdm_to_pcm_set_amp_loopback_buffer(uint8_t **buf);

/*!
 * @brief Get the amp loopback event bit
 *
 * @returns Event bit for amp loopback event
 */
EventBits_t pdm_to_pcm_get_amp_loopback_event(void);

/*!
 * @brief Get output buffer from amp loopback; to pass onto consuming task
 *
 * @returns 16-bit pointer to amp loopback buffer
 */
int16_t *pdm_to_pcm_get_amp_output(void);

/*!
 * @brief Get pointer to PCM output for consuming task
 *
 * @returns 16-bit pointer to microphone PCM data
 */
int16_t *pdm_to_pcm_get_pcm_output(void);

/*!
 * @brief Formats PCM data based on type of triggered event and desired pattern of channels
 *
 * @param *pcmBuffer A single channel PCM buffer to merge into output stream
 * @param micEvent Ping or pong event for given microphone
 * @param pcmFormat Channel format type 1 for LRLR, 0 for LLRR
 */
void pdm_to_pcm_stream_formatter(int16_t *pcmBuffer, pdm_pcm_input_event_t micEvent, uint8_t pcmFormat);

/*!
 * @brief Sets the microphone gain for all mic streams
 *
 * @param u8Gain The gain shift in all microphones
 */
int32_t pdm_to_pcm_set_gain(uint8_t u8Gain);

/*!
 * @brief Turns microphones off by disabling DMA / SAI interfaces settings
 */
void pdm_to_pcm_mics_off(void);

/*!
 * @brief Turns microphones on by enabling DMA / SAI interfaces settings
 */
void pdm_to_pcm_mics_on(void);

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* _PDM_TO_PCM_TASK_H_ */

/*******************************************************************************
 * API
 ******************************************************************************/
