/*
 * Copyright 2024 NXP
 * Copyright 2022 xuehao.ma
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* This file provides information about the TFLite model persondetect_220_220_npu64_tflite.h, such as width, heigth, etc.
   Other parameters include input mean, output zero point, output scale, grid height and width,
   the number of channels, and the maximum number of boxes.
*/

#ifndef PERSONDETECT_220_220_NPU64_TFLITE_INFO_H
#define PERSONDETECT_220_220_NPU64_TFLITE_INFO_H

#define PERSONDETECT_NAME               "persondetect"
/* mean and std will be used to get input values in the model input data range.
 * for Persondetect data should be between -1 and 1. */
#define PERSONDETECT_INPUT_MEAN         128
#define PERSONDETECT_INPUT_STD          127
#define PERSONDETECT_WIDTH              220
#define PERSONDETECT_HEIGHT             220
#define PERSONDETECT_OUTPUT_ZERO_POINT -22.0f
#define PERSONDETECT_OUTPUT_SCALE       0.024162346497178078f
#define PERSONDETECT_GRID_HEIGHT        14 // Number of vertical cells in the output grid of Persondetect
#define PERSONDETECT_GRID_WIDTH         14
#define PERSONDETECT_CANAL              6 /* box data : Score,x, y, width, height, class  */
#define PERSONDETECT_MAX_BOXES          PERSONDETECT_GRID_HEIGHT * PERSONDETECT_GRID_WIDTH

#define PERSONDETECT_NMS_THRESHOLD      0.6f
#define PERSONDETECT_SCORE_THRESHOLD    57.0f

#endif /* PERSONDETECT_220_220_NPU64_TFLITE_INFO_H */
