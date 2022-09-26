/*
 * Copyright 2021-2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MPP_CONFIG_H
#define _MPP_CONFIG_H

/* Set here all the static configuration of the Media Processing Pipeline */

#define ENABLE_CAMERA_DEV_MipiOv5640 1
#define ENABLE_DISPLAY_DEV_Lcdifv2Rk055ah 1
#define ENABLE_GFX_DEV_Pxp 1

/* Log level configuration
 * ERR:   0
 * INFO:  1
 * DEBUG: 2
 */
#ifndef HAL_LOG_LEVEL
#define HAL_LOG_LEVEL 0
#endif

/* MPP version */
#define MPP_VERSION_MAJOR 0
#define MPP_VERSION_MINOR 9
#ifdef MPP_COMMIT
#define MPP_VERSION_COMMIT MPP_COMMIT
#else
#define MPP_VERSION_COMMIT 0
#endif

/* Workaround for the PXP bug (iMXRT1170) where BGR888 is output instead
 * of RGB888 [MPP-97].
 */
#ifndef IMXRT1170_PXP_WORKAROUND_OUT_RGB
#define IMXRT1170_PXP_WORKAROUND_OUT_RGB 1
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
#ifndef HAL_TFLM_TENSOR_ARENA_SIZE_KB
/* minimum required arena size for MobileNetv1 */
#define HAL_TFLM_TENSOR_ARENA_SIZE_KB 512
#endif
/* The memory size used for weights and activations when using glow inference with Mobilenet v1,
 * these macros should be adjusted when using another model*/
#ifndef GLOW_CONSTANT_WEIGHTS_MAX_MEMSIZE
#define GLOW_CONSTANT_WEIGHTS_MAX_MEMSIZE 515328
#endif
#ifndef GLOW_MUTABLE_WEIGHTS_MAX_MEMSIZE
#define GLOW_MUTABLE_WEIGHTS_MAX_MEMSIZE  53184
#endif
#ifndef GLOW_ACTIVATIONS_MAX_MEMSIZE
#define GLOW_ACTIVATIONS_MAX_MEMSIZE      98304
#endif
#endif /* _MPP_CONFIG_H */
