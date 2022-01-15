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
     * ReLU activation performs \f$ f(x) = max(0, x) \f$
     */
    NNActivation_ReLU = 1,
    /**
     * ReLU6 activation performs \f$ f(x) = min(max(0, x), 6) \f$
     */
    NNActivation_ReLU6 = 2,
    /**
     * Sigmoid activation performs \f$ f(x) = 1/(1+e^-x) \f$
     */
    NNActivation_Sigmoid     = 3,
    NNActivation_SigmoidFast = 4,
    /**
     * Tanh activation performs \f$ f(x) = tanh(x) \f$
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
 *
 * @return NNError
 *
 * @since 2.0
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
 *
 * @return NNError
 *
 * @since 2.0
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
 *
 * @return NNError
 *
 * @since 2.0
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
 *
 * @return NNError
 *
 * @since 2.0
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
 *
 * @return NNError
 *
 * @since 2.0
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
 *
 * @return NNError
 *
 * @since 2.0
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
 * @param epsilon epsilon parameter is applied to the input to avoid divide by
 * zero.
 *
 * @return NNError
 *
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_rsqrt(NNTensor* output, NNTensor* input, const float* epsilon);

/**
 * Implements the log operation.
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 *
 * @return NNError
 *
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
NNError
nn_log(NNTensor* output, NNTensor* input);

/**
 * Implements the exp operation.
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 *
 * @return NNError
 *
 * @since 2.4
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
 *
 * @return NNError
 *
 * @since 2.0
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
 *
 * @return NNError
 *
 * @since 2.0
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
 *
 * @return NNError
 *
 * @since 2.0
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
 *
 * @return NNError
 *
 * @since 2.0
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
 *
 * @return NNError
 *
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNError
nn_relu6(NNTensor* output, NNTensor* input);

/**
 * Hard-Swish activation function.
 *
 * @param output
 * @param input
 *
 * @return NNError
 *
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
NNError
nn_swish(NNTensor* output, NNTensor* input);

/**
 * Implements the parametric rectified linear unit.  A PReLU is a specialization
 * of ReLU where negative values are multiplied against a weight instead of set
 * to zero.
 *
 * The weights can be a vector but must fit evenly within the output tensor.
 *
 * @f[
 *  f(x) =
 *  \begin{cases}
 *      x,  & \text{if } x \geq 0\\
 *      x*w,& \text{otherwise}
 *  \end{cases}
 * @f]
 *
 * @param output tensor which will receive the processed results
 * @param input input tensor against which the output will be generated
 * @param weights the weights tensor used for calculating the prelu
 *
 * @return NNError
 *
 * @since 2.4
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
 * @param alpha the alpha value multiplied to the result.
 *
 * @return NNError
 *
 * @since 2.4
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
 *
 * @return NNError
 *
 * @since 2.0
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
 * If the cache tensor is at least as large as the b tensor then it will be
 * cached there.
 *
 * @param output pointer to the output tensor
 * @param cache pointer to the cache tensor
 * @param a pointer to the first input tensor
 * @param b pointer to the second input tensor
 * @param transpose_a boolean stating whether to transpose A
 * @param transpose_b boolean stating whether to transpose B
 *
 * @return NNError
 *
 * @since 2.0
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
 * @see NNActivation
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 * @param weights pointer to the weight tensor
 * @param bias pointer to the bias tensor
 * @param activation the activation to be performed
 *
 * @return NNError
 *
 * @since 2.0
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
 * @see NNActivation
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 * @param weights pointer to the weight tensor
 * @param bias pointer to the bias tensor
 * @param activation the activation to be performed
 *
 * @return NNError
 *
 * @since 2.0
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
 * @see NNActivation
 *
 * @param output pointer to the output tensor
 * @param cache pointer to the cache tensor, used to increase efficiency
 * @param input pointer to the input tensor
 * @param filter pointer to the filter tensor
 * @param bias pointer to the bias tensor
 * @param stride array providing the strides
 * @param groups the number of convolution groups
 * @param activation the activation to be performed after the convolution
 *
 * @return NNError
 *
 * @since 2.0
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
 *
 * @see NNActivation
 *
 * @param output pointer to the output tensor
 * @param cache pointer to the cache tensor, used to increase efficiency
 * @param input pointer to the input tensor
 * @param filter pointer to the filter tensor
 * @param bias pointer to the bias tensor
 * @param stride array providing the strides
 * @param padding array providing the padding sizes
 * @param dilation array providing the dilation amounts
 * @param groups the number of convolution groups
 * @param activation the activation to be performed after the convolution
 *
 * @return NNError
 *
 * @since 2.3
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
 * @brief nn_transpose_conv2d_ex implements the 2d transpose convolution
 * It is the adjoint operator of the standard conv2d.
 *
 * @see NNActivation
 *
 * @param output pointer to the output tensor
 * @param cache pointer to the cache tensor, used to increase efficiency
 * @param input pointer to the input tensor
 * @param filter pointer to the filter tensor
 * @param bias pointer to the bias tensor
 * @param stride array providing the strides
 * @param padding array providing the padding amounts
 * @param activation the activation to be performed after the convolution
 *
 * @return NNError
 *
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
NNError
nn_transpose_conv2d_ex(NNTensor*     output,
                       NNTensor*     cache,
                       NNTensor*     input,
                       NNTensor*     filter,
                       NNTensor*     bias,
                       const int32_t stride[4],
                       const int32_t padding[8],
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
 *
 * @return NNError
 *
 * @since 2.0
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
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 * @param window array providing the window size
 * @param stride array providing the strides
 * @param padding array providing the padding amounts
 * @param dilation array providing the dilation amounts
 *
 * @return NNError
 *
 * @since 2.3
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
 *
 * @return NNError
 *
 * @since 2.0
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
 *
 * @param output pointer to the output tensor
 * @param input pointer to the input tensor
 * @param window array providing the window size
 * @param stride array providing the strides
 * @param padding array providing the padding amounts
 * @param dilation array providing the dilation amounts
 *
 * @return NNError
 *
 * @since 2.0
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
 *
 * @return NNError
 *
 * @since 2.0
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
 *
 * @return NNError
 *
 * @since 2.0
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
 *
 * @return NNError
 *
 * @since 2.0
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
 *
 * @return NNError
 *
 * @since 2.0
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
 *
 * @return NNError
 *
 * @since 2.0
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
 *
 * @return NNError
 *
 * @since 2.0
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
 *
 * @param input pointer to the input tensor
 * @param index pointer to an integer to receive the index of the maximum value.
 * @param value pointer to a variable of size @p value_size which will receive
 * the maximum value found.  If NULL value is not saved and @p value_size can
 * be zero.
 * @param value_size size of the variable pointed to by @p value.
 *
 * @return NNError
 *
 * @since 2.0
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
 *
 * @return NNError
 *
 * @since 2.0
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
 * Resizes the tensor spatial (height/width) dimensions using either nearest
 * neighbour or bilinear interpolation.
 *
 * Modes:
 *  - 0: nearest neighbour scaling
 *  - 1: bilinear interpolation scaling
 *
 * @note Currently bilinear interpolation is not implemented on CPU and will
 * fallback to nearest neighbour.
 *
 * @param output tensor which will receive the results and should be the target
 *               shape into which the resize operation will be performed.
 * @param input input tensor for resize operation.
 * @param mode the interpolation model to be used, modes are noted above.
 * @param align_corners not currently implemented
 * @param half_pixel_centers not currently implemented
 *
 * @return NNError
 *
 * @since 2.4.18
 */
NN_AVAILABLE_SINCE_2_4_18
NN_API
NNError
nn_resize(NNTensor* output,
          NNTensor* input,
          int       mode,
          bool      align_corners,
          bool      half_pixel_centers);

/**
 *
 * @note Currently bilinear interpolation is not implemented on CPU and will
 * fallback to nearest neighbour.
 *
 * @param output tensor which will receive the results and should be the target
 *               shape into which the resize operation will be performed.
 * @param input input tensor for resize operation.
 * @param mode
 * @param align_corners
 * @param half_pixel_centers
 *
 * @return
 *
 * @since 2.4.18
 */
NN_AVAILABLE_SINCE_2_4_18
NN_API
NNError
nn_resize(NNTensor* output,
          NNTensor* input,
          int       mode,
          bool      align_corners,
          bool      half_pixel_centers);

/**
 * Implements the post-processing for TensorFlow Object Detection API SSD.
 *
 * It supports both version 1 and 2 of the TensorFlow Object Detection API for
 * SSD models.
 *
 * Anchors are an unlearned constant which defines the starting candidates for
 * bounding boxes.  The anchors can generally be extracted from the source .pb
 * or SavedModel files as well as the Keras H5.  Otherwise the source code for
 * the model trainer is where the anchors would be defined.
 *
 * The detectv4.c example provides an example of using this API and how to
 * access the needed tensors from the model.
 *
 * @note While this API was originally added in DeepViewRT 2.4.28 it was not
 * exposed by deepview_ops.h until 2.4.32.  To use it in versions 2.4.28 and
 * 2.4.30 the function signature needs to be manually "extern" into the user
 * application.
 *
 * @param score Tensor holding scores of tensorflow ssd (the first output)
 * @param trans Tensor holding transformation parameters of tensorflow ssd
 *  (second output).
 * @param anchors Tensor holding pre-defined anchors for the ssd model
 * @param cache_tensor Tensor holding the optional cache tensor
 * @param score_threshold threshold to filter out noises
 * @param iou_threshold threshold for nms
 * @param max_output_size_per_class maximum detection per class
 * @param bbx_out_tensor Tensor receiving the bounding box output.
 * @param bbx_out_dim_tensor Tensor receiving the number of valid detections
 *  for each class.
 *
 * @return NNError
 *
 * @since 2.4.28
 */
NN_AVAILABLE_SINCE_2_4_28
NN_API
NNError
nn_ssd_decode_nms_standard_bbx(NNTensor* score,
                               NNTensor* trans,
                               NNTensor* anchors,
                               NNTensor* cache,
                               float     score_threshold,
                               float     iou_threshold,
                               int32_t   max_output_size_per_class,
                               NNTensor* bbx_out_tensor,
                               NNTensor* bbx_out_dim_tensor);

/**
 * Implements the post-processing for the eIQ Portal variant of SSD.
 *
 * In this approach, the bounding box encoding method is slightly different
 * compared to the standard TensorFlow approach, each encoded representation
 * is further divided by their corresponding variance and the two outputs of
 * `score` and `trans` are concatenated into a single output `prediction`.
 *
 * Anchors are an unlearned constant which defines the starting candidates for
 * bounding boxes.  When using eIQ Portal the exported DeepViewRT Model will
 * have the anchors embedded into the model automatically.  They are also saved
 * in the model's checkpoint folder.
 *
 * The detectv4.c example provides an example of using this API and how to
 * access the needed tensors from the model.
 *
 * @note While this API was originally added in DeepViewRT 2.4.28 it was not
 * exposed by deepview_ops.h until 2.4.32.  To use it in versions 2.4.28 and
 * 2.4.30 the function signature needs to be manually "extern" into the user
 * application.
 *
 * @param prediction Tensor holding the concatenated scores and transforms
 * @param anchors Tensor holding the pre-defined anchors for the ssd model
 * @param cache_tensor Tensor holding the optional cache memory
 * @param score_threshold threshold to filter out noises
 * @param iou_threshold threshold for nms
 * @param max_output_size_per_class maximum detection per class
 * @param bbx_out_tensor Tensor receiving the bounding boxes
 * @param bbx_out_dim_tensor Tensor receiving the number of valid detections
 *  for each class.
 *
 * @return NNError
 *
 * @since 2.4.28
 */
NN_AVAILABLE_SINCE_2_4_28
NN_API
NNError
nn_ssd_decode_nms_variance_bbx(NNTensor* prediction,
                               NNTensor* anchors,
                               NNTensor* cache,
                               float     score_threshold,
                               float     iou_threshold,
                               int32_t   max_output_size_per_class,
                               NNTensor* bbx_out_tensor,
                               NNTensor* bbx_out_dim_tensor);

/**
 * @brief Performs the NMS portion of the SSD box decoding algorithm.
 *
 * @deprecated It is deprecated since 2.4.32 in favour of the more complete
 * implementations applicable to eIQ Portal models using
 * @ref nn_ssd_decode_nms_variance_bbx() or for TensorFlow 1.x/2.x  Object
 * Detection API models by using @ref nn_ssd_decode_nms_standard_bbx().
 *
 * @return NNError
 *
 * @since 2.4.25
 */
NN_DEPRECATED_SINCE_2_4_30
NN_AVAILABLE_SINCE_2_4_25
NN_API
NNError
nn_ssd_nms_full(NNTensor* input,
                NNTensor* bbx_tensor,
                NNTensor* score_tensor,
                NNTensor* cache,
                float     score_threshold,
                float     iou_threshold,
                int32_t   max_output_size,
                NNTensor* bbx_out_tensor,
                NNTensor* bbx_out_dim_tensor);

#ifdef __cplusplus
}
#endif

#endif /* __DEEPVIEW_OPS_H__ */
