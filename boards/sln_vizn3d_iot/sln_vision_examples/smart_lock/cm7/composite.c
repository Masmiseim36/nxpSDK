/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <FreeRTOS.h>
#include <task.h>

#include <stdio.h>
#include <stdlib.h>

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_cdc_acm.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

#include "composite.h"
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0)
#include "usb_phy.h"
#endif

#include "pin_mux.h"
#include "fwk_log.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitHardware(void);
/*!
 * @brief USB device callback function.
 *
 * This function handles the usb device specific requests.
 *
 * @param handle          The USB device handle.
 * @param event           The USB device event type.
 * @param param           The parameter of the device specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static bool s_USBinited            = false;
static uint8_t s_CompositeListSize = 0;
/* Composite device structure. */
static usb_device_composite_struct_t s_Composite;

/* USB device class information */
static usb_device_class_config_struct_t *s_CompositeClassConfig;

/* USB device class configuration information */
static usb_device_class_config_list_struct_t s_UsbDeviceCompositeConfigList;

/*******************************************************************************
 * Code
 ******************************************************************************/

void USB_OTG1_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(s_Composite.deviceHandle);
}

void USB_OTG2_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(s_Composite.deviceHandle);
}

void USB_DeviceClockInit(void)
{
    uint32_t usbClockFreq;
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };
    usbClockFreq = 24000000;

    if (CONTROLLER_ID == kUSB_ControllerEhci0)
    {
        CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, usbClockFreq);
        CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, usbClockFreq);
    }
    else
    {
        CLOCK_EnableUsbhs1PhyPllClock(kCLOCK_Usbphy480M, usbClockFreq);
        CLOCK_EnableUsbhs1Clock(kCLOCK_Usb480M, usbClockFreq);
    }

    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
}

void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber                  = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ((IRQn_Type)irqNumber);
}

void USB_DeviceIsrDisable(void)
{
    uint8_t irqNumber;

    uint8_t usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber                  = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];

    DisableIRQ((IRQn_Type)irqNumber);
}

/*!
 * @brief USB device callback function.
 *
 * This function handles the usb device specific requests.
 *
 * @param handle          The USB device handle.
 * @param event           The USB device event type.
 * @param param           The parameter of the device specific request.
 *
 * @return A USB error code or kStatus_USB_Success.
 */
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Error;
    uint16_t *temp16   = (uint16_t *)param;
    uint8_t *temp8     = (uint8_t *)param;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
        {
            s_Composite.attach               = 0;
            s_Composite.currentConfiguration = 0U;
            error                            = kStatus_USB_Success;
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
            /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
            if (kStatus_USB_Success == USB_DeviceClassGetSpeed(CONTROLLER_ID, &s_Composite.speed))
            {
                USB_DeviceSetSpeed(handle, s_Composite.speed);
            }

#endif
        }
        break;
        case kUSB_DeviceEventSetConfiguration:
            if (0U == (*temp8))
            {
                s_Composite.attach               = 0U;
                s_Composite.currentConfiguration = 0U;
            }
            else if (USB_COMPOSITE_CONFIGURE_INDEX == (*temp8))
            {
                s_Composite.attach = 1;
                error              = kStatus_USB_Success;
            }
            else
            {
                error = kStatus_USB_InvalidRequest;
            }
            break;

        case kUSB_DeviceEventSetInterface:
            if (s_Composite.attach)
            {
                uint8_t interface        = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                uint8_t alternateSetting = (uint8_t)(*temp16 & 0x00FFU);
                if (interface < USB_INTERFACE_COUNT &&
                    s_Composite.currentInterfaceAlternateSetting[interface] != alternateSetting)
                {
                    s_Composite.currentInterfaceAlternateSetting[interface] = alternateSetting;
                    error                                                   = kStatus_USB_Success;
                }
            }
            break;
        case kUSB_DeviceEventGetConfiguration:
            if (param)
            {
                *temp8 = s_Composite.currentConfiguration;
                error  = kStatus_USB_Success;
            }
            break;
        case kUSB_DeviceEventGetInterface:
            if (param)
            {
                uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                if (interface < USB_INTERFACE_COUNT)
                {
                    *temp16 = (*temp16 & 0xFF00U) | s_Composite.currentInterfaceAlternateSetting[interface];
                    error   = kStatus_USB_Success;
                }
                else
                {
                    error = kStatus_USB_InvalidRequest;
                }
            }
            break;
        case kUSB_DeviceEventGetDeviceDescriptor:
            if (param)
            {
                error = USB_DeviceGetDeviceDescriptor(handle, (usb_device_get_device_descriptor_struct_t *)param);
            }
            break;
        case kUSB_DeviceEventGetConfigurationDescriptor:
            if (param)
            {
                error = USB_DeviceGetConfigurationDescriptor(handle,
                                                             (usb_device_get_configuration_descriptor_struct_t *)param);
            }
            break;
#if (defined(USB_DEVICE_CONFIG_CV_TEST) && (USB_DEVICE_CONFIG_CV_TEST > 0U))
        case kUSB_DeviceEventGetDeviceQualifierDescriptor:
            if (param)
            {
                /* Get device descriptor request */
                error = USB_DeviceGetDeviceQualifierDescriptor(
                    handle, (usb_device_get_device_qualifier_descriptor_struct_t *)param);
            }
            break;
#endif
        case kUSB_DeviceEventGetStringDescriptor:
            if (param)
            {
                error = USB_DeviceGetStringDescriptor(handle, (usb_device_get_string_descriptor_struct_t *)param);
            }
            break;
        default:
            break;
    }

    if (error == kStatus_USB_Success)
    {
        for (uint8_t i = 0; i < sizeof(s_Composite.deviceCallback) / sizeof(s_Composite.deviceCallback[0]); i++)
        {
            if (s_Composite.deviceCallback[i] != NULL)
            {
                error = s_Composite.deviceCallback[i](handle, event, param);
            }
        }
    }
    return error;
}

/*!
 * @brief Application initialization function.
 *
 * This function initializes the application.
 *
 * @return None.
 */
int USB_DeviceApplicationInit(void)
{
    USB_DeviceClockInit();
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
    SYSMPU_Enable(SYSMPU, 0);
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

    if (s_USBinited == false)
    {
        s_USBinited = true;
    }
    else
    {
        return 0;
    }

    if (s_CompositeClassConfig == NULL)
    {
        LOGE("[USB] No device class registered");
        return -1;
    }

    s_UsbDeviceCompositeConfigList.config         = s_CompositeClassConfig;
    s_UsbDeviceCompositeConfigList.deviceCallback = USB_DeviceCallback;
    s_UsbDeviceCompositeConfigList.count          = s_CompositeListSize;
    s_Composite.speed                             = USB_SPEED_FULL;
    s_Composite.attach                            = 0;
    s_Composite.deviceHandle                      = NULL;

    if (kStatus_USB_Success !=
        USB_DeviceClassInit(CONTROLLER_ID, &s_UsbDeviceCompositeConfigList, &s_Composite.deviceHandle))
    {
        LOGE("USB device composite init failed");
        return -1;
    }
    else
    {
        LOGD("USB device composite ");
        /*Init classhandle in cdc instance*/

        for (int i = 0; i < s_CompositeListSize; i++)
        {
            s_Composite.deviceCallback[i](s_Composite.deviceHandle, kUSB_DeviceClassInit,
                                          s_UsbDeviceCompositeConfigList.config[i].classHandle);
        }
    }

    return 0;
}

void USB_CompositeClassRegister(usb_device_callback_t deviceCallback,
                                usb_device_class_callback_t classCallback,
                                usb_device_class_struct_t *classInfomation)
{
    usb_device_class_config_struct_t *newCompositeClassConfig;

    newCompositeClassConfig = (usb_device_class_config_struct_t *)pvPortMalloc(
        sizeof(usb_device_class_config_struct_t) * (s_CompositeListSize + 1));
    if (newCompositeClassConfig != NULL)
    {
        if (s_CompositeClassConfig != NULL)
        {
            memcpy(newCompositeClassConfig, s_CompositeClassConfig,
                   sizeof(usb_device_class_config_struct_t) * s_CompositeListSize);
            vPortFree(s_CompositeClassConfig);
        }
        s_Composite.deviceCallback[s_CompositeListSize]              = deviceCallback;
        newCompositeClassConfig[s_CompositeListSize].classCallback   = classCallback;
        newCompositeClassConfig[s_CompositeListSize].classHandle     = NULL;
        newCompositeClassConfig[s_CompositeListSize].classInfomation = classInfomation;
        s_CompositeListSize++;
        s_CompositeClassConfig = newCompositeClassConfig;
    }
    else
    {
        LOGE("USB device composite error register class ");
    }
}
