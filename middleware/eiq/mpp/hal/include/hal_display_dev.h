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

/*! \addtogroup HAL_TYPES
*  @{
*/

/**
 * @brief hal display device declaration. Display devices can be used to display images, GUI overlays, etc.
 * Examples of display devices include display panels like the RK024hh298 display, and external displays like UVC (video
 * over USB).
 */

#ifndef _HAL_DISPLAY_DEV_H_
#define _HAL_DISPLAY_DEV_H_

#include "mpp_api_types.h"
#include "hal_types.h"

typedef struct _display_dev display_dev_t;
typedef struct _display_dev_private_capability display_dev_private_capability_t;

/** @brief Type of events that are supported by calling the callback function */
typedef enum _display_event
{
    kDisplayEvent_RequestFrame, /*!< Display finished sending the frame asynchronously, provide another frame */
} display_event_t;

/** @brief The mpp callback function prototype */
typedef int (*mpp_callback_t)(mpp_t mpp, mpp_evt_t evt, void *evt_data, void *user_data);

/*! @brief Error codes for display hal devices */
typedef enum _hal_display_status
{
    kStatus_HAL_DisplaySuccess = 0, /*!< HAL display successful */
    kStatus_HAL_DisplayTxBusy,      /*!< Display tx is busy */
    kStatus_HAL_DisplayNonBlocking, /*!< Display will return immediately */
    kStatus_HAL_DisplayError,       /*!< Error occurs on HAL Display */
} hal_display_status_t;

/** @} */

/*! \addtogroup HAL_OPERATIONS
*  @{
*/
/** @brief Operation that needs to be implemented by a display device */
typedef struct _display_dev_operator
{
    hal_display_status_t (*init)(
        display_dev_t *dev, mpp_display_params_t *config, mpp_callback_t callback, void *user_data); /*!< initialize the dev */
    hal_display_status_t (*deinit)(const display_dev_t *dev); /*!< deinitialize the dev */
    hal_display_status_t (*start)(display_dev_t *dev);        /*!< start the dev */
    hal_display_status_t (*stop)(display_dev_t *dev);         /*!< stop the dev */
    hal_display_status_t (*blit)(const display_dev_t *dev, void *frame, int stripe); /*!< blit a buffer to the dev */
    hal_display_status_t (*get_buf_desc)(const display_dev_t *dev, hw_buf_desc_t *in_buf, mpp_memory_policy_t *policy); /*!< get buffer descriptors and policy */
} display_dev_operator_t;

/** @} */

/*! \addtogroup HAL_TYPES
*  @{
*/

/** @brief Structure that characterizes the display device. */
struct _display_dev_private_capability
{
    int height;                 /*!< buffer height */
    int width;                  /*!< buffer width */
    int pitch;                  /*!< buffer pitch */
    int left;                   /*!< left position */
    int top;                    /*!< top position */
    int right;                  /*!< right position */
    int bottom;                 /*!< bottom position */
    int stripe_height;          /*!< stripe height (0 if stripe mode is off) */
    bool stripe;                /*!< stripe mode */
    mpp_rotate_degree_t rotate; /*!< rotate degree */
    mpp_pixel_format_t format;  /*!< pixel format */
    int nbFrameBuffer;          /*!< number of input buffers */
    void **frameBuffers;        /*!< array of pointers to frame buffer */
    mpp_callback_t callback;    /*!< callback */
    void *user_data;            /*!< parameter for the callback */
};

/** @brief Attributes of a display device. */
struct _display_dev
{
    int id;                                /*!< unique id which is assigned by display manager during the registration */
    char name[HAL_DEVICE_NAME_MAX_LENGTH]; /*!< name of the device */
    const display_dev_operator_t *ops;     /*!< operations */
    display_dev_private_capability_t cap;  /*!< private capability */
};

/** @} */

#endif /*_HAL_DISPLAY_DEV_H_*/

