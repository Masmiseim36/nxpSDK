/*
 * Copyright 2020-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief hal camera device declaration. Camera devices can enqueue and dequeue frames as well as react to events from
 * input devices via the "inputNotify" function. Camera devices can use any number of interfaces, including MIPI and CSI
 * as long as the HAL driver implements the necessary functions found in camera_dev_operator_t.
 * Examples of camera devices include the Orbbec U1S 3D SLM camera module, and the OnSemi MT9M114 camera module.
 */

#ifndef _HAL_CAMERA_DEV_H_
#define _HAL_CAMERA_DEV_H_

#include "hal.h"
#include "mpp_api_types.h"
#include "fsl_video_common.h"
#include "hal_types.h"

/**
 * @brief declare the camera dev ##name
 */
#define HAL_CAMERA_DEV_DECLARE(name) int HAL_CameraDev_##name##_Register();

/**
 * @brief register the camera dev ##name
 */
#define HAL_CAMERA_DEV_REGISTER(name, ret)                             \
    {                                                                  \
        ret = HAL_CameraDev_##name##_Register();                       \
        if (ret != 0)                                                  \
        {                                                              \
            HAL_LOGE("HAL_CameraDev_%s_Register error %d\n", "##name", ret); \
            return ret;                                                \
        }                                                              \
    }

typedef struct _camera_dev camera_dev_t;

typedef enum _hal_camera_status
{
    kStatus_HAL_CameraSuccess = 0,  /*!< Successfully */
    kStatus_HAL_CameraBusy,         /*!< Camera is busy */
    kStatus_HAL_CameraNonBlocking,  /*!< Camera will return immediately */
    kStatus_HAL_CameraError         /*!< Error occurs on HAL Camera */
} hal_camera_status_t;

/*! @brief Type of events that are supported by calling the callback function */
typedef enum _camera_event
{
    /* Camera new frame is available */
    kCameraEvent_SendFrame,
    /* Camera device finished the init process */
    kCameraEvent_CameraDeviceInit,
    kCameraEvent_Count
} camera_event_t;

/**
 * @brief Callback function to notify camera manager that one frame is dequeued
 * @param dev Device structure of the camera device calling this function
 * @param event id of the event that took place
 * @param param Parameters
 * @param fromISR True if this operation takes place in an irq, 0 otherwise
 * @return 0 if the operation was successfully
 */
typedef int (*camera_dev_callback_t)(const camera_dev_t *dev, camera_event_t event, void *param, uint8_t fromISR);

/*! @brief Operation that needs to be implemented by a camera device */
typedef struct _camera_dev_operator
{
    /* initialize the dev */
    hal_camera_status_t (*init)(camera_dev_t *dev, mpp_camera_params_t *config, camera_dev_callback_t callback, void *param);
    /* deinitialize the dev */
    hal_camera_status_t (*deinit)(camera_dev_t *dev);
    /* start the dev */
    hal_camera_status_t (*start)(const camera_dev_t *dev);
    /* stop the dev */
    hal_camera_status_t (*stop)(const camera_dev_t *dev);
    /* enqueue a buffer to the dev */
    hal_camera_status_t (*enqueue)(const camera_dev_t *dev, void *data);
    /* dequeue a buffer from the dev */
    hal_camera_status_t (*dequeue)(const camera_dev_t *dev, void **data, mpp_pixel_format_t *format);
    /* get buffer descriptors and policy */
    hal_camera_status_t (*get_buf_desc)(const camera_dev_t *dev, hw_buf_desc_t *out_buf, mpp_memory_policy_t *policy);
} camera_dev_operator_t;

/*! @brief Structure that characterize the camera device. */
typedef struct
{
    /* buffer resolution */
    int height;
    int width;
    int pitch;
    /* active rect */
    int left;
    int top;
    int right;
    int bottom;
    /* rotate degree */
    mpp_rotate_degree_t rotate;
    /* flip */
    mpp_flip_mode_t flip;
    /* swap byte per two bytes */
    int swapByte;
    video_pixel_format_t format;
    int framerate;
} camera_dev_static_config_t;

typedef struct
{
    /* callback */
    camera_dev_callback_t callback;
    /* param for the callback */
    void *param;
} camera_dev_private_capability_t;

/*! @brief Attributes of a camera device. */
struct _camera_dev
{
    /* unique id which is assigned by camera manager during registration */
    int id;
    /* name of the device */
    char name[MPP_DEVICE_NAME_MAX_LENGTH];

    /* operations */
    const camera_dev_operator_t *ops;
    /* static configs */
    camera_dev_static_config_t config;
    /* private capability */
    camera_dev_private_capability_t cap;
};

#endif /*_HAL_CAMERA_DEV_H_*/
