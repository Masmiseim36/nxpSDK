/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017, 2019 NXP
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

#include "usb_device_cdc_acm.h"
#include "usb_device_ch9.h"
#include "fsl_debug_console.h"

#include "usb_device_descriptor.h"
#include "composite.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Weak function. */
#if defined(__GNUC__)
#define __WEAK_FUNC __attribute__((weak))
#elif defined(__ICCARM__)
#define __WEAK_FUNC __weak
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
#define __WEAK_FUNC __attribute__((weak))
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Line coding of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_lineCoding[LINE_CODING_SIZE] = {
    /* E.g. 0x00,0xC2,0x01,0x00 : 0x0001C200 is 115200 bits per second */
    (LINE_CODING_DTERATE >> 0U) & 0x000000FFU,
    (LINE_CODING_DTERATE >> 8U) & 0x000000FFU,
    (LINE_CODING_DTERATE >> 16U) & 0x000000FFU,
    (LINE_CODING_DTERATE >> 24U) & 0x000000FFU,
    LINE_CODING_CHARFORMAT,
    LINE_CODING_PARITYTYPE,
    LINE_CODING_DATABITS};

/* Abstract state of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_abstractState[COMM_FEATURE_DATA_SIZE] = {(STATUS_ABSTRACT_STATE >> 0U) & 0x00FFU,
                                                          (STATUS_ABSTRACT_STATE >> 8U) & 0x00FFU};

/* Country code of cdc device */
USB_DMA_INIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_countryCode[COMM_FEATURE_DATA_SIZE] = {(COUNTRY_SETTING >> 0U) & 0x00FFU,
                                                        (COUNTRY_SETTING >> 8U) & 0x00FFU};

/* CDC ACM information */
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static usb_cdc_acm_info_t s_usbCdcAcmInfo;
/* Data buffer for receiving and sending*/
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_currRecvBuf[DATA_BUFF_SIZE];
volatile static uint32_t s_recvSize    = 0;
static uint32_t s_usbBulkMaxPacketSize = FS_CDC_VCOM_BULK_OUT_PACKET_SIZE;
volatile static usb_device_composite_struct_t *s_deviceComposite;

static usb_cdc_callback_t s_deviceTxCallback;
static usb_cdc_callback_t s_deviceRxCallback;

/*******************************************************************************
 * Code
 ******************************************************************************/

void USB_DeviceVcomTxDone(uint8_t *buffer, uint32_t length, serial_manager_status_t status)
{
    serial_manager_callback_message_t msg;

    if ((NULL != s_deviceTxCallback.callback))
    {
        msg.buffer = buffer;
        msg.length = length;
        s_deviceTxCallback.callback(s_deviceTxCallback.callbackParam, &msg, status);
    }
}

void USB_DeviceVcomRxDone(uint8_t *buffer, uint32_t length, serial_manager_status_t status)
{
    serial_manager_callback_message_t msg;

    if ((NULL != s_deviceRxCallback.callback))
    {
        msg.buffer = buffer;
        msg.length = length;
        s_deviceRxCallback.callback(s_deviceRxCallback.callbackParam, &msg, status);
    }
}

/*!
 * @brief Interrupt in pipe callback function.
 *
 * This function serves as the callback function for interrupt in pipe.
 *
 * @param handle The USB device handle.
 * @param message The endpoint callback message
 * @param callbackParam The parameter of the callback.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcAcmInterruptIn(usb_device_handle handle,
                                         usb_device_endpoint_callback_message_struct_t *message,
                                         void *callbackParam)
{
    usb_status_t error                      = kStatus_USB_Error;
    s_deviceComposite->cdcVcom.hasSentState = 0;
    if ((s_deviceComposite->cdcVcom.startTransactions) && (s_deviceComposite->cdcVcom.tx.waiting4Prime))
    {
        s_deviceComposite->cdcVcom.tx.waiting4Prime = 0U;
        if (kStatus_USB_Success !=
            USB_DeviceSendRequest(s_deviceComposite->deviceHandle, USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT,
                                  s_deviceComposite->cdcVcom.tx.buffer, s_deviceComposite->cdcVcom.tx.length))
        {
            /* Prime failure */
            /* TODO: Callback with errors */
            s_deviceComposite->cdcVcom.tx.busy = 0U;
            USB_DeviceVcomTxDone(s_deviceComposite->cdcVcom.tx.buffer, s_deviceComposite->cdcVcom.tx.length,
                                 kStatus_SerialManager_Error);
        }
    }
    return error;
}

/*!
 * @brief Bulk in pipe callback function.
 *
 * This function serves as the callback function for bulk in pipe.
 *
 * @param handle The USB device handle.
 * @param message The endpoint callback message
 * @param callbackParam The parameter of the callback.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcAcmBulkIn(usb_device_handle handle,
                                    usb_device_endpoint_callback_message_struct_t *message,
                                    void *callbackParam)
{
    usb_status_t error = kStatus_USB_Error;

    if ((1 == s_deviceComposite->attach))
    {
        if ((message->length != 0) && (!(message->length % s_usbBulkMaxPacketSize)))
        {
            /* If the last packet is the size of endpoint, then send also zero-ended packet,
             ** meaning that we want to inform the host that we do not have any additional
             ** data, so it can flush the output.
             */
            error = USB_DeviceSendRequest(handle, USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT, NULL, 0);
        }
        else
        {
            serial_manager_status_t callbackStatus = kStatus_SerialManager_Success;
            s_deviceComposite->cdcVcom.tx.busy     = 0U;
            if (USB_UNINITIALIZED_VAL_32 == message->length)
            {
                callbackStatus = kStatus_SerialManager_Canceled;
            }
            USB_DeviceVcomTxDone(s_deviceComposite->cdcVcom.tx.buffer, s_deviceComposite->cdcVcom.tx.length,
                                 callbackStatus);
        }
    }
    else
    {
    }
    return error;
}

/*!
 * @brief Bulk out pipe callback function.
 *
 * This function serves as the callback function for bulk out pipe.
 *
 * @param handle The USB device handle.
 * @param message The endpoint callback message
 * @param callbackParam The parameter of the callback.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcAcmBulkOut(usb_device_handle handle,
                                     usb_device_endpoint_callback_message_struct_t *message,
                                     void *callbackParam)
{
    usb_status_t error = kStatus_USB_Error;

    s_deviceComposite->cdcVcom.rx.busy = 0U;
    if ((1U == s_deviceComposite->cdcVcom.startTransactions))
    {
        serial_manager_status_t callbackStatus = kStatus_SerialManager_Success;
        if (USB_UNINITIALIZED_VAL_32 == message->length)
        {
            message->length = 0U;
            callbackStatus  = kStatus_SerialManager_Canceled;
        }
        USB_DeviceVcomRxDone(message->buffer, message->length, callbackStatus);
    }
    return error;
}

/*!
 * @brief USB configure endpoint function.
 *
 * This function configure endpoint status.
 *
 * @param handle The USB device handle.
 * @param ep Endpoint address.
 * @param status A flag to indicate whether to stall the endpoint. 1: stall, 0: unstall.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcVcomConfigureEndpointStatus(usb_device_handle handle, uint8_t ep, uint8_t status)
{
    usb_status_t error = kStatus_USB_Error;
    if (status)
    {
        if ((USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
            (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK))
        {
            error = USB_DeviceStallEndpoint(handle, ep);
        }
        else if ((USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
                 (!(ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK)))
        {
            error = USB_DeviceStallEndpoint(handle, ep);
        }
        else
        {
        }
    }
    else
    {
        if ((USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
            (ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK))
        {
            error = USB_DeviceUnstallEndpoint(handle, ep);
        }
        else if ((USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT == (ep & USB_ENDPOINT_NUMBER_MASK)) &&
                 (!(ep & USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_MASK)))
        {
            error = USB_DeviceUnstallEndpoint(handle, ep);
        }
        else
        {
        }
    }
    return error;
}

/*!
 * @brief CDC class specific callback function.
 *
 * This function handles the CDC class specific requests.
 *
 * @param handle The USB device handle.
 * @param setup The pointer to the setup packet.
 * @param length The pointer to the length of the data buffer.
 * @param buffer The pointer to the address of setup packet data buffer.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcVcomClassRequest(usb_device_handle handle,
                                           usb_setup_struct_t *setup,
                                           uint32_t *length,
                                           uint8_t **buffer)
{
    usb_status_t error = kStatus_USB_InvalidRequest;

    usb_cdc_acm_info_t *acmInfo = &s_usbCdcAcmInfo;
    uint32_t len;
    uint8_t *uartBitmap;
    if (setup->wIndex != USB_CDC_VCOM_CIC_INTERFACE_INDEX)
    {
        return error;
    }

    switch (setup->bRequest)
    {
        case USB_DEVICE_CDC_REQUEST_SEND_ENCAPSULATED_COMMAND:
            break;
        case USB_DEVICE_CDC_REQUEST_GET_ENCAPSULATED_RESPONSE:
            break;
        case USB_DEVICE_CDC_REQUEST_SET_COMM_FEATURE:
            if (USB_DEVICE_CDC_FEATURE_ABSTRACT_STATE == setup->wValue)
            {
                *buffer = s_abstractState;
            }
            else if (USB_DEVICE_CDC_FEATURE_COUNTRY_SETTING == setup->wValue)
            {
                *buffer = s_countryCode;
            }
            else
            {
            }
            error = kStatus_USB_Success;
            break;
        case USB_DEVICE_CDC_REQUEST_GET_COMM_FEATURE:
            if (USB_DEVICE_CDC_FEATURE_ABSTRACT_STATE == setup->wValue)
            {
                *buffer = s_abstractState;
                *length = COMM_FEATURE_DATA_SIZE;
            }
            else if (USB_DEVICE_CDC_FEATURE_COUNTRY_SETTING == setup->wValue)
            {
                *buffer = s_countryCode;
                *length = COMM_FEATURE_DATA_SIZE;
            }
            else
            {
            }
            error = kStatus_USB_Success;
            break;
        case USB_DEVICE_CDC_REQUEST_CLEAR_COMM_FEATURE:
            break;
        case USB_DEVICE_CDC_REQUEST_GET_LINE_CODING:
            *buffer = s_lineCoding;
            *length = LINE_CODING_SIZE;
            error   = kStatus_USB_Success;
            break;
        case USB_DEVICE_CDC_REQUEST_SET_LINE_CODING:
            *buffer = s_lineCoding;
            error   = kStatus_USB_Success;
            break;
        case USB_DEVICE_CDC_REQUEST_SET_CONTROL_LINE_STATE:
        {
            acmInfo->dteStatus = setup->wValue;
            /* activate/deactivate Tx carrier */
            if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_CARRIER_ACTIVATION)
            {
                acmInfo->uartState |= USB_DEVICE_CDC_UART_STATE_TX_CARRIER;
            }
            else
            {
                acmInfo->uartState &= (uint16_t)~USB_DEVICE_CDC_UART_STATE_TX_CARRIER;
            }

            /* activate carrier and DTE. Com port of terminal tool running on PC is open now */
            if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE)
            {
                acmInfo->uartState |= USB_DEVICE_CDC_UART_STATE_RX_CARRIER;
            }
            /* Com port of terminal tool running on PC is closed now */
            else
            {
                acmInfo->uartState &= (uint16_t)~USB_DEVICE_CDC_UART_STATE_RX_CARRIER;
            }

            /* Indicates to DCE if DTE is present or not */
            acmInfo->dtePresent = (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE) ? true : false;

            /* Initialize the serial state buffer */
            acmInfo->serialStateBuf[0] = NOTIF_REQUEST_TYPE;                        /* bmRequestType */
            acmInfo->serialStateBuf[1] = USB_DEVICE_CDC_REQUEST_SERIAL_STATE_NOTIF; /* bNotification */
            acmInfo->serialStateBuf[2] = 0x00;                                      /* wValue */
            acmInfo->serialStateBuf[3] = 0x00;
            acmInfo->serialStateBuf[4] = 0x00; /* wIndex */
            acmInfo->serialStateBuf[5] = 0x00;
            acmInfo->serialStateBuf[6] = UART_BITMAP_SIZE; /* wLength */
            acmInfo->serialStateBuf[7] = 0x00;
            /* Notify to host the line state */
            acmInfo->serialStateBuf[4] = setup->wIndex;
            /* Lower byte of UART BITMAP */
            uartBitmap    = (uint8_t *)&acmInfo->serialStateBuf[NOTIF_PACKET_SIZE + UART_BITMAP_SIZE - 2];
            uartBitmap[0] = acmInfo->uartState & 0xFFu;
            uartBitmap[1] = (acmInfo->uartState >> 8) & 0xFFu;
            len           = (uint32_t)(NOTIF_PACKET_SIZE + UART_BITMAP_SIZE);
            if (0 == s_deviceComposite->cdcVcom.hasSentState)
            {
                error =
                    USB_DeviceSendRequest(handle, USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT, acmInfo->serialStateBuf, len);
                if (kStatus_USB_Success != error)
                {
                    usb_echo("kUSB_DeviceCdcEventSetControlLineState error!");
                }
                s_deviceComposite->cdcVcom.hasSentState = 1;
            }
            /* Update status */
            if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_CARRIER_ACTIVATION)
            {
                /*    To do: CARRIER_ACTIVATED */
            }
            else
            {
                /* To do: CARRIER_DEACTIVATED */
            }
            if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE)
            {
                /* DTE_ACTIVATED */
                if (1 == s_deviceComposite->attach)
                {
                    s_deviceComposite->cdcVcom.startTransactions = 1;
                }
            }
            else
            {
                /* DTE_DEACTIVATED */
                if (1 == s_deviceComposite->attach)
                {
                    s_deviceComposite->cdcVcom.startTransactions = 0;
                }
            }
        }
        break;
        case USB_DEVICE_CDC_REQUEST_SEND_BREAK:
            break;
        default:
            break;
    }

    return error;
}

/*!
 * @brief Virtual COM device set configuration function.
 *
 * This function sets configuration for CDC class.
 *
 * @param handle The CDC ACM class handle.
 * @param configure The CDC ACM class configure index.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcVcomSetConfigure(usb_device_handle handle, uint8_t configure)
{
    usb_device_endpoint_init_struct_t epInitStruct;
    usb_device_endpoint_callback_struct_t epCallback;

    if (USB_COMPOSITE_CONFIGURE_INDEX == s_deviceComposite->currentConfiguration)
    {
        USB_DeviceDeinitEndpoint(
            s_deviceComposite->deviceHandle,
            (USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT)));
        USB_DeviceDeinitEndpoint(
            s_deviceComposite->deviceHandle,
            (USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT)));
        USB_DeviceDeinitEndpoint(
            s_deviceComposite->deviceHandle,
            (USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT | (USB_OUT << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT)));
    }
    if (USB_COMPOSITE_CONFIGURE_INDEX == configure)
    {
        s_deviceComposite->attach = 1;

        /* Initiailize endpoint for interrupt pipe */
        epCallback.callbackFn    = USB_DeviceCdcAcmInterruptIn;
        epCallback.callbackParam = handle;

        epInitStruct.zlt          = 0;
        epInitStruct.transferType = USB_ENDPOINT_INTERRUPT;
        epInitStruct.endpointAddress =
            USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
        if (USB_SPEED_HIGH == s_deviceComposite->speed)
        {
            epInitStruct.maxPacketSize = HS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE;
            epInitStruct.interval      = HS_CDC_VCOM_INTERRUPT_IN_INTERVAL;
        }
        else
        {
            epInitStruct.maxPacketSize = FS_CDC_VCOM_INTERRUPT_IN_PACKET_SIZE;
            epInitStruct.interval      = FS_CDC_VCOM_INTERRUPT_IN_INTERVAL;
        }

        USB_DeviceInitEndpoint(handle, &epInitStruct, &epCallback);

        /* Initiailize endpoints for bulk pipe */
        epCallback.callbackFn    = USB_DeviceCdcAcmBulkIn;
        epCallback.callbackParam = handle;

        epInitStruct.zlt          = 0U;
        epInitStruct.interval     = 0U;
        epInitStruct.transferType = USB_ENDPOINT_BULK;
        epInitStruct.endpointAddress =
            USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
        if (USB_SPEED_HIGH == s_deviceComposite->speed)
        {
            epInitStruct.maxPacketSize = HS_CDC_VCOM_BULK_IN_PACKET_SIZE;
        }
        else
        {
            epInitStruct.maxPacketSize = FS_CDC_VCOM_BULK_IN_PACKET_SIZE;
        }

        USB_DeviceInitEndpoint(handle, &epInitStruct, &epCallback);

        epCallback.callbackFn    = USB_DeviceCdcAcmBulkOut;
        epCallback.callbackParam = handle;

        epInitStruct.zlt          = 0;
        epInitStruct.transferType = USB_ENDPOINT_BULK;
        epInitStruct.endpointAddress =
            USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT | (USB_OUT << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT);
        if (USB_SPEED_HIGH == s_deviceComposite->speed)
        {
            epInitStruct.maxPacketSize = HS_CDC_VCOM_BULK_OUT_PACKET_SIZE;
        }
        else
        {
            epInitStruct.maxPacketSize = FS_CDC_VCOM_BULK_OUT_PACKET_SIZE;
        }

        USB_DeviceInitEndpoint(handle, &epInitStruct, &epCallback);

        if (USB_SPEED_HIGH == s_deviceComposite->speed)
        {
            s_usbBulkMaxPacketSize = HS_CDC_VCOM_BULK_OUT_PACKET_SIZE;
        }
        else
        {
            s_usbBulkMaxPacketSize = FS_CDC_VCOM_BULK_OUT_PACKET_SIZE;
        }

        /* Schedule buffer for receive */

        /* Schedule buffer for receive */
        s_deviceComposite->cdcVcom.rx.busy = 1U;
        if (kStatus_USB_Success !=
            USB_DeviceRecvRequest(handle, USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT, s_currRecvBuf, s_usbBulkMaxPacketSize))
        {
            s_deviceComposite->cdcVcom.rx.busy = 0U;
            return kStatus_USB_Error;
        }
    }
    return kStatus_USB_Success;
}

/*!
 * @brief Virtual COM device initialization function.
 *
 * This function initializes the device with the composite device class information.
 *
 * @param deviceComposite The pointer to the composite device structure.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcVcomInit(usb_device_composite_struct_t *deviceComposite)
{
    s_deviceComposite = deviceComposite;
    return kStatus_USB_Success;
}

status_t USB_DeviceVcomInit(uint8_t controller)
{
    return kStatus_Success;
}

status_t USB_DeviceVcomDeinit(uint8_t controller)
{
    return kStatus_Success;
}

status_t USB_DeviceVcomWrite(uint8_t controller, uint8_t *buffer, uint32_t length)
{
    uint32_t needToPrime = 0;
    uint32_t regPrimask;

    if (controller != (uint8_t)CONTROLLER_ID)
    {
        return kStatus_Fail;
    }

    regPrimask = DisableGlobalIRQ();
    if (s_deviceComposite->cdcVcom.tx.busy)
    {
        EnableGlobalIRQ(regPrimask);
        return kStatus_Fail;
    }
    s_deviceComposite->cdcVcom.tx.busy = 1U;
    EnableGlobalIRQ(regPrimask);

    s_deviceComposite->cdcVcom.tx.waiting4Prime = 0U;
    s_deviceComposite->cdcVcom.tx.buffer        = buffer;
    s_deviceComposite->cdcVcom.tx.length        = length;

    if ((s_deviceComposite->attach))
    {
        needToPrime = 1;
    }

    if (needToPrime)
    {
        if (kStatus_USB_Success !=
            USB_DeviceSendRequest(s_deviceComposite->deviceHandle, USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT, buffer, length))
        {
            s_deviceComposite->cdcVcom.tx.busy = 0U;
            return kStatus_Fail;
        }
    }
    else
    {
        s_deviceComposite->cdcVcom.tx.waiting4Prime = 1U;
    }
    return kStatus_Success;
}

status_t USB_DeviceVcomRead(uint8_t controller, uint8_t *buffer, uint32_t length)
{
    uint8_t *primeBuffer;
    uint32_t primeLength;
    uint32_t regPrimask;

    if (controller != (uint8_t)CONTROLLER_ID)
    {
        return kStatus_Fail;
    }

    if (!(s_deviceComposite->attach))
    {
        return kStatus_Fail;
    }

    regPrimask = DisableGlobalIRQ();
    if (s_deviceComposite->cdcVcom.rx.busy)
    {
        EnableGlobalIRQ(regPrimask);
        return kStatus_Fail;
    }
    s_deviceComposite->cdcVcom.rx.busy = 1U;
    EnableGlobalIRQ(regPrimask);

    if (length < s_usbBulkMaxPacketSize)
    {
        s_deviceComposite->cdcVcom.rx.busy = 0U;
        return kStatus_Fail;
    }

    if (NULL == buffer)
    {
        primeBuffer = s_currRecvBuf;
        primeLength = s_usbBulkMaxPacketSize;
    }
    else
    {
        primeBuffer = buffer;
        primeLength = length;
    }
    /* Schedule buffer for next receive event */

    if (kStatus_USB_Success != USB_DeviceRecvRequest(s_deviceComposite->deviceHandle,
                                                     USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT, primeBuffer, primeLength))
    {
        s_deviceComposite->cdcVcom.rx.busy = 0U;
        return kStatus_Fail;
    }

    return kStatus_Success;
}

status_t USB_DeviceVcomCancelWrite(uint8_t controller)
{
    if (controller != (uint8_t)CONTROLLER_ID)
    {
        return kStatus_Fail;
    }

    USB_DeviceCancel(s_deviceComposite->deviceHandle,
                     (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT) | USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT);
    return kStatus_Success;
}

status_t USB_DeviceVcomInstallTxCallback(uint8_t controller, serial_manager_callback_t callback, void *callbackParam)
{
    if (controller != (uint8_t)CONTROLLER_ID)
    {
        return kStatus_Fail;
    }

    s_deviceTxCallback.callback      = callback;
    s_deviceTxCallback.callbackParam = callbackParam;

    return kStatus_Success;
}

status_t USB_DeviceVcomInstallRxCallback(uint8_t controller, serial_manager_callback_t callback, void *callbackParam)
{
    if (controller != (uint8_t)CONTROLLER_ID)
    {
        return kStatus_Fail;
    }

    s_deviceRxCallback.callback      = callback;
    s_deviceRxCallback.callbackParam = callbackParam;

    return kStatus_Success;
}
