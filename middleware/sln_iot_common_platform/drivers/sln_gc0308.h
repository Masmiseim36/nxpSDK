/*
 * Copyright 2020-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SLN_GC0308_H_
#define _SLN_GC0308_H_

#include "fsl_common.h"
#include "fsl_camera_device.h"
#include "fsl_video_i2c.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define GC0308_I2C_ADDR      0x21
#define GC0308_CHIP_ID       0x9B
#define GC0308_REG_CHIP_ID   0x00
#define GC0308_SENSOR_WIDTH  640U
#define GC0308_SENSOR_HEIGHT 480U

/*!
 * @brief GC0308 resource.
 *
 * Before initialize the GC0308, the resource must be initialized that the
 * Two-Wire-Serial-Interface I2C could start to work.
 */
typedef struct _gc0308_resource
{
    video_i2c_send_func_t i2cSendFunc;        /*!< I2C send function. */
    video_i2c_receive_func_t i2cReceiveFunc;  /*!< I2C receive function. */
    void (*pullResetPin)(bool pullUp);        /*!< Function to pull reset pin high or low. */
    void (*pullPowerDownPin)(bool powerDown); /*!< Function activate / deactivate the sensor power down. */
    uint32_t inputClockFreq_Hz;               /*!< Input clock frequency, EXTCLK. */
} gc0308_resource_t;

/*! @brief GC0308 operation functions. */
extern const camera_device_operations_t gc0308_ops;

#endif /* _SLN_GC0308_H_ */
