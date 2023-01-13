/*
 * Copyright 2021-2022 NXP
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

/* enable TFlite by default */
#if !defined(INFERENCE_ENGINE_GLOW) && !defined(INFERENCE_ENGINE_DeepViewRT)
#define HAL_ENABLE_INFERENCE_TFLITE 1
#define HAL_ENABLE_INFERENCE_GLOW 0
#define HAL_ENABLE_INFERENCE_DVRT 0
#elif defined(INFERENCE_ENGINE_GLOW)
#define HAL_ENABLE_INFERENCE_TFLITE 0
#define HAL_ENABLE_INFERENCE_GLOW 1
#define HAL_ENABLE_INFERENCE_DVRT 0
#elif defined(INFERENCE_ENGINE_DeepViewRT)
#define HAL_ENABLE_INFERENCE_TFLITE 0
#define HAL_ENABLE_INFERENCE_GLOW 0
#define HAL_ENABLE_INFERENCE_DVRT 1
#endif

/* Log level configuration
 * ERR:   0
 * INFO:  1
 * DEBUG: 2
 */
#ifndef HAL_LOG_LEVEL
#define HAL_LOG_LEVEL 0
#endif

/**
 *  Mutex lock timeout definition
 *  An arbitrary default value is defined to 5 seconds
 *  value unit should be milliseconds
 * */
#define MAX_MUTEX_TIME_MS   (5000)

/* The size of Tensor Arena buffer for TensorFlowLite-Micro */
/* minimum required arena size for MobileNetv1 */
#define HAL_TFLM_TENSOR_ARENA_SIZE_KB 512

/* The memory size used for weights and activations when using glow inference with Mobilenet v1,
 * these macros should be adjusted when using another model*/
#define GLOW_CONSTANT_WEIGHTS_MAX_MEMSIZE 479168
#define GLOW_MUTABLE_WEIGHTS_MAX_MEMSIZE  53184
#define GLOW_ACTIVATIONS_MAX_MEMSIZE      98368

/* The mempool holds DeepViewRT intermediate buffers that can be multiple megabytes in size and therefore
 * should be stored in the SDRAM. It's allocated in the heap (not FreeRTOS's heap) */
#define HAL_DVRT_MEMPOOL_SIZE_MB 5

#endif /* _MPP_CONFIG_H */
