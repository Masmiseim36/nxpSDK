/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/kernels/builtin_op_kernels.h"

void MODEL_RegisterOps(tflite::MutableOpResolver &resolver)
{
    resolver.AddBuiltin(tflite::BuiltinOperator_MAX_POOL_2D,
                        tflite::ops::builtin::Register_MAX_POOL_2D(),
                        /* min_version */ 1,
                        /* max_version */ 2);
    resolver.AddBuiltin(tflite::BuiltinOperator_CONV_2D,
                        tflite::ops::builtin::Register_CONV_2D(),
                        /* min_version */ 1,
                        /* max_version */ 3);
    resolver.AddBuiltin(tflite::BuiltinOperator_FULLY_CONNECTED,
                        tflite::ops::builtin::Register_FULLY_CONNECTED(),
                        /* min_version */ 1,
                        /* max_version */ 6);
    resolver.AddBuiltin(tflite::BuiltinOperator_SOFTMAX,
                        tflite::ops::builtin::Register_SOFTMAX(),
                        /* min_version */ 1,
                        /* max_version */ 2);
}
