/*
 * Copyright (c) 2017, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _FSL_CAMERA_DEVICE_H_
#define _FSL_CAMERA_DEVICE_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct _camera_device_handle camera_device_handle_t;

/*! @brief Camera device operations. */
typedef struct _camera_device_operations
{
    status_t (*init)(camera_device_handle_t *handle, const camera_config_t *config);
    status_t (*deinit)(camera_device_handle_t *handle);
    status_t (*start)(camera_device_handle_t *handle);
    status_t (*stop)(camera_device_handle_t *handle);
} camera_device_operations_t;

/*! @brief Camera device handle. */
struct _camera_device_handle
{
    void *resource;                        /*!< The resource used by the camera device, it is device specific. */
    const camera_device_operations_t *ops; /*!< The device related operations. */
};

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize the camera device.
 *
 * Initialize the camera device with the user defined configuration.
 *
 * @param handle Camera device handle.
 * @param config Pointer to the configuration.
 * @return Returns @ref kStatus_Success if initialize success, otherwise returns
 * error code.
 */
static inline status_t CAMERA_DEVICE_Init(camera_device_handle_t *handle, const camera_config_t *config)
{
    return handle->ops->init(handle, config);
}

/*!
 * @brief Deinitialize the camera device.
 *
 * @param handle Camera device handle.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
static inline status_t CAMERA_DEVICE_Deinit(camera_device_handle_t *handle)
{
    return handle->ops->deinit(handle);
}

/*!
 * @brief Start the camera device to output data.
 *
 * @param handle Camera device handle.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
static inline status_t CAMERA_DEVICE_Start(camera_device_handle_t *handle)
{
    return handle->ops->start(handle);
}

/*!
 * @brief Stop the camera device outputing data.
 *
 * @param handle Camera device handle.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
static inline status_t CAMERA_DEVICE_Stop(camera_device_handle_t *handle)
{
    return handle->ops->stop(handle);
}

#if defined(__cplusplus)
}
#endif

#endif /* _FSL_CAMERA_DEVICE_H_ */
