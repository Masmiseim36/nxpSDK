/*
 * Copyright 2017-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"

#include <stdio.h>
#include <stdlib.h>
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_cdc_acm.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "virtual_com.h"
#include "composite.h"
#include "usb_device_descriptor.h"

#include "fsl_component_serial_port_internal.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

serial_manager_status_t Serial_UsbCdcDeinit(serial_handle_t serialHandle)
{
    serial_usb_cdc_state_t *serialUsbCdc;

    assert(serialHandle);

    serialUsbCdc = (serial_usb_cdc_state_t *)(*(uint32_t *)serialHandle);

    USB_DeviceClassDeinit(serialUsbCdc->instance);

    //    USB_DeviceRemoveItem(&s_UsbCdcHead, serialUsbCdc);

    return kStatus_SerialManager_Success;
}

serial_manager_status_t Serial_UsbCdcWrite(serial_handle_t serialHandle, uint8_t *buffer, uint32_t length)
{
    serial_usb_cdc_state_t *serialUsbCdc;
    uint32_t needToPrime = 0;

    assert(serialHandle);

    serialUsbCdc = (serial_usb_cdc_state_t *)(*(uint32_t *)serialHandle);

    if (serialUsbCdc->tx.busy)
    {
        return kStatus_SerialManager_Busy;
    }

    /* Prevents SerialManager_Write from blocking when no USB Host is attached */
    if ((serialUsbCdc->attach == 0))
    {
        return kStatus_SerialManager_NotConnected;
    }

    /* Prevents SerialManager_Write from blocking when USB Host is attached but CDC terminal is closed */
    if ((serialUsbCdc->attach == 1) && (serialUsbCdc->startTransactions == 0))
    {
        return kStatus_SerialManager_NotConnected;
    }

    serialUsbCdc->tx.busy          = 1;
    serialUsbCdc->tx.waiting4Prime = 0;

    serialUsbCdc->tx.buffer = buffer;
    serialUsbCdc->tx.length = length;

    if ((serialUsbCdc->attach))
    {
        needToPrime = 1;
    }

    if (needToPrime)
    {
        if (kStatus_USB_Success !=
            USB_DeviceCdcAcmSend(serialUsbCdc->cdcAcmHandle, serialUsbCdc->bulkInEndpoint, buffer, length))
        {
            serialUsbCdc->tx.busy = 0;
            return kStatus_SerialManager_Error;
        }
    }
    else
    {
        serialUsbCdc->tx.waiting4Prime = 1;
    }

    return kStatus_SerialManager_Success;
}

serial_manager_status_t Serial_UsbCdcRead(serial_handle_t serialHandle, uint8_t *buffer, uint32_t length)
{
    serial_usb_cdc_state_t *serialUsbCdc;
    uint8_t *primeBuffer;
    uint32_t primeLength;
    uint32_t regPrimask;

    assert(serialHandle);

    serialUsbCdc = (serial_usb_cdc_state_t *)(*(uint32_t *)serialHandle);

    if (!(serialUsbCdc->attach))
    {
        return kStatus_SerialManager_Error;
    }

    regPrimask = DisableGlobalIRQ();
    if (serialUsbCdc->rx.busy)
    {
        EnableGlobalIRQ(regPrimask);
        return kStatus_SerialManager_Busy;
    }
    serialUsbCdc->rx.busy = 1U;
    EnableGlobalIRQ(regPrimask);

    if (length < serialUsbCdc->bulkOutEndpointMaxPacketSize)
    {
        serialUsbCdc->rx.busy = 0U;
        return kStatus_SerialManager_Error;
    }

    if (NULL == buffer)
    {
        primeBuffer = serialUsbCdc->currRecvBuf;
        primeLength = serialUsbCdc->bulkOutEndpointMaxPacketSize;
    }
    else
    {
        primeBuffer = buffer;
        primeLength = length;
    }
    /* Schedule buffer for next receive event */

    if (kStatus_USB_Success !=
        USB_DeviceCdcAcmRecv(serialUsbCdc->cdcAcmHandle, serialUsbCdc->bulkOutEndpoint, primeBuffer, primeLength))
    {
        serialUsbCdc->rx.busy = 0U;
        return kStatus_SerialManager_Error;
    }

    return kStatus_SerialManager_Success;
}

serial_manager_status_t Serial_UsbCdcCancelWrite(serial_handle_t serialHandle)
{
    serial_usb_cdc_state_t *serialUsbCdc;

    assert(serialHandle);

    serialUsbCdc = (serial_usb_cdc_state_t *)(*(uint32_t *)serialHandle);
    USB_DeviceCancel(serialUsbCdc->deviceHandle,
                     (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT) | serialUsbCdc->bulkInEndpoint);
    return kStatus_SerialManager_Success;
}

serial_manager_status_t Serial_UsbCdcInstallTxCallback(serial_handle_t serialHandle,
                                                       serial_manager_callback_t callback,
                                                       void *callbackParam)
{
    serial_usb_cdc_state_t *serialUsbCdc;

    assert(serialHandle);

    serialUsbCdc = (serial_usb_cdc_state_t *)(*(uint32_t *)serialHandle);

    serialUsbCdc->tx.callback      = callback;
    serialUsbCdc->tx.callbackParam = callbackParam;

    return kStatus_SerialManager_Success;
}

serial_manager_status_t Serial_UsbCdcInstallRxCallback(serial_handle_t serialHandle,
                                                       serial_manager_callback_t callback,
                                                       void *callbackParam)
{
    serial_usb_cdc_state_t *serialUsbCdc;

    assert(serialHandle);

    serialUsbCdc = (serial_usb_cdc_state_t *)(*(uint32_t *)serialHandle);

    serialUsbCdc->rx.callback      = callback;
    serialUsbCdc->rx.callbackParam = callbackParam;

    return kStatus_SerialManager_Success;
}

void Serial_UsbCdcIsrFunction(serial_handle_t serialHandle)
{
    serial_usb_cdc_state_t *serialUsbCdc;

    assert(serialHandle);

    serialUsbCdc = (serial_usb_cdc_state_t *)(*(uint32_t *)serialHandle);
#if 0
    DisableIRQ((IRQn_Type)serialUsbCdc->irqNumber);
#endif
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
    if ((kSerialManager_UsbControllerEhci0 == serialUsbCdc->instance) ||
        (kSerialManager_UsbControllerEhci1 == serialUsbCdc->instance))
    {
        USB_DeviceEhciIsrFunction(serialUsbCdc->deviceHandle);
    }
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0)
    if ((kSerialManager_UsbControllerKhci0 == serialUsbCdc->instance) ||
        (kSerialManager_UsbControllerKhci1 == serialUsbCdc->instance))
    {
        USB_DeviceKhciIsrFunction(serialUsbCdc->deviceHandle);
    }
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
    if ((kSerialManager_UsbControllerLpcIp3511Fs0 == serialUsbCdc->instance) ||
        (kSerialManager_UsbControllerLpcIp3511Fs1 == serialUsbCdc->instance))
    {
        USB_DeviceLpcIp3511IsrFunction(serialUsbCdc->deviceHandle);
    }
#endif

#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
    if ((kSerialManager_UsbControllerLpcIp3511Hs0 == serialUsbCdc->instance) ||
        (kSerialManager_UsbControllerLpcIp3511Hs1 == serialUsbCdc->instance))
    {
        USB_DeviceLpcIp3511IsrFunction(serialUsbCdc->deviceHandle);
    }
#endif
#if 0
    EnableIRQ((IRQn_Type)serialUsbCdc->irqNumber);
#endif
}
