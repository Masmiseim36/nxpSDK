/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.
   Copyright 2021-2024 NXP

SPDX-License-Identifier: Apache-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

/* File modified by NXP. Changes are described in file
   /middleware/eiq/tensorflow-lite/readme.txt in section "Release notes" */

#include "mpp_config.h"

#if (HAL_ENABLE_INFERENCE_TFLITE == 1)

#include <stdio.h>
#include "hal_valgo_dev.h"
#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include "model.h"

/* trick to replace 'float division' with 'multiply by integer and bitshift'
   integer is the inverse multiplied by factor to keep precision */
#define FAST_DIV_BITS 16
#define FAST_DIV_FACTOR (1 << FAST_DIV_BITS)

/* TODO replace by dynamic object construction to allow multiple instances to run concurrently */
static const tflite::Model* s_model = nullptr;
static tflite::MicroInterpreter* s_interpreter = nullptr;

extern tflite::MicroOpResolver &MODEL_GetOpsResolver();

uint8_t* MODEL_GetInputTensorData(tflite::MicroInterpreter* interpreter, mpp_tensor_dims_t* dims, mpp_tensor_type_t* type);
uint8_t* MODEL_GetOutputTensorData(tflite::MicroInterpreter* interpreter, mpp_tensor_dims_t* dims, mpp_tensor_type_t* type, int idx);

// An area of memory to use for input, output, and intermediate arrays.
// (Can be adjusted based on the model needs.)
constexpr int kTensorArenaSize = HAL_TFLM_TENSOR_ARENA_SIZE_KB * 1024;

// On some devices tensor arena should be non-cacheable
#if defined(HAL_TENSOR_ARENA_NCACHE) && (HAL_TENSOR_ARENA_NCACHE == 1)
static uint8_t s_tensorArena[kTensorArenaSize] __ALIGNED(HAL_TFLITE_BUFFER_ALIGN) __attribute__((section("NonCacheable")));
#else
static uint8_t s_tensorArena[kTensorArenaSize] __ALIGNED(HAL_TFLITE_BUFFER_ALIGN);
#endif

status_t MODEL_Init(const void *model_data,
        mpp_inference_tensor_params_t *inputTensor,
        mpp_inference_tensor_params_t *outputTensor[],
        int nb_out_tensor)
{
    // Map the model into a usable data structure. This doesn't involve any
    // copying or parsing, it's a very lightweight operation.
    s_model = tflite::GetModel(model_data);
    if (s_model->version() != TFLITE_SCHEMA_VERSION)
    {
        HAL_LOGE("Model provided is schema version %d not equal "
               "to supported version %d.",
               s_model->version(), TFLITE_SCHEMA_VERSION);
        return kStatus_Fail;
    }

    // Pull in only the operation implementations we need.
    // This relies on a complete list of all the ops needed by this graph.
    // NOLINTNEXTLINE(runtime-global-variables)
    static tflite::MicroOpResolver &s_micro_op_resolver = MODEL_GetOpsResolver();

    // Build an interpreter to run the model with.
    s_interpreter = new tflite::MicroInterpreter(
            s_model, s_micro_op_resolver, s_tensorArena, kTensorArenaSize);

    // Allocate memory from the tensor_arena for the model's tensors.
    TfLiteStatus allocate_status = s_interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk)
    {
    	HAL_LOGE("AllocateTensors() failed");
        return kStatus_Fail;
    }

    inputTensor->data = MODEL_GetInputTensorData(s_interpreter, &inputTensor->dims, &inputTensor->type);

    for(int i = 0; i < nb_out_tensor; i++)
    {
        outputTensor[i]->data = MODEL_GetOutputTensorData(s_interpreter, &outputTensor[i]->dims, &outputTensor[i]->type, i);
    }

    return kStatus_Success;
}

status_t MODEL_DeInit(void)
{
    s_interpreter->Reset();
    delete s_interpreter;

    return kStatus_Success;
}

status_t MODEL_RunInference(void)
{
    if (s_interpreter->Invoke() != kTfLiteOk)
    {
        HAL_LOGE("Invoke failed!\r\n");
        return kStatus_Fail;
    }

    return kStatus_Success;
}

uint8_t* GetTensorData(TfLiteTensor* tensor, mpp_tensor_dims_t* dims, mpp_tensor_type_t* type)
{
    switch (tensor->type)
    {
        case kTfLiteFloat32:
            *type = MPP_TENSOR_TYPE_FLOAT32;
            break;
        case kTfLiteUInt8:
            *type = MPP_TENSOR_TYPE_UINT8;
            break;
        case kTfLiteInt8:
            *type = MPP_TENSOR_TYPE_INT8;
            break;
        default:
            assert("Unknown input tensor data type");
    };

    dims->size = tensor->dims->size;
    assert(dims->size <= MAX_TENSOR_DIMS);
    for (int i = 0; i < tensor->dims->size; i++)
    {
        dims->data[i] = tensor->dims->data[i];
    }

    return tensor->data.uint8;
}

uint8_t* MODEL_GetInputTensorData(tflite::MicroInterpreter* interpreter, mpp_tensor_dims_t* dims, mpp_tensor_type_t* type)
{
	TfLiteTensor* inputTensor = interpreter->input(0);

    return GetTensorData(inputTensor, dims, type);
}

uint8_t* MODEL_GetOutputTensorData(tflite::MicroInterpreter* interpreter, mpp_tensor_dims_t* dims, mpp_tensor_type_t* type, int idx)
{
    /* handles multiple outputs */
    TfLiteTensor* outputTensor = interpreter->output(idx);

    return GetTensorData(outputTensor, dims, type);
}

// Convert and normalize unsigned 8-bit image data to model input format in-place.
void MODEL_ConvertInput(uint8_t* data, mpp_tensor_dims_t* dims, mpp_tensor_type_t type, int mean, int std)
{
    int size = dims->data[2] * dims->data[1] * dims->data[3];
    /* Quantization parameters:
     * input_scale : model input scale.
     * input_zero_point: model input zero point.
     */
    float input_scale = s_interpreter->input(0)->params.scale;
    float input_zero_point = s_interpreter->input(0)->params.zero_point;
    int inv_scale = 0;
    int i_zero_point = input_zero_point;
    switch (type)
    {
        case MPP_TENSOR_TYPE_UINT8:
            break;
        case MPP_TENSOR_TYPE_INT8:
             /* to calculate quantized value:
             * quantized_value = real_value / scale + zero_point
             * to normalize the input data:
             * normalized_value = (real_value - mean) / std
             *
             * these two formulas can be combined to perform both normalization and
             * quantization in the same time:
             * final_value = (real_value - mean) / (scale * std) + zero_point
             */

            /* check if normalization is needed. */
            if ((mean != 0) || (std != 1))
            {
                if (std != 0)
                {
                    inv_scale = FAST_DIV_FACTOR / (input_scale * std);
                    for (int i = size - 1; i >= 0; i--)
                    {
                        /* optimized form of: ((data[i] - i_mean) / scale) + zero_point */
                        data[i] = (int8_t) ((data[i] - mean) * inv_scale >> FAST_DIV_BITS) + i_zero_point;
                    }
                 }
                else
                {
                    HAL_LOGE("Standard deviation should be different of 0.");
                    break;
                }
            }
            else /* only quantization should be performed */
            {
                inv_scale = FAST_DIV_FACTOR / input_scale;
                for (int i = size - 1; i >= 0; i--)
                {
                    /* optimized form of: (data[i] / scale) + zero_point */
                    data[i] = (int8_t) (data[i] * inv_scale >> FAST_DIV_BITS) + i_zero_point;
        	    }
            }
            break;
        case MPP_TENSOR_TYPE_FLOAT32:
            for (int i = size - 1; i >= 0; i--)
            {
                reinterpret_cast<float*>(data)[i] =
                    (static_cast<int>(data[i]) - input_zero_point) / input_scale;
            }
            break;
        default:
            assert("Unknown input tensor data type");
    }
}
#endif /* (HAL_ENABLE_INFERENCE_TFLITE == 1) */
