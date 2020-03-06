/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016, 2019 NXP
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
extern usb_device_endpoint_struct_t g_cdcVcomDicEndpoints[];

/* Line codinig of cdc device */
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

static usb_device_composite_struct_t *s_deviceComposite;

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
 * @brief CDC class specific callback function.
 *
 * This function handles the CDC class specific requests.
 *
 * @param handle          The CDC ACM class handle.
 * @param event           The CDC ACM class event type.
 * @param param           The parameter of the class specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCdcVcomCallback(class_handle_t handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Error;
    uint32_t len;
    uint8_t *uartBitmap;
    usb_cdc_acm_info_t *acmInfo = &s_usbCdcAcmInfo;
    usb_device_cdc_acm_request_param_struct_t *acmReqParam;
    usb_device_endpoint_callback_message_struct_t *epCbParam;

    acmReqParam = (usb_device_cdc_acm_request_param_struct_t *)param;
    epCbParam   = (usb_device_endpoint_callback_message_struct_t *)param;
    switch (event)
    {
        case kUSB_DeviceCdcEventSendResponse:
        {
            if ((1 == s_deviceComposite->attach))
            {
                if ((epCbParam->length != 0) && (!(epCbParam->length % g_cdcVcomDicEndpoints[0].maxPacketSize)))
                {
                    /* If the last packet is the size of endpoint, then send also zero-ended packet,
                     ** meaning that we want to inform the host that we do not have any additional
                     ** data, so it can flush the output.
                     */
                    error = USB_DeviceCdcAcmSend(handle, USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT, NULL, 0);
                }
                else
                {
                    serial_manager_status_t callbackStatus = kStatus_SerialManager_Success;
                    s_deviceComposite->cdcVcom.tx.busy     = 0U;
                    if (USB_UNINITIALIZED_VAL_32 == epCbParam->length)
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
        }
        break;
        case kUSB_DeviceCdcEventRecvResponse:
        {
            s_deviceComposite->cdcVcom.rx.busy = 0U;
            if ((1U == s_deviceComposite->cdcVcom.startTransactions))
            {
                serial_manager_status_t callbackStatus = kStatus_SerialManager_Success;
                if (USB_UNINITIALIZED_VAL_32 == epCbParam->length)
                {
                    epCbParam->length = 0U;
                    callbackStatus    = kStatus_SerialManager_Canceled;
                }
                USB_DeviceVcomRxDone(epCbParam->buffer, epCbParam->length, callbackStatus);
            }
        }
        break;
        case kUSB_DeviceCdcEventSerialStateNotif:
            ((usb_device_cdc_acm_struct_t *)handle)->hasSentState = 0;
            if ((s_deviceComposite->cdcVcom.startTransactions) && (s_deviceComposite->cdcVcom.tx.waiting4Prime))
            {
                s_deviceComposite->cdcVcom.tx.waiting4Prime = 0U;
                if (kStatus_USB_Success !=
                    USB_DeviceCdcAcmSend(s_deviceComposite->cdcVcom.cdcAcmHandle, USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT,
                                         s_deviceComposite->cdcVcom.tx.buffer, s_deviceComposite->cdcVcom.tx.length))
                {
                    /* Prime failure */
                    /* TODO: Callback with errors */
                    s_deviceComposite->cdcVcom.tx.busy = 0U;
                    USB_DeviceVcomTxDone(s_deviceComposite->cdcVcom.tx.buffer, s_deviceComposite->cdcVcom.tx.length,
                                         kStatus_SerialManager_Error);
                }
            }
            error = kStatus_USB_Success;
            break;
        case kUSB_DeviceCdcEventSendEncapsulatedCommand:
            break;
        case kUSB_DeviceCdcEventGetEncapsulatedResponse:
            break;
        case kUSB_DeviceCdcEventSetCommFeature:
            if (USB_DEVICE_CDC_FEATURE_ABSTRACT_STATE == acmReqParam->setupValue)
            {
                if (1 == acmReqParam->isSetup)
                {
                    *(acmReqParam->buffer) = s_abstractState;
                }
                else
                {
                    *(acmReqParam->length) = 0;
                }
            }
            else if (USB_DEVICE_CDC_FEATURE_COUNTRY_SETTING == acmReqParam->setupValue)
            {
                if (1 == acmReqParam->isSetup)
                {
                    *(acmReqParam->buffer) = s_countryCode;
                }
                else
                {
                    *(acmReqParam->length) = 0;
                }
            }
            else
            {
            }
            error = kStatus_USB_Success;
            break;
        case kUSB_DeviceCdcEventGetCommFeature:
            if (USB_DEVICE_CDC_FEATURE_ABSTRACT_STATE == acmReqParam->setupValue)
            {
                *(acmReqParam->buffer) = s_abstractState;
                *(acmReqParam->length) = COMM_FEATURE_DATA_SIZE;
            }
            else if (USB_DEVICE_CDC_FEATURE_COUNTRY_SETTING == acmReqParam->setupValue)
            {
                *(acmReqParam->buffer) = s_countryCode;
                *(acmReqParam->length) = COMM_FEATURE_DATA_SIZE;
            }
            else
            {
            }
            error = kStatus_USB_Success;
            break;
        case kUSB_DeviceCdcEventClearCommFeature:
            break;
        case kUSB_DeviceCdcEventGetLineCoding:
            *(acmReqParam->buffer) = s_lineCoding;
            *(acmReqParam->length) = LINE_CODING_SIZE;
            error                  = kStatus_USB_Success;
            break;
        case kUSB_DeviceCdcEventSetLineCoding:
        {
            if (1 == acmReqParam->isSetup)
            {
                *(acmReqParam->buffer) = s_lineCoding;
            }
            else
            {
                *(acmReqParam->length) = 0;
            }
        }
            error = kStatus_USB_Success;
            break;
        case kUSB_DeviceCdcEventSetControlLineState:
        {
            s_usbCdcAcmInfo.dteStatus = acmReqParam->setupValue;
            /* activate/deactivate Tx carrier */
            if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_CARRIER_ACTIVATION)
            {
                acmInfo->uartState |= USB_DEVICE_CDC_UART_STATE_TX_CARRIER;
            }
            else
            {
                acmInfo->uartState &= (uint16_t)~USB_DEVICE_CDC_UART_STATE_TX_CARRIER;
            }

            /* activate carrier and DTE */
            if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE)
            {
                acmInfo->uartState |= USB_DEVICE_CDC_UART_STATE_RX_CARRIER;
            }
            else
            {
                acmInfo->uartState &= (uint16_t)~USB_DEVICE_CDC_UART_STATE_RX_CARRIER;
            }

            /* Indicates to DCE if DTE is present or not */
            acmInfo->dtePresent = (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_DTE_PRESENCE) ? true : false;

            /* Initialize the serial state buffer */
            acmInfo->serialStateBuf[0] = NOTIF_REQUEST_TYPE;                /* bmRequestType */
            acmInfo->serialStateBuf[1] = USB_DEVICE_CDC_NOTIF_SERIAL_STATE; /* bNotification */
            acmInfo->serialStateBuf[2] = 0x00;                              /* wValue */
            acmInfo->serialStateBuf[3] = 0x00;
            acmInfo->serialStateBuf[4] = 0x00; /* wIndex */
            acmInfo->serialStateBuf[5] = 0x00;
            acmInfo->serialStateBuf[6] = UART_BITMAP_SIZE; /* wLength */
            acmInfo->serialStateBuf[7] = 0x00;
            /* Notifiy to host the line state */
            acmInfo->serialStateBuf[4] = acmReqParam->interfaceIndex;
            /* Lower byte of UART BITMAP */
            uartBitmap    = (uint8_t *)&acmInfo->serialStateBuf[NOTIF_PACKET_SIZE + UART_BITMAP_SIZE - 2];
            uartBitmap[0] = acmInfo->uartState & 0xFFu;
            uartBitmap[1] = (acmInfo->uartState >> 8) & 0xFFu;
            len           = (uint32_t)(NOTIF_PACKET_SIZE + UART_BITMAP_SIZE);
            if (0 == ((usb_device_cdc_acm_struct_t *)handle)->hasSentState)
            {
                error =
                    USB_DeviceCdcAcmSend(handle, USB_CDC_VCOM_CIC_INTERRUPT_IN_ENDPOINT, acmInfo->serialStateBuf, len);
                if (kStatus_USB_Success != error)
                {
                    usb_echo("kUSB_DeviceCdcEventSetControlLineState error!");
                }
                ((usb_device_cdc_acm_struct_t *)handle)->hasSentState = 1;
            }

            /* Update status */
            if (acmInfo->dteStatus & USB_DEVICE_CDC_CONTROL_SIG_BITMAP_CARRIER_ACTIVATION)
            {
                /*  To do: CARRIER_ACTIVATED */
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
        case kUSB_DeviceCdcEventSendBreak:
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
usb_status_t USB_DeviceCdcVcomSetConfigure(class_handle_t handle, uint8_t configure)
{
    if (USB_COMPOSITE_CONFIGURE_INDEX == configure)
    {
        /* Schedule buffer for receive */
        s_deviceComposite->cdcVcom.rx.busy = 1U;
        if (kStatus_USB_Success != USB_DeviceCdcAcmRecv(s_deviceComposite->cdcVcom.cdcAcmHandle,
                                                        USB_CDC_VCOM_DIC_BULK_OUT_ENDPOINT, s_currRecvBuf,
                                                        g_cdcVcomDicEndpoints[1].maxPacketSize))
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
        if (kStatus_USB_Success != USB_DeviceCdcAcmSend(s_deviceComposite->cdcVcom.cdcAcmHandle,
                                                        USB_CDC_VCOM_DIC_BULK_IN_ENDPOINT, buffer, length))
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

    if (length < g_cdcVcomDicEndpoints[1].maxPacketSize)
    {
        s_deviceComposite->cdcVcom.rx.busy = 0U;
        return kStatus_Fail;
    }

    if (NULL == buffer)
    {
        primeBuffer = s_currRecvBuf;
        primeLength = g_cdcVcomDicEndpoints[1].maxPacketSize;
    }
    else
    {
        primeBuffer = buffer;
        primeLength = length;
    }
    /* Schedule buffer for next receive event */

    if (kStatus_USB_Success != USB_DeviceCdcAcmRecv(s_deviceComposite->cdcVcom.cdcAcmHandle,
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
