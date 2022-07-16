/*
* Copyright (c) 2015-2021 Cadence Design Systems Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef __TFLM_INTERFACE_API_H__
#define __TFLM_INTERFACE_API_H__

#if 0
// Expose a C friendly interface for main functions.
#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/c/common.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if 0
typedef enum {
    TFLM_OPERATOR_ABS = 0,
    TFLM_OPERATOR_ADD,
    TFLM_OPERATOR_ARG_MAX,
    TFLM_OPERATOR_ARG_MIN,
    TFLM_OPERATOR_AVERAGE_POOL_2D,
    TFLM_OPERATOR_CEIL,
    TFLM_OPERATOR_CIRCULAR_BUFFER,
    TFLM_OPERATOR_CONCATENATION,
    TFLM_OPERATOR_CONV_2D,
    TFLM_OPERATOR_COS,
    TFLM_OPERATOR_DEPTHWISE_CONV_2D,
    TFLM_OPERATOR_DEQUANTIZE,
    TFLM_OPERATOR_EQUAL,
    TFLM_OPERATOR_FLOOR,
    TFLM_OPERATOR_FULLY_CONNECTED,
    TFLM_OPERATOR_GREATER,
    TFLM_OPERATOR_GREATER_EQUAL,
    TFLM_OPERATOR_HARD_SWISH,
    TFLM_OPERATOR_L2_NORMALIZATION,
    TFLM_OPERATOR_LESS,
    TFLM_OPERATOR_LESS_EQUAL,
    TFLM_OPERATOR_LOG,
    TFLM_OPERATOR_LOGICAL_AND,
    TFLM_OPERATOR_LOGICAL_NOT,
    TFLM_OPERATOR_LOGICAL_OR,
    TFLM_OPERATOR_LOGISTIC,
    TFLM_OPERATOR_MAXIMUM,
    TFLM_OPERATOR_MAX_POOL_2D,
    TFLM_OPERATOR_MEAN,
    TFLM_OPERATOR_MINIMUM,
    TFLM_OPERATOR_MUL,
    TFLM_OPERATOR_NEG,
    TFLM_OPERATOR_NOT_EQUAL,
    TFLM_OPERATOR_PACK,
    TFLM_OPERATOR_PAD,
    TFLM_OPERATOR_PADV2,
    TFLM_OPERATOR_PRELU,
    TFLM_OPERATOR_QUANTIZE,
    TFLM_OPERATOR_REDUCE_MAX,
    TFLM_OPERATOR_RELU,
    TFLM_OPERATOR_RELU6,
    TFLM_OPERATOR_RESHAPE,
    TFLM_OPERATOR_RESIZE_NEAREST_NEIGHBOR,
    TFLM_OPERATOR_ROUND,
    TFLM_OPERATOR_RSQRT,
    TFLM_OPERATOR_SHAPE,
    TFLM_OPERATOR_SIN,
    TFLM_OPERATOR_SOFTMAX,
    TFLM_OPERATOR_SPLIT,
    TFLM_OPERATOR_SPLIT_V,
    TFLM_OPERATOR_SQRT,
    TFLM_OPERATOR_SQUARE,
    TFLM_OPERATOR_STRIDED_SLICE,
    TFLM_OPERATOR_SUB,
    TFLM_OPERATOR_SVDF,
    TFLM_OPERATOR_TANH,
    TFLM_OPERATOR_UNPACK
} tflm_operators;
#endif

int  inference_persistent_byte_size(int kTensorArenaSize);
int  inference_init(void *pPersist, void * pModel, int kTensorArenaSize, void *op_resolver);
int  inference_exec_process(void *pIn, int inp_bytes, void *pOut, int *out_bytes, void **output_tensor, void *pPersist);
#if 0
int op_resolver_add_operator(void *pPersist, int tflm_operator);
#endif
#ifdef __cplusplus
}
#endif

#endif   //__TFLM_INTERFACE_API_H__
