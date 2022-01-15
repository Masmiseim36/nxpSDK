/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"

tflite::MicroOpResolver &MODEL_GetOpsResolver(tflite::ErrorReporter* errorReporter)
{
    static tflite::MicroMutableOpResolver<8> s_microOpResolver(errorReporter);

    s_microOpResolver.AddAveragePool2D();
    s_microOpResolver.AddConv2D();
    s_microOpResolver.AddDepthwiseConv2D();
    s_microOpResolver.AddDequantize();
    s_microOpResolver.AddFullyConnected();
    s_microOpResolver.AddQuantize();
    s_microOpResolver.AddReshape();
    s_microOpResolver.AddSoftmax();

    return s_microOpResolver;
}
