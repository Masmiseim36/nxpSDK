/*
 * Copyright 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief hal audio processing device declaration.
 */

#ifndef HAL_AUDIO_PROCESSING_DEV_H_
#define HAL_AUDIO_PROCESSING_DEV_H_

#include "fwk_common.h"

typedef struct _audio_processing_dev audio_processing_dev_t;

/*! @brief Type of events that are supported by calling the callback function */
typedef enum _audio_processing_event
{
    kAudioProcessingEvent_Done,
    kAudioProcessingEvent_Dump,
} audio_processing_event_t;

/**
 * @brief Callback function to notify audio processing manager that an async event took place
 * @param dev Device structure of the audio processing device calling this function
 * @param event id of the event that took place
 * @param param Parameters
 * @param fromISR True if this operation takes place in an irq, 0 otherwise
 * @return 0 if the operation was successfully
 */
typedef int (*audio_processing_dev_callback_t)(
    const audio_processing_dev_t *dev, audio_processing_event_t event, void *param, unsigned int size, uint8_t fromISR);

/*! @brief Error codes for audio processing hal devices */
typedef enum _hal_audio_processing_status
{
    kStatus_HAL_AudioProcessingSuccess = 0, /*!< Successfully */
    kStatus_HAL_AudioProcessingError =
        MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_AudioProcessing, 1), /*!< Error occurs */
} hal_audio_processing_status_t;

/*! @brief Operation that needs to be implemented by a audio processing device */
typedef struct _audio_processing_dev_operator
{
    /* initialize the dev */
    hal_audio_processing_status_t (*init)(audio_processing_dev_t *dev, audio_processing_dev_callback_t callback);
    /* deinitialize the dev */
    hal_audio_processing_status_t (*deinit)(const audio_processing_dev_t *dev);
    /* start the dev */
    hal_audio_processing_status_t (*start)(const audio_processing_dev_t *dev);
    /* start the dev */
    hal_audio_processing_status_t (*stop)(const audio_processing_dev_t *dev);
    /* notify the audio_processing_dev_t */
    hal_audio_processing_status_t (*run)(const audio_processing_dev_t *dev, void *param);
    /* notify the audio_processing_dev_t */
    hal_audio_processing_status_t (*inputNotify)(const audio_processing_dev_t *dev, void *param);
} audio_processing_dev_operator_t;

typedef struct _audio_processing_dev_private_capability
{
    /* callback */
    audio_processing_dev_callback_t callback;
} audio_processing_dev_private_capability_t;

/*! @brief Attributes of an audio processing device. */
struct _audio_processing_dev
{
    /* unique id which is assigned by camera manager during the registration */
    int id;
    /* name of the device */
    char name[DEVICE_NAME_MAX_LENGTH];
    /* operations */
    const audio_processing_dev_operator_t *ops;
    /* private capability */
    audio_processing_dev_private_capability_t cap;
};

#endif /* HAL_AUDIO_PROCESSING_DEV_H_ */
