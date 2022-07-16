/*
 * Copyright 2021-2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MPP_CONFIG_H
#define _MPP_CONFIG_H

/* Set here all the static configuration of the Media Processing Pipeline */

/* Setting TFLITE_CUSTOM_OPS_RESOLVER to 1 implies to provide an implementation of function
 * tflite::MicroOpResolver &MODEL_GetOpsResolver(tflite::ErrorReporter* errorReporter)
 * using the tensorflow-lite API.
 * Setting TFLITE_CUSTOM_OPS_RESOLVER to 0 will automatically compile all available Operations and thus increase code size.
 **/
#define TFLITE_CUSTOM_OPS_RESOLVER 1

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
