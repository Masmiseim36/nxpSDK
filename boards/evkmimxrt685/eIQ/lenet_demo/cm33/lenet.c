/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdlib.h>
#include <string.h>
#include <fsl_common.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "lenet.h"
#include "dsp_ipc.h"
#include "dsp_nn_utils.h"
#include "dsp_nn.h"

/*******************************************************************************
 * DATA
 ******************************************************************************/

// MODEL
#include "lenet_model.h"

// WEIGHTS
#include "conv1_W.h"
#include "conv1_b.h"
#include "conv2_W.h"
#include "conv2_b.h"
#include "fc1_W.h"
#include "fc1_b.h"
#include "fc2_W.h"
#include "fc2_b.h"

/*******************************************************************************
 * CODE
 ******************************************************************************/

#ifdef OPTIMIZE_ASYNC
static void nn_cb(void *params, srtm_message *msg)
{
    QueueHandle_t queue = (QueueHandle_t)params;
    xQueueSend(queue, msg, portMAX_DELAY);
}
#endif

int lenet_init(ml_lenet_t *handle)
{
#ifdef OPTIMIZE_ASYNC
    handle->queue = xQueueCreate(15, sizeof(srtm_message));
    vQueueAddToRegistry(handle->queue, "lenet->queue");
#endif

    int layer1_conv_scratch_size = xa_nn_conv2d_std_getsize(
        layer1_conv_input_height, layer1_conv_input_channels, layer1_conv_kernel_height, layer1_conv_kernel_width,
        layer1_conv_y_stride, layer1_conv_y_padding, layer1_conv_out_height, data_precision);

    int layer3_maxpool_scratch_size = xa_nn_maxpool_getsize(
        layer3_maxpool_input_channels, data_precision, data_precision, layer3_maxpool_input_height,
        layer3_maxpool_input_width, layer3_maxpool_kernel_height, layer3_maxpool_kernel_width, layer3_maxpool_x_stride,
        layer3_maxpool_y_stride, layer3_maxpool_x_padding, layer3_maxpool_y_padding, layer3_maxpool_out_height,
        layer3_maxpool_out_width, 1, 1);

    int layer4_conv_scratch_size = xa_nn_conv2d_std_getsize(
        layer4_conv_input_height, layer4_conv_input_channels, layer4_conv_kernel_height, layer4_conv_kernel_width,
        layer4_conv_y_stride, layer4_conv_y_padding, layer4_conv_out_height, data_precision);

    int layer6_maxpool_scratch_size = xa_nn_maxpool_getsize(
        layer6_maxpool_input_channels, data_precision, data_precision, layer6_maxpool_input_height,
        layer6_maxpool_input_width, layer6_maxpool_kernel_height, layer6_maxpool_kernel_width, layer6_maxpool_x_stride,
        layer6_maxpool_y_stride, layer6_maxpool_x_padding, layer6_maxpool_y_padding, layer6_maxpool_out_height,
        layer6_maxpool_out_width, 1, 1);

    int scratch_size  = 0;
    scratch_size      = MAX(scratch_size, layer1_conv_scratch_size);
    scratch_size      = MAX(scratch_size, layer3_maxpool_scratch_size);
    scratch_size      = MAX(scratch_size, layer4_conv_scratch_size);
    scratch_size      = MAX(scratch_size, layer6_maxpool_scratch_size);
    handle->p_scratch = xa_nn_malloc(scratch_size);

    // INPUT/OUTPUT BUFFERS
    /**
     * buffer0 will hold the inputs of layers 1, 3, 5, 7, 9, 11 and outputs of layers 2, 4, 6, 8, 10
     * buffer1 will hold the inputs of layers 2, 4, 6, 8, 10 and outputs of layers 1, 3, 5, 7, 9, 11
     */
    int buffer0_size = 0;
    buffer0_size     = MAX(buffer0_size, layer1_conv_input_size);     // INPUT
    buffer0_size     = MAX(buffer0_size, layer2_relu_size);           // OUTPUT
    buffer0_size     = MAX(buffer0_size, layer3_maxpool_input_size);  // INPUT
    buffer0_size     = MAX(buffer0_size, layer4_conv_output_size);    // OUTPUT
    buffer0_size     = MAX(buffer0_size, layer5_relu_size);           // INPUT
    buffer0_size     = MAX(buffer0_size, layer6_maxpool_output_size); // OUTPUT
    buffer0_size     = MAX(buffer0_size, layer7_fc_input_size);       // INPUT
    buffer0_size     = MAX(buffer0_size, layer8_relu_size);           // OUTPUT
    buffer0_size     = MAX(buffer0_size, layer9_fc_input_size);       // INPUT
    buffer0_size *= data_width;

    int buffer1_size = 0;
    buffer1_size     = MAX(buffer1_size, layer1_conv_output_size);    // OUTPUT
    buffer1_size     = MAX(buffer1_size, layer2_relu_size);           // INPUT
    buffer1_size     = MAX(buffer1_size, layer3_maxpool_output_size); // OUTPUT
    buffer1_size     = MAX(buffer1_size, layer4_conv_input_size);     // INPUT
    buffer1_size     = MAX(buffer1_size, layer5_relu_size);           // OUTPUT
    buffer1_size     = MAX(buffer1_size, layer6_maxpool_input_size);  // INPUT
    buffer1_size     = MAX(buffer1_size, layer7_fc_output_size);      // OUTPUT
    buffer1_size     = MAX(buffer1_size, layer8_relu_size);           // INPUT
    buffer1_size     = MAX(buffer1_size, layer9_fc_output_size);      // OUTPUT
    buffer1_size *= data_width;

    handle->p_buffer0 = (float *)SDK_Malloc(buffer0_size, 8);
    handle->p_buffer1 = (float *)SDK_Malloc(buffer1_size, 8);

    handle->p_layer1_kernel  = (float *)conv1_W;
    handle->p_layer1_bias    = (float *)conv1_b;
    handle->p_layer4_kernel  = (float *)conv2_W;
    handle->p_layer4_bias    = (float *)conv2_b;
    handle->p_layer7_weights = (float *)fc1_W;
    handle->p_layer7_bias    = (float *)fc1_b;
    handle->p_layer9_weights = (float *)fc2_W;
    handle->p_layer9_bias    = (float *)fc2_b;

    return 0;
}

int lenet_feed_input(ml_lenet_t *handle, void *input)
{
    // LAYER 0 INPUT
    memcpy(handle->p_buffer0, input, layer1_conv_input_size * data_width);

    return 0;
}

int lenet_process(ml_lenet_t *handle, unsigned char *output)
{
    int i;
    void *p_kernel;
    void *p_bias;
#ifdef OPTIMIZE_ASYNC
    srtm_message msg;
#endif

    // LAYER 1 CONVOLUTION STANDARD
    p_kernel = handle->p_layer1_kernel;
    p_bias   = handle->p_layer1_bias;

#ifdef OPTIMIZE_ASYNC
    xa_nn_conv2d_std_f32_async(nn_cb, handle->queue,
#else
    xa_nn_conv2d_std_f32(
#endif
                               handle->p_buffer1, // OUTPUT
                               handle->p_buffer0, // INPUT
                               p_kernel, p_bias, layer1_conv_input_height, layer1_conv_input_width,
                               layer1_conv_input_channels, layer1_conv_kernel_height, layer1_conv_kernel_width,
                               layer1_conv_out_channels, layer1_conv_x_stride, layer1_conv_y_stride,
                               layer1_conv_x_padding, layer1_conv_y_padding, layer1_conv_out_height,
                               layer1_conv_out_width, 0, handle->p_scratch);

    // LAYER 2 RELU
#ifdef OPTIMIZE_ASYNC
    xa_nn_vec_relu_std_f32_f32_async(nn_cb, handle->queue,
#else
    xa_nn_vec_relu_std_f32_f32(
#endif
                                 handle->p_buffer0, // OUTPUT
                                 handle->p_buffer1, // INPUT
                                 layer2_relu_size);

    // LAYER 3 MAX POOLING
#ifdef OPTIMIZE_ASYNC
    xa_nn_maxpool_f32_async(nn_cb, handle->queue,
#else
    xa_nn_maxpool_f32(
#endif
                            handle->p_buffer1, // OUTPUT
                            handle->p_buffer0, // INPUT
                            layer3_maxpool_input_height, layer3_maxpool_input_width, layer3_maxpool_input_channels,
                            layer3_maxpool_kernel_height, layer3_maxpool_kernel_width, layer3_maxpool_x_stride,
                            layer3_maxpool_y_stride, layer3_maxpool_x_padding, layer3_maxpool_y_padding,
                            layer3_maxpool_out_height, layer3_maxpool_out_width, 0, 0, handle->p_scratch);

    // LAYER 4 CONVOLUTION STANDARD
    p_kernel = handle->p_layer4_kernel;
    p_bias   = handle->p_layer4_bias;

#ifdef OPTIMIZE_ASYNC
    xa_nn_conv2d_std_f32_async(nn_cb, handle->queue,
#else
    xa_nn_conv2d_std_f32(
#endif
                               handle->p_buffer0, // OUTPUT
                               handle->p_buffer1, // INPUT
                               p_kernel, p_bias, layer4_conv_input_height, layer4_conv_input_width,
                               layer4_conv_input_channels, layer4_conv_kernel_height, layer4_conv_kernel_width,
                               layer4_conv_out_channels, layer4_conv_x_stride, layer4_conv_y_stride,
                               layer4_conv_x_padding, layer4_conv_y_padding, layer4_conv_out_height,
                               layer4_conv_out_width, 0, handle->p_scratch);

    // LAYER 5 RELU
#ifdef OPTIMIZE_ASYNC
    xa_nn_vec_relu_std_f32_f32_async(nn_cb, handle->queue,
#else
    xa_nn_vec_relu_std_f32_f32(
#endif
                                 handle->p_buffer1, // OUTPUT
                                 handle->p_buffer0, // INPUT
                                 layer5_relu_size);

    // LAYER 6 MAX POOLING
#ifdef OPTIMIZE_ASYNC
    xa_nn_maxpool_f32_async(nn_cb, handle->queue,
#else
    xa_nn_maxpool_f32(
#endif
                            handle->p_buffer0, // OUTPUT
                            handle->p_buffer1, // INPUT
                            layer6_maxpool_input_height, layer6_maxpool_input_width, layer6_maxpool_input_channels,
                            layer6_maxpool_kernel_height, layer6_maxpool_kernel_width, layer6_maxpool_x_stride,
                            layer6_maxpool_y_stride, layer6_maxpool_x_padding, layer6_maxpool_y_padding,
                            layer6_maxpool_out_height, layer6_maxpool_out_width, 0, 0, handle->p_scratch);

    // LAYER 7 FULLY CONNECTED
    p_kernel = handle->p_layer7_weights;
    p_bias   = handle->p_layer7_bias;

#ifdef OPTIMIZE_ASYNC
    xa_nn_fully_connected_f32_async(nn_cb, handle->queue,
#else
    xa_nn_fully_connected_f32(
#endif
                                    handle->p_buffer1, // OUTPUT
                                    p_kernel,
                                    handle->p_buffer0, // INPUT
                                    p_bias, layer7_fc_input_size, layer7_fc_output_size);

    // LAYER 8 RELU
#ifdef OPTIMIZE_ASYNC
    xa_nn_vec_relu_std_f32_f32_async(nn_cb, handle->queue,
#else
    xa_nn_vec_relu_std_f32_f32(
#endif
                                 handle->p_buffer0, // OUTPUT
                                 handle->p_buffer1, // INPUT
                                 layer8_relu_size);

    // LAYER 9 FULLY CONNECTED
    p_kernel = handle->p_layer9_weights;
    p_bias   = handle->p_layer9_bias;

#ifdef OPTIMIZE_ASYNC
    xa_nn_fully_connected_f32_async(nn_cb, handle->queue,
#else
    xa_nn_fully_connected_f32(
#endif
                                    handle->p_buffer1, // OUTPUT
                                    p_kernel,
                                    handle->p_buffer0, // INPUT
                                    p_bias, layer9_fc_input_size, layer9_fc_output_size);

    // LAYER 10 SOFTMAX
#ifdef OPTIMIZE_ASYNC
    xa_nn_vec_softmax_f32_f32_async(nn_cb, handle->queue,
#else
    xa_nn_vec_softmax_f32_f32(
#endif
                                    handle->p_buffer0, // OUTPUT
                                    handle->p_buffer1, // INPUT
                                    layer10_softmax_size);

    /**************************************************************************
     * OUTPUT
     *************************************************************************/

#ifdef OPTIMIZE_ASYNC
    for (i = 0; i < 10; i++)
        if (xQueueReceive(handle->queue, &msg, portMAX_DELAY) == pdFALSE)
            break;
#endif

    float prob                = handle->p_buffer0[0];
    unsigned char best_prob_i = 0;
    for (i = 1; i < layer10_softmax_size; i++)
    {
        if (prob < handle->p_buffer0[i])
        {
            prob        = handle->p_buffer0[i];
            best_prob_i = i;
        }
    }
    *output = best_prob_i;

    return 0;
}

int lenet_deinit(ml_lenet_t *handle)
{
#ifdef OPTIMIZE_ASYNC
    vQueueUnregisterQueue(handle->queue);
    vQueueDelete(handle->queue);
#endif

    SDK_Free(handle->p_buffer0);
    SDK_Free(handle->p_buffer1);
    xa_nn_free(handle->p_scratch);

    return 0;
}
