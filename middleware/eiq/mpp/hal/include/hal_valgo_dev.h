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

/*
 * @brief vision algorithms device declaration. Vision algorithm devices include ML/AI models like the
 * TFLite inference engine.
 */

#ifndef _HAL_VALGO_DEV_H_
#define _HAL_VALGO_DEV_H_

#include "hal_os.h"
#include "mpp_api_types.h"
#include "hal_types.h"

/** @brief Structure passed to HAL as description of the binary model provided by user */
typedef struct _model_param_t {
    const void *model_data;                  /*!< pointer to model binary */
    int model_size;                          /*!< model binary size */
    float model_input_mean;                  /*!< model 'mean' of input values, used for normalization */
    float model_input_std;                   /*!< model 'standard deviation' of input values, used for normalization */
    mpp_inference_params_t inference_params; /*!< inference parameters */
    int height;                              /*!< frame height */
    int width;                               /*!< frame width  */
    mpp_pixel_format_t format;               /*!< pixel format */
    mpp_tensor_type_t inputType;             /*!< input type */
    mpp_tensor_order_t tensor_order;         /*!< tensor order */
    int (*evt_callback_f)(mpp_t mpp, mpp_evt_t evt, void *evt_data, void *user_data); /*!< the callback to be called when model output is ready */
    void *cb_userdata;                       /*!< pointer to user data, should be passed by callback */
} model_param_t;

/** @brief Valgo Error codes for hal operations */
typedef enum _hal_valgo_status
{
    kStatus_HAL_ValgoSuccess = 0,   /*!< Successfully */
    kStatus_HAL_ValgoMallocError,   /*!< memory allocation failed for HAL algorithm */
    kStatus_HAL_ValgoInitError,     /*!< algorithm initialization error */
    kStatus_HAL_ValgoError,         /*!< Error occurs in HAL algorithm */
    kStatus_HAL_ValgoStop,          /*!< HAL algorithm stop */
} hal_valgo_status_t;

/** @brief Valgo devices private capability */
typedef struct _valgo_dev_private_capability
{
    void *param;                    /*!< param for the callback */
} valgo_dev_private_capability_t;

/** @brief Characteristics that need to be defined by a vision algo. */
typedef struct _vision_frame
{
    int height;  /*!< frame height */
    int width;   /*!< frame width */
    int pitch;   /*!< frame pitch */
    mpp_pixel_format_t format; /*!< pixel format */
    void *input_buf;           /*!< pixel input buffer */
} vision_frame_t;

typedef void *vision_algo_private_data_t;

typedef struct _vision_algo_dev vision_algo_dev_t;

/** @} */

/*! \addtogroup HAL_OPERATIONS
*  @{
*/

/** @brief Operation that needs to be implemented by a vision algorithm device */
typedef struct
{
    hal_valgo_status_t (*init)(vision_algo_dev_t *dev, model_param_t *param); /*!< initialize the dev */
    hal_valgo_status_t (*deinit)(vision_algo_dev_t *dev);                /*!< deinitialize the dev */
    hal_valgo_status_t (*run)(const vision_algo_dev_t *dev, void *data); /*!< start the dev */
    hal_valgo_status_t (*get_buf_desc)(const vision_algo_dev_t *dev, hw_buf_desc_t *in_buf, mpp_memory_policy_t *policy); /*!< read input parameters */

} vision_algo_dev_operator_t;

/** @} */

/*! \addtogroup HAL_TYPES
 *  @{
 */

/** @brief Attributes of a vision algo device */
struct _vision_algo_dev
{
    int id; /*!< unique id which is assigned by algorithm manager during the registration */
    char name[HAL_DEVICE_NAME_MAX_LENGTH]; /*!< name to identify */
    valgo_dev_private_capability_t cap;    /*!< private capability */
    const vision_algo_dev_operator_t *ops; /*!< operations */
    vision_algo_private_data_t priv_data;  /*!< private data */
};

/** @} */

#endif /*_HAL_VALGO_DEV_H_*/
