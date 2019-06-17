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
#include "tensorflow/contrib/lite/kernels/eigen_support.h"

#include <utility>

#include "third_party/eigen3/Eigen/Core"
#include "tensorflow/contrib/lite/arena_planner.h"
#ifndef TFLITE_MCU
#include "tensorflow/contrib/lite/kernels/internal/optimized/eigen_spatial_convolutions.h"
#endif
#include "tensorflow/contrib/lite/kernels/op_macros.h"

namespace tflite {
namespace eigen_support {
namespace {

#ifndef EIGEN_DONT_ALIGN
// Eigen may require buffers to be algiend to 16, 32 or 64 bytes depending on
// hardware architecture and build configurations.
// If the static assertion fails, try to increase `kDefaultTensorAlignment` to
// in `arena_planner.h` to 32 or 64.
static_assert(
    kDefaultTensorAlignment % EIGEN_MAX_ALIGN_BYTES == 0,
    "kDefaultArenaAlignment doesn't comply with Eigen alignment requirement.");
#endif  // EIGEN_DONT_ALIGN

#ifndef TFLITE_MCU
// We have a single global threadpool for all convolution operations. This means
// that inferences started from different threads may block each other, but
// since the underlying resource of CPU cores should be consumed by the
// operations anyway, it shouldn't affect overall performance.
class EigenThreadPoolWrapper : public Eigen::ThreadPoolInterface {
 public:
  // Takes ownership of 'pool'
  explicit EigenThreadPoolWrapper(Eigen::ThreadPool* pool) : pool_(pool) {}
  ~EigenThreadPoolWrapper() override {}

  void Schedule(std::function<void()> fn) override {
    pool_->Schedule(std::move(fn));
  }
  int NumThreads() const override { return pool_->NumThreads(); }
  int CurrentThreadId() const override { return pool_->CurrentThreadId(); }

 private:
  std::unique_ptr<Eigen::ThreadPool> pool_;
};
#endif

struct RefCountedEigenContext : public TfLiteExternalContext {
#ifndef TFLITE_MCU
  std::unique_ptr<Eigen::ThreadPoolInterface> thread_pool_wrapper;
  std::unique_ptr<Eigen::ThreadPoolDevice> device;
#endif
  int num_references = 0;
};

RefCountedEigenContext* GetEigenContext(TfLiteContext* context) {
  return reinterpret_cast<RefCountedEigenContext*>(
      context->GetExternalContext(context, kTfLiteEigenContext));
}

void InitDevice(TfLiteContext* context, RefCountedEigenContext* ptr) {
#ifndef TFLITE_MCU
  int num_threads = 4;
  if (context->recommended_num_threads != -1) {
    num_threads = context->recommended_num_threads;
  }
  ptr->device.reset();  // destroy before we invalidate the thread pool
  ptr->thread_pool_wrapper.reset(
      new EigenThreadPoolWrapper(new Eigen::ThreadPool(num_threads)));
  ptr->device.reset(
      new Eigen::ThreadPoolDevice(ptr->thread_pool_wrapper.get(), num_threads));
#endif
}

TfLiteStatus Refresh(TfLiteContext* context) {
#ifndef TFLITE_MCU
  Eigen::setNbThreads(context->recommended_num_threads);
#endif

  auto* ptr = GetEigenContext(context);
  if (ptr != nullptr) {
    InitDevice(context, ptr);
  }

  return kTfLiteOk;
}

}  // namespace

void IncrementUsageCounter(TfLiteContext* context) {
  auto* ptr = GetEigenContext(context);
  if (ptr == nullptr) {
#ifndef TFLITE_MCU
    if (context->recommended_num_threads != -1) {
      Eigen::setNbThreads(context->recommended_num_threads);
    }
#endif
    ptr = new RefCountedEigenContext;
    ptr->type = kTfLiteEigenContext;
    ptr->Refresh = Refresh;
    ptr->num_references = 0;
    InitDevice(context, ptr);
    context->SetExternalContext(context, kTfLiteEigenContext, ptr);
  }
  ptr->num_references++;
}

void DecrementUsageCounter(TfLiteContext* context) {
  auto* ptr = GetEigenContext(context);
  if (ptr == nullptr) {
    TF_LITE_FATAL(
        "Call to DecrementUsageCounter() not preceded by "
        "IncrementUsageCounter()");
  }
  if (--ptr->num_references == 0) {
    delete ptr;
    context->SetExternalContext(context, kTfLiteEigenContext, nullptr);
  }
}

#ifndef TFLITE_MCU
const Eigen::ThreadPoolDevice* GetThreadPoolDevice(TfLiteContext* context) {
  auto* ptr = GetEigenContext(context);
  if (ptr == nullptr) {
    TF_LITE_FATAL(
        "Call to GetFromContext() not preceded by IncrementUsageCounter()");
  }
  return ptr->device.get();
}
#endif

}  // namespace eigen_support
}  // namespace tflite
