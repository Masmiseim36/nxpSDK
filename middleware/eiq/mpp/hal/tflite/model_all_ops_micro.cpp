/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mpp_config.h"
#if (HAL_ENABLE_INFERENCE_TFLITE == 1)

#include "tensorflow/lite/micro/all_ops_resolver.h"

tflite::MicroOpResolver __attribute__((weak)) &MODEL_GetOpsResolver()
{
    static tflite::AllOpsResolver s_microOpResolver;
    return s_microOpResolver;
}

#endif // (HAL_ENABLE_INFERENCE_TFLITE == 1)
