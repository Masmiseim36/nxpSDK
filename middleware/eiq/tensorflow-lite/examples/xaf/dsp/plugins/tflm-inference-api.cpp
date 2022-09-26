/*
* Copyright (c) 2015-2021 Cadence Design Systems Inc.
* Copyright 2022 NXP
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

#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/c/common.h"

#include "fsl_debug_console.h"
#include "tflm-inference-api.h"

namespace {}  // namespace

typedef struct
{
    tflite::MicroErrorReporter error_reporter;
    //void *p_micro_op_resolver;
    tflite::MicroInterpreter interpreter;
    int kTensorArenaSize;
    uint8_t tensor_arena[0];
} xa_inference_state_struct;

int inference_persistent_byte_size(int kTensorArenaSize)
{
    return kTensorArenaSize
            + sizeof(xa_inference_state_struct);
}

int inference_init(void *pPersist, void * pModel, int kTensorArenaSize, void *p_micro_op_resolver)
{
    xa_inference_state_struct *pState = (xa_inference_state_struct *)pPersist;
    tflite::MicroErrorReporter *error_reporter = new (&pState->error_reporter) tflite::MicroErrorReporter;
    const tflite::Model* model;
    model = tflite::GetModel(pModel);
        
    // Build an interpreter to run the model with.
    tflite::MicroInterpreter *interpreter = new (&pState->interpreter) tflite::MicroInterpreter(
            model, *(tflite::MicroOpResolver *) p_micro_op_resolver, (uint8_t *)pState->tensor_arena, kTensorArenaSize, error_reporter);

    pState->kTensorArenaSize = kTensorArenaSize;

    // Allocate memory from the tensor_arena for the model's tensors.
    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk) 
    {
        PRINTF("AllocateTensors() failed\n");
        return -1;
    }
    return 0;
}

int  inference_exec_process(void *pIn, int inp_bytes, void *pOut, int *out_bytes, void **output_tensor, void *pPersist)
{
    /* derive the pointers for instances */
    xa_inference_state_struct *pState = (xa_inference_state_struct *)pPersist;
    tflite::MicroInterpreter* interpreter= &pState->interpreter;

    TfLiteTensor* tensor = nullptr;
    uint8_t* tensor_data = nullptr;
    uint8_t *buffer = nullptr;

    tensor = interpreter->input(0);
    tensor_data = tensor->data.uint8;
    buffer = (uint8_t *)pIn;

    // Copy feature buffer to input tensor
    for (int i = 0; i < inp_bytes; i++) 
    {
        tensor_data[i] = buffer[i];
    }

    // Run the model on the spectrogram input and make sure it succeeds.
    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) 
    {
        PRINTF("Invoke failed\n");
        return invoke_status;
    }

    // Obtain a pointer to the output tensor
    *output_tensor = (void *) interpreter->output(0);

    tensor = interpreter->output(0);
    tensor_data = tensor->data.uint8;
    buffer = (uint8_t *)pOut;
    *out_bytes = tensor->bytes;
    int nbytes = tensor->bytes;

    // Copy output tensor data to output buffer
    for (int i = 0; i < nbytes; i++) 
    {
        buffer[i] = tensor_data[i];
    }

    return kTfLiteOk;
}

#if 0
int op_resolver_add_operator(void *pPersist, int tflm_operator)
{
    xa_inference_state_struct *pState = (xa_inference_state_struct *)pPersist;

    tflite::MicroMutableOpResolver<5> *micro_op_resolver = &pState->micro_op_resolver;

    switch(tflm_operator)
    {
        case TFLM_OPERATOR_ABS: micro_op_resolver->AddAbs(); break;
        case TFLM_OPERATOR_ADD: micro_op_resolver->AddAdd(); break;
        case TFLM_OPERATOR_ARG_MAX: micro_op_resolver->AddArgMax(); break;
        case TFLM_OPERATOR_ARG_MIN: micro_op_resolver->AddArgMin(); break;
        case TFLM_OPERATOR_AVERAGE_POOL_2D: micro_op_resolver->AddAveragePool2D(); break;
        case TFLM_OPERATOR_CEIL: micro_op_resolver->AddCeil(); break;
        case TFLM_OPERATOR_CIRCULAR_BUFFER: micro_op_resolver->AddCircularBuffer(); break;
        case TFLM_OPERATOR_CONCATENATION: micro_op_resolver->AddConcatenation(); break;
        case TFLM_OPERATOR_CONV_2D: micro_op_resolver->AddConv2D(); break;
        case TFLM_OPERATOR_COS: micro_op_resolver->AddCos(); break;
        case TFLM_OPERATOR_DEPTHWISE_CONV_2D: micro_op_resolver->AddDepthwiseConv2D(); break;
        case TFLM_OPERATOR_DEQUANTIZE: micro_op_resolver->AddDequantize(); break;
        case TFLM_OPERATOR_EQUAL: micro_op_resolver->AddEqual(); break;
        case TFLM_OPERATOR_FLOOR: micro_op_resolver->AddFloor(); break;
        case TFLM_OPERATOR_FULLY_CONNECTED: micro_op_resolver->AddFullyConnected(); break;
        case TFLM_OPERATOR_GREATER: micro_op_resolver->AddGreater(); break;
        case TFLM_OPERATOR_GREATER_EQUAL: micro_op_resolver->AddGreaterEqual(); break;
        case TFLM_OPERATOR_HARD_SWISH: micro_op_resolver->AddHardSwish(); break;
        case TFLM_OPERATOR_L2_NORMALIZATION: micro_op_resolver->AddL2Normalization(); break;
        case TFLM_OPERATOR_LESS: micro_op_resolver->AddLess(); break;
        case TFLM_OPERATOR_LESS_EQUAL: micro_op_resolver->AddLessEqual(); break;
        case TFLM_OPERATOR_LOG: micro_op_resolver->AddLog(); break;
        case TFLM_OPERATOR_LOGICAL_AND: micro_op_resolver->AddLogicalAnd(); break;
        case TFLM_OPERATOR_LOGICAL_NOT: micro_op_resolver->AddLogicalNot(); break;
        case TFLM_OPERATOR_LOGICAL_OR: micro_op_resolver->AddLogicalOr(); break;
        case TFLM_OPERATOR_LOGISTIC: micro_op_resolver->AddLogistic(); break;
        case TFLM_OPERATOR_MAXIMUM: micro_op_resolver->AddMaximum(); break;
        case TFLM_OPERATOR_MAX_POOL_2D: micro_op_resolver->AddMaxPool2D(); break;
        case TFLM_OPERATOR_MEAN: micro_op_resolver->AddMean(); break;
        case TFLM_OPERATOR_MINIMUM: micro_op_resolver->AddMinimum(); break;
        case TFLM_OPERATOR_MUL: micro_op_resolver->AddMul(); break;
        case TFLM_OPERATOR_NEG: micro_op_resolver->AddNeg(); break;
        case TFLM_OPERATOR_NOT_EQUAL: micro_op_resolver->AddNotEqual(); break;
        case TFLM_OPERATOR_PACK: micro_op_resolver->AddPack(); break;
        case TFLM_OPERATOR_PAD: micro_op_resolver->AddPad(); break;
        case TFLM_OPERATOR_PADV2: micro_op_resolver->AddPadV2(); break;
        case TFLM_OPERATOR_PRELU: micro_op_resolver->AddPrelu(); break;
        case TFLM_OPERATOR_QUANTIZE: micro_op_resolver->AddQuantize(); break;
        case TFLM_OPERATOR_REDUCE_MAX: micro_op_resolver->AddReduceMax(); break;
        case TFLM_OPERATOR_RELU: micro_op_resolver->AddRelu(); break;
        case TFLM_OPERATOR_RELU6: micro_op_resolver->AddRelu6(); break;
        case TFLM_OPERATOR_RESHAPE: micro_op_resolver->AddReshape(); break;
        case TFLM_OPERATOR_RESIZE_NEAREST_NEIGHBOR: micro_op_resolver->AddResizeNearestNeighbor(); break;
        case TFLM_OPERATOR_ROUND: micro_op_resolver->AddRound(); break;
        case TFLM_OPERATOR_RSQRT: micro_op_resolver->AddRsqrt(); break;
        case TFLM_OPERATOR_SHAPE: micro_op_resolver->AddShape(); break;
        case TFLM_OPERATOR_SIN: micro_op_resolver->AddSin(); break;
        case TFLM_OPERATOR_SOFTMAX: micro_op_resolver->AddSoftmax(); break;
        case TFLM_OPERATOR_SPLIT: micro_op_resolver->AddSplit(); break;
        case TFLM_OPERATOR_SPLIT_V: micro_op_resolver->AddSplitV(); break;
        case TFLM_OPERATOR_SQRT: micro_op_resolver->AddSqrt(); break;
        case TFLM_OPERATOR_SQUARE: micro_op_resolver->AddSquare(); break;
        case TFLM_OPERATOR_STRIDED_SLICE: micro_op_resolver->AddStridedSlice(); break;
        case TFLM_OPERATOR_SUB: micro_op_resolver->AddSub(); break;
        case TFLM_OPERATOR_SVDF: micro_op_resolver->AddSvdf(); break;
        case TFLM_OPERATOR_TANH: micro_op_resolver->AddTanh(); break;
        case TFLM_OPERATOR_UNPACK: micro_op_resolver->AddUnpack(); break;
    }

    return kTfLiteOk;
}
#endif

