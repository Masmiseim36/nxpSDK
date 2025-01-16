/*
 * Copyright 2020, 2022-2024 NXP
 * All rights reserved.
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

#ifndef _MODEL_H_
#define _MODEL_H_

#include <stdint.h>

#include "fsl_common.h"
#include "hal_valgo_dev.h"
#include "hal.h"
#include "mpp_config.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#ifndef HAL_TFLITE_BUFFER_ALIGN
#define HAL_TFLITE_BUFFER_ALIGN 16
#endif

status_t MODEL_Init(const void *model_data,
        mpp_inference_tensor_params_t *inputTensor,
        mpp_inference_tensor_params_t *outputTensor[],
        int nb_out_tensor);
status_t MODEL_DeInit(void);
void MODEL_ConvertInput(uint8_t* data, mpp_tensor_dims_t* dims, mpp_tensor_type_t type, int mean, int std);
status_t MODEL_RunInference(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _MODEL_H_ */
