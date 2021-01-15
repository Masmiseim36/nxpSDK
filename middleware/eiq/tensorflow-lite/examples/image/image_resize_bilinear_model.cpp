/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.
   Copyright 2018-2020 NXP

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

#include "tensorflow/lite/builtin_op_data.h"
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/builtin_op_kernels.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/string_util.h"
#include "tensorflow/lite/version.h"

#include "image_utils.h"

static std::unique_ptr<tflite::Interpreter> s_interpreter;
static int s_resizeInputWidth = 0;
static int s_resizeInputHeight = 0;

// Bilinear resize using a TensorFlow Lite model
void IMAGE_Resize(uint8_t* srcData, int srcWidth, int srcHeight,
                  uint8_t* dstData, int dstWidth, int dstHeight, int channels)
{
    int number_of_pixels = srcHeight * srcWidth * channels;

    if (!s_interpreter || s_resizeInputWidth != srcWidth || s_resizeInputHeight != srcHeight)
    {
        s_interpreter.reset(new tflite::Interpreter);
        s_resizeInputWidth = srcWidth;
        s_resizeInputHeight = srcHeight;
        int base_index = 0;

        // two inputs: input and new_sizes
        s_interpreter->AddTensors(2, &base_index);
        // one output
        s_interpreter->AddTensors(1, &base_index);
        // set input and output tensors
        s_interpreter->SetInputs({0, 1});
        s_interpreter->SetOutputs({2});

        // set parameters of tensors
        TfLiteQuantizationParams quant = {.scale = 1.0, .zero_point = 0};
        s_interpreter->SetTensorParametersReadWrite(
            0, kTfLiteUInt8, "input",
            {1, srcHeight, srcWidth, channels}, quant);
        s_interpreter->SetTensorParametersReadWrite(
            1, kTfLiteInt32, "new_size", {2}, quant);
        s_interpreter->SetTensorParametersReadWrite(
            2, kTfLiteUInt8, "output",
            {1, dstHeight, dstWidth, channels}, quant);

        tflite::MutableOpResolver resolver;
        resolver.AddBuiltin(tflite::BuiltinOperator_RESIZE_BILINEAR,
                            tflite::ops::builtin::Register_RESIZE_BILINEAR(),
                            /* min_version */ 1,
                            /* max_version */ 2);
        const TfLiteRegistration* resize_op =
            resolver.FindOp(tflite::BuiltinOperator_RESIZE_BILINEAR, 1);
        auto* params = reinterpret_cast<TfLiteResizeBilinearParams*>(
            malloc(sizeof(TfLiteResizeBilinearParams)));
        params->align_corners = false;
        s_interpreter->AddNodeWithParameters(
            {0, 1}, {2}, nullptr, 0, params, resize_op, nullptr);

        s_interpreter->AllocateTensors();
    }

    // fill input image
    auto input = s_interpreter->typed_tensor<uint8_t>(0);
    for (int i = 0; i < number_of_pixels; i++)
    {
        input[i] = srcData[i];
    }

    // fill new_sizes
    s_interpreter->typed_tensor<int>(1)[0] = dstHeight;
    s_interpreter->typed_tensor<int>(1)[1] = dstWidth;

    s_interpreter->Invoke();

    auto output = s_interpreter->typed_tensor<uint8_t>(2);
    auto output_number_of_pixels = dstHeight * dstWidth * channels;

    for (int i = 0; i < output_number_of_pixels; i++)
    {
        dstData[i] = output[i];
    }
}
