/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __LENET_H__
#define __LENET_H__

//#define OPTIMIZE_ASYNC

typedef struct _ml_lenet
{
#ifdef OPTIMIZE_ASYNC
    QueueHandle_t queue;
#endif // OPTIMIZE_ASYNC
    float *p_buffer0;
    float *p_buffer1;
    float *p_layer1_kernel;
    float *p_layer1_bias;
    float *p_layer4_kernel;
    float *p_layer4_bias;
    float *p_layer7_weights;
    float *p_layer7_bias;
    float *p_layer9_weights;
    float *p_layer9_bias;
    float *p_layer13_weights;
    float *p_layer13_bias;
    float *p_scratch;

} ml_lenet_t;

int lenet_init(ml_lenet_t *handle);
int lenet_feed_input(ml_lenet_t *handle, void *input);
int lenet_process(ml_lenet_t *handle, unsigned char *output);
int lenet_deinit(ml_lenet_t *handle);

#endif // __LENET_H__
