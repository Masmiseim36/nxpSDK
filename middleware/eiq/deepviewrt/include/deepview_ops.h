/**
 * Copyright 2018 by Au-Zone Technologies.  All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential.
 *
 * Authorization to use this header is provided through a DeepViewRT license.
 *
 * @file deepview_ops.h DeepViewRT Operator Kernels
 *
 * The DeepViewRT Operator Kernels provide the implementations of the various
 * algorithms used in the assembly of layers.
 */

#ifndef __DEEPVIEW_OPS_H__
#define __DEEPVIEW_OPS_H__

#include "deepview_rt.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Activation function to apply, if desired.
 */
typedef enum {
    /**
     * Linear activation results in a no-op.
     */
    NNActivation_Linear = 0,
    /**
     * ReLU activation performs x = max(0, x)
     */
    NNActivation_ReLU = 1,
    /**
     * ReLU6 activation performs x = min(max(0, x), 6)
     */
    NNActivation_ReLU6 = 2,
    /**
     * Sigmoid activation performs x = 1/(1+e^-x)
     */
    NNActivation_Sigmoid     = 3,
    NNActivation_SigmoidFast = 4,
    /**
     * Tanh activation performs x = tanh(x)
     */
    NNActivation_Tanh = 5,
} NNActivation;

/**
 * Padding tuple indices, this deals with 8-element padding arrays.
 */
typedef enum {
    NNPadding_BatchIn    = 0,
    NNPadding_BatchOut   = 1,
    NNPadding_Top        = 2,
    NNPadding_Bottom     = 3,
    NNPadding_Left       = 4,
    NNPadding_Right      = 5,
    NNPadding_ChannelIn  = 6,
    NNPadding_ChannelOut = 7,
} NNPadding;

/**
 * @brief nn_add implements addition for two tensors, a + b = c.
 * This function follows standard broadcasting rules for tensors.
 * The output shape of the result is equal to the largest tensor a or b.
 *
 * @param output pointer to the output tensor
 * @param input_a pointer to the first input tensor
 * @param input_b pointer to the second input tensor
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_add(NNTensor* output, NNTensor* input_a, NNTensor* input_b);

/**
 * @brief nn_subtract implements subtraction for two tensors, a - b = c.
 *<p>
 * This function follows standard broadcasting rules for tensors.
 * The output shape of the result is equal to the shape of input_a.
 * </p>
 *
 * @param output pointer to the output tensor
 * @param input_a pointer to the tensor being subtracted from
 * @param input_b pointer to the tensor to subtract from input_a
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_subtract(NNTensor* output, NNTensor* input_a, NNTensor* input_b);

/**
 * @brief nn_multiply implements addition for two tensors, a * b = c.
 * <p>
 * This function follows standard broadcasting rules for tensors.
 * The output shape of the result is equal to the largest tensor a or b.
 * </p>
 *
 * @param output pointer to the output tensor
 * @param input_a pointer to the first input tensor
 * @param input_b pointer to the second input tensor
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_multiply(NNTensor* output, NNTensor* input_a, NNTensor* input_b);

/**
 * @brief nn_divide implements addition for two tensors, a / b = c.
 * <p>
 * This function follows standard broadcasting rules for tensors.
 * The output shape of the result is equal the shape of input_b.
 * </p>
 *
 * @param output pointer to the output tensor
 * @param input_a pointer to the tensor representing the dividend
 * @param input_b pointer to the tensor representing the divisor
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_divide(NNTensor* output, NNTensor* input_a, NNTensor* input_b);

/**
 * @brief nn_abs implements the absolute value function, |input| = output.
 *
 * @param output pointer to the output tensor
 * @param input point to the input tensor
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_abs(NNTensor* output, NNTensor* input);

/**
 * @brief nn_sqrt implements the square root function, input ^ 0.5 = output.
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_sqrt(NNTensor* output, NNTensor* input);

/**
 * @brief nn_rsqrt implements the inverse square root function, 1 / (input ^
 * 0.5) = output.
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_rsqrt(NNTensor* output, NNTensor* input, const float* epsilon);

/**
 * Pre-release of log operation.
 */
NN_AVAILABLE_SINCE_2_4
NN_API
NNError
nn_log(NNTensor* output, NNTensor* input);

/**
 * Pre-release of exp operation.
 */
NN_AVAILABLE_SINCE_2_4
NN_API
NNError
nn_exp(NNTensor* output, NNTensor* input);

/**
 * @brief A fast approximation of the logistic sigmoid implemented as 1/(1+|x|).
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_sigmoid_fast(NNTensor* output, NNTensor* input);

/**
 * @brief The logistic sigmoid 1/(1+e^-x) or e^x/(e^x+1).
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_sigmoid(NNTensor* output, NNTensor* input);

/**
 * @brief nn_tanh implements the hyperbolic tangent function.
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_tanh(NNTensor* output, NNTensor* input);

/**
 * @brief nn_relu implements the rectified linear function.
 * <p>
 * The formula used for a given input tensor is,
 * max(input, 0) = output.
 * The output shape is equal to the shape of the input.
 * </p>
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_relu(NNTensor* output, NNTensor* input);

/**
 * @brief nn_relu implements the rectified linear 6 function.
 * <p>
 * The formula used for a given input tensor is,
 * min(max(input, 0), 6) = output.
 * The output shape is equal to the shape of the input.
 * </p>
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_relu6(NNTensor* output, NNTensor* input);


NN_AVAILABLE_SINCE_2_4
NN_API
NNError
nn_swish(NNTensor* output, NNTensor* input);


/**
 * @brief nn_prelu implements the parametric rectified linear unit.
 */
NN_AVAILABLE_SINCE_2_4
NN_API
NNError
nn_prelu(NNTensor* output, NNTensor* input, NNTensor* weights);

/**
 * @brief nn_leaky_relu implements the LeakyRelu function.
 * <p>
 * The formula used for a given input tensor is,
 * output = (input < 0)*alpha*input + (input >= 0) * input.
 * The output shape is equal to the shape of the input.
 * </p>
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 */
NN_AVAILABLE_SINCE_2_4
NN_API
NNError
nn_leaky_relu(NNTensor* output, NNTensor* input, float alpha);

/**
 * @brief nn_matmul implements matrix multiplication, A*B = C.
 *<p>
 * If transpose_a is true, then
 * tensor a is transposed before the matrix multiplication.
 * If transpose_b is true, then tensor a is transposed
 * before the matrix multiplication.
 * Output Shapes:
 * transpose_a = false, transpose_b = false, [a[0], b[1]]
 * transpose_a = false, transpose_b = true, [a[0], b[0]]
 * transpose_a = true, transpose_b = false, [a[1], b[1]]
 * transpose_a = true, transpose_b = true, [a[1], b[0]]
 * </p>
 *
 * @param output pointer to the output tensor
 * @param a pointer to the first input tensor
 * @param b pointer to the second input tensor
 * @param transpose_a boolean stating whether to transpose A
 * @param transpose_b boolean stating whether to transpose B
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_matmul(NNTensor* output,
          NNTensor* a,
          NNTensor* b,
          bool      transpose_a,
          bool      transpose_b);

/**
 * Special version of @ref nn_matmul that can use cache tensor, useful for MCU.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_matmul_cache(NNTensor* output,
                NNTensor* cache,
                NNTensor* a,
                NNTensor* b,
                bool      transpose_a,
                bool      transpose_b);

/**
 * @brief nn_dense implements the dense operation.
 * <p>
 *The formula used for the dense operation is as follows,
 * activation(input * weights + bias) = output.
 * The output shape is [input[0], weights[1]].
 * </p>
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 * @param weights pointer to the weight tensor
 * @param bias pointer to the bias tensor
 * @param activation the activation to be performed
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_dense(NNTensor*    output,
         NNTensor*    input,
         NNTensor*    weights,
         NNTensor*    bias,
         NNActivation activation);

/**
 * @brief nn_linear implements the linear operation.
 * <p>
 * The formula used for the linear operation is as follows,
 * activation(input * transpose(weights) + bias) = output.
 * The output shape is [input[0], weights[0]].
 * </p>
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 * @param weights pointer to the weight tensor
 * @param bias pointer to the bias tensor
 * @param activation the activation to be performed
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_linear(NNTensor*    output,
          NNTensor*    input,
          NNTensor*    weights,
          NNTensor*    bias,
          NNActivation activation);

/**
 * @brief nn_conv implements the 2D convolution layer
 * given a 4-D input and filter.
 * <p>
 * The output shape will have the shape format NHWC
 * with N = input[0], C = filter[3] and the shape
 * values HW given by the formula,
 * floor((input[i] - filter[i]) / stride[i]) + 1
 * </p>
 *
 * @param output pointer to the output tensor
 * @param cache pointer to the cache tensor, used to increase efficiency
 * @param input pointer to the input tensor
 * @param filter pointer to the filter tensor
 * @param bias pointer to the bias tensor
 * @param stride array providing the strides
 * @param groups the number of convolution groups
 * @param activation the activation to be performed after the convolution
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_conv(NNTensor*     output,
        NNTensor*     cache,
        NNTensor*     input,
        NNTensor*     filter,
        NNTensor*     bias,
        const int32_t stride[4],
        int           groups,
        NNActivation  activation);

/**
 * Extended version of the @ref nn_conv operation.  The extensions provide
 * support for custom padding handling as well as filter dilation.  It is
 * otherwise the same as @ref nn_conv which itself calls into this function
 * with padding [0,0,0,0,0,0,0,0] and dilation [1,1,1,1].
 *
 * The padding parameter must be an array of 4 pairs (8 elements) of int32_t
 * which represent the head/tail paddings for each of the 4 dimensions used
 * by nn_conv_ex.
 *
 * The dilation parameter must be an array of 4 elements of int32_t which
 * represent the dilation rate of the filter in each of the 4 dimensions used by
 * nn_conv_ex.  Dilation of 1 is the same as the standard convolution with each
 * filter spatial element moving by 1 in their respective dimension.
 */
NN_AVAILABLE_SINCE_2_3
NN_API
NNError
nn_conv_ex(NNTensor*     output,
           NNTensor*     cache,
           NNTensor*     input,
           NNTensor*     filter,
           NNTensor*     bias,
           const int32_t stride[4],
           const int32_t padding[8],
           const int32_t dilation[4],
           int           groups,
           NNActivation  activation);

/**
 * @brief nn_maxpool implements the max pooling layer
 * given a 4-D input.
 * <p>
 * The output shape will have the shape format NHWC
 * with N = input[0], C = input[3] and the shape
 * values HW given by the formula,
 * floor((input[i] - window[i]) / stride[i]) + 1
 * </p>
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 * @param window array providing the window size
 * @param stride array providing the strides
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_maxpool(NNTensor*     output,
           NNTensor*     input,
           const int32_t window[4],
           const int32_t stride[4]);

/**
 * Extended version of @ref nn_maxpool which provides fused padding support and
 * window dilation, refer to @ref nn_conv_ex for details for padding and
 * dilation parameters.
 */
NN_AVAILABLE_SINCE_2_3
NN_API
NNError
nn_maxpool_ex(NNTensor*     output,
              NNTensor*     input,
              const int32_t window[4],
              const int32_t stride[4],
              const int32_t padding[8],
              const int32_t dilation[4]);

/**
 * @brief nn_avgpool implements the average pooling layer
 * given a 4-D input.
 * <p>
 * The output shape will have the shape format NHWC
 * with N = input[0], C = input[3] and the shape
 * values HW given by the formula,
 * floor((input[i] - window[i]) / stride[i]) + 1
 * </p>
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 * @param window array providing the window size
 * @param stride array providing the strides
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_avgpool(NNTensor*     output,
           NNTensor*     input,
           const int32_t window[4],
           const int32_t stride[4]);

/**
 * Extended version of @ref nn_avgpool which provides fused padding support and
 * window dilation, refer to @ref nn_conv_ex for details for padding and
 * dilation parameters.
 */
NN_AVAILABLE_SINCE_2_3
NN_API
NNError
nn_avgpool_ex(NNTensor*     output,
              NNTensor*     input,
              NNTensor*     cache,
              const int32_t window[4],
              const int32_t stride[4],
              const int32_t padding[8],
              const int32_t dilation[4]);

/**
 * @brief nn_reduce_sum reduces the axes in (axes[n_axes]) removing
 * the axes in the output tensor unless keep_dims is true in
 * which case they are kept at 1.
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 * @param axes pointer to the axes to be reduced
 * @param n_axes number of axes to be reduced
 * @param keep_dims boolean to determine whether to maintain the reduced
 * dimensions as 1
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_reduce_sum(NNTensor*     output,
              NNTensor*     input,
              int32_t       n_axes,
              const int32_t axes[NN_ARRAY_PARAM(n_axes)],
              bool          keep_dims);

/**
 * @brief nn_reduce_min reduces the axes in (axes[n_axes]) removing
 * the axes in the output tensor unless keep_dims is true in
 * which case they are kept at 1.
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 * @param axes pointer to the axes to be reduced
 * @param n_axes number of axes to be reduced
 * @param keep_dims boolean to determine whether to maintain the reduced
 * dimensions as 1
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_reduce_min(NNTensor*     output,
              NNTensor*     input,
              int32_t       n_axes,
              const int32_t axes[NN_ARRAY_PARAM(n_axes)],
              bool          keep_dims);

/**
 * @brief nn_reduce_max reduces the axes in (axes[n_axes]) removing
 * the axes in the output tensor unless keep_dims is true in
 * which case they are kept at 1.
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 * @param axes pointer to the axes to be reduced
 * @param n_axes number of axes to be reduced
 * @param keep_dims boolean to determine whether to maintain the reduced
 * dimensions as 1
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_reduce_max(NNTensor*     output,
              NNTensor*     input,
              int32_t       n_axes,
              const int32_t axes[NN_ARRAY_PARAM(n_axes)],
              bool          keep_dims);

/**
 * @brief nn_reduce_mean reduces the axes in (axes[n_axes]) removing
 * the axes in the output tensor unless keep_dims is true in
 * which case they are kept at 1.
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 * @param axes pointer to the axes to be reduced
 * @param n_axes number of axes to be reduced
 * @param keep_dims boolean to determine whether to maintain the reduced
 * dimensions as 1
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_reduce_mean(NNTensor*     output,
               NNTensor*     input,
               int32_t       n_axes,
               const int32_t axes[NN_ARRAY_PARAM(n_axes)],
               bool          keep_dims);

/**
 * @brief nn_reduce_product reduces the axes in (axes[n_axes]) removing
 * the axes in the output tensor unless keep_dims is true in
 * which case they are kept at 1.
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 * @param axes pointer to the axes to be reduced
 * @param n_axes number of axes to be reduced
 * @param keep_dims boolean to determine whether to maintain the reduced
 * dimensions as 1
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_reduce_product(NNTensor*     output,
                  NNTensor*     input,
                  int32_t       n_axes,
                  const int32_t axes[NN_ARRAY_PARAM(n_axes)],
                  bool          keep_dims);

/**
 * @brief nn_softmax implements the softmax on the tensor along the 4th
 * dimension. <p> The output shape is equal to the input shape
 * </p>
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_softmax(NNTensor* output, NNTensor* input);

/**
 * Finds the maximum value in the flattened tensor, storing the index into the
 * target of the index pointer maximum scalar value into value.  Both index and
 * value are ignored if NULL.  The value pointer must be at least value_size
 * bytes long which in turn must be large enough to hold input's scalar size.
 *
 * For non-flattened version refer to @ref nn_reduce_max.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_argmax(NNTensor* input, int* index, void* value, size_t value_size);

/**
 * @brief nn_batchnorm implements batch normalization on a given input.
 * <p>
 * The formula used for batch normalization is as follows,
 * scale * (input - mean) / sqrt(variance + epsilon) + offset = output.
 * The shape of the output tensor is equal to that of the input tensor.
 * </p>
 *
 * @param output pointer to the output tensor
 * @param cache pointer to the cache tensor, used to increase efficiency
 * @param input pointer to the input tensor
 * @param mean pointer to the mean tensor
 * @param variance pointer to the variance tensor
 * @param offset pointer to the offset tensor
 * @param scale pointer to the scale tensor
 * @param epsilon pointer to the epsilon tensor
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_batchnorm(NNTensor* output,
             NNTensor* cache,
             NNTensor* input,
             NNTensor* mean,
             NNTensor* variance,
             NNTensor* offset,
             NNTensor* scale,
             NNTensor* epsilon);

/**
 *
 */
typedef struct nn_gru_context NNGruContext;

/**
 *
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_gru_init(NNGruContext** gru_,
            NNTensor*      cache,
            NNTensorType   datatype,
            int            num_features,
            int            num_hidden,
            int            num_outputs);

/**
 *
 */
NN_AVAILABLE_SINCE_2_0
NN_API
void
nn_gru_release(NNGruContext* gru);

/**
 * @brief nn_gru implements the GRU layer.
 *
 * @note GRU requires a number of tensor views internally which means this
 * function will use 1KB of stack space beyond typical ops.
 *
 * @param output pointer to the output tensor
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_gru(NNTensor*     output,
       NNTensor*     input,
       NNTensor*     H,
       NNTensor*     Wir,
       NNTensor*     Bir,
       NNTensor*     Wh,
       NNTensor*     Bwh,
       NNTensor*     Rh,
       NNTensor*     Brh,
       NNGruContext* gru);

NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_svm_update_kernel(NNTensor* output,
                     NNTensor* cache,
                     NNTensor* input,
                     NNTensor* sv,
                     NNTensor* idx);

NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_svm_decision_stats(NNTensor* output,
                      NNTensor* cache,
                      NNTensor* input,
                      NNTensor* alpha,
                      NNTensor* rho);

NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_svm_soft_probability(NNTensor* output,
                        NNTensor* cache,
                        NNTensor* input,
                        NNTensor* a,
                        NNTensor* b);

NN_AVAILABLE_SINCE_2_4
NN_API
NNError
nn_decode_centernet(NNTensor* heatmap_tensor,
					NNTensor* regression_tensor,
					NNTensor* size_tensor,
					NNTensor* cache,
					NNTensor* decode_out);

/**
 * @brief nn_resize implements Nearest Neighbor or Bilinear interpolation
 */
NN_AVAILABLE_SINCE_2_4
NN_API
NNError
nn_resize(NNTensor* output,
          NNTensor* input,
          int       mode,
          bool      align_corners,
          bool      half_pixel_centers);

#ifdef __cplusplus
}
#endif

#endif /* __DEEPVIEW_OPS_H__ */
