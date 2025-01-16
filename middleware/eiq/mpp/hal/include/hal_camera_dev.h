/*
 * Copyright 2020-2024 NXP.
 *
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * @defgroup HAL_TYPES HAL Types
 *
 * This section provides the detailed documentation for the MPP HAL types
 *
 * @{
 */

/**
 * @brief hal camera device declaration. Camera devices can enqueue and dequeue frames as well as react to events from
 * input devices via the "inputNotify" function. Camera devices can use any number of interfaces, including MIPI and CSI
 * as long as the HAL driver implements the necessary functions found in camera_dev_operator_t.
 * Examples of camera devices include the Orbbec U1S 3D SLM camera module, and the OnSemi MT9M114 camera module.
 */

#ifndef _HAL_CAMERA_DEV_H_
#define _HAL_CAMERA_DEV_H_

#include "mpp_api_types.h"
#include "hal_types.h"

typedef struct _camera_dev camera_dev_t;

/** Camera return status*/
typedef enum _hal_camera_status
{
    kStatus_HAL_CameraSuccess = 0,  /*!< HAL camera successful */
    kStatus_HAL_CameraBusy,         /*!< Camera is busy */
    kStatus_HAL_CameraNonBlocking,  /*!< Camera will return immediately */
    kStatus_HAL_CameraError         /*!< Error occurs on HAL Camera */
} hal_camera_status_t;

/** @brief Type of events that are supported by calling the callback function */
typedef enum _camera_event
{
    kCameraEvent_SendFrame,        /*!< Camera new frame is available */
    kCameraEvent_CameraDeviceInit, /*!< Camera device finished the initialization process */
} camera_event_t;

/**
 * @brief Callback function to notify camera manager that one frame is dequeued
 * @param dev Device structure of the camera device calling this function
 * @param event id of the event that took place
 * @param param Parameters
 * @param fromISR True if this operation takes place in an irq, 0 otherwise
 * @return 0 if the operation was successfully
 */
typedef int (*camera_dev_callback_t)(const camera_dev_t *dev, camera_event_t event, void *param, uint8_t fromISR);

/** @} */

/**
 * @defgroup HAL_OPERATIONS HAL OPERATIONS
 *
 * This section provides the detailed documentation for the MPP HAL operations that needs to be implemented
 * for each component.
 *
 * @{
 */

/** @brief Operation that needs to be implemented by a camera device */
typedef struct _camera_dev_operator
{
    hal_camera_status_t (*init)(camera_dev_t *dev, mpp_camera_params_t *config, camera_dev_callback_t callback, void *param); /*!< initialize the dev */
    hal_camera_status_t (*deinit)(camera_dev_t *dev);      /*!< deinitialize the dev */
    hal_camera_status_t (*start)(const camera_dev_t *dev); /*!< start the dev */
    hal_camera_status_t (*stop)(const camera_dev_t *dev);  /*!< stop the dev */
    hal_camera_status_t (*enqueue)(const camera_dev_t *dev, void *data); /*!< enqueue a buffer to the dev */
    hal_camera_status_t (*dequeue)(const camera_dev_t *dev, void **data, int *stripe); /*!< dequeue a buffer from the dev (blocking) */
    hal_camera_status_t (*get_buf_desc)(const camera_dev_t *dev, hw_buf_desc_t *out_buf, mpp_memory_policy_t *policy); /*!< get buffer descriptors and policy */
} camera_dev_operator_t;

/** @} */

/*! \addtogroup HAL_TYPES
 *  @{
 */

/** @brief Structure that characterizes the camera device. */
typedef struct
{
    int height;                  /*!< buffer height */
    int width;                   /*!< buffer width */
    int pitch;                   /*!< buffer pitch */
    int left;                    /*!< left position */
    int top;                     /*!< top position */
    int right;                   /*!< right position */
    int bottom;                  /*!< bottom position */
    mpp_rotate_degree_t rotate;  /*!< rotate degree */
    mpp_flip_mode_t flip;        /*!< flip */
    int swapByte;                /*!< swap byte per two bytes */
    mpp_pixel_format_t format;   /*!< pixel format */
    int framerate;               /*!< frame rate */
    int stripe_size;             /*!< stripe size in bytes */
    bool stripe;                 /*!< stripe mode */
} camera_dev_static_config_t;

/** @brief camera device private capability. */
typedef struct
{
    camera_dev_callback_t callback;  /*!< callback */
    void *param;                     /*!< parameter for the callback */
} camera_dev_private_capability_t;

/** @brief Attributes of a camera device. */
struct _camera_dev
{
    int id; /*!< unique id which is assigned by camera manager during registration */
    char name[HAL_DEVICE_NAME_MAX_LENGTH]; /*!< name of the device */
    const camera_dev_operator_t *ops;      /*!< operations */
    camera_dev_static_config_t config;     /*!< static configurations */
    camera_dev_private_capability_t cap;   /*!< private capability */
};

/** @} */

#endif /*_HAL_CAMERA_DEV_H_*/
