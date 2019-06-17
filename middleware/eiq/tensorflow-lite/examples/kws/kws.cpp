/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.
   Copyright 2018 NXP. All Rights Reserved.

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

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "timer.h"

#include <iostream>
#include <string>
#include <vector>

#include "tensorflow/contrib/lite/kernels/register.h"
#include "tensorflow/contrib/lite/model.h"
#include "tensorflow/contrib/lite/optional_debug_tools.h"
#include "tensorflow/contrib/lite/string_util.h"

#include "tiny_conv_model_data.h"
#include "yes_features_data.h"
#include "get_top_n.h"

#define LOG(x) std::cout

void RunInference(void)
{
  std::unique_ptr<tflite::FlatBufferModel> model;
  std::unique_ptr<tflite::Interpreter> interpreter;
  model = tflite::FlatBufferModel::BuildFromBuffer(g_tiny_conv_model_data, g_tiny_conv_model_data_len);
  if (!model) {
    LOG(FATAL) << "Failed to load model\r\n";
    return;
  }
  model->error_reporter();

  tflite::ops::builtin::BuiltinOpResolver resolver;

  tflite::InterpreterBuilder(*model, resolver)(&interpreter);
  if (!interpreter) {
    LOG(FATAL) << "Failed to construct interpreter\r\n";
    return;
  }
  
  int input = interpreter->inputs()[0];

  const std::vector<int> inputs = interpreter->inputs();
  const std::vector<int> outputs = interpreter->outputs();

  if (interpreter->AllocateTensors() != kTfLiteOk) {
    LOG(FATAL) << "Failed to allocate tensors!\r\n";
  }

  /* Get input dimension from the input tensor metadata
     assuming one input only */
  TfLiteTensor* input_tensor = interpreter->tensor(input);
  int input_bytes = input_tensor->bytes;
  for (int i = 0; i < input_bytes; i++) {
    input_tensor->data.uint8[i] = g_yes_features_data[i];
  }

  auto start_time = GetTimeInUS();
  if (interpreter->Invoke() != kTfLiteOk) {
    LOG(FATAL) << "Failed to invoke tflite!\r\n";
    return;
  }
  auto end_time = GetTimeInUS();
  LOG(INFO) << "Elapsed time: "
            << (end_time - start_time) / 1000
            << " ms\r\n";

  const float threshold = 0.001f;

  std::vector<std::pair<float, int>> top_results;

  int output = interpreter->outputs()[0];
  TfLiteTensor* output_tensor = interpreter->tensor(output);
  TfLiteIntArray* output_dims = output_tensor->dims;
  /* Assume output dims to be something like (1, 1, ... , size) */
  auto output_size = output_dims->data[output_dims->size - 1];
  get_top_n<uint8_t>(interpreter->typed_output_tensor<uint8_t>(0),
                     output_size, 1, threshold,
                     &top_results, false);

  std::vector<std::string> labels = {"Silence", "Unknown", "yes", "no"};

  for (const auto& result : top_results) {
    const float confidence = result.first;
    const int index = result.second;
    LOG(INFO) << "Detected: " << labels[index] << " (" << (int)(confidence * 100) << "% confidence)\r\n";
  }
}

int main(void)
{
  /* Init board hardware */
  BOARD_ConfigMPU();
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  InitTimer();

  std::cout << "Keyword spotting example using a TensorFlow Lite model\r\n";

  RunInference();

  std::flush(std::cout);

  for (;;) {}
}
