/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Set the operations used in model MobileNet_v1. This allows reducing the code size.
 * Important Notice: User may find the list of operations needed by its model using tool https://netron.app
 */

#include "mpp_config.h"

#if (TFLITE_ALL_OPS_RESOLVER == 0)

#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/kernels/softmax.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"

tflite::MicroOpResolver &MODEL_GetOpsResolver(tflite::ErrorReporter* errorReporter)
{
    static tflite::MicroMutableOpResolver<6> s_microOpResolver(errorReporter);

    s_microOpResolver.AddAveragePool2D();
    s_microOpResolver.AddConv2D();
    s_microOpResolver.AddDepthwiseConv2D();
    s_microOpResolver.AddDequantize();
    s_microOpResolver.AddReshape();
    s_microOpResolver.AddSoftmax();

    return s_microOpResolver;
}

#endif // TFLITE_ALL_OPS_RESOLVER
