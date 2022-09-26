/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MODEL_H_
#define _MODEL_H_
#define SIGN_UINT 128
#define GLOW_MEMORY_ALIGN 64 /*Alignment used for weights and activations*/

#include <stdint.h>

#include "fsl_common.h"
#include "hal_valgo_dev.h"
#include "hal.h"


#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

status_t GLOW_ConvertInput(uint8_t* input_Addr, mpp_tensor_type_t type, int input_size);
status_t GLOW_MODEL_RunInference(inference_entry_point_t model_entry_point, uint8_t * constant_weights, uint8_t * mutable_weights, uint8_t * activations);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _MODEL_H_ */
