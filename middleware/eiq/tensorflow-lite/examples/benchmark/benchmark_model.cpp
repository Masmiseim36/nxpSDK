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

#include "tensorflow/lite/tools/benchmark/benchmark_model.h"

#include <iostream>
#include <sstream>

#include "tensorflow/lite/profiling/memory_info.h"
#include "tensorflow/lite/profiling/time.h"
#include "tensorflow/lite/tools/benchmark/benchmark_utils.h"
#include "tensorflow/lite/tools/benchmark/logging.h"

namespace tflite {
namespace benchmark {
namespace {
using tflite::profiling::memory::MemoryUsage;
void LogMemUsageDelta(const std::string& tag, const MemoryUsage& before,
                      const MemoryUsage& after) {
  const auto delta = after - before;
  TFLITE_LOG(INFO) << "[" << tag << "] - Memory usage: max resident set size = "
                   << delta.max_rss_kb / 1024.0
                   << " MB, total malloc-ed size = "
                   << delta.total_allocated_bytes / 1024.0 / 1024.0 << " MB";
}
}  // namespace

using tensorflow::Stat;

BenchmarkParams BenchmarkModel::DefaultParams() {
  BenchmarkParams params;
  params.AddParam("num_runs", BenchmarkParam::Create<int32_t>(50));
  params.AddParam("min_secs", BenchmarkParam::Create<float>(1.0f));
  params.AddParam("max_secs", BenchmarkParam::Create<float>(150.0f));
  params.AddParam("run_delay", BenchmarkParam::Create<float>(-1.0f));
  params.AddParam("num_threads", BenchmarkParam::Create<int32_t>(1));
  params.AddParam("benchmark_name", BenchmarkParam::Create<std::string>(""));
  params.AddParam("output_prefix", BenchmarkParam::Create<std::string>(""));
  params.AddParam("warmup_runs", BenchmarkParam::Create<int32_t>(1));
  params.AddParam("warmup_min_secs", BenchmarkParam::Create<float>(0.5f));
  return params;
}

BenchmarkModel::BenchmarkModel() : params_(DefaultParams()) {}

void BenchmarkLoggingListener::OnBenchmarkEnd(const BenchmarkResults& results) {
  auto inference_us = results.inference_time_us();
  auto init_us = results.startup_latency_us();
  auto warmup_us = results.warmup_time_us();
  TFLITE_LOG(INFO) << "Average inference timings in us: "
                   << "Warmup: " << warmup_us.avg() << ", "
                   << "Init: " << init_us << ", "
                   << "no stats: " << inference_us.avg();
}

std::vector<Flag> BenchmarkModel::GetFlags() {
  return {
      CreateFlag<int32_t>(
          "num_runs", &params_,
          "expected number of runs, see also min_secs, max_secs"),
      CreateFlag<float>(
          "min_secs", &params_,
          "minimum number of seconds to rerun for, potentially making the "
          "actual number of runs to be greater than num_runs"),
      CreateFlag<float>(
          "max_secs", &params_,
          "maximum number of seconds to rerun for, potentially making the "
          "actual number of runs to be less than num_runs. Note if --max-secs "
          "is exceeded in the middle of a run, the benchmark will continue to "
          "the end of the run but will not start the next run."),
      CreateFlag<float>("run_delay", &params_, "delay between runs in seconds"),
      CreateFlag<int32_t>("num_threads", &params_, "number of threads"),
      CreateFlag<std::string>("benchmark_name", &params_, "benchmark name"),
      CreateFlag<std::string>("output_prefix", &params_,
                              "benchmark output prefix"),
      CreateFlag<int32_t>(
          "warmup_runs", &params_,
          "minimum number of runs performed on initialization, to "
          "allow performance characteristics to settle, see also "
          "warmup_min_secs"),
      CreateFlag<float>(
          "warmup_min_secs", &params_,
          "minimum number of seconds to rerun for, potentially making the "
          "actual number of warm-up runs to be greater than warmup_runs"),
  };
}

void BenchmarkModel::LogParams() {
  TFLITE_LOG(INFO) << "Min num runs: [" << params_.Get<int32_t>("num_runs")
                   << "]";
  TFLITE_LOG(INFO) << "Min runs duration (seconds): ["
                   << params_.Get<float>("min_secs") << "]";
  TFLITE_LOG(INFO) << "Max runs duration (seconds): ["
                   << params_.Get<float>("max_secs") << "]";
  TFLITE_LOG(INFO) << "Inter-run delay (seconds): ["
                   << params_.Get<float>("run_delay") << "]";
  TFLITE_LOG(INFO) << "Num threads: [" << params_.Get<int32_t>("num_threads")
                   << "]";
  TFLITE_LOG(INFO) << "Benchmark name: ["
                   << params_.Get<std::string>("benchmark_name") << "]";
  TFLITE_LOG(INFO) << "Output prefix: ["
                   << params_.Get<std::string>("output_prefix") << "]";
  TFLITE_LOG(INFO) << "Min warmup runs: ["
                   << params_.Get<int32_t>("warmup_runs") << "]";
  TFLITE_LOG(INFO) << "Min warmup runs duration (seconds): ["
                   << params_.Get<float>("warmup_min_secs") << "]";
}

TfLiteStatus BenchmarkModel::PrepareInputData() { return kTfLiteOk; }

TfLiteStatus BenchmarkModel::ResetInputsAndOutputs() { return kTfLiteOk; }

Stat<int64_t> BenchmarkModel::Run(int min_num_times, float min_secs,
                                  float max_secs, RunType run_type,
                                  TfLiteStatus* invoke_status) {
  Stat<int64_t> run_stats;
  TFLITE_LOG(INFO) << "Running benchmark for at least " << min_num_times
                   << " iterations and at least " << min_secs << " seconds but"
                   << " terminate if exceeding " << max_secs << " seconds.";
  int64_t now_us = profiling::time::NowMicros();
  int64_t min_finish_us = now_us + static_cast<int64_t>(min_secs * 1.e6f);
  int64_t max_finish_us = now_us + static_cast<int64_t>(max_secs * 1.e6f);

  *invoke_status = kTfLiteOk;
  for (int run = 0;
	   (run < min_num_times || now_us < min_finish_us) && now_us <= max_finish_us;
       run++) {
    ResetInputsAndOutputs();
    listeners_.OnSingleRunStart(run_type);
    int64_t start_us = profiling::time::NowMicros();
    TfLiteStatus status = RunImpl();
    int64_t end_us = profiling::time::NowMicros();
    listeners_.OnSingleRunEnd();

    run_stats.UpdateStat(end_us - start_us);
    util::SleepForSeconds(params_.Get<float>("run_delay"));
    now_us = profiling::time::NowMicros();

    if (status != kTfLiteOk) {
      *invoke_status = status;
    }
  }

  std::stringstream stream;
  run_stats.OutputToStream(&stream);
  TFLITE_LOG(INFO) << stream.str() << std::endl;

  return run_stats;
}

TfLiteStatus BenchmarkModel::ValidateParams() { return kTfLiteOk; }

TfLiteStatus BenchmarkModel::Run(const BenchmarkParams &params) {
  params_.Set(params);
  return Run();
}

TfLiteStatus BenchmarkModel::Run() {
  TF_LITE_ENSURE_STATUS(ValidateParams());

  LogParams();

  const auto start_mem_usage = profiling::memory::GetMemoryUsage();
  int64_t initialization_start_us = profiling::time::NowMicros();
  TF_LITE_ENSURE_STATUS(Init());
  const auto init_end_mem_usage = profiling::memory::GetMemoryUsage();
  int64_t initialization_end_us = profiling::time::NowMicros();
  int64_t startup_latency_us = initialization_end_us - initialization_start_us;
  TFLITE_LOG(INFO) << "Initialized session in " << startup_latency_us / 1e3
                   << "ms";
  LogMemUsageDelta("Init Phase", start_mem_usage, init_end_mem_usage);

  TF_LITE_ENSURE_STATUS(PrepareInputData());

  TfLiteStatus status = kTfLiteOk;
  uint64_t input_bytes = ComputeInputBytes();
  listeners_.OnBenchmarkStart(params_);
  Stat<int64_t> warmup_time_us =
      Run(params_.Get<int32_t>("warmup_runs"),
          params_.Get<float>("warmup_min_secs"), params_.Get<float>("max_secs"),
          WARMUP, &status);
  if (status != kTfLiteOk) {
    return status;
  }

  Stat<int64_t> inference_time_us =
      Run(params_.Get<int32_t>("num_runs"), params_.Get<float>("min_secs"),
          params_.Get<float>("max_secs"), REGULAR, &status);

  const auto after_run_mem_usage = profiling::memory::GetMemoryUsage();
  LogMemUsageDelta("Overall", start_mem_usage, after_run_mem_usage);

  listeners_.OnBenchmarkEnd(
      {startup_latency_us, input_bytes, warmup_time_us, inference_time_us});

  return status;
}

}  // namespace benchmark
}  // namespace tflite
