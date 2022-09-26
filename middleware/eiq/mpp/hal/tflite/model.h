/*
 * Copyright 2020, 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MODEL_H_
#define _MODEL_H_

#include <stdint.h>

#include "fsl_common.h"
#include "hal_valgo_dev.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

status_t MODEL_Init(const void *model_data, int model_size);
uint8_t* MODEL_GetInputTensorData(mpp_tensor_dims_t* dims, mpp_tensor_type_t* type);
uint8_t* MODEL_GetOutputTensorData(mpp_tensor_dims_t* dims, mpp_tensor_type_t* type, int idx);
void MODEL_ConvertInput(uint8_t* data, mpp_tensor_dims_t* dims, mpp_tensor_type_t type, float mean, float std);
status_t MODEL_RunInference(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _MODEL_H_ */
