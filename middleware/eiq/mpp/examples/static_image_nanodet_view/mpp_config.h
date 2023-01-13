/*
 * Copyright 2021-2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MPP_CONFIG_H
#define _MPP_CONFIG_H

/* Set here all the static configuration of the Media Processing Pipeline */

#define HAL_DISPLAY_MAX_BPP 2

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

/* enable TFlite by default */
#ifndef INFERENCE_ENGINE_GLOW
#define HAL_ENABLE_INFERENCE_TFLITE 1
#define HAL_ENABLE_INFERENCE_GLOW 0
#else
#define HAL_ENABLE_INFERENCE_TFLITE 0
#define HAL_ENABLE_INFERENCE_GLOW 1
#endif

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
/* minimum required arena size for Nanodet-M */
#define HAL_TFLM_TENSOR_ARENA_SIZE_KB 2048

/* The memory size used for weights and activations when using glow inference with Nanodet-M,
 * these macros should be adjusted when using another model*/
#define GLOW_CONSTANT_WEIGHTS_MAX_MEMSIZE 216704
#define GLOW_MUTABLE_WEIGHTS_MAX_MEMSIZE  352000
#define GLOW_ACTIVATIONS_MAX_MEMSIZE      1075200

#endif /* _MPP_CONFIG_H */
