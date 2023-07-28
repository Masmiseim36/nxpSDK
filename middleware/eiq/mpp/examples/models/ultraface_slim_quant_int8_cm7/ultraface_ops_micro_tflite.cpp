/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Set the operations used in model Nanodet-M. This allows reducing the code size.
 * Important Notice: User may find the list of operations needed by its model using tool https://netron.app
 */

#include "mpp_config.h"

#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/kernels/softmax.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"

tflite::MicroOpResolver &MODEL_GetOpsResolver()
{
	//return tflite::AllOpsResolver();

    static tflite::MicroMutableOpResolver<22> s_microOpResolver;

    s_microOpResolver.AddConv2D();
    s_microOpResolver.AddDepthwiseConv2D();
    s_microOpResolver.AddPad();
    s_microOpResolver.AddRelu();
    s_microOpResolver.AddMaxPool2D();
    s_microOpResolver.AddConcatenation();
    s_microOpResolver.AddReshape();
    s_microOpResolver.AddTranspose();
    s_microOpResolver.AddSplit();
    s_microOpResolver.AddResizeBilinear();
    s_microOpResolver.AddAdd();
    s_microOpResolver.AddSub();
    s_microOpResolver.AddSlice();
    s_microOpResolver.AddShape();
    s_microOpResolver.AddStridedSlice();
    s_microOpResolver.AddPack();
    s_microOpResolver.AddSoftmax();
    s_microOpResolver.AddQuantize();
    s_microOpResolver.AddDequantize();
    s_microOpResolver.AddMul();
    s_microOpResolver.AddExp();
    s_microOpResolver.AddMinimum();

    return s_microOpResolver;
}
