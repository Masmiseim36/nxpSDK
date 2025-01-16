/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _ULTRAFACE_SLIM_TFLITE_INFO_H_
#define _ULTRAFACE_SLIM_TFLITE_INFO_H_

#define ULTRAFACE_NAME "version_slim_int8_tflite"
/* mean and std will be used to get input values in the model input data range.
 * for Ultraface data should be between -1 and 1. */
#define ULTRAFACE_INPUT_MEAN   128
#define ULTRAFACE_INPUT_STD    127
#define ULTRAFACE_WIDTH        320
#define ULTRAFACE_HEIGHT       240

/* ULTRAFACE_OUTPUT_SCALE and ULTRAFACE_OUTPUT_ZERO_POINT are the model quantization parameters
 * and can be found by visualizing the model using netron */
#define ULTRAFACE_OUTPUT_SCALE      0.00392156f
#define ULTRAFACE_OUTPUT_ZERO_POINT -128.0f

#endif /* _ULTRAFACE_SLIM_TFLITE_INFO_H_ */
