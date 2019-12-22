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

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <limits>

#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/c/c_api_internal.h"
#include "tensorflow/lite/kernels/internal/optimized/integer_ops/softmax.h"
#include "tensorflow/lite/kernels/internal/optimized/optimized_ops.h"
#include "tensorflow/lite/kernels/internal/quantization_util.h"
#include "tensorflow/lite/kernels/internal/reference/integer_ops/log_softmax.h"
#include "tensorflow/lite/kernels/internal/reference/integer_ops/logistic.h"
#include "tensorflow/lite/kernels/internal/reference/integer_ops/tanh.h"
#include "tensorflow/lite/kernels/internal/reference/reference_ops.h"
#include "tensorflow/lite/kernels/internal/tensor.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/kernels/op_macros.h"

namespace tflite {
namespace ops {
namespace builtin {
namespace activations {

enum KernelType {
  kReference,
  kGenericOptimized,
};

struct OpData {
  int32_t input_multiplier = 0;
  int input_left_shift = 0;
  int32_t input_range_radius = 0;
  int diff_min = 0;
};

struct LogSoftmaxOpData : public OpData {
  int32_t reverse_scaling_divisor = 0;
  int32_t reverse_scaling_right_shift = 0;
};

struct LeakyReluOpData : public OpData {
  uint8_t q_alpha;
  int32_t output_multiplier = 0;
  int output_shift = 0;
};

struct PreluOpData : public OpData {
  int32_t output_multiplier = 0;
  int output_shift = 0;
};

namespace {
TfLiteStatus CheckOutputQuantParams(TfLiteContext* context,
                                    const TfLiteTensor* input,
                                    const TfLiteTensor* output) {
  TF_LITE_ENSURE(context, output->params.scale == 1. / 256);
  if (input->type == kTfLiteUInt8) {
    TF_LITE_ENSURE_EQ(context, output->params.zero_point, 0);
  } else {
    TF_LITE_ENSURE_EQ(context, output->params.zero_point, -128);
  }
  return kTfLiteOk;
}
}  // namespace

void* Init(TfLiteContext* context, const char* buffer, size_t length) {
  // This is a builtin op, so we don't use the contents in 'buffer', if any.
  // Instead, we allocate a new object to carry information from Prepare() to
  // Eval().
  return new OpData;
}

void* LogSoftmaxInit(TfLiteContext* context, const char* buffer,
                     size_t length) {
  return new LogSoftmaxOpData;
}

void* PreluInit(TfLiteContext* context, const char* buffer, size_t length) {
  return new PreluOpData;
}

void Free(TfLiteContext* context, void* buffer) {
  delete reinterpret_cast<OpData*>(buffer);
}

void LogSoftmaxFree(TfLiteContext* context, void* buffer) {
  delete reinterpret_cast<LogSoftmaxOpData*>(buffer);
}

void PreluFree(TfLiteContext* context, void* buffer) {
  delete reinterpret_cast<PreluOpData*>(buffer);
}

TfLiteStatus GenericPrepare(TfLiteContext* context, TfLiteNode* node) {
  TF_LITE_ENSURE_EQ(context, NumInputs(node), 1);
  TF_LITE_ENSURE_EQ(context, NumOutputs(node), 1);
  const TfLiteTensor* input = GetInput(context, node, 0);
  TfLiteTensor* output = GetOutput(context, node, 0);
  TF_LITE_ENSURE_EQ(context, input->type, output->type);

  return context->ResizeTensor(context, output,
                               TfLiteIntArrayCopy(input->dims));
}

void* LeakyReluInit(TfLiteContext* context, const char* buffer, size_t length) {
  return new LeakyReluOpData;
}

void LeakyReluFree(TfLiteContext* context, void* buffer) {
  delete reinterpret_cast<LeakyReluOpData*>(buffer);
}

TfLiteStatus LeakyReluPrepare(TfLiteContext* context, TfLiteNode* node) {
  TF_LITE_ENSURE_EQ(context, NumInputs(node), 1);
  TF_LITE_ENSURE_EQ(context, NumOutputs(node), 1);
  const TfLiteTensor* input = GetInput(context, node, 0);
  TfLiteTensor* output = GetOutput(context, node, 0);
  TF_LITE_ENSURE_EQ(context, input->type, output->type);

  LeakyReluOpData* data = reinterpret_cast<LeakyReluOpData*>(node->user_data);

  if (output->type == kTfLiteUInt8) {
    const auto* params =
        reinterpret_cast<TfLiteLeakyReluParams*>(node->builtin_data);
    // Quantize the alpha with same zero-point and scale as of input
    data->q_alpha = static_cast<uint8_t>(std::max<float>(
        std::numeric_limits<uint8_t>::min(),
        std::min<float>(std::numeric_limits<uint8_t>::max(),
                        std::round(input->params.zero_point +
                                   (params->alpha / input->params.scale)))));

    double real_multiplier =
        input->params.scale * input->params.scale / output->params.scale;
    QuantizeMultiplierSmallerThanOneExp(
        real_multiplier, &data->output_multiplier, &data->output_shift);
  }
  return context->ResizeTensor(context, output,
                               TfLiteIntArrayCopy(input->dims));
}

TfLiteStatus TanhPrepare(TfLiteContext* context, TfLiteNode* node) {
  OpData* data = reinterpret_cast<OpData*>(node->user_data);

  TF_LITE_ENSURE_EQ(context, NumInputs(node), 1);
  TF_LITE_ENSURE_EQ(context, NumOutputs(node), 1);
  const TfLiteTensor* input = GetInput(context, node, 0);
  TfLiteTensor* output = GetOutput(context, node, 0);
  TF_LITE_ENSURE_EQ(context, input->type, output->type);

  if (input->type == kTfLiteUInt8 || input->type == kTfLiteInt8) {
    static constexpr int kInputIntegerBits = 4;

    const double input_real_multiplier =
        input->params.scale *
        static_cast<double>(1 << (31 - kInputIntegerBits));

    QuantizeMultiplierGreaterThanOne(input_real_multiplier,
                                     &data->input_multiplier,
                                     &data->input_left_shift);
    data->input_range_radius =
        CalculateInputRadius(kInputIntegerBits, data->input_left_shift);
  } else if (input->type == kTfLiteInt16) {
    static constexpr int kInputIntegerBits = 3;
    static constexpr int kOutputFractionalBits = 15;

    // These operators are implemented in fixed-point arithmetic,
    // which intrinsically wants symmetric ranges (zero_point==0)
    // and power-of-two scales (power-of-two is abbreviated below as POT).
    // While more general support would be possible by means of rescaling,
    // that would add some overhead and some loss of accuracy and wouldn't
    // be used at the moment as current quantized LSTM applications are
    // happy with symmetric, power-of-two-scales quantization. So we just
    // implement that narrow case only for now.

    TF_LITE_ENSURE_EQ(context, input->params.zero_point, 0);
    TF_LITE_ENSURE_EQ(context, output->params.zero_point, 0);

    int input_scale_log2_rounded;
    TF_LITE_ENSURE(context,
                   CheckedLog2(input->params.scale, &input_scale_log2_rounded));

    int output_scale_log2_rounded;
    TF_LITE_ENSURE(
        context, CheckedLog2(output->params.scale, &output_scale_log2_rounded));
    TF_LITE_ENSURE_EQ(context, output_scale_log2_rounded,
                      -kOutputFractionalBits);

    data->input_left_shift =
        (15 - kInputIntegerBits) + input_scale_log2_rounded;
    // Support for shifts is limited until we have a parameterized version of
    // SaturatingRoundingMultiplyByPOT().
    TF_LITE_ENSURE(context, data->input_left_shift >= 0);
    TF_LITE_ENSURE(context, data->input_left_shift <= 1);
  }

  return context->ResizeTensor(context, output,
                               TfLiteIntArrayCopy(input->dims));
}

TfLiteStatus SigmoidPrepare(TfLiteContext* context, TfLiteNode* node) {
  OpData* data = reinterpret_cast<OpData*>(node->user_data);

  TF_LITE_ENSURE_EQ(context, NumInputs(node), 1);
  TF_LITE_ENSURE_EQ(context, NumOutputs(node), 1);
  const TfLiteTensor* input = GetInput(context, node, 0);
  TfLiteTensor* output = GetOutput(context, node, 0);
  TF_LITE_ENSURE_EQ(context, input->type, output->type);

  if (input->type == kTfLiteUInt8 || input->type == kTfLiteInt8) {
    if (input->type == kTfLiteUInt8) {
      TF_LITE_ENSURE_EQ(context, output->params.zero_point,
                        std::numeric_limits<uint8_t>::min());
    }
    if (input->type == kTfLiteInt8) {
      TF_LITE_ENSURE_EQ(context, output->params.zero_point,
                        std::numeric_limits<int8_t>::min());
    }
    TF_LITE_ENSURE(context, output->params.scale == 1. / 256);

    static constexpr int kInputIntegerBits = 4;

    const double input_real_multiplier =
        input->params.scale *
        static_cast<double>(1 << (31 - kInputIntegerBits));

    QuantizeMultiplierGreaterThanOne(input_real_multiplier,
                                     &data->input_multiplier,
                                     &data->input_left_shift);
    data->input_range_radius =
        CalculateInputRadius(kInputIntegerBits, data->input_left_shift);
  } else if (input->type == kTfLiteInt16) {
    static constexpr int kInputIntegerBits = 3;
    static constexpr int kOutputFractionalBits = 15;

    // See comments in TanhPrepare about requiring zero_point==0
    // and a power-of-two ("POT") scale.

    TF_LITE_ENSURE_EQ(context, input->params.zero_point, 0);
    TF_LITE_ENSURE_EQ(context, output->params.zero_point, 0);

    int input_scale_log2_rounded;
    TF_LITE_ENSURE(context,
                   CheckedLog2(input->params.scale, &input_scale_log2_rounded));

    int output_scale_log2_rounded;
    TF_LITE_ENSURE(
        context, CheckedLog2(output->params.scale, &output_scale_log2_rounded));
    TF_LITE_ENSURE_EQ(context, output_scale_log2_rounded,
                      -kOutputFractionalBits);

    data->input_left_shift =
        (15 - kInputIntegerBits) + input_scale_log2_rounded;
    // The int16 logistic implementation does not support shifting of the input.
    TF_LITE_ENSURE_EQ(context, data->input_left_shift, 0);
  }

  return context->ResizeTensor(context, output,
                               TfLiteIntArrayCopy(input->dims));
}

TfLiteStatus SoftmaxPrepare(TfLiteContext* context, TfLiteNode* node) {
  auto* params = reinterpret_cast<TfLiteSoftmaxParams*>(node->builtin_data);
  OpData* data = reinterpret_cast<OpData*>(node->user_data);

  TF_LITE_ENSURE_EQ(context, NumInputs(node), 1);
  TF_LITE_ENSURE_EQ(context, NumOutputs(node), 1);
  const TfLiteTensor* input = GetInput(context, node, 0);
  TfLiteTensor* output = GetOutput(context, node, 0);
  TF_LITE_ENSURE_EQ(context, input->type, output->type);

  const int num_dims = NumDimensions(input);
  TF_LITE_ENSURE(context, num_dims >= 1 && num_dims <= 4);

  if (input->type == kTfLiteUInt8 || input->type == kTfLiteInt8) {
    if (CheckOutputQuantParams(context, input, output) == kTfLiteError) {
      return kTfLiteError;
    }

    static const int kScaledDiffIntegerBits = 5;
    tflite::PreprocessSoftmaxScaling(
        params->beta, input->params.scale, kScaledDiffIntegerBits,
        &data->input_multiplier, &data->input_left_shift);
    data->diff_min = -1.0 * tflite::CalculateInputRadius(
                                kScaledDiffIntegerBits, data->input_left_shift);
  }

  return context->ResizeTensor(context, output,
                               TfLiteIntArrayCopy(input->dims));
}

TfLiteStatus LogSoftmaxPrepare(TfLiteContext* context, TfLiteNode* node) {
  LogSoftmaxOpData* data = reinterpret_cast<LogSoftmaxOpData*>(node->user_data);

  TF_LITE_ENSURE_EQ(context, NumInputs(node), 1);
  TF_LITE_ENSURE_EQ(context, NumOutputs(node), 1);
  const TfLiteTensor* input = GetInput(context, node, 0);
  TfLiteTensor* output = GetOutput(context, node, 0);
  TF_LITE_ENSURE_EQ(context, input->type, output->type);

  if (input->type == kTfLiteUInt8 || input->type == kTfLiteInt8) {
    if (input->type == kTfLiteUInt8) {
      TF_LITE_ENSURE_EQ(context, output->params.zero_point, 255);
    }
    if (input->type == kTfLiteInt8) {
      TF_LITE_ENSURE_EQ(context, output->params.zero_point, 127);
    }
    TF_LITE_ENSURE_EQ(context, output->params.scale, 16.0 / 256);

    static const double kBeta = 1.0;
    static const int kScaledDiffIntegerBits = 5;
    tflite::PreprocessLogSoftmaxScalingExp(
        kBeta, input->params.scale, kScaledDiffIntegerBits,
        &data->input_multiplier, &data->input_left_shift,
        &data->reverse_scaling_divisor, (int*)&data->reverse_scaling_right_shift);
    data->reverse_scaling_right_shift *= -1;
    data->diff_min = -1.0 * tflite::CalculateInputRadius(
                                kScaledDiffIntegerBits, data->input_left_shift);
  }

  return context->ResizeTensor(context, output,
                               TfLiteIntArrayCopy(input->dims));
}

TfLiteStatus PreluPrepare(TfLiteContext* context, TfLiteNode* node) {
  TF_LITE_ENSURE_EQ(context, NumInputs(node), 2);
  TF_LITE_ENSURE_EQ(context, NumOutputs(node), 1);
  const TfLiteTensor* input = GetInput(context, node, 0);
  TfLiteTensor* output = GetOutput(context, node, 0);
  const TfLiteTensor* alpha = GetInput(context, node, 1);
  PreluOpData* data = reinterpret_cast<PreluOpData*>(node->user_data);

  TF_LITE_ENSURE_EQ(context, input->type, alpha->type);
  output->type = input->type;

  if (output->type == kTfLiteUInt8 || output->type == kTfLiteInt16) {
    double real_multiplier =
        input->params.scale * alpha->params.scale / output->params.scale;
    QuantizeMultiplierSmallerThanOneExp(
        real_multiplier, &data->output_multiplier, &data->output_shift);
  }

  // PRelu (parameteric Relu) shares the same alpha value on "shared axis".
  // This means it's always required to "broadcast" alpha values in PRelu.
  TfLiteIntArray* output_size = nullptr;
  TF_LITE_ENSURE_OK(
      context, CalculateShapeForBroadcast(context, input, alpha, &output_size));

  TF_LITE_ENSURE_OK(context,
                    context->ResizeTensor(context, output, output_size));
  // After broadcasting, the output shape should always be the same as the
  // input shape.
  TF_LITE_ENSURE(context, HaveSameShapes(input, output));

  return kTfLiteOk;
}

TfLiteStatus ReluEval(TfLiteContext* context, TfLiteNode* node) {
  const TfLiteTensor* input = GetInput(context, node, 0);
  TfLiteTensor* output = GetOutput(context, node, 0);
  switch (input->type) {
    case kTfLiteFloat32: {
      optimized_ops::Relu(GetTensorShape(input), GetTensorData<float>(input),
                          GetTensorShape(output), GetTensorData<float>(output));
      return kTfLiteOk;
    } break;
    default:
      context->ReportError(context,
                           "Only float32 is supported currently, got %s.",
                           TfLiteTypeGetName(input->type));
      return kTfLiteError;
  }
}

namespace {
template <typename T>
void QuantizedRelu1(const TfLiteTensor* input, TfLiteTensor* output) {
  ActivationParams params;
  int32 kMin = -1;
  int32 kMax = 1;
  params.activation_type = FusedActivationFunctionType::kRelu1;

  // Relu1 has a min range of -1, we need to quantize this
  params.quantized_activation_min =
      std::max(static_cast<int32_t>(std::numeric_limits<T>::min()),
               output->params.zero_point +
                   static_cast<int32>(roundf(kMin / output->params.scale)));

  // Relu1 has a max range of 1, we need to quantize this
  params.quantized_activation_max =
      std::min(static_cast<int32_t>(std::numeric_limits<T>::max()),
               output->params.zero_point +
                   static_cast<int32>(roundf(kMax / output->params.scale)));

  // Reused the optimized function written for ReluX
  optimized_ops::ReluX(params, GetTensorShape(input), GetTensorData<T>(input),
                       GetTensorShape(output), GetTensorData<T>(output));
}
}  // namespace

TfLiteStatus Relu1Eval(TfLiteContext* context, TfLiteNode* node) {
  const TfLiteTensor* input = GetInput(context, node, 0);
  TfLiteTensor* output = GetOutput(context, node, 0);
  switch (input->type) {
    case kTfLiteFloat32: {
      optimized_ops::Relu1(GetTensorShape(input), GetTensorData<float>(input),
                           GetTensorShape(output),
                           GetTensorData<float>(output));
      return kTfLiteOk;
    } break;
    case kTfLiteUInt8: {
      QuantizedRelu1<uint8_t>(input, output);
      return kTfLiteOk;
    } break;
    case kTfLiteInt8: {
      QuantizedRelu1<int8_t>(input, output);
      return kTfLiteOk;
    } break;
    default:
      context->ReportError(context,
                           "Only float32, uint8, int8 supported "
                           "currently, got %s.",
                           TfLiteTypeGetName(input->type));
      return kTfLiteError;
  }
}

namespace {
template <typename T>
void QuantizedRelu6(const TfLiteTensor* input, TfLiteTensor* output) {
  ActivationParams params;
  params.activation_type = FusedActivationFunctionType::kRelu6;
  params.quantized_activation_min =
      std::max(static_cast<int32_t>(std::numeric_limits<T>::min()),
               output->params.zero_point +
                   static_cast<int32>(roundf(0.f / output->params.scale)));
  params.quantized_activation_max =
      std::min(static_cast<int32_t>(std::numeric_limits<T>::max()),
               output->params.zero_point +
                   static_cast<int32>(roundf(6.f / output->params.scale)));
  optimized_ops::ReluX(params, GetTensorShape(input), GetTensorData<T>(input),
                       GetTensorShape(output), GetTensorData<T>(output));
}
}  // namespace

TfLiteStatus Relu6Eval(TfLiteContext* context, TfLiteNode* node) {
  const TfLiteTensor* input = GetInput(context, node, 0);
  TfLiteTensor* output = GetOutput(context, node, 0);
  switch (input->type) {
    case kTfLiteFloat32: {
      size_t elements = input->bytes / sizeof(float);
      float* in = input->data.f;
      float* in_end = in + elements;
      float* out = output->data.f;
      for (; in < in_end; in++, out++) *out = std::min(std::max(0.f, *in), 6.f);
      return kTfLiteOk;
    } break;
    case kTfLiteUInt8:
      QuantizedRelu6<uint8_t>(input, output);
      return kTfLiteOk;
    case kTfLiteInt8: {
      QuantizedRelu6<int8_t>(input, output);
      return kTfLiteOk;
    } break;
    default:
      context->ReportError(
          context,
          "Only float32, uint8 and int8 are supported currently, got %s.",
          TfLiteTypeGetName(input->type));
      return kTfLiteError;
  }
}

template <KernelType kernel_type>
TfLiteStatus TanhEval(TfLiteContext* context, TfLiteNode* node) {
  OpData* data = reinterpret_cast<OpData*>(node->user_data);
  const TfLiteTensor* input = GetInput(context, node, 0);
  TfLiteTensor* output = GetOutput(context, node, 0);
  switch (input->type) {
    case kTfLiteFloat32: {
      if (kernel_type == kGenericOptimized) {
        optimized_ops::Tanh(GetTensorShape(input), GetTensorData<float>(input),
                            GetTensorShape(output),
                            GetTensorData<float>(output));
      } else {
        reference_ops::Tanh(GetTensorShape(input), GetTensorData<float>(input),
                            GetTensorShape(output),
                            GetTensorData<float>(output));
      }
      return kTfLiteOk;
    } break;
    case kTfLiteInt16: {
      TanhParams params;
      params.input_left_shift = data->input_left_shift;
      if (kernel_type == kGenericOptimized) {
        optimized_ops::Tanh(
            params, GetTensorShape(input), GetTensorData<int16_t>(input),
            GetTensorShape(output), GetTensorData<int16_t>(output));
      } else {
        reference_ops::Tanh(
            params, GetTensorShape(input), GetTensorData<int16_t>(input),
            GetTensorShape(output), GetTensorData<int16_t>(output));
      }
      return kTfLiteOk;
    } break;
    case kTfLiteUInt8: {
      TanhParams params;
      params.input_zero_point = input->params.zero_point;
      params.input_range_radius = data->input_range_radius;
      params.input_multiplier = data->input_multiplier;
      params.input_left_shift = data->input_left_shift;
      if (kernel_type == kGenericOptimized) {
        optimized_ops::Tanh(
            params, GetTensorShape(input), GetTensorData<uint8_t>(input),
            GetTensorShape(output), GetTensorData<uint8_t>(output));
      } else {
        reference_ops::Tanh(
            params, GetTensorShape(input), GetTensorData<uint8_t>(input),
            GetTensorShape(output), GetTensorData<uint8_t>(output));
      }
      return kTfLiteOk;
    } break;
    case kTfLiteInt8: {
      const auto input_shape = GetTensorShape(input);
      const auto output_shape = GetTensorShape(output);
      const int size = MatchingFlatSize(input_shape, output_shape);
      reference_integer_ops::Tanh(
          input->params.zero_point, data->input_range_radius,
          data->input_multiplier, data->input_left_shift, size,
          GetTensorData<int8_t>(input), GetTensorData<int8_t>(output));
      return kTfLiteOk;
    } break;
    default:
      context->ReportError(context,
                           "Only float32, uint8, int16 and int8 are supported "
                           "currently, got %s.",
                           TfLiteTypeGetName(input->type));
      return kTfLiteError;
  }
}

// Sigmoid is also know as "Logistic".
template <KernelType kernel_type>
TfLiteStatus SigmoidEval(TfLiteContext* context, TfLiteNode* node) {
  OpData* data = reinterpret_cast<OpData*>(node->user_data);

  const TfLiteTensor* input = GetInput(context, node, 0);
  TfLiteTensor* output = GetOutput(context, node, 0);
  switch (input->type) {
    case kTfLiteFloat32: {
      if (kernel_type == kGenericOptimized) {
        optimized_ops::Logistic(
            GetTensorShape(input), GetTensorData<float>(input),
            GetTensorShape(output), GetTensorData<float>(output));
      } else {
        reference_ops::Logistic(
            GetTensorShape(input), GetTensorData<float>(input),
            GetTensorShape(output), GetTensorData<float>(output));
      }
      break;
    }
    case kTfLiteInt16: {
      LogisticParams params;
      if (kernel_type == kGenericOptimized) {
        optimized_ops::Logistic(
            params, GetTensorShape(input), GetTensorData<int16_t>(input),
            GetTensorShape(output), GetTensorData<int16_t>(output));
      } else {
        reference_ops::Logistic(
            params, GetTensorShape(input), GetTensorData<int16_t>(input),
            GetTensorShape(output), GetTensorData<int16_t>(output));
      }
      break;
    }
    case kTfLiteUInt8: {
      LogisticParams params;
      params.input_zero_point = input->params.zero_point;
      params.input_range_radius = data->input_range_radius;
      params.input_multiplier = data->input_multiplier;
      params.input_left_shift = data->input_left_shift;
      if (kernel_type == kGenericOptimized) {
        optimized_ops::Logistic(
            params, GetTensorShape(input), GetTensorData<uint8_t>(input),
            GetTensorShape(output), GetTensorData<uint8_t>(output));
      } else {
        reference_ops::Logistic(
            params, GetTensorShape(input), GetTensorData<uint8_t>(input),
            GetTensorShape(output), GetTensorData<uint8_t>(output));
      }
      break;
    }
    case kTfLiteInt8: {
      const int input_size =
          MatchingFlatSize(GetTensorShape(input), GetTensorShape(output));
      reference_integer_ops::Logistic(
          input->params.zero_point, data->input_range_radius,
          data->input_multiplier, data->input_left_shift, input_size,
          GetTensorData<int8_t>(input), GetTensorData<int8_t>(output));
      break;
    }
    default:
      context->ReportError(context,
                           "Only float32, uint8, int16 and int8 are supported "
                           "currently, got %s.",
                           TfLiteTypeGetName(input->type));
      return kTfLiteError;
  }
  return kTfLiteOk;
}

// Performs softmax along the input of size (input_size * batch_size).
void Softmax(const float* in, const int input_size, const int batch_size,
             const float beta, float* out) {
  TF_LITE_ASSERT(input_size > 0);

  // For each batch
  for (int b = 0; b < batch_size; b++) {
    // Find the max coeff.
    float max_coeff = in[0];
    for (int i = 1; i < input_size; i++) {
      if (in[i] > max_coeff) max_coeff = in[i];
    }

    // Compute the normalized sum of exps.
    float exp_sum = 0.0;
    for (int i = 0; i < input_size; i++) {
      out[i] = std::exp((in[i] - max_coeff) * beta);
      exp_sum += out[i];
    }

    // Divide by the sum of exps.
    float reciprocal_sum_exp = 1.f / exp_sum;
    for (int i = 0; i < input_size; i++) {
      out[i] *= reciprocal_sum_exp;
    }

    // Advance in and out pointers for the next batch.
    in += input_size;
    out += input_size;
  }
}

// Takes a 1D tensor and performs softmax along it.
void Softmax1DFloat(const TfLiteTensor* input, TfLiteTensor* output,
                    TfLiteSoftmaxParams* params) {
  const int input_size = input->dims->data[0];
  Softmax(input->data.f, input_size, 1, params->beta, output->data.f);
}

// Takes a 2D tensor and perform softmax along the last dimension.
void Softmax2DFloat(const TfLiteTensor* input, TfLiteTensor* output,
                    TfLiteSoftmaxParams* params) {
  const int batch_size = input->dims->data[0];
  const int input_size = input->dims->data[1];
  Softmax(input->data.f, input_size, batch_size, params->beta, output->data.f);
}

// Takes a 3D tensor and perform softmax along the last dimension.
void Softmax3DFloat(const TfLiteTensor* input, TfLiteTensor* output,
                    TfLiteSoftmaxParams* params) {
  const int batch_size = input->dims->data[0];
  const int intermediate_size = input->dims->data[1];
  const int input_size = input->dims->data[2];
  SoftmaxParams op_params;
  op_params.beta = params->beta;
  optimized_ops::Softmax(
      op_params, GetTensorShape({batch_size, intermediate_size, 1, input_size}),
      GetTensorData<float>(input),
      GetTensorShape({batch_size, intermediate_size, 1, input_size}),
      GetTensorData<float>(output));
}

void Softmax1DQuantizedUint8(const TfLiteTensor* input, TfLiteTensor* output,
                             TfLiteSoftmaxParams* params, OpData* data) {
  // TODO(ahentz): this is arguably a dirty trick. Since the implementation
  // always traverses the last dimension of a 4D tensor, we will pretend our 1D
  // tensor is 4D in a special way. We will convert a (Y) shape into a (1,
  // 1, 1, Y) shape.
  const int input_size = input->dims->data[0];
  SoftmaxParams op_params;
  op_params.input_multiplier = data->input_multiplier;
  op_params.input_left_shift = data->input_left_shift;
  op_params.diff_min = data->diff_min;
  optimized_ops::Softmax(op_params, GetTensorShape({1, 1, 1, input_size}),
                         GetTensorData<uint8_t>(input),
                         GetTensorShape({1, 1, 1, input_size}),
                         GetTensorData<uint8_t>(output));
}
void Softmax2DQuantizedUint8(const TfLiteTensor* input, TfLiteTensor* output,
                             TfLiteSoftmaxParams* params, OpData* data) {
  // TODO(ahentz): this is arguably a dirty trick. Since the implementation
  // always traverses the last dimension of a 4D tensor, we will pretend our 2D
  // tensor is 4D in a special way. We will convert a (X, Y) shape into a (X,
  // 1, 1, Y) shape.
  const int batch_size = input->dims->data[0];
  const int input_size = input->dims->data[1];
  SoftmaxParams op_params;
  op_params.input_multiplier = data->input_multiplier;
  op_params.input_left_shift = data->input_left_shift;
  op_params.diff_min = data->diff_min;
  optimized_ops::Softmax(op_params,
                         GetTensorShape({batch_size, 1, 1, input_size}),
                         GetTensorData<uint8_t>(input),
                         GetTensorShape({batch_size, 1, 1, input_size}),
                         GetTensorData<uint8_t>(output));
}

void Softmax3DQuantizedUint8(const TfLiteTensor* input, TfLiteTensor* output,
                             TfLiteSoftmaxParams* params, OpData* data) {
  const int batch_size = input->dims->data[0];
  const int intermediate_size = input->dims->data[1];
  const int input_size = input->dims->data[2];
  SoftmaxParams op_params;
  op_params.input_multiplier = data->input_multiplier;
  op_params.input_left_shift = data->input_left_shift;
  op_params.diff_min = data->diff_min;
  optimized_ops::Softmax(
      op_params, GetTensorShape({batch_size, intermediate_size, 1, input_size}),
      GetTensorData<uint8_t>(input),
      GetTensorShape({batch_size, intermediate_size, 1, input_size}),
      GetTensorData<uint8_t>(output));
}

// Takes a 4D tensor and perform softmax along the forth dimension.
void Softmax4DFloat(const TfLiteTensor* input, TfLiteTensor* output,
                    TfLiteSoftmaxParams* params) {
  SoftmaxParams op_params;
  op_params.beta = params->beta;
  optimized_ops::Softmax(op_params, GetTensorShape(input),
                         GetTensorData<float>(input), GetTensorShape(output),
                         GetTensorData<float>(output));
}

void Softmax4DQuantizedUint8(const TfLiteTensor* input, TfLiteTensor* output,
                             TfLiteSoftmaxParams* params, OpData* data) {
  SoftmaxParams op_params;
  op_params.input_multiplier = data->input_multiplier;
  op_params.input_left_shift = data->input_left_shift;
  op_params.diff_min = data->diff_min;
  optimized_ops::Softmax(op_params, GetTensorShape(input),
                         GetTensorData<uint8_t>(input), GetTensorShape(output),
                         GetTensorData<uint8_t>(output));
}

// TODO(jianlijianli): Try merging Softmax<n>DQuantizedInt8 with
// Softmax<n>DQuantized, which needs a larger refactor.
void Softmax1DQuantizedInt8(const TfLiteTensor* input, TfLiteTensor* output,
                            TfLiteSoftmaxParams* params, OpData* data) {
  const int input_size = input->dims->data[0];
  SoftmaxParams op_params;
  op_params.input_multiplier = data->input_multiplier;
  op_params.input_left_shift = data->input_left_shift;
  op_params.diff_min = data->diff_min;
  optimized_integer_ops::Softmax(
      op_params, GetTensorShape({1, 1, 1, input_size}),
      GetTensorData<int8_t>(input), GetTensorShape({1, 1, 1, input_size}),
      GetTensorData<int8_t>(output));
}

void Softmax2DQuantizedInt8(const TfLiteTensor* input, TfLiteTensor* output,
                            TfLiteSoftmaxParams* params, OpData* data) {
  const int batch_size = input->dims->data[0];
  const int input_size = input->dims->data[1];
  SoftmaxParams op_params;
  op_params.input_multiplier = data->input_multiplier;
  op_params.input_left_shift = data->input_left_shift;
  op_params.diff_min = data->diff_min;
  optimized_integer_ops::Softmax(op_params,
                                 GetTensorShape({batch_size, 1, 1, input_size}),
                                 GetTensorData<int8_t>(input),
                                 GetTensorShape({batch_size, 1, 1, input_size}),
                                 GetTensorData<int8_t>(output));
}

void Softmax3DQuantizedInt8(const TfLiteTensor* input, TfLiteTensor* output,
                            TfLiteSoftmaxParams* params, OpData* data) {
  const int batch_size = input->dims->data[0];
  const int intermediate_size = input->dims->data[1];
  const int input_size = input->dims->data[2];
  SoftmaxParams op_params;
  op_params.input_multiplier = data->input_multiplier;
  op_params.input_left_shift = data->input_left_shift;
  op_params.diff_min = data->diff_min;
  optimized_integer_ops::Softmax(
      op_params, GetTensorShape({batch_size, intermediate_size, 1, input_size}),
      GetTensorData<int8_t>(input),
      GetTensorShape({batch_size, intermediate_size, 1, input_size}),
      GetTensorData<int8_t>(output));
}

void Softmax4DQuantizedInt8(const TfLiteTensor* input, TfLiteTensor* output,
                            TfLiteSoftmaxParams* params, OpData* data) {
  SoftmaxParams op_params;
  op_params.input_multiplier = data->input_multiplier;
  op_params.input_left_shift = data->input_left_shift;
  op_params.diff_min = data->diff_min;
  optimized_integer_ops::Softmax(
      op_params, GetTensorShape(input), GetTensorData<int8_t>(input),
      GetTensorShape(output), GetTensorData<int8_t>(output));
}

TfLiteStatus SoftmaxEval(TfLiteContext* context, TfLiteNode* node) {
  auto* params = reinterpret_cast<TfLiteSoftmaxParams*>(node->builtin_data);
  OpData* data = reinterpret_cast<OpData*>(node->user_data);

  const TfLiteTensor* input = GetInput(context, node, 0);
  TfLiteTensor* output = GetOutput(context, node, 0);

  // TODO(ahentz): consider an implementation that works for many (all?)
  // dimensions.
  switch (input->type) {
    case kTfLiteFloat32: {
      if (NumDimensions(input) == 1) {
        Softmax1DFloat(input, output, params);
        return kTfLiteOk;
      }
      if (NumDimensions(input) == 2) {
        Softmax2DFloat(input, output, params);
        return kTfLiteOk;
      }
      if (NumDimensions(input) == 3) {
        Softmax3DFloat(input, output, params);
        return kTfLiteOk;
      }
      if (NumDimensions(input) == 4) {
        Softmax4DFloat(input, output, params);
        return kTfLiteOk;
      }
      context->ReportError(
          context, "Only 1D, 2D and 4D tensors supported currently, got %dD.",
          NumDimensions(input));
      return kTfLiteError;
    }
    case kTfLiteUInt8: {
      if (NumDimensions(input) == 1) {
        Softmax1DQuantizedUint8(input, output, params, data);
        return kTfLiteOk;
      }
      if (NumDimensions(input) == 2) {
        Softmax2DQuantizedUint8(input, output, params, data);
        return kTfLiteOk;
      }
      if (NumDimensions(input) == 3) {
        Softmax3DQuantizedUint8(input, output, params, data);
        return kTfLiteOk;
      }
      if (NumDimensions(input) == 4) {
        Softmax4DQuantizedUint8(input, output, params, data);
        return kTfLiteOk;
      }
      context->ReportError(
          context, "Only 2D and 4D tensors supported currently, got %dD.",
          NumDimensions(input));
      return kTfLiteError;
    }
    case kTfLiteInt8: {
      if (NumDimensions(input) == 1) {
        Softmax1DQuantizedInt8(input, output, params, data);
        return kTfLiteOk;
      }
      if (NumDimensions(input) == 2) {
        Softmax2DQuantizedInt8(input, output, params, data);
        return kTfLiteOk;
      }
      if (NumDimensions(input) == 3) {
        Softmax3DQuantizedInt8(input, output, params, data);
        return kTfLiteOk;
      }
      if (NumDimensions(input) == 4) {
        Softmax4DQuantizedInt8(input, output, params, data);
        return kTfLiteOk;
      }
      context->ReportError(
          context,
          "Only 4D tensors supported currently for Int8 kernels, got %dD.",
          NumDimensions(input));
      return kTfLiteError;
    }

    default:
      context->ReportError(
          context, "Only float32 and uint8_t are supported currently, got %s.",
          TfLiteTypeGetName(input->type));
      return kTfLiteError;
  }
}

template <KernelType kernel_type>
TfLiteStatus LogSoftmaxEval(TfLiteContext* context, TfLiteNode* node) {
  const LogSoftmaxOpData* data =
      reinterpret_cast<LogSoftmaxOpData*>(node->user_data);
  const TfLiteTensor* input = GetInput(context, node, 0);
  TfLiteTensor* output = GetOutput(context, node, 0);
  switch (input->type) {
    case kTfLiteFloat32: {
      SoftmaxParams op_params;
      if (kernel_type == kGenericOptimized) {
        optimized_ops::LogSoftmax(
            op_params, GetTensorShape(input), GetTensorData<float>(input),
            GetTensorShape(output), GetTensorData<float>(output));
      } else {
        reference_ops::LogSoftmax(
            op_params, GetTensorShape(input), GetTensorData<float>(input),
            GetTensorShape(output), GetTensorData<float>(output));
      }
      return kTfLiteOk;
    }
    case kTfLiteUInt8: {
      SoftmaxParams op_params;
      op_params.input_multiplier = data->input_multiplier;
      op_params.input_left_shift = data->input_left_shift;
      op_params.reverse_scaling_divisor = data->reverse_scaling_divisor;
      op_params.reverse_scaling_right_shift = data->reverse_scaling_right_shift;
      op_params.diff_min = data->diff_min;
      if (kernel_type == kGenericOptimized) {
        optimized_ops::LogSoftmax(
            op_params, GetTensorShape(input), GetTensorData<uint8_t>(input),
            GetTensorShape(output), GetTensorData<uint8_t>(output));
      } else {
        reference_ops::LogSoftmax(
            op_params, GetTensorShape(input), GetTensorData<uint8_t>(input),
            GetTensorShape(output), GetTensorData<uint8_t>(output));
      }
      return kTfLiteOk;
    }
    case kTfLiteInt8: {
      const auto input_shape = GetTensorShape(input);
      const auto output_shape = GetTensorShape(output);
      const int trailing_dim = input_shape.DimensionsCount() - 1;
      const int outer_size =
          MatchingFlatSizeSkipDim(input_shape, trailing_dim, output_shape);
      const int depth =
          MatchingDim(input_shape, trailing_dim, output_shape, trailing_dim);
      reference_integer_ops::LogSoftmax(
          data->input_multiplier, data->input_left_shift,
          data->reverse_scaling_divisor, data->reverse_scaling_right_shift,
          data->diff_min, outer_size, depth, GetTensorData<int8_t>(input),
          GetTensorData<int8_t>(output));
      return kTfLiteOk;
    }
    default:
      context->ReportError(
          context,
          "Only float32, uint8 and int8 are supported currently, got %s.",
          TfLiteTypeGetName(input->type));
      return kTfLiteError;
  }
}

template <typename T>
T ApplyPrelu(T input, T alpha) {
  return input >= 0.0 ? input : input * alpha;
}

TfLiteStatus PreluEval(TfLiteContext* context, TfLiteNode* node) {
  const TfLiteTensor* input = GetInput(context, node, 0);
  const TfLiteTensor* alpha = GetInput(context, node, 1);
  TfLiteTensor* output = GetOutput(context, node, 0);
  const PreluOpData* data = reinterpret_cast<PreluOpData*>(node->user_data);
  switch (input->type) {
    case kTfLiteFloat32: {
      reference_ops::BroadcastBinaryFunction4DSlow<float, float, float>(
          GetTensorShape(input), GetTensorData<float>(input),
          GetTensorShape(alpha), GetTensorData<float>(alpha),
          GetTensorShape(output), GetTensorData<float>(output),
          ApplyPrelu<float>);
      return kTfLiteOk;
    } break;
    case kTfLiteUInt8: {
      PreluParams op_params;
      op_params.input_offset = -input->params.zero_point;
      op_params.alpha_offset = -alpha->params.zero_point;
      op_params.output_offset = output->params.zero_point;
      op_params.output_multiplier = data->output_multiplier;
      op_params.output_shift = data->output_shift;
      reference_ops::BroadcastPrelu4DSlow(
          op_params, GetTensorShape(input), GetTensorData<uint8_t>(input),
          GetTensorShape(alpha), GetTensorData<uint8_t>(alpha),
          GetTensorShape(output), GetTensorData<uint8_t>(output));
      return kTfLiteOk;
    } break;
    default:
      context->ReportError(
          context, "Only float32 and uint8 are supported currently, got %d.",
          TfLiteTypeGetName(input->type));
      return kTfLiteError;
  }
}

namespace {
template <typename T>
void QLeakyRelu(const TfLiteTensor* input, TfLiteTensor* output, float alpha,
                const LeakyReluOpData* data) {
  LeakyReluParams op_params;
  op_params.input_offset = input->params.zero_point;
  op_params.alpha_offset = input->params.zero_point;
  op_params.output_offset = output->params.zero_point;

  op_params.output_multiplier = data->output_multiplier;
  op_params.output_shift = data->output_shift;

  reference_ops::QuantizeLeakyRelu(
      op_params, data->q_alpha, GetTensorShape(input), GetTensorData<T>(input),
      GetTensorShape(output), GetTensorData<T>(output));
}
}  // namespace

TfLiteStatus LeakyReluEval(TfLiteContext* context, TfLiteNode* node) {
  const TfLiteTensor* input = GetInput(context, node, 0);
  TfLiteTensor* output = GetOutput(context, node, 0);
  const auto* params =
      reinterpret_cast<TfLiteLeakyReluParams*>(node->builtin_data);
  const LeakyReluOpData* data =
      reinterpret_cast<LeakyReluOpData*>(node->user_data);

  LeakyReluParams op_params;
  op_params.alpha = params->alpha;
  switch (input->type) {
    case kTfLiteFloat32: {
      optimized_ops::LeakyRelu(
          op_params, GetTensorShape(input), GetTensorData<float>(input),
          GetTensorShape(output), GetTensorData<float>(output));
      return kTfLiteOk;
    } break;
    case kTfLiteUInt8: {
      QLeakyRelu<uint8_t>(input, output, params->alpha, data);
      return kTfLiteOk;
    } break;
    default:
      context->ReportError(
          context, "Only float32 and uint8 is supported currently, got %s.",
          TfLiteTypeGetName(input->type));
      return kTfLiteError;
  }
}

TfLiteStatus EluEval(TfLiteContext* context, TfLiteNode* node) {
  const TfLiteTensor* input = GetInput(context, node, 0);
  TfLiteTensor* output = GetOutput(context, node, 0);
  switch (input->type) {
    case kTfLiteFloat32: {
      optimized_ops::Elu(GetTensorShape(input), GetTensorData<float>(input),
                         GetTensorShape(output), GetTensorData<float>(output));
      return kTfLiteOk;
    } break;
    default:
      context->ReportError(context,
                           "Only float32 is supported currently, got %s.",
                           TfLiteTypeGetName(input->type));
      return kTfLiteError;
  }
}

}  // namespace activations

TfLiteRegistration* Register_ELU() {
  static TfLiteRegistration r = {/*init=*/nullptr, /*free=*/nullptr,
                                 activations::GenericPrepare,
                                 activations::EluEval};
  return &r;
}

TfLiteRegistration* Register_RELU() {
  static TfLiteRegistration r = {/*init=*/nullptr, /*free=*/nullptr,
                                 activations::GenericPrepare,
                                 activations::ReluEval};
  return &r;
}

TfLiteRegistration* Register_RELU_N1_TO_1() {
  static TfLiteRegistration r = {/*init=*/nullptr, /*free=*/nullptr,
                                 activations::GenericPrepare,
                                 activations::Relu1Eval};
  return &r;
}

TfLiteRegistration* Register_RELU6() {
  static TfLiteRegistration r = {/*init=*/nullptr, /*free=*/nullptr,
                                 activations::GenericPrepare,
                                 activations::Relu6Eval};
  return &r;
}

TfLiteRegistration* Register_TANH_REF() {
  static TfLiteRegistration r = {
      activations::Init, activations::Free, activations::TanhPrepare,
      activations::TanhEval<activations::kReference>};
  return &r;
}

TfLiteRegistration* Register_TANH() {
  static TfLiteRegistration r = {
      activations::Init, activations::Free, activations::TanhPrepare,
      activations::TanhEval<activations::kGenericOptimized>};
  return &r;
}

TfLiteRegistration* Register_LOGISTIC_REF() {
  static TfLiteRegistration r = {
      activations::Init, activations::Free, activations::SigmoidPrepare,
      activations::SigmoidEval<activations::kReference>};
  return &r;
}

TfLiteRegistration* Register_LOGISTIC() {
  static TfLiteRegistration r = {
      activations::Init, activations::Free, activations::SigmoidPrepare,
      activations::SigmoidEval<activations::kGenericOptimized>};
  return &r;
}

TfLiteRegistration* Register_SOFTMAX() {
  static TfLiteRegistration r = {activations::Init, activations::Free,
                                 activations::SoftmaxPrepare,
                                 activations::SoftmaxEval};
  return &r;
}

TfLiteRegistration* Register_LOG_SOFTMAX_REF() {
  static TfLiteRegistration r = {
      activations::LogSoftmaxInit, activations::LogSoftmaxFree,
      activations::LogSoftmaxPrepare,
      activations::LogSoftmaxEval<activations::kReference>};
  return &r;
}

TfLiteRegistration* Register_LOG_SOFTMAX() {
  static TfLiteRegistration r = {
      activations::LogSoftmaxInit, activations::LogSoftmaxFree,
      activations::LogSoftmaxPrepare,
      activations::LogSoftmaxEval<activations::kGenericOptimized>};
  return &r;
}

TfLiteRegistration* Register_PRELU() {
  static TfLiteRegistration r = {activations::PreluInit, activations::PreluFree,
                                 activations::PreluPrepare,
                                 activations::PreluEval};
  return &r;
}

TfLiteRegistration* Register_LEAKY_RELU() {
  static TfLiteRegistration r = {
      activations::LeakyReluInit, activations::LeakyReluFree,
      activations::LeakyReluPrepare, activations::LeakyReluEval};
  return &r;
}

}  // namespace builtin
}  // namespace ops
}  // namespace tflite
