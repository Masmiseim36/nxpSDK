/*
 * Copyright 2022-2024 NXP
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

/*
 * HAL functions public header
 */

/**
 * @defgroup HAL_SETUP HAL Setup Functions
 *
 * This section provides the detailed documentation for the HAL setup functions that should be
 * defined by each device.
 *
 * @{
 */

#ifndef _HAL_H
#define _HAL_H

#include <stdint.h>
#include "mpp_api_types.h"

#include "hal_graphics_dev.h"
#include "hal_valgo_dev.h"
#include "hal_debug.h"
#include "hal_display_dev.h"
#include "hal_camera_dev.h"

/*!
 * @brief Implementation of hal labeled rectangle component that draws a rectangle and a text on an input image.
 * @param[in] frame: The buffer address
 * @param[in] width: Image width
 * @param[in] height: Image height
 * @param[in] format: Image format
 * @param[in] lr: Labeled rectangle parameters
 * @param[in] stripe: stripe number (0=no stripe)
 * @param[in] stripe_max: max nb of stripes
 * @return 0
 *
 */
int hal_label_rectangle(uint8_t *frame, int width, int height, mpp_pixel_format_t format,
                        mpp_labeled_rect_t *lr, int stripe, int stripe_max);

/*!
 * @brief Hal setup function for inference engine Tensorflow-Lite Micro
 *
 * @param[in] dev vision algo device to register
 * @return error code (0: success, otherwise: failure)
 *
 */
int hal_inference_tflite_setup(vision_algo_dev_t *dev);

/*!
 * @brief Register with a display device specified by name.
 *        If name is NULL, return error.
 *
 * @param[in] name display name
 * @param[in] dev display device to register
 * @return error code (0: success, otherwise: failure)
 *
 */
int hal_display_setup(const char *name, display_dev_t *dev);

/*!
 * @brief Register with a camera device specified by name.
 *        If name is NULL, return error.
 *
 * @param[in] name camera name
 * @param[in] dev camera device to register
 * @return error code (0: success, otherwise: failure)
 *
 */
int hal_camera_setup(const char *name, camera_dev_t *dev);

/*!
 * @brief Register with a graphic processing device specified by name.
 *        If name is NULL, the first available graphic processing supported by Hw
 *        will be selected.
 *        The graphic device using CPU operations will be selected if
 *        name is not specified and if no graphic processing is available for the Hw.
 *
 * @param[in] name graphic processing device performing the image conversion
 * @param[in] dev graphic device to register
 * @return error code (0: success, otherwise: failure)
 *
 */
int hal_gfx_setup(const char *name, gfx_dev_t *dev);

/** @} */

#endif /* _HAL_H */
