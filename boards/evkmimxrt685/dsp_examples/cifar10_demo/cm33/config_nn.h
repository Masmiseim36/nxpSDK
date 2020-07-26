/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_NN_H__
#define __CONFIG_NN_H__

#define NN_ENABLE_xa_nn_malloc                      1
#define NN_ENABLE_xa_nn_free                        1
#define NN_ENABLE_xa_nn_matXvec_16x16_16            1
#define NN_ENABLE_xa_nn_matXvec_16x16_32            1
#define NN_ENABLE_xa_nn_matXvec_16x16_64            1
#define NN_ENABLE_xa_nn_matXvec_16x16_16_tanh       1
#define NN_ENABLE_xa_nn_matXvec_16x16_16_sigmoid    1
#define NN_ENABLE_xa_nn_matXvec_batch_16x16_64      1
#define NN_ENABLE_xa_nn_matXvec_8x16_16             1
#define NN_ENABLE_xa_nn_matXvec_8x16_32             1
#define NN_ENABLE_xa_nn_matXvec_8x16_64             1
#define NN_ENABLE_xa_nn_matXvec_8x16_16_tanh        1
#define NN_ENABLE_xa_nn_matXvec_8x16_16_sigmoid     1
#define NN_ENABLE_xa_nn_matXvec_batch_8x16_64       1
#define NN_ENABLE_xa_nn_matXvec_8x8_8               1
#define NN_ENABLE_xa_nn_matXvec_8x8_16              1
#define NN_ENABLE_xa_nn_matXvec_8x8_32              1
#define NN_ENABLE_xa_nn_matXvec_8x8_8_tanh          1
#define NN_ENABLE_xa_nn_matXvec_8x8_8_sigmoid       1
#define NN_ENABLE_xa_nn_matXvec_batch_8x8_32        1
#define NN_ENABLE_xa_nn_matXvec_f32xf32_f32_sigmoid 1
#define NN_ENABLE_xa_nn_matXvec_f32xf32_f32_tanh    1
#define NN_ENABLE_xa_nn_matXvec_f32xf32_f32         1
#define NN_ENABLE_xa_nn_matXvec_batch_f32xf32_f32   1
#define NN_ENABLE_xa_nn_vec_sigmoid_32_32           1
#define NN_ENABLE_xa_nn_vec_tanh_32_32              1
#define NN_ENABLE_xa_nn_vec_relu_32_32              1
#define NN_ENABLE_xa_nn_vec_relu1_32_32             1
#define NN_ENABLE_xa_nn_vec_relu6_32_32             1
#define NN_ENABLE_xa_nn_vec_softmax_32_32           1
#define NN_ENABLE_xa_nn_vec_sigmoid_f32_f32         1
#define NN_ENABLE_xa_nn_vec_tanh_f32_f32            1
#define NN_ENABLE_xa_nn_vec_relu_f32_f32            1
#define NN_ENABLE_xa_nn_vec_relu1_f32_f32           1
#define NN_ENABLE_xa_nn_vec_relu6_f32_f32           1
#define NN_ENABLE_xa_nn_vec_softmax_f32_f32         1
#define NN_ENABLE_xa_nn_vec_sigmoid_32_16           1
#define NN_ENABLE_xa_nn_vec_tanh_32_16              1
#define NN_ENABLE_xa_nn_vec_sigmoid_32_8            1
#define NN_ENABLE_xa_nn_vec_tanh_32_8               1
#define NN_ENABLE_xa_nn_vec_interpolation_q15       1
#define NN_ENABLE_xa_nn_conv1d_std_getsize          1
#define NN_ENABLE_xa_nn_conv1d_std_8x16             1
#define NN_ENABLE_xa_nn_conv1d_std_8x8              1
#define NN_ENABLE_xa_nn_conv1d_std_16x16            1
#define NN_ENABLE_xa_nn_conv1d_std_f32              1
#define NN_ENABLE_xa_nn_conv2d_std_getsize          1
#define NN_ENABLE_xa_nn_conv2d_std_8x16             1
#define NN_ENABLE_xa_nn_conv2d_std_8x8              1
#define NN_ENABLE_xa_nn_conv2d_std_16x16            1
#define NN_ENABLE_xa_nn_conv2d_std_f32              1
#define NN_ENABLE_xa_nn_conv2d_depthwise_f32        1
#define NN_ENABLE_xa_nn_conv2d_pointwise_f32        1
#define NN_ENABLE_xa_nn_conv2d_depthwise_getsize    1
#define NN_ENABLE_xa_nn_conv2d_pointwise_8x16       1
#define NN_ENABLE_xa_nn_conv2d_pointwise_8x8        1
#define NN_ENABLE_xa_nn_conv2d_depthwise_8x16       1
#define NN_ENABLE_xa_nn_conv2d_depthwise_8x8        1
#define NN_ENABLE_xa_nn_conv2d_depthwise_16x16      1
#define NN_ENABLE_xa_nn_conv2d_pointwise_16x16      1
#define NN_ENABLE_xa_nn_avgpool_getsize             1
#define NN_ENABLE_xa_nn_avgpool_8                   1
#define NN_ENABLE_xa_nn_avgpool_16                  1
#define NN_ENABLE_xa_nn_avgpool_f32                 1
#define NN_ENABLE_xa_nn_maxpool_getsize             1
#define NN_ENABLE_xa_nn_maxpool_8                   1
#define NN_ENABLE_xa_nn_maxpool_16                  1
#define NN_ENABLE_xa_nn_maxpool_f32                 1
#define NN_ENABLE_xa_nn_fully_connected_f32         1
#define NN_ENABLE_xa_nn_fully_connected_16x16_16    1
#define NN_ENABLE_xa_nn_fully_connected_8x16_16     1
#define NN_ENABLE_xa_nn_fully_connected_8x8_8       1

#endif // __CONFIG_NN_H__
