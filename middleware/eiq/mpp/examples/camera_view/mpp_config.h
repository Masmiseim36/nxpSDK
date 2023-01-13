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
#define HAL_ENABLE_INFERENCE_TFLITE 0
#define HAL_ENABLE_INFERENCE_GLOW 0

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
#endif /* _MPP_CONFIG_H */
