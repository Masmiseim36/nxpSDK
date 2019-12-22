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

/* File modified by NXP. Changes are described in file
   /middleware/eiq/tensorflow-lite/readme.txt in section "Release notes" */

#include "tensorflow/lite/kernels/gemmlowp_support.h"

#include <memory>

#include "tensorflow/lite/kernels/op_macros.h"

namespace tflite {
namespace gemmlowp_support {
namespace {

struct RefCountedGemmlowpContext : public TfLiteExternalContext {
  std::unique_ptr<gemmlowp::GemmContext> gemmlowp_context;
  int num_references = 0;
};

RefCountedGemmlowpContext* GetGemmLowpContext(TfLiteContext* context) {
  return reinterpret_cast<RefCountedGemmlowpContext*>(
      context->GetExternalContext(context, kTfLiteGemmLowpContext));
}

TfLiteStatus Refresh(TfLiteContext* context) {
#ifndef TFLITE_MCU
  auto* ptr = GetGemmLowpContext(context);
  if (ptr != nullptr) {
    ptr->gemmlowp_context->set_max_num_threads(
        context->recommended_num_threads);
  }
#endif
  return kTfLiteOk;
}

}  // namespace

void IncrementUsageCounter(TfLiteContext* context) {
  auto* ptr = GetGemmLowpContext(context);
  if (ptr == nullptr) {
    ptr = new RefCountedGemmlowpContext;
    ptr->type = kTfLiteGemmLowpContext;
    ptr->Refresh = Refresh;
    ptr->gemmlowp_context.reset(new gemmlowp::GemmContext());
#ifndef TFLITE_MCU
    if (context->recommended_num_threads != -1) {
      ptr->gemmlowp_context->set_max_num_threads(
          context->recommended_num_threads);
    }
#endif
    ptr->num_references = 0;
    context->SetExternalContext(context, kTfLiteGemmLowpContext, ptr);
  }
  ptr->num_references++;
}

void DecrementUsageCounter(TfLiteContext* context) {
  auto* ptr = GetGemmLowpContext(context);
  if (ptr == nullptr) {
    TF_LITE_FATAL(
        "Call to DecrementUsageCounter() not preceded by "
        "IncrementUsageCounter()");
  }
  if (--ptr->num_references == 0) {
    delete ptr;
    context->SetExternalContext(context, kTfLiteGemmLowpContext, nullptr);
  }
}

gemmlowp::GemmContext* GetFromContext(TfLiteContext* context) {
  auto* ptr = GetGemmLowpContext(context);
  if (ptr == nullptr) {
    TF_LITE_FATAL(
        "Call to GetFromContext() not preceded by IncrementUsageCounter()");
  }
  return ptr->gemmlowp_context.get();
}

}  // namespace gemmlowp_support
}  // namespace tflite
