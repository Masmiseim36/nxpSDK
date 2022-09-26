/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Set the operations used in model Nanodet-M. This allows reducing the code size.
 * Important Notice: User may find the list of operations needed by its model using tool https://netron.app
 */

#include "mpp_config.h"

#if (TFLITE_ALL_OPS_RESOLVER == 0)

#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/kernels/softmax.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"

tflite::MicroOpResolver &MODEL_GetOpsResolver(tflite::ErrorReporter* errorReporter)
{
    static tflite::MicroMutableOpResolver<13> s_microOpResolver(errorReporter);

    s_microOpResolver.AddConv2D();
    s_microOpResolver.AddDepthwiseConv2D();
    s_microOpResolver.AddPad();
    s_microOpResolver.AddLeakyRelu();
    s_microOpResolver.AddMaxPool2D();
    s_microOpResolver.AddConcatenation();
    s_microOpResolver.AddReshape();
    s_microOpResolver.AddTranspose();
    s_microOpResolver.AddSplit();
    s_microOpResolver.AddResizeBilinear();
    s_microOpResolver.AddAdd();
    s_microOpResolver.AddLogistic();
    s_microOpResolver.AddSlice();

    return s_microOpResolver;
}

#endif // TFLITE_ALL_OPS_RESOLVER
