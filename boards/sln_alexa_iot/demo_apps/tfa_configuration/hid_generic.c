/*
 * Copyright 2019 NXP
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

#include "hid_generic.h"

#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

#include "stdio.h"
#include "stdlib.h"
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

#include "pin_mux.h"
#include "usb_phy.h"
#include "tfa_device_hal.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitHardware(void);
void USB_DeviceClockInit(void);
void USB_DeviceIsrEnable(void);
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle);
#endif

static usb_status_t USB_DeviceHidGenericInterruptIn(usb_device_handle handle,
                                                    usb_device_endpoint_callback_message_struct_t *message,
                                                    void *callbackParam);
static usb_status_t USB_DeviceHidGenericInterruptOut(usb_device_handle handle,
                                                     usb_device_endpoint_callback_message_struct_t *message,
                                                     void *callbackParam);

usb_status_t USB_DeviceInterruptInTask(void);
usb_status_t USB_DeviceInterruptOutTask(tfa9xxx_handle_t *handle);

/*******************************************************************************
 * Variables
 ******************************************************************************/

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint32_t s_GenericBuffer0[USB_HID_GENERIC_IN_BUFFER_LENGTH >> 2];
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint32_t s_GenericBuffer1[USB_HID_GENERIC_IN_BUFFER_LENGTH >> 2];

usb_hid_generic_struct_t g_UsbDeviceHidGeneric;
usb_device_composite_struct_t *g_UsbDeviceComposite;
extern tfa_hal_msg_status_t g_TfaDeviceMessageStatus;
bool volatile g_UsbDeviceInterruptInReady                                = false;
bool volatile g_UsbDeviceInterruptOutReady                               = false;
usb_device_endpoint_callback_message_struct_t g_UsbDeviceCallbackMessage = {NULL, 0, false};

/*******************************************************************************
 * Code
 ******************************************************************************/

/* The hid generic interrupt IN endpoint callback */
static usb_status_t USB_DeviceHidGenericInterruptIn(usb_device_handle handle,
                                                    usb_device_endpoint_callback_message_struct_t *message,
                                                    void *callbackParam)
{
    // ignore while tfa_hal message is not complete
    if (!g_TfaDeviceMessageStatus.msgComplete)
        return kStatus_USB_Success;

    if (g_UsbDeviceComposite->attach)
    {
        g_UsbDeviceInterruptInReady = 1;

        static uint8_t s_in_message_buffer[USB_HID_GENERIC_IN_BUFFER_LENGTH];
        memcpy(&g_UsbDeviceCallbackMessage, message, sizeof(usb_device_endpoint_callback_message_struct_t));
        memcpy(&s_in_message_buffer, message->buffer, message->length);
        g_UsbDeviceCallbackMessage.buffer = s_in_message_buffer;
    }

    return kStatus_USB_Success;
}

/* The hid generic interrupt OUT endpoint callback */
static usb_status_t USB_DeviceHidGenericInterruptOut(usb_device_handle handle,
                                                     usb_device_endpoint_callback_message_struct_t *message,
                                                     void *callbackParam)
{
    if (g_UsbDeviceComposite->attach)
    {
        g_UsbDeviceInterruptOutReady = 1;

        static uint8_t s_out_message_buffer[USB_HID_GENERIC_OUT_BUFFER_LENGTH];
        memcpy(&g_UsbDeviceCallbackMessage, message, sizeof(usb_device_endpoint_callback_message_struct_t));
        memcpy(&s_out_message_buffer, message->buffer, message->length);
        g_UsbDeviceCallbackMessage.buffer = s_out_message_buffer;
    }

    return kStatus_USB_Success;
}

usb_status_t USB_DeviceInterruptOutTask(tfa9xxx_handle_t *handle)
{
    if (TFA_Hal_CollectMsg(handle, g_UsbDeviceCallbackMessage.buffer, g_UsbDeviceCallbackMessage.length))
    {
        g_UsbDeviceHidGeneric.bufferIndex ^= 1U;
        // hexdump("ret msg:", sndBuffer, USB_HID_GENERIC_OUT_BUFFER_LENGTH);
        return USB_DeviceSendRequest(g_UsbDeviceHidGeneric.deviceHandle, USB_HID_GENERIC_ENDPOINT_IN,
                                     g_TfaDeviceMessageStatus.sndBuffer, USB_HID_GENERIC_OUT_BUFFER_LENGTH);
    }
    else
        // get new chunk
        return USB_DeviceRecvRequest(g_UsbDeviceComposite->deviceHandle, USB_HID_GENERIC_ENDPOINT_OUT,
                                     (uint8_t *)&g_UsbDeviceHidGeneric.buffer[g_UsbDeviceHidGeneric.bufferIndex][0],
                                     USB_HID_GENERIC_OUT_BUFFER_LENGTH);
}

usb_status_t USB_DeviceInterruptInTask(void)
{
    uint8_t *sendBuffer               = &g_TfaDeviceMessageStatus.sndBuffer[g_TfaDeviceMessageStatus.totalSend];
    g_UsbDeviceCallbackMessage.length = TFA_Hal_GetNextChunkLength(USB_HID_GENERIC_OUT_BUFFER_LENGTH);
    if (g_UsbDeviceCallbackMessage.length)
    {
        return USB_DeviceSendRequest(g_UsbDeviceHidGeneric.deviceHandle, USB_HID_GENERIC_ENDPOINT_IN, sendBuffer,
                                     USB_HID_GENERIC_OUT_BUFFER_LENGTH);
    }
    else
    {
        g_TfaDeviceMessageStatus.msgComplete = 0;
        return USB_DeviceRecvRequest(g_UsbDeviceHidGeneric.deviceHandle, USB_HID_GENERIC_ENDPOINT_OUT,
                                     (uint8_t *)&g_UsbDeviceHidGeneric.buffer[g_UsbDeviceHidGeneric.bufferIndex][0],
                                     USB_HID_GENERIC_OUT_BUFFER_LENGTH);
    }
}

usb_status_t USB_DeviceHidGenericSetConfigure(usb_device_handle handle, uint8_t configure)
{
    usb_status_t error = kStatus_USB_Success;

    if (USB_HID_GENERIC_CONFIGURE_INDEX == configure)
    {
        /* If the confguration is valid, initliaze the HID generic interrupt IN pipe */
        usb_device_endpoint_init_struct_t epInitStruct;
        usb_device_endpoint_callback_struct_t epCallback;

        epCallback.callbackFn    = USB_DeviceHidGenericInterruptIn;
        epCallback.callbackParam = handle;

        epInitStruct.zlt          = 0U;
        epInitStruct.transferType = USB_ENDPOINT_INTERRUPT;
        epInitStruct.endpointAddress =
            USB_HID_GENERIC_ENDPOINT_IN | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
        if (USB_SPEED_HIGH == g_UsbDeviceComposite->speed)
        {
            epInitStruct.maxPacketSize = HS_HID_GENERIC_INTERRUPT_IN_PACKET_SIZE;
        }
        else
        {
            epInitStruct.maxPacketSize = FS_HID_GENERIC_INTERRUPT_IN_PACKET_SIZE;
        }

        USB_DeviceInitEndpoint(handle, &epInitStruct, &epCallback);

        epCallback.callbackFn    = USB_DeviceHidGenericInterruptOut;
        epCallback.callbackParam = handle;

        epInitStruct.zlt          = 0U;
        epInitStruct.transferType = USB_ENDPOINT_INTERRUPT;
        epInitStruct.endpointAddress =
            USB_HID_GENERIC_ENDPOINT_OUT | (USB_OUT << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
        if (USB_SPEED_HIGH == g_UsbDeviceComposite->speed)
        {
            epInitStruct.maxPacketSize = HS_HID_GENERIC_INTERRUPT_OUT_PACKET_SIZE;
        }
        else
        {
            epInitStruct.maxPacketSize = FS_HID_GENERIC_INTERRUPT_OUT_PACKET_SIZE;
        }

        USB_DeviceInitEndpoint(handle, &epInitStruct, &epCallback);

        g_UsbDeviceComposite->attach = 1U;

        error = USB_DeviceRecvRequest(g_UsbDeviceComposite->deviceHandle, USB_HID_GENERIC_ENDPOINT_OUT,
                                      (uint8_t *)&g_UsbDeviceHidGeneric.buffer[g_UsbDeviceHidGeneric.bufferIndex][0],
                                      USB_HID_GENERIC_OUT_BUFFER_LENGTH);
    }

    return error;
}

/* Configure the endpoint status (idle or stall) */
usb_status_t USB_DeviceHidConfigureEndpointStatus(usb_device_handle handle, uint8_t ep, uint8_t status)
{
    if (status)
    {
        if (((USB_HID_GENERIC_ENDPOINT_IN == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
             (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK)) ||
            ((USB_HID_GENERIC_ENDPOINT_OUT == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
             (!(ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK))))
        {
            return USB_DeviceStallEndpoint(handle, ep);
        }
        else
        {
        }
    }
    else
    {
        if (((USB_HID_GENERIC_ENDPOINT_IN == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
             (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK)) ||
            ((USB_HID_GENERIC_ENDPOINT_OUT == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
             (!(ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK))))
        {
            return USB_DeviceUnstallEndpoint(handle, ep);
        }
        else
        {
        }
    }
    return kStatus_USB_InvalidRequest;
}

/* Handle class-specific request */
usb_status_t USB_DeviceHidGenericClassRequest(usb_device_handle handle,
                                              usb_setup_struct_t *setup,
                                              uint32_t *length,
                                              uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;

    if (setup->wIndex != USB_HID_GENERIC_INTERFACE_INDEX)
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

usb_status_t USB_DeviceHidGenericInit(usb_device_composite_struct_t *deviceComposite)
{
    g_UsbDeviceComposite = deviceComposite;
    // s_UsbDeviceHidKeyboard.buffer = s_KeyboardBuffer;
    /* Set HID generic default state */
    g_UsbDeviceHidGeneric.speed        = USB_SPEED_FULL;
    g_UsbDeviceHidGeneric.attach       = 0U;
    g_UsbDeviceHidGeneric.deviceHandle = g_UsbDeviceComposite->deviceHandle;
    g_UsbDeviceHidGeneric.buffer[0]    = (uint8_t *)&s_GenericBuffer0[0];
    g_UsbDeviceHidGeneric.buffer[1]    = (uint8_t *)&s_GenericBuffer1[0];

    return kStatus_USB_Success;
}
