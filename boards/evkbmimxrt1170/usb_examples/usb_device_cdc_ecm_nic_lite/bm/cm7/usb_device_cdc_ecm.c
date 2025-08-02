/**
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"
#include "usb_device_cdc_ecm.h"
#include "usb_device_descriptor.h"
#include "usb_eth_adapter.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void APP_TransferEthernet2USB_USBSend(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern uint8_t dataOutBuffer[];

static usb_device_cdc_ecm_pipe_status_t cdcEcmPipe;

/*******************************************************************************
 * Code
 ******************************************************************************/
usb_status_t USB_DeviceCdcEcmBulkIn(usb_device_handle handle, usb_device_endpoint_callback_message_struct_t *message, void *callbackParam)
{
    APP_TransferEthernet2USB_USBSend();

    cdcEcmPipe.bulkInIsBusy = 0U;

    return kStatus_USB_Success;
}

usb_status_t USB_DeviceCdcEcmBulkOut(usb_device_handle handle, usb_device_endpoint_callback_message_struct_t *message, void *callbackParam)
{
    if (message->length != USB_CANCELLED_TRANSFER_LENGTH)
    {
        eth_adapter_frame_buf_t frame;
        frame.len = message->length;
        frame.payload = message->buffer;
        if (ETH_ADAPTER_FrameQueuePush(&ethNicHandle.ethHandle->txFrameQueue, &frame) != ETH_ADAPTER_OK)
        {
            (void)usb_echo("USB(DATA OUT CALLBACK): Lost frame.\r\n");
        }
        (void)USB_DeviceCdcEcmRecv(ethNicHandle.deviceHandle, USB_DEVICE_CDC_ECM_DATA_BULK_OUT_EP_NUMBER, dataOutBuffer, APP_ETH_FRAME_MAX_LENGTH);
    }

    cdcEcmPipe.bulkOutIsBusy = 0U;

    return kStatus_USB_Success;
}

usb_status_t USB_DeviceCdcEcmInterruptIn(usb_device_handle handle, usb_device_endpoint_callback_message_struct_t *message, void *callbackParam)
{
    cdcEcmPipe.interruptInIsBusy = 0U;

    return kStatus_USB_Success;
}

usb_status_t USB_DeviceProcessClassRequest(usb_device_handle handle, usb_setup_struct_t *setup, uint32_t *length, uint8_t **buffer)
{
    usb_status_t status = kStatus_USB_InvalidRequest;

    if (setup->wIndex != USB_DEVICE_CDC_ECM_COMM_INTERFACE_NUMBER)
    {
        return status;
    }

    switch (setup->bRequest)
    {
        case USB_DEVICE_CDC_ECM_SET_ETHERNET_PACKET_FILTER:
            ethNicHandle.attachStatus = 1U;
            if (setup->wValue & USB_DEVICE_CDC_ECM_PACKET_TYPE_PROMISCUOUS_MASK)
            {
                ethNicHandle.ethHandle->boardcastFramePass = true;
                ethNicHandle.ethHandle->multicastFramePass = true;
                ethNicHandle.ethHandle->unicastFramePass = true;
            }
            else
            {
                if (setup->wValue & USB_DEVICE_CDC_ECM_PACKET_TYPE_ALL_MULTICAST_MASK)
                {
                    ethNicHandle.ethHandle->multicastFramePass = true;
                }
                else
                {
                    ethNicHandle.ethHandle->multicastFramePass = false;
                }

                if (setup->wValue & USB_DEVICE_CDC_ECM_PACKET_TYPE_DIRECTED_MASK)
                {
                    ethNicHandle.ethHandle->unicastFramePass = true;
                }
                else
                {
                    ethNicHandle.ethHandle->unicastFramePass = false;
                }

                if (setup->wValue & USB_DEVICE_CDC_ECM_PACKET_TYPE_BROADCAST_MASK)
                {
                    ethNicHandle.ethHandle->boardcastFramePass = true;
                }
                else
                {
                    ethNicHandle.ethHandle->boardcastFramePass = false;
                }
            }
            status = kStatus_USB_Success;
            break;

        default:
            break;
    }

    return status;
}

usb_status_t USB_DeviceGetSetupBuffer(usb_device_handle handle, usb_setup_struct_t **setupBuffer)
{
    static usb_setup_struct_t buffer;

    if (setupBuffer == NULL)
    {
        return kStatus_USB_InvalidParameter;
    }

    *setupBuffer = &buffer;

    return kStatus_USB_Success;
}

usb_status_t USB_DeviceGetClassReceiveBuffer(usb_device_handle handle, usb_setup_struct_t *setup, uint32_t *length, uint8_t **buffer)
{
    static uint32_t recvBuffer;

    if ((NULL == buffer) || ((*length) > 4))
    {
        return kStatus_USB_InvalidRequest;
    }
    *buffer = (uint8_t *)&recvBuffer;
    return kStatus_USB_Success;
}

usb_status_t USB_DeviceCdcEcmSend(usb_device_handle handle, uint8_t ep, uint8_t *buffer, uint32_t length)
{
    usb_status_t status = kStatus_USB_InvalidParameter;

    if (NULL == handle)
    {
        return status;
    }

    switch (ep)
    {
        case USB_DEVICE_CDC_ECM_COMM_INTERRUPT_IN_EP_NUMBER:
            if (cdcEcmPipe.interruptInIsBusy)
            {
                return kStatus_USB_Busy;
            }

            cdcEcmPipe.interruptInIsBusy = 1U;
            break;

        case USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_NUMBER:
            if (cdcEcmPipe.bulkInIsBusy)
            {
                return kStatus_USB_Busy;
            }

            cdcEcmPipe.bulkInIsBusy = 1U;
            break;

        default:
            return status;
    }

    status = USB_DeviceSendRequest(handle, ep, buffer, length);
    if (kStatus_USB_Success != status)
    {
        switch (ep)
        {
            case USB_DEVICE_CDC_ECM_COMM_INTERRUPT_IN_EP_NUMBER:
                cdcEcmPipe.interruptInIsBusy = 0U;
                break;

            case USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_NUMBER:
                cdcEcmPipe.bulkInIsBusy = 0U;
                break;

            default:
                break;
        }
    }

    return status;
}

usb_status_t USB_DeviceCdcEcmRecv(usb_device_handle handle, uint8_t ep, uint8_t *buffer, uint32_t length)
{
    usb_status_t status = kStatus_USB_InvalidParameter;

    if (NULL == handle)
    {
        return kStatus_USB_InvalidHandle;
    }

    switch (ep)
    {
        case USB_DEVICE_CDC_ECM_DATA_BULK_OUT_EP_NUMBER:
            if (cdcEcmPipe.bulkOutIsBusy)
            {
                return kStatus_USB_Busy;
            }

            cdcEcmPipe.bulkOutIsBusy = 1U;
            break;

        default:
            return status;
    }

    status = USB_DeviceRecvRequest(handle, ep, buffer, length);
    if (kStatus_USB_Success != status)
    {
        switch (ep)
        {
            case USB_DEVICE_CDC_ECM_DATA_BULK_OUT_EP_NUMBER:
                cdcEcmPipe.bulkOutIsBusy = 0U;
                break;

            default:
                break;
        }
    }

    return status;
}
