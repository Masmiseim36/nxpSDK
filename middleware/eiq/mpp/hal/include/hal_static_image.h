/*
 * Copyright 2020-2023 NXP.
 * NXP Confidential and Proprietary.
 * This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such
 * terms or by downloading, installing, activating and/or otherwise using
 * the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms. If you do not agree to be
 * bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software.
 */

#ifndef _HAL_STATIC_IMAGE_H_
#define _HAL_STATIC_IMAGE_H_

/*! \addtogroup HAL_TYPES
 *  @{
 */

#include "mpp_api_types.h"
#include "hal_types.h"

typedef struct _static_image static_image_t;

/** static image return status*/
typedef enum _hal_image_status
{
    MPP_kStatus_HAL_ImageSuccess = 0,  /*!< Successfully */
	MPP_kStatus_HAL_ImageError         /*!< Error occurs on HAL Image */

} hal_image_status_t;

/** @} */

/*! \addtogroup HAL_OPERATIONS
 *  @{
 */

/** @brief Operation that needs to be implemented by an image element */
typedef struct _static_image_operator
{
    hal_image_status_t (*init)(static_image_t *elt, mpp_img_params_t *config, void *param); /*!< initialize the elt */
    hal_image_status_t (*dequeue)(const static_image_t *elt, hw_buf_desc_t *out_buf, mpp_pixel_format_t *format); /*!< dequeue a buffer from the elt */
} static_image_operator_t;

/** @} */

/*! \addtogroup HAL_TYPES
 *  @{
 */

/** @brief Structure that characterize the image element. */
typedef struct
{
    int height;                /*!< buffer height */
    int width;                 /*!< buffer width */
    int left;                  /*!< left position */
    int top;                   /*!< top position */
    int right;                 /*!< right position */
    int bottom;                /*!< bottom position */
    mpp_pixel_format_t format; /*!< pixel format */
} static_image_static_config_t;

/** @brief Attributes of a an image element. */
struct _static_image
{
    int id;                              /*!< unique id which is assigned by image manager */
    const static_image_operator_t *ops;  /*!< operations */
    static_image_static_config_t config; /*!< static configs */
    uint8_t *buffer;                     /*!< static image buffer */
};

/** @} */

#endif /*_HAL_STATIC_IMAGE_H_*/
