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

#ifndef _HAL_STATIC_IMAGE_H_
#define _HAL_STATIC_IMAGE_H_

/*! \addtogroup HAL_TYPES
 *  @{
 */

#include "mpp_api_types.h"
#include "hal_types.h"

typedef struct _static_image static_image_t;

/** static image return status*/
typedef enum _hal_image_status
{
    MPP_kStatus_HAL_ImageSuccess = 0,  /*!< Successfully */
	MPP_kStatus_HAL_ImageError         /*!< Error occurs on HAL Image */

} hal_image_status_t;

/** @} */

/*! \addtogroup HAL_OPERATIONS
 *  @{
 */

/** @brief Operation that needs to be implemented by an image element */
typedef struct _static_image_operator
{
    hal_image_status_t (*init)(static_image_t *elt, mpp_img_params_t *config, void *param); /*!< initialize the elt */
    hal_image_status_t (*dequeue)(static_image_t *elt, hw_buf_desc_t *out_buf, int *stripe_num); /*!< dequeue a buffer from the elt */
} static_image_operator_t;

/** @} */

/*! \addtogroup HAL_TYPES
 *  @{
 */

/** @brief Structure that characterize the image element. */
typedef struct
{
    int height;                /*!< buffer height */
    int width;                 /*!< buffer width */
    int left;                  /*!< left position */
    int top;                   /*!< top position */
    int right;                 /*!< right position */
    int bottom;                /*!< bottom position */
    mpp_pixel_format_t format; /*!< pixel format */
    bool stripe;               /*!< stripe mode */
} static_image_static_config_t;

/** @brief Attributes of a an image element. */
struct _static_image
{
    int id;                              /*!< unique id which is assigned by image manager */
    const static_image_operator_t *ops;  /*!< operations */
    static_image_static_config_t config; /*!< static configs */
    int stripe_idx;                      /*!< the current stripe index */
    uint8_t *buffer;                     /*!< static image buffer */
};

/** @} */

#endif /*_HAL_STATIC_IMAGE_H_*/
