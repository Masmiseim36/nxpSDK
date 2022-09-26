/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Setting TFLITE_ALL_OPS_RESOLVER to 0 implies to provide an implementation of function
 * tflite::MicroOpResolver &MODEL_GetOpsResolver(tflite::ErrorReporter* errorReporter)
 * using the tensorflow-lite API.
 * Setting TFLITE_ALL_OPS_RESOLVER to 1 will automatically compile all available Operations and thus increase code size.
 **/

#ifndef TFLITE_ALL_OPS_RESOLVER
#define TFLITE_ALL_OPS_RESOLVER 0
#endif
#ifndef TFLITE_CUSTOM_OPS_DUMMY_RESOLVER
#define TFLITE_CUSTOM_OPS_DUMMY_RESOLVER 0
#endif

#if (TFLITE_ALL_OPS_RESOLVER == 1)

#include "tensorflow/lite/micro/all_ops_resolver.h"

tflite::MicroOpResolver &MODEL_GetOpsResolver(tflite::ErrorReporter* errorReporter)
{
    static tflite::AllOpsResolver s_microOpResolver;
    return s_microOpResolver;
}

#else

#if (TFLITE_CUSTOM_OPS_DUMMY_RESOLVER == 1)

/*
 * This function creates a "dummy" OPS resolver that contains no operation.
 * It can be used for applications that do not need to use TFLite
 * but still require Model_GetOpsResolver() to be defined
 */

#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"

tflite::MicroOpResolver &MODEL_GetOpsResolver(tflite::ErrorReporter* errorReporter)
{
    static tflite::MicroMutableOpResolver<1> s_microOpResolver(errorReporter);
    return s_microOpResolver;
}
#endif

#endif // TFLITE_CUSTOM_OPS_RESOLVER
