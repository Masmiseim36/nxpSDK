/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief hal output device declaration. Output devices receive event notifications via the output manager and react to
 * those events accordingly. For example, an LED output device may want to change colors/brightness based on detection
 * of a face. Output devices can include things like LEDs, speakers, and more.
 */

#ifndef _HAL_OUTPUT_DEV_H_
#define _HAL_OUTPUT_DEV_H_

#include "fwk_common.h"
#include "hal_graphics_dev.h"

typedef struct _output_dev output_dev_t;

/*! @brief Sources for the output messages */
typedef enum _output_algo_source
{
    kOutputAlgoSource_Vision,
    kOutputAlgoSource_Voice,
    kOutputAlgoSource_LPM,
    kOutputAlgoSource_Other,
} output_algo_source_t;

/*! @brief Types of output devices */
typedef enum _output_dev_type
{
    kOutputDevType_UI,
    kOutputDevType_Audio,
    kOutputDevType_Other,
} output_dev_type_t;

/*! @brief Error codes for output hal devices */
typedef enum _hal_output_status
{
    kStatus_HAL_OutputSuccess = 0,                                                       /*!< Successfully */
    kStatus_HAL_OutputError   = MAKE_FRAMEWORK_STATUS(kStatusFrameworkGroups_Output, 1), /*!< Error occurs */
} hal_output_status_t;

typedef struct _output_dev_attr_t
{
    output_dev_type_t type;
    union
    {
        gfx_surface_t *pSurface;
        void *reserve;
    };
} output_dev_attr_t;

/*! @brief Operation that needs to be implemented by an output device */
typedef struct _output_dev_operator
{
    /* initialize the dev */
    hal_output_status_t (*init)(const output_dev_t *dev);
    /* deinitialize the dev */
    hal_output_status_t (*deinit)(const output_dev_t *dev);
    /* start the dev */
    hal_output_status_t (*start)(const output_dev_t *dev);
    /* start the dev */
    hal_output_status_t (*stop)(const output_dev_t *dev);

} output_dev_operator_t;

/*! @brief Attributes of an output device */
struct _output_dev
{
    int id;
    char name[DEVICE_NAME_MAX_LENGTH];
    /* attributes */
    output_dev_attr_t attr;
    hal_device_config configs[MAXIMUM_CONFIGS_PER_DEVICE];

    /* operations */
    const output_dev_operator_t *ops;
};

#endif /*_HAL_OUTPUT_DEV_H_*/
