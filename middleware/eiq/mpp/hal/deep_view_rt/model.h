/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MODEL_H_
#define _MODEL_H_

#include <stdint.h>

#include "fsl_common.h"
#include "hal_valgo_dev.h"
#include "hal.h"
#include "deepview_rt.h"
#include "deepview_ops.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

NNContext* dvrt_model_init(const void *model_data, size_t model_size);
uint8_t* dvrt_get_input_layer_tensor(const void *model_data, NNContext* context, mpp_tensor_type_t* type,
        mpp_tensor_dims_t* dims, size_t* tensor_size);
const uint8_t* dvrt_get_output_layer_tensor(const void *model_data, NNContext* context,
        mpp_tensor_dims_t* dims, mpp_tensor_type_t* type, int output_idx);
status_t dvrt_convert_input(uint8_t* input_addr, mpp_tensor_type_t type, int input_size);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _MODEL_H_ */
