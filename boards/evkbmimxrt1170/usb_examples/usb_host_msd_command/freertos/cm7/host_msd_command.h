/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HOST_MSD_COMMAND_H_
#define _HOST_MSD_COMMAND_H_

#include "usb_host.h"
#include "usb_host_msd.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief 1 - execute throughput test; 0 - don't execute throughput test */
#define MSD_THROUGHPUT_TEST_ENABLE (0U)

/*! @brief host app run status */
typedef enum _usb_host_msd_run_state
{
    kUSB_HostMsdRunIdle = 0,         /*!< idle */
    kUSB_HostMsdRunSetInterface,     /*!< execute set interface code */
    kUSB_HostMsdRunWaitSetInterface, /*!< wait set interface done */
    kUSB_HostMsdRunMassStorageTest   /*!< execute mass storage test code */
} usb_host_msd_run_state_t;

/*! @brief USB host msd command instance structure */
typedef struct _usb_host_msd_command_instance
{
    usb_host_configuration_handle configHandle; /*!< configuration handle */
    usb_device_handle deviceHandle;             /*!< device handle */
    usb_host_class_handle classHandle;          /*!< class handle */
    usb_host_interface_handle interfaceHandle;  /*!< interface handle */
    SemaphoreHandle_t commandSemaphore;         /*!< semaphore for waiting msd command callback */
    uint8_t *testUfiBuffer;                     /*!< test buffer */
    uint8_t prevDeviceState;                    /*!< device attach/detach previous status */
    uint8_t deviceState;                        /*!< device attach/detach status */
    uint8_t runWaitState; /*!< application wait status, go to next run status when the wait status success */
    uint8_t runState;     /*!< application run status */
} usb_host_msd_command_instance_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief host msd callback function.
 *
 * This function should be called in the host callback function.
 *
 * @param deviceHandle        device handle.
 * @param configurationHandle attached device's configuration descriptor information.
 * @param eventCode           callback event code, please reference to enumeration host_event_t.
 *
 * @retval kStatus_USB_Success              The host is initialized successfully.
 * @retval kStatus_USB_NotSupported         The configuration don't contain msd interface.
 * @retval kStatus_USB_Error                There is no idle msd instance.
 */
extern usb_status_t USB_HostMsdEvent(usb_device_handle deviceHandle,
                                     usb_host_configuration_handle configurationHandle,
                                     uint32_t eventCode);

/*!
 * @brief host msd command task function.
 *
 * This function implements the host msd command action, it is used to create task.
 *
 * @param arg   the host msd command instance pointer.
 */
extern void USB_HostMsdTask(void *arg);

#endif /* _HOST_MSD_COMMAND_H_ */
