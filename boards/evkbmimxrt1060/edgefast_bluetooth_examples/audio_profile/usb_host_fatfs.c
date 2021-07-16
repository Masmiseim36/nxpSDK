/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_host_config.h"
#include "usb_host.h"
#include "usb_host_msd.h"
#include "usb_host_fatfs.h"
#include "ff.h"
#include "diskio.h"
#include "app_music_control.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void USB_HostTaskFn(void *param);
void USB_HostClockInit();
void USB_HostIsrEnable();

/*******************************************************************************
 * Variables
 ******************************************************************************/
usb_host_handle g_HostHandle;
static usb_host_msd_fatfs_instance_t g_MsdFatfsInstance;
static FATFS fatfs;

/*******************************************************************************
 * Code
 ******************************************************************************/

void USB_HostMsdControlSetInterfaceCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status)
{
    const TCHAR driver_number_buffer[4U] = {USBDISK + '0', ':', '/', 0x00u};

    if (f_mount(&fatfs, driver_number_buffer, 0U))
    {
        PRINTF("Fatfs mount failed.\r\n");
        return;
    }
    music_initialize();
    PRINTF("usb msd device is ready\r\n");
}

usb_status_t USB_HostMsdEvent(usb_device_handle deviceHandle,
                              usb_host_configuration_handle configurationHandle,
                              uint32_t eventCode)
{
    usb_status_t status = kStatus_USB_Success;
    usb_host_configuration_t *configuration;
    uint8_t interfaceIndex;
    usb_host_interface_t *interface;
    uint32_t infoValue;
    uint8_t id;

    switch (eventCode)
    {
        case kUSB_HostEventAttach:
            /* judge whether is configurationHandle supported */
            configuration = (usb_host_configuration_t *)configurationHandle;
            for (interfaceIndex = 0; interfaceIndex < configuration->interfaceCount; ++interfaceIndex)
            {
                interface = &configuration->interfaceList[interfaceIndex];
                id        = interface->interfaceDesc->bInterfaceClass;
                if (id != USB_HOST_MSD_CLASS_CODE)
                {
                    continue;
                }
                id = interface->interfaceDesc->bInterfaceSubClass;
                if ((id != USB_HOST_MSD_SUBCLASS_CODE_UFI) && (id != USB_HOST_MSD_SUBCLASS_CODE_SCSI))
                {
                    continue;
                }
                id = interface->interfaceDesc->bInterfaceProtocol;
                if (id != USB_HOST_MSD_PROTOCOL_BULK)
                {
                    continue;
                }
                else
                {
                    if (g_MsdFatfsInstance.deviceState == kStatus_DEV_Idle)
                    {
                        /* the interface is supported by the application */
                        g_MsdFatfsInstance.deviceHandle    = deviceHandle;
                        g_MsdFatfsInstance.interfaceHandle = interface;
                        g_MsdFatfsInstance.configHandle    = configurationHandle;
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
            if (g_MsdFatfsInstance.configHandle == configurationHandle)
            {
                if ((g_MsdFatfsInstance.deviceHandle != NULL) && (g_MsdFatfsInstance.interfaceHandle != NULL))
                {
                    /* the device enumeration is done */
                    if (g_MsdFatfsInstance.deviceState == kStatus_DEV_Idle)
                    {
                        g_MsdFatfsInstance.deviceState = kStatus_DEV_Attached;

                        USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDevicePID, &infoValue);
                        PRINTF("mass storage device attached:pid=0x%x", infoValue);
                        USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDeviceVID, &infoValue);
                        PRINTF("vid=0x%x ", infoValue);
                        USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDeviceAddress, &infoValue);
                        PRINTF("address=%d\r\n", infoValue);

                        /* Initialize usb host msd class */
                        status = USB_HostMsdInit(g_MsdFatfsInstance.deviceHandle, &g_MsdFatfsInstance.classHandle);
                        g_UsbFatfsClassHandle = g_MsdFatfsInstance.classHandle;
                        if (status != kStatus_USB_Success)
                        {
                            PRINTF("usb host msd init fail\r\n");
                            return kStatus_USB_Error;
                        }

                        /* set the device interface to class */
                        status = USB_HostMsdSetInterface(g_MsdFatfsInstance.classHandle, g_MsdFatfsInstance.interfaceHandle, 0,
                                                         USB_HostMsdControlSetInterfaceCallback, &g_MsdFatfsInstance);
                        if (status != kStatus_USB_Success)
                        {
                            PRINTF("set interface fail\r\n");
                        }
                    }
                    else
                    {
                        PRINTF("not idle msd instance\r\n");
                        status = kStatus_USB_Error;
                    }
                }
            }
            break;

        case kUSB_HostEventDetach:
            if (g_MsdFatfsInstance.configHandle == configurationHandle)
            {
                /* the device is detached */
                g_MsdFatfsInstance.configHandle = NULL;
                if (g_MsdFatfsInstance.deviceState != kStatus_DEV_Idle)
                {
                    g_UsbFatfsClassHandle = NULL;
                    g_MsdFatfsInstance.deviceState = kStatus_DEV_Idle;
                    /* msd class de-initialization */
                    USB_HostMsdDeinit(g_MsdFatfsInstance.deviceHandle, g_MsdFatfsInstance.classHandle);
                    g_MsdFatfsInstance.classHandle = NULL;
                    music_deinitialize();

                    usb_echo("mass storage device detached\r\n");
                }
            }
            break;

        default:
            break;
    }
    return status;
}

usb_status_t USB_HostEvent(usb_device_handle deviceHandle,
                                  usb_host_configuration_handle configurationHandle,
                                  uint32_t eventCode)
{
    usb_status_t status = kStatus_USB_Success;
    switch (eventCode & 0x0000FFFFU)
    {
        case kUSB_HostEventAttach:
            status = USB_HostMsdEvent(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventNotSupported:
            usb_echo("device not supported.\r\n");
            break;

        case kUSB_HostEventEnumerationDone:
            status = USB_HostMsdEvent(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventDetach:
            status = USB_HostMsdEvent(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventEnumerationFail:
            usb_echo("enumeration failed\r\n");
            break;

        default:
            break;
    }
    return status;
}

void USB_HostTask(void *param)
{
    while (1)
    {
        USB_HostTaskFn(param);
    }
}

void USB_HostApplicationInit(void)
{
    usb_status_t status = kStatus_USB_Success;

    USB_HostClockInit();

#if ((defined FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT))
    SYSMPU_Enable(SYSMPU, 0);
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

    status = USB_HostInit(CONTROLLER_ID, &g_HostHandle, USB_HostEvent);
    if (status != kStatus_USB_Success)
    {
        usb_echo("usb host init error\r\n");
        return;
    }
    USB_HostIsrEnable();

    usb_echo("usb host init done\r\n");
}

