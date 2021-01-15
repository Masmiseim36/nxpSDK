/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <iostream>

#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/optional_debug_tools.h"
#include "tensorflow/lite/string_util.h"

#include "model.h"
#include "model_data.h"

std::unique_ptr<tflite::FlatBufferModel> model;
std::unique_ptr<tflite::Interpreter> s_interpreter;
TfLiteTensor* inputTensor;

extern void MODEL_RegisterOps(tflite::MutableOpResolver &resolver);

status_t MODEL_Init()
{
    model = tflite::FlatBufferModel::BuildFromBuffer(model_data, model_data_len);
    if (!model)
    {
        std::cerr << "Failed to load model.\r\n";
        return kStatus_Fail;
    }

    tflite::MutableOpResolver resolver;
    MODEL_RegisterOps(resolver);

    tflite::InterpreterBuilder(*model, resolver)(&s_interpreter);
    if (!s_interpreter)
    {
        std::cerr << "Failed to construct interpreter.\r\n";
        return kStatus_Fail;
    }

    int input = s_interpreter->inputs()[0];

    const std::vector<int> inputs = s_interpreter->inputs();
    const std::vector<int> outputs = s_interpreter->outputs();

    if (s_interpreter->AllocateTensors() != kTfLiteOk)
    {
        std::cerr << "Failed to allocate tensors!\r\n";
        return kStatus_Fail;
    }

    /* Get input dimension from the input tensor metadata
       assuming one input only */
    inputTensor = s_interpreter->tensor(input);

#ifdef DEMO_VERBOSE
    const std::vector<int> inputs = s_interpreter->inputs();
    const std::vector<int> outputs = s_interpreter->outputs();

    std::cout << "input: " << inputs[0] << "\r\n";
    std::cout << "number of inputs: " << inputs.size() << "\r\n";
    std::cout << "number of outputs: " << outputs.size() << "\r\n";

    std::cout << "tensors size: " << s_interpreter->tensors_size() << "\r\n";
    std::cout << "nodes size: " << s_interpreter->nodes_size() << "\r\n";
    std::cout << "inputs: " << s_interpreter->inputs().size() << "\r\n";
    std::cout << "input(0) name: " << s_interpreter->GetInputName(0) << "\r\n";

    int t_size = s_interpreter->tensors_size();
    TfLiteTensor *tensorPtr = 0;
    for (int i = 0; i < t_size; i++)
    {
        if (s_interpreter->tensor(i)->name)
        {
            tensorPtr = s_interpreter->tensor(i);
            std::cout << i << ": " << tensorPtr->name << ", "
                      << tensorPtr->bytes << ", "
                      << tensorPtr->type << ", "
                      << tensorPtr->params.scale << ", "
                      << tensorPtr->params.zero_point << "\r\n";
        }
    }

    std::cout << "\r\n";
#endif

    return kStatus_Success;
}

status_t MODEL_RunInference()
{
    if (s_interpreter->Invoke() != kTfLiteOk)
    {
        std::cerr << "Failed to invoke tflite!\r\n" << std::endl;
        return kStatus_Fail;
    }

    return kStatus_Success;
}

uint8_t* GetTensorData(TfLiteTensor* tensor, tensor_dims_t* dims, tensor_type_t* type)
{
    switch (tensor->type)
    {
        case kTfLiteFloat32:
            *type = kTensorType_FLOAT32;
            break;
        case kTfLiteUInt8:
            *type = kTensorType_UINT8;
            break;
        case kTfLiteInt8:
            *type = kTensorType_INT8;
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

uint8_t* MODEL_GetInputTensorData(tensor_dims_t* dims, tensor_type_t* type)
{
    return GetTensorData(inputTensor, dims, type);
}

uint8_t* MODEL_GetOutputTensorData(tensor_dims_t* dims, tensor_type_t* type)
{
    int output = s_interpreter->outputs()[0];
    TfLiteTensor* outputTensor = s_interpreter->tensor(output);

    return GetTensorData(outputTensor, dims, type);
}

// Convert unsigned 8-bit image data to model input format in-place.
void MODEL_ConvertInput(uint8_t* data, tensor_dims_t* dims, tensor_type_t type)
{
    int size = dims->data[2] * dims->data[1] * dims->data[3];
    switch (type)
    {
        case kTensorType_UINT8:
            break;
        case kTensorType_INT8:
            for (int i = size - 1; i >= 0; i--)
            {
                reinterpret_cast<int8_t*>(data)[i] =
                    static_cast<int>(data[i]) - 127;
            }
            break;
        case kTensorType_FLOAT32:
            for (int i = size - 1; i >= 0; i--)
            {
                reinterpret_cast<float*>(data)[i] =
                    (static_cast<int>(data[i]) - MODEL_INPUT_MEAN) / MODEL_INPUT_STD;
            }
            break;
        default:
            assert("Unknown input tensor data type");
    }
}

const char* MODEL_GetModelName()
{
    return MODEL_NAME;
}
