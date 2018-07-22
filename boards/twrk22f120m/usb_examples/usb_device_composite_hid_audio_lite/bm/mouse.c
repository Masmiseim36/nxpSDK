/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
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

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_hid.h"

#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "mouse.h"

#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"

#include <stdio.h>
#include <stdlib.h>

#include "composite.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
USB_DATA_ALIGNMENT static uint8_t s_MouseBuffer[USB_HID_MOUSE_REPORT_LENGTH];
static usb_device_composite_struct_t *g_deviceComposite;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief device mouse moves action.
 *
 * This function handle the mouse moves action.
 */
usb_status_t USB_DeviceMouseAction(void)
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
            g_deviceComposite->hidMouse.buffer[1] = 2;
            g_deviceComposite->hidMouse.buffer[2] = 0U;
            x++;
            if (x > 5U)
            {
                dir++;
            }
            break;
        case DOWN:
            g_deviceComposite->hidMouse.buffer[1] = 0U;
            g_deviceComposite->hidMouse.buffer[2] = 2;
            y++;
            if (y > 5U)
            {
                dir++;
            }
            break;
        case LEFT:
            g_deviceComposite->hidMouse.buffer[1] = (uint8_t)(-2);
            g_deviceComposite->hidMouse.buffer[2] = 0U;
            x--;
            if (x < 1U)
            {
                dir++;
            }
            break;
        case UP:
            g_deviceComposite->hidMouse.buffer[1] = 0U;
            g_deviceComposite->hidMouse.buffer[2] = (uint8_t)(-2);
            y--;
            if (y < 1U)
            {
                dir = RIGHT;
            }
            break;
        default:
            break;
    }
    return USB_DeviceSendRequest(g_deviceComposite->deviceHandle, USB_HID_MOUSE_ENDPOINT,
                                 g_deviceComposite->hidMouse.buffer, USB_HID_MOUSE_REPORT_LENGTH);
}

/*!
 * @brief device Hid service interrupt function.
 *
 * This function handle the Hid class specified event.
 * @param deviceHandle    The USB class  device handle.
 * @param event           The USB device event type.
 * @param arg             The parameter of the class specific event.
 * @return kStatus_USB_Success or error.
 */
usb_status_t USB_DeviceHidInterruptIn(usb_device_handle deviceHandle,
                                      usb_device_endpoint_callback_message_struct_t *event,
                                      void *arg)
{
    if (g_deviceComposite->hidMouse.attach)
    {
        return USB_DeviceMouseAction();
    }

    return kStatus_USB_Error;
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
        if ((USB_HID_MOUSE_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
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
        if ((USB_HID_MOUSE_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
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

usb_status_t USB_DeviceHidClassRequest(usb_device_handle handle,
                                       usb_setup_struct_t *setup,
                                       uint32_t *length,
                                       uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;

    if (setup->wIndex != USB_HID_MOUSE_INTERFACE_INDEX)
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
            break;
        case USB_DEVICE_HID_REQUEST_SET_PROTOCOL:
            break;
        default:
            break;
    }

    return error;
}

/*!
 * @brief Hid device set configuration function.
 *
 * This function sets configuration for msc class.
 *
 * @param handle The Hid class handle.
 * @param configure Hid class configure index.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceHidMouseSetConfigure(usb_device_handle handle, uint8_t configure)
{
    usb_device_endpoint_init_struct_t epInitStruct;
    usb_device_endpoint_callback_struct_t endpointCallback = {0};

    g_deviceComposite->hidMouse.attach = 1U;

    endpointCallback.callbackFn = USB_DeviceHidInterruptIn;
    endpointCallback.callbackParam = handle;
    epInitStruct.zlt = 0U;
    epInitStruct.transferType = USB_ENDPOINT_INTERRUPT;
    epInitStruct.endpointAddress = USB_HID_MOUSE_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
    if (USB_SPEED_HIGH == g_deviceComposite->speed)
    {
        epInitStruct.maxPacketSize = HS_INTERRUPT_IN_PACKET_SIZE;
    }
    else
    {
        epInitStruct.maxPacketSize = FS_INTERRUPT_IN_PACKET_SIZE;
    }

    USB_DeviceInitEndpoint(handle, &epInitStruct, &endpointCallback);

    return USB_DeviceMouseAction();
}

/*!
 * @brief device Hid init function.
 *
 * This function initializes the device with the composite device class information.
 *
 * @param deviceComposite          The pointer to the composite device structure.
 * @return kStatus_USB_Success .
 */
usb_status_t USB_DeviceHidMouseInit(usb_device_composite_struct_t *deviceComposite)
{
    g_deviceComposite = deviceComposite;
    g_deviceComposite->hidMouse.buffer = s_MouseBuffer;
    return kStatus_USB_Success;
}
