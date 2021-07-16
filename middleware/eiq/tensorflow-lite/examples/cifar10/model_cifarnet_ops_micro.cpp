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
    static tflite::MicroMutableOpResolver<6> s_microOpResolver(errorReporter);

    s_microOpResolver.AddConv2D();
    s_microOpResolver.AddAveragePool2D();
    s_microOpResolver.AddFullyConnected();
    s_microOpResolver.AddMaxPool2D();
    s_microOpResolver.AddSoftmax();
    s_microOpResolver.AddDequantize();

    return s_microOpResolver;
}
