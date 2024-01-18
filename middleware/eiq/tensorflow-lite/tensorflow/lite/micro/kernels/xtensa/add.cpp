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
#include "tensorflow/lite/kernels/internal/reference/add.h"

#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/quantization_util.h"
#include "tensorflow/lite/kernels/internal/reference/integer_ops/add.h"
#include "tensorflow/lite/kernels/internal/reference/process_broadcast_shapes.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/kernels/op_macros.h"
#include "tensorflow/lite/micro/kernels/add.h"
#include "tensorflow/lite/micro/kernels/kernel_util.h"
#include "tensorflow/lite/micro/kernels/xtensa/xtensa.h"
#include "tensorflow/lite/micro/kernels/xtensa/xtensa_add.h"
#include "tensorflow/lite/micro/memory_helpers.h"
#include "tensorflow/lite/micro/micro_log.h"

namespace tflite {

TfLiteStatus Prepare(TfLiteContext* context, TfLiteNode* node) {
  TF_LITE_ENSURE_OK(context, AddPrepare(context, node));

#if defined(VISION_P6)
  TF_LITE_ENSURE_OK(context, AddPrepareVision(context, node));
#endif  // VISION_P6
  return kTfLiteOk;
}

TfLiteStatus EvalAdd(TfLiteContext* context, TfLiteNode* node,
                     TfLiteAddParams* params, const OpDataAdd* data,
                     const TfLiteEvalTensor* input1,
                     const TfLiteEvalTensor* input2, TfLiteEvalTensor* output) {
  switch (output->type) {
    case kTfLiteFloat32: {
      tflite::ArithmeticParams op_params;
      SetActivationParams(data->output_activation_min_f32,
                          data->output_activation_max_f32, &op_params);
      if (data->requires_broadcast) {
        reference_ops::BroadcastAdd4DSlow(
            op_params, tflite::micro::GetTensorShape(input1),
            tflite::micro::GetTensorData<float>(input1),
            tflite::micro::GetTensorShape(input2),
            tflite::micro::GetTensorData<float>(input2),
            tflite::micro::GetTensorShape(output),
            tflite::micro::GetTensorData<float>(output));
      } else {
        reference_ops::Add(op_params, tflite::micro::GetTensorShape(input1),
                           tflite::micro::GetTensorData<float>(input1),
                           tflite::micro::GetTensorShape(input2),
                           tflite::micro::GetTensorData<float>(input2),
                           tflite::micro::GetTensorShape(output),
                           tflite::micro::GetTensorData<float>(output));
      }
    } break;
    case kTfLiteInt32: {
      tflite::ArithmeticParams op_params;
      SetActivationParams(std::numeric_limits<int32_t>::lowest(),
                          std::numeric_limits<int32_t>::max(), &op_params);
      if (data->requires_broadcast) {
        reference_ops::BroadcastAdd4DSlow(
            op_params, tflite::micro::GetTensorShape(input1),
            tflite::micro::GetTensorData<int32_t>(input1),
            tflite::micro::GetTensorShape(input2),
            tflite::micro::GetTensorData<int32_t>(input2),
            tflite::micro::GetTensorShape(output),
            tflite::micro::GetTensorData<int32_t>(output));
      } else {
        reference_ops::Add(op_params, tflite::micro::GetTensorShape(input1),
                           tflite::micro::GetTensorData<int32_t>(input1),
                           tflite::micro::GetTensorShape(input2),
                           tflite::micro::GetTensorData<int32_t>(input2),
                           tflite::micro::GetTensorShape(output),
                           tflite::micro::GetTensorData<int32_t>(output));
      }
    } break;
    default:
      MicroPrintf("Type %s (%d) not supported.",
                  TfLiteTypeGetName(output->type), output->type);
      return kTfLiteError;
  }
  return kTfLiteOk;
}

TfLiteStatus EvalAddQuantized(TfLiteContext* context, TfLiteNode* node,
                              TfLiteAddParams* params, const OpDataAdd* data,
                              const TfLiteEvalTensor* input1,
                              const TfLiteEvalTensor* input2,
                              TfLiteEvalTensor* output) {
  tflite::ArithmeticParams op_params;
  op_params.left_shift = data->left_shift;
  op_params.input1_offset = data->input1_offset;
  op_params.input1_multiplier = data->input1_multiplier;
  op_params.input1_shift = data->input1_shift;
  op_params.input2_offset = data->input2_offset;
  op_params.input2_multiplier = data->input2_multiplier;
  op_params.input2_shift = data->input2_shift;
  op_params.output_offset = data->output_offset;
  op_params.output_multiplier = data->output_multiplier;
  op_params.output_shift = data->output_shift;
  SetActivationParams(data->output_activation_min, data->output_activation_max,
                      &op_params);
#if !(defined(HIFI4))
  bool need_broadcast = reference_ops::ProcessBroadcastShapes(
      tflite::micro::GetTensorShape(input1),
      tflite::micro::GetTensorShape(input2), &op_params);
#endif  // !defined(HIFI4)

  switch (output->type) {
    case kTfLiteInt8: {
#if defined(VISION_P6)
      const auto& op_data =
          *(reinterpret_cast<XtensaAddOpData*>(node->user_data));
      AddEvalQuantizedVision(context, node, *params, op_data, input1, input2,
                             output);
#elif defined(HIFI4)  // defined(VISION_P6)
      int err;
      const RuntimeShape extended_input1_shape =
          RuntimeShape::ExtendedShape(4, tflite::micro::GetTensorShape(input1));
      const RuntimeShape extended_input2_shape =
          RuntimeShape::ExtendedShape(4, tflite::micro::GetTensorShape(input2));
      const RuntimeShape extended_output_shape =
          RuntimeShape::ExtendedShape(4, tflite::micro::GetTensorShape(output));

      err = xa_nn_elm_add_broadcast_4D_asym8sxasym8s_asym8s(
          tflite::micro::GetTensorData<int8_t>(output),
          extended_output_shape.DimsData(), op_params.output_offset,
          op_params.output_shift, op_params.output_multiplier,
          op_params.quantized_activation_min,
          op_params.quantized_activation_max,
          tflite::micro::GetTensorData<int8_t>(input1),
          extended_input1_shape.DimsData(), op_params.input1_offset,
          op_params.input1_shift, op_params.input1_multiplier,
          tflite::micro::GetTensorData<int8_t>(input2),
          extended_input2_shape.DimsData(), op_params.input2_offset,
          op_params.input2_shift, op_params.input2_multiplier,
          op_params.left_shift);

      TF_LITE_ENSURE(context, err == 0);
#else                 // defined(VISION_P6)
      if (need_broadcast) {
        reference_integer_ops::BroadcastAdd4DSlow(
            op_params, tflite::micro::GetTensorShape(input1),
            tflite::micro::GetTensorData<int8_t>(input1),
            tflite::micro::GetTensorShape(input2),
            tflite::micro::GetTensorData<int8_t>(input2),
            tflite::micro::GetTensorShape(output),
            tflite::micro::GetTensorData<int8_t>(output));
      } else {
        reference_integer_ops::Add(
            op_params, tflite::micro::GetTensorShape(input1),
            tflite::micro::GetTensorData<int8_t>(input1),
            tflite::micro::GetTensorShape(input2),
            tflite::micro::GetTensorData<int8_t>(input2),
            tflite::micro::GetTensorShape(output),
            tflite::micro::GetTensorData<int8_t>(output));
      }
#endif                // defined(VISION_P6)
      break;
    }
    case kTfLiteInt16: {
#if defined(HIFI4)
      int err;
      const RuntimeShape extended_input1_shape =
          RuntimeShape::ExtendedShape(4, tflite::micro::GetTensorShape(input1));
      const RuntimeShape extended_input2_shape =
          RuntimeShape::ExtendedShape(4, tflite::micro::GetTensorShape(input2));
      const RuntimeShape extended_output_shape =
          RuntimeShape::ExtendedShape(4, tflite::micro::GetTensorShape(output));

      err = xa_nn_elm_add_broadcast_4D_asym16sxasym16s_asym16s(
          tflite::micro::GetTensorData<int16_t>(output),
          extended_output_shape.DimsData(), op_params.output_offset,
          op_params.output_shift, op_params.output_multiplier,
          op_params.quantized_activation_min,
          op_params.quantized_activation_max,
          tflite::micro::GetTensorData<int16_t>(input1),
          extended_input1_shape.DimsData(), op_params.input1_offset,
          op_params.input1_shift, op_params.input1_multiplier,
          tflite::micro::GetTensorData<int16_t>(input2),
          extended_input2_shape.DimsData(), op_params.input2_offset,
          op_params.input2_shift, op_params.input2_multiplier,
          op_params.left_shift);

      TF_LITE_ENSURE(context, err == 0);
#else   // defined(HIFI4)
      if (need_broadcast) {
        reference_ops::BroadcastAdd4DSlow(
            op_params, tflite::micro::GetTensorShape(input1),
            tflite::micro::GetTensorData<int16_t>(input1),
            tflite::micro::GetTensorShape(input2),
            tflite::micro::GetTensorData<int16_t>(input2),
            tflite::micro::GetTensorShape(output),
            tflite::micro::GetTensorData<int16_t>(output));
      } else {
        reference_ops::Add(op_params, tflite::micro::GetTensorShape(input1),
                           tflite::micro::GetTensorData<int16_t>(input1),
                           tflite::micro::GetTensorShape(input2),
                           tflite::micro::GetTensorData<int16_t>(input2),
                           tflite::micro::GetTensorShape(output),
                           tflite::micro::GetTensorData<int16_t>(output),
                           false);
      }
#endif  // defined(HIFI4)
      break;
    }
    default:
      MicroPrintf("Type %s (%d) not supported.",
                  TfLiteTypeGetName(output->type), output->type);
      return kTfLiteError;
  }

  return kTfLiteOk;
}

void* AddInit(TfLiteContext* context, const char* buffer, size_t length) {
  TFLITE_DCHECK(context->AllocatePersistentBuffer != nullptr);
  void* data;
#if defined(VISION_P6)
  data = context->AllocatePersistentBuffer(context, sizeof(XtensaAddOpData));
  if (InitXtensaContext()) {
    return nullptr;
  }
#else
  data = context->AllocatePersistentBuffer(context, sizeof(OpDataAdd));
#endif  // defined(VISION_P6)
  return data;
}

TfLiteStatus AddEval(TfLiteContext* context, TfLiteNode* node) {
  auto* params = reinterpret_cast<TfLiteAddParams*>(node->builtin_data);

  TFLITE_DCHECK(node->user_data != nullptr);
  const OpDataAdd* data = static_cast<const OpDataAdd*>(node->user_data);

  const TfLiteEvalTensor* input1 =
      tflite::micro::GetEvalInput(context, node, kAddInputTensor1);
  const TfLiteEvalTensor* input2 =
      tflite::micro::GetEvalInput(context, node, kAddInputTensor2);
  TfLiteEvalTensor* output =
      tflite::micro::GetEvalOutput(context, node, kAddOutputTensor);

  if (output->type == kTfLiteFloat32 || output->type == kTfLiteInt32) {
    TF_LITE_ENSURE_OK(
        context, EvalAdd(context, node, params, data, input1, input2, output));
  } else if (output->type == kTfLiteInt8 || output->type == kTfLiteInt16) {
    TF_LITE_ENSURE_OK(context, EvalAddQuantized(context, node, params, data,
                                                input1, input2, output));
  } else {
    MicroPrintf("Type %s (%d) not supported.", TfLiteTypeGetName(output->type),
                output->type);
    return kTfLiteError;
  }

  return kTfLiteOk;
}

TFLMRegistration Register_ADD() {
  return tflite::micro::RegisterOp(AddInit, Prepare, AddEval);
}

}  // namespace tflite
