/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __LENET_MODEL_H__
#define __LENET_MODEL_H__

#define data_width     (4)
#define data_precision (-1)

// LAYER 0 INPUT
#define layer0_input_height   (28)
#define layer0_input_width    (28)
#define layer0_input_channels (1)

// LAYER 1 CONVOLUTION STANDARD
#define layer1_conv_input_height       (layer0_input_height)
#define layer1_conv_input_width        (layer0_input_width)
#define layer1_conv_input_channels     (layer0_input_channels)
#define layer1_conv_kernel_height      (5)
#define layer1_conv_kernel_width       (5)
#define layer1_conv_out_channels       (20)
#define layer1_conv_x_stride           (1)
#define layer1_conv_y_stride           (1)
#define layer1_conv_x_padding          (0)
#define layer1_conv_y_padding          (0)
#define layer1_conv_out_height         (24)
#define layer1_conv_out_width          (24)
#define layer1_conv_input_size         (layer1_conv_input_height * layer1_conv_input_width * layer1_conv_input_channels)
#define layer1_conv_input_channels_pad ((layer1_conv_input_channels + 2 - 1) & ~(2 - 1))
#define layer1_conv_kernel_size \
    (layer1_conv_kernel_height * layer1_conv_kernel_width * layer1_conv_out_channels * layer1_conv_input_channels_pad)
#define layer1_conv_bias_size   (layer1_conv_out_channels)
#define layer1_conv_output_size (layer1_conv_out_height * layer1_conv_out_width * layer1_conv_out_channels)

// LAYER 2 RELU
#define layer2_relu_size (layer1_conv_output_size)

// LAYER 3 MAX POOLING
#define layer3_maxpool_input_height   (24)
#define layer3_maxpool_input_width    (24)
#define layer3_maxpool_input_channels (20)
#define layer3_maxpool_kernel_height  (2)
#define layer3_maxpool_kernel_width   (2)
#define layer3_maxpool_x_stride       (2)
#define layer3_maxpool_y_stride       (2)
#define layer3_maxpool_x_padding      (0)
#define layer3_maxpool_y_padding      (0)
#define layer3_maxpool_out_height     (12)
#define layer3_maxpool_out_width      (12)
#define layer3_maxpool_input_size \
    (layer3_maxpool_input_height * layer3_maxpool_input_width * layer3_maxpool_input_channels)
#define layer3_maxpool_output_size \
    (layer3_maxpool_out_height * layer3_maxpool_out_width * layer3_maxpool_input_channels)

// LAYER 4 CONVOLUTION STANDARD
#define layer4_conv_input_height       (12)
#define layer4_conv_input_width        (12)
#define layer4_conv_input_channels     (20)
#define layer4_conv_kernel_height      (5)
#define layer4_conv_kernel_width       (5)
#define layer4_conv_out_channels       (50)
#define layer4_conv_x_stride           (1)
#define layer4_conv_y_stride           (1)
#define layer4_conv_x_padding          (0)
#define layer4_conv_y_padding          (0)
#define layer4_conv_out_height         (8)
#define layer4_conv_out_width          (8)
#define layer4_conv_input_size         (layer4_conv_input_height * layer4_conv_input_width * layer4_conv_input_channels)
#define layer4_conv_input_channels_pad ((layer4_conv_input_channels + 2 - 1) & ~(2 - 1))
#define layer4_conv_kernel_size \
    (layer4_conv_kernel_height * layer4_conv_kernel_width * layer4_conv_out_channels * layer4_conv_input_channels_pad)
#define layer4_conv_bias_size   (layer4_conv_out_channels)
#define layer4_conv_output_size (layer4_conv_out_height * layer4_conv_out_width * layer4_conv_out_channels)

// LAYER 5 RELU
#define layer5_relu_size (layer4_conv_output_size)

// LAYER 6 MAX POOLING
#define layer6_maxpool_input_height   (8)
#define layer6_maxpool_input_width    (8)
#define layer6_maxpool_input_channels (50)
#define layer6_maxpool_kernel_height  (2)
#define layer6_maxpool_kernel_width   (2)
#define layer6_maxpool_x_stride       (2)
#define layer6_maxpool_y_stride       (2)
#define layer6_maxpool_x_padding      (0)
#define layer6_maxpool_y_padding      (0)
#define layer6_maxpool_out_height     (4)
#define layer6_maxpool_out_width      (4)
#define layer6_maxpool_input_size \
    (layer6_maxpool_input_height * layer6_maxpool_input_width * layer6_maxpool_input_channels)
#define layer6_maxpool_output_size \
    (layer6_maxpool_out_height * layer6_maxpool_out_width * layer6_maxpool_input_channels)

// LAYER 7 FULLY CONNECTED
#define layer7_fc_input_size   (layer6_maxpool_out_height * layer6_maxpool_out_width * layer6_maxpool_input_channels)
#define layer7_fc_output_size  (500)
#define layer7_fc_weights_size (layer7_fc_input_size * layer7_fc_output_size)
#define layer7_fc_bias_size    (layer7_fc_output_size)

// LAYER 8 RELU
#define layer8_relu_size (layer7_fc_output_size)

// LAYER 9 FULLY CONNECTED
#define layer9_fc_input_size   (layer8_relu_size)
#define layer9_fc_output_size  (10)
#define layer9_fc_weights_size (layer9_fc_input_size * layer9_fc_output_size)
#define layer9_fc_bias_size    (layer9_fc_output_size)

// LAYER 10 SOFTMAX
#define layer10_softmax_size (layer9_fc_output_size)

#endif // __LENET_MODEL_H__
