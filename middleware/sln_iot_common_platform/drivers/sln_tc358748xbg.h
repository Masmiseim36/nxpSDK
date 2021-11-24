/*
 * Copyright 2020-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SLN_TC358748XBG_H_
#define _SLN_TC358748XBG_H_

#include "fsl_common.h"
#include "fsl_video_i2c.h"

/*******************************************************************************
 * Definitions
 ********************************************"**********************************/
/*!
 * @brief TC358748XBG resource.
 *
 * Before initialize the TC358748XBG, the resource must be initialized that the
 * Two-Wire-Serial-Interface I2C could start to work.
 */
typedef struct _tc358748_resource
{
    video_i2c_send_func_t i2cSendFunc;       /*!< I2C send function. */
    video_i2c_receive_func_t i2cReceiveFunc; /*!< I2C receive function. */
    void (*pullResetPin)(bool pullUp);       /*!< Function to pull reset pin high or low. */
    void (*pullPowerEnablePin)(bool pullUp); /*!< Function to pull the power enable pin high or low. */
} tc358748_resource_t;

/*!
 * @brief TC358748XBG work mode.
 */
typedef enum _tc358748_work_mode
{
    work_mode_normal,
    work_mode_sleep,
} tc358748_work_mode_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif
status_t TC358748_Init(tc358748_resource_t *resource);
status_t TC358748_SleepControl(tc358748_work_mode_t mode);
void TC358748_DeInit(void);

#if defined(__cplusplus)
}
#endif

#endif /* _SLN_TC358748XBG_H_ */
