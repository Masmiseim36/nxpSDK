/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_hid.h"

#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "composite.h"

#include "hid_mouse.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static usb_status_t USB_DeviceHidMouseAction(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_MouseBuffer[USB_HID_MOUSE_REPORT_LENGTH];
static usb_device_composite_struct_t *s_UsbDeviceComposite;
static usb_device_hid_mouse_struct_t s_UsbDeviceHidMouse;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Update mouse pointer location. Draw a rectangular rotation*/
static usb_status_t USB_DeviceHidMouseAction(void)
{
    static int8_t x = 0U;
    static int8_t y = 0U;
    enum
    {
        RIGHT,
        DOWN,
        LEFT,
        UP
    };
    static uint8_t dir = RIGHT;

    switch (dir)
    {
        case RIGHT:
            /* Move right. Increase X value. */
            s_UsbDeviceHidMouse.buffer[1] = 1U;
            s_UsbDeviceHidMouse.buffer[2] = 0U;
            x++;
            if (x > 99U)
            {
                dir++;
            }
            break;
        case DOWN:
            /* Move down. Increase Y value. */
            s_UsbDeviceHidMouse.buffer[1] = 0U;
            s_UsbDeviceHidMouse.buffer[2] = 1U;
            y++;
            if (y > 99U)
            {
                dir++;
            }
            break;
        case LEFT:
            /* Move left. Discrease X value. */
            s_UsbDeviceHidMouse.buffer[1] = (uint8_t)(0xFFU);
            s_UsbDeviceHidMouse.buffer[2] = 0U;
            x--;
            if (x < 1U)
            {
                dir++;
            }
            break;
        case UP:
            /* Move up. Discrease Y value. */
            s_UsbDeviceHidMouse.buffer[1] = 0U;
            s_UsbDeviceHidMouse.buffer[2] = (uint8_t)(0xFFU);
            y--;
            if (y < 1U)
            {
                dir = RIGHT;
            }
            break;
        default:
            break;
    }
    return USB_DeviceHidSend(s_UsbDeviceComposite->hidMouseHandle, USB_HID_MOUSE_ENDPOINT_IN,
                             s_UsbDeviceHidMouse.buffer, USB_HID_MOUSE_REPORT_LENGTH);
}

/* The device HID class callback */
usb_status_t USB_DeviceHidMouseCallback(class_handle_t handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_InvalidRequest;

    switch (event)
    {
        case kUSB_DeviceHidEventSendResponse:
            if (s_UsbDeviceComposite->attach)
            {
                return USB_DeviceHidMouseAction();
            }
            break;
        case kUSB_DeviceHidEventGetReport:
        case kUSB_DeviceHidEventSetReport:
        case kUSB_DeviceHidEventRequestReportBuffer:
            break;
        case kUSB_DeviceHidEventGetIdle:
        case kUSB_DeviceHidEventGetProtocol:
        case kUSB_DeviceHidEventSetIdle:
        case kUSB_DeviceHidEventSetProtocol:
            error = kStatus_USB_Success;
            break;
        default:
            break;
    }

    return error;
}

/* The device callback */
usb_status_t USB_DeviceHidMouseSetConfigure(class_handle_t handle, uint8_t configure)
{
    if (USB_COMPOSITE_CONFIGURE_INDEX == configure)
    {
        return USB_DeviceHidMouseAction(); /* run the cursor movement code */
    }
    return kStatus_USB_Error;
}

/* Set interface */
usb_status_t USB_DeviceHidMouseSetInterface(class_handle_t handle, uint8_t interface, uint8_t alternateSetting)
{
    if (USB_HID_KEYBOARD_INTERFACE_INDEX == interface)
    {
        return USB_DeviceHidMouseAction(); /* run the cursor movement code */
    }
    return kStatus_USB_Error;
}

/* Initialize the HID mouse */
usb_status_t USB_DeviceHidMouseInit(usb_device_composite_struct_t *deviceComposite)
{
    s_UsbDeviceComposite       = deviceComposite;
    s_UsbDeviceHidMouse.buffer = s_MouseBuffer;
    return kStatus_USB_Success;
}
