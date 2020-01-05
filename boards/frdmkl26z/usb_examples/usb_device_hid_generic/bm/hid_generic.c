/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
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

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_hid.h"

#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

#include "hid_generic.h"

#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

#include <stdio.h>
#include <stdlib.h>
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
#include "usb_phy.h"
#endif

#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* USB clock source and frequency*/
#define USB_FS_CLK_SRC kCLOCK_UsbSrcPll0
#define USB_FS_CLK_FREQ CLOCK_GetFreq(kCLOCK_PllFllSelClk)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitHardware(void);

static usb_status_t USB_DeviceHidGenericCallback(class_handle_t handle, uint32_t event, void *param);
static usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);
static void USB_DeviceApplicationInit(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

USB_DATA_ALIGNMENT static uint32_t s_GenericBuffer0[USB_HID_GENERIC_IN_BUFFER_LENGTH >> 2];
USB_DATA_ALIGNMENT static uint32_t s_GenericBuffer1[USB_HID_GENERIC_IN_BUFFER_LENGTH >> 2];
usb_hid_generic_struct_t g_UsbDeviceHidGeneric;

extern usb_device_class_struct_t g_UsbDeviceHidGenericConfig;

/* Set class configurations */
usb_device_class_config_struct_t g_UsbDeviceHidConfig[1] = {{
    USB_DeviceHidGenericCallback, /* HID generic class callback pointer */
    (class_handle_t)NULL,         /* The HID class handle, This field is set by USB_DeviceClassInit */
    &g_UsbDeviceHidGenericConfig, /* The HID mouse configuration, including class code, subcode, and protocol, class
                             type,
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

/* The hid class callback */
static usb_status_t USB_DeviceHidGenericCallback(class_handle_t handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_Error;

    switch (event)
    {
        case kUSB_DeviceHidEventSendResponse:
            break;
        case kUSB_DeviceHidEventRecvResponse:
            if (g_UsbDeviceHidGeneric.attach)
            {
                USB_DeviceHidSend(g_UsbDeviceHidGeneric.hidHandle, USB_HID_GENERIC_ENDPOINT_IN,
                                  (uint8_t *)&g_UsbDeviceHidGeneric.buffer[g_UsbDeviceHidGeneric.bufferIndex][0],
                                  USB_HID_GENERIC_OUT_BUFFER_LENGTH);
                g_UsbDeviceHidGeneric.bufferIndex ^= 1U;
                return USB_DeviceHidRecv(g_UsbDeviceHidGeneric.hidHandle, USB_HID_GENERIC_ENDPOINT_OUT,
                                         (uint8_t *)&g_UsbDeviceHidGeneric.buffer[g_UsbDeviceHidGeneric.bufferIndex][0],
                                         USB_HID_GENERIC_OUT_BUFFER_LENGTH);
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
    usb_status_t error = kStatus_USB_Success;
    uint8_t *temp8 = (uint8_t *)param;
    uint16_t *temp16 = (uint16_t *)param;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
        {
            /* USB bus reset signal detected */
            g_UsbDeviceHidGeneric.attach = 0U;
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
            /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
            if (kStatus_USB_Success == USB_DeviceClassGetSpeed(CONTROLLER_ID, &g_UsbDeviceHidGeneric.speed))
            {
                USB_DeviceSetSpeed(handle, g_UsbDeviceHidGeneric.speed);
            }
#endif
        }
        break;
        case kUSB_DeviceEventSetConfiguration:
            if (param)
            {
                /* Set device configuration request */
                g_UsbDeviceHidGeneric.attach = 1U;
                g_UsbDeviceHidGeneric.currentConfiguration = *temp8;
                if (USB_HID_GENERIC_CONFIGURE_INDEX == (*temp8))
                {
                    error = USB_DeviceHidRecv(
                        g_UsbDeviceHidGeneric.hidHandle, USB_HID_GENERIC_ENDPOINT_OUT,
                        (uint8_t *)&g_UsbDeviceHidGeneric.buffer[g_UsbDeviceHidGeneric.bufferIndex][0],
                        USB_HID_GENERIC_OUT_BUFFER_LENGTH);
                }
            }
            break;
        case kUSB_DeviceEventSetInterface:
            if (g_UsbDeviceHidGeneric.attach)
            {
                /* Set device interface request */
                uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                uint8_t alternateSetting = (uint8_t)(*temp16 & 0x00FFU);
                if (interface < USB_HID_GENERIC_INTERFACE_COUNT)
                {
                    g_UsbDeviceHidGeneric.currentInterfaceAlternateSetting[interface] = alternateSetting;
                    if (alternateSetting == 0U)
                    {
                        error = USB_DeviceHidRecv(
                            g_UsbDeviceHidGeneric.hidHandle, USB_HID_GENERIC_ENDPOINT_OUT,
                            (uint8_t *)&g_UsbDeviceHidGeneric.buffer[g_UsbDeviceHidGeneric.bufferIndex][0],
                            USB_HID_GENERIC_OUT_BUFFER_LENGTH);
                    }
                }
            }
            break;
        case kUSB_DeviceEventGetConfiguration:
            if (param)
            {
                /* Get current configuration request */
                *temp8 = g_UsbDeviceHidGeneric.currentConfiguration;
                error = kStatus_USB_Success;
            }
            break;
        case kUSB_DeviceEventGetInterface:
            if (param)
            {
                /* Get current alternate setting of the interface request */
                uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                if (interface < USB_HID_GENERIC_INTERFACE_COUNT)
                {
                    *temp16 = (*temp16 & 0xFF00U) | g_UsbDeviceHidGeneric.currentInterfaceAlternateSetting[interface];
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
    USB_DeviceEhciIsrFunction(g_UsbDeviceHidGeneric.deviceHandle);
}
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 1U)
#if defined(FSL_FEATURE_SOC_USBNC_COUNT) && (FSL_FEATURE_SOC_USBNC_COUNT > 1U)
void USB1_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_UsbDeviceHidGeneric.deviceHandle);
}
#endif
#endif
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
void USB0_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(g_UsbDeviceHidGeneric.deviceHandle);
}
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
void USB0_IRQHandler(void)
{
    USB_DeviceLpcIp3511IsrFunction(g_UsbDeviceHidGeneric.deviceHandle);
}
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
void USB1_IRQHandler(void)
{
    USB_DeviceLpcIp3511IsrFunction(g_UsbDeviceHidGeneric.deviceHandle);
}
#endif

static void USB_DeviceApplicationInit(void)
{
    uint8_t irqNumber;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    uint8_t usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 1U)
    if (CONTROLLER_ID == kUSB_ControllerEhci0)
    {
        CLOCK_EnableUsbhs0PhyPllClock(USB_HS_PHY_CLK_SRC, USB_HS_PHY_CLK_FREQ);
        CLOCK_EnableUsbhs0Clock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
    }
    else
    {
        CLOCK_EnableUsbhs1PhyPllClock(USB_HS_PHY_CLK_SRC, USB_HS_PHY_CLK_FREQ);
        CLOCK_EnableUsbhs1Clock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
    }
#else
    CLOCK_EnableUsbhs0PhyPllClock(USB_HS_PHY_CLK_SRC, USB_HS_PHY_CLK_FREQ);
    CLOCK_EnableUsbhs0Clock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
#endif

    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    uint8_t usbDeviceKhciIrq[] = USB_IRQS;
    irqNumber = usbDeviceKhciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];

    SystemCoreClockUpdate();

    CLOCK_EnableUsbfs0Clock(USB_FS_CLK_SRC, USB_FS_CLK_FREQ);
#endif

#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
    uint8_t usbDeviceIP3511Irq[] = USB_IRQS;
    irqNumber = usbDeviceIP3511Irq[CONTROLLER_ID - kUSB_ControllerLpcIp3511Fs0];

    /* enable USB IP clock */
    CLOCK_EnableUsbfs0DeviceClock(USB_FS_CLK_SRC, USB_FS_CLK_FREQ);
#endif

#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
    uint8_t usbDeviceIP3511Irq[] = USBHSD_IRQS;
    irqNumber = usbDeviceIP3511Irq[CONTROLLER_ID - kUSB_ControllerLpcIp3511Hs0];
    /* enable USB IP clock */
    CLOCK_EnableUsbhs0DeviceClock(USB_HS_CLK_SRC, USB_HS_CLK_FREQ);
#endif

#if (((defined(USB_DEVICE_CONFIG_LPCIP3511FS)) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)) || \
     ((defined(USB_DEVICE_CONFIG_LPCIP3511HS)) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)))
#if defined(FSL_FEATURE_USBHSD_USB_RAM) && (FSL_FEATURE_USBHSD_USB_RAM)
    for (int i = 0; i < FSL_FEATURE_USBHSD_USB_RAM; i++)
    {
        ((uint8_t *)FSL_FEATURE_USBHSD_USB_RAM_BASE_ADDRESS)[i] = 0x00U;
    }
#endif
#endif

#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
    SYSMPU_Enable(SYSMPU, 0);
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

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

    /* Set HID generic to default state */
    g_UsbDeviceHidGeneric.speed = USB_SPEED_FULL;
    g_UsbDeviceHidGeneric.attach = 0U;
    g_UsbDeviceHidGeneric.hidHandle = (class_handle_t)NULL;
    g_UsbDeviceHidGeneric.deviceHandle = NULL;
    g_UsbDeviceHidGeneric.buffer[0] = (uint8_t *)&s_GenericBuffer0[0];
    g_UsbDeviceHidGeneric.buffer[1] = (uint8_t *)&s_GenericBuffer1[0];

    /* Initialize the usb stack and class drivers */
    if (kStatus_USB_Success !=
        USB_DeviceClassInit(CONTROLLER_ID, &g_UsbDeviceHidConfigList, &g_UsbDeviceHidGeneric.deviceHandle))
    {
        usb_echo("USB device HID generic failed\r\n");
        return;
    }
    else
    {
        usb_echo("USB device HID generic demo\r\n");
        /* Get the HID mouse class handle */
        g_UsbDeviceHidGeneric.hidHandle = g_UsbDeviceHidConfigList.config->classHandle;
    }

/* Install isr, set priority, and enable IRQ. */
#if defined(__GIC_PRIO_BITS)
    GIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
#else
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
#endif
    EnableIRQ((IRQn_Type)irqNumber);

    /* Start USB device HID generic */
    USB_DeviceRun(g_UsbDeviceHidGeneric.deviceHandle);
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

    USB_DeviceApplicationInit();

    while (1U)
    {
#if USB_DEVICE_CONFIG_USE_TASK
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
        USB_DeviceEhciTaskFunction(g_UsbDeviceHidGeneric.deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
        USB_DeviceKhciTaskFunction(g_UsbDeviceHidGeneric.deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
        USB_DeviceLpcIp3511TaskFunction(g_UsbDeviceHidGeneric.deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
        USB_DeviceLpcIp3511TaskFunction(g_UsbDeviceHidGeneric.deviceHandle);
#endif
#endif
    }
}
