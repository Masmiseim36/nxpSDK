/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.

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

#include "label_image.h"
#include "bitmap_helpers.h"
#include "get_top_n.h"

#include "stopwatch_image.h"
#include "mobilenet_v1_0.25_128_quant_model.h"
#include "labels.h"

#define LOG(x) std::cout

namespace tflite {
namespace label_image {

/* Loads a list of labels, one per line, and returns a vector of the strings.
   It pads with empty strings so the length of the result is a multiple of 16,
   because the model expects that. */
TfLiteStatus ReadLabels(const string& labels,
                        std::vector<string>* result,
                        size_t* found_label_count) {
  std::istringstream stream(labels);
  result->clear();
  string line;
  while (std::getline(stream, line)) {
    result->push_back(line);
  }
  *found_label_count = result->size();
  const int padding = 16;
  while (result->size() % padding) {
    result->emplace_back();
  }
  return kTfLiteOk;
}

void RunInference(Settings* s) {
  std::unique_ptr<tflite::FlatBufferModel> model;
  std::unique_ptr<tflite::Interpreter> interpreter;
  model = tflite::FlatBufferModel::BuildFromBuffer(mobilenet_model, mobilenet_model_len);
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

  if (s->verbose) {
    LOG(INFO) << "tensors size: " << interpreter->tensors_size() << "\r\n";
    LOG(INFO) << "nodes size: " << interpreter->nodes_size() << "\r\n";
    LOG(INFO) << "inputs: " << interpreter->inputs().size() << "\r\n";
    LOG(INFO) << "input(0) name: " << interpreter->GetInputName(0) << "\r\n";

    int t_size = interpreter->tensors_size();
    for (int i = 0; i < t_size; i++) {
      if (interpreter->tensor(i)->name)
        LOG(INFO) << i << ": " << interpreter->tensor(i)->name << ", "
                  << interpreter->tensor(i)->bytes << ", "
                  << interpreter->tensor(i)->type << ", "
                  << interpreter->tensor(i)->params.scale << ", "
                  << interpreter->tensor(i)->params.zero_point << "\r\n";
    }
  }

  int image_width = 128;
  int image_height = 128;
  int image_channels = 3;
  uint8_t* in = read_bmp(stopwatch_bmp, stopwatch_bmp_len, &image_width, &image_height,
                         &image_channels, s);
  
  int input = interpreter->inputs()[0];
  if (s->verbose) LOG(INFO) << "input: " << input << "\r\n";

  const std::vector<int> inputs = interpreter->inputs();
  const std::vector<int> outputs = interpreter->outputs();

  if (s->verbose) {
    LOG(INFO) << "number of inputs: " << inputs.size() << "\r\n";
    LOG(INFO) << "number of outputs: " << outputs.size() << "\r\n";
  }

  if (interpreter->AllocateTensors() != kTfLiteOk) {
    LOG(FATAL) << "Failed to allocate tensors!";
  }

  if (s->verbose) PrintInterpreterState(interpreter.get());

  /* Get input dimension from the input tensor metadata
     assuming one input only */
  TfLiteIntArray* dims = interpreter->tensor(input)->dims;
  int wanted_height = dims->data[1];
  int wanted_width = dims->data[2];
  int wanted_channels = dims->data[3];

  switch (interpreter->tensor(input)->type) {
    case kTfLiteFloat32:
      s->input_floating = true;
      resize<float>(interpreter->typed_tensor<float>(input), in, image_height,
                    image_width, image_channels, wanted_height, wanted_width,
                    wanted_channels, s);
      break;
    case kTfLiteUInt8:
      resize<uint8_t>(interpreter->typed_tensor<uint8_t>(input), in,
                      image_height, image_width, image_channels, wanted_height,
                      wanted_width, wanted_channels, s);
      break;
    default:
      LOG(FATAL) << "cannot handle input type "
                 << interpreter->tensor(input)->type << " yet";
      exit(-1);
  }

  auto start_time = GetTimeInUS();
  for (int i = 0; i < s->loop_count; i++) {
    if (interpreter->Invoke() != kTfLiteOk) {
      LOG(FATAL) << "Failed to invoke tflite!\r\n";
    }
  }
  auto end_time = GetTimeInUS();
  LOG(INFO) << "Average time: " << (end_time - start_time) / 1000 << " ms\r\n";

  const float threshold = 0.001f;

  std::vector<std::pair<float, int>> top_results;

  int output = interpreter->outputs()[0];
  TfLiteIntArray* output_dims = interpreter->tensor(output)->dims;
  /* Assume output dims to be something like (1, 1, ... , size) */
  auto output_size = output_dims->data[output_dims->size - 1];
  switch (interpreter->tensor(output)->type) {
    case kTfLiteFloat32:
      get_top_n<float>(interpreter->typed_output_tensor<float>(0), output_size,
                       s->number_of_results, threshold, &top_results, true);
      break;
    case kTfLiteUInt8:
      get_top_n<uint8_t>(interpreter->typed_output_tensor<uint8_t>(0),
                         output_size, s->number_of_results, threshold,
                         &top_results, false);
      break;
    default:
      LOG(FATAL) << "cannot handle output type "
                 << interpreter->tensor(input)->type << " yet";
      return;
  }

  std::vector<string> labels;
  size_t label_count;

  if (ReadLabels(labels_txt, &labels, &label_count) != kTfLiteOk)
    return;

  LOG(INFO) << "Detected:\r\n";
  for (const auto& result : top_results) {
    const float confidence = result.first;
    const int index = result.second;
    LOG(INFO) << "  " << labels[index] << " (" << (int)(confidence * 100) << "% confidence)\r\n";
  }
}

}  /* namespace label_image */
}  /* namespace tflite */

int main(void)
{
  tflite::label_image::Settings s;

  /* Init board hardware */
  BOARD_ConfigMPU();
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  InitTimer();

  std::cout << "Label image example using a TensorFlow Lite model\r\n";

  tflite::label_image::RunInference(&s);

  std::flush(std::cout);

  for (;;) {}
}
