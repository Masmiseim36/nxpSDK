/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.

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

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

#include <time.h>

#include <iostream>
#include "timer.h"

#include "tensorflow/lite/tools/benchmark/benchmark_tflite_model.h"
#include "tensorflow/lite/tools/benchmark/benchmark_params.h"
#include "tensorflow/lite/tools/benchmark/logging.h"

using tflite::benchmark::BenchmarkTfLiteModel;
using tflite::benchmark::BenchmarkParam;
using tflite::benchmark::BenchmarkLoggingListener;

int main(void) {
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();
  InitTimer();

  TFLITE_LOG(INFO) << "STARTING!";
  BenchmarkTfLiteModel benchmark;
  BenchmarkLoggingListener listener;
  benchmark.AddListener(&listener);

  tflite::benchmark::BenchmarkParams params;

  /* Benchmark configuration. */
  params.AddParam("num_threads",
                   BenchmarkParam::Create<int32_t>(1));
  params.AddParam("warmup_runs", BenchmarkParam::Create<int32_t>(1));
  params.AddParam("num_runs", BenchmarkParam::Create<int32_t>(50));
  params.AddParam("warmup_min_secs", BenchmarkParam::Create<float>(0.5f));
  params.AddParam("min_secs", BenchmarkParam::Create<float>(1.0f));
  params.AddParam("max_secs", BenchmarkParam::Create<float>(150.0f));
  params.AddParam("enable_op_profiling", BenchmarkParam::Create<bool>(true));

  if (benchmark.Run(params) != kTfLiteOk) {
    TFLITE_LOG(ERROR) << "Benchmarking failed.";
    return EXIT_FAILURE;
  }

  std::cout << "Benchmark has been finished" << std::endl;

  for (;;)
  {
  }
}
