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

#include "usb_host_config.h"
#include "usb_host.h"
#include "usb_host_hid.h"
#include "host_mouse.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
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
usb_host_handle g_hostHandle;
usb_host_mouse_instance_t g_HostHidMouse;
IRQn_Type usbIrq;

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
static usb_status_t USB_HostEvent(usb_device_handle deviceHandle,
                                  usb_host_configuration_handle configurationHandle,
                                  uint32_t eventCode)
{
    usb_status_t status = kStatus_USB_Success;
    switch (eventCode)
    {
        case kUSB_HostEventAttach:
            status = USB_HostHidMouseEvent(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventNotSupported:
            usb_echo("device not supported.\r\n");
            break;

        case kUSB_HostEventEnumerationDone:
            status = USB_HostHidMouseEvent(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventDetach:
            status = USB_HostHidMouseEvent(deviceHandle, configurationHandle, eventCode);
            break;

        default:
            break;
    }
    return status;
}
static void USB_HostHidControlCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status)
{
    usb_host_mouse_instance_t *mouseInstance = (usb_host_mouse_instance_t *)param;

    if (mouseInstance->runWaitState == kRunWaitSetInterface) /* set interface finish */
    {
        mouseInstance->runState = kRunSetInterfaceDone;
    }
    else if (mouseInstance->runWaitState == kRunWaitSetIdle) /* hid set idle finish */
    {
        mouseInstance->runState = kRunSetIdleDone;
    }
    else if (mouseInstance->runWaitState == kRunWaitGetReportDescriptor) /* hid get report descriptor finish */
    {
        mouseInstance->runState = kRunGetReportDescriptorDone;
    }
    else if (mouseInstance->runWaitState == kRunWaitSetProtocol) /* hid set protocol finish */
    {
        mouseInstance->runState = kRunSetProtocolDone;
    }
    else
    {
    }
}

static void USB_HostHidInCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status)
{
    usb_host_mouse_instance_t *mouseInstance = (usb_host_mouse_instance_t *)param;

    if (mouseInstance->runWaitState == kRunWaitDataReceived)
    {
        if (mouseInstance->deviceState == kStatus_DEV_Attached)
        {
            if (status == kStatus_USB_Success)
            {
                mouseInstance->runState = kRunDataReceived; /* go to process data */
            }
            else
            {
                mouseInstance->runState = kRunPrimeDataReceive; /* go to prime next receiving */
            }
        }
    }
}

void USB_HostHidMouseTask(void *param)
{
    usb_status_t status = kStatus_USB_Success;
    usb_host_hid_descriptor_t *hidDescriptor;
    uint32_t mouseReportLength = 0;
    uint8_t *descriptor;
    uint32_t endPosition;
    usb_host_mouse_instance_t *mouseInstance = (usb_host_mouse_instance_t *)param;

    /* device state changes, process once for each state */
    if (mouseInstance->deviceState != mouseInstance->prevState)
    {
        mouseInstance->prevState = mouseInstance->deviceState;
        switch (mouseInstance->deviceState)
        {
            case kStatus_DEV_Idle:
                break;

            case kStatus_DEV_Attached: /* deivce is attached and numeration is done */
                mouseInstance->runState = kRunSetInterface;
                status = USB_HostHidInit(mouseInstance->deviceHandle,
                                         &mouseInstance->classHandle); /* hid class initialization */
                usb_echo("mouse attached\r\n");
                break;

            case kStatus_DEV_Detached: /* device is detached */

                break;

            default:
                break;
        }
    }

    /* run state */
    switch (mouseInstance->runState)
    {
        case kRunIdle:
            break;

        case kRunSetInterface: /* 1. set hid interface */
            mouseInstance->runWaitState = kRunWaitSetInterface;
            mouseInstance->runState = kRunIdle;
            if (USB_HostHidSetInterface(mouseInstance->classHandle, mouseInstance->interfaceHandle, 0,
                                        USB_HostHidControlCallback, mouseInstance) != kStatus_USB_Success)
            {
                usb_echo("set interface error\r\n");
            }
            break;

        case kRunSetInterfaceDone: /* 2. hid set idle */
            mouseInstance->maxPacketSize =
                USB_HostHidGetPacketsize(mouseInstance->classHandle, USB_ENDPOINT_INTERRUPT, USB_IN);

            /* first: set idle */
            mouseInstance->runWaitState = kRunWaitSetIdle;
            mouseInstance->runState = kRunIdle;
            if (USB_HostHidSetIdle(mouseInstance->classHandle, 0, 0, USB_HostHidControlCallback, mouseInstance) !=
                kStatus_USB_Success)
            {
                usb_echo("Error in USB_HostHidSetIdle: %x\r\n", status);
            }
            break;

        case kRunSetIdleDone: /* 3. hid get report descriptor */
            /* get report descriptor's length */
            hidDescriptor = NULL;
            descriptor = (uint8_t *)((usb_host_interface_t *)mouseInstance->interfaceHandle)->interfaceExtension;
            endPosition = (uint32_t)descriptor +
                          ((usb_host_interface_t *)mouseInstance->interfaceHandle)->interfaceExtensionLength;

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
                usb_host_hid_class_descriptor_t *Hid_classDesPtr;
                Hid_classDesPtr = (usb_host_hid_class_descriptor_t *)&(hidDescriptor->bHidDescriptorType);
                for (uint8_t index = 0; index < hidDescriptor->bNumDescriptors; ++index)
                {
                    Hid_classDesPtr += index;
                    if (Hid_classDesPtr->bHidDescriptorType == USB_DESCRIPTOR_TYPE_HID_REPORT)
                    {
                        mouseReportLength =
                            (uint16_t)USB_SHORT_FROM_LITTLE_ENDIAN_ADDRESS(Hid_classDesPtr->wDescriptorLength);
                        break;
                    }
                }
            }
            if (mouseReportLength > HID_BUFFER_SIZE)
            {
                usb_echo("hid buffer is too small\r\n");
                mouseInstance->runState = kRunIdle;
                return;
            }

            mouseInstance->runWaitState = kRunWaitGetReportDescriptor;
            mouseInstance->runState = kRunIdle;
            /* second: get report descriptor */
            USB_HostHidGetReportDescriptor(mouseInstance->classHandle, mouseInstance->mouseBuffer, mouseReportLength,
                                           USB_HostHidControlCallback, mouseInstance);
            break;

        case kRunGetReportDescriptorDone: /* 4. hid set protocol */
            mouseInstance->runWaitState = kRunWaitSetProtocol;
            mouseInstance->runState = kRunIdle;
            /* third: set protocol */
            if (USB_HostHidSetProtocol(mouseInstance->classHandle, USB_HOST_HID_REQUEST_PROTOCOL_REPORT,
                                       USB_HostHidControlCallback, mouseInstance) != kStatus_USB_Success)
            {
                usb_echo("Error in USB_HostHidSetProtocol: %x\r\n", status);
            }
            break;

        case kRunSetProtocolDone: /* 5. start to receive data */
            mouseInstance->runWaitState = kRunWaitDataReceived;
            mouseInstance->runState = kRunIdle;
            if (USB_HostHidRecv(mouseInstance->classHandle, mouseInstance->mouseBuffer, mouseInstance->maxPacketSize,
                                USB_HostHidInCallback, mouseInstance) != kStatus_USB_Success)
            {
                usb_echo("Error in USB_HostHidRecv: %x\r\n", status);
            }
            break;

        case kRunDataReceived: /* process received data and receive next data */
            USB_HostMouseProcessBuffer(mouseInstance->mouseBuffer);

            mouseInstance->runWaitState = kRunWaitDataReceived;
            mouseInstance->runState = kRunIdle;
            if (USB_HostHidRecv(mouseInstance->classHandle, mouseInstance->mouseBuffer, mouseInstance->maxPacketSize,
                                USB_HostHidInCallback, mouseInstance) != kStatus_USB_Success)
            {
                usb_echo("Error in USB_HostHidRecv: %x\r\n", status);
            }
            break;

        case kRunPrimeDataReceive: /* receive data */
            mouseInstance->runWaitState = kRunWaitDataReceived;
            mouseInstance->runState = kRunIdle;
            if (USB_HostHidRecv(mouseInstance->classHandle, mouseInstance->mouseBuffer, mouseInstance->maxPacketSize,
                                USB_HostHidInCallback, mouseInstance) != kStatus_USB_Success)
            {
                usb_echo("Error in USB_HostHidRecv: %x\r\n", status);
            }
            break;

        default:
            break;
    }
}

usb_status_t USB_HostHidMouseEvent(usb_device_handle deviceHandle,
                                   usb_host_configuration_handle configurationHandle,
                                   uint32_t eventCode)
{
    /* process the same and supported device's configuration handle */
    static usb_host_configuration_handle s_ConfigHandle = NULL;
    uint8_t id;
    usb_host_configuration_t *configuration;
    uint8_t interfaceIndex;
    usb_host_interface_t *interface;
    uint32_t infoValue;
    usb_status_t status = kStatus_USB_Success;

    switch (eventCode)
    {
        case kUSB_HostEventAttach:
            /* judge whether is configurationHandle supported */
            configuration = (usb_host_configuration_t *)configurationHandle;
            for (interfaceIndex = 0; interfaceIndex < configuration->interfaceCount; ++interfaceIndex)
            {
                interface = &configuration->interfaceList[interfaceIndex];
                id = interface->interfaceDesc->bInterfaceClass;
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
                    /* the interface is supported by the application */
                    g_HostHidMouse.deviceHandle = deviceHandle;
                    g_HostHidMouse.interfaceHandle = interface;
                    s_ConfigHandle = configurationHandle;
                    return kStatus_USB_Success;
                }
            }
            status = kStatus_USB_NotSupported;
            break;

        case kUSB_HostEventNotSupported:
            break;

        case kUSB_HostEventEnumerationDone:
            if (s_ConfigHandle == configurationHandle)
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
                    }
                }
            }
            break;

        case kUSB_HostEventDetach:
            if (s_ConfigHandle == configurationHandle)
            {
                /* the device is detached */
                s_ConfigHandle = NULL;
                if (g_HostHidMouse.deviceState != kStatus_DEV_Idle)
                {
                    g_HostHidMouse.deviceState = kStatus_DEV_Detached;
                }
            }

            USB_HostHidDeinit(g_HostHidMouse.deviceHandle,
                              g_HostHidMouse.classHandle); /* hid class de-initialization */
            g_HostHidMouse.classHandle = NULL;
            g_HostHidMouse.deviceState = kStatus_DEV_Idle;
            g_HostHidMouse.runState = kRunIdle;
            usb_echo("mouse detached\r\n");
            break;

        default:
            break;
    }
    return status;
}

/*!
 * @brief USB host ehci isr function.
 */
void USB_HostEhciIsr(void)
{
#if (USB_HOST_CONFIG_EHCI)
    USB_HostEhciIsrFunction(g_hostHandle);
#endif
}

/*!
 * @brief USB host khci isr function.
 */
void USB_HostKhciIsr(void)
{
#if (USB_HOST_CONFIG_KHCI)
    USB_HostKhciIsrFunction(g_hostHandle);
#endif
}
/*!
 * @brief host app initialization.
 */
void Host_AppInit(void)
{
    usb_status_t status = kStatus_USB_Success;
    uint8_t controller_id;
#if ((defined USB_HOST_CONFIG_KHCI) && (USB_HOST_CONFIG_KHCI))
    IRQn_Type usbFsIrqs[] = USB_IRQS;
    controller_id = kUSB_ControllerKhci0;
    usbIrq = usbFsIrqs[controller_id - kUSB_ControllerKhci0];
#endif /* USB_HOST_CONFIG_KHCI */
#if ((defined USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI))
    IRQn_Type usbHsIrqs[] = USBHS_IRQS;
    controller_id = kUSB_ControllerEhci0;
    usbIrq = usbHsIrqs[controller_id - kUSB_ControllerEhci0];
#endif /* USB_HOST_CONFIG_EHCI */
    status = USB_HostInit(controller_id, &g_hostHandle, USB_HostEvent);
    if (status != kStatus_USB_Success)
    {
        usb_echo("host init error\r\n");
        return;
    }
#if defined(__GIC_PRIO_BITS)
    GIC_SetPriority(usbIrq, 4);
#else
    NVIC_SetPriority(usbIrq, 4);
#endif
    EnableIRQ(usbIrq);

    usb_echo("host init done\r\n");
}

/*!
 * @brief host app deinitialization.
 */
void Host_AppDeinit(void)
{
    usb_status_t status = kStatus_USB_Success;

    DisableIRQ(usbIrq);
    status = USB_HostDeinit(g_hostHandle);
    if (status != kStatus_USB_Success)
    {
        usb_echo("host deinit error\r\n");
        return;
    }

    usb_echo("host deinit done\r\n");
}

/*!
 * @brief host app task function.
 */
void Host_AppTaskFunction(void)
{
#if (USB_HOST_CONFIG_EHCI)
    USB_HostEhciTaskFunction(g_hostHandle);
#endif
#if (USB_HOST_CONFIG_KHCI)
    USB_HostKhciTaskFunction(g_hostHandle);
#endif
    USB_HostHidMouseTask(&g_HostHidMouse);
}
