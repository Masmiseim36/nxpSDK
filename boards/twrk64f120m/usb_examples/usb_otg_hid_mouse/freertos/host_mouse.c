/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_host_config.h"
#include "usb_host.h"
#include "usb_host_hid.h"
#include "host_mouse.h"
#include "usb_otg.h"
#include "usb_otg_mouse.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief process hid data and print mouse action.
 *
 * @param buffer   hid data buffer.
 */
static void USB_HostMouseProcessBuffer(uint8_t *buffer);

/*!
 * @brief host mouse control transfer callback.
 *
 * This function is used as callback function for control transfer .
 *
 * @param param      the host mouse instance pointer.
 * @param data       data buffer pointer.
 * @param dataLength data length.
 * @status         transfer result status.
 */
static void USB_HostHidControlCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status);

/*!
 * @brief host mouse interrupt in transfer callback.
 *
 * This function is used as callback function when call USB_HostHidRecv .
 *
 * @param param      the host mouse instance pointer.
 * @param data       data buffer pointer.
 * @param dataLength data length.
 * @status           transfer result status.
 */
static void USB_HostHidInCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status);

/*******************************************************************************
 * Variables
 ******************************************************************************/

extern usb_otg_handle g_OtgHandle;
extern usb_otg_mouse_instance_t g_OtgMouseInstance;
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t s_MouseBuffer[HID_BUFFER_SIZE]; /*!< use to receive report descriptor and data */
usb_host_mouse_instance_t g_HostHidMouse;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*Host hid example doesn't support HID report descriptor analysis, this example assume that the received data are sent
 * by specific order. */
static void USB_HostMouseProcessBuffer(uint8_t *buffer)
{
    /* 1. Left key action */
    if (buffer[0] & 0x01)
    {
        usb_echo("Left Click ");
    }
    else
    {
        usb_echo("           ");
    }

    /* 2. Middle key action */
    if (buffer[0] & 0x04)
    {
        usb_echo("Middle Click ");
    }
    else
    {
        usb_echo("            ");
    }

    /* 3. Right key action */
    if (buffer[0] & 0x02)
    {
        usb_echo("Right Click ");
    }
    else
    {
        usb_echo("           ");
    }

    /* 4. Left/Right movement */
    if (buffer[1])
    {
        if (buffer[1] > 127)
        {
            usb_echo("Left  ");
        }
        else
        {
            usb_echo("Right ");
        }
    }
    else
    {
        usb_echo("      ");
    }

    /* 5. UP/Down movement */
    if (buffer[2])
    {
        if (buffer[2] > 127)
        {
            usb_echo("UP   ");
        }
        else
        {
            usb_echo("Down ");
        }
    }
    else
    {
        usb_echo("     ");
    }

    /* 6. Whell Down/Wheel UP action */
    if (buffer[3])
    {
        if (buffer[3] > 127)
        {
            usb_echo("Wheel Down");
        }
        else
        {
            usb_echo("Wheel UP  ");
        }
    }
    else
    {
        usb_echo("          ");
    }

    usb_echo("\r\n");
}

static void USB_HostHidControlCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status)
{
    usb_host_mouse_instance_t *mouseInstance = (usb_host_mouse_instance_t *)param;

    if (mouseInstance->runWaitState == kUSB_HostHidRunWaitSetInterface) /* set interface finish */
    {
        mouseInstance->runState = kUSB_HostHidRunSetInterfaceDone;
    }
    else if (mouseInstance->runWaitState == kUSB_HostHidRunWaitSetIdle) /* hid set idle finish */
    {
        mouseInstance->runState = kUSB_HostHidRunSetIdleDone;
    }
    else if (mouseInstance->runWaitState ==
             kUSB_HostHidRunWaitGetReportDescriptor) /* hid get report descriptor finish */
    {
        mouseInstance->runState = kUSB_HostHidRunGetReportDescriptorDone;
    }
    else if (mouseInstance->runWaitState == kUSB_HostHidRunWaitSetProtocol) /* hid set protocol finish */
    {
        mouseInstance->runState = kUSB_HostHidRunSetProtocolDone;
    }
    else
    {
    }
}

static void USB_HostHidInCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status)
{
    usb_host_mouse_instance_t *mouseInstance = (usb_host_mouse_instance_t *)param;

    if (mouseInstance->runWaitState == kUSB_HostHidRunWaitDataReceived)
    {
        if (mouseInstance->deviceState == kStatus_DEV_Attached)
        {
            if (status == kStatus_USB_Success)
            {
                mouseInstance->runState = kUSB_HostHidRunDataReceived; /* go to process data */
            }
            else
            {
                mouseInstance->runState = kUSB_HostHidRunPrimeDataReceive; /* go to prime next receiving */
            }
        }
    }
}

static usb_status_t USB_HostGetOtgDescriptor(usb_device_handle deviceHandle, usb_otg_descriptor_t **otgDescriptor)
{
    uint32_t endPos;
    usb_descriptor_union_t *unionDes;
    uint32_t configurationDesc;
    uint32_t configurationLength;

    USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetConfigurationDes, &configurationDesc);
    USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetConfigurationLength, &configurationLength);

    /* parse configuration descriptor */
    unionDes = (usb_descriptor_union_t *)configurationDesc;
    endPos   = (uint32_t)(configurationDesc + configurationLength);
    while ((uint32_t)unionDes < endPos)
    {
        if (unionDes->common.bDescriptorType != USB_DESCRIPTOR_TYPE_OTG)
        {
            unionDes = (usb_descriptor_union_t *)((uint32_t)unionDes + unionDes->common.bLength);
        }
        else
        {
            *otgDescriptor = (usb_otg_descriptor_t *)(&(unionDes->common.bLength));
            return kStatus_USB_Success;
        }
    }

    return kStatus_USB_Error;
}

static void USB_HostSetHNPCallback(void *param, struct _usb_host_transfer *transfer, usb_status_t status)
{
    usb_host_mouse_instance_t *mouseInstance = (usb_host_mouse_instance_t *)param;

    mouseInstance->HNPTransfer = NULL;
    USB_HostFreeTransfer(mouseInstance->hostHandle, transfer);

    if (status == kStatus_USB_Success)
    {
        g_OtgMouseInstance.aSetBHNPEnable = 1U;
        mouseInstance->runState           = kUSB_HostHidRunSetInterface;
        USB_OtgNotifyChange(g_OtgHandle, kOtg_StatusBHNPFeature, 1);
        usb_echo("set device HNP feature enable success\r\n");
    }
    else
    {
        mouseInstance->runState = kUSB_HostHidRunSetInterface;
    }
}

static usb_status_t USB_HostSetOtgHNPFeatureEnable(usb_host_mouse_instance_t *mouseInstance)
{
    usb_host_process_feature_param_t featureParam;
    usb_status_t status;

    /* malloc one transfer */
    if (USB_HostMallocTransfer(mouseInstance->hostHandle, &mouseInstance->HNPTransfer) != kStatus_USB_Success)
    {
        return kStatus_USB_Busy;
    }
    mouseInstance->HNPTransfer->callbackFn     = USB_HostSetHNPCallback;
    mouseInstance->HNPTransfer->callbackParam  = mouseInstance;
    mouseInstance->HNPTransfer->transferBuffer = NULL;
    mouseInstance->HNPTransfer->transferLength = 0;

    featureParam.requestType     = kRequestDevice;
    featureParam.featureSelector = USB_REQUEST_STANDARD_FEATURE_SELECTOR_B_HNP_ENABLE;
    status                       = USB_HostRequestControl(mouseInstance->deviceHandle, USB_REQUEST_STANDARD_SET_FEATURE,
                                    mouseInstance->HNPTransfer, &featureParam);
    if (status != kStatus_USB_Success)
    {
        USB_HostFreeTransfer(mouseInstance->hostHandle, mouseInstance->HNPTransfer);
    }

    return status;
}

static void USB_HostPollOtgStatusCallback(void *param, struct _usb_host_transfer *transfer, usb_status_t status)
{
    usb_host_mouse_instance_t *mouseInstance = (usb_host_mouse_instance_t *)param;
    uint8_t otgStatus                        = *(transfer->transferBuffer);

    mouseInstance->HNPTransfer = NULL;
    USB_HostFreeTransfer(mouseInstance->hostHandle, transfer);

    if (status == kStatus_USB_Success)
    {
        if (otgStatus & USB_OTG_STATUS_HOST_REQUEST_FLAG)
        {
            USB_OtgBusRequest(g_OtgHandle);
        }
    }
}

static usb_status_t USB_HostPollOtgDeviceHnp(usb_host_mouse_instance_t *mouseInstance)
{
    usb_host_get_status_param_t param;
    usb_status_t status;

    /* malloc one transfer */
    if (USB_HostMallocTransfer(mouseInstance->hostHandle, &mouseInstance->HNPTransfer) != kStatus_USB_Success)
    {
        return kStatus_USB_Busy;
    }
    mouseInstance->HNPTransfer->callbackFn     = USB_HostPollOtgStatusCallback;
    mouseInstance->HNPTransfer->callbackParam  = mouseInstance;
    mouseInstance->HNPTransfer->transferBuffer = &(mouseInstance->pollHNPValue);
    mouseInstance->HNPTransfer->transferLength = 1;

    param.requestType    = kRequestDevice;
    param.statusSelector = USB_REQUEST_STANDARD_GET_STATUS_OTG_STATUS_SELECTOR;
    param.statusBuffer   = mouseInstance->HNPTransfer->transferBuffer;
    status               = USB_HostRequestControl(mouseInstance->deviceHandle, USB_REQUEST_STANDARD_GET_STATUS,
                                    mouseInstance->HNPTransfer, &param);
    if (status != kStatus_USB_Success)
    {
        USB_HostFreeTransfer(mouseInstance->hostHandle, mouseInstance->HNPTransfer);
    }

    return status;
}

void USB_HostHidMouseTask(void)
{
    usb_host_hid_descriptor_t *hidDescriptor;
    uint32_t mouseReportLength = 0;
    uint8_t *descriptor;
    uint32_t endPosition;
    usb_otg_descriptor_t *otgDescriptor = NULL;

    xSemaphoreTake(g_HostHidMouse.hostSemaphore, portMAX_DELAY);
    /* device state changes, process once for each state */
    if (g_HostHidMouse.deviceState != g_HostHidMouse.prevState)
    {
        g_HostHidMouse.prevState = g_HostHidMouse.deviceState;
        switch (g_HostHidMouse.deviceState)
        {
            case kStatus_DEV_Idle:
                break;

            case kStatus_DEV_Attached: /* deivce is attached and numeration is done */
                if (g_OtgMouseInstance.otgStateMachine == kOtg_State_AHost)
                {
                    g_HostHidMouse.runState = kUSB_HostHidRunParseOtgDescriptor;
                }
                else
                {
                    g_HostHidMouse.runState = kUSB_HostHidRunSetInterface;
                }

                /* hid class initialization */
                if (USB_HostHidInit(g_HostHidMouse.deviceHandle, &g_HostHidMouse.classHandle) != kStatus_USB_Success)
                {
                    usb_echo("host hid class initialize fail\r\n");
                }
                else
                {
                    usb_echo("mouse attached\r\n");
                }
                break;

            case kStatus_DEV_Detached: /* device is detached */
                g_HostHidMouse.deviceState = kStatus_DEV_Idle;
                g_HostHidMouse.runState    = kUSB_HostHidRunIdle;
                USB_HostHidDeinit(g_HostHidMouse.deviceHandle,
                                  g_HostHidMouse.classHandle); /* hid class de-initialization */
                g_HostHidMouse.classHandle = NULL;
                usb_echo("mouse detached\r\n");
                break;

            default:
                break;
        }
    }

    if (g_HostHidMouse.pollHNPInterval < USB_HOST_OTG_POLL_HNP_INTERVAL)
    {
        g_HostHidMouse.pollHNPInterval++;
    }
    /* run state */
    switch (g_HostHidMouse.runState)
    {
        case kUSB_HostHidRunIdle:
            break;

        case kUSB_HostHidRunIdlePoll:
            if ((g_HostHidMouse.HNPTransfer == NULL) &&
                (g_HostHidMouse.pollHNPInterval >= USB_HOST_OTG_POLL_HNP_INTERVAL))
            {
                g_HostHidMouse.pollHNPInterval = 0U;
                USB_HostPollOtgDeviceHnp(&g_HostHidMouse);
            }
            else
            {
            }
            break;

        case kUSB_HostHidRunParseOtgDescriptor:
            g_HostHidMouse.runState = kUSB_HostHidRunSetInterface;
            USB_HostGetOtgDescriptor(g_HostHidMouse.deviceHandle, &otgDescriptor);
            if ((otgDescriptor != NULL) && (otgDescriptor->bmAttributes & USB_DESCRIPTOR_OTG_ATTRIBUTES_HNP_MASK))
            {
                g_HostHidMouse.runState     = kUSB_HostHidRunIdle;
                g_HostHidMouse.runWaitState = kUSB_HostHidRunWaitParseOtgDescriptor;
                if (USB_HostSetOtgHNPFeatureEnable(&g_HostHidMouse) != kStatus_USB_Success)
                {
                    g_HostHidMouse.runState = kUSB_HostHidRunSetInterface;
                }
            }
            break;

        case kUSB_HostHidRunSetInterface: /* 1. set hid interface */
            g_HostHidMouse.runWaitState = kUSB_HostHidRunWaitSetInterface;
            g_HostHidMouse.runState     = kUSB_HostHidRunIdle;
            if (USB_HostHidSetInterface(g_HostHidMouse.classHandle, g_HostHidMouse.interfaceHandle, 0,
                                        USB_HostHidControlCallback, &g_HostHidMouse) != kStatus_USB_Success)
            {
                usb_echo("set interface error\r\n");
            }
            break;

        case kUSB_HostHidRunSetInterfaceDone: /* 2. hid set idle */
            g_HostHidMouse.maxPacketSize =
                USB_HostHidGetPacketsize(g_HostHidMouse.classHandle, USB_ENDPOINT_INTERRUPT, USB_IN);

            /* first: set idle */
            g_HostHidMouse.runWaitState = kUSB_HostHidRunWaitSetIdle;
            g_HostHidMouse.runState     = kUSB_HostHidRunIdle;
            if (USB_HostHidSetIdle(g_HostHidMouse.classHandle, 0, 0, USB_HostHidControlCallback, &g_HostHidMouse) !=
                kStatus_USB_Success)
            {
                usb_echo("Error in USB_HostHidSetIdle\r\n");
            }
            break;

        case kUSB_HostHidRunSetIdleDone: /* 3. hid get report descriptor */
            /* get report descriptor's length */
            hidDescriptor = NULL;
            descriptor    = (uint8_t *)((usb_host_interface_t *)g_HostHidMouse.interfaceHandle)->interfaceExtension;
            endPosition   = (uint32_t)descriptor +
                          ((usb_host_interface_t *)g_HostHidMouse.interfaceHandle)->interfaceExtensionLength;

            while ((uint32_t)descriptor < endPosition)
            {
                if (*(descriptor + 1) == USB_DESCRIPTOR_TYPE_HID) /* descriptor type */
                {
                    hidDescriptor = (usb_host_hid_descriptor_t *)descriptor;
                    break;
                }
                else
                {
                    descriptor = (uint8_t *)((uint32_t)descriptor + (*descriptor)); /* next descriptor */
                }
            }

            if (hidDescriptor != NULL)
            {
                usb_host_hid_class_descriptor_t *hidClassDescriptor;
                hidClassDescriptor = (usb_host_hid_class_descriptor_t *)&(hidDescriptor->bHidDescriptorType);
                for (uint8_t index = 0; index < hidDescriptor->bNumDescriptors; ++index)
                {
                    hidClassDescriptor += index;
                    if (hidClassDescriptor->bHidDescriptorType == USB_DESCRIPTOR_TYPE_HID_REPORT)
                    {
                        mouseReportLength = USB_SHORT_FROM_LITTLE_ENDIAN_ADDRESS(hidClassDescriptor->wDescriptorLength);
                        break;
                    }
                }
            }
            if (mouseReportLength > HID_BUFFER_SIZE)
            {
                usb_echo("hid buffer is too small\r\n");
                g_HostHidMouse.runState = kUSB_HostHidRunSetProtocolDone;
                return;
            }

            if (mouseReportLength > 0) /* when report descriptor length is zero, go to next step */
            {
                g_HostHidMouse.runWaitState = kUSB_HostHidRunWaitGetReportDescriptor;
                g_HostHidMouse.runState     = kUSB_HostHidRunIdle;
                /* second: get report descriptor */
                USB_HostHidGetReportDescriptor(g_HostHidMouse.classHandle, g_HostHidMouse.mouseBuffer,
                                               mouseReportLength, USB_HostHidControlCallback, &g_HostHidMouse);
                break;
            }

        case kUSB_HostHidRunGetReportDescriptorDone: /* 4. hid set protocol */
            g_HostHidMouse.runWaitState = kUSB_HostHidRunWaitSetProtocol;
            g_HostHidMouse.runState     = kUSB_HostHidRunIdle;
            /* third: set protocol */
            if (USB_HostHidSetProtocol(g_HostHidMouse.classHandle, USB_HOST_HID_REQUEST_PROTOCOL_REPORT,
                                       USB_HostHidControlCallback, &g_HostHidMouse) != kStatus_USB_Success)
            {
                usb_echo("Error in USB_HostHidSetProtocol\r\n");
            }
            break;

        case kUSB_HostHidRunSetProtocolDone: /* 5. start to receive data and poll HNP */
            g_HostHidMouse.pollHNPInterval = 0;
            USB_HostPollOtgDeviceHnp(&g_HostHidMouse);

            g_HostHidMouse.runWaitState = kUSB_HostHidRunWaitDataReceived;
            g_HostHidMouse.runState     = kUSB_HostHidRunIdlePoll;
            if (USB_HostHidRecv(g_HostHidMouse.classHandle, g_HostHidMouse.mouseBuffer, g_HostHidMouse.maxPacketSize,
                                USB_HostHidInCallback, &g_HostHidMouse) != kStatus_USB_Success)
            {
                usb_echo("Error in USB_HostHidRecv\r\n");
            }
            break;

        case kUSB_HostHidRunDataReceived: /* process received data and receive next data */
            USB_HostMouseProcessBuffer(g_HostHidMouse.mouseBuffer);

            g_HostHidMouse.runWaitState = kUSB_HostHidRunWaitDataReceived;
            g_HostHidMouse.runState     = kUSB_HostHidRunIdlePoll;
            if (USB_HostHidRecv(g_HostHidMouse.classHandle, g_HostHidMouse.mouseBuffer, g_HostHidMouse.maxPacketSize,
                                USB_HostHidInCallback, &g_HostHidMouse) != kStatus_USB_Success)
            {
                usb_echo("Error in USB_HostHidRecv\r\n");
            }
            break;

        case kUSB_HostHidRunPrimeDataReceive: /* receive data */
            g_HostHidMouse.runWaitState = kUSB_HostHidRunWaitDataReceived;
            g_HostHidMouse.runState     = kUSB_HostHidRunIdlePoll;
            if (USB_HostHidRecv(g_HostHidMouse.classHandle, g_HostHidMouse.mouseBuffer, g_HostHidMouse.maxPacketSize,
                                USB_HostHidInCallback, &g_HostHidMouse) != kStatus_USB_Success)
            {
                usb_echo("Error in USB_HostHidRecv\r\n");
            }
            break;

        default:
            break;
    }
    xSemaphoreGive(g_HostHidMouse.hostSemaphore);
}

usb_status_t USB_HostHidMouseEvent(usb_device_handle deviceHandle,
                                   usb_host_configuration_handle configurationHandle,
                                   uint32_t eventCode)
{
    usb_status_t status = kStatus_USB_Success;
    uint8_t id;
    usb_host_configuration_t *configuration;
    uint8_t interfaceIndex;
    usb_host_interface_t *interface;
    uint32_t infoValue;

    switch (eventCode & 0x0000FFFFU)
    {
        case kUSB_HostEventAttach:
            /* judge whether is configurationHandle supported */
            configuration = (usb_host_configuration_t *)configurationHandle;
            for (interfaceIndex = 0; interfaceIndex < configuration->interfaceCount; ++interfaceIndex)
            {
                interface = &configuration->interfaceList[interfaceIndex];
                id        = interface->interfaceDesc->bInterfaceClass;
                if (id != USB_HOST_HID_CLASS_CODE)
                {
                    continue;
                }
                id = interface->interfaceDesc->bInterfaceSubClass;
                if ((id != USB_HOST_HID_SUBCLASS_CODE_NONE) && (id != USB_HOST_HID_SUBCLASS_CODE_BOOT))
                {
                    continue;
                }
                id = interface->interfaceDesc->bInterfaceProtocol;
                if (id != USB_HOST_HID_PROTOCOL_MOUSE)
                {
                    continue;
                }
                else
                {
                    if (g_HostHidMouse.deviceState == kStatus_DEV_Idle)
                    {
                        /* the interface is supported by the application */
                        g_HostHidMouse.mouseBuffer     = s_MouseBuffer;
                        g_HostHidMouse.deviceHandle    = deviceHandle;
                        g_HostHidMouse.interfaceHandle = interface;
                        g_HostHidMouse.configHandle    = configurationHandle;
                        return kStatus_USB_Success;
                    }
                    else
                    {
                        continue;
                    }
                }
            }
            status = kStatus_USB_NotSupported;
            break;

        case kUSB_HostEventNotSupported:
            break;

        case kUSB_HostEventEnumerationDone:
            if (g_HostHidMouse.configHandle == configurationHandle)
            {
                if ((g_HostHidMouse.deviceHandle != NULL) && (g_HostHidMouse.interfaceHandle != NULL))
                {
                    /* the device enumeration is done */
                    if (g_HostHidMouse.deviceState == kStatus_DEV_Idle)
                    {
                        g_HostHidMouse.deviceState = kStatus_DEV_Attached;

                        USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDevicePID, &infoValue);
                        usb_echo("hid mouse attached:pid=0x%x", infoValue);
                        USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDeviceVID, &infoValue);
                        usb_echo("vid=0x%x ", infoValue);
                        USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDeviceAddress, &infoValue);
                        usb_echo("address=%d\r\n", infoValue);
                    }
                    else
                    {
                        usb_echo("not idle mouse instance\r\n");
                        status = kStatus_USB_Error;
                    }
                }
            }
            break;

        case kUSB_HostEventDetach:
            if (g_HostHidMouse.configHandle == configurationHandle)
            {
                /* the device is detached */
                g_HostHidMouse.configHandle = NULL;
                if (g_HostHidMouse.deviceState != kStatus_DEV_Idle)
                {
                    g_HostHidMouse.deviceState = kStatus_DEV_Detached;
                }
            }
            break;

        case kUSB_HostEventEnumerationFail:
            usb_echo("enumeration failed\r\n");
            break;

        default:
            break;
    }
    return status;
}

static void USB_HostTask(void *param)
{
    while (1)
    {
        USB_HostKhciTaskFunction(param);
    }
}

usb_status_t USB_OtgHostMouseInit(usb_host_handle *hostHandle)
{
    usb_status_t status;

    g_HostHidMouse.deviceState     = kStatus_DEV_Idle;
    g_HostHidMouse.prevState       = kStatus_DEV_Idle;
    g_HostHidMouse.runState        = kUSB_HostHidRunIdle;
    g_HostHidMouse.runWaitState    = kUSB_HostHidRunIdle;
    g_HostHidMouse.configHandle    = NULL;
    g_HostHidMouse.deviceHandle    = NULL;
    g_HostHidMouse.classHandle     = NULL;
    g_HostHidMouse.interfaceHandle = NULL;
    g_HostHidMouse.HNPTransfer     = NULL;
    status                         = USB_HostInit(kUSB_ControllerKhci0, hostHandle, USB_HostHidMouseEvent);
    g_HostHidMouse.hostHandle      = *hostHandle;
    if (status == kStatus_USB_Success)
    {
        if (xTaskCreate(USB_HostTask, "host stack task", 2000L / sizeof(portSTACK_TYPE), *hostHandle, 5,
                        &(g_HostHidMouse.hostTaskHandler)) != pdPASS)
        {
            status = kStatus_USB_Error;
        }
        if (g_HostHidMouse.hostSemaphore == NULL)
        {
            if ((g_HostHidMouse.hostSemaphore = xSemaphoreCreateMutex()) == NULL)
            {
                status = kStatus_USB_Error;
            }
        }
        if (status != kStatus_USB_Success)
        {
            usb_echo("host init error\r\n");
        }
    }

    return status;
}

usb_status_t USB_OtgHostMouseDeinit(usb_host_handle hostHandle)
{
    usb_status_t status;
    uint32_t controlPipe;

    if (hostHandle == NULL)
    {
        return kStatus_USB_Success;
    }

    xSemaphoreTake(g_HostHidMouse.hostSemaphore, portMAX_DELAY);
    if ((g_HostHidMouse.deviceState == kStatus_DEV_Attached) || (g_HostHidMouse.prevState == kStatus_DEV_Attached))
    {
        g_HostHidMouse.deviceState = kStatus_DEV_Idle;
        g_HostHidMouse.prevState   = kStatus_DEV_Idle;
        g_HostHidMouse.runState    = kUSB_HostHidRunIdle;
        if (g_HostHidMouse.HNPTransfer != NULL)
        {
            USB_HostHelperGetPeripheralInformation(g_HostHidMouse.deviceHandle, kUSB_HostGetDeviceControlPipe,
                                                   &controlPipe);
            USB_HostCancelTransfer(g_HostHidMouse.hostHandle, (usb_host_pipe_handle)controlPipe,
                                   g_HostHidMouse.HNPTransfer);
        }
        status = USB_HostHidDeinit(g_HostHidMouse.deviceHandle, g_HostHidMouse.classHandle);
        if (kStatus_USB_Success != status)
        {
            usb_echo("failed in Deinit\r\n");
        }
        g_HostHidMouse.classHandle = NULL;
        USB_HostRemoveDevice(hostHandle, g_HostHidMouse.deviceHandle);
        usb_echo("mouse detached\r\n");
    }
    vTaskDelete(g_HostHidMouse.hostTaskHandler);
    status = USB_HostDeinit(hostHandle);
    xSemaphoreGive(g_HostHidMouse.hostSemaphore);

    return status;
}
