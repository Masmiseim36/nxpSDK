/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

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

#include "tensorflow/lite/kernels/internal/reference/softmax.h"

#include "cmsis/CMSIS/NN/Include/arm_nnfunctions.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/micro/kernels/kernel_util.h"

namespace tflite {
namespace {

TfLiteStatus CalculateSoftmaxParams(TfLiteContext* context,
                                    const TfLiteTensor* input,
                                    TfLiteTensor* output,
                                    const TfLiteSoftmaxParams* params,
                                    SoftmaxParams* op_data) {
  if (input->type == kTfLiteUInt8 || input->type == kTfLiteInt8) {
    if (input->type == kTfLiteUInt8) {
      TF_LITE_ENSURE_TYPES_EQ(context, output->type, kTfLiteUInt8);
      TF_LITE_ENSURE_EQ(context, output->params.zero_point, 0);
    } else {
      TF_LITE_ENSURE_TYPES_EQ(context, input->type, kTfLiteInt8);
      if (output->type == kTfLiteInt16) {
        TF_LITE_ENSURE_EQ(context, output->params.zero_point, -32768);
        // NOTE: Current int16_t softmax output does not require symmetric
        // scaling
        // - so no need to verify scale here.
      } else {
        TF_LITE_ENSURE_TYPES_EQ(context, output->type, kTfLiteInt8);
        TF_LITE_ENSURE_EQ(context, output->params.zero_point, -128);
        TF_LITE_ENSURE(context, output->params.scale == 1.f / 256);
      }
    }

    static const int kScaledDiffIntegerBits = 5;

    int input_left_shift;
    tflite::PreprocessSoftmaxScaling(
        static_cast<double>(params->beta),
        static_cast<double>(input->params.scale), kScaledDiffIntegerBits,
        &op_data->input_multiplier, &input_left_shift);
    op_data->input_left_shift = input_left_shift;
    op_data->diff_min =
        -1.0 * tflite::CalculateInputRadius(kScaledDiffIntegerBits,
                                            op_data->input_left_shift);
  } else {
    TF_LITE_ENSURE_TYPES_EQ(context, input->type, kTfLiteFloat32);
    TF_LITE_ENSURE_TYPES_EQ(context, output->type, kTfLiteFloat32);
    op_data->beta = static_cast<double>(params->beta);
  }
  return kTfLiteOk;
}

void* SoftmaxInit(TfLiteContext* context, const char* buffer, size_t length) {
  TFLITE_DCHECK(context->AllocatePersistentBuffer != nullptr);
  return context->AllocatePersistentBuffer(context, sizeof(SoftmaxParams));
}

TfLiteStatus SoftmaxPrepare(TfLiteContext* context, TfLiteNode* node) {
  auto* params = static_cast<TfLiteSoftmaxParams*>(node->builtin_data);

  TF_LITE_ENSURE_EQ(context, NumInputs(node), 1);
  TF_LITE_ENSURE_EQ(context, NumOutputs(node), 1);
  const TfLiteTensor* input = GetInput(context, node, 0);
  TF_LITE_ENSURE(context, NumDimensions(input) >= 1);

  TfLiteTensor* output = GetOutput(context, node, 0);

  TFLITE_DCHECK(node->user_data != nullptr);
  SoftmaxParams* data = static_cast<SoftmaxParams*>(node->user_data);
  return CalculateSoftmaxParams(context, input, output, params, data);
}

// Takes a tensor and performs softmax along the last dimension.
void SoftmaxFloat(const TfLiteEvalTensor* input, TfLiteEvalTensor* output,
                  const SoftmaxParams& op_data) {
  tflite::reference_ops::Softmax(op_data, tflite::micro::GetTensorShape(input),
                                 tflite::micro::GetTensorData<float>(input),
                                 tflite::micro::GetTensorShape(output),
                                 tflite::micro::GetTensorData<float>(output));
}

void SoftmaxQuantized(const TfLiteEvalTensor* input, TfLiteEvalTensor* output,
                      const SoftmaxParams& op_data) {
  const auto input_shape = tflite::micro::GetTensorShape(input);
  const auto output_shape = tflite::micro::GetTensorShape(output);

  if (input->type == kTfLiteUInt8) {
    tflite::reference_ops::Softmax(
        op_data, tflite::micro::GetTensorShape(input),
        tflite::micro::GetTensorData<uint8_t>(input),
        tflite::micro::GetTensorShape(output),
        tflite::micro::GetTensorData<uint8_t>(output));
  } else {
    if (output->type == kTfLiteInt16) {
      tflite::reference_ops::Softmax(
          op_data, tflite::micro::GetTensorShape(input),
          tflite::micro::GetTensorData<int8_t>(input),
          tflite::micro::GetTensorShape(output),
          tflite::micro::GetTensorData<int16_t>(output));
    } else {
      const int trailing_dim = input_shape.DimensionsCount() - 1;
      const int outer_size =
          MatchingFlatSizeSkipDim(input_shape, trailing_dim, output_shape);
      const int depth =
          MatchingDim(input_shape, trailing_dim, output_shape, trailing_dim);

      arm_softmax_s8(tflite::micro::GetTensorData<int8_t>(input), outer_size,
                     depth, op_data.input_multiplier, op_data.input_left_shift,
                     op_data.diff_min,
                     tflite::micro::GetTensorData<int8_t>(output));
    }
  }
}

TfLiteStatus SoftmaxEval(TfLiteContext* context, TfLiteNode* node) {
  const TfLiteEvalTensor* input = tflite::micro::GetEvalInput(context, node, 0);
  TfLiteEvalTensor* output = tflite::micro::GetEvalOutput(context, node, 0);

  TFLITE_DCHECK(node->user_data != nullptr);
  const SoftmaxParams& data =
      *(static_cast<const SoftmaxParams*>(node->user_data));

  switch (input->type) {
    case kTfLiteFloat32: {
      SoftmaxFloat(input, output, data);
      return kTfLiteOk;
    }
    case kTfLiteInt8:
    case kTfLiteUInt8: {
      SoftmaxQuantized(input, output, data);
      return kTfLiteOk;
    }
    default:
      TF_LITE_KERNEL_LOG(context, "Type %s (%d) not supported.",
                         TfLiteTypeGetName(input->type), input->type);
      return kTfLiteError;
  }
}

}  // namespace

TfLiteRegistration Register_SOFTMAX() {
  return {/*init=*/SoftmaxInit,
          /*free=*/nullptr,
          /*prepare=*/SoftmaxPrepare,
          /*invoke=*/SoftmaxEval,
          /*profiling_string=*/nullptr,
          /*builtin_code=*/0,
          /*custom_name=*/nullptr,
          /*version=*/0};
}

}  // namespace tflite
