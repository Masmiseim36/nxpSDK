/*
 * Copyright 2021-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MPP_CONFIG_H
#define _MPP_CONFIG_H

/* Set here all the static configuration of the Media Processing Pipeline */
#define HAL_DISPLAY_MAX_BPP 2

#define HAL_ENABLE_CAMERA
#define HAL_ENABLE_DISPLAY
#define HAL_ENABLE_2D_IMGPROC

/* Log level configuration
 * ERR:   0
 * INFO:  1
 * DEBUG: 2
 */
#ifndef HAL_LOG_LEVEL
#define HAL_LOG_LEVEL 0
#endif


/* This example uses TFlite inference engine */
#define HAL_ENABLE_INFERENCE_TFLITE 1
#define HAL_ENABLE_INFERENCE_GLOW 0
#define HAL_ENABLE_INFERENCE_DVRT 0

/**
 *  Mutex lock timeout definition
 *  An arbitrary default value is defined to 5 seconds
 *  user can modify it at build time by adding the flag
 *       -DCONFIG_MAX_MUTEX_TIME_MS=value
 *  value unit should be milliseconds
 * */
#ifdef CONFIG_MAX_MUTEX_TIME_MS
#define MAX_MUTEX_TIME_MS   CONFIG_MAX_MUTEX_TIME_MS
#else
#define MAX_MUTEX_TIME_MS   (5000)
#endif /* CONFIG_MAX_MUTEX_TIME_MS */

#define HAL_MAX_MUTEX_TIME_MS   (MAX_MUTEX_TIME_MS / portTICK_PERIOD_MS)

/* The size of Tensor Arena buffer for TensorFlowLite-Micro */
/* minimum required arena size for Ultraface is 1536*/
#define HAL_TFLM_TENSOR_ARENA_SIZE_KB 1536

#endif /* _MPP_CONFIG_H */
