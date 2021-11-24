/*
 * Copyright 2020-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SLN_ORBBECU1S_H_
#define _SLN_ORBBECU1S_H_

#include "fsl_common.h"
#include "fsl_camera_device.h"
#include "fsl_video_i2c.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define OBU1S_SENSOR_WIDTH  540U
#define OBU1S_SENSOR_HEIGHT 640U

/*!
 * @brief OBU1S resource.
 *
 * Before initialize the OBU1S, the resource must be initialized that the
 * Two-Wire-Serial-Interface I2C could start to work.
 */
typedef struct _obU1S_resource
{
    video_i2c_send_func_t i2cSendFunc;       /*!< I2C send function. */
    video_i2c_receive_func_t i2cReceiveFunc; /*!< I2C receive function. */
    void (*pullResetPin)(bool pullUp);       /*!< Function to pull reset pin high or low. */
    void (*pullPowerEnablePin)(bool pullUp); /*!< Function to pull the power enable pin high or low. */
    void (*pullTestModePin)(bool pullUp);    /*!< Function to pull test mode pin high or low. */
    void (*pullLdpIntPin)(bool pullUp);      /*!< Function to pull  LDP INT  pin high or low. */
} obU1S_resource_t;

/*! @brief OBU1S operation functions. */
extern const camera_device_operations_t obU1S_ops;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _SLN_ORBBECU1S_H_ */
