/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_hid.h"
#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"
#include "mouse.h"

#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

#include <stdio.h>
#include <stdlib.h>
#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
#include "fsl_mpu.h"
#endif /* FSL_FEATURE_SOC_MPU_COUNT */
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
#include "usb_phy.h"
#endif

#include "pin_mux.h"
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitHardware(void);

static usb_status_t USB_DeviceHidMouseAction(void);
static usb_status_t USB_DeviceHidMouseCallback(class_handle_t handle, uint32_t event, void *param);
static usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);
static void USB_DeviceApplicationInit(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

usb_hid_mouse_struct_t g_UsbDeviceHidMouse;

extern usb_device_class_struct_t g_UsbDeviceHidMouseConfig;

/* Set class configurations */
usb_device_class_config_struct_t g_UsbDeviceHidConfig[1] = {{
    USB_DeviceHidMouseCallback, /* HID mouse class callback pointer */
    (class_handle_t)NULL,       /* The HID class handle, This field is set by USB_DeviceClassInit */
    &g_UsbDeviceHidMouseConfig, /* The HID mouse configuration, including class code, subcode, and protocol, class type,
                           transfer type, endpoint address, max packet size, etc.*/
}};

/* Set class configuration list */
usb_device_class_config_list_struct_t g_UsbDeviceHidConfigList = {
    g_UsbDeviceHidConfig, /* Class configurations */
    USB_DeviceCallback,   /* Device callback pointer */
    1U,                   /* Class count */
};

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Update mouse pointer location. Draw a rectangular rotation*/
static usb_status_t USB_DeviceHidMouseAction(void)
{
    static int8_t x = 0U;
    static int8_t y = 0U;
    enum
    {
        RIGHT,
        DOWN,
        LEFT,
        UP
    };
    static uint8_t dir = RIGHT;

    switch (dir)
    {
        case RIGHT:
            /* Move right. Increase X value. */
            g_UsbDeviceHidMouse.buffer[1] = 2U;
            g_UsbDeviceHidMouse.buffer[2] = 0U;
            x++;
            if (x > 99U)
            {
                dir++;
            }
            break;
        case DOWN:
            /* Move down. Increase Y value. */
            g_UsbDeviceHidMouse.buffer[1] = 0U;
            g_UsbDeviceHidMouse.buffer[2] = 2U;
            y++;
            if (y > 99U)
            {
                dir++;
            }
            break;
        case LEFT:
            /* Move left. Discrease X value. */
            g_UsbDeviceHidMouse.buffer[1] = (uint8_t)(-2);
            g_UsbDeviceHidMouse.buffer[2] = 0U;
            x--;
            if (x < 2U)
            {
                dir++;
            }
            break;
        case UP:
            /* Move up. Discrease Y value. */
            g_UsbDeviceHidMouse.buffer[1] = 0U;
            g_UsbDeviceHidMouse.buffer[2] = (uint8_t)(-2);
            y--;
            if (y < 2U)
            {
                dir = RIGHT;
            }
            break;
        default:
            break;
    }
    /* Send mouse report to the host */
    return USB_DeviceHidSend(g_UsbDeviceHidMouse.hidHandle, USB_HID_MOUSE_ENDPOINT_IN, g_UsbDeviceHidMouse.buffer,
                             USB_HID_MOUSE_REPORT_LENGTH);
}

/* The hid class callback */
static usb_status_t USB_DeviceHidMouseCallback(class_handle_t handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Error;
    usb_device_endpoint_callback_message_struct_t *message = (usb_device_endpoint_callback_message_struct_t *)param;

    switch (event)
    {
        case kUSB_DeviceHidEventSendResponse:
            /* Resport sent */
            if (g_UsbDeviceHidMouse.attach)
            {
                if ((NULL != message) && (message->length == USB_UNINITIALIZED_VAL_32))
                {
                    return error;
                }
                error = USB_DeviceHidMouseAction();
            }
            break;
        case kUSB_DeviceHidEventGetReport:
        case kUSB_DeviceHidEventSetReport:
        case kUSB_DeviceHidEventRequestReportBuffer:
            error = kStatus_USB_InvalidRequest;
            break;
        case kUSB_DeviceHidEventGetIdle:
        case kUSB_DeviceHidEventGetProtocol:
        case kUSB_DeviceHidEventSetIdle:
        case kUSB_DeviceHidEventSetProtocol:
            break;
        default:
            break;
    }

    return error;
}

/* The device callback */
static usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Error;
    uint16_t *temp16 = (uint16_t *)param;
    uint8_t *temp8 = (uint8_t *)param;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
        {
            /* USB bus reset signal detected */
            g_UsbDeviceHidMouse.attach = 0U;
            error = kStatus_USB_Success;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
            /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
            if (kStatus_USB_Success == USB_DeviceClassGetSpeed(CONTROLLER_ID, &g_UsbDeviceHidMouse.speed))
            {
                USB_DeviceSetSpeed(handle, g_UsbDeviceHidMouse.speed);
            }
#endif
        }
        break;
#if (defined(USB_DEVICE_CONFIG_DETACH_ENABLE) && (USB_DEVICE_CONFIG_DETACH_ENABLE > 0U))
        case kUSB_DeviceEventAttach:
        {
            usb_echo("USB device attached.\r\n");
            USB_DeviceRun(g_UsbDeviceHidMouse.deviceHandle);
        }
        break;
        case kUSB_DeviceEventDetach:
        {
            usb_echo("USB device detached.\r\n");
            g_UsbDeviceHidMouse.attach = 0;
            USB_DeviceStop(g_UsbDeviceHidMouse.deviceHandle);
        }
        break;
#endif
        case kUSB_DeviceEventSetConfiguration:
            if (param)
            {
                /* Set device configuration request */
                g_UsbDeviceHidMouse.attach = 1U;
                g_UsbDeviceHidMouse.currentConfiguration = *temp8;
                if (USB_HID_MOUSE_CONFIGURE_INDEX == (*temp8))
                {
                    error = USB_DeviceHidMouseAction();
                }
            }
            break;
        case kUSB_DeviceEventSetInterface:
            if (g_UsbDeviceHidMouse.attach)
            {
                /* Set device interface request */
                uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                uint8_t alternateSetting = (uint8_t)(*temp16 & 0x00FFU);
                if (interface < USB_HID_MOUSE_INTERFACE_COUNT)
                {
                    g_UsbDeviceHidMouse.currentInterfaceAlternateSetting[interface] = alternateSetting;
                    if (alternateSetting == 0U)
                    {
                        error = USB_DeviceHidMouseAction();
                    }
                }
            }
            break;
        case kUSB_DeviceEventGetConfiguration:
            if (param)
            {
                /* Get current configuration request */
                *temp8 = g_UsbDeviceHidMouse.currentConfiguration;
                error = kStatus_USB_Success;
            }
            break;
        case kUSB_DeviceEventGetInterface:
            if (param)
            {
                /* Get current alternate setting of the interface request */
                uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                if (interface < USB_HID_MOUSE_INTERFACE_COUNT)
                {
                    *temp16 = (*temp16 & 0xFF00U) | g_UsbDeviceHidMouse.currentInterfaceAlternateSetting[interface];
                    error = kStatus_USB_Success;
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
                /* Get device descriptor request */
                error = USB_DeviceGetDeviceDescriptor(handle, (usb_device_get_device_descriptor_struct_t *)param);
            }
            break;
        case kUSB_DeviceEventGetConfigurationDescriptor:
            if (param)
            {
                /* Get device configuration descriptor request */
                error = USB_DeviceGetConfigurationDescriptor(handle,
                                                             (usb_device_get_configuration_descriptor_struct_t *)param);
            }
            break;
        case kUSB_DeviceEventGetStringDescriptor:
            if (param)
            {
                /* Get device string descriptor request */
                error = USB_DeviceGetStringDescriptor(handle, (usb_device_get_string_descriptor_struct_t *)param);
            }
            break;
        case kUSB_DeviceEventGetHidDescriptor:
            if (param)
            {
                /* Get hid descriptor request */
                error = USB_DeviceGetHidDescriptor(handle, (usb_device_get_hid_descriptor_struct_t *)param);
            }
            break;
        case kUSB_DeviceEventGetHidReportDescriptor:
            if (param)
            {
                /* Get hid report descriptor request */
                error =
                    USB_DeviceGetHidReportDescriptor(handle, (usb_device_get_hid_report_descriptor_struct_t *)param);
            }
            break;
        case kUSB_DeviceEventGetHidPhysicalDescriptor:
            if (param)
            {
                /* Get hid physical descriptor request */
                error = USB_DeviceGetHidPhysicalDescriptor(handle,
                                                           (usb_device_get_hid_physical_descriptor_struct_t *)param);
            }
            break;
        default:
            break;
    }

    return error;
}

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
void USBHS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_UsbDeviceHidMouse.deviceHandle);
}
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
void USB0_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(g_UsbDeviceHidMouse.deviceHandle);
}
#endif

static void USB_DeviceApplicationInit(void)
{
    uint8_t irqNumber;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    uint8_t usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];

    CLOCK_EnableUsbhs0Clock(kCLOCK_UsbSrcPll0, CLOCK_GetFreq(kCLOCK_PllFllSelClk));
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    uint8_t usbDeviceKhciIrq[] = USB_IRQS;
    irqNumber = usbDeviceKhciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];

    SystemCoreClockUpdate();

#if ((defined FSL_FEATURE_USB_KHCI_IRC48M_MODULE_CLOCK_ENABLED) && (FSL_FEATURE_USB_KHCI_IRC48M_MODULE_CLOCK_ENABLED))
    CLOCK_EnableUsbfs0Clock(kCLOCK_UsbSrcIrc48M, 48000000U);
#else
    CLOCK_EnableUsbfs0Clock(kCLOCK_UsbSrcPll0, CLOCK_GetFreq(kCLOCK_PllFllSelClk));
#endif /* FSL_FEATURE_USB_KHCI_IRC48M_MODULE_CLOCK_ENABLED */
#endif
#if (defined(FSL_FEATURE_SOC_MPU_COUNT) && (FSL_FEATURE_SOC_MPU_COUNT > 0U))
    MPU_Enable(MPU, 0);
#endif /* FSL_FEATURE_SOC_MPU_COUNT */

/*
 * If the SOC has USB KHCI dedicated RAM, the RAM memory needs to be clear after
 * the KHCI clock is enabled. When the demo uses USB EHCI IP, the USB KHCI dedicated
 * RAM can not be used and the memory can't be accessed.
 */
#if (defined(FSL_FEATURE_USB_KHCI_USB_RAM) && (FSL_FEATURE_USB_KHCI_USB_RAM > 0U))
#if (defined(FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS) && (FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS > 0U))
    for (int i = 0; i < FSL_FEATURE_USB_KHCI_USB_RAM; i++)
    {
        ((uint8_t *)FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif /* FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS */
#endif /* FSL_FEATURE_USB_KHCI_USB_RAM */

    /* Set HID mouse to default state */
    g_UsbDeviceHidMouse.speed = USB_SPEED_FULL;
    g_UsbDeviceHidMouse.attach = 0U;
    g_UsbDeviceHidMouse.hidHandle = (class_handle_t)NULL;
    g_UsbDeviceHidMouse.deviceHandle = NULL;

    /* Initialize the usb stack and class drivers */
    if (kStatus_USB_Success !=
        USB_DeviceClassInit(CONTROLLER_ID, &g_UsbDeviceHidConfigList, &g_UsbDeviceHidMouse.deviceHandle))
    {
        usb_echo("USB device mouse failed\r\n");
        return;
    }
    else
    {
        usb_echo("USB device HID mouse demo\r\n");
        /* Get the HID mouse class handle */
        g_UsbDeviceHidMouse.hidHandle = g_UsbDeviceHidConfigList.config->classHandle;
    }

    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ((IRQn_Type)irqNumber);

    /* Start USB device HID mouse */
    USB_DeviceRun(g_UsbDeviceHidMouse.deviceHandle);
}

#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTask(void *handle)
{
    while (1U)
    {
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
        USB_DeviceEhciTaskFunction(handle);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
        USB_DeviceKhciTaskFunction(handle);
#endif
    }
}
#endif

void APP_task(void *handle)
{
    USB_DeviceApplicationInit();

#if USB_DEVICE_CONFIG_USE_TASK
    if (g_UsbDeviceHidMouse.deviceHandle)
    {
        if (xTaskCreate(USB_DeviceTask,                       /* pointer to the task */
                        "usb device task",                    /* task name for kernel awareness debugging */
                        5000L / sizeof(portSTACK_TYPE),       /* task stack size */
                        g_UsbDeviceHidMouse.deviceHandle,     /* optional task startup argument */
                        5U,                                   /* initial priority */
                        &g_UsbDeviceHidMouse.deviceTaskHandle /* optional task handle to create */
                        ) != pdPASS)
        {
            usb_echo("usb device task create failed!\r\n");
            return;
        }
    }
#endif

    while (1U)
    {
    }
}

#if defined(__CC_ARM) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    if (xTaskCreate(APP_task,                                  /* pointer to the task */
                    "app task",                                /* task name for kernel awareness debugging */
                    5000L / sizeof(portSTACK_TYPE),            /* task stack size */
                    &g_UsbDeviceHidMouse,                      /* optional task startup argument */
                    4U,                                        /* initial priority */
                    &g_UsbDeviceHidMouse.applicationTaskHandle /* optional task handle to create */
                    ) != pdPASS)
    {
        usb_echo("app task create failed!\r\n");
#if (defined(__CC_ARM) || defined(__GNUC__))
        return 1U;
#else
        return;
#endif
    }

    vTaskStartScheduler();

#if (defined(__CC_ARM) || defined(__GNUC__))
    return 1U;
#endif
}
