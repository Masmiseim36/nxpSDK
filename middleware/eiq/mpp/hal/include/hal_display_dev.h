/*
 * Copyright 2020-2022 NXP.
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

#include "mpp_api_types.h"
#include "hal_types.h"

/**
 * @brief declare the display dev ##name
 */
#define HAL_DISPLAY_DEV_DECLARE(name) int HAL_DisplayDev_##name##_Register();

/**
 * @brief register the display dev ##name
 */
#define HAL_DISPLAY_DEV_REGISTER(name, ret)                             \
    {                                                                   \
        ret = HAL_DisplayDev_##name##_Register();                       \
        if (ret != 0)                                                   \
        {                                                               \
            HAL_LOGE("HAL_DisplayDev_%s_Register error %d\n", "##name", ret); \
            return ret;                                                 \
        }                                                               \
    }

typedef struct _display_dev display_dev_t;
typedef struct _display_dev_private_capability display_dev_private_capability_t;

/*! @brief Type of events that are supported by calling the callback function */
typedef enum _display_event
{
    /* Display finished sending the frame asynchronously, provide another frame */
    kDisplayEvent_RequestFrame,
    kDisplayEvent_Count
} display_event_t;

/*! @brief The mpp callback function prototype */
typedef int (*mpp_callback_t)(mpp_t mpp, mpp_evt_t evt, void *evt_data, void *user_data);

/*! @brief Error codes for display hal devices */
typedef enum _hal_display_status
{
    kStatus_HAL_DisplaySuccess = 0, /*!< Successfully */
    kStatus_HAL_DisplayTxBusy,      /*!< Display tx is busy */
    kStatus_HAL_DisplayNonBlocking, /*!< Display will return immediately */
    kStatus_HAL_DisplayError,       /*!< Error occurs on HAL Display */
} hal_display_status_t;

/*! @brief Operation that needs to be implemented by a display device */
typedef struct _display_dev_operator
{
    /* initialize the dev */
    hal_display_status_t (*init)(
        display_dev_t *dev, mpp_display_params_t *config, mpp_callback_t callback, void *user_data);
    /* deinitialize the dev */
    hal_display_status_t (*deinit)(const display_dev_t *dev);
    /* start the dev */
    hal_display_status_t (*start)(display_dev_t *dev);
    /* stop the dev */
    hal_display_status_t (*stop)(display_dev_t *dev);
    /* blit a buffer to the dev */
    hal_display_status_t (*blit)(const display_dev_t *dev, void *frame, int width, int height);
    /* get buffer descriptors and policy */
    hal_display_status_t (*get_buf_desc)(const display_dev_t *dev, hw_buf_desc_t *in_buf, mpp_memory_policy_t *policy);
} display_dev_operator_t;

/*! @brief Structure that characterizes the display device. */
struct _display_dev_private_capability
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
    /* pixel format */
    mpp_pixel_format_t format;
    /* number of input buffers */
    int nbFrameBuffer;
    /* array of pointers to framebuffer */
    void **frameBuffers;
    /* callback */
    mpp_callback_t callback;
    /* param for the callback */
    void *user_data;
};

/*! @brief Attributes of a display device. */
struct _display_dev
{
    /* unique id which is assigned by display manager during the registration */
    int id;
    /* name of the device */
    char name[HAL_DEVICE_NAME_MAX_LENGTH];
    /* operations */
    const display_dev_operator_t *ops;
    /* private capability */
    display_dev_private_capability_t cap;
};

#endif /*_HAL_DISPLAY_DEV_H_*/

