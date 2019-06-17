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

#include "cifar10_model.h"
#include "ship_image.h"
#include "get_top_n.h"

#define LOG(x) std::cout

void RunInference()
{
  std::unique_ptr<tflite::FlatBufferModel> model;
  std::unique_ptr<tflite::Interpreter> interpreter;
  model = tflite::FlatBufferModel::BuildFromBuffer(cifar10_model, cifar10_model_len);
  if (!model) {
    LOG(FATAL) << "Failed to load model\r\n";
    exit(-1);
  }
  model->error_reporter();

  tflite::ops::builtin::BuiltinOpResolver resolver;

  tflite::InterpreterBuilder(*model, resolver)(&interpreter);
  if (!interpreter) {
    LOG(FATAL) << "Failed to construct interpreter\r\n";
    exit(-1);
  }

  int input = interpreter->inputs()[0];

  const std::vector<int> inputs = interpreter->inputs();
  const std::vector<int> outputs = interpreter->outputs();

  if (interpreter->AllocateTensors() != kTfLiteOk) {
    LOG(FATAL) << "Failed to allocate tensors!\r\n";
  }

  /* Input dimension from the input tensor metadata
     assuming one input only, dimensions could be read from
     interpreter->tensor(input)->dims. Where size of the picture
     is 32*32*3. This size is the same as input size of the model.
     No resize is needed. Input size of the model could be check using:
       TfLiteIntArray* dims = interpreter->tensor(input)->dims;
       int height = dims->data[1];
       int width = dims->data[2];
       int channels = dims->data[3]; */

  uint8_t* input_tensor_data = interpreter->typed_tensor<uint8_t>(input);
  memcpy(input_tensor_data, ship_image, ship_image_len);

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
  TfLiteIntArray* output_dims = interpreter->tensor(output)->dims;
  /* Assume output dims to be something like (1, 1, ... , size) */
  auto output_size = output_dims->data[output_dims->size - 1];
  get_top_n<uint8_t>(interpreter->typed_output_tensor<uint8_t>(0),
                     output_size, 1, threshold,
                     &top_results, false);

  std::vector<std::string> labels = {
    "airplane", "automobile", "bird", "cat", "deer", "dog", "frog", "horse", "ship", "truck"
  };

  for (const auto& result : top_results) {
    const float confidence = result.first;
    const int index = result.second;
    LOG(INFO) << "Detected: " << labels[index] << " (" << (int)(confidence * 100) << "% confidence)\r\n";
  }
}

/*
 * @brief   Application entry point.
 */
int main(void)
{
  /* Init board hardware */
  BOARD_ConfigMPU();
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  InitTimer();

  std::cout << "CIFAR-10 object recognition example using a TensorFlow Lite model\r\n";

  RunInference();

  std::flush(std::cout);

  for (;;) {}
}
