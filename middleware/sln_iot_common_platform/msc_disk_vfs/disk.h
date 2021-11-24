/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_DISK_H__
#define __USB_DISK_H__ 1

#include "usb.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_msc.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
#define CONTROLLER_ID kUSB_ControllerEhci0
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0)
#define CONTROLLER_ID kUSB_ControllerKhci0
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Fs0
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Hs0
#endif

#if defined(__GIC_PRIO_BITS)
#define USB_DEVICE_INTERRUPT_PRIORITY (25U)
#else
#define USB_DEVICE_INTERRUPT_PRIORITY (3U)
#endif

/* Length of Each Logical Address Block */
#define LENGTH_OF_EACH_LBA (512U)
/* Total number of logical blocks present */
//#define TOTAL_LOGICAL_ADDRESS_BLOCKS_NORMAL (48U)
#define TOTAL_LOGICAL_ADDRESS_BLOCKS_NORMAL  (0x5000)
#define TOTAL_PHYSICAL_ADDRESS_BLOCKS_NORMAL (0x100)
/* Net Disk Size , default disk is 48*512, that is 24kByte, however , the disk reconnised by that PC only has 4k Byte,
 * This is caused by that the file system also need memory*/
//#define DISK_SIZE_NORMAL (TOTAL_LOGICAL_ADDRESS_BLOCKS_NORMAL * LENGTH_OF_EACH_LBA)
#define DISK_LOGICAL_SIZE_NORMAL  (TOTAL_LOGICAL_ADDRESS_BLOCKS_NORMAL * LENGTH_OF_EACH_LBA)  // 921600
#define DISK_PHYSICAL_SIZE_NORMAL (TOTAL_PHYSICAL_ADDRESS_BLOCKS_NORMAL * LENGTH_OF_EACH_LBA) // 131072

#define LOGICAL_UNIT_SUPPORTED (1U)

typedef struct _usb_msc_struct
{
    usb_device_handle deviceHandle;
    class_handle_t mscHandle;
    TaskHandle_t device_task_handle;
    TaskHandle_t application_task_handle;
    uint8_t *storageDisk;
    uint8_t diskLock;
    uint8_t read_write_error;
    uint8_t currentConfiguration;
    uint8_t currentInterfaceAlternateSetting[USB_MSC_INTERFACE_COUNT];
    uint8_t speed;
    uint8_t attach;
} usb_msc_struct_t;

void USB_DeviceApplicationInit();

int USB_MSC_Init(void);

/*!
 * @brief Check if there is a MSD mode request and if there is enter MSD Mode.
 *
 * @return Return true if there is a request pending, false otherwise
 */

bool CheckForMSDMode(void);

#endif
