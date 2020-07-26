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
#include "tensorflow/lite/kernels/lstm_eval.h"

#include <algorithm>
#include <cstdint>

#include "tensorflow/lite/kernels/cpu_backend_context.h"
#include "tensorflow/lite/kernels/internal/compatibility.h"

#ifdef GEMMLOWP_PROFILING
#include "profiling/profiler.h"
#endif

#include "third_party/eigen3/Eigen/Core"
#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/c/c_api_internal.h"
#include "tensorflow/lite/kernels/internal/kernel_utils.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"
#include "tensorflow/lite/kernels/internal/tensor_utils.h"
#include "tensorflow/lite/kernels/op_macros.h"

namespace tflite {
namespace ops {
namespace builtin {
namespace lstm_eval {

namespace {

// Small float to avoid divergence during calculation of deviation for layer
// norm lstm.
const float kLayerNormEpsilon = 1e-8;

// Performs an LSTM batch inference step for input specified by input_ptr_batch.
// The LSTM cell is specified by the pointers to its weights (*_weights_ptr) and
// biases (*_bias_ptr), and buffers (*_scratch), along with additional
// parameters:
//  - params: various LSTM params including activation, clipping, etc.,
//  - n_batch: size of batch,
//  - n_cell: number of cells (or units),
//  - n_input: the input size,
//  - n_aux_input: the auxiliary input size.
//  - n_output: the output size.
//  - output_batch_leading_dim: the leading dimension of the output buffer.
//
// LSTM weights:
// Input weights of size 'n_cell * n_input':
//   input_to_input_weights            - optional (can be nullptr)
//   input_to_forget_weights
//   input_to_cell_weights
//   input_to_output_weights
// Auxiliary input weights of size 'n_cell * n_aux_input':
//   aux_input_to_input_weights        - optional
//   aux_input_to_forget_weights       - optional
//   aux_input_to_cell_weights         - optional
//   aux_input_to_output_weights       - optional
// Recurrent weights of size 'n_cell * n_output':
//   recurrent_to_input_weights        - optional
//   recurrent_to_forget_weights
//   recurrent_to_cell_weights
//   recurrent_to_input_weights
// Peephole weights of size 'n_cell', representing diagonal matrices.
//   cell_to_input_weights             - optional
//   cell_to_cell_weights              - optional
//   cell_to_output_weights            - optional
// Projection weights of size 'n_output * n_cell'
//   projection_weights_ptr            - optional
// Gate biases of size 'n_cell':
//   input_gate_bias_ptr               - optional
//   forget_gate_bias_ptr
//   cell_gate_bias_ptr
//   output_gate_bias_ptr
//
// Layer norm coefficients of size 'n_cell', representing diagonal matrices.
//   input_layer_norm_coefficients_ptr  - optional
//   forget_layer_norm_coefficients_ptr - optional
//   cell_layer_norm_coefficients_ptr   - optional
//   output_layer_norm_coefficients_ptr - optional
//
// The pointers to the cell and output state and the output are updated.
//
// The pointers with the suffix "_batch" point to data aligned in batch_major
// order, and each step processes batch_size many inputs from input_ptr_batch,
// and updates batch_size many cell and output states.
//
// The output_batch_dim is output.shape[-1], i.e. the outermost dimension of the
// output tensor, and in most cases will be equal to n_output. It is usually not
// when we want to store the LSTM output into a slice of the output tensor, e.g.
// for bidirectional LSTMs with merge_outputs. In this case, the batched
// operations cannot be used since they assume that the batched outputs are
// contiguous, and we manually loop over the batched outputs.
inline void LstmStepWithAuxInput(
    const float* input_ptr_batch, const float* input_to_input_weights_ptr,
    const float* input_to_forget_weights_ptr,
    const float* input_to_cell_weights_ptr,
    const float* input_to_output_weights_ptr, const float* aux_input_ptr_batch,
    const float* aux_input_to_input_weights_ptr,
    const float* aux_input_to_forget_weights_ptr,
    const float* aux_input_to_cell_weights_ptr,
    const float* aux_input_to_output_weights_ptr,
    const float* recurrent_to_input_weights_ptr,
    const float* recurrent_to_forget_weights_ptr,
    const float* recurrent_to_cell_weights_ptr,
    const float* recurrent_to_output_weights_ptr,
    const float* cell_to_input_weights_ptr,
    const float* cell_to_forget_weights_ptr,
    const float* cell_to_output_weights_ptr,
    const float* input_layer_norm_coefficients_ptr,
    const float* forget_layer_norm_coefficients_ptr,
    const float* cell_layer_norm_coefficients_ptr,
    const float* output_layer_norm_coefficients_ptr,
    const float* input_gate_bias_ptr, const float* forget_gate_bias_ptr,
    const float* cell_bias_ptr, const float* output_gate_bias_ptr,
    const float* projection_weights_ptr, const float* projection_bias_ptr,
    const TfLiteLSTMParams* params, int n_batch, int n_cell, int n_input,
    int n_aux_input, int n_output, int output_batch_leading_dim,
    float* output_state_ptr, float* cell_state_ptr, float* input_gate_scratch,
    float* forget_gate_scratch, float* cell_scratch, float* output_gate_scratch,
    float* output_ptr_batch) {
#ifdef GEMMLOWP_PROFILING
  gemmlowp::ScopedProfilingLabel label("LstmStepWithAuxInputFloat");
#endif
  // Since we have already checked that weights are all there or none, we can
  // check the existence of only one to the get the condition.
  const bool use_cifg = (input_to_input_weights_ptr == nullptr);
  const bool use_peephole = (cell_to_output_weights_ptr != nullptr);
  const bool is_layer_norm_lstm =
      (forget_layer_norm_coefficients_ptr != nullptr);

  // Initialize scratch buffers with bias for regular lstm or initialize with
  // zero for layer norm lstm.
  if (is_layer_norm_lstm) {
    if (!use_cifg) {
      std::fill_n(input_gate_scratch, n_cell * n_batch, 0.0f);
    }
    std::fill_n(forget_gate_scratch, n_cell * n_batch, 0.0f);
    std::fill_n(cell_scratch, n_cell * n_batch, 0.0f);
    std::fill_n(output_gate_scratch, n_cell * n_batch, 0.0f);
  } else {
    if (!use_cifg) {
      tensor_utils::VectorBatchVectorAssign(input_gate_bias_ptr, n_cell,
                                            n_batch, input_gate_scratch);
    }
    tensor_utils::VectorBatchVectorAssign(forget_gate_bias_ptr, n_cell, n_batch,
                                          forget_gate_scratch);
    tensor_utils::VectorBatchVectorAssign(cell_bias_ptr, n_cell, n_batch,
                                          cell_scratch);
    tensor_utils::VectorBatchVectorAssign(output_gate_bias_ptr, n_cell, n_batch,
                                          output_gate_scratch);
  }

  // For each batch and cell: compute input_weight * input.
  if (!use_cifg) {
    tensor_utils::MatrixBatchVectorMultiplyAccumulate(
        input_to_input_weights_ptr, n_cell, n_input, input_ptr_batch, n_batch,
        input_gate_scratch, /*result_stride=*/1);
  }

  tensor_utils::MatrixBatchVectorMultiplyAccumulate(
      input_to_forget_weights_ptr, n_cell, n_input, input_ptr_batch, n_batch,
      forget_gate_scratch, /*result_stride=*/1);
  tensor_utils::MatrixBatchVectorMultiplyAccumulate(
      input_to_cell_weights_ptr, n_cell, n_input, input_ptr_batch, n_batch,
      cell_scratch, /*result_stride=*/1);
  tensor_utils::MatrixBatchVectorMultiplyAccumulate(
      input_to_output_weights_ptr, n_cell, n_input, input_ptr_batch, n_batch,
      output_gate_scratch, /*result_stride=*/1);

  // If auxiliary input is available then compute aux_input_weight * aux_input
  if (aux_input_ptr_batch != nullptr) {
    if (!use_cifg) {
      tensor_utils::MatrixBatchVectorMultiplyAccumulate(
          aux_input_to_input_weights_ptr, n_cell, n_aux_input,
          aux_input_ptr_batch, n_batch, input_gate_scratch,
          /*result_stride=*/1);
    }

    tensor_utils::MatrixBatchVectorMultiplyAccumulate(
        aux_input_to_forget_weights_ptr, n_cell, n_aux_input,
        aux_input_ptr_batch, n_batch, forget_gate_scratch, /*result_stride=*/1);
    tensor_utils::MatrixBatchVectorMultiplyAccumulate(
        aux_input_to_cell_weights_ptr, n_cell, n_aux_input, aux_input_ptr_batch,
        n_batch, cell_scratch, /*result_stride=*/1);
    tensor_utils::MatrixBatchVectorMultiplyAccumulate(
        aux_input_to_output_weights_ptr, n_cell, n_aux_input,
        aux_input_ptr_batch, n_batch, output_gate_scratch, /*result_stride=*/1);
  }

  // For each batch and cell: compute recurrent_weight * output_state.
  if (!use_cifg) {
    tensor_utils::MatrixBatchVectorMultiplyAccumulate(
        recurrent_to_input_weights_ptr, n_cell, n_output, output_state_ptr,
        n_batch, input_gate_scratch, /*result_stride=*/1);
  }
  tensor_utils::MatrixBatchVectorMultiplyAccumulate(
      recurrent_to_forget_weights_ptr, n_cell, n_output, output_state_ptr,
      n_batch, forget_gate_scratch,
      /*result_stride=*/1);
  tensor_utils::MatrixBatchVectorMultiplyAccumulate(
      recurrent_to_cell_weights_ptr, n_cell, n_output, output_state_ptr,
      n_batch, cell_scratch, /*result_stride=*/1);
  tensor_utils::MatrixBatchVectorMultiplyAccumulate(
      recurrent_to_output_weights_ptr, n_cell, n_output, output_state_ptr,
      n_batch, output_gate_scratch,
      /*result_stride=*/1);

  // For each batch and cell: update input gate.
  if (!use_cifg) {
    if (use_peephole) {
      tensor_utils::VectorBatchVectorCwiseProductAccumulate(
          cell_to_input_weights_ptr, n_cell, cell_state_ptr, n_batch,
          input_gate_scratch);
    }
    if (is_layer_norm_lstm) {
      tensor_utils::MeanStddevNormalization(input_gate_scratch,
                                            input_gate_scratch, n_cell, n_batch,
                                            kLayerNormEpsilon);
      tensor_utils::VectorBatchVectorCwiseProduct(
          input_layer_norm_coefficients_ptr, n_cell, input_gate_scratch,
          n_batch, input_gate_scratch);
      tensor_utils::VectorBatchVectorAdd(input_gate_bias_ptr, n_cell, n_batch,
                                         input_gate_scratch);
    }
    tensor_utils::ApplySigmoidToVector(input_gate_scratch, n_cell * n_batch,
                                       input_gate_scratch);
  }

  // For each batch and cell: update forget gate.
  if (use_peephole) {
    tensor_utils::VectorBatchVectorCwiseProductAccumulate(
        cell_to_forget_weights_ptr, n_cell, cell_state_ptr, n_batch,
        forget_gate_scratch);
  }
  if (is_layer_norm_lstm) {
    tensor_utils::MeanStddevNormalization(forget_gate_scratch,
                                          forget_gate_scratch, n_cell, n_batch,
                                          kLayerNormEpsilon);
    tensor_utils::VectorBatchVectorCwiseProduct(
        forget_layer_norm_coefficients_ptr, n_cell, forget_gate_scratch,
        n_batch, forget_gate_scratch);
    tensor_utils::VectorBatchVectorAdd(forget_gate_bias_ptr, n_cell, n_batch,
                                       forget_gate_scratch);
  }
  tensor_utils::ApplySigmoidToVector(forget_gate_scratch, n_cell * n_batch,
                                     forget_gate_scratch);

  // For each batch and cell: update the cell.
  tensor_utils::VectorVectorCwiseProduct(forget_gate_scratch, cell_state_ptr,
                                         n_batch * n_cell, cell_state_ptr);
  if (is_layer_norm_lstm) {
    tensor_utils::MeanStddevNormalization(cell_scratch, cell_scratch, n_cell,
                                          n_batch, kLayerNormEpsilon);
    tensor_utils::VectorBatchVectorCwiseProduct(
        cell_layer_norm_coefficients_ptr, n_cell, cell_scratch, n_batch,
        cell_scratch);
    tensor_utils::VectorBatchVectorAdd(cell_bias_ptr, n_cell, n_batch,
                                       cell_scratch);
  }
  tensor_utils::ApplyActivationToVector(cell_scratch, n_batch * n_cell,
                                        params->activation, cell_scratch);
  if (use_cifg) {
    tensor_utils::Sub1Vector(forget_gate_scratch, n_batch * n_cell,
                             forget_gate_scratch);
    tensor_utils::VectorVectorCwiseProductAccumulate(
        cell_scratch, forget_gate_scratch, n_batch * n_cell, cell_state_ptr);
  } else {
    tensor_utils::VectorVectorCwiseProductAccumulate(
        cell_scratch, input_gate_scratch, n_batch * n_cell, cell_state_ptr);
  }
  if (params->cell_clip > 0.0) {
    tensor_utils::ClipVector(cell_state_ptr, n_batch * n_cell,
                             params->cell_clip, cell_state_ptr);
  }

  // For each batch and cell: update the output gate.
  if (use_peephole) {
    tensor_utils::VectorBatchVectorCwiseProductAccumulate(
        cell_to_output_weights_ptr, n_cell, cell_state_ptr, n_batch,
        output_gate_scratch);
  }
  if (is_layer_norm_lstm) {
    tensor_utils::MeanStddevNormalization(output_gate_scratch,
                                          output_gate_scratch, n_cell, n_batch,
                                          kLayerNormEpsilon);
    tensor_utils::VectorBatchVectorCwiseProduct(
        output_layer_norm_coefficients_ptr, n_cell, output_gate_scratch,
        n_batch, output_gate_scratch);
    tensor_utils::VectorBatchVectorAdd(output_gate_bias_ptr, n_cell, n_batch,
                                       output_gate_scratch);
  }
  tensor_utils::ApplySigmoidToVector(output_gate_scratch, n_batch * n_cell,
                                     output_gate_scratch);
  tensor_utils::ApplyActivationToVector(cell_state_ptr, n_batch * n_cell,
                                        params->activation, cell_scratch);
  tensor_utils::VectorVectorCwiseProduct(output_gate_scratch, cell_scratch,
                                         n_batch * n_cell, output_gate_scratch);

  const bool use_projection_weight = (projection_weights_ptr != nullptr);
  const bool use_projection_bias = (projection_bias_ptr != nullptr);

  // For each batch: update the projection and output_state. Note that since
  // the output batch rows may not be contiguous (output_batch_leading_dim !=
  // n_output), we unroll the batched operations where this is the case.
  if (output_batch_leading_dim == n_output) {
    if (use_projection_weight) {
      if (use_projection_bias) {
        tensor_utils::VectorBatchVectorAssign(projection_bias_ptr, n_output,
                                              n_batch, output_ptr_batch);
      } else {
        std::fill_n(output_ptr_batch, n_batch * n_output, 0.0f);
      }
      tensor_utils::MatrixBatchVectorMultiplyAccumulate(
          projection_weights_ptr, n_output, n_cell, output_gate_scratch,
          n_batch, output_ptr_batch, /*result_stride=*/1);
      if (params->proj_clip > 0.0) {
        tensor_utils::ClipVector(output_ptr_batch, n_batch * n_output,
                                 params->proj_clip, output_ptr_batch);
      }
    } else {
      std::copy_n(output_gate_scratch, n_batch * n_output, output_ptr_batch);
    }
    std::copy_n(output_ptr_batch, n_batch * n_output, output_state_ptr);
  } else {
    if (use_projection_weight) {
      if (use_projection_bias) {
        for (int k = 0; k < n_batch; k++) {
          std::copy_n(projection_bias_ptr, n_output,
                      output_ptr_batch + k * output_batch_leading_dim);
        }
      } else {
        for (int k = 0; k < n_batch; k++) {
          std::fill_n(output_ptr_batch + k * output_batch_leading_dim, n_output,
                      0.0f);
        }
      }
      for (int k = 0; k < n_batch; k++) {
        tensor_utils::MatrixBatchVectorMultiplyAccumulate(
            projection_weights_ptr, n_output, n_cell,
            output_gate_scratch + k * n_cell,
            /*n_batch=*/1, output_ptr_batch + k * output_batch_leading_dim,
            /*result_stride=*/1);
        if (params->proj_clip > 0.0) {
          tensor_utils::ClipVector(
              output_ptr_batch + k * output_batch_leading_dim, n_output,
              params->proj_clip,
              output_ptr_batch + k * output_batch_leading_dim);
        }
      }
    } else {
      for (int k = 0; k < n_batch; k++) {
        std::copy_n(output_gate_scratch + k * n_output, n_output,
                    output_ptr_batch + k * output_batch_leading_dim);
      }
    }
    for (int k = 0; k < n_batch; k++) {
      std::copy_n(output_ptr_batch + k * output_batch_leading_dim, n_output,
                  output_state_ptr + k * n_output);
    }
  }
}

void ApplyActivationsToVector(float* input, int input_size,
                              TfLiteFusedActivation activation_type,
                              float* output) {
  using VectorMap = Eigen::Map<Eigen::Matrix<float, Eigen::Dynamic, 1>>;
  VectorMap input_map(input, input_size, 1);
  VectorMap output_map(output, input_size, 1);
  switch (activation_type) {
    case kTfLiteActSigmoid: {
      output_map.array() = input_map.array().logistic();
      break;
    }
    case kTfLiteActTanh: {
      output_map.array() = input_map.array().tanh();
      break;
    }
    default: {
      tensor_utils::ApplyActivationToVector(input, input_size, activation_type,
                                            output);
    }
  }
}

// Same as above but with quantized weight matrices. In detail:
// Input of size 'n_batch * n_input':
//   input_ptr_batch
//
// LSTM weights:
// Quantized input weights of size 'n_cell * n_input':
//   input_to_input_weights            - optional (can be nullptr)
//   input_to_forget_weights
//   input_to_cell_weights
//   input_to_input_weights
// Quantized auxiliary input weights of size 'n_cell * n_aux_input':
//   aux_input_to_input_weights        - optional
//   aux_input_to_forget_weights       - optional
//   aux_input_to_cell_weights         - optional
//   aux_input_to_output_weights       - optional
// Quantized recurrent weights of size 'n_cell * n_output':
//   recurrent_to_input_weights        - optional
//   recurrent_to_forget_weights
//   recurrent_to_cell_weights
//   recurrent_to_input_weights
// Quantized peephole weights of size 'n_cell', representing diagonal matrices.
//   cell_to_input_weights             - optional
//   cell_to_cell_weights              - optional
//   cell_to_output_weights            - optional
// Quantized projection weights of size 'n_output * n_cell'
//   projection_weights_ptr            - optional
// Weight scales (scalars) for each of the weights above.
//   input_to_input_weights_scale      - optional
//   input_to_forget_weights_scale
//   input_to_cell_weights_scale
//   input_to_output_weights_scale
//   aux_input_to_input_weights_scale  - optional
//   aux_input_to_forget_weights_scale - optional
//   aux_input_to_cell_weights_scale   - optional
//   aux_input_to_output_weights_scale - optional
//   recurrent_to_input_weights_scale  - optional
//   recurrent_to_forget_weights_scale
//   recurrent_to_cell_weights_scale
//   recurrent_to_output_weights_scale
//   cell_to_input_weights_scale,
//   cell_to_forget_weights_scale,
//   cell_to_output_weights_scale,
//   projection_weights_scale          - optional
// Gate biases of size 'n_cell':
//   input_gate_bias_ptr               - optional
//   forget_gate_bias_ptr
//   cell_gate_bias_ptr
//   output_gate_bias_ptr
//
// Layer norm coefficients of size 'n_cell', representing diagonal matrices.
//   input_layer_norm_coefficients_ptr  - optional
//   forget_layer_norm_coefficients_ptr - optional
//   cell_layer_norm_coefficients_ptr   - optional
//   output_layer_norm_coefficients_ptr - optional
//
// Temporary pre-allocated storage for quantized values:
//   quantized_input_ptr_batch (same size as input_ptr_batch)
//   quantized_output_state_ptr (same size as output_state_ptr)
//   quantized_cell_state_ptr (same size as cell_state_ptr)
// Temporary pre-allocated storage for recovered values:
//   recovered_cell_weights (same size as cell_to_*_weights)
//
// Outputs:
//   output_state_ptr - size 'n_batch * n_output'
//   cell_state_ptr   - size 'n_batch * n_cell'
//   output_ptr_batch - size 'n_batch * output_batch_leading_dim'
inline void LstmStepWithAuxInput(
    const float* input_ptr_batch, const int8_t* input_to_input_weights_ptr,
    float input_to_input_weights_scale,
    const int8_t* input_to_forget_weights_ptr,
    float input_to_forget_weights_scale,
    const int8_t* input_to_cell_weights_ptr, float input_to_cell_weights_scale,
    const int8_t* input_to_output_weights_ptr,
    float input_to_output_weights_scale, const float* aux_input_ptr_batch,
    const int8_t* aux_input_to_input_weights_ptr,
    float aux_input_to_input_weights_scale,
    const int8_t* aux_input_to_forget_weights_ptr,
    float aux_input_to_forget_weights_scale,
    const int8_t* aux_input_to_cell_weights_ptr,
    float aux_input_to_cell_weights_scale,
    const int8_t* aux_input_to_output_weights_ptr,
    float aux_input_to_output_weights_scale,
    const int8_t* recurrent_to_input_weights_ptr,
    float recurrent_to_input_weights_scale,
    const int8_t* recurrent_to_forget_weights_ptr,
    float recurrent_to_forget_weights_scale,
    const int8_t* recurrent_to_cell_weights_ptr,
    float recurrent_to_cell_weights_scale,
    const int8_t* recurrent_to_output_weights_ptr,
    float recurrent_to_output_weights_scale,
    const int8_t* cell_to_input_weights_ptr, float cell_to_input_weights_scale,
    const int8_t* cell_to_forget_weights_ptr,
    float cell_to_forget_weights_scale,
    const int8_t* cell_to_output_weights_ptr,
    float cell_to_output_weights_scale,
    const float* input_layer_norm_coefficients_ptr,
    const float* forget_layer_norm_coefficients_ptr,
    const float* cell_layer_norm_coefficients_ptr,
    const float* output_layer_norm_coefficients_ptr,
    const float* input_gate_bias_ptr, const float* forget_gate_bias_ptr,
    const float* cell_bias_ptr, const float* output_gate_bias_ptr,
    const int8_t* projection_weights_ptr, float projection_weights_scale,
    const float* projection_bias_ptr, const TfLiteLSTMParams* params,
    int n_batch, int n_cell, int n_input, int n_aux_input, int n_output,
    int output_batch_leading_dim, float* input_gate_scratch,
    float* forget_gate_scratch, float* cell_scratch, float* output_gate_scratch,
    float* scaling_factors, float* product_scaling_factors,
    float* recovered_cell_weights, int8_t* quantized_input_ptr_batch,
    int8_t* quantized_aux_input_ptr_batch, int8_t* quantized_output_state_ptr,
    int8_t* quantized_cell_state_ptr, float* output_state_ptr,
    float* cell_state_ptr, float* output_ptr_batch) {
#ifdef GEMMLOWP_PROFILING
  gemmlowp::ScopedProfilingLabel label("LstmStepWithAuxInputHybrid");
#endif
  // Since we have already checked that weights are all there or none, we
  // can check the existence of only one to the get the condition.
  const bool use_cifg = (input_to_input_weights_ptr == nullptr);
  const bool use_peephole = (cell_to_output_weights_ptr != nullptr);
  const bool is_layer_norm_lstm =
      (forget_layer_norm_coefficients_ptr != nullptr);

  // Initialize scratch buffers with bias.
  if (is_layer_norm_lstm) {
    if (!use_cifg) {
      std::fill_n(input_gate_scratch, n_cell * n_batch, 0.0f);
    }
    std::fill_n(forget_gate_scratch, n_cell * n_batch, 0.0f);
    std::fill_n(cell_scratch, n_cell * n_batch, 0.0f);
    std::fill_n(output_gate_scratch, n_cell * n_batch, 0.0f);
  } else {
    if (!use_cifg) {
      tensor_utils::VectorBatchVectorAssign(input_gate_bias_ptr, n_cell,
                                            n_batch, input_gate_scratch);
    }
    tensor_utils::VectorBatchVectorAssign(forget_gate_bias_ptr, n_cell, n_batch,
                                          forget_gate_scratch);
    tensor_utils::VectorBatchVectorAssign(cell_bias_ptr, n_cell, n_batch,
                                          cell_scratch);
    tensor_utils::VectorBatchVectorAssign(output_gate_bias_ptr, n_cell, n_batch,
                                          output_gate_scratch);
  }

  if (!tensor_utils::IsZeroVector(input_ptr_batch, n_batch * n_input)) {
    // Save quantization and matmul computation for all zero input.
    float unused_min, unused_max;
    for (int b = 0; b < n_batch; ++b) {
      const int offset = b * n_input;
      tensor_utils::SymmetricQuantizeFloats(
          input_ptr_batch + offset, n_input, quantized_input_ptr_batch + offset,
          &unused_min, &unused_max, &scaling_factors[b]);
    }
    // For each batch and cell: compute input_weight * input.
    if (!use_cifg) {
      for (int b = 0; b < n_batch; ++b) {
        product_scaling_factors[b] =
            scaling_factors[b] * input_to_input_weights_scale;
      }
      tensor_utils::MatrixBatchVectorMultiplyAccumulate(
          input_to_input_weights_ptr, n_cell, n_input,
          quantized_input_ptr_batch, product_scaling_factors, n_batch,
          input_gate_scratch, /*result_stride=*/1);
    }

    for (int b = 0; b < n_batch; ++b) {
      product_scaling_factors[b] =
          scaling_factors[b] * input_to_forget_weights_scale;
    }
    tensor_utils::MatrixBatchVectorMultiplyAccumulate(
        input_to_forget_weights_ptr, n_cell, n_input, quantized_input_ptr_batch,
        product_scaling_factors, n_batch, forget_gate_scratch,
        /*result_stride=*/1);

    for (int b = 0; b < n_batch; ++b) {
      product_scaling_factors[b] =
          scaling_factors[b] * input_to_cell_weights_scale;
    }
    tensor_utils::MatrixBatchVectorMultiplyAccumulate(
        input_to_cell_weights_ptr, n_cell, n_input, quantized_input_ptr_batch,
        product_scaling_factors, n_batch, cell_scratch, /*result_stride=*/1);

    for (int b = 0; b < n_batch; ++b) {
      product_scaling_factors[b] =
          scaling_factors[b] * input_to_output_weights_scale;
    }
    tensor_utils::MatrixBatchVectorMultiplyAccumulate(
        input_to_output_weights_ptr, n_cell, n_input, quantized_input_ptr_batch,
        product_scaling_factors, n_batch, output_gate_scratch,
        /*result_stride=*/1);
  }

  if (aux_input_ptr_batch != nullptr &&
      !tensor_utils::IsZeroVector(aux_input_ptr_batch, n_batch * n_input)) {
    // Save quantization and matmul computation for all zero input.
    float unused_min, unused_max;
    for (int b = 0; b < n_batch; ++b) {
      const int offset = b * n_input;
      tensor_utils::SymmetricQuantizeFloats(
          aux_input_ptr_batch + offset, n_input,
          quantized_aux_input_ptr_batch + offset, &unused_min, &unused_max,
          &scaling_factors[b]);
    }
    // For each batch and cell: compute input_weight * input.
    if (!use_cifg) {
      for (int b = 0; b < n_batch; ++b) {
        product_scaling_factors[b] =
            scaling_factors[b] * aux_input_to_input_weights_scale;
      }
      tensor_utils::MatrixBatchVectorMultiplyAccumulate(
          aux_input_to_input_weights_ptr, n_cell, n_input,
          quantized_aux_input_ptr_batch, product_scaling_factors, n_batch,
          input_gate_scratch, /*result_stride=*/1);
    }

    for (int b = 0; b < n_batch; ++b) {
      product_scaling_factors[b] =
          scaling_factors[b] * aux_input_to_forget_weights_scale;
    }
    tensor_utils::MatrixBatchVectorMultiplyAccumulate(
        aux_input_to_forget_weights_ptr, n_cell, n_input,
        quantized_aux_input_ptr_batch, product_scaling_factors, n_batch,
        forget_gate_scratch, /*result_stride=*/1);

    for (int b = 0; b < n_batch; ++b) {
      product_scaling_factors[b] =
          scaling_factors[b] * aux_input_to_cell_weights_scale;
    }
    tensor_utils::MatrixBatchVectorMultiplyAccumulate(
        aux_input_to_cell_weights_ptr, n_cell, n_input,
        quantized_aux_input_ptr_batch, product_scaling_factors, n_batch,
        cell_scratch, /*result_stride=*/1);

    for (int b = 0; b < n_batch; ++b) {
      product_scaling_factors[b] =
          scaling_factors[b] * aux_input_to_output_weights_scale;
    }
    tensor_utils::MatrixBatchVectorMultiplyAccumulate(
        aux_input_to_output_weights_ptr, n_cell, n_input,
        quantized_aux_input_ptr_batch, product_scaling_factors, n_batch,
        output_gate_scratch, /*result_stride=*/1);
  }

  if (!tensor_utils::IsZeroVector(output_state_ptr, n_batch * n_output)) {
    // Save quantization and matmul computation for all zero input.
    float unused_min, unused_max;
    for (int b = 0; b < n_batch; ++b) {
      const int offset = b * n_output;
      tensor_utils::SymmetricQuantizeFloats(output_state_ptr + offset, n_output,
                                            quantized_output_state_ptr + offset,
                                            &unused_min, &unused_max,
                                            &scaling_factors[b]);
    }
    // For each batch and cell: compute recurrent_weight * output_state.
    if (!use_cifg) {
      for (int b = 0; b < n_batch; ++b) {
        product_scaling_factors[b] =
            scaling_factors[b] * recurrent_to_input_weights_scale;
      }
      tensor_utils::MatrixBatchVectorMultiplyAccumulate(
          recurrent_to_input_weights_ptr, n_cell, n_output,
          quantized_output_state_ptr, product_scaling_factors, n_batch,
          input_gate_scratch, /*result_stride=*/1);
    }

    for (int b = 0; b < n_batch; ++b) {
      product_scaling_factors[b] =
          scaling_factors[b] * recurrent_to_forget_weights_scale;
    }
    tensor_utils::MatrixBatchVectorMultiplyAccumulate(
        recurrent_to_forget_weights_ptr, n_cell, n_output,
        quantized_output_state_ptr, product_scaling_factors, n_batch,
        forget_gate_scratch, /*result_stride=*/1);

    for (int b = 0; b < n_batch; ++b) {
      product_scaling_factors[b] =
          scaling_factors[b] * recurrent_to_cell_weights_scale;
    }
    tensor_utils::MatrixBatchVectorMultiplyAccumulate(
        recurrent_to_cell_weights_ptr, n_cell, n_output,
        quantized_output_state_ptr, product_scaling_factors, n_batch,
        cell_scratch, /*result_stride=*/1);

    for (int b = 0; b < n_batch; ++b) {
      product_scaling_factors[b] =
          scaling_factors[b] * recurrent_to_output_weights_scale;
    }
    tensor_utils::MatrixBatchVectorMultiplyAccumulate(
        recurrent_to_output_weights_ptr, n_cell, n_output,
        quantized_output_state_ptr, product_scaling_factors, n_batch,
        output_gate_scratch, /*result_stride=*/1);
  }

  // Save quantization and matmul computation for all zero input.
  bool is_cell_state_all_zeros =
      tensor_utils::IsZeroVector(cell_state_ptr, n_batch * n_cell);

  // For each batch and cell: update input gate.
  if (!use_cifg) {
    if (use_peephole && !is_cell_state_all_zeros) {
      tensor_utils::VectorScalarMultiply(cell_to_input_weights_ptr, n_cell,
                                         cell_to_input_weights_scale,
                                         recovered_cell_weights);
      tensor_utils::VectorBatchVectorCwiseProductAccumulate(
          recovered_cell_weights, n_cell, cell_state_ptr, n_batch,
          input_gate_scratch);
    }
    if (is_layer_norm_lstm) {
      tensor_utils::MeanStddevNormalization(input_gate_scratch,
                                            input_gate_scratch, n_cell, n_batch,
                                            kLayerNormEpsilon);
      tensor_utils::VectorBatchVectorCwiseProduct(
          input_layer_norm_coefficients_ptr, n_cell, input_gate_scratch,
          n_batch, input_gate_scratch);
      tensor_utils::VectorBatchVectorAdd(input_gate_bias_ptr, n_cell, n_batch,
                                         input_gate_scratch);
    }
    ApplyActivationsToVector(input_gate_scratch, n_cell * n_batch,
                             kTfLiteActSigmoid, input_gate_scratch);
  }

  // For each batch and cell: update forget gate.
  if (use_peephole && !is_cell_state_all_zeros) {
    tensor_utils::VectorScalarMultiply(cell_to_forget_weights_ptr, n_cell,
                                       cell_to_forget_weights_scale,
                                       recovered_cell_weights);
    tensor_utils::VectorBatchVectorCwiseProductAccumulate(
        recovered_cell_weights, n_cell, cell_state_ptr, n_batch,
        forget_gate_scratch);
  }
  if (is_layer_norm_lstm) {
    tensor_utils::MeanStddevNormalization(forget_gate_scratch,
                                          forget_gate_scratch, n_cell, n_batch,
                                          kLayerNormEpsilon);
    tensor_utils::VectorBatchVectorCwiseProduct(
        forget_layer_norm_coefficients_ptr, n_cell, forget_gate_scratch,
        n_batch, forget_gate_scratch);
    tensor_utils::VectorBatchVectorAdd(forget_gate_bias_ptr, n_cell, n_batch,
                                       forget_gate_scratch);
  }
  ApplyActivationsToVector(forget_gate_scratch, n_cell * n_batch,
                           kTfLiteActSigmoid, forget_gate_scratch);

  // For each batch and cell: update the cell.
  tensor_utils::VectorVectorCwiseProduct(forget_gate_scratch, cell_state_ptr,
                                         n_batch * n_cell, cell_state_ptr);
  if (is_layer_norm_lstm) {
    tensor_utils::MeanStddevNormalization(cell_scratch, cell_scratch, n_cell,
                                          n_batch, kLayerNormEpsilon);
    tensor_utils::VectorBatchVectorCwiseProduct(
        cell_layer_norm_coefficients_ptr, n_cell, cell_scratch, n_batch,
        cell_scratch);
    tensor_utils::VectorBatchVectorAdd(cell_bias_ptr, n_cell, n_batch,
                                       cell_scratch);
  }
  ApplyActivationsToVector(cell_scratch, n_batch * n_cell, params->activation,
                           cell_scratch);
  if (use_cifg) {
    tensor_utils::Sub1Vector(forget_gate_scratch, n_batch * n_cell,
                             forget_gate_scratch);
    tensor_utils::VectorVectorCwiseProductAccumulate(
        cell_scratch, forget_gate_scratch, n_batch * n_cell, cell_state_ptr);
  } else {
    tensor_utils::VectorVectorCwiseProductAccumulate(
        cell_scratch, input_gate_scratch, n_batch * n_cell, cell_state_ptr);
  }
  if (params->cell_clip > 0.0) {
    tensor_utils::ClipVector(cell_state_ptr, n_batch * n_cell,
                             params->cell_clip, cell_state_ptr);
  }

  is_cell_state_all_zeros =
      tensor_utils::IsZeroVector(cell_state_ptr, n_batch * n_cell);
  // For each batch and cell: update the output gate.
  if (use_peephole && !is_cell_state_all_zeros) {
    tensor_utils::VectorScalarMultiply(cell_to_output_weights_ptr, n_cell,
                                       cell_to_output_weights_scale,
                                       recovered_cell_weights);
    tensor_utils::VectorBatchVectorCwiseProductAccumulate(
        recovered_cell_weights, n_cell, cell_state_ptr, n_batch,
        output_gate_scratch);
  }
  if (is_layer_norm_lstm) {
    tensor_utils::MeanStddevNormalization(output_gate_scratch,
                                          output_gate_scratch, n_cell, n_batch,
                                          kLayerNormEpsilon);
    tensor_utils::VectorBatchVectorCwiseProduct(
        output_layer_norm_coefficients_ptr, n_cell, output_gate_scratch,
        n_batch, output_gate_scratch);
    tensor_utils::VectorBatchVectorAdd(output_gate_bias_ptr, n_cell, n_batch,
                                       output_gate_scratch);
  }
  ApplyActivationsToVector(output_gate_scratch, n_batch * n_cell,
                           kTfLiteActSigmoid, output_gate_scratch);
  ApplyActivationsToVector(cell_state_ptr, n_batch * n_cell, params->activation,
                           cell_scratch);
  tensor_utils::VectorVectorCwiseProduct(output_gate_scratch, cell_scratch,
                                         n_batch * n_cell, output_gate_scratch);

  const bool use_projection_weight = (projection_weights_ptr != nullptr);
  const bool use_projection_bias = (projection_bias_ptr != nullptr);

  // For each batch: update the projection and output_state. Note that since
  // the output batch rows may not be contiguous (output_batch_leading_dim !=
  // n_output), we unroll the batched operations where this is the case.
  if (output_batch_leading_dim == n_output) {
    if (use_projection_weight) {
      if (use_projection_bias) {
        tensor_utils::VectorBatchVectorAssign(projection_bias_ptr, n_output,
                                              n_batch, output_ptr_batch);
      } else {
        std::fill_n(output_ptr_batch, n_batch * n_output, 0.0f);
      }
      if (!tensor_utils::IsZeroVector(output_gate_scratch, n_batch * n_cell)) {
        // Save quantization and matmul computation for all zero input.
        float unused_min, unused_max;
        for (int b = 0; b < n_batch; ++b) {
          const int offset = b * n_cell;
          tensor_utils::SymmetricQuantizeFloats(
              output_gate_scratch + offset, n_cell,
              quantized_cell_state_ptr + offset, &unused_min, &unused_max,
              &scaling_factors[b]);
        }
        for (int b = 0; b < n_batch; ++b) {
          product_scaling_factors[b] =
              scaling_factors[b] * projection_weights_scale;
        }
        tensor_utils::MatrixBatchVectorMultiplyAccumulate(
            projection_weights_ptr, n_output, n_cell, quantized_cell_state_ptr,
            product_scaling_factors, n_batch, output_ptr_batch,
            /*result_stride=*/1);
      }
      if (params->proj_clip > 0.0) {
        tensor_utils::ClipVector(output_ptr_batch, n_batch * n_output,
                                 params->proj_clip, output_ptr_batch);
      }
    } else {
      std::copy_n(output_gate_scratch, n_batch * n_output, output_ptr_batch);
    }
    std::copy_n(output_ptr_batch, n_batch * n_output, output_state_ptr);
  } else {
    if (use_projection_weight) {
      if (use_projection_bias) {
        for (int k = 0; k < n_batch; k++) {
          std::copy_n(projection_bias_ptr, n_output,
                      output_ptr_batch + k * output_batch_leading_dim);
        }
      } else {
        for (int k = 0; k < n_batch; k++) {
          std::fill_n(output_ptr_batch + k * output_batch_leading_dim, n_output,
                      0.0f);
        }
      }
      if (!tensor_utils::IsZeroVector(output_gate_scratch, n_batch * n_cell)) {
        // Save quantization and matmul computation for all zero input.
        float unused_min, unused_max;
        for (int b = 0; b < n_batch; ++b) {
          const int offset = b * n_cell;
          tensor_utils::SymmetricQuantizeFloats(
              output_gate_scratch + offset, n_cell,
              quantized_cell_state_ptr + offset, &unused_min, &unused_max,
              &scaling_factors[b]);
        }
        for (int b = 0; b < n_batch; ++b) {
          product_scaling_factors[b] =
              scaling_factors[b] * projection_weights_scale;
        }
        for (int k = 0; k < n_batch; k++) {
          tensor_utils::MatrixBatchVectorMultiplyAccumulate(
              projection_weights_ptr, n_output, n_cell,
              quantized_cell_state_ptr + k * n_cell,
              &product_scaling_factors[k],
              /*n_batch=*/1, output_ptr_batch + k * output_batch_leading_dim,
              /*result_stride=*/1);
        }
      }
      if (params->proj_clip > 0.0) {
        for (int k = 0; k < n_batch; k++) {
          tensor_utils::ClipVector(
              output_ptr_batch + k * output_batch_leading_dim, n_output,
              params->proj_clip,
              output_ptr_batch + k * output_batch_leading_dim);
        }
      }
    } else {
      for (int k = 0; k < n_batch; k++) {
        std::copy_n(output_gate_scratch + k * n_output, n_output,
                    output_ptr_batch + k * output_batch_leading_dim);
      }
    }
    for (int k = 0; k < n_batch; k++) {
      std::copy_n(output_ptr_batch + k * output_batch_leading_dim, n_output,
                  output_state_ptr + k * n_output);
    }
  }
}

// Fully quantized lstm kernel. Currently supports both cifg and non-cifg.
//
// Input activatoin of size n_batch * n_input:
//   input_ptr
//
// LSTM weights:
// Quantized input weights of size 'n_cell * n_input':
//   input_to_input_weight_ptr            - optional
//   input_to_forget_weight_ptr           - optional
//   input_to_cell_weight_ptr             - optional
//   input_to_output_weight_ptr           - optional
//
// Quantized recurrent weights of size 'n_cell * n_output':
//   recurrent_to_input_weight_ptr        - optional
//   recurrent_to_forget_weights_ptr
//   recurrent_to_cell_weights_ptr
//   recurrent_to_input_weights_ptr
//
// Quantized peephole weights of size 'n_cell', representing diagonal matrices.
//   cell_to_input_weights               - optional
//   cell_to_cell_weights                - optional
//   cell_to_output_weights              - optional
//
// Quantized projection weights of size 'n_output * n_cell'
//   proj_weight_ptr                     - optional
//
// Weight scales (scalars) for each of the weights above.
//   effective_input_to_input_scale_a    - optional
//   effective_input_to_input_scale_b    - optional
//   effective_input_to_forget_scale_a
//   effective_input_to_forget_scale_b
//   effective_input_to_cell_scale_a
//   effective_input_to_cell_scale_b
//   effective_input_to_output_scale_a
//   effective_input_to_output_scale_b
//   effective_recurrent_to_input_scale_a    - optional
//   effective_recurrent_to_input_scale_b    - optional
//   effective_recurrent_to_forget_scale_a
//   effective_recurrent_to_forget_scale_b
//   effective_recurrent_to_cell_scale_a
//   effective_recurrent_to_cell_scale_b
//   effective_recurrent_to_output_scale_a
//   effective_recurrent_to_output_scale_b
//   effective_proj_scale_a                  - optional
//   effective_proj_scale_b                  - optional
//
// Gate biases of size 'n_cell':
//   input_bias_ptr                 - optional
//   forget_bias_ptr
//   cell_bias_ptr
//   output_bias_ptr
//
// Layer norm coefficients of size 'n_cell', representing diagonal matrices.
//   layer_norm_input_weight_ptr    - optional
//   layer_norm_forput_weight_ptr   - optional
//   layer_norm_cell_weight_ptr     - optional
//   layer_norm_output_weight_ptr   - optional
//
// Layer norm scales of size 'n_cell'.
//   layer_norm_input_scale_a     - optional
//   layer_norm_input_scale_b     - optional
//   layer_norm_forget_scale_a    - optional
//   layer_norm_forget_scale_b    - optional
//   layer_norm_cell_scale_a      - optional
//   layer_norm_cell_scale_b      - optional
//   layer_norm_output_scale_a    - optional
//   layer_norm_output_scale_b    - optional
//
// Scalar values:
//   quantized_cell_clip: quantized clip value for cell.
//   quantized_proj_clip: quantized clip value for projection.
//   cell_scale: the POT scale for cell state.
//
// Zero points:
//   activation_zp: zero point of activation
//   hidden_zp: zero point for hidden state.
//
// Temporary pre-allocated storage for the calculation. Each is of size n_cell *
// n_batch.
//   scratch_0:
//   scratch_1
//   scratch_2
//   scratch_3
//   scratch_4
//   scratch_5: this scratch buffer is created purely for optimizing the
//              MatrixBatchVectorMultiplyAccumulate.
//
// Outputs:
//   output_state_ptr - size 'n_batch * n_output'
//   cell_state_ptr   - size 'n_batch * n_cell'
//   output_ptr       - size 'n_batch * n_output'
inline void LstmStepQuantized(
    const int8_t* input_ptr, const int8_t* input_to_input_weight_ptr,
    int32_t effective_input_to_input_scale_a,
    int32_t effective_input_to_input_scale_b,
    const int8_t* input_to_forget_weight_ptr,
    int32_t effective_input_to_forget_scale_a,
    int32_t effective_input_to_forget_scale_b,
    const int8_t* input_to_cell_weight_ptr,
    int32_t effective_input_to_cell_scale_a,
    int32_t effective_input_to_cell_scale_b,
    const int8_t* input_to_output_weight_ptr,
    int32_t effective_input_to_output_scale_a,
    int32_t effective_input_to_output_scale_b,
    const int8_t* recurrent_to_input_weight_ptr,
    int32_t effective_recurrent_to_input_scale_a,
    int32_t effective_recurrent_to_input_scale_b,
    const int8_t* recurrent_to_forget_weight_ptr,
    int32_t effective_recurrent_to_forget_scale_a,
    int32_t effective_recurrent_to_forget_scale_b,
    const int8_t* recurrent_to_cell_weight_ptr,
    int32_t effective_recurrent_to_cell_scale_a,
    int32_t effective_recurrent_to_cell_scale_b,
    const int8_t* recurrent_to_output_weight_ptr,
    int32_t effective_recurrent_to_output_scale_a,
    int32_t effective_recurrent_to_output_scale_b,
    const int8_t* cell_to_input_weight_ptr,
    int32_t effective_cell_to_input_scale_a,
    int32_t effective_cell_to_input_scale_b,
    const int8_t* cell_to_forget_weight_ptr,
    int32_t effective_cell_to_forget_scale_a,
    int32_t effective_cell_to_forget_scale_b,
    const int8_t* cell_to_output_weight_ptr,
    int32_t effective_cell_to_output_scale_a,
    int32_t effective_cell_to_output_scale_b, const int8_t* proj_weight_ptr,
    int32_t effective_proj_scale_a, int32_t effective_proj_scale_b,
    int32_t hidden_zp, int32_t effective_hidden_scale_a,
    int32_t effective_hidden_scale_b,
    const int16_t* layer_norm_input_weight_ptr,
    int32_t layer_norm_input_scale_a, int32_t layer_norm_input_scale_b,
    const int16_t* layer_norm_forget_weight_ptr,
    int32_t layer_norm_forget_scale_a, int32_t layer_norm_forget_scale_b,
    const int16_t* layer_norm_cell_weight_ptr, int32_t layer_norm_cell_scale_a,
    int32_t layer_norm_cell_scale_b,
    const int16_t* layer_norm_output_weight_ptr,
    int32_t layer_norm_output_scale_a, int32_t layer_norm_output_scale_b,
    const int32_t* input_bias_ptr, const int32_t* forget_bias_ptr,
    const int32_t* cell_bias_ptr, const int32_t* output_bias_ptr,
    int16_t quantized_cell_clip, int8_t quantized_proj_clip, int32_t cell_scale,
    const int32_t* inv_large_value,
    const int32_t* input_to_forget_effective_bias,
    const int32_t* recurrent_to_forget_effective_bias,
    const int32_t* input_to_cell_effective_bias,
    const int32_t* recurrent_to_cell_effective_bias,
    const int32_t* input_to_output_effective_bias,
    const int32_t* recurrent_to_output_effective_bias,
    const int32_t* input_to_input_effective_bias,
    const int32_t* recurrent_to_input_effective_bias,
    const int32_t* projection_effective_bias, int32 n_batch, int32 n_cell,
    int32 n_input, int32 n_output, int32 output_batch_leading_dim,
    int8_t* activation_ptr, int32_t activation_zp, int16_t* cell_ptr,
    int8_t* output_ptr, int16_t* scratch_0_ptr, int16_t* scratch_1_ptr,
    int16_t* scratch_2_ptr, int16_t* scratch_3_ptr, int8_t* scratch_4_ptr,
    int32_t* scratch_5_ptr, CpuBackendContext* context) {
  // Get hyper parameters.
  const bool use_cifg = (input_to_input_weight_ptr == nullptr);

  // Check for nullptrs.
  TFLITE_DCHECK(input_to_forget_effective_bias);
  TFLITE_DCHECK(recurrent_to_forget_effective_bias);
  TFLITE_DCHECK(input_to_cell_effective_bias);
  TFLITE_DCHECK(recurrent_to_cell_effective_bias);
  TFLITE_DCHECK(input_to_output_effective_bias);
  TFLITE_DCHECK(recurrent_to_output_effective_bias);
  if (!use_cifg) {
    TFLITE_DCHECK(input_to_input_effective_bias);
    TFLITE_DCHECK(recurrent_to_input_effective_bias);
  }
  TFLITE_DCHECK(projection_effective_bias);

  // TODO(renjieliu): Handle optional arguments processing here:
  // case not peephole: cell_to_forget_weight should be nullptr
  //                    cell_to_output_weight should be nullptr
  //                    cifg: cell_to_input_weight should be nullptr

  // Set scratch to 0.
  if (!use_cifg) {
    memset(scratch_0_ptr, 0, n_batch * n_cell * sizeof(int16_t));
  }
  memset(scratch_1_ptr, 0, n_batch * n_cell * sizeof(int16_t));
  memset(scratch_2_ptr, 0, n_batch * n_cell * sizeof(int16_t));
  memset(scratch_3_ptr, 0, n_batch * n_cell * sizeof(int16_t));

  // Forget gate.
  tensor_utils::MatrixBatchVectorMultiplyAccumulate(
      input_ptr, input_to_forget_effective_bias, input_to_forget_weight_ptr,
      effective_input_to_forget_scale_a, effective_input_to_forget_scale_b,
      n_batch, n_input, n_cell, 0, scratch_5_ptr, scratch_1_ptr, context);

  tensor_utils::MatrixBatchVectorMultiplyAccumulate(
      activation_ptr, recurrent_to_forget_effective_bias,
      recurrent_to_forget_weight_ptr, effective_recurrent_to_forget_scale_a,
      effective_recurrent_to_forget_scale_b, n_batch, n_output, n_cell, 0,
      scratch_5_ptr, scratch_1_ptr, context);

  if (layer_norm_forget_weight_ptr != nullptr) {
    tensor_utils::ApplyLayerNorm(scratch_1_ptr, layer_norm_forget_weight_ptr,
                                 forget_bias_ptr, layer_norm_forget_scale_a,
                                 layer_norm_forget_scale_b, inv_large_value[1],
                                 n_batch, n_cell, scratch_1_ptr);
  }

  tensor_utils::ApplySigmoid(scratch_1_ptr, n_batch, n_cell, scratch_1_ptr);

  // Modulation gate.
  tensor_utils::MatrixBatchVectorMultiplyAccumulate(
      input_ptr, input_to_cell_effective_bias, input_to_cell_weight_ptr,
      effective_input_to_cell_scale_a, effective_input_to_cell_scale_b, n_batch,
      n_input, n_cell, 0, scratch_5_ptr, scratch_2_ptr, context);

  tensor_utils::MatrixBatchVectorMultiplyAccumulate(
      activation_ptr, recurrent_to_cell_effective_bias,
      recurrent_to_cell_weight_ptr, effective_recurrent_to_cell_scale_a,
      effective_recurrent_to_cell_scale_b, n_batch, n_output, n_cell, 0,
      scratch_5_ptr, scratch_2_ptr, context);

  if (layer_norm_cell_weight_ptr != nullptr) {
    tensor_utils::ApplyLayerNorm(scratch_2_ptr, layer_norm_cell_weight_ptr,
                                 cell_bias_ptr, layer_norm_cell_scale_a,
                                 layer_norm_cell_scale_b, inv_large_value[2],
                                 n_batch, n_cell, scratch_2_ptr);
  }

  tensor_utils::ApplyTanh3(scratch_2_ptr, n_batch, n_cell, scratch_2_ptr);

  // Ouptut gate.
  tensor_utils::MatrixBatchVectorMultiplyAccumulate(
      input_ptr, input_to_output_effective_bias, input_to_output_weight_ptr,
      effective_input_to_output_scale_a, effective_input_to_output_scale_b,
      n_batch, n_input, n_cell, 0, scratch_5_ptr, scratch_3_ptr, context);

  tensor_utils::MatrixBatchVectorMultiplyAccumulate(
      activation_ptr, recurrent_to_output_effective_bias,
      recurrent_to_output_weight_ptr, effective_recurrent_to_output_scale_a,
      effective_recurrent_to_output_scale_b, n_batch, n_output, n_cell, 0,
      scratch_5_ptr, scratch_3_ptr, context);

  if (layer_norm_output_weight_ptr != nullptr) {
    tensor_utils::ApplyLayerNorm(scratch_3_ptr, layer_norm_output_weight_ptr,
                                 output_bias_ptr, layer_norm_output_scale_a,
                                 layer_norm_output_scale_b, inv_large_value[3],
                                 n_batch, n_cell, scratch_3_ptr);
  }

  tensor_utils::ApplySigmoid(scratch_3_ptr, n_batch, n_cell, scratch_3_ptr);

  // Input gate.
  if (use_cifg) {
    tensor_utils::Sub1Vector(scratch_1_ptr, n_batch * n_cell, scratch_0_ptr);
  } else {
    tensor_utils::MatrixBatchVectorMultiplyAccumulate(
        input_ptr, input_to_input_effective_bias, input_to_input_weight_ptr,
        effective_input_to_input_scale_a, effective_input_to_input_scale_b,
        n_batch, n_input, n_cell, 0, scratch_5_ptr, scratch_0_ptr, context);

    tensor_utils::MatrixBatchVectorMultiplyAccumulate(
        activation_ptr, recurrent_to_input_effective_bias,
        recurrent_to_input_weight_ptr, effective_recurrent_to_input_scale_a,
        effective_recurrent_to_input_scale_b, n_batch, n_output, n_cell, 0,
        scratch_5_ptr, scratch_0_ptr, context);

    if (layer_norm_input_weight_ptr != nullptr) {
      tensor_utils::ApplyLayerNorm(scratch_0_ptr, layer_norm_input_weight_ptr,
                                   input_bias_ptr, layer_norm_input_scale_a,
                                   layer_norm_input_scale_b, inv_large_value[0],
                                   n_batch, n_cell, scratch_0_ptr);
    }

    tensor_utils::ApplySigmoid(scratch_0_ptr, n_batch, n_cell, scratch_0_ptr);
  }

  // Cell and hidden.
  tensor_utils::CwiseMul(scratch_1_ptr, cell_ptr, n_batch, n_cell, 15,
                         scratch_1_ptr);

  tensor_utils::CwiseMul(scratch_0_ptr, scratch_2_ptr, n_batch, n_cell,
                         30 + cell_scale, scratch_2_ptr);

  tensor_utils::CwiseAdd(scratch_1_ptr, scratch_2_ptr, n_batch, n_cell,
                         cell_ptr);

  if (quantized_cell_clip > 0) {
    tensor_utils::CwiseClipping(cell_ptr, quantized_cell_clip, n_batch, n_cell);
  }

  // TODO(jianlijianli): swtich to a tempalte.
  if (cell_scale == -11) {
    tensor_utils::ApplyTanh4(cell_ptr, n_batch, n_cell, scratch_0_ptr);
  } else if (cell_scale == -15) {
    tensor_utils::ApplyTanh0(cell_ptr, n_batch, n_cell, scratch_0_ptr);
  }

  tensor_utils::CwiseMul(scratch_3_ptr, scratch_0_ptr, effective_hidden_scale_a,
                         effective_hidden_scale_b, n_batch, n_cell, hidden_zp,
                         scratch_4_ptr);

  // Projection.
  if (proj_weight_ptr != nullptr) {
    memset(output_ptr, 0, n_batch * n_output * sizeof(int8_t));
    tensor_utils::MatrixBatchVectorMultiplyAccumulate(
        scratch_4_ptr, projection_effective_bias, proj_weight_ptr,
        effective_proj_scale_a, effective_proj_scale_b, n_batch, n_cell,
        n_output, activation_zp, scratch_5_ptr, output_ptr, context);
  }

  if (quantized_proj_clip > 0) {
    tensor_utils::CwiseClipping(output_ptr, quantized_proj_clip, n_batch,
                                n_output);
  }

  memcpy(activation_ptr, output_ptr, n_batch * n_output * sizeof(int8_t));
}

}  // namespace

TfLiteStatus EvalFloat(
    const TfLiteTensor* input, const TfLiteTensor* input_to_input_weights,
    const TfLiteTensor* input_to_forget_weights,
    const TfLiteTensor* input_to_cell_weights,
    const TfLiteTensor* input_to_output_weights,
    const TfLiteTensor* recurrent_to_input_weights,
    const TfLiteTensor* recurrent_to_forget_weights,
    const TfLiteTensor* recurrent_to_cell_weights,
    const TfLiteTensor* recurrent_to_output_weights,
    const TfLiteTensor* cell_to_input_weights,
    const TfLiteTensor* cell_to_forget_weights,
    const TfLiteTensor* cell_to_output_weights,
    const TfLiteTensor* input_layer_norm_coefficients,
    const TfLiteTensor* forget_layer_norm_coefficients,
    const TfLiteTensor* cell_layer_norm_coefficients,
    const TfLiteTensor* output_layer_norm_coefficients,
    const TfLiteTensor* aux_input,
    const TfLiteTensor* aux_input_to_input_weights,
    const TfLiteTensor* aux_input_to_forget_weights,
    const TfLiteTensor* aux_input_to_cell_weights,
    const TfLiteTensor* aux_input_to_output_weights,
    const TfLiteTensor* input_gate_bias, const TfLiteTensor* forget_gate_bias,
    const TfLiteTensor* cell_bias, const TfLiteTensor* output_gate_bias,
    const TfLiteTensor* projection_weights, const TfLiteTensor* projection_bias,
    const TfLiteLSTMParams* params, bool forward_sequence, bool time_major,
    int output_offset, TfLiteTensor* scratch_buffer,
    TfLiteTensor* activation_state, TfLiteTensor* cell_state,
    TfLiteTensor* output) {
  TF_LITE_ASSERT(input->dims->size >= 2 && input->dims->size <= 3);
  int max_time, n_batch;
  if (input->dims->size == 3) {
    max_time = (time_major) ? input->dims->data[0] : input->dims->data[1];
    n_batch = (time_major) ? input->dims->data[1] : input->dims->data[0];
  } else {
    max_time = 1;
    n_batch = input->dims->data[0];
  }
  const int n_input = input->dims->data[input->dims->size - 1];
  const int aux_input_size =
      (aux_input) ? aux_input->dims->data[aux_input->dims->size - 1] : 0;

  // n_cell and n_output will be the same size when there is no projection.
  const int n_cell = input_to_output_weights->dims->data[0];
  const int n_output = recurrent_to_output_weights->dims->data[1];

  // Since we have already checked that weights are all there or none, we can
  // check the existence of only one to the get the condition.
  const bool use_cifg = (input_to_input_weights == nullptr);
  const bool use_peephole = (cell_to_output_weights != nullptr);
  const bool is_layer_norm_lstm = (forget_layer_norm_coefficients != nullptr);

  // Index the scratch buffers pointers to the global scratch buffer.
  float* input_gate_scratch = nullptr;
  float* cell_scratch = nullptr;
  float* forget_gate_scratch = nullptr;
  float* output_gate_scratch = nullptr;
  if (use_cifg) {
    cell_scratch = GetTensorData<float>(scratch_buffer);
    forget_gate_scratch =
        GetTensorData<float>(scratch_buffer) + n_cell * n_batch;
    output_gate_scratch =
        GetTensorData<float>(scratch_buffer) + 2 * n_cell * n_batch;
  } else {
    input_gate_scratch = GetTensorData<float>(scratch_buffer);
    cell_scratch = GetTensorData<float>(scratch_buffer) + n_cell * n_batch;
    forget_gate_scratch =
        GetTensorData<float>(scratch_buffer) + 2 * n_cell * n_batch;
    output_gate_scratch =
        GetTensorData<float>(scratch_buffer) + 3 * n_cell * n_batch;
  }

  // Check optional tensors, the respective pointers can be null.
  const float* input_to_input_weights_ptr =
      (use_cifg) ? nullptr : GetTensorData<float>(input_to_input_weights);
  const float* recurrent_to_input_weights_ptr =
      (use_cifg) ? nullptr : GetTensorData<float>(recurrent_to_input_weights);
  const float* input_gate_bias_ptr =
      (use_cifg) ? nullptr : GetTensorData<float>(input_gate_bias);
  const float* cell_to_input_weights_ptr =
      (use_peephole && !use_cifg) ? GetTensorData<float>(cell_to_input_weights)
                                  : nullptr;
  const float* cell_to_forget_weights_ptr =
      (use_peephole) ? GetTensorData<float>(cell_to_forget_weights) : nullptr;
  const float* cell_to_output_weights_ptr =
      (use_peephole) ? GetTensorData<float>(cell_to_output_weights) : nullptr;
  const float* input_layer_norm_coefficients_ptr =
      (is_layer_norm_lstm && !use_cifg)
          ? GetTensorData<float>(input_layer_norm_coefficients)
          : nullptr;
  const float* forget_layer_norm_coefficients_ptr =
      is_layer_norm_lstm ? GetTensorData<float>(forget_layer_norm_coefficients)
                         : nullptr;
  const float* cell_layer_norm_coefficients_ptr =
      is_layer_norm_lstm ? GetTensorData<float>(cell_layer_norm_coefficients)
                         : nullptr;
  const float* output_layer_norm_coefficients_ptr =
      is_layer_norm_lstm ? GetTensorData<float>(output_layer_norm_coefficients)
                         : nullptr;
  const float* projection_weights_ptr =
      (projection_weights == nullptr)
          ? nullptr
          : GetTensorData<float>(projection_weights);
  const float* projection_bias_ptr =
      (projection_bias == nullptr) ? nullptr
                                   : GetTensorData<float>(projection_bias);

  const float* aux_input_ptr = nullptr;
  const float* aux_input_to_input_weights_ptr = nullptr;
  const float* aux_input_to_forget_weights_ptr = nullptr;
  const float* aux_input_to_cell_weights_ptr = nullptr;
  const float* aux_input_to_output_weights_ptr = nullptr;
  if (aux_input_size > 0) {
    if (!use_cifg) {
      aux_input_to_input_weights_ptr =
          GetTensorData<float>(aux_input_to_input_weights);
    }
    aux_input_to_forget_weights_ptr =
        GetTensorData<float>(aux_input_to_forget_weights);
    aux_input_to_cell_weights_ptr =
        GetTensorData<float>(aux_input_to_cell_weights);
    aux_input_to_output_weights_ptr =
        GetTensorData<float>(aux_input_to_output_weights);
  }

  const int output_batch_leading_dim =
      output->dims->data[output->dims->size - 1];
  if (time_major) {
    // Loop through the sequence.
    const int input_step = n_batch * n_input;
    const int output_step = n_batch * output_batch_leading_dim;
    for (int t = 0; t < max_time; t++) {
      // If this is the forward_sequence, step forward, otherwise step
      // backwards.
      const int t_rel = forward_sequence ? t : max_time - t - 1;
      const float* input_ptr_batch =
          GetTensorData<float>(input) + t_rel * input_step;
      if (aux_input) {
        aux_input_ptr = GetTensorData<float>(aux_input) + t_rel * input_step;
      }
      float* output_ptr_time =
          GetTensorData<float>(output) + t_rel * output_step + output_offset;

      LstmStepWithAuxInput(
          input_ptr_batch, input_to_input_weights_ptr,
          GetTensorData<float>(input_to_forget_weights),
          GetTensorData<float>(input_to_cell_weights),
          GetTensorData<float>(input_to_output_weights), aux_input_ptr,
          aux_input_to_input_weights_ptr, aux_input_to_forget_weights_ptr,
          aux_input_to_cell_weights_ptr, aux_input_to_output_weights_ptr,
          recurrent_to_input_weights_ptr,
          GetTensorData<float>(recurrent_to_forget_weights),
          GetTensorData<float>(recurrent_to_cell_weights),
          GetTensorData<float>(recurrent_to_output_weights),
          cell_to_input_weights_ptr, cell_to_forget_weights_ptr,
          cell_to_output_weights_ptr, input_layer_norm_coefficients_ptr,
          forget_layer_norm_coefficients_ptr, cell_layer_norm_coefficients_ptr,
          output_layer_norm_coefficients_ptr, input_gate_bias_ptr,
          GetTensorData<float>(forget_gate_bias),
          GetTensorData<float>(cell_bias),
          GetTensorData<float>(output_gate_bias), projection_weights_ptr,
          projection_bias_ptr, params, n_batch, n_cell, n_input, aux_input_size,
          n_output, output_batch_leading_dim,
          GetTensorData<float>(activation_state),
          GetTensorData<float>(cell_state), input_gate_scratch,
          forget_gate_scratch, cell_scratch, output_gate_scratch,
          output_ptr_time);
    }
  } else {
    for (int b = 0; b < n_batch; b++) {
      const int input_step = n_input;
      const int output_step = output_batch_leading_dim;
      for (int t = 0; t < max_time; t++) {
        // If this is the forward_sequence, step forward, otherwise step
        // backwards.
        const int t_rel = forward_sequence ? t : max_time - t - 1;
        const int time_offset = b * max_time + t_rel;
        const float* input_ptr =
            GetTensorData<float>(input) + time_offset * input_step;
        if (aux_input) {
          aux_input_ptr =
              GetTensorData<float>(aux_input) + time_offset * input_step;
        }
        float* output_ptr = GetTensorData<float>(output) +
                            time_offset * output_step + output_offset;

        // Offset the {activation,cell}_state pointers to the right batch.
        float* activation_state_ptr = GetTensorData<float>(activation_state) +
                                      b * output_batch_leading_dim;
        float* cell_state_ptr = GetTensorData<float>(cell_state) + b * n_cell;
        // Offset the scratch pointers to the right batch.
        float* input_gate_scratch_ptr =
            input_gate_scratch ? input_gate_scratch + b * n_cell : nullptr;
        float* forget_gate_scratch_ptr = forget_gate_scratch + b * n_cell;
        float* cell_scratch_ptr = cell_scratch + b * n_cell;
        float* output_gate_scratch_ptr = output_gate_scratch + b * n_cell;

        LstmStepWithAuxInput(
            input_ptr, input_to_input_weights_ptr,
            GetTensorData<float>(input_to_forget_weights),
            GetTensorData<float>(input_to_cell_weights),
            GetTensorData<float>(input_to_output_weights), aux_input_ptr,
            aux_input_to_input_weights_ptr, aux_input_to_forget_weights_ptr,
            aux_input_to_cell_weights_ptr, aux_input_to_output_weights_ptr,
            recurrent_to_input_weights_ptr,
            GetTensorData<float>(recurrent_to_forget_weights),
            GetTensorData<float>(recurrent_to_cell_weights),
            GetTensorData<float>(recurrent_to_output_weights),
            cell_to_input_weights_ptr, cell_to_forget_weights_ptr,
            cell_to_output_weights_ptr, input_layer_norm_coefficients_ptr,
            forget_layer_norm_coefficients_ptr,
            cell_layer_norm_coefficients_ptr,
            output_layer_norm_coefficients_ptr, input_gate_bias_ptr,
            GetTensorData<float>(forget_gate_bias),
            GetTensorData<float>(cell_bias),
            GetTensorData<float>(output_gate_bias), projection_weights_ptr,
            projection_bias_ptr, params, /*n_batch=*/1, n_cell, n_input,
            aux_input_size, n_output, output_batch_leading_dim,
            activation_state_ptr, cell_state_ptr, input_gate_scratch_ptr,
            forget_gate_scratch_ptr, cell_scratch_ptr, output_gate_scratch_ptr,
            output_ptr);
      }
    }
  }
  return kTfLiteOk;
}

TfLiteStatus EvalHybrid(
    const TfLiteTensor* input, const TfLiteTensor* input_to_input_weights,
    const TfLiteTensor* input_to_forget_weights,
    const TfLiteTensor* input_to_cell_weights,
    const TfLiteTensor* input_to_output_weights,
    const TfLiteTensor* recurrent_to_input_weights,
    const TfLiteTensor* recurrent_to_forget_weights,
    const TfLiteTensor* recurrent_to_cell_weights,
    const TfLiteTensor* recurrent_to_output_weights,
    const TfLiteTensor* cell_to_input_weights,
    const TfLiteTensor* cell_to_forget_weights,
    const TfLiteTensor* cell_to_output_weights,
    const TfLiteTensor* input_layer_norm_coefficients,
    const TfLiteTensor* forget_layer_norm_coefficients,
    const TfLiteTensor* cell_layer_norm_coefficients,
    const TfLiteTensor* output_layer_norm_coefficients,
    const TfLiteTensor* aux_input,
    const TfLiteTensor* aux_input_to_input_weights,
    const TfLiteTensor* aux_input_to_forget_weights,
    const TfLiteTensor* aux_input_to_cell_weights,
    const TfLiteTensor* aux_input_to_output_weights,
    const TfLiteTensor* input_gate_bias, const TfLiteTensor* forget_gate_bias,
    const TfLiteTensor* cell_bias, const TfLiteTensor* output_gate_bias,
    const TfLiteTensor* projection_weights, const TfLiteTensor* projection_bias,
    const TfLiteLSTMParams* params, bool forward_sequence, bool time_major,
    int output_offset, TfLiteTensor* scratch_buffer,
    TfLiteTensor* scaling_factors, TfLiteTensor* prod_scaling_factors,
    TfLiteTensor* recovered_cell_weights, TfLiteTensor* input_quantized,
    TfLiteTensor* aux_input_quantized, TfLiteTensor* output_state_quantized,
    TfLiteTensor* cell_state_quantized, TfLiteTensor* output_state,
    TfLiteTensor* cell_state, TfLiteTensor* output) {
  TF_LITE_ASSERT(input->dims->size >= 2 && input->dims->size <= 3);
  const int n_input = input->dims->data[input->dims->size - 1];
  int max_time, n_batch;
  if (input->dims->size == 2) {
    max_time = 1;
    n_batch = input->dims->data[0];
  } else {
    max_time = (time_major) ? input->dims->data[0] : input->dims->data[1];
    n_batch = (time_major) ? input->dims->data[1] : input->dims->data[0];
  }
  const int aux_input_size =
      (aux_input) ? aux_input->dims->data[aux_input->dims->size - 1] : 0;
  // n_cell and n_output will be the same size when there is no projection.
  const int n_cell = input_to_output_weights->dims->data[0];
  const int n_output = recurrent_to_output_weights->dims->data[1];

  // Since we have already checked that weights are all there or none, we can
  // check the existence of only one to get the condition.
  const bool use_cifg = (input_to_input_weights == nullptr);
  const bool use_peephole = (cell_to_output_weights != nullptr);
  const bool is_layer_norm_lstm = (forget_layer_norm_coefficients != nullptr);

  float* input_gate_scratch = nullptr;
  float* cell_scratch = nullptr;
  float* forget_gate_scratch = nullptr;
  float* output_gate_scratch = nullptr;
  if (use_cifg) {
    cell_scratch = GetTensorData<float>(scratch_buffer);
    forget_gate_scratch =
        GetTensorData<float>(scratch_buffer) + n_cell * n_batch;
    output_gate_scratch =
        GetTensorData<float>(scratch_buffer) + 2 * n_cell * n_batch;
  } else {
    input_gate_scratch = GetTensorData<float>(scratch_buffer);
    cell_scratch = GetTensorData<float>(scratch_buffer) + n_cell * n_batch;
    forget_gate_scratch =
        GetTensorData<float>(scratch_buffer) + 2 * n_cell * n_batch;
    output_gate_scratch =
        GetTensorData<float>(scratch_buffer) + 3 * n_cell * n_batch;
  }

  // Check optional tensors, the respective pointers can be null.
  const int8_t* input_to_input_weights_ptr = nullptr;
  float input_to_input_weights_scale = 1.0f;
  const int8_t* recurrent_to_input_weights_ptr = nullptr;
  float recurrent_to_input_weights_scale = 1.0f;
  const float* input_gate_bias_ptr = nullptr;
  if (!use_cifg) {
    input_to_input_weights_ptr = GetTensorData<int8_t>(input_to_input_weights);
    recurrent_to_input_weights_ptr =
        GetTensorData<int8_t>(recurrent_to_input_weights);
    input_gate_bias_ptr = GetTensorData<float>(input_gate_bias);
    input_to_input_weights_scale = input_to_input_weights->params.scale;
    recurrent_to_input_weights_scale = recurrent_to_input_weights->params.scale;
  }

  const int8_t* cell_to_input_weights_ptr = nullptr;
  const int8_t* cell_to_forget_weights_ptr = nullptr;
  const int8_t* cell_to_output_weights_ptr = nullptr;
  float cell_to_input_weights_scale = 1.0f;
  float cell_to_forget_weights_scale = 1.0f;
  float cell_to_output_weights_scale = 1.0f;
  if (use_peephole) {
    if (!use_cifg) {
      cell_to_input_weights_ptr = GetTensorData<int8_t>(cell_to_input_weights);
      cell_to_input_weights_scale = cell_to_input_weights->params.scale;
    }
    cell_to_forget_weights_ptr = GetTensorData<int8_t>(cell_to_forget_weights);
    cell_to_output_weights_ptr = GetTensorData<int8_t>(cell_to_output_weights);
    cell_to_forget_weights_scale = cell_to_forget_weights->params.scale;
    cell_to_output_weights_scale = cell_to_output_weights->params.scale;
  }

  const float* input_layer_norm_coefficients_ptr =
      (is_layer_norm_lstm && !use_cifg)
          ? GetTensorData<float>(input_layer_norm_coefficients)
          : nullptr;
  const float* forget_layer_norm_coefficients_ptr =
      is_layer_norm_lstm ? GetTensorData<float>(forget_layer_norm_coefficients)
                         : nullptr;
  const float* cell_layer_norm_coefficients_ptr =
      is_layer_norm_lstm ? GetTensorData<float>(cell_layer_norm_coefficients)
                         : nullptr;
  const float* output_layer_norm_coefficients_ptr =
      is_layer_norm_lstm ? GetTensorData<float>(output_layer_norm_coefficients)
                         : nullptr;

  const int8_t* projection_weights_ptr =
      (projection_weights == nullptr)
          ? nullptr
          : GetTensorData<int8_t>(projection_weights);
  const float projection_weights_scale =
      (projection_weights == nullptr) ? 1.0f : projection_weights->params.scale;
  const float* projection_bias_ptr =
      (projection_bias == nullptr) ? nullptr
                                   : GetTensorData<float>(projection_bias);

  // Required tensors, pointers are non-null.
  const int8_t* input_to_forget_weights_ptr =
      GetTensorData<int8_t>(input_to_forget_weights);
  const float input_to_forget_weights_scale =
      input_to_forget_weights->params.scale;
  const int8_t* input_to_cell_weights_ptr =
      GetTensorData<int8_t>(input_to_cell_weights);
  const float input_to_cell_weights_scale = input_to_cell_weights->params.scale;
  const int8_t* input_to_output_weights_ptr =
      GetTensorData<int8_t>(input_to_output_weights);
  const float input_to_output_weights_scale =
      input_to_output_weights->params.scale;
  const int8_t* recurrent_to_forget_weights_ptr =
      GetTensorData<int8_t>(recurrent_to_forget_weights);
  const float recurrent_to_forget_weights_scale =
      recurrent_to_forget_weights->params.scale;
  const int8_t* recurrent_to_cell_weights_ptr =
      GetTensorData<int8_t>(recurrent_to_cell_weights);
  const float recurrent_to_cell_weights_scale =
      recurrent_to_cell_weights->params.scale;
  const int8_t* recurrent_to_output_weights_ptr =
      GetTensorData<int8_t>(recurrent_to_output_weights);
  const float recurrent_to_output_weights_scale =
      recurrent_to_output_weights->params.scale;
  const float* forget_gate_bias_ptr = GetTensorData<float>(forget_gate_bias);
  const float* cell_bias_ptr = GetTensorData<float>(cell_bias);
  const float* output_gate_bias_ptr = GetTensorData<float>(output_gate_bias);

  // Temporary storage for quantized values and scaling factors.
  int8_t* quantized_input_ptr = GetTensorData<int8_t>(input_quantized);
  int8_t* quantized_aux_input_ptr =
      (aux_input_quantized == nullptr)
          ? nullptr
          : GetTensorData<int8_t>(aux_input_quantized);
  int8_t* quantized_output_state_ptr =
      GetTensorData<int8_t>(output_state_quantized);
  int8_t* quantized_cell_state_ptr =
      GetTensorData<int8_t>(cell_state_quantized);
  float* scaling_factors_ptr = GetTensorData<float>(scaling_factors);
  float* prod_scaling_factors_ptr = GetTensorData<float>(prod_scaling_factors);
  float* recovered_cell_weights_ptr =
      GetTensorData<float>(recovered_cell_weights);

  // Auxiliary input and weights.
  const float* aux_input_ptr = nullptr;
  const int8_t* aux_input_to_input_weights_ptr = nullptr;
  const int8_t* aux_input_to_forget_weights_ptr = nullptr;
  const int8_t* aux_input_to_cell_weights_ptr = nullptr;
  const int8_t* aux_input_to_output_weights_ptr = nullptr;
  float aux_input_to_input_weights_scale = 0.0f;
  float aux_input_to_forget_weights_scale = 0.0f;
  float aux_input_to_cell_weights_scale = 0.0f;
  float aux_input_to_output_weights_scale = 0.0f;
  if (aux_input_size > 0) {
    if (!use_cifg) {
      aux_input_to_input_weights_ptr =
          GetTensorData<int8_t>(aux_input_to_input_weights);
    }
    aux_input_to_forget_weights_ptr =
        GetTensorData<int8_t>(aux_input_to_forget_weights);
    aux_input_to_cell_weights_ptr =
        GetTensorData<int8_t>(aux_input_to_cell_weights);
    aux_input_to_output_weights_ptr =
        GetTensorData<int8_t>(aux_input_to_output_weights);
    if (!use_cifg) {
      aux_input_to_input_weights_scale =
          aux_input_to_input_weights->params.scale;
    }
    aux_input_to_forget_weights_scale =
        aux_input_to_forget_weights->params.scale;
    aux_input_to_cell_weights_scale = aux_input_to_cell_weights->params.scale;
    aux_input_to_output_weights_scale =
        aux_input_to_output_weights->params.scale;
  }

  const int output_batch_leading_dim =
      output->dims->data[output->dims->size - 1];
  if (time_major) {
    // Feed the sequence into the LSTM step-by-step.
    const int input_step = n_batch * n_input;
    const int output_step = n_batch * output_batch_leading_dim;
    for (int t = 0; t < max_time; t++) {
      // If this is the forward_sequence, step forward, otherwise step
      // backwards.
      const int t_rel = forward_sequence ? t : max_time - t - 1;
      const float* input_ptr_batch =
          GetTensorData<float>(input) + t_rel * input_step;
      if (aux_input) {
        aux_input_ptr = GetTensorData<float>(aux_input) + t_rel * input_step;
      }
      float* output_ptr_batch =
          GetTensorData<float>(output) + t_rel * output_step + output_offset;

      LstmStepWithAuxInput(
          input_ptr_batch, input_to_input_weights_ptr,
          input_to_input_weights_scale, input_to_forget_weights_ptr,
          input_to_forget_weights_scale, input_to_cell_weights_ptr,
          input_to_cell_weights_scale, input_to_output_weights_ptr,
          input_to_output_weights_scale, aux_input_ptr,
          aux_input_to_input_weights_ptr, aux_input_to_input_weights_scale,
          aux_input_to_forget_weights_ptr, aux_input_to_forget_weights_scale,
          aux_input_to_cell_weights_ptr, aux_input_to_cell_weights_scale,
          aux_input_to_output_weights_ptr, aux_input_to_output_weights_scale,
          recurrent_to_input_weights_ptr, recurrent_to_input_weights_scale,
          recurrent_to_forget_weights_ptr, recurrent_to_forget_weights_scale,
          recurrent_to_cell_weights_ptr, recurrent_to_cell_weights_scale,
          recurrent_to_output_weights_ptr, recurrent_to_output_weights_scale,
          cell_to_input_weights_ptr, cell_to_input_weights_scale,
          cell_to_forget_weights_ptr, cell_to_forget_weights_scale,
          cell_to_output_weights_ptr, cell_to_output_weights_scale,
          input_layer_norm_coefficients_ptr, forget_layer_norm_coefficients_ptr,
          cell_layer_norm_coefficients_ptr, output_layer_norm_coefficients_ptr,
          input_gate_bias_ptr, forget_gate_bias_ptr, cell_bias_ptr,
          output_gate_bias_ptr, projection_weights_ptr,
          projection_weights_scale, projection_bias_ptr, params, n_batch,
          n_cell, n_input, aux_input_size, n_output, output_batch_leading_dim,
          input_gate_scratch, forget_gate_scratch, cell_scratch,
          output_gate_scratch, scaling_factors_ptr, prod_scaling_factors_ptr,
          recovered_cell_weights_ptr, quantized_input_ptr,
          quantized_aux_input_ptr, quantized_output_state_ptr,
          quantized_cell_state_ptr, GetTensorData<float>(output_state),
          GetTensorData<float>(cell_state), output_ptr_batch);
    }
  } else {
    for (int b = 0; b < n_batch; b++) {
      const int input_step = n_input;
      const int output_step = output_batch_leading_dim;
      for (int t = 0; t < max_time; t++) {
        // If this is the forward_sequence, step forward, otherwise step
        // backwards.
        const int t_rel = forward_sequence ? t : max_time - t - 1;
        const int time_offset = b * max_time + t_rel;
        const float* input_ptr =
            GetTensorData<float>(input) + time_offset * input_step;
        if (aux_input) {
          aux_input_ptr =
              GetTensorData<float>(aux_input) + time_offset * input_step;
        }
        float* output_ptr = GetTensorData<float>(output) +
                            time_offset * output_step + output_offset;

        // Offset the {output,cell}_state pointers to the right batch.
        float* output_state_ptr =
            GetTensorData<float>(output_state) + b * output_batch_leading_dim;
        float* cell_state_ptr = GetTensorData<float>(cell_state) + b * n_cell;
        // Offset the scratch pointers to the right batch.
        float* input_gate_scratch_ptr =
            input_gate_scratch ? input_gate_scratch + b * n_cell : nullptr;
        float* forget_gate_scratch_ptr = forget_gate_scratch + b * n_cell;
        float* cell_scratch_ptr = cell_scratch + b * n_cell;
        float* output_gate_scratch_ptr = output_gate_scratch + b * n_cell;

        LstmStepWithAuxInput(
            input_ptr, input_to_input_weights_ptr, input_to_input_weights_scale,
            input_to_forget_weights_ptr, input_to_forget_weights_scale,
            input_to_cell_weights_ptr, input_to_cell_weights_scale,
            input_to_output_weights_ptr, input_to_output_weights_scale,
            aux_input_ptr, aux_input_to_input_weights_ptr,
            aux_input_to_input_weights_scale, aux_input_to_forget_weights_ptr,
            aux_input_to_forget_weights_scale, aux_input_to_cell_weights_ptr,
            aux_input_to_cell_weights_scale, aux_input_to_output_weights_ptr,
            aux_input_to_output_weights_scale, recurrent_to_input_weights_ptr,
            recurrent_to_input_weights_scale, recurrent_to_forget_weights_ptr,
            recurrent_to_forget_weights_scale, recurrent_to_cell_weights_ptr,
            recurrent_to_cell_weights_scale, recurrent_to_output_weights_ptr,
            recurrent_to_output_weights_scale, cell_to_input_weights_ptr,
            cell_to_input_weights_scale, cell_to_forget_weights_ptr,
            cell_to_forget_weights_scale, cell_to_output_weights_ptr,
            cell_to_output_weights_scale, input_layer_norm_coefficients_ptr,
            forget_layer_norm_coefficients_ptr,
            cell_layer_norm_coefficients_ptr,
            output_layer_norm_coefficients_ptr, input_gate_bias_ptr,
            forget_gate_bias_ptr, cell_bias_ptr, output_gate_bias_ptr,
            projection_weights_ptr, projection_weights_scale,
            projection_bias_ptr, params,
            /*n_batch=*/1, n_cell, n_input, aux_input_size, n_output,
            output_batch_leading_dim, input_gate_scratch_ptr,
            forget_gate_scratch_ptr, cell_scratch_ptr, output_gate_scratch_ptr,
            scaling_factors_ptr, prod_scaling_factors_ptr,
            recovered_cell_weights_ptr, quantized_input_ptr,
            quantized_aux_input_ptr, quantized_output_state_ptr,
            quantized_cell_state_ptr, output_state_ptr, cell_state_ptr,
            output_ptr);
      }
    }
  }

  return kTfLiteOk;
}

TfLiteStatus EvalQuantized(
    const TfLiteTensor* input, const TfLiteTensor* input_to_input_weights,
    const TfLiteTensor* input_to_forget_weights,
    const TfLiteTensor* input_to_cell_weights,
    const TfLiteTensor* input_to_output_weights,
    const TfLiteTensor* recurrent_to_input_weights,
    const TfLiteTensor* recurrent_to_forget_weights,
    const TfLiteTensor* recurrent_to_cell_weights,
    const TfLiteTensor* recurrent_to_output_weights,
    const TfLiteTensor* cell_to_input_weights,
    const TfLiteTensor* cell_to_forget_weights,
    const TfLiteTensor* cell_to_output_weights,
    const TfLiteTensor* input_layer_norm_coefficients,
    const TfLiteTensor* forget_layer_norm_coefficients,
    const TfLiteTensor* cell_layer_norm_coefficients,
    const TfLiteTensor* output_layer_norm_coefficients,
    const TfLiteTensor* input_gate_bias, const TfLiteTensor* forget_gate_bias,
    const TfLiteTensor* cell_bias, const TfLiteTensor* output_gate_bias,
    const TfLiteTensor* projection_weights, const TfLiteTensor* projection_bias,
    const TfLiteLSTMParams* params,
    const lstm_eval::QuantizedLstmParameter* quantized_lstm_param,
    TfLiteTensor* activation_state, TfLiteTensor* cell_state,
    TfLiteTensor* output, TfLiteTensor* scratch0, TfLiteTensor* scratch1,
    TfLiteTensor* scratch2, TfLiteTensor* scratch3, TfLiteTensor* scratch4,
    TfLiteTensor* scratch5, CpuBackendContext* context) {
  TF_LITE_ASSERT(input->dims->size >= 2 && input->dims->size <= 3);
  const int n_input = input->dims->data[input->dims->size - 1];
  int max_time, n_batch;
  if (input->dims->size == 2) {
    max_time = 1;
    n_batch = input->dims->data[0];
  } else {
    max_time = input->dims->data[0];
    n_batch = input->dims->data[1];
  }

  // n_cell and n_output will be the same size when there is no projection.
  const int n_cell = input_to_output_weights->dims->data[0];
  const int n_output = recurrent_to_output_weights->dims->data[1];

  // Weights and states.
  const int8_t* input_to_input_weight_ptr =
      GetTensorData<int8_t>(input_to_input_weights);
  const int8_t* recurrent_to_input_weight_ptr =
      GetTensorData<int8_t>(recurrent_to_input_weights);
  const int8_t* cell_to_input_weight_ptr =
      GetTensorData<int8_t>(cell_to_input_weights);
  const int8_t* input_to_forget_weight_ptr =
      GetTensorData<int8_t>(input_to_forget_weights);
  const int8_t* recurrent_to_forget_weight_ptr =
      GetTensorData<int8_t>(recurrent_to_forget_weights);
  const int8_t* cell_to_forget_weight_ptr =
      GetTensorData<int8_t>(cell_to_forget_weights);
  const int8_t* input_to_cell_weight_ptr =
      GetTensorData<int8_t>(input_to_cell_weights);
  const int8_t* recurrent_to_cell_weight_ptr =
      GetTensorData<int8_t>(recurrent_to_cell_weights);
  const int8_t* input_to_output_weight_ptr =
      GetTensorData<int8_t>(input_to_output_weights);
  const int8_t* recurrent_to_output_weight_ptr =
      GetTensorData<int8_t>(recurrent_to_output_weights);
  const int8_t* cell_to_output_weight_ptr =
      GetTensorData<int8_t>(cell_to_output_weights);
  const int8_t* proj_weight_ptr = GetTensorData<int8_t>(projection_weights);
  const int16_t* layer_norm_input_weight_ptr =
      GetTensorData<int16_t>(input_layer_norm_coefficients);
  const int16_t* layer_norm_forget_weight_ptr =
      GetTensorData<int16_t>(forget_layer_norm_coefficients);
  const int16_t* layer_norm_cell_weight_ptr =
      GetTensorData<int16_t>(cell_layer_norm_coefficients);
  const int16_t* layer_norm_output_weight_ptr =
      GetTensorData<int16_t>(output_layer_norm_coefficients);
  const int32_t* input_bias_ptr = GetTensorData<int32_t>(input_gate_bias);
  const int32_t* forget_bias_ptr = GetTensorData<int32_t>(forget_gate_bias);
  const int32_t* cell_bias_ptr = GetTensorData<int32_t>(cell_bias);
  const int32_t* output_bias_ptr = GetTensorData<int32_t>(output_gate_bias);

  int16_t* cell_ptr = GetTensorData<int16_t>(cell_state);
  int8_t* activation_ptr = GetTensorData<int8_t>(activation_state);
  int8_t* output_ptr = GetTensorData<int8_t>(output);

  // Zero points
  int input_zp = 0;
  int activation_zp = 0;

  input_zp = input->params.zero_point;
  activation_zp = activation_state->params.zero_point;

  // Get params for time/batch/sequence.
  const int output_batch_leading_dim =
      output->dims->data[output->dims->size - 1];
  const int input_step = n_batch * n_input;
  const int output_step = n_batch * output_batch_leading_dim;

  for (int t = 0; t < max_time; t++) {
    const int t_rel = t;
    output_ptr = output_ptr + t_rel * output_step;

    // Input can be int8 asymmetric or int16 symmetric.
    const int8_t* input_ptr = GetTensorData<int8_t>(input) + t_rel * input_step;
    LstmStepQuantized(
        input_ptr, input_to_input_weight_ptr,
        quantized_lstm_param->effective_input_to_input_scale_a,
        quantized_lstm_param->effective_input_to_input_scale_b,
        input_to_forget_weight_ptr,
        quantized_lstm_param->effective_input_to_forget_scale_a,
        quantized_lstm_param->effective_input_to_forget_scale_b,
        input_to_cell_weight_ptr,
        quantized_lstm_param->effective_input_to_cell_scale_a,
        quantized_lstm_param->effective_input_to_cell_scale_b,
        input_to_output_weight_ptr,
        quantized_lstm_param->effective_input_to_output_scale_a,
        quantized_lstm_param->effective_input_to_output_scale_b,
        recurrent_to_input_weight_ptr,
        quantized_lstm_param->effective_recurrent_to_input_scale_a,
        quantized_lstm_param->effective_recurrent_to_input_scale_b,
        recurrent_to_forget_weight_ptr,
        quantized_lstm_param->effective_recurrent_to_forget_scale_a,
        quantized_lstm_param->effective_recurrent_to_forget_scale_b,
        recurrent_to_cell_weight_ptr,
        quantized_lstm_param->effective_recurrent_to_cell_scale_a,
        quantized_lstm_param->effective_recurrent_to_cell_scale_b,
        recurrent_to_output_weight_ptr,
        quantized_lstm_param->effective_recurrent_to_output_scale_a,
        quantized_lstm_param->effective_recurrent_to_output_scale_b,
        cell_to_input_weight_ptr,
        quantized_lstm_param->effective_cell_to_input_scale_a,
        quantized_lstm_param->effective_cell_to_input_scale_b,
        cell_to_forget_weight_ptr,
        quantized_lstm_param->effective_cell_to_forget_scale_a,
        quantized_lstm_param->effective_cell_to_forget_scale_b,
        cell_to_output_weight_ptr,
        quantized_lstm_param->effective_cell_to_output_scale_a,
        quantized_lstm_param->effective_cell_to_output_scale_b, proj_weight_ptr,
        quantized_lstm_param->effective_proj_scale_a,
        quantized_lstm_param->effective_proj_scale_b,
        quantized_lstm_param->hidden_zp,
        quantized_lstm_param->effective_hidden_scale_a,
        quantized_lstm_param->effective_hidden_scale_b,
        layer_norm_input_weight_ptr,
        quantized_lstm_param->layer_norm_input_scale_a,
        quantized_lstm_param->layer_norm_input_scale_b,
        layer_norm_forget_weight_ptr,
        quantized_lstm_param->layer_norm_forget_scale_a,
        quantized_lstm_param->layer_norm_forget_scale_b,
        layer_norm_cell_weight_ptr,
        quantized_lstm_param->layer_norm_cell_scale_a,
        quantized_lstm_param->layer_norm_cell_scale_b,
        layer_norm_output_weight_ptr,
        quantized_lstm_param->layer_norm_output_scale_a,
        quantized_lstm_param->layer_norm_output_scale_b, input_bias_ptr,
        forget_bias_ptr, cell_bias_ptr, output_bias_ptr,
        quantized_lstm_param->quantized_cell_clip,
        quantized_lstm_param->quantized_proj_clip,
        quantized_lstm_param->cell_scale,
        quantized_lstm_param->inv_large_value.data(),
        quantized_lstm_param->input_to_forget_effective_bias.get(),
        quantized_lstm_param->recurrent_to_forget_effective_bias.get(),
        quantized_lstm_param->input_to_cell_effective_bias.get(),
        quantized_lstm_param->recurrent_to_cell_effective_bias.get(),
        quantized_lstm_param->input_to_output_effective_bias.get(),
        quantized_lstm_param->recurrent_to_output_effective_bias.get(),
        quantized_lstm_param->input_to_input_effective_bias.get(),
        quantized_lstm_param->recurrent_to_input_effective_bias.get(),
        quantized_lstm_param->projection_effective_bias.get(), n_batch, n_cell,
        n_input, n_output, output_batch_leading_dim, activation_ptr,
        activation_zp, cell_ptr, output_ptr, GetTensorData<int16_t>(scratch0),
        GetTensorData<int16_t>(scratch1), GetTensorData<int16_t>(scratch2),
        GetTensorData<int16_t>(scratch3), GetTensorData<int8_t>(scratch4),
        GetTensorData<int32_t>(scratch5), context);
  }

  return kTfLiteOk;
}

}  // namespace lstm_eval
}  // namespace builtin
}  // namespace ops
}  // namespace tflite
