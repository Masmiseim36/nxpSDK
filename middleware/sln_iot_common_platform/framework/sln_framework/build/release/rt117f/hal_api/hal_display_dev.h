/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief hal display device declaration. Display devices can be used to display images, GUI overlays, etc.
 * Examples of display devices include display panels like the RK024hh298 display, and external displays like UVC (video
 * over USB).
 */

#ifndef _HAL_DISPLAY_DEV_H_
#define _HAL_DISPLAY_DEV_H_

#include "fwk_common.h"

typedef struct _display_dev display_dev_t;

/*! @brief Type of events that are supported by calling the callback function */
typedef enum _display_event
{
    /* Display finished sending the frame asynchronously, provide another frame */
    kDisplayEvent_RequestFrame = MAKE_FRAMEWORK_EVENTS(kStatusFrameworkGroups_Display, 1),
    kDisplayEvent_Count
} display_event_t;

/**
 * @brief callback function to notify display manager that an async event took place
 * @param dev Device structure of the display device calling this function
 * @param event id of the event that took place
 * @param param Parameters
 * @param fromISR True if this operation takes place in an irq, 0 otherwise
 * @return 0 if the operation was successfully
 */
typedef int (*display_dev_callback_t)(const display_dev_t *dev, display_event_t event, void *param, uint8_t fromISR);

/*! @brief Error codes for display hal devices */
typedef enum _hal_display_status
{
    kStatus_HAL_DisplaySuccess = 0,                                                        /*!< Successfully */
    kStatus_HAL_DisplayTxBusy  = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Display, 1), /*!< Display tx is busy */
    kStatus_HAL_DisplayNonBlocking =
        MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Display, 2), /*!< Display will return immediately */
    kStatus_HAL_DisplayError =
        MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Display, 3), /*!< Error occurs on HAL Display */
} hal_display_status_t;

/*! @brief Operation that needs to be implemented by a display device */
typedef struct _display_dev_operator
{
    /* initialize the dev */
    hal_display_status_t (*init)(
        display_dev_t *dev, int width, int height, display_dev_callback_t callback, void *param);
    /* deinitialize the dev */
    hal_display_status_t (*deinit)(const display_dev_t *dev);
    /* start the dev */
    hal_display_status_t (*start)(const display_dev_t *dev);
    /* blit a buffer to the dev */
    hal_display_status_t (*blit)(const display_dev_t *dev, void *frame, int width, int height);
    /* input notify */
    hal_display_status_t (*inputNotify)(const display_dev_t *dev, void *data);
} display_dev_operator_t;

/*! @brief Structure that characterize the display device. */
typedef struct _display_dev_private_capability
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
    /* pixel format */
    pixel_format_t format;
    /* the source pixel format of the requested frame */
    pixel_format_t srcFormat;
    void *frameBuffer;
    /* callback */
    display_dev_callback_t callback;
    /* param for the callback */
    void *param;
} display_dev_private_capability_t;

/*! @brief Attributes of a display device. */
struct _display_dev
{
    /* unique id which is assigned by camera manager during the registration */
    int id;
    /* name of the device */
    char name[DEVICE_NAME_MAX_LENGTH];
    /* operations */
    const display_dev_operator_t *ops;
    /* private capability */
    display_dev_private_capability_t cap;
};

#endif /*_HAL_DISPLAY_DEV_H_*/
