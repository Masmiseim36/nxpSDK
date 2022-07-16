/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016, 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_hid.h"

#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "composite.h"

#include "hid_keyboard.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
usb_status_t USB_DeviceHidKeyboardAction(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_KeyboardBuffer[USB_HID_KEYBOARD_REPORT_LENGTH];
usb_device_composite_struct_t *g_UsbDeviceComposite;
static usb_device_hid_keyboard_struct_t s_UsbDeviceHidKeyboard;
extern volatile bool g_ButtonPress;
extern volatile bool g_CodecSpeakerMuteUnmute;

/*******************************************************************************
 * Code
 ******************************************************************************/

usb_status_t USB_DeviceHidKeyboardAction(void)
{
    if (g_ButtonPress)
    {
        s_UsbDeviceHidKeyboard.buffer[0] = 0x04U;
        g_ButtonPress                    = false;
        return USB_DeviceSendRequest(g_UsbDeviceComposite->deviceHandle, USB_HID_KEYBOARD_ENDPOINT,
                                     s_UsbDeviceHidKeyboard.buffer, USB_HID_KEYBOARD_REPORT_LENGTH);
    }
    else if (g_CodecSpeakerMuteUnmute)
    {
        s_UsbDeviceHidKeyboard.buffer[0] = 0x00U;
        g_CodecSpeakerMuteUnmute         = false;
        return USB_DeviceSendRequest(g_UsbDeviceComposite->deviceHandle, USB_HID_KEYBOARD_ENDPOINT,
                                     s_UsbDeviceHidKeyboard.buffer, USB_HID_KEYBOARD_REPORT_LENGTH);
    }
    else
    {
        return kStatus_USB_Success;
    }
}

static usb_status_t USB_DeviceHidKeyboardInterruptIn(usb_device_handle handle,
                                                     usb_device_endpoint_callback_message_struct_t *message,
                                                     void *callbackParam)
{
    if (g_UsbDeviceComposite->attach)
    {
        return USB_DeviceHidKeyboardAction();
    }

    return kStatus_USB_Error;
}

usb_status_t USB_DeviceHidKeyboardSetConfigure(usb_device_handle handle, uint8_t configure)
{
    usb_device_endpoint_init_struct_t epInitStruct;
    usb_device_endpoint_callback_struct_t epCallback;

    if (USB_COMPOSITE_CONFIGURE_INDEX == configure)
    {
        epCallback.callbackFn    = USB_DeviceHidKeyboardInterruptIn;
        epCallback.callbackParam = handle;

        epInitStruct.zlt = 0U;

        epInitStruct.transferType = USB_ENDPOINT_INTERRUPT;
        epInitStruct.endpointAddress =
            USB_HID_KEYBOARD_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
        if (USB_SPEED_HIGH == g_UsbDeviceComposite->speed)
        {
            epInitStruct.maxPacketSize = HS_HID_KEYBOARD_INTERRUPT_IN_PACKET_SIZE;
            epInitStruct.interval      = HS_HID_KEYBOARD_INTERRUPT_IN_INTERVAL;
        }
        else
        {
            epInitStruct.maxPacketSize = FS_HID_KEYBOARD_INTERRUPT_IN_PACKET_SIZE;
            epInitStruct.interval      = FS_HID_KEYBOARD_INTERRUPT_IN_INTERVAL;
        }

        USB_DeviceInitEndpoint(handle, &epInitStruct, &epCallback);

        return USB_DeviceHidKeyboardAction(); /* run the cursor movement code */
    }
    return kStatus_USB_Error;
}

usb_status_t USB_DeviceHidKeyboardClassRequest(usb_device_handle handle,
                                               usb_setup_struct_t *setup,
                                               uint8_t **buffer,
                                               uint32_t *length)
{
    usb_status_t error = kStatus_USB_InvalidRequest;

    if (setup->wIndex != USB_HID_KEYBOARD_INTERFACE_INDEX)
    {
        return error;
    }

    switch (setup->bRequest)
    {
        case USB_DEVICE_HID_REQUEST_GET_REPORT:
            break;
        case USB_DEVICE_HID_REQUEST_GET_IDLE:
            break;
        case USB_DEVICE_HID_REQUEST_GET_PROTOCOL:
            break;
        case USB_DEVICE_HID_REQUEST_SET_REPORT:
            break;
        case USB_DEVICE_HID_REQUEST_SET_IDLE:
            if (((setup->bmRequestType & USB_REQUEST_TYPE_DIR_MASK) == USB_REQUEST_TYPE_DIR_OUT) &&
                (setup->wLength == 0U))
            {
                error                           = kStatus_USB_Success;
                s_UsbDeviceHidKeyboard.idleRate = 125U;
            }
            break;
        case USB_DEVICE_HID_REQUEST_SET_PROTOCOL:
            break;
        default:
            break;
    }

    return error;
}

/*!
 * @brief device Hid configure endpoint status.
 *
 * This function handle the Hid class specified event.
 * @param handle          The USB class  handle.
 * @param ep              The USB device endpoint.
 * @param status          The USB endpoint specific status.
 * @return kStatus_USB_Success or error.
 */
usb_status_t USB_DeviceHidConfigureEndpointStatus(usb_device_handle handle, uint8_t ep, uint8_t status)
{
    if (status)
    {
        if ((USB_HID_KEYBOARD_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
            (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK))
        {
            return USB_DeviceStallEndpoint(handle, ep);
        }
        else
        {
        }
    }
    else
    {
        if ((USB_HID_KEYBOARD_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
            (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK))
        {
            return USB_DeviceUnstallEndpoint(handle, ep);
        }
        else
        {
        }
    }
    return kStatus_USB_InvalidRequest;
}

usb_status_t USB_DeviceHidKeyboardInit(usb_device_composite_struct_t *deviceComposite)
{
    g_UsbDeviceComposite          = deviceComposite;
    s_UsbDeviceHidKeyboard.buffer = s_KeyboardBuffer;
    return kStatus_USB_Success;
}
