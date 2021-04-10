/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_HOST_FATFS_H__
#define __USB_HOST_FATFS_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define CONTROLLER_ID kUSB_ControllerEhci0

#if defined(__GIC_PRIO_BITS)
#define USB_HOST_INTERRUPT_PRIORITY (25U)
#elif defined(__NVIC_PRIO_BITS) && (__NVIC_PRIO_BITS >= 3)
#define USB_HOST_INTERRUPT_PRIORITY (6U)
#else
#define USB_HOST_INTERRUPT_PRIORITY (3U)
#endif

extern usb_host_handle g_HostHandle;
extern usb_host_class_handle g_UsbFatfsClassHandle;

/*! @brief host app device attach/detach status */
typedef enum _usb_host_app_state
{
    kStatus_DEV_Idle = 0, /*!< there is no device attach/detach */
    kStatus_DEV_Attached, /*!< device is attached */
    kStatus_DEV_Detached, /*!< device is detached */
} usb_host_app_state_t;

/*! @brief USB host msd fatfs instance structure */
typedef struct _usb_host_msd_fatfs_instance
{
    usb_host_configuration_handle configHandle; /*!< configuration handle */
    usb_device_handle deviceHandle;             /*!< device handle */
    usb_host_class_handle classHandle;          /*!< class handle */
    usb_host_interface_handle interfaceHandle;  /*!< interface handle */
    uint8_t deviceState;                        /*!< device attach/detach status */
} usb_host_msd_fatfs_instance_t;

extern usb_status_t USB_HostEvent(usb_device_handle deviceHandle,
                                  usb_host_configuration_handle configurationHandle,
                                  uint32_t eventCode);

void USB_HostTask(void *param);

void USB_HostApplicationInit(void);

#endif /* __USB_HOST_FATFS_H__ */
