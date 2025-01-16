/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MOBILENETV1_MODEL_TFLITE_NPU16_INFO_H_
#define _MOBILENETV1_MODEL_TFLITE_NPU16_INFO_H_

/* This file provides information about the TFLite model mobilenetv1_model_data_tflite_npu16.h, such as width, heigth, etc.
   Other parameters include input mean, output zero point, output scale, grid height and width,
   the number of channels, and the maximum number of boxes.
*/

#include "mpp_api_types.h"

#define MOBILENET_NAME "mobilenet_v1_0.25_128_quant_int8"
/* mean and std will be used to get input values in the model input data range.
 * for Mobilenet V1 data should be between -1 and 1. */
#define MOBILENET_INPUT_MEAN  128
#define MOBILENET_INPUT_STD   127
#define MOBILENET_WIDTH       128
#define MOBILENET_HEIGHT      128
/* output tensor properties */
#define MOBILENET_TENSOR_SIZE 1001
#define MOBILENET_TENSOR_TYPE MPP_TENSOR_TYPE_FLOAT32

#endif /* _MOBILENETV1_MODEL_TFLITE_NPU16_INFO_H_ */

