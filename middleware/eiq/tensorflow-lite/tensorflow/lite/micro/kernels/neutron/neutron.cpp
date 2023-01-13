/* Copyright 2022 The TensorFlow Authors. All Rights Reserved.

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

#include "flatbuffers/flexbuffers.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/kernels/kernel_util.h"
#include "tensorflow/lite/micro/micro_context.h"

extern "C" {
#include "neutron-firmware.h"
}

namespace tflite {
namespace {

struct NeutronConfig {
  int buffer_idx;
};

#define MAX_BASE_ADDR_NUM 6

void* Init(TfLiteContext* context, const char* buffer, size_t length) {
  TFLITE_DCHECK(context->AllocatePersistentBuffer != nullptr);

  neutronFwInit();

  return context->AllocatePersistentBuffer(context, sizeof(NeutronConfig));
}

TfLiteStatus Prepare(TfLiteContext* context, TfLiteNode* node) {
  TFLITE_DCHECK(context != nullptr);
  TF_LITE_ENSURE(context, node->inputs->size > 0);
  TFLITE_DCHECK(node->user_data != nullptr);
  TF_LITE_ENSURE(context, node->custom_initial_data_size > 0);

  NeutronConfig* neutron_data = static_cast<NeutronConfig*>(node->user_data);
  MicroContext* micro_context = GetMicroContext(context);
  TfLiteTensor* microcode = micro_context->AllocateTempInputTensor(node, node->inputs->size - 2);
  TF_LITE_ENSURE_STATUS(context->RequestScratchBufferInArena(
      context, microcode->bytes, &neutron_data->buffer_idx));
  micro_context->DeallocateTempTfLiteTensor(microcode);

  return kTfLiteOk;
}

TfLiteStatus Eval(TfLiteContext* context, TfLiteNode* node) {
  TFLITE_DCHECK(node->user_data != nullptr);
  TFLITE_DCHECK(context != nullptr);
  TFLITE_DCHECK(context->GetScratchBuffer != nullptr);

  void* base_addrs[MAX_BASE_ADDR_NUM] = {nullptr};
  int base_addr_num = 0;

  for (int i = 0; i < node->inputs->size - 2; i++) {
    TfLiteEvalTensor* input = context->GetEvalTensor(context, node->inputs->data[i]);
    base_addrs[base_addr_num++] = input->data.data;
  }

  TfLiteEvalTensor* microcode = context->GetEvalTensor(context, node->inputs->data[node->inputs->size - 2]);
  TfLiteEvalTensor* weights = context->GetEvalTensor(context, node->inputs->data[node->inputs->size - 1]);
  base_addrs[base_addr_num++] = weights->data.data;

  for (int i = 0; i < node->outputs->size - 1; i++) {
    TfLiteEvalTensor* output = context->GetEvalTensor(context, node->outputs->data[i]);
    base_addrs[base_addr_num++] = output->data.data;
  }

  TfLiteEvalTensor* scratch = context->GetEvalTensor(context, node->outputs->data[node->outputs->size - 1]);
  base_addrs[base_addr_num++] = scratch->data.data;

  NeutronConfig* neutron_data = static_cast<NeutronConfig*>(node->user_data);
  void* microcode_data = context->GetScratchBuffer(context, neutron_data->buffer_idx);
  const RuntimeShape& microcode_shape = tflite::micro::GetTensorShape(microcode);
  memcpy(microcode_data, microcode->data.data, microcode_shape.FlatSize());

  neutronFwInterpreter(microcode_data, base_addrs, base_addr_num);

  return kTfLiteOk;
}

}  // namespace

TfLiteRegistration* Register_NEUTRON_GRAPH() {
  static TfLiteRegistration r = {
        /*init=*/Init,
        /*free=*/nullptr,
        /*prepare=*/Prepare,
        /*invoke=*/Eval,
        /*profiling_string=*/nullptr,
        /*builtin_code=*/0,
        /*custom_name=*/nullptr,
        /*version=*/0};
  return &r;
}

const char* GetString_NEUTRON_GRAPH() { return "NeutronGraph"; }

}  // namespace tflite
