/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_host_config.h"
#include "usb_host.h"
#include "fsl_device_registers.h"
#include "usb_host_hid.h"
#include "host_mouse_common.h"
#include "host_mouse_usb1.h"
#include "host_mouse_usb2.h"
#include "board.h"

#include "fsl_common.h"
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */
#include "app.h"

#if ((!USB_HOST_CONFIG_KHCI) && (!USB_HOST_CONFIG_EHCI) && (!USB_HOST_CONFIG_OHCI) && (!USB_HOST_CONFIG_IP3516HS))
#error Please enable USB_HOST_CONFIG_KHCI, USB_HOST_CONFIG_EHCI, USB_HOST_CONFIG_OHCI, or USB_HOST_CONFIG_IP3516HS in file usb_host_config.
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief application initialization.
 */
static void USB_HostApplicationInit(void);

/*!
 * @brief host 1 freertos task function.
 *
 * @param g_HostHandle   host handle
 */
static void USB_HostTask_1(void *param);

/*!
 * @brief host 2 freertos task function.
 *
 * @param g_HostHandle   host handle
 */
static void USB_HostTask_2(void *param);

/*!
 * @brief host mouse 1 freertos task function.
 *
 * @param param   the host mouse instance pointer.
 */
static void USB_HostApplicationTask_1(void *param);

/*!
 * @brief host mouse 2 freertos task function.
 *
 * @param param   the host mouse instance pointer.
 */
static void USB_HostApplicationTask_2(void *param);

extern void USB_HostClockInit(void);
extern void USB_HostIsrEnable(uint8_t controller_id);
extern void USB_HostTaskFn(void *param);
void BOARD_InitHardware(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief USB host mouse instance global variable */
extern usb_host_mouse_instance_t g_HostHidMouse_1;
extern usb_host_mouse_instance_t g_HostHidMouse_2;
usb_host_handle g_HostHandle_1;
usb_host_handle g_HostHandle_2;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief USB 1 isr function.
 */
usb_status_t USB_HostEvent_1(usb_device_handle deviceHandle,
                             usb_host_configuration_handle configurationHandle,
                             uint32_t eventCode)
{
    usb_status_t status = kStatus_USB_Success;

    switch (eventCode & 0x0000FFFFU)
    {
        case kUSB_HostEventAttach:
            status = USB_HostHidMouseEvent_1(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventNotSupported:
            usb_echo("device not supported.\r\n");
            break;

        case kUSB_HostEventEnumerationDone:
            status = USB_HostHidMouseEvent_1(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventDetach:
            status = USB_HostHidMouseEvent_1(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventEnumerationFail:
            usb_echo("enumeration failed\r\n");
            break;

        default:
            break;
    }
    return status;
}

/*!
 * @brief USB 2 isr function.
 */
usb_status_t USB_HostEvent_2(usb_device_handle deviceHandle,
                             usb_host_configuration_handle configurationHandle,
                             uint32_t eventCode)
{
    usb_status_t status = kStatus_USB_Success;

    switch (eventCode & 0x0000FFFFU)
    {
        case kUSB_HostEventAttach:
            status = USB_HostHidMouseEvent_2(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventNotSupported:
            usb_echo("device not supported.\r\n");
            break;

        case kUSB_HostEventEnumerationDone:
            status = USB_HostHidMouseEvent_2(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventDetach:
            status = USB_HostHidMouseEvent_2(deviceHandle, configurationHandle, eventCode);
            break;

        case kUSB_HostEventEnumerationFail:
            usb_echo("enumeration failed\r\n");
            break;

        default:
            break;
    }
    return status;
}

static void USB_HostApplicationInit(void)
{
    uint32_t usbHostVersion;
    usb_status_t status = kStatus_USB_Success;

    USB_HostClockInit();

#if ((defined FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT))
    SYSMPU_Enable(SYSMPU, 0);
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

    status = USB_HostInit(CONTROLLER_USB1_ID, &g_HostHandle_1, USB_HostEvent_1);
    if (status != kStatus_USB_Success)
    {
        usb_echo("host 1 init error\r\n");
        return;
    }
    USB_HostIsrEnable(CONTROLLER_USB1_ID);
    usb_echo("host 1 init done\r\n");

    status = USB_HostInit(CONTROLLER_USB2_ID, &g_HostHandle_2, USB_HostEvent_2);
    if (status != kStatus_USB_Success)
    {
        usb_echo("host 2 init error\r\n");
        return;
    }
    USB_HostIsrEnable(CONTROLLER_USB2_ID);
    usb_echo("host 2 init done\r\n");

    USB_HostGetVersion(&usbHostVersion);
    usb_echo("the host stack version is %d.%d.%d.\r\n", ((uint8_t)(usbHostVersion >> 16)),
             ((uint8_t)(usbHostVersion >> 8)), ((uint8_t)(usbHostVersion)));
}

static void USB_HostTask_1(void *param)
{
    while (1)
    {
        USB_HostTaskFn(param);
    }
}

static void USB_HostTask_2(void *param)
{
    while (1)
    {
        USB_HostTaskFn(param);
    }
}

static void USB_HostApplicationTask_1(void *param)
{
    while (1)
    {
        USB_HostHidMouseTask_1(param);
    }
}

static void USB_HostApplicationTask_2(void *param)
{
    while (1)
    {
        USB_HostHidMouseTask_2(param);
    }
}

int main(void)
{
    BOARD_InitHardware();

    USB_HostApplicationInit();

    if (xTaskCreate(USB_HostTask_1, "usb host 1 task", 2000L / sizeof(portSTACK_TYPE), g_HostHandle_1, 4, NULL) !=
        pdPASS)
    {
        usb_echo("create host 1 task error\r\n");
    }

    if (xTaskCreate(USB_HostTask_2, "usb host 2 task", 2000L / sizeof(portSTACK_TYPE), g_HostHandle_2, 4, NULL) !=
        pdPASS)
    {
        usb_echo("create host 2 task error\r\n");
    }
    if (xTaskCreate(USB_HostApplicationTask_1, "app task", 2000L / sizeof(portSTACK_TYPE), &g_HostHidMouse_1, 3,
                    NULL) != pdPASS)
    {
        usb_echo("create mouse 1 task error\r\n");
    }

    if (xTaskCreate(USB_HostApplicationTask_2, "app task", 2000L / sizeof(portSTACK_TYPE), &g_HostHidMouse_2, 3,
                    NULL) != pdPASS)
    {
        usb_echo("create mouse 2 task error\r\n");
    }

    vTaskStartScheduler();

    while (1)
    {
        ;
    }
}
