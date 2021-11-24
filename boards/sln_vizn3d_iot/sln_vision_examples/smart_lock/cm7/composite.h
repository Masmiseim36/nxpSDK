/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _USB_DEVICE_COMPOSITE_H_
#define _USB_DEVICE_COMPOSITE_H_ 1
#include "FreeRTOS.h"
#include "semphr.h"
#include "event_groups.h"

#include "usb_device_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define kUSB_DeviceClassInit (0U)

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

#define USB_DEVICE_INTERRUPT_PRIORITY (3U)

typedef struct _usb_device_composite_struct
{
    usb_device_handle deviceHandle; /* USB device handle. */
    usb_device_callback_t deviceCallback[USB_DEVICE_CONFIG_CDC_ACM + USB_DEVICE_CONFIG_VIDEO];
    uint8_t speed;  /* Speed of USB device. USB_SPEED_FULL/USB_SPEED_LOW/USB_SPEED_HIGH.                 */
    uint8_t attach; /* A flag to indicate whether a usb device is attached. 1: attached, 0: not attached */
    uint8_t currentConfiguration; /* Current configuration value. */
    uint8_t
        currentInterfaceAlternateSetting[USB_INTERFACE_COUNT]; /* Current alternate setting value for each interface. */
} usb_device_composite_struct_t;

/*******************************************************************************
 * API
 ******************************************************************************/

int USB_DeviceApplicationInit(void);
void USB_CompositeClassRegister(usb_device_callback_t deviceCallback,
                                usb_device_class_callback_t classCallback,
                                usb_device_class_struct_t *classInfomation);
void USB_DeviceIsrEnable(void);
void USB_DeviceIsrDisable(void);

#endif /* _USB_DEVICE_COMPOSITE_H_ */
