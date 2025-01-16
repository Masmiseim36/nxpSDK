/* Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MPP_CONFIG_H
#define _MPP_CONFIG_H

/* This header configures the MPP HAL and the application according to the board model */

/*******************************************************************************
 * HAL configuration (Mandatory)
 ******************************************************************************/

/**
 * This is the mimxrt700evk board configuration
 * Disabling HAL of unused/missing devices saves memory
 */

#define HAL_ENABLE_DISPLAY
#define HAL_ENABLE_DISPLAY_DEV_Lcdifv2Rk055   1
#define HAL_ENABLE_2D_IMGPROC

/* use GPU backend */
#define HAL_ENABLE_GFX_DEV_Pxp                0
#define HAL_ENABLE_GFX_DEV_Cpu                0
#define HAL_ENABLE_GFX_DEV_GPU                1

/* use TFlite micro inference engine for this application */
#define HAL_ENABLE_INFERENCE_TFLITE           1

/**
 * This is the inference HAL configuration
 */

/* The size of Tensor Arena buffer for TensorFlowLite-Micro */
/* minimum required arena size for persondetect converted for NPU */
#define HAL_TFLM_TENSOR_ARENA_SIZE_KB         550

/*
 * TFLite tensor arena buffer alignment requirement:
 * TFLite input buffer allocation is not dynamically controlled by the pipeline.
 * Thus, set HAL_TFLITE_BUFFER_ALIGN to 64B when using GPU backend as its output buffer
 * is 64B aligned.
 * Default value is 16Bytes.
 */
#define HAL_TFLITE_BUFFER_ALIGN               64

/*
 * Enable this flag to define TFlite tensor arena non-cacheable.
 * Allocating tensor arena in non-cacheable memory may improve performance of
 * operators executed on NPU, but decreases performance on CPU.
 */
#define HAL_TENSOR_ARENA_NCACHE               0

/**
 * VGLite heap size for MIMXRT700 EVK.
 */
#define HAL_VGLITE_HEAP_SZ                    0x100000 /* 1 MB */
#define HAL_VGLITE_BUFFER_ALIGN               64

/* GPU chip ID for MIMXRT700EVK board.*/
#define HAL_GPU_CHIPID                        0x555

/**
 * This is HAL debug configuration
 */

/*
 * Log level configuration
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
#define HAL_MUTEX_TIMEOUT_MS   (5000)

/*******************************************************************************
 * Application configuration (Optional)
 ******************************************************************************/

/* Set here all the static configuration of the Application */

/* display parameters */
#define APP_DISPLAY_NAME                      "Lcdifv2Rk055"
#define APP_DISPLAY_WIDTH                     720
#define APP_DISPLAY_HEIGHT                    1280
#define APP_DISPLAY_FORMAT                    MPP_PIXEL_RGB565

/* other parameters */
/* rotation is needed to display in landscape because display RK055 is portrait */
#define APP_DISPLAY_LANDSCAPE_ROTATE          ROTATE_90

/* detection boxes params */
/* maximum number of boxes stored in RAM by APP (1box ~= 16B) */
#define APP_MAX_BOXES 10000

/* select inference model converted for NPU */
#define APP_USE_NEUTRON64_MODEL

#define APP_GFX_BACKEND_NAME "gfx_GPU"

/* Tensorflow lite Model data */
#ifdef USE_NAS_OPTIMIZED_MODEL
#define APP_TFLITE_PERSONDETECT_DATA "persondetect_220x220_npu64_tflite.h"
#define APP_TFLITE_PERSONDETECT_INFO "persondetect_220_220_npu64_tflite_info.h"
#else
#define APP_TFLITE_PERSONDETECT_DATA "persondetect_160_128_npu64_tflite.h"
#define APP_TFLITE_PERSONDETECT_INFO "persondetect_160_128_npu64_tflite_info.h"
#endif

/* define a static image that will be included if needed */
#define APP_STATIC_IMAGE_NAME "skigirl_COCO_160_128_rgb.h"

#endif /* _MPP_CONFIG_H */
