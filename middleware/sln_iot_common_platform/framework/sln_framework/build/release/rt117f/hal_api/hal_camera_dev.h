/*
 * Copyright 2020-2021 NXP.
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

#include "fwk_common.h"

typedef struct _camera_dev camera_dev_t;

typedef enum _hal_camera_status
{
    kStatus_HAL_CameraSuccess = 0,                                                       /*!< Successfully */
    kStatus_HAL_CameraBusy    = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Camera, 1), /*!< Camera is busy */
    kStatus_HAL_CameraNonBlocking, /*!< Camera will return immediately */
    kStatus_HAL_CameraError        /*!< Error occurs on HAL Camera */

} hal_camera_status_t;

/*! @brief Type of events that are supported by calling the callback function */
typedef enum _camera_event
{
    /* Camera new frame is available */
    kCameraEvent_SendFrame = MAKE_FRAMEWORK_EVENTS(kStatusFrameworkGroups_Camera, 1),
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
    hal_camera_status_t (*init)(camera_dev_t *dev, int width, int height, camera_dev_callback_t callback, void *param);
    /* deinitialize the dev */
    hal_camera_status_t (*deinit)(camera_dev_t *dev);
    /* start the dev */
    hal_camera_status_t (*start)(const camera_dev_t *dev);
    /* enqueue a buffer to the dev */
    hal_camera_status_t (*enqueue)(const camera_dev_t *dev, void *data);
    /* dequeue a buffer from the dev */
    hal_camera_status_t (*dequeue)(const camera_dev_t *dev, void **data, pixel_format_t *format);
    /* postProcess a buffer from the dev */
    /*
    * Only do the minimum determination(data point and the format) of the frame in the dequeue.
    *
    * And split the CPU based post process(IR/Depth/... processing) to postProcess as they will eat CPU
    * which is critical for the whole system as camera manager is running with the highest priority.
    *
    * Camera manager will do the postProcess if there is a consumer of this frame.
    *
    * Note:
    * Camera manager will call multiple times of the posProcess of the same frame determinted by dequeue.
    * The HAL driver needs to guarantee the postProcess only do once for the first call.
    *
    */
    hal_camera_status_t (*postProcess)(const camera_dev_t *dev, void **data, pixel_format_t *format);
    /* input notify */
    hal_camera_status_t (*inputNotify)(const camera_dev_t *dev, void *data);
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
    cw_rotate_degree_t rotate;
    /* flip */
    flip_mode_t flip;
    /* swap byte per two bytes */
    int swapByte;
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
    /* unique id which is assigned by camera manager during the registration */
    int id;
    /* state in which the device is found */
    hal_device_state_t state;
    /* name of the device */
    char name[DEVICE_NAME_MAX_LENGTH];

    /* operations */
    const camera_dev_operator_t *ops;
    /* static configs */
    camera_dev_static_config_t config;
    /* private capability */
    camera_dev_private_capability_t cap;
};

#endif /*_HAL_CAMERA_DEV_H_*/
